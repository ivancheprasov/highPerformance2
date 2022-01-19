#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "high_life.h"
#include "mpi.h"

int main(int argc, char **argv) {
    int8_t time = -1;
    char *src_path = "";
    char *dst_path = "";
    static struct option long_options[] = {
            {"src",  required_argument, 0, 's'},
            {"dst",  required_argument, 0, 'd'},
            {"time", required_argument, 0, 't'},
            {"mpi",  no_argument,       0, 'm'},
            {0, 0,                      0, 0}
    };
    int long_index = 0;
    int result = 0;
    bool is_parallel = false;
    while ((result = getopt_long(argc, argv, "d:s:t:", long_options, &long_index)) != -1) {
        switch (result) {
            case 's':
                src_path = optarg;
                break;
            case 'd':
                dst_path = optarg;
                break;
            case 't':
                time = (int8_t) strtol(optarg, NULL, 10);
                break;
            case 'm':
                is_parallel = true;
                break;
            default:
                break;
        }
    }
    if (time == -1 || strlen(src_path) == 0 || strlen(dst_path) == 0) {
        puts("Missing required arguments\n"
             "--src or -s <src_file>\n"
             "--dst or -d <dst_dir>\n"
             "--current_time or -t <step_count>");
        return EXIT_FAILURE;
    }
    field *high_life = init_field(src_path);
    if (high_life == NULL) {
        puts("Failed to parse src field");
        return EXIT_FAILURE;
    }
    if (is_parallel) {
        int rank, process_number;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &process_number);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank > process_number) {
            puts("Unable to split the field");
            return EXIT_FAILURE;
        }
        partition_info *current_partition_info = get_field_partition_info(high_life, rank, process_number);
        compute_using_mpi(high_life, current_partition_info, time, dst_path);
        free(current_partition_info);
        MPI_Finalize();
    } else {
        compute(high_life, time, dst_path);
    }
    return EXIT_SUCCESS;
}

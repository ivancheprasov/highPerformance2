#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "high_life.h"

int main(int argc, char **argv) {
    int8_t time = -1;
    char *src_path = "";
    char *dst_path = "";
    static struct option long_options[] = {
            {"src",  required_argument, 0, 's'},
            {"dst",  required_argument, 0, 'd'},
            {"time", required_argument, 0, 't'},
            {0, 0,                      0, 0}
    };
    int long_index = 0;
    int result = 0;
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
            default:
                break;
        }
    }
    if (time == -1 || strlen(src_path) == 0 || strlen(dst_path) == 0) {
        puts("Missing required arguments\n"
             "--src or -s <src_file>\n"
             "--dst or -d <dst_dir>\n"
             "--current_time or -t <step_count>");
        return 1;
    }
    field *high_life = init_field(src_path);
    if (high_life == NULL) {
        puts("Failed to parse src field");
        return 1;
    }
    compute(high_life, time, dst_path);
    return 0;
}

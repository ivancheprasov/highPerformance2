//
// Created by subhuman on 19.01.2022.
//

#include <memory.h>
#include <malloc.h>
#include <mpi.h>
#include "mpi.h"
#include "file.h"
#include "const.h"

void compute_using_mpi(field *high_life, partition_info *current_partition_info, int time, char *dst_path) {
    int rank = current_partition_info->rank;
    int process_number = current_partition_info->process_number;
    size_t start_row = current_partition_info->start_row;
    size_t row_size = current_partition_info->row_size;
    bool **partition_field = get_field_partition(high_life, current_partition_info);
    bool **next_part = compute_partition_next_step(
            high_life,
            current_partition_info,
            dst_path,
            high_life->data[(start_row + (high_life->row_number - 1)) % high_life->row_number],
            high_life->data[(start_row + row_size) % high_life->row_number],
            1,
            partition_field
    );
    gather_result(high_life, dst_path, next_part, current_partition_info, 1);
    for (int i = 2; i <= time; ++i) {
        size_t rank_above = (rank + process_number - 1) % process_number;
        size_t rank_below = (rank + 1) % process_number;
        send_part(next_part[0], high_life->column_number, (int) rank_above, true);
        send_part(next_part[row_size - 1], high_life->column_number, (int) rank_below, false);
        bool row_above[high_life->column_number];
        bool row_below[high_life->column_number];
        receive_part(row_below, high_life->column_number, (int) rank_below, true);
        receive_part(row_above, high_life->column_number, (int) rank_above, false);
        next_part = compute_partition_next_step(
                high_life,
                current_partition_info,
                dst_path,
                row_above,
                row_below,
                i,
                next_part
        );
        gather_result(high_life, dst_path, next_part, current_partition_info, i);
    }
};

static void gather_result(field *field, char *dest_dir, bool **next_part, partition_info *info, int cycle) {
    if (info->rank == 0) {
        for (int i = 0; i < info->row_size; ++i) {
            memcpy(field->data[i], next_part[i], field->column_number * sizeof(bool));
        }
        partition_info *current_partition_info;
        for (int i = 1; i < info->process_number; ++i) {
            current_partition_info = get_field_partition_info(field, i, info->process_number);
            receive_result(field, current_partition_info);
            free(current_partition_info);
        }
        log_state(field->data, dest_dir, cycle, field->is_transposed, field->row_number, field->column_number, -1, 0);
    } else {
        send_result(next_part, info->row_size, field->column_number);
    }
}

static void receive_part(bool *buffer, size_t columns, int src_rank, bool is_sender_above) {
    MPI_Status status;
    int tag = is_sender_above ? PART_ABOVE_TAG : PART_BELOW_TAG;
    MPI_Recv(buffer, (int) columns, MPI_C_BOOL, src_rank, tag, MPI_COMM_WORLD, &status);
}

static void receive_result(field *field, partition_info *info) {
    bool buf[info->row_size * field->column_number];
    MPI_Status status;
    MPI_Recv(buf, (int) (info->row_size * field->column_number), MPI_C_BOOL, info->rank, RESULT_TAG, MPI_COMM_WORLD, &status);
    for (int i = 0; i < info->row_size; ++i) {
        memcpy(field->data[info->start_row + i], &buf[i * field->column_number], field->column_number * sizeof(bool));
    }
}

static void send_part(bool *row, size_t columns, int dest_rank, bool is_sender_above) {
    MPI_Request request;
    int tag = is_sender_above ? PART_ABOVE_TAG : PART_BELOW_TAG;
    MPI_Isend(row, (int) columns, MPI_C_BOOL, dest_rank, tag, MPI_COMM_WORLD, &request);
}

static void send_result(bool **result, size_t rows, size_t columns) {
    bool buf[rows * columns];
    MPI_Request request;
    for (int i = 0; i < rows; ++i) {
        memcpy(&buf[i * columns], result[i], columns * sizeof(bool));
    }
    MPI_Isend(buf, (int) (rows * columns), MPI_C_BOOL, 0, RESULT_TAG, MPI_COMM_WORLD, &request);
}
//
// Created by subhuman on 19.01.2022.
//

#ifndef HIGHPERFORMANCE2_MPI_H
#define HIGHPERFORMANCE2_MPI_H

#include "high_life.h"

void compute_using_mpi(field *high_life, partition_info *current_partition_info, int time, char *dst_path);

static void gather_result(field *field, char *dest_dir, bool **next_part, partition_info *current_partition_info, int cycle);

static void receive_part(bool *buffer, size_t columns, int src_rank, bool is_sender_above);

static void receive_result(field *field, partition_info *info);

static void send_part(bool *row, size_t columns, int dest_rank, bool is_sender_above);

static void send_result(bool **result, size_t rows, size_t columns);

#endif //HIGHPERFORMANCE2_MPI_H

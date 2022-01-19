//
// Created by subhuman on 16.01.2022.
//

#ifndef HIGHPERFORMANCE2_HIGH_LIFE_H
#define HIGHPERFORMANCE2_HIGH_LIFE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    size_t row_number;
    size_t column_number;
    bool is_transposed;
    bool **data;
} field;

typedef struct {
    size_t start_row;
    size_t row_size;
    int rank;
    int process_number;
} partition_info;

field *init_field(const char *src_path);

bool **alloc_data_array(size_t row_number, size_t column_number);

void free_data_array(bool **data, size_t row_number);

bool **init_bordered_data(bool **data, uint16_t rows, uint16_t columns);

bool **get_bordered_data(field *field);

void compute(field *field, size_t time, char *dst_path);

bool **compute_next_step(bool **data, size_t row_number, size_t column_number);

bool **compute_partition_next_step(
        field *field, partition_info *current_partition_info, const char *dest_dir, bool *row_above,
        bool *row_below, int cycle, bool **partition_field
);

partition_info *get_field_partition_info(field *field, int part_num, int total_parts);

bool **get_field_partition(field *field, partition_info *current_partition_info);

static void divide_partitions(size_t *array, int array_size, size_t row_number);

static int count_living(bool **data, size_t row_index, size_t column_index);

static void transpose(field *field);

static void fill_partition_borders(bool **bordered_part, size_t row_num, size_t row_len, bool *row_above, bool *row_below);

#endif //HIGHPERFORMANCE2_HIGH_LIFE_H

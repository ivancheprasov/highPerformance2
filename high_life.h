//
// Created by subhuman on 16.01.2022.
//

#ifndef HIGHPERFORMANCE2_HIGH_LIFE_H
#define HIGHPERFORMANCE2_HIGH_LIFE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t current_time;
    size_t row_number;
    size_t column_number;
    bool is_transposed;
    bool **data;
} field;

field *init_field(const char *src_path);

bool **alloc_data_array(size_t row_number, size_t column_number);

void free_data_array(bool **data, size_t row_number);

bool **init_bordered_data(bool **data, uint16_t rows, uint16_t columns);

bool **get_bordered_data(field *field);

void compute(field *field, size_t time, char *dst_path);

bool **compute_next_step(bool **data, size_t row_number, size_t column_number);

static int count_living(bool **data, size_t row_index, size_t column_index);

static void transpose(field *field);

#endif //HIGHPERFORMANCE2_HIGH_LIFE_H

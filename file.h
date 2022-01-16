//
// Created by subhuman on 16.01.2022.
//

#include <stdbool.h>

#ifndef HIGHPERFORMANCE2_FILE_H

bool **read_bool_array(const char *file_path, size_t *row_number, size_t *column_number);

void log_state(const bool **data, const char *dst_path, size_t current_iteration, bool is_transposed, size_t row_number, size_t column_number);

void print_raw(const bool **data, bool is_transposed, size_t src_row_number, size_t src_column_number, FILE *out, size_t row_offset);

#define HIGHPERFORMANCE2_FILE_H

#endif //HIGHPERFORMANCE2_FILE_H

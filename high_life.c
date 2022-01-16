//
// Created by subhuman on 16.01.2022.
//

#include <stddef.h>
#include <malloc.h>
#include <memory.h>
#include "high_life.h"
#include "file.h"

field *init_field(const char *src_path) {
    size_t row_number;
    size_t column_number;
    bool **data = read_bool_array(src_path, &row_number, &column_number);
    if (data == NULL) {
        return NULL;
    }
    field *result = calloc(1, sizeof(field));
    result->data = data;
    result->row_number = row_number;
    result->column_number = column_number;
    result->is_transposed = row_number < column_number;
    if (result->is_transposed) {
        transpose(result);
    }
    return result;
}

bool **alloc_data_array(size_t row_number, size_t column_number) {
    bool **result = calloc(row_number, sizeof(bool *));
    for (int i = 0; i < row_number; ++i) {
        result[i] = calloc(column_number, sizeof(bool));
    }
    return result;
}

void free_data_array(bool **data, size_t row_number) {
    for (int i = 0; i < row_number; ++i) {
        free(data[i]);
    }
    free(data);
}

bool **init_bordered_data(bool **data, uint16_t rows, uint16_t columns) {
    bool **result = alloc_data_array(rows + 2, columns + 2);
    for (int i = 0; i < rows; ++i) {
        memcpy(result[i + 1] + sizeof(bool), data[i], columns * sizeof(bool));
    }
    return result;
}

bool **get_bordered_data(field *field) {
    size_t row_number = field->row_number;
    size_t column_number = field->column_number;
    bool **bordered = init_bordered_data(field->data, row_number, column_number);
    memcpy(bordered[0] + sizeof(bool), bordered[row_number] + sizeof(bool), column_number);
    memcpy(bordered[row_number + 1] + sizeof(bool), bordered[1] + sizeof(bool), column_number);
    for (int i = 1; i < row_number + 1; ++i) {
        bordered[i][0] = bordered[i][column_number];
    }
    for (int i = 1; i < row_number + 1; ++i) {
        bordered[i][column_number + 1] = bordered[i][1];
    }
    bordered[0][0] = bordered[row_number][column_number];
    bordered[0][column_number + 1] = bordered[row_number][1];
    bordered[row_number + 1][0] = bordered[1][column_number];
    bordered[row_number + 1][column_number + 1] = bordered[1][1];
    return bordered;
}

void compute(field *field, size_t time, char *dst_path) {
    size_t row_number = field->row_number;
    size_t column_number = field->column_number;
    for (int current_iteration = 0; current_iteration < time; ++current_iteration) {
        bool **bordered_data = get_bordered_data(field);
        bool **next_step = compute_next_step(bordered_data, row_number + 2, column_number + 2);
        log_state((const bool **) next_step, dst_path, current_iteration + 1, field->is_transposed, row_number, column_number);
        free_data_array(bordered_data, row_number + 2);
        free_data_array(field->data, row_number);
        field->data = next_step;
    }
}

bool **compute_next_step(bool **data, size_t row_number, size_t column_number) {
    bool **result = alloc_data_array(row_number, column_number);
    for (int i = 0; i < row_number - 2; ++i) {
        for (int j = 0; j < column_number - 2; ++j) {
            int living = count_living(data, i + 1, j + 1);
            if (data[i + 1][j + 1]) {
                if (living != 2 && living != 3) {
                    result[i][j] = false;
                } else {
                    result[i][j] = true;
                }
            } else {
                if (living == 3 || living == 6) {
                    result[i][j] = true;
                } else {
                    result[i][j] = false;
                }
            }
        }
    }
    return result;
}

static int count_living(bool **data, size_t row_index, size_t column_index) {
    int living = 0;
    for (int k = -1; k < 2; ++k) {
        for (int l = -1; l < 2; ++l) {
            if (k != 0 || l != 0) {
                if (data[row_index + k][column_index + l]) {
                    living++;
                }
            }
        }
    }
    return living;
}

static void transpose(field *field) {
    size_t transposed_row_number = field->column_number;
    size_t transposed_column_number = field->row_number;
    bool **transposed_data = alloc_data_array(transposed_row_number, transposed_column_number);
    for (int i = 0; i < transposed_column_number; ++i) {
        for (int j = 0; j < transposed_row_number; ++j) {
            transposed_data[j][i] = field->data[i][j];
        }
    }
    free_data_array(field->data, transposed_column_number);
    field->data = transposed_data;
    field->row_number = transposed_row_number;
    field->column_number = transposed_column_number;
}
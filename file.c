//
// Created by subhuman on 16.01.2022.
//

#include <bits/types/FILE.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "file.h"
#include "const.h"
#include "utils.h"

bool **read_bool_array(const char *file_path, size_t *row_number, size_t *column_number) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        return NULL;
    }
    char buffer[BUFFER_SIZE];
    fread(buffer, sizeof(char), BUFFER_SIZE, file);
    fclose(file);
    char *current_line = strtok(buffer, "\n");
    if (current_line == NULL) {
        return NULL;
    }
    bool *ptr_array[BUFFER_SIZE];
    size_t line_size = strlen(current_line);
    size_t line_index = 0;
    bool *result_row;
    while (current_line != NULL) {
        result_row = calloc(line_size, sizeof(bool));
        for (int i = 0; i < line_size; ++i) {
            result_row[i] = parse_bool(current_line[i]);
        }
        current_line = strtok(NULL, "\n");
        ptr_array[line_index] = result_row;
        line_index++;
    }
    bool **data = calloc(line_index, sizeof(bool *));
    memcpy(data, ptr_array, line_index * sizeof(bool *));
    *row_number = line_index;
    *column_number = line_size;
    return data;
}

void log_state(bool **data, const char *dst_path, const size_t current_iteration, const bool is_transposed,
               const size_t row_number, const size_t column_number, int rank, size_t row_offset) {
    char filename[300] = {0};
    if (rank == -1) {
        sprintf(filename, "%s/result_%d.txt", dst_path, (int) current_iteration);
    } else {
        sprintf(filename, "%s/result_%d_pid_%d.txt", dst_path, (int) current_iteration, rank);
    }
    FILE *out = fopen(filename, "w+");
    print_raw(data, is_transposed, row_number, column_number, out, row_offset);
    fclose(out);
}

void print_raw(bool **data, const bool is_transposed, const size_t src_row_number, const size_t src_column_number,
               FILE *out, const size_t row_offset) {
    char buf[BUFFER_SIZE] = {0};
    size_t number = 0;
    size_t row_number = is_transposed ? src_column_number : src_row_number;
    size_t column_number = is_transposed ? src_row_number : src_column_number;
    for (int i = 0; i < row_offset; ++i) {
        buf[number++] = '\n';
    }
    for (int i = 0; i < row_number; ++i) {
        for (int j = 0; j < column_number; ++j) {
            if (is_transposed) {
                buf[number++] = data[j][i] ? '1' : '0';
            } else {
                buf[number++] = data[i][j] ? '1' : '0';
            }
        }
        buf[number++] = '\n';
    }
    fputs(buf, out);
}
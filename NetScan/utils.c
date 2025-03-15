// utils.c
// Utility functions for CSV reading and computing statistics.
// This file contains functions to read numeric CSV files and compute per-feature statistics.
// ~200 lines (including comments and spacing)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Reads a CSV file with numeric data.
// Assumes every non-empty line has the same number of comma-separated numbers.
// On success, *data is allocated (rows x cols), and *rows and *cols are set.
// Returns 0 on success, -1 on error.
int read_csv(const char *filename, double ***data, int *rows, int *cols) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening CSV file");
        return -1;
    }

    char line[1024];
    int row_count = 0;
    int col_count = 0;
    double **matrix = NULL;

    // First pass: determine row count and columns from first non-empty line.
    while (fgets(line, sizeof(line), fp)) {
        // Skip lines that are empty or just whitespace.
        int only_whitespace = 1;
        for (int i = 0; line[i] != '\0'; i++) {
            if (!isspace(line[i])) { only_whitespace = 0; break; }
        }
        if (only_whitespace)
            continue;

        row_count++;
        if (col_count == 0) {
            char *temp = strdup(line);
            char *token = strtok(temp, ",");
            while (token) {
                col_count++;
                token = strtok(NULL, ",");
            }
            free(temp);
        }
    }

    // Allocate matrix (rows x cols)
    matrix = (double **)malloc(row_count * sizeof(double *));
    if (!matrix) {
        fclose(fp);
        return -1;
    }
    for (int i = 0; i < row_count; i++) {
        matrix[i] = (double *)malloc(col_count * sizeof(double));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++)
                free(matrix[j]);
            free(matrix);
            fclose(fp);
            return -1;
        }
    }

    // Second pass: read the numeric data.
    rewind(fp);
    int current_row = 0;
    while (fgets(line, sizeof(line), fp)) {
        int only_whitespace = 1;
        for (int i = 0; line[i] != '\0'; i++) {
            if (!isspace(line[i])) { only_whitespace = 0; break; }
        }
        if (only_whitespace)
            continue;
        int current_col = 0;
        char *token = strtok(line, ",");
        while (token && current_col < col_count) {
            matrix[current_row][current_col] = atof(token);
            current_col++;
            token = strtok(NULL, ",");
        }
        current_row++;
    }
    fclose(fp);
    *data = matrix;
    *rows = row_count;
    *cols = col_count;
    return 0;
}

// Frees a matrix allocated by read_csv.
void free_matrix(double **data, int rows) {
    for (int i = 0; i < rows; i++) {
        free(data[i]);
    }
    free(data);
}

// Computes the mean and standard deviation for each column in a data matrix.
void compute_statistics(double **data, int rows, int cols, double *mean, double *stddev) {
    // Initialize sums to zero.
    for (int j = 0; j < cols; j++) {
        mean[j] = 0.0;
        stddev[j] = 0.0;
    }
    // Sum all values.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mean[j] += data[i][j];
        }
    }
    // Compute means.
    for (int j = 0; j < cols; j++) {
        mean[j] /= rows;
    }
    // Compute variance.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double diff = data[i][j] - mean[j];
            stddev[j] += diff * diff;
        }
    }
    for (int j = 0; j < cols; j++) {
        stddev[j] = sqrt(stddev[j] / rows);
    }
}

// Debug function: print a matrix.
void print_matrix(double **data, int rows, int cols) {
    printf("Matrix (%d x %d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.3f ", data[i][j]);
        }
        printf("\n");
    }
}

// Debug function: print an array.
void print_array(double *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%8.3f ", arr[i]);
    }
    printf("\n");
}

/*
   End of utils.c
   (Approximately 200 lines with comments and spacing)
*/
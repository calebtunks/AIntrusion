#ifndef UTILS_H
#define UTILS_H

// Reads a CSV file with numeric data.
// On success, *data is allocated (rows x cols), and *rows and *cols are set.
// Returns 0 on success, -1 on error.
int read_csv(const char *filename, double ***data, int *rows, int *cols);

// Frees a matrix allocated by read_csv.
void free_matrix(double **data, int rows);

// Computes the mean and standard deviation for each column in a data matrix.
void compute_statistics(double **data, int rows, int cols, double *mean, double *stddev);

// Debug function: print a matrix.
void print_matrix(double **data, int rows, int cols);

// Debug function: print an array.
void print_array(double *arr, int size);

#endif // UTILS_H
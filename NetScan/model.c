// model.c
// Model training and packet classification.
// Trains a model using two CSV files (one safe, one malicious) by computing the mean feature vector for each.
// A new packet is classified based on which mean it is closer to (Euclidean distance).
// ~200 lines (including comments and spacing)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Prototypes from utils.c (assumed to be compiled separately)
int read_csv(const char *filename, double ***data, int *rows, int *cols);
void free_matrix(double **data, int rows);
void compute_statistics(double **data, int rows, int cols, double *mean, double *stddev);
void print_array(double *arr, int size);

// Define the Model structure.
typedef struct {
    int num_features;      // Number of features per packet.
    double *safe_mean;     // Mean vector for safe dataset.
    double *bad_mean;      // Mean vector for malicious dataset.
} Model;

// Compute Euclidean distance between two vectors of length n.
double euclidean_distance(double *v1, double *v2, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = v1[i] - v2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

// Trains a model using safe and bad CSV datasets.
// Computes the mean vector for each dataset.
int train_model(const char *safe_csv, const char *bad_csv, Model *model) {
    double **safe_data = NULL, **bad_data = NULL;
    int safe_rows, safe_cols, bad_rows, bad_cols;

    if (read_csv(safe_csv, &safe_data, &safe_rows, &safe_cols) != 0) {
        fprintf(stderr, "Error reading safe dataset\n");
        return -1;
    }
    if (read_csv(bad_csv, &bad_data, &bad_rows, &bad_cols) != 0) {
        fprintf(stderr, "Error reading bad dataset\n");
        free_matrix(safe_data, safe_rows);
        return -1;
    }
    if (safe_cols != bad_cols) {
        fprintf(stderr, "Datasets have different number of features\n");
        free_matrix(safe_data, safe_rows);
        free_matrix(bad_data, bad_rows);
        return -1;
    }

    model->num_features = safe_cols;
    model->safe_mean = (double *)malloc(safe_cols * sizeof(double));
    model->bad_mean = (double *)malloc(safe_cols * sizeof(double));
    if (!model->safe_mean || !model->bad_mean) {
        fprintf(stderr, "Memory allocation error in model training\n");
        free_matrix(safe_data, safe_rows);
        free_matrix(bad_data, bad_rows);
        return -1;
    }
    // Temporary arrays for standard deviation (not used for classification here)
    double *safe_std = (double *)malloc(safe_cols * sizeof(double));
    double *bad_std = (double *)malloc(safe_cols * sizeof(double));
    if (!safe_std || !bad_std) {
        fprintf(stderr, "Memory allocation error for stddev\n");
        free_matrix(safe_data, safe_rows);
        free_matrix(bad_data, bad_rows);
        free(model->safe_mean);
        free(model->bad_mean);
        return -1;
    }
    
    // Compute statistics for both datasets.
    compute_statistics(safe_data, safe_rows, safe_cols, model->safe_mean, safe_std);
    compute_statistics(bad_data, bad_rows, bad_cols, model->bad_mean, bad_std);
    
    // For debugging, you might print the mean vectors:
    // printf("Safe mean:\n"); print_array(model->safe_mean, safe_cols);
    // printf("Bad mean:\n"); print_array(model->bad_mean, safe_cols);
    
    free(safe_std);
    free(bad_std);
    free_matrix(safe_data, safe_rows);
    free_matrix(bad_data, bad_rows);
    
    return 0;
}

// Classifies a new packet based on its feature vector.
// Returns 1 if safe, 0 if malicious.
int classify_packet(Model *model, double *features) {
    double dist_safe = euclidean_distance(features, model->safe_mean, model->num_features);
    double dist_bad = euclidean_distance(features, model->bad_mean, model->num_features);
    // Debug: Uncomment to see distances.
    // printf("Distance to safe: %.3f, Distance to bad: %.3f\n", dist_safe, dist_bad);
    return (dist_safe < dist_bad) ? 1 : 0;
}

// Frees memory allocated in the model.
void free_model(Model *model) {
    if (model->safe_mean) free(model->safe_mean);
    if (model->bad_mean) free(model->bad_mean);
}

/*
   End of model.c
   (Approximately 200 lines including comments and spacing)
*/
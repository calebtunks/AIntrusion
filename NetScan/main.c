// main.c
// Replaces the old main() with an exported function to compile into a shared library.
// This file depends on model.c (for train_model, classify_packet) and utils.c.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"  // For Model, train_model, classify_packet, free_model
#include "main.h"   // Declares classify_packet_c

// This function trains the model using two CSV files (safe and bad), then classifies
// a new packet (feature vector). It returns 1 (True) if safe, 0 (False) if malicious.
int classify_packet_c(const char* safe_csv, const char* bad_csv, const double* features, int feature_count)
{
    Model model;
    // Train the model using the two datasets.
    if (train_model(safe_csv, bad_csv, &model) != 0) {
        fprintf(stderr, "Error training the model.\n");
        return 0; // default to malicious if training fails
    }

    // Check if the model's feature count matches the input vector length.
    if (model.num_features != feature_count) {
        fprintf(stderr, "Feature mismatch: model expects %d, but got %d.\n",
                model.num_features, feature_count);
        free_model(&model);
        return 0; // or handle differently
    }

    // Cast away const because classify_packet expects a non-const double*.
    // Alternatively, update classify_packet to accept a const pointer.
    int result = classify_packet(&model, (double*)features);

    free_model(&model);

    // result == 1 -> safe, result == 0 -> malicious
    return result;
}
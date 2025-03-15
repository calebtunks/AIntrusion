#ifndef MODEL_H
#define MODEL_H

// Model structure
typedef struct {
    int num_features;      // Number of features per packet.
    double *safe_mean;     // Mean vector for safe dataset.
    double *bad_mean;      // Mean vector for malicious dataset.
} Model;

// Trains a model using two CSV datasets (safe and bad).
// Returns 0 on success, nonzero on error.
int train_model(const char *safe_csv, const char *bad_csv, Model *model);

// Classifies a packet given its feature vector.
// Returns 1 if safe, 0 if malicious.
int classify_packet(Model *model, double *features);

// Frees memory allocated in the model.
void free_model(Model *model);

#endif // MODEL_H
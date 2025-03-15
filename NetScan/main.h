#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

// This function returns 1 (True/Safe) or 0 (False/Malicious).
// safe_csv: path to CSV of "safe" training data
// bad_csv: path to CSV of "bad" (malicious) training data
// features: an array of doubles representing the new packet's feature vector
// feature_count: number of features in that vector
int classify_packet_c(const char* safe_csv, const char* bad_csv, const double* features, int feature_count);

#ifdef __cplusplus
}
#endif

#endif // MAIN_H
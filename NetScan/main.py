import os
import ctypes
import random

###############################################################
# CSV Generation Functions
###############################################################

def generate_csv(filename, num_rows, num_features, base_values, noise):
    """
    Generates a CSV file with num_rows rows and num_features columns.
    Each value is generated as: base_value ± random noise.
    """
    with open(filename, "w") as f:
        for _ in range(num_rows):
            row = []
            for i in range(num_features):
                # Generate a value close to base_values[i] with noise in range [-noise, noise]
                value = base_values[i] + random.uniform(-noise, noise)
                row.append(f"{value:.3f}")
            f.write(",".join(row) + "\n")
    print(f"Generated {filename} with {num_rows} rows and {num_features} features.")

def ensure_csv_files():
    """
    Ensures that the safe and bad training CSV files exist.
    If not, they are generated with 200 rows of 7 features each.
    """
    num_rows = 200
    num_features = 7
    safe_file = "safe_packets.csv"
    bad_file = "bad_packets.csv"
    
    if not os.path.exists(safe_file):
        safe_base = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0]  # "safe" mean vector
        noise = 0.2
        generate_csv(safe_file, num_rows, num_features, safe_base, noise)
    else:
        print(f"{safe_file} already exists.")
        
    if not os.path.exists(bad_file):
        bad_base = [10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0]  # "bad" mean vector
        noise = 0.2
        generate_csv(bad_file, num_rows, num_features, bad_base, noise)
    else:
        print(f"{bad_file} already exists.")

###############################################################
# Shared Library Loading and Classification Function
###############################################################

def load_library():
    """
    Loads the compiled shared library (libintrusion.so) using ctypes.
    Adjust the path if necessary.
    """
    lib_path = os.path.abspath("libintrusion.so")
    if not os.path.exists(lib_path):
        raise FileNotFoundError(f"Shared library {lib_path} not found.")
    lib = ctypes.CDLL(lib_path)
    # Define the function signature for:
    # int classify_packet_c(const char* safe_csv, const char* bad_csv, const double* features, int feature_count);
    lib.classify_packet_c.argtypes = [ctypes.c_char_p, ctypes.c_char_p,
                                      ctypes.POINTER(ctypes.c_double),
                                      ctypes.c_int]
    lib.classify_packet_c.restype = ctypes.c_int
    return lib

def classify_packet(lib, safe_csv, bad_csv, features):
    """
    Calls the C function classify_packet_c from the shared library.
    features: a list of float values representing the new packet's features.
    Returns True if the packet is classified as safe; otherwise, False.
    """
    feature_count = len(features)
    arr_type = ctypes.c_double * feature_count
    features_arr = arr_type(*features)
    safe_csv_bytes = safe_csv.encode('utf-8')
    bad_csv_bytes = bad_csv.encode('utf-8')
    result = lib.classify_packet_c(safe_csv_bytes, bad_csv_bytes, features_arr, feature_count)
    return result == 1

###############################################################
# Main Function
###############################################################

def main():
    # Step 1: Ensure training CSV files exist.
    ensure_csv_files()
    
    # Step 2: Load the shared library (libintrusion.so).
    lib = load_library()
    
    # Step 3: Define a new packet's feature vector.
    # (Choose a vector near the safe base to test for safe, or near the bad base to test for malicious.)
    # For example, this is close to the safe base:
    new_packet_features = [1.05, 2.05, 3.00, 4.10, 5.00, 6.05, 7.00]
    # Uncomment the following line to simulate a malicious packet:
    # new_packet_features = [9.95, 9.00, 8.10, 7.05, 6.00, 5.10, 4.00]
    
    safe_csv = "safe_packets.csv"
    bad_csv = "bad_packets.csv"
    
    # Step 4: Classify the new packet.
    is_safe = classify_packet(lib, safe_csv, bad_csv, new_packet_features)
    if is_safe:
        print("Packet is SAFE (True)")
    else:
        print("Packet is MALICIOUS (False)")

if __name__ == "__main__":
    main()
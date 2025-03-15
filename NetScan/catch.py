from pyshark import LiveCapture


def packet():
    
    #print("Scapy version: ", scapy.__version__)
    
    capture = LiveCapture(interface='Wi-Fi',output_file='output.txt')  # Remove encryption_type if not needed
    
    print("Starting packet capture...")
    # Capture 1 packet with a timeout of 10 seconds (so it won't wait indefinitely)
    capture.sniff(packet_count=1, timeout=10)
    
    #print(capture._packets[0])

    return capture._packets[0]
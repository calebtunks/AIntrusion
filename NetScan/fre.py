import platform
import subprocess

def block_ip(ip):
    system = platform.system()
    
    if system == "Linux":
        # Option 1: Using iptables on Linux to block incoming traffic from ip
        # (Alternatively, you could use UFW if that's your preferred firewall.)
        cmd = ["sudo", "iptables", "-A", "INPUT", "-s", ip, "-j", "DROP"]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode == 0:
            print(f"Successfully blocked {ip} on Linux using iptables.")
        else:
            print(f"Error blocking IP on Linux: {result.stderr}")
    
    elif system == "Windows":
        # Use Windows Firewall via netsh.
        # This command adds a firewall rule named "BlockIP" to block incoming traffic from ip.
        cmd = ["netsh", "advfirewall", "firewall", "add", "rule", 
               "name=BlockIP", "dir=in", "action=block", f"remoteip={ip}"]
        # shell=True is sometimes required on Windows for netsh commands.
        result = subprocess.run(cmd, capture_output=True, text=True, shell=True)
        if result.returncode == 0:
            print(f"Successfully blocked {ip} on Windows using Windows Firewall.")
        else:
            print(f"Error blocking IP on Windows: {result.stderr}")
    
    elif system == "Darwin":
        # On macOS, we can use pfctl to add an IP to a table.
        # IMPORTANT: This example assumes that you already have a PF table named "blockedips" defined in /etc/pf.conf,
        # and that PF is enabled. In /etc/pf.conf, you would need a rule such as:
        #   block drop from <blockedips> to any
        cmd = ["sudo", "pfctl", "-t", "blockedips", "-T", "add", ip]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode == 0:
            print(f"Successfully blocked {ip} on macOS using PF (pfctl).")
        else:
            print(f"Error blocking IP on macOS: {result.stderr}")
    
    else:
        print(f"Unsupported OS: {system}")
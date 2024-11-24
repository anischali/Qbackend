import socket
import struct
import random
import time
import ipaddress
import hmac
import hashlib
import base64
import os

# Constants
STUN_SERVER = "localhost"#stun.l.google.com"  # Example public STUN server
STUN_PORT = 3478
MAGIC_COOKIE = 0x2112A442  # Fixed magic cookie as per RFC 5389
TRANSACTION_ID_LENGTH = 12
HEADER_FORMAT = "!HHI12s"  # STUN Header: Type (16), Length (16), Magic Cookie (32), Transaction ID (96)
BINDING_REQUEST = 0x0001  # Binding Request Message Type

class credential(object):
    def __init__(self, _username, _realm, _password, _nonce):
        self.username = _username
        self.realm = _realm
        self.password = _password
        self.nonce = _nonce
        
        self.key = hashlib.md5(f"{self.username}:{self.realm}:{self.password}".encode()).digest()

credentials = {}

# Generate random transaction ID
def generate_transaction_id():
    return bytes(random.randint(0, 255) for _ in range(TRANSACTION_ID_LENGTH))

# Create a STUN Binding Request
def create_binding_request():
    transaction_id = generate_transaction_id()
    message_type = BINDING_REQUEST
    message_length = 0  # No attributes in this minimal request
    header = struct.pack(HEADER_FORMAT, message_type, message_length, MAGIC_COOKIE, transaction_id)
    return header, transaction_id

# Generate MESSAGE-INTEGRITY attribute
def generate_message_integrity(key, message):
    return hmac.new(key, message, hashlib.sha1).digest()

# Function to handle long-term authentication
def authenticated_binding_request(sock, stun_server, stun_port, username, password):
    
    # Initial unauthenticated request
    request, transaction_id = create_binding_request()
    sock.sendto(request, (stun_server, stun_port))

    response, _ = sock.recvfrom(1024)
    header = response[:20]
    _, _, _, transaction_id = struct.unpack(HEADER_FORMAT, header)
    attributes = response[20:]

    # Check for ERROR-CODE and NONCE
    for offset in range(0, len(attributes), 4):
        attr_type, attr_length = struct.unpack("!HH", attributes[offset:offset+4])
        offset += 4
        if attr_type == 0x0009:  # NONCE attribute
            nonce_attr = attributes[offset:offset + attr_length]
        if attr_type == 0x0006:  # USERNAME attribute
            username_attr = attributes[offset:offset + attr_length]
            username = username_attr.split(':')[0]
        if attr_type == 0x0014:
            realm_attr = attributes[offset:offset+attr_length]
        

    if credentials.get(username) == None:
        credentials[username] = credential(username, realm_attr, password, nonce_attr)    
    
    
    cred = credentials.get(username)
    new_request, transaction_id = create_binding_request()
    new_request += struct.pack("!HH", 0x0006, len(username)) + username.encode()  # USERNAME attribute
    new_request += struct.pack("!HH", 0x0009, len(cred.nonce)) + cred.nonce.encode()  # NONCE attribute
    
    # Add MESSAGE-INTEGRITY
    print("new request: {}".format(new_request))
    message_integrity = generate_message_integrity(cred.key, new_request)
    print("message_integrity: {}".format(message_integrity))
    new_request += struct.pack("!HH", 0x0008, len(message_integrity)) + message_integrity

    # Send authenticated request
    sock.sendto(new_request, (stun_server, stun_port))
    response, _ = sock.recvfrom(1024)
    print("Authenticated Response Received")


# Parse STUN Response
def parse_response(response):
    header = response[:20]  # First 20 bytes: STUN header
    _, _, magic_cookie, transaction_id = struct.unpack(HEADER_FORMAT, header)

    if magic_cookie != MAGIC_COOKIE:
        raise ValueError("Invalid Magic Cookie in response")

    # Parse MAPPED-ADDRESS attribute
    offset = 20  # Skip the header
    while offset < len(response):
        attr_type, attr_length = struct.unpack("!HH", response[offset:offset+4])
        offset += 4

        if attr_type == 0x020:  # MAPPED-ADDRESS
            _, family, port, ip = struct.unpack("!BBHi", response[offset:offset+8])
            ip ^= magic_cookie
            port ^= (0x2112)
            ip_address = str(ipaddress.ip_address(ip))
            return ip_address, port
        offset += attr_length

    raise ValueError("MAPPED-ADDRESS attribute not found")


# Function to maintain NAT mapping
def keep_alive(sock, stun_server, stun_port):
    request, transaction_id = create_binding_request()  # Generate STUN Binding Request
    interval = 15  # Default interval in seconds (adjust based on your NAT type)

    while True:
        try:
            # Send Binding Request
            sock.sendto(request, (stun_server, stun_port))
            
            # Wait for response
            response, _ = sock.recvfrom(1024)
            public_ip, public_port = parse_response(response)
            print(f"Keep-Alive: Public IP: {public_ip}, Public Port: {public_port}")

        except socket.timeout:
            print("Keep-Alive failed: STUN server did not respond")
        
        # Sleep for the keep-alive interval
        time.sleep(interval)

# Main Function
def main():
    # Create UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    sock.settimeout(2)  # 2-second timeout

    # Prepare STUN Binding Request
    request, transaction_id = authenticated_binding_request(sock, STUN_SERVER, STUN_PORT, "user1", "password1")

    # Send request to STUN server
    sock.sendto(request, (STUN_SERVER, STUN_PORT))

    try:
        # Receive response
        response, _ = sock.recvfrom(1024)
        public_ip, public_port = parse_response(response)
        print(f"Public IP: {public_ip}, Public Port: {public_port}")
    except socket.timeout:
        print("STUN server did not respond")
    finally:
        
        while(True):
            keep_alive(sock, STUN_SERVER, STUN_PORT)
        
        sock.close()

if __name__ == "__main__":
    main()

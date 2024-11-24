import socket
import struct
import hmac
import hashlib
import random
import time
import base64
import os

# Constants
MAGIC_COOKIE = 0x2112A442
HEADER_FORMAT = "!HHI12s"  # STUN header
BINDING_RESPONSE = 0x0101
BINDING_ERROR_RESPONSE = 0x0111
STUN_PORT = 3478  # Default STUN server port


class credential(object):
    def __init__(self, _username, _realm, _password, _nonce):
        self.username = _username
        self.realm = _realm
        self.password = _password
        self.nonce = _nonce
        
        self.key = hashlib.md5(f"{self.username}:{self.realm}:{self.password}".encode()).digest()


credentials = {}

SECRET_KEY = bytes(os.urandom(8))

def generate_nonce(secret_key):
    timestamp = int(time.time())
    random_bytes = os.urandom(8)  # 8 bytes of randomness
    nonce_data = f"{timestamp}:{base64.b64encode(random_bytes).decode()}"
    
    # Optionally sign the NONCE for integrity
    signature = hmac.new(secret_key, nonce_data.encode(), hashlib.sha256).hexdigest()
    return f"{nonce_data}:{signature}"


# Validate NONCE (check timestamp freshness and integrity)
def validate_nonce(nonce, secret_key, timeout=300):
    try:
        nonce_parts = nonce.split(":")
        timestamp = int(nonce_parts[0])
        received_signature = nonce_parts[2]
        
        # Recalculate signature
        nonce_data = f"{nonce_parts[0]}:{nonce_parts[1]}"
        expected_signature = hmac.new(secret_key.encode(), nonce_data.encode(), hashlib.sha256).hexdigest()
        
        # Check integrity and freshness
        if not hmac.compare_digest(received_signature, expected_signature):
            raise ValueError("Invalid NONCE signature")
        if time.time() - timestamp > timeout:
            raise ValueError("NONCE expired")
        
        return True
    except Exception as e:
        print(f"NONCE validation error: {e}")
        return False

# Generate random transaction ID
def generate_transaction_id():
    return bytes(random.randint(0, 255) for _ in range(12))

# Parse incoming STUN message
def parse_request(data):
    header = data[:20]
    message_type, message_length, magic_cookie, transaction_id = struct.unpack(HEADER_FORMAT, header)
    if magic_cookie != MAGIC_COOKIE:
        raise ValueError("Invalid Magic Cookie")
    return message_type, transaction_id, data[20:]

# Add attributes to STUN response
def add_attribute(attr_type, value):
    attr_length = len(value)
    return struct.pack("!HH", attr_type, attr_length) + value

# Generate MESSAGE-INTEGRITY
def generate_message_integrity(key, message):
    return hmac.new(key, message, hashlib.sha1).digest()

# Handle Binding Request
def handle_binding_request(client_address, transaction_id, attributes, sock):
    username = None
    password = None
    received_nonce = None
    username_attr = ""
    integrity_valid = False
    has_integrety = False
    key = None

    # Parse attributes
    offset = 0
    while offset < len(attributes):
        attr_type, attr_length = struct.unpack("!HH", attributes[offset:offset + 4])
        offset += 4
        value = attributes[offset:offset + attr_length]
        offset += attr_length

        if attr_type == 0x0006:  # USERNAME
            username_attr = value.decode()
            username = username_attr.split(':')[0]
            password = username_attr.split(':')[1]
        elif attr_type == 0x0009:  # NONCE
            received_nonce = value
        elif attr_type == 0x0008:  # MESSAGE-INTEGRITY
            has_integrety = True
            if credentials.get(username) == None:
                integrity_valid = False
            else:
                cred = credentials.get(username)
                original_message = attributes[:offset - (4 + attr_length)]  # Without integrity attribute
                expected_integrity = generate_message_integrity(cred.key, original_message)
                integrity_valid = hmac.compare_digest(value, expected_integrity)
            

    if not username or len(attributes) <= 0:
        # Unauthenticated response (401 Unauthorized)
        nonce = generate_nonce(SECRET_KEY)
        response = struct.pack(HEADER_FORMAT, BINDING_ERROR_RESPONSE, 0, MAGIC_COOKIE, transaction_id)
        response += add_attribute(0x0009, bytes(nonce, encoding='utf-8'))
        response += add_attribute(0x0014, bytes("stun.localhost.home", encoding="utf-8"))
        
        sock.sendto(response, client_address)

    if username and not integrity_valid:
        if has_integrety:
            print("Invalid MESSAGE-INTEGRITY")
            return
        else:
            credentials[username] =  credential(username, "stun.localhost.home", password , received_nonce)       

    cred = credentials.get(username)
    if username and received_nonce == cred.nonce and integrity_valid:
        # Authenticated Binding Response
        response = struct.pack(HEADER_FORMAT, BINDING_RESPONSE, 0, MAGIC_COOKIE, transaction_id)
        mapped_address = client_address[0]
        port = client_address[1]
        mapped_address_attr = add_attribute(0x0001, struct.pack("!BBH4s", 0, 1, port, socket.inet_aton(mapped_address)))
        response += mapped_address_attr
        message_integrity = generate_message_integrity(key, response)
        response += add_attribute(0x0008, message_integrity)
        sock.sendto(response, client_address)

# Main STUN server function
def stun_server():
    
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("0.0.0.0", STUN_PORT))
    print(f"STUN server listening on port {STUN_PORT}...")

    while True:
        data, client_address = sock.recvfrom(1024)
        try:
            message_type, transaction_id, attributes = parse_request(data)
            if message_type == 0x0001:  # Binding Request
                handle_binding_request(client_address, transaction_id, attributes, sock)
        except Exception as e:
            print(f"Error handling request: {e}")


if __name__ == "__main__":
    stun_server()

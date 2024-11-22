import socket
import sys
import random

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)

s.bind(("", random.randint(40000, 60000)))

s.sendto(b'hello world!', (sys.argv[1], int(sys.argv[2])))
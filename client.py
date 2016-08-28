import socket

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

#test = input()

s.connect(('localhost',1234))

data = s.recv(1024)

print(data)

text = input()

s.send(text)

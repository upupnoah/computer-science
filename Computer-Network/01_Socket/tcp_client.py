from socket import *

server_name = 'localhost'
server_port = 12001

while True:
    client_socket = socket(AF_INET, SOCK_STREAM)
    try:
        client_socket.connect((server_name, server_port))
        message = input('Input lowercase sentence:')
        if message == 'exit':
            break
        client_socket.send(message.encode())
        modified_sentence = client_socket.recv(1024)
        print('From Server: ', modified_sentence.decode())
    finally:
        client_socket.close()

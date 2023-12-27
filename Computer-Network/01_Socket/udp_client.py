from socket import *

server_name = 'localhost'
server_port = 12000

client_socket = socket(AF_INET, SOCK_DGRAM)

while True:

    message = input('Input lowercase sentence:')
    if message == 'exit':
        break

    # 源地址会被操作系统自动分配, 我们只需要指定目的地址(目的主机的IP地址和端口号)
    # 如果是主机名, 会自动 DNS lookup 转换为 IP 地址
    client_socket.sendto(message.encode(), (server_name, server_port))

    modified_message, server_address = client_socket.recvfrom(2048)

    print(modified_message.decode())

client_socket.close()

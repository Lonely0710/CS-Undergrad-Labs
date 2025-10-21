import socket
import os
import mimetypes

def handle_request(client_socket):
    """处理 HTTP 请求"""
    try:
        request_data = b''
        while True:
            chunk = client_socket.recv(1024)
            if not chunk:
                break
            request_data += chunk
            if b'\r\n\r\n' in request_data:
                break

        request_data_str = request_data.decode('utf-8', errors='ignore')
        print('请求：\n', request_data_str)  # 打印完整请求

        if not request_data_str:
            client_socket.close()
            return

        # 解析 HTTP 请求
        lines = request_data_str.split('\r\n')
        request_line = lines[0]
        method, path, _ = request_line.split(' ')

        # 处理请求的文件路径
        if path == '/':
            filename = 'index.html'
        else:
            filename = path[1:]  # 去掉路径开头的斜杠

        # 获取文件内容和 MIME 类型
        try:
            with open(filename, 'rb') as f:
                file_content = f.read()
            mime_type, _ = mimetypes.guess_type(filename)
            if mime_type is None:
                mime_type = 'application/octet-stream'  # 默认 MIME 类型
            if 'text' in mime_type:
                mime_type += '; charset=utf-8' #文本文件设置编码

            response_line = 'HTTP/1.1 200 OK\r\n'
            response_headers = f'Content-Type: {mime_type}\r\n\r\n'
        except FileNotFoundError:
            file_content = b'<h1>404 Not Found</h1>'
            response_line = 'HTTP/1.1 404 Not Found\r\n'
            response_headers = 'Content-Type: text/html; charset=utf-8\r\n\r\n'

        # 构建 HTTP 响应
        response = response_line.encode('utf-8') + response_headers.encode('utf-8') + file_content

        # 发送响应
        client_socket.send(response)
        print('响应：\n', response.decode('utf-8', errors='ignore')) #打印响应

    except OSError as e:
        print(f'套接字错误：{e}')  # 打印套接字错误
    finally:
        client_socket.close()  # 确保套接字关闭

def main():
    """主函数"""
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 8080))  # 设置服务器 IP 和端口
    server_socket.listen(1)

    print('服务器已启动，监听端口 8080...')

    while True:
        client_socket, client_address = server_socket.accept()
        print(f'客户端 {client_address} 已连接')
        handle_request(client_socket)

if __name__ == '__main__':
    main()
# File transfer from client to server

2 console applications Client.exe and Server.exe under Windows, exchanging files via UDP socket with TCP confirmation.

## 1. The server. Server.exe
The first parameter is the IP, the second is the port number, and the third is the directory for storing files.
The server starts listening to the port by IP address and waits for the client to connect via TCP.
```
Example:
Server.exe 127.0.0.1 5555 temp
```
## 2. The client. Client.exe
Start with 5 parameters.

1 and 2 parameters - IP address and port to connect to the server. The third is the port for sending UDP packets. The fourth is the path to the file. The fifth is a timeout for confirming UDP
packets in milliseconds.
```
Example
Client.exe 127.0.0.1 5555 6000 test.txt 500
```
## 3. Server and client interaction.
At startup, the server opens a TCP socket with an IP address and a port from the start parameters (parameters 1 and 2), takes it to listen and waits for connections from the client.

At the start, the client tries to connect to the server via TCP to the IP address and port from the start parameters (parameters 1 and 2) until a connection is established. After
the connection is established, the client loads a file into memory (parameter 4, file size no more than 10 MB.) and sends the file name and UDP port to the server over a TCP connection.
Next, the client begins to send the file in udp datagram blocks and receive TCP acknowledgements of reception by the server.
If no packet confirmation was received during the timeout (parameter 5), the udp packet is sent again. When all packets are confirmed, the client notifies the TCP server about the end of the file transfer, closes the TCP connection, and shuts down.

After establishing a TCP client connection, receiving the file name and the udp port, the server opens a udp socket and begins receiving udp packets originating from
the client's IP address and the port transmitted by the client.
For each udp packet received, the server sends a confirmation via a tcp socket to the client. The received packets are stored in memory. After receiving
a notification from the client about the end of the transfer (all packets are confirmed on the client), the server saves the file to a directory (parameter 3).
After disconnecting the client, the server should continue working and wait for another connection.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

# Передача файла от клиента к серверу

2 консольных приложения Client.exe и Server.exe под Windows, обменивающихся файлами через UDP сокет с подтверждением по TCP.

## 1.Сервер. Server.exe
Первый параметр - IP, второй номер порта, третий - каталог для хранения файлов.
Сервер начинает прослушивать по IP адресу порт и ждет подключения клиента по TCP.
 ```
Пример:
Server.exe 127.0.0.1 5555 temp
```
## 2. Клиент. Client.exe
Старт с 5 параметрами.
 
1 и 2 параметры - IP адрес и порт для подключения к серверу. Третий - порт для отправки UDP пакетов. Четвертый - путь к файлу. Пятый - тайм аут на подтверждение UDP 
пакетов в милисекундах.
```
Пример
Client.exe 127.0.0.1 5555 6000 test.txt 500
```
## 3.Взаимодействие сервера и клиента.
При старте сервер открывает TCP сокет с IP адресом и портом из стартовых параметров(1 и 2 параметры), берет его на прослушивание и ожидает подключений от клиента.
 
Клиент при старте пытается подключиться к серверу по TCP на IP адрес и порт из стартовых параметров (1 и 2 параметры ) пока не будет установлено соединение. После 
установления соединения клиент загружает в память файл (4 параметр, размер файла не более 10 Мб.) и отправляет по TCP соединению имя файл и порт UDP на сервер.
Далее клиент начинает отправлять файл блоками  udp датаграммами и получать по TCP подтверждения о приеме сервером. 
В случае если в течении таймаута (5 параметр) не было получено подтверждение на пакет, то пакет по udp отправляется повторно. В случае когда все пакеты подтверждены клиент уведомляет сервер TCP об окончании передачи файла, закрывает TCP соединение, завершает работу.
 
Сервер после установления подключения клиента по ТСP, получения имя файла и порта udp открывает udp сокет и начинает принимать udp пакеты исходящие с IP адреса 
клиента и порта переданного клиентом.
На каждый полученный udp пакет сервер отправляет подтверждение через tcp сокет на клиент. Полученные пакеты хранятся в памяти. После получения от клиента 
уведомления об окончании передачи (все пакеты подтверждены на клиенте) сервер сохраняет файл в каталог (3 параметр).
После отключения клиента сервер должен продолжать работу и ждать еще подключения.

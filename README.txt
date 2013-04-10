158-C-client-server
===================

Client/server program in C in both UDP and TCP.

At a command prompt, enter:
gcc client-udp.c -o client-udp.exe
gcc server-udp.c -o server-udp.exe
gcc client-tcp.c -o client-tcp.exe
gcc multi-server-tcp.c -o multi-server-tcp.exe

The UDP programs do not require any parameters at command prompt.
./client-udp.exe
./server-udp.exe

The TCP programs require parameters:
./client-tcp.exe 192.168.1.1 9930 100 1024
where 192.168.1.1 is the ip and 9930 is the port to use and 100 is the number of packets and 1024 is the packet size.

./multi-server-tcp.exe 9930
where 9930 is the port.

Both servers require a force quit with control C.

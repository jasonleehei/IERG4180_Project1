## IERG4180_Project1


SID: 1155095162
Name: Lee Cheung Hei

<h3 align="center">NetProbe</h3>

## About The Project
In this project, the students will develop a simple C program for network testing and benchmarking. Key
concepts applied in this project include: sockets programming basics, blocking I/O, socket configuration
and control, simple protocol design and implementation. The NetProbe is a very useful tool for
diagnosing network problems, as well a good tool for benchmarking performance of various
programming models. 

`jasonleehei`, `IERG4180_Project1`, `1155095162@gmail.com`, `NetProbe`

## Getting Started
To get a local copy up and running follow these simple steps.

### Installation
1. Clone the repo
   ```sh
   git clone https://github.com/jasonleehei/IERG4180_Project1
   ```

## Usage

Please choose NetProbe's mode!
SEND MODE: -send
RECEIEVE MODE: -recv
HOST MODE: -host
If[mode] = -send then the following are the supported parameters :
           < -stat yyy >         update statistics once every yyy ms. (Default = 500 ms)
           < -rhost hostname > send data to host specified by hostname. (Default 'localhost')
           < -rport portnum > send data to remote host at port number portnum. (Default '4180')
           < -proto tcp || udp > send data using TCP or UDP. (Default UDP)
           < -pktsize bsize > send message of bsize bytes. (Default 1000 bytes)
           < -pktrate txrate > send data at a data rate of txrate bytes per second,
                               0 means as fast as possible. (Default 1000 bytes / second
           < -pktnum num > send or receive a total of num messages. (Default = infinite)
           < -sbufsize bsize > set the outgoing socket buffer size to bsize bytes.
else if[mode] = -recv then
           < -stat yyy >         update statistics once every yyy ms. (Default = 500 ms)
           < -lhost hostname > hostname to bind to. (Default late binding)
           < -lport portnum > port number to bind to. (Default '4180')
           < -proto tcp || udp > send data using TCP or UDP. (Default UDP)
           < -pktsize bsize > send message of bsize bytes. (Default 1000 bytes)
           < -rbufsize bsize > set the incoming socket buffer size to bsize bytes.
else if[mode] = -host then           <hostname>     resolve DNS records for hostname(Default 'localhost').
end if.




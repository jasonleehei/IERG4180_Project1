#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <mswsock.h>
#include<errno.h>
#include "getopt.h"

#pragma comment(lib, "Ws2_32.lib")

#define SERVER "127.0.0.1"
#define BUFLEN 65536
#define PORT 4180	//The port on which to send data

/* DEFAULT define of send & recv */
#define DEFAULT_UPDATE 500                 
#define DEFAULT_PROTO "UDP"
#define DEFAULT_BSIZE 1000

/* DEFAULT define of send */
#define DEFAULT_HOST_NAME "localhost"
#define DEFAULT_REMOTE_PORT_NUMBER 4180
#define DEFAULT_TXRATE 1000
#define DEFAULT_TOTAL_NUM_MESSAGES 0
#define DEFAULT_SEND_BUFFER 65536

/* DEFAULT define of recv */
#define DEFAULT_BIND_HOSTNAME "IN_ADDR_ANY"
#define DEFAULT_BIND_PORT 4180

int SEND(int argc, char* argv[]);
int RECV(int argc, char* argv[]);
int HOST(char *host);

static struct option mode_options[] =
{   
    {"send", no_argument, 0, 1},
    {"recv", no_argument, 0, 2},
    {"host", required_argument, 0, 3},
};

static struct option send_setting[] =
{
    {"stat", required_argument, 0, 1},
    {"rhost", required_argument, 0, 2},
    {"rport", required_argument, 0, 3},
    {"proto", required_argument, 0, 4},
    {"pktsize", required_argument, 0, 5},
    {"pktrate", required_argument, 0, 6},
    {"pktnum", required_argument, 0, 7},
    {"sbufsize", required_argument, 0, 8},
};

static struct option recv_setting[] =
{
    {"stat", required_argument, 0, 1},
    {"lhost", required_argument, 0, 2},
    {"lport", required_argument, 0, 3},
    {"proto", required_argument, 0, 4},
    {"pktsize", required_argument, 0, 5},
    {"rbufsize", required_argument, 0, 6},
};


int main(int argc, char* argv[]) {

    char *host; /* for host mode usage*/

    /* Select Mode */
    int MODE = getopt_long_only(argc, argv, "", mode_options, 0);
    
    if (MODE != -1)
    {
        switch (MODE)
        {
            case 1:
                SEND(argc, argv);
                break;
            case 2:
                RECV(argc, argv);
                break;
            case 3:
                host = optarg;
                HOST(host);
                break;
            default:
                break;
        }
    }
    else{printf("Please choose NetProbe's mode!\n"
                "SEND MODE: -send\n"
                "RECEIEVE MODE: -recv\n"
                "HOST MODE: -host\n"
                "If[mode] = -send then the following are the supported parameters :\n"
                "           < -stat yyy >         update statistics once every yyy ms. (Default = 500 ms)\n"
                "           < -rhost hostname > send data to host specified by hostname. (Default 'localhost')\n"
                "           < -rport portnum > send data to remote host at port number portnum. (Default '4180')\n"
                "           < -proto tcp || udp > send data using TCP or UDP. (Default UDP)\n"
                "           < -pktsize bsize > send message of bsize bytes. (Default 1000 bytes)\n"
                "           < -pktrate txrate > send data at a data rate of txrate bytes per second,\n"
                "                               0 means as fast as possible. (Default 1000 bytes / second\n"
                "           < -pktnum num > send or receive a total of num messages. (Default = infinite)\n"
                "           < -sbufsize bsize > set the outgoing socket buffer size to bsize bytes.\n"
                "else if[mode] = -recv then\n"
                "           < -stat yyy >         update statistics once every yyy ms. (Default = 500 ms)\n"
                "           < -lhost hostname > hostname to bind to. (Default late binding)\n"
                "           < -lport portnum > port number to bind to. (Default '4180')\n"
                "           < -proto tcp || udp > send data using TCP or UDP. (Default UDP)\n"
                "           < -pktsize bsize > send message of bsize bytes. (Default 1000 bytes)\n"
                "           < -rbufsize bsize > set the incoming socket buffer size to bsize bytes.\n"
                "else if[mode] = -host then"
                "           <hostname>     resolve DNS records for hostname(Default 'localhost').\n"
                "end if."
    );}

    return(0);

}

/* SEND MODE */
int SEND(int argc, char* argv[])
{
    /* Paramters */
    int stat = DEFAULT_UPDATE;
    char* rhost = SERVER;
    char* proto = DEFAULT_PROTO;
    int rport = DEFAULT_REMOTE_PORT_NUMBER;
    int pktsize = DEFAULT_BSIZE;
    int pktrate = DEFAULT_TXRATE;
    int pktnum = DEFAULT_TOTAL_NUM_MESSAGES;
    int sbufsize = 0;
    int parameters;

    /* getopt of parameters in mode */
    while ((parameters = getopt_long_only(argc, argv, "", send_setting, 0)) != -1)
    {
        switch (parameters)
        {
        case 1:
            stat = atoi(optarg);
            break;
        case 2:
            rhost = optarg;
            break;
        case 3:
            rport = atoi(optarg);
            break;
        case 4:
            proto = optarg;
            break;
        case 5:
            pktsize = atoi(optarg);
            break;
        case 6:
            pktrate = atoi(optarg);
            break;
        case 7:
            pktnum = atoi(optarg);
            break;
        case 8:
            sbufsize = atoi(optarg);
            break;
        default:
            printf("Please choose NetProbe's mode!\n"
                "SEND MODE: -send\n"
                "RECEIEVE MODE: -recv\n"
                "HOST MODE: -host\n"
                "If[mode] = -send then the following are the supported parameters :\n"
                "           < -stat yyy >         update statistics once every yyy ms. (Default = 500 ms)\n"
                "           < -rhost hostname > send data to host specified by hostname. (Default 'localhost')\n"
                "           < -rport portnum > send data to remote host at port number portnum. (Default '4180')\n"
                "           < -proto tcp || udp > send data using TCP or UDP. (Default UDP)\n"
                "           < -pktsize bsize > send message of bsize bytes. (Default 1000 bytes)\n"
                "           < -pktrate txrate > send data at a data rate of txrate bytes per second,\n"
                "                               0 means as fast as possible. (Default 1000 bytes / second\n"
                "           < -pktnum num > send or receive a total of num messages. (Default = infinite)\n"
                "           < -sbufsize bsize > set the outgoing socket buffer size to bsize bytes.\n"
                "else if[mode] = -recv then\n"
                "           < -stat yyy >         update statistics once every yyy ms. (Default = 500 ms)\n"
                "           < -lhost hostname > hostname to bind to. (Default late binding)\n"
                "           < -lport portnum > port number to bind to. (Default '4180')\n"
                "           < -proto tcp || udp > send data using TCP or UDP. (Default UDP)\n"
                "           < -pktsize bsize > send message of bsize bytes. (Default 1000 bytes)\n"
                "           < -rbufsize bsize > set the incoming socket buffer size to bsize bytes.\n"
                "else if[mode] = -host then"
                "           <hostname>     resolve DNS records for hostname(Default 'localhost').\n"
                "end if."
            );
            break;
        }
        
    }

    printf("NetProbe Configurations:\n");
    printf("Mode:SEND Protocol:%s\n", proto);
    printf("-stat = %d\n-pkstize = %d\n-pktrate = %d\n-pktnum = %d\n-sbufsize = %d\n", stat, pktsize, pktrate, pktnum, sbufsize);


    /* Statistics Display Setting */
    clock_t current_clock, previous_clock = clock();
    double  cum_time_cost = 0, coum_bytes_send = 0;
    double total_time = 0;

    /* Initialise winsock */
    SOCKET s;
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {printf("Failed. Error Code : %d", WSAGetLastError());  exit(EXIT_FAILURE);}


    unsigned long numSent = 0;


    /***** SEND by UDP *****/
    if (strcmp(proto, "UDP") == 0 || strcmp(proto, "udp") == 0)
    {
        struct sockaddr_in  si_other;
        char* buf;
        if (sbufsize == 0) { buf = (char*)calloc(sizeof(char), BUFLEN); }
        else { buf = (char*)calloc(sizeof(char), sbufsize); }
        char message[sizeof(int)];
        double total_sent_bit = 0;
        double average = 0;
        double jitter = 0;

        memset(buf, 'a', pktsize);
        memset(message, 0, sizeof(int));

        //Create a socket
        if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
        { printf("Could not create socket : %d", WSAGetLastError()); exit(EXIT_FAILURE); }
        
        //setup address structure
        memset((char*)&si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(PORT);
        si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

        printf("Remote host localhost lookup successful, IP address is 127.0.0.1\n");
        if (sbufsize == 0) { printf("Default send buffer size = 65536 bytes\n"); }
        else { printf("Custom send buffer size = %d", sbufsize); }

        while (pktnum == 0 || (pktnum != 0 && (int)numSent <= pktnum))
        {
            int bytes_sent = 0;
            *((unsigned long*)message) = htonl(numSent);
            sendto(s, message, sizeof(int), 0, (struct sockaddr*)&si_other, sizeof(SOCKADDR));
            
            current_clock = clock();
            cum_time_cost += (double)(current_clock - previous_clock) / CLOCKS_PER_SEC;
            average = (double)(average * (double)numSent + previous_clock - current_clock) / ((double)numSent + 1);
            jitter = (double)(jitter * (double)numSent + abs(previous_clock - current_clock - average)) / ((double)numSent + 1);

            //send package
            if (cum_time_cost > stat) {
                total_time += cum_time_cost;
                printf("\rElapsed:%dms | Rate:%.2fkbps | Jitter:%.2fms ", (int)total_time, (total_sent_bit/8192.0/total_time), jitter);
                cum_time_cost = 0;
                previous_clock = clock();
            }

            //send the package
            while (bytes_sent < pktsize) {

                int sent_bit = sendto(s, buf + bytes_sent, pktsize, 0, (struct sockaddr*)&si_other, sizeof(SOCKADDR));
                
                if (sent_bit > 0) { bytes_sent = bytes_sent + sent_bit; }
                else
                {
                    if (WSAGetLastError() == 10054) { printf("\nDisconnect from the server"); }
                    else { printf("sendto() failed with error code : %d", WSAGetLastError()); }
                    exit(EXIT_FAILURE);
                }
                total_sent_bit += bytes_sent;
            }
            numSent++;
        }

        closesocket(s);
        WSACleanup();
        return 0;
    }


    /***** SEND by TCP *****/
    else if (strcmp(proto, "TCP") == 0 || strcmp(proto, "tcp") == 0)
    {
        struct sockaddr_in server;
        char* buf;
        if (sbufsize == 0) { buf = (char*)calloc(sizeof(char), BUFLEN); }
        else { buf = (char*)calloc(sizeof(char), sbufsize); }
        double total_sent_bit = 0;
        double average = 0;
        double jitter = 0;

        //Create a socket
        if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        { printf("Could not create socket : %d", WSAGetLastError()); return 1; }
        printf("Socket created.\n");

        server.sin_addr.s_addr = inet_addr(SERVER);
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);

        //Connect to remote server
        if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
        { puts("connect error"); return 1; }

        printf("Remote host localhost lookup successful, IP address is 127.0.0.1\n");
        if (sbufsize == 0) { printf("Default send buffer size = 65536 bytes\n"); }
        else { printf("Custom send buffer size = %d", sbufsize); }

        while (pktnum == 0 || (pktnum != 0 && (int)numSent <= pktnum))
        {
            int bytes_sent = 0;

            current_clock = clock();
            cum_time_cost += (double)(current_clock - previous_clock) / CLOCKS_PER_SEC;
            average = (double)(average * (double)numSent + previous_clock - current_clock) / ((double)numSent + 1);
            jitter = (double)(jitter * (double)numSent + abs(previous_clock - current_clock - average)) / ((double)numSent + 1);

            //print status
            if (cum_time_cost > stat) {
                total_time += cum_time_cost;
                printf("\rElapsed:%dms | Rate:%.2fkbps | Jitter:%.2fms ", (int)total_time, (total_sent_bit / 8192.0 / total_time), jitter);
                cum_time_cost = 0;
                previous_clock = clock();
            }

            //send the package
            while (bytes_sent < pktsize) {

                int sent_bit = send(s, buf + bytes_sent, pktsize, 0);
                if (sent_bit > 0) { bytes_sent = bytes_sent + sent_bit; }
                else
                {
                    if (WSAGetLastError() == 10054) { printf("\nDisconnect from the server"); }
                    else { printf("sendto() failed with error code : %d", WSAGetLastError()); }
                    return -1;
                }
                total_sent_bit += bytes_sent;
            }
            numSent++;
        }

        closesocket(s);
        WSACleanup();
        return 0;
    }
}

/* RECV MODE */
int RECV(int argc, char* argv[])
{

    /* parameters of recv */
    int stat = DEFAULT_UPDATE;
    char* lhost = DEFAULT_BIND_HOSTNAME;
    int lport = DEFAULT_BIND_PORT;
    char* proto = DEFAULT_PROTO;
    int pksize = DEFAULT_BSIZE;
    int rbufsize = 0;
        int parameters;

    double total_recv_bit = 0;
    double average = 0;
    double jitter = 0;
    float loss_ratio = 0;
    int stat_recv = 0;

    u_long packRecv = 0;

    /* getopt in recv */
    while ((parameters = getopt_long_only(argc, argv, "", recv_setting, 0)) != -1)
    {
        switch (parameters)
        {
        case 1:
            stat = atoi(optarg);
            break;
        case 2:
            lhost = optarg;
            break;
        case 3:
            lport = atoi(optarg);
            break;
        case 4:
            proto = optarg;
            break;
        case 5:
            pksize = atoi(optarg);
            break;
        case 6:
            rbufsize = atoi(optarg);
            break;
        default:
            printf("Please choose NetProbe's mode!\n"
                "SEND MODE: -send\n"
                "RECEIEVE MODE: -recv\n"
                "HOST MODE: -host\n"
                "If[mode] = -send then the following are the supported parameters :\n"
                "           < -stat yyy >         update statistics once every yyy ms. (Default = 500 ms)\n"
                "           < -rhost hostname > send data to host specified by hostname. (Default 'localhost')\n"
                "           < -rport portnum > send data to remote host at port number portnum. (Default '4180')\n"
                "           < -proto tcp || udp > send data using TCP or UDP. (Default UDP)\n"
                "           < -pktsize bsize > send message of bsize bytes. (Default 1000 bytes)\n"
                "           < -pktrate txrate > send data at a data rate of txrate bytes per second,\n"
                "                               0 means as fast as possible. (Default 1000 bytes / second\n"
                "           < -pktnum num > send or receive a total of num messages. (Default = infinite)\n"
                "           < -sbufsize bsize > set the outgoing socket buffer size to bsize bytes.\n"
                "else if[mode] = -recv then\n"
                "           < -stat yyy >         update statistics once every yyy ms. (Default = 500 ms)\n"
                "           < -lhost hostname > hostname to bind to. (Default late binding)\n"
                "           < -lport portnum > port number to bind to. (Default '4180')\n"
                "           < -proto tcp || udp > send data using TCP or UDP. (Default UDP)\n"
                "           < -pktsize bsize > send message of bsize bytes. (Default 1000 bytes)\n"
                "           < -rbufsize bsize > set the incoming socket buffer size to bsize bytes.\n"
                "else if[mode] = -host then"
                "           <hostname>     resolve DNS records for hostname(Default 'localhost').\n"
                "end if."
            );
            break;
        }
    }
    
    printf("NetProbe Configurations:\n");
    printf("Mode:RECV Protocol:%s\n", proto);
    printf("-stat = %d\n-pkstize = %d\n-rbufsize = %d\n-rhost = %s\n-rport = %d\n", stat, pksize, rbufsize, lhost, lport);
    if (rbufsize == 0) { printf("Default recv buffer size = 65536 bytes\n"); }
    else { printf("Custom recv buffer size = %d", rbufsize); }

    /* For Statistics Display */
    clock_t current_clock, previous_clock = clock();
    double  cum_time_cost = 0, coum_bytes_send = 0;
    double total_time = 0;
    
    //Initialise winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }


    /***** RECV by UDP *****/
    if (strcmp(proto, "UDP") == 0 || strcmp(proto, "udp") == 0)
    {
        SOCKET s;
        struct sockaddr_in server, si_other;
        int slen = sizeof(si_other);
        char* buf;
        if (rbufsize == 0) { buf = (char*)calloc(sizeof(char), BUFLEN); }
        else { buf = (char*)calloc(sizeof(char), rbufsize); }
        
        //Prepare the sockaddr_in structure
        memset((char*)&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = htonl(INADDR_ANY);
        server.sin_port = htons(PORT);

        //Create a socket
        if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
        { printf("Could not create socket : %d", WSAGetLastError()); }

        //Bind
        if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
        {
            printf("Bind failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        printf("Binding local socket to port %d using late binding ... successful.\n", lport);
        printf("Waiting for incoming datagrames ...\n");

        //keep listening for data
        while (1)
        {
            int bytes_recv = 0;
            
            current_clock = clock();
            cum_time_cost += (double)(current_clock - previous_clock) / CLOCKS_PER_SEC;
            average = (double)(average * (double)packRecv + previous_clock - current_clock) / ((double)packRecv + 1);
            jitter = (double)(jitter * (double)packRecv + abs(previous_clock - current_clock - average)) / ((double)packRecv + 1);
            
            //print message
            if (cum_time_cost > stat) {
                total_time += cum_time_cost;
                printf("\rElapsed:%dms | Pkts:%d | Lost:%.2f%% | Rate:%.2fkbps | Jitter:%.2fms ", (int)total_time, stat_recv/stat, loss_ratio, (total_recv_bit / 8192.0 / total_time), jitter);
                cum_time_cost = 0;
                stat_recv = 0;
                previous_clock = clock();
            }


            while (bytes_recv < pksize) {

                int recv_len = recvfrom(s, buf + bytes_recv, pksize, 0, (struct sockaddr*)&si_other, &slen);
                loss_ratio = (((float)ntohl(*((unsigned long*)(buf))) - (float)packRecv) / (float)ntohl(*((unsigned long*)(buf)))) * 100.00;

                if (recv_len == SOCKET_ERROR)
                {
                    printf("recvfrom() failed with error code : %d", WSAGetLastError());
                    exit(EXIT_FAILURE);
                }

                else if (recv_len > 0) {
                    bytes_recv += recv_len;
                    total_recv_bit += recv_len;
                }
            }
            packRecv++;
            stat_recv++;
        }

        closesocket(s);
        WSACleanup();

        return 0;
    }


    /***** RECV by TCP *****/
    else if (strcmp(proto, "TCP") == 0 || strcmp(proto, "tcp") == 0)
    {
        SOCKET s, new_socket;
        struct sockaddr_in server, client;
        int c, slen;
        char* buf;
        if (rbufsize == 0) { buf = (char*)calloc(sizeof(char), BUFLEN); }
        else { buf = (char*)calloc(sizeof(char), rbufsize); }

        slen = sizeof(client);

        //Create a socket
        if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        { printf("Could not create socket : %d", WSAGetLastError()); return 0; }

        //Prepare the sockaddr_in structure
        server.sin_addr.s_addr = inet_addr(SERVER);
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);

        //Bind
        if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
        {
            printf("Bind failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        printf("Binding local socket to port %d using late binding ... successful.\n", lport);

        //Listen to incoming connections
        listen(s, 3);

        //Accept and incoming connection
        puts("Waiting for incoming connections...");

        c = sizeof(struct sockaddr_in);
        new_socket = accept(s, (struct sockaddr*)&client, &c);
        if (new_socket == INVALID_SOCKET)
        {
            printf("accept failed with error code : %d", WSAGetLastError());
        }

        puts("Connection accepted");

        while (1)
        {
            int bytes_recv = 0;

            current_clock = clock();
            cum_time_cost += (double)(current_clock - previous_clock) / CLOCKS_PER_SEC;
            average = (double)(average * (double)packRecv + previous_clock - current_clock) / ((double)packRecv + 1);
            jitter = (double)(jitter * (double)packRecv + abs(previous_clock - current_clock - average)) / ((double)packRecv + 1);

            //print message
            if (cum_time_cost > stat) {
                total_time += cum_time_cost;
                printf("\rElapsed:%dms | Pkts:%d | Rate:%.2fkbps | Jitter:%.2fms ", (int)total_time, stat_recv/stat, (total_recv_bit / 8192.0 / total_time), jitter);
                cum_time_cost = 0;
                stat_recv = 0;
                previous_clock = clock();
            }

            while (bytes_recv < pksize) {

                int recv_len = recv(new_socket, buf + bytes_recv, pksize, MSG_WAITALL);

                if (recv_len == SOCKET_ERROR)
                {
                    if ( WSAGetLastError() == 10054 ) { printf("\nDisconnected from the client"); }
                    else { printf("recvfrom() failed with error code : %d", WSAGetLastError()); }
                    exit(EXIT_FAILURE);
                }
                else if (recv_len > 0) {
                    bytes_recv += recv_len;
                    total_recv_bit += recv_len;
                }
            }
            packRecv++;
            stat_recv++;
        }

        closesocket(s);
        WSACleanup();

        return 0;
    }
    
}

/* HOST MODE*/
int HOST(char *host)
{   
    printf("NetProbe Configurations:\n");
    printf("Mode:HOST \n");

    WSADATA wsaData;
    int iResult;
    INT iRetval;
    DWORD dwRetval;

    int i = 1;

    struct addrinfo* result = NULL;
    struct addrinfo* ptr = NULL;
    struct addrinfo hints;

    struct sockaddr_in* sockaddr_ipv4;
    //    struct sockaddr_in6 *sockaddr_ipv6;
    LPSOCKADDR sockaddr_ip;

    char ipstringbuffer[46];
    DWORD ipbufferlength = 46;
    struct hostent* remoteHost;

    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    dwRetval = getaddrinfo(host, NULL, &hints, &result);
    if (dwRetval != 0) {
        printf("getaddrinfo failed with error: %d\n", dwRetval);
        WSACleanup();
        return 1;
    }

    printf("Hostname: %s\n", host);
    remoteHost = gethostbyname(host);
    printf("Official name: %s\n", remoteHost->h_name);
    printf("Address length: %d\n", remoteHost->h_length);

    printf("Address type: ");
    switch (remoteHost->h_addrtype) {
    case AF_INET:
        printf("AF_INET\n");
        break;
    case AF_NETBIOS:
        printf("AF_NETBIOS\n");
        break;
    default:
        printf(" %d\n", remoteHost->h_addrtype);
        break;
    }

    // Retrieve each address and print out the hex bytes
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    
        printf("Address %d", i++);
        //printf("\tFlags: 0x%x\n", ptr->ai_flags);

        switch (ptr->ai_family) {
        case AF_UNSPEC:
            printf("Unspecified");
            break;
        case AF_INET:
            printf("(IPv4):");
            sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
            printf(" %s",
                inet_ntoa(sockaddr_ipv4->sin_addr));
            break;
        case AF_INET6:
            printf("AF_INET6 (IPv6)");
            sockaddr_ip = (LPSOCKADDR)ptr->ai_addr;
            ipbufferlength = 46;
            iRetval = WSAAddressToString(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL,
                ipstringbuffer, &ipbufferlength);
            if (iRetval)
                printf("WSAAddressToString failed with %u\n", WSAGetLastError());
            else
                printf(" %s", ipstringbuffer);
            break;
        case AF_NETBIOS:
            printf("AF_NETBIOS (NetBIOS)");
            break;
        default:
            printf("Other %ld", ptr->ai_family);
            break;
        }


        switch (ptr->ai_protocol) {
        case 0:
            printf("Unspecified\n");
            break;
        case IPPROTO_TCP:
            printf("(TCP)\n");
            break;
        case IPPROTO_UDP:
            printf("(UDP) \n");
            break;
        default:
            printf("Other %ld\n", ptr->ai_protocol);
            break;
        }
        

    }

    freeaddrinfo(result);
    WSACleanup();

    return 0;
}






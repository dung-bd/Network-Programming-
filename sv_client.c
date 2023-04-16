//Bach Dang Dung_20194253
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    // socket()
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == -1) {
        perror("socket() failed!");
        return 1;
    }

    // server
    char *serverIP = argv[1];
    int portNum = atoi(argv[2]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(serverIP);
    addr.sin_port = htons(portNum);
    
    // connect()
    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) {
        perror("connect() failed!");
        return 1;
    }
    printf("Connect to server successfully!\n");

    char buf[256];
    // receive()
    ret = recv(client, buf, sizeof(buf), 0);
    if(ret == -1) {
        perror("recv() failed!");
        return 1;
    }
    if(ret < sizeof(buf))
        buf[ret] = '\0';

    printf("%s\n", buf);

    // send()
    while(1) {
        printf("Enter message: ");
        fgets(buf, sizeof(buf), stdin);
        int msg_len = strlen(buf);
        int len_n = htonl(msg_len);
        send(client, &len_n, sizeof(len_n), 0);
        send(client, buf, msg_len, 0);

        if(strncmp(buf, "exit", 4) == 0)
            break;
    }
    close(client);
}
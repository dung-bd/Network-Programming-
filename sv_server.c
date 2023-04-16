//Bach Dang Dung_20194253
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    // socket()
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1) {
        perror("socket() failed!");
        return 1;
    }
    // server
    int portNum = atoi(argv[1]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portNum);
    
    // bind()
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind() failed!");
        return 1;
    }

    // listen()
    int backlog = 5;
    if(listen(listener, backlog) == -1) {
        perror("listen() failed!");
        return 1;
    }
    printf("Waiting for clients...\n");

    // accept()
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    while(1) {
        int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if(client == -1) {
            perror("accept() failed!");
            break;
        }
        printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // send()
        char *msg = "Hello client!";
        int ret = send(client, msg, strlen(msg), 0);
        if(ret == -1) {
            perror("send() failed!");
            continue;
        }

        // receive()
        char *logFile = argv[2];
        FILE *fp = fopen(logFile, "a");
        time_t now;
        char *current_time;

        while(1) {
            int len_n;
            ret = recv(client, &len_n, sizeof(len_n), 0);
            time(&now);
            
            strftime(curr_time, sizeof(curr_time), "%Y-%m-%d %H:%M:%S", localtime(&));
            if(ret == -1) {
                perror("recv() failed!");
                break;
            }else if(ret == 0) {
                printf("Connection closed!\n");
                break;
            }

            int len = ntohl(len_n);

            char *buf = malloc(len + 1);
            ret = recv(client, buf, len, 0);
            if(ret == -1) {
                perror("recv() failed!");
                free(buf);
                break;
            }else if(ret == 0) {
                printf("Connection closed!\n");
                free(buf);
                break;
            }

            buf[len] = '\0';
        
            fprintf(fp, "%s %s %s", inet_ntoa(clientAddr.sin_addr), date,buf);
            printf("%d bytes received and saved!\n", len);
            free(buf);
        }
        close(client);
        fclose(fp);
    }
    close(listener);
}
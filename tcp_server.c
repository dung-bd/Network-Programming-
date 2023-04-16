//Bach Dang Dung_20194253
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1) {
        perror("socket() failed");
        return 1;
    }

    // server
    int portNum = atoi(argv[1]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portNum);

    //bind
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind() failed!\n");
        return 1;
    }
    
    //listen
    int backlog = 5;
    if(listen(listener, backlog) == -1) {
        perror("listen() failed!");
        return 1;
    }
    printf("Waiting for clients...\n");
    
    //accept
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int i = 1;

    while(1) {  
        int client = accept(listener, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if(client == -1) {
            perror("accept() failed!");
            return 1;
        }
        printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        char buf[256];
        char *helloFile = argv[2];
        FILE *fp1 = fopen(helloFile, "r");
        if(fp1 == NULL) {
            perror("fopen() failed!");
            return 1;
        }
        // send
        char *msg = fgets(buf, sizeof(buf), fp1);
        int ret = send(client, msg, strlen(msg), 0);
        if(ret == -1) {
            perror("send() failed!");
            continue;
        }
        fclose(fp1);
    
        //receive
        char *logFile = argv[3];
        FILE *fp2 = fopen(logFile, "a");
        
        fprintf(fp2, "Messages from Client %d:\n", i);
        
        while(1) {
            ret = recv(client, buf, sizeof(buf), 0);
            if(ret == -1) {
                perror("recv() failed!");
                break;
            } else if(ret == 0) {
                printf("connection with client %d closed!\n", i);
                break;
            }
            
            if(ret < sizeof(buf))
                buf[ret] = '\0';
            
            fprintf(fp2, "%s", buf);
            printf("%d bytes received and saved!\n", ret);
            
        }
        i++;
        close(client);
        fclose(fp2);
    }
    close(listener);

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void createUDPServer() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int len, n;
    int arr[100];
    int sum = 0;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    len = sizeof(cliaddr);
    n = recvfrom(sockfd, (int *)arr, sizeof(arr), MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    arr[n / sizeof(int)] = '\0';

    for (int i = 0; i < n / sizeof(int); i++) {
        sum += arr[i];
    }

    sendto(sockfd, &sum, sizeof(sum), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    printf("Sum sent to client.\n");

    close(sockfd);
}

int main() {
    createUDPServer();
    return 0;
}

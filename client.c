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

void createUDPClient(char *ip, int port) {
    int sockfd;
    struct sockaddr_in servaddr;
    int size, i;
    int *arr;
    int sum = 0;

    printf("Enter the size of the array: ");
    scanf("%d", &size);
    arr = (int *)malloc(size * sizeof(int));

    printf("Enter %d elements of the array:\n", size);
    for (i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);

    sendto(sockfd, arr, size * sizeof(int), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Array sent to server.\n");

    int len, n;
    len = sizeof(servaddr);
    n = recvfrom(sockfd, &sum, sizeof(sum), MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    printf("Sum received from server: %d\n", sum);

    close(sockfd);
    free(arr);
}

int main() {
    char ip[] = "127.0.0.1";
    int port = PORT;
    
    createUDPClient(ip, port);
    return 0;
}

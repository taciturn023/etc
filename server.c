#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc,char *argv[]){

    if(argc<2){
        fprintf(stderr,"port number not there");
        exit(1);
    }

    int sockfd,newsockfd,portno,n;

    char buffer[255];
    portno=atoi(argv[1]);

    struct sockaddr_in servAddr,cliAddr;

    socklen_t cliLen;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        error("socket error");
    }

    bzero((char *)&servAddr,sizeof(servAddr));

    servAddr.sin_family=AF_INET;
    servAddr.sin_addr.s_addr=INADDR_ANY;
    servAddr.sin_port=htons(portno);

    if(bind(sockfd,(struct sockaddr *)&servAddr,sizeof(servAddr))<0){
        error("bind error");
    }

    listen(sockfd,5);

    cliLen=sizeof(cliAddr);

    newsockfd=accept(sockfd,(struct sockaddr *)&cliAddr,&cliLen);

     if(newsockfd<0){error("accept failed");}

    while(1){
        bzero(buffer,sizeof(buffer));
        n=read(newsockfd,buffer,sizeof(buffer));
        if(n<0){error("read error");}
        printf("client: %s\n",buffer);

        bzero(buffer,sizeof(buffer));
        fgets(buffer,sizeof(buffer),stdin);

        n=write(newsockfd,buffer,sizeof(buffer));
        if(n<0){
            error("write faulded");
        }

        int i=strncmp("Bye",buffer,3);
    }
    close(newsockfd);
    close(sockfd);
}
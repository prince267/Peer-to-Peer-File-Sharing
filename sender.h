#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

#define BUFFERT 512

int create_client_socket (int port, char* ipaddr);

struct sockaddr_in sock_serv;

  struct timeval start, stop, delta;
  int sfd,fd;
  char buf[BUFFERT];
long int m, sz, n, count;
  int l=sizeof(struct sockaddr_in);
  struct stat buffer;


int create_client_socket (int port, char* ipaddr){
  int l;
  int sfd;
  
  sfd = socket(PF_INET,SOCK_STREAM,0);
  if (sfd == -1){
    perror("socket fail");
    return EXIT_FAILURE;
  }
  
  
  l=sizeof(sock_serv);
  bzero(&sock_serv,l);

  
  sock_serv.sin_family=AF_INET;
  sock_serv.sin_port=htons(port);
  if (inet_pton(AF_INET,ipaddr,&sock_serv.sin_addr)==0){
    printf("Invalid IP adress\n");
    return EXIT_FAILURE;
  }
  
  return sfd;
}

int send_file(int sfd, char* fname) {
  if ((fd = open(fname,O_RDONLY))==-1){
          perror("open fail");
          return EXIT_FAILURE;
        }
        
        if (stat(fname,&buffer)==-1){
          perror("stat fail");
          return EXIT_FAILURE;
        }
        else
          sz=buffer.st_size;
        
        
        bzero(&buf,BUFFERT);
        
        if(connect(sfd,(struct sockaddr*)&sock_serv,l)==-1){
          perror("connection error\n");
          exit (3);
        }

        n=read(fd,buf,BUFFERT);
        while(n){
          if(n==-1){
            perror("read action failed");
            return EXIT_FAILURE;
          }
          m=sendto(sfd,buf,n,0,(struct sockaddr*)&sock_serv,l);
          if(m==-1){
            perror("send action error");
            return EXIT_FAILURE;
          }
          count+=m;
          
          bzero(buf,BUFFERT);
          n=read(fd,buf,BUFFERT);
        }
        
        m=sendto(sfd,buf,0,0,(struct sockaddr*)&sock_serv,l);
    

        
        printf("\nNumber of bytes transferred : %ld\n", count);
        printf("Total Size  : %ld \n", sz);
   
        close(sfd);
        return 0;
}

#include "sender.h"
#include <time.h>

#define BUFFERT 512
//#define BACKLOG 1
#define PORT 2312

int file_no = 0;
int create_server_socket (int port);

struct sockaddr_in sock_clt;

  int sfd,fd;
    unsigned int length=sizeof(struct sockaddr_in);
    long int n, m,count=0;
    int nsid, choice;
    ushort clt_port;
    char buffer1[BUFFERT],filename[256];
    char dst[INET_ADDRSTRLEN];
    

int create_server_socket (int port){
  int l;
  int sfd;
  int yes=1;
  
  sfd = socket(AF_INET,SOCK_STREAM,0);
  if (sfd == -1){
    perror("socket fail");
    return EXIT_SUCCESS;
  }
 /* 
  if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) == -1 ) {
    perror("setsockopt error");
    exit(5);
  }
  */
  l=sizeof(sock_clt);
  bzero(&sock_clt,l);
  
  sock_clt.sin_family=AF_INET;
  sock_clt.sin_port=htons(port);
  sock_clt.sin_addr.s_addr=INADDR_ANY;
  
  
  if(bind(sfd,(struct sockaddr*)&sock_clt,l)==-1){
    perror("bind fail");
    return EXIT_FAILURE;
  }
  printf("Listener on port %d \nWaiting...\n", PORT);   
  return sfd;
}

int receive_file(int sfd){
  bzero(buffer1,BUFFERT);
  listen(sfd,1);
  nsid=accept(sfd,(struct sockaddr*)&sock_clt,&length);
  if(nsid == -1){
    perror("accept action failed");
    return EXIT_FAILURE;
  }
  else {
    if(inet_ntop(AF_INET,&sock_clt.sin_addr,dst,INET_ADDRSTRLEN)==NULL){
      perror("socket error");
      exit (4);
    }
    clt_port=ntohs(sock_clt.sin_port);
    printf("Connection made with : %s:%d\n",dst,clt_port);  
    bzero(filename,256);
    sprintf(filename,"received.%d",file_no);
    file_no++;
    printf("Creating the copied output file : %s\n",filename);
    
    if ((fd=open(filename,O_CREAT|O_WRONLY,0600))==-1)
    {
      perror("open fail");
      exit (3);
    }
    bzero(buffer1,BUFFERT);
    
    n=recv(nsid,buffer1,BUFFERT,0);
    while(n) {
      if(n==-1){
        perror("recv fail");
        exit(5);
      }
      if((m=write(fd,buffer1,n))==-1){
        perror("write fail");
        exit (6);
      }
      count=count+m;
      bzero(buffer1,BUFFERT);
      n=recv(nsid,buffer1,BUFFERT,0);
    }
    close(sfd);
    close(fd);
  }
  close(nsid);
  printf("End of the transmission with %s.%d\n",dst,clt_port);
  printf("Total Number of bytes transferred so far: %ld \n",count);
  return EXIT_SUCCESS;
}

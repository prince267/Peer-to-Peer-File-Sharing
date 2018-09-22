#include "receiver.h"

#include<string.h>
int main(){
  char ip[20];
  char fname[100];
  int choice=100;
    while(choice != 0){
        printf("\n1.Receive A File \n2.Send a File \n0.Exit\n\n\nEnter Choice: ");
    scanf("%d",&choice);

    switch(choice) {
      case 1:
        sfd = create_server_socket(PORT);
        receive_file(sfd);
        break;
      case 2:
        printf("\nEnter IP Address of Receiver:");
        scanf("%s",ip);
        printf("Enter name of file to be sent:");
        scanf("%s",fname);
        sfd=create_client_socket(PORT, ip);
        send_file(sfd,fname);
        break;
      default:
        exit(1);
    }
    }
    return 0;
}

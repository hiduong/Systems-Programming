#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>


#define MAX   256
#define PORT 1234

char line[MAX], ans[MAX];
int n;

int main(int argc, char *argv[ ]) 
{ 
    int cfd; 
    struct sockaddr_in saddr; 
  
    printf("1. create a TCP socket\n");
    cfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (cfd < 0) { 
        printf("socket creation failed\n"); 
        exit(1); 
    }

    printf("2. fill in [localhost IP port=1234] as server address\n");
    bzero(&saddr, sizeof(saddr)); 
    saddr.sin_family = AF_INET; 
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    saddr.sin_port = htons(PORT); 
  
    printf("3. connect client socket to server\n");
    if (connect(cfd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0) { 
        printf("connection to server failed\n"); 
        exit(2); 
    }
    
    printf("********  processing loop  *********\n");
    while(1){
       printf("input a line : ");
       bzero(line, MAX);                // zero out line[ ]
       fgets(line, MAX, stdin);         // get a line (end with \n) from stdin

       line[strlen(line)-1] = 0;        // kill \n at end
       if (line[0]==0)                  // exit if NULL line
          exit(0);

       // Send ENTIRE line to server
       n = write(cfd, line, MAX);
       printf("client: wrote n=%d bytes; line=(%s)\n", n, line);

       // Read a line from sock and show it
       n = read(cfd, ans, MAX);
       printf("client: read  n=%d bytes; Sum=(%s)\n",n, ans);
    }
} 

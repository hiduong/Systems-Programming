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
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

//name: Hien Duong
//partner: Jimmy Zheng

#define MAX   256
#define PORT 1234

char line[MAX], ans[MAX];
char cmd[16],pathname[128];
char c_pathname[128];
int n, m = 0;
char cwd[128];
char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";


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


  printf("Menu:\n");
  printf("client commands: lcat, lls, lcd, lpwd, lmkdir, lrmdir, lrm\n");
  printf("server commands: get, put, ls, cd, pwd, mkdir, rmdir, rm\n");
    
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
    memset(pathname,0,sizeof(pathname));
    memset(cmd,0,sizeof(cmd));
     memset(c_pathname,0,sizeof(c_pathname));
    
    sscanf(line,"%s %s", cmd, pathname);
    strcpy(c_pathname,pathname);

    if(strcmp(cmd,"quit")==0){

      exit(1);
    }
    else if(strcmp(cmd,"get")==0){
      n = read(cfd,ans,MAX);
      int l;

      l = atoi(ans);
      char line5[l];
      FILE *mp = fopen(pathname,"w");

      n = read(cfd,line5,l);
      fwrite(line5,1,l,mp);

      printf("client recieved %d bytes from server\n",l);
      printf("get - ok\n");

      fclose(mp);
    }
    else if(strcmp(cmd,"put")==0){
      struct stat fstat, *sp;
	char size[MAX];
	int y, q, s_total = 0;
	 sp = &fstat;
	 y = lstat(pathname,&fstat);
	 sprintf(&size[0],"%d",sp->st_size);
	 write(cfd,size,MAX);
	 printf("client sending : %d bytes to server\n",sp->st_size);

	 FILE *gp = fopen(pathname,"r");
	 char line5[sp->st_size];
	 while(q = fread(line5,1,sp->st_size,gp)){
	 
	 }
	 write(cfd,line5,sp->st_size);
    }
    else if(strcmp(cmd,"ls")==0){
      
      printf("client: read ls result\n");
      printf("------ls result------\n\n");
       n = read(cfd,ans,MAX);
       printf("%s\n",ans);
       while(strcmp(ans,"ls - ok") != 0){
	  n = read(cfd,ans,MAX);
       printf("%s\n",ans);
       }  
    }
    else if(strcmp(cmd,"cd") == 0){
      n = read(cfd,ans,MAX);
      printf("client: read = %d bytes\n",n);
      printf("server cwd changed to : %s\n",ans);
    }
    else if(strcmp(cmd,"lcd") == 0){
      if(strcmp(pathname,"\0")==0){
	  chdir("/home/hien/Desktop");
	}
	else{
	  chdir(pathname);
	}
	getcwd(cwd,128);
	
	printf("client cwd changed to : %s\n", cwd);
	n = read(cfd,ans,MAX);
      
    }
    else if(strcmp(cmd,"lmkdir")==0){
	int r = mkdir(pathname,0755);
	if(r == 0){
	  strcpy(line,"lmkdir - ok");
	}
	else{
	  printf("lmkdir - failed\n");
	}
	n = read(cfd, ans, MAX);
      }
      else if(strcmp(cmd,"lrmdir")==0){
	int r = rmdir(pathname);
	if(r == 0){
	  printf("lrmdir - ok\n");
	}
	else{
	  printf("lrmdir - failed\n");
	}
	n = read(cfd, ans, MAX);
      }
      else if(strcmp(cmd,"lrm")==0){
	int r = unlink(pathname);
	if(r == 0){
	  printf("lrm - ok\n");
	}
	else{
	  printf("lrm - failed\n");
	}
	n = read(cfd,ans,MAX);
      }
      else if(strcmp(cmd, "lpwd") == 0){
	getcwd(cwd,128);
	printf("client cwd: %s\n",cwd);
	n = read(cfd,ans,MAX);
      }
     else if(strcmp(cmd,"lls")==0){
	getcwd(cwd,128);
	struct dirent *ep;
	struct stat fstat, *sp;
	char ftime[64];
	DIR *dp;
	int r, i;
	char path[MAX];
	char line3[4096];
	printf("------lls result------\n\n");
        memset(line3,0,sizeof(line3));
	 memset(path,0,sizeof(path));
	if(strcmp(pathname,"\0")==0){

	  dp = opendir(cwd);
	}
	else if(pathname[0] != '/'){
	  strcpy(path,cwd);
	  strcat(path,"/");
	  strcat(path,pathname);
	  dp = opendir(path);	  
	}
	else{
	  dp = opendir(pathname);	  
	}
       
	while(ep = readdir(dp)){
	  if(strcmp(ep->d_name,".")==0){

	  }
	  else if(strcmp(ep->d_name,"..")==0){
	  }
	  else{
	    sp = &fstat;
	    r = lstat(ep->d_name,&fstat);
	    if((sp->st_mode & 0xF000) == 0x4000){
	      printf("%c",'d');
	    }
	    if((sp->st_mode & 0xF000) == 0x8000){
	      printf("%c",'-');
	    }
	    if((sp->st_mode &0xF000) == 0xA000){
	      printf("%c",'l');
	    }
	    m = strlen(line3);
	    for(i = 8; i >= 0; i--){
	      if(sp->st_mode & (1 << i)){
		printf("%c",t1[i]);
	      }
	      else{
		printf("%c",t2[i]);
	      }
	    }
	    printf("%4d ",sp->st_nlink);
	 
	    printf("%4d ",sp->st_gid);
      
	    printf("%4d ",sp->st_uid);
	     
	    printf("%8d ",sp->st_size);
	   
	    strcpy(ftime,ctime(&sp->st_ctime));
	    ftime[strlen(ftime) - 1] = 0;
	    printf("%s  ",ftime);
	     printf("%s",basename(ep->d_name));
	     printf("\n");
	  }
	}
	printf("ls - ok\n");
	closedir(dp);
	n = read(cfd,ans,MAX);
      }
     else if(strcmp(cmd,"lcat")==0){
       FILE *tp;
       int c;
       printf("--start of lcat--\n\n");
       tp = fopen(pathname,"r");
       if(tp == 0){
	 printf("failed to open file for read\n");
       }
       else{
	 while((c = fgetc(tp)) != EOF){
	   if(c == '\n'){
	     putchar('\n');
	     putchar('\r');
	   }
	   else{
	     putchar(c);
	   }
	 }     
       }
       printf("---end of lcat---\n");
     }
    else{
      n = read(cfd,ans,MAX);
      printf("client: read=%d bytes\n",n);
      printf("%s\n",ans);
      }
  }
} 

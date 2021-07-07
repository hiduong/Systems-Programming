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

//Name : Hien Duong
//Partner : Jimmy Zheng

#define MAX   256
#define PORT 1234
  
char line[MAX];
char ans[MAX];
char cmd[16], pathname[128];
char c_pathname[128];
int n;
int m = 0;
char cwd[128];
char p_cwd[128];
char home[128];
char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

int main(int argc, char *argv[]) 
{ 
  int sfd, cfd, len; 
  struct sockaddr_in saddr, caddr; 
  
  printf("1. create a TCP socket\n");
  sfd = socket(AF_INET, SOCK_STREAM, 0); 
  if (sfd < 0) { 
    printf("socket creation failed\n"); 
    exit(1); 
  }

  printf("2. fill in [localhost IP port=1234] as server address\n");
  bzero(&saddr, sizeof(saddr)); 
  saddr.sin_family = AF_INET; 
  saddr.sin_addr.s_addr = htonl(INADDR_ANY); 
  saddr.sin_port = htons(PORT); 
  
  printf("3. bind socket with server address\n");
  if ((bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr))) != 0) { 
    printf("socket bind failed\n"); 
    exit(2); 
  }
      
  printf("4. server listens\n");
  if ((listen(sfd, 5)) != 0) { 
    printf("Listen failed\n"); 
    exit(3); 
  }

  len = sizeof(caddr);
  while(1){
    printf("server accepting connection\n");
    cfd = accept(sfd, (struct sockaddr *)&caddr, &len); 
    if (cfd < 0) { 
      printf("server acccept failed\n"); 
      exit(4); 
    }
    printf("server acccepted a client connection\n"); 

    // server Processing loop
    while(1){
      printf("server: ready for next request\n");
      n = read(cfd, line, MAX);
      if (n==0){
	printf("server: client died, server loops\n");
	close(cfd);
	break;
      }
      
      // show the line contents
      printf("server: read  n=%d bytes; line=[%s]\n", n, line);

      // Process command
      memset(pathname,0,sizeof(pathname));
        memset(cmd,0,sizeof(cmd));
      sscanf(line,"%s %s", cmd, pathname);

      strcpy(c_pathname,pathname);
      if(strcmp(cmd,"mkdir")==0){
	int r = mkdir(pathname,0755);
	if(r == 0){
	  strcpy(line,"mkdir - ok");
	}
	else{
	  strcpy(line,"mkdir - failed");
	}
	n = write(cfd, line, MAX);
      }
      else if(strcmp(cmd,"rmdir")==0){
	int r = rmdir(pathname);
	if(r == 0){
	  strcpy(line,"rmdir - ok");
	}
	else{
	  strcpy(line,"rmdir - failed");
	}
	n = write(cfd, line, MAX);
      }
      else if(strcmp(cmd,"rm")==0){
	int r = unlink(pathname);
	if(r == 0){
	  strcpy(line,"rm - ok");
	}
	else{
	  strcpy(line,"rm - failed");
	}
	n = write(cfd,line,MAX);
      }
      else if(strcmp(cmd, "cd") == 0){
	if(strcmp(pathname,"\0")==0){
	  chdir("/home/hien/Desktop");
	}
	else{
	  chdir(pathname);
	}
	getcwd(cwd,128);
	n = write(cfd,cwd,MAX);
      }
      else if(strcmp(cmd,"get")==0){
        struct stat fstat, *sp;
	char size[MAX];
	int y, q, s_total = 0;
	 sp = &fstat;
	 y = lstat(pathname,&fstat);
	 sprintf(&size[0],"%d",sp->st_size);
	 write(cfd,size,MAX);
	 printf("server sending : %d bytes to client\n",sp->st_size);

	 FILE *gp = fopen(pathname,"r");
	 char line5[sp->st_size];
	 while(q = fread(line5,1,sp->st_size,gp)){
	 
	 }
	 write(cfd,line5,sp->st_size);
	
      }
      else if(strcmp(cmd,"put")==0){
	 n = read(cfd,ans,MAX);
      int l;

      l = atoi(ans);
      char line5[l];
      FILE *mp = fopen(pathname,"w");

      n = read(cfd,line5,l);
      fwrite(line5,1,l,mp);
      printf("server recieved %d bytes from client\n",l);
      printf("put - ok\n");
      
      fclose(mp);

	
      }
      else if(strcmp(cmd,"ls")==0){
	getcwd(cwd,128);
	struct dirent *ep;
	struct stat fstat, *sp;
	char ftime[64];
	DIR *dp;
	int r, i;
	char path[MAX];
	char line3[4096];
        memset(line3,0,sizeof(line3));
	 memset(line,0,sizeof(line));
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
	      sprintf(line3,"%c",'d');
	    }
	    if((sp->st_mode & 0xF000) == 0x8000){
	      sprintf(line3,"%c",'-');
	    }
	    if((sp->st_mode &0xF000) == 0xA000){
	      sprintf(line3,"%c",'l');
	    }
	    m = strlen(line3);
	    for(i = 8; i >= 0; i--){
	      if(sp->st_mode & (1 << i)){
		sprintf(&line3[m],"%c",t1[i]);
	      }
	      else{
		sprintf(&line3[m],"%c",t2[i]);
	      }
	      m = strlen(line3);
	    }
	     m = strlen(line3);
	    sprintf(&line3[m],"%4d ",sp->st_nlink);
	     m = strlen(line3);
	    sprintf(&line3[m],"%4d ",sp->st_gid);
	     m = strlen(line3);
	    sprintf(&line3[m],"%4d ",sp->st_uid);
	     m = strlen(line3);
	    sprintf(&line3[m],"%8d ",sp->st_size);
	     m = strlen(line3);

	    strcpy(ftime,ctime(&sp->st_ctime));
	    ftime[strlen(ftime) - 1] = 0;
	    sprintf(&line3[m],"%s  ",ftime);
	     m = strlen(line3);
	     sprintf(&line3[m],"%s",basename(ep->d_name));
	    n = write(cfd,line3,MAX);
	  }
	}
	strcpy(line,"ls - ok");
	closedir(dp);
	n = write(cfd,line,MAX);
	printf("server: sent ls result\n");
      }
      else if(strcmp(cmd,"pwd")==0){
	getcwd(cwd,128);
	strcpy(line,"cwd : ");
	strcat(line,cwd);
	n = write(cfd,line,MAX);
      }
      else if(strcmp(cmd,"quit") == 0){

      }
      else{
	printf("server: echos: %s\n", line);
	n = write(cfd,line,MAX);
      }
    }	      
  }
}


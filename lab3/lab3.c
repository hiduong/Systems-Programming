#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

char *myargv[128];
char *myargv2[128];
char *myargv3[128];
char *myargv4[128];
char mypath[128];
char path[16][64];
char home[128];
char line[128];

void piped(char *env[]){
  int pd[2];
  int cid,ccid,cccid;
  int status;
  int r = -1, k = 0;
  pipe(pd);
  cid = fork(); //fork a child process to share the pipe, parent is writer and child is reader
  if(cid){ // parent will be the pipe writer
    close(1); //close output
    dup(pd[1]); //replace output with pd[1](writer procc)
    close(pd[0]); //writer closes pd[0](reader proc)
    close(pd[1]); //writer closes pd[1];
    ccid = fork(); //fork a process to do the head
    if(ccid){
      ccid = wait(&status);
    }
    else{ 
      k = 0;
      r = -1;
      fprintf(stderr,"Trying %s in each PATH dir\n",myargv[0]);
      while(r == -1){
	strcpy(line,path[k]);
	strcat(line,"/");
	strcat(line,myargv[0]);
	fprintf(stderr,"PATH[%d] = %s\n",k,line);
	r = execve(line,myargv,env);
	k++;
      }
    }
    close(1);
    cid = wait(&status);
  }
  else{ //child will be the pipe reader
    close(0); //close input
    dup(pd[0]); //replace input with pd[0](reader procc)
    close(pd[1]); // reader closes pd[1] the writer procc
    close(pd[0]); // close pd[0]
    cccid = fork(); //fork a child process to do command
    if(cccid){
      cccid = wait(&status);
    }
    else{
      k = 0;
      r = -1;
      fprintf(stderr,"Trying %s in each PATH dir\n",myargv2[0]);
      while(r == -1){
	strcpy(line,path[k]);
	strcat(line,"/");
	strcat(line,myargv2[0]);
	fprintf(stderr,"PATH[%d] = %s\n",k,line);
	r = execve(line,myargv2,env);
	k++;
      }
    }
  }
}

void piped2(char *env[]){

  int pd[2],pd2[2];
  int cid,ccid,cccid;
  int id,iid,iiid;
  int status;
  int r = -1, k = 0;
  pipe(pd);
  cid = fork();
  if(cid){
    close(1);
    dup(pd[1]);
    close(pd[0]);
    close(pd[1]);

    ccid = fork();
    if(ccid){
      ccid = wait(&status);
    }
    else{
      k = 0;
      r = -1;
      fprintf(stderr,"Trying %s in each PATH dir\n",myargv[0]);
      while(r == -1){
	strcpy(line,path[k]);
	strcat(line,"/");
	strcat(line,myargv[0]);
	fprintf(stderr,"PATH[%d] = %s\n",k,line);
	r = execve(line,myargv,env);
	k++;
      }
    }
    close(1);
    cid = wait(&status);
  }
  else{
    close(0);
    dup(pd[0]);
    close(pd[1]);
    close(pd[0]);
    
    pipe(pd2);
    id = fork();
    if(id){
      close(1);
      dup(pd2[1]);
      close(pd2[0]);
      close(pd2[1]);

      iid = fork();
      if(iid){
	iid = wait(&status);
      }
      else{
	k = 0;
	r = -1;
	fprintf(stderr,"Trying %s in each PATH dir\n",myargv2[0]);
	while(r == -1){
	  strcpy(line,path[k]);
	  strcat(line,"/");
	  strcat(line,myargv2[0]);
	  fprintf(stderr,"PATH[%d] = %s\n",k,line);
	  r = execve(line,myargv2,env);
	  k++;
	}
      }
      close(1);
      id = wait(&status);
    }
    else{
      close(0);
      dup(pd2[0]);
      close(pd2[1]);
      close(pd2[0]);

      iiid = fork();
      if(iiid){
	iiid = wait(&status);
      }
      else{
	k = 0;
	r = -1;
	fprintf(stderr,"Trying %s in each PATH dir\n",myargv3[0]);
	while(r == -1){
	  strcpy(line,path[k]);
	  strcat(line,"/");
	  strcat(line,myargv3[0]);
	  fprintf(stderr,"PATH[%d] = %s\n",k,line);
	  r = execve(line,myargv3,env);
	  k++;
	}
      }
    }
    
  }
}

void piped3(char *env[]){

  int pd[2],pd2[2],pd3[2];
  int cid,ccid,cccid;
  int id,iid,iiid;
  int kid,kkid,kkkid;
  int status;
  int r = -1, k = 0;
  pipe(pd);
  cid = fork();
  if(cid){
    close(1);
    dup(pd[1]);
    close(pd[0]);
    close(pd[1]);

    ccid = fork();
    if(ccid){
      ccid = wait(&status);
    }
    else{
      k = 0;
      r = -1;
      fprintf(stderr,"Trying %s in each PATH dir\n",myargv[0]);
      while(r == -1){
	strcpy(line,path[k]);
	strcat(line,"/");
	strcat(line,myargv[0]);
	fprintf(stderr,"PATH[%d] = %s\n",k,line);
	r = execve(line,myargv,env);
	k++;
      }
    }
    close(1);
    cid = wait(&status);
  }
  else{
    close(0);
    dup(pd[0]);
    close(pd[1]);
    close(pd[0]);
    
    pipe(pd2);
    id = fork();
    if(id){
      close(1);
      dup(pd2[1]);
      close(pd2[0]);
      close(pd2[1]);

      iid = fork();
      if(iid){
	iid = wait(&status);
      }
      else{
	k = 0;
	r = -1;
	fprintf(stderr,"Trying %s in each PATH dir\n",myargv2[0]);
	while(r == -1){
	  strcpy(line,path[k]);
	  strcat(line,"/");
	  strcat(line,myargv2[0]);
	  fprintf(stderr,"PATH[%d] = %s\n",k,line);
	  r = execve(line,myargv2,env);
	  k++;
	}
      }
      close(1);
      id = wait(&status);
    }
    else{
      close(0);
      dup(pd2[0]);
      close(pd2[1]);
      close(pd2[0]);

      //3rd pipe
      pipe(pd3);

      kid = fork();
      if(kid){
	close(1);
	dup(pd3[1]);
	close(pd3[0]);
	close(pd3[1]);

	kkid = fork();
	if(kkid){
	  kkid = wait(&status);
	}
	else{
	  k = 0;
	  r = -1;
	  fprintf(stderr,"Trying %s in each PATH dir\n",myargv3[0]);
	  while(r == -1){
	    strcpy(line,path[k]);
	    strcat(line,"/");
	    strcat(line,myargv3[0]);
	    fprintf(stderr,"PATH[%d] = %s\n",k,line);
	    r = execve(line,myargv3,env);
	    k++;
	  }
	}
	close(1);
	kid = wait(&status);
      }
      else{
	close(0);
	dup(pd3[0]);
	close(pd3[1]);
	close(pd3[0]);

	kkkid = fork();
	if(kkkid){
	  kkkid = wait(&status);
	}
	else{
	  k = 0;
	  r = -1;
	  fprintf(stderr,"Trying %s in each PATH dir\n",myargv4[0]);
	  while(r == -1){
	    strcpy(line,path[k]);
	    strcat(line,"/");
	    strcat(line,myargv4[0]);
	    fprintf(stderr,"PATH[%d] = %s\n",k,line);
	    r = execve(line,myargv4,env);
	    k++;
	  }
	}
      } 
    } //end of else    
  }  
}

int main(int argc,char*argv[],char *env[]){

  int i = 0,n = 0, r = -1, k = 0,j=0,w = 0;
  int pid,status;
  char line2[128];
  char *s;
  char *d;
  char *b;
  int pipe_counter = 0;
  int count = 0;
  int counte = 0;
  int counter = 0;
  char cmd[16][16],param[16][64];
  //----------GET PATH & HOME---------
  strcpy(mypath,&env[21][5]);
  strcpy(home,&env[38][5]);
  printf("1.Show PATH:\n");
  printf("MYPATH = %s\n\n",mypath);

  //-----------TOKENIZE PATH----------
  printf("2.Decompose PATH into dir strings:\n");
  s = strtok(mypath,":");
  while(s){
    strcpy(path[n++],s);
    s = strtok(0,":");
  }
  for(i = 0; i < n; i++)
    {
      printf("dir[%d] : %s\n",i,path[i]);
    }
  printf("\n");
  //--------SHOW HOME DIRECTORY--------
  printf("3.Showing HOME directory:\n");
  printf("HOME = %s\n\n",home);

  //--------HDSH PROCESSING LOOP-------
  printf("4.*********** hdsh processing loop **********\n");
  while(1){
    char cline[64][128];
    char dline[64][128];
    memset(line,0,sizeof(line));
    memset(line2,0,sizeof(line2));
    memset(cmd,0,sizeof(cmd));
    memset(param,0,sizeof(param));
    memset(cline,0,sizeof(cline));
    memset(dline,0,sizeof(dline));
    w = 0;
    j = 0;
    k = 0;
    r = -1;
    pipe_counter = 0;
    counter = 0;
    count = 0;
    counte = 0;
    //Reading the command
    fprintf(stderr,"\nhdsh $ : ");
    fgets(line,128,stdin);
    line[strlen(line) - 1] = 0;
    strcpy(line2,line);

    //tokenize the command by whitespace
    d = strtok(line," ");
    while(d){
      strcpy(cline[j++],d);
      d = strtok(0," ");
    }

    //tokenize by |
    b = strtok(line2,"|");
    while(b){
      strcpy(dline[w++],b);
      b = strtok(0,"|");
    }

    //adding cmd and param
    for(int a = 0; a < w; a++){
      sscanf(dline[a],"%s %s",cmd[a],param[a]);
    }

    //check for any pipes
    for(int p = 0;p < j; p++){
      if(strcmp(cline[p],"|")==0){
	pipe_counter++;
      }
    }

    //check for any i/0 redirections
    
    for(int p = 0; p < j; p++){
      if(strcmp(cline[p],">")==0){
	count++;
      }
    }

    for(int p = 0; p < j; p++){
      if(strcmp(cline[p],">>")==0){
	counte++;
      }
    }

    for(int p = 0; p < j; p++){
      if(strcmp(cline[p],"<")==0){
	counter++;
      }
    }
    
    if(strcmp(cline[0],"ls") == 0){
      if(strcmp(cline[1],"\0")==0){
	strcpy(cline[1],"-L");
      }
    }

    if(strcmp(cline[0],"pwd") == 0){
      if(strcmp(cline[1],"\0")==0){
	strcpy(cline[1],"-L");
      }
    }
    
    //running commands
    //cd and exit command
    if(strcmp(cline[0],"cd") == 0){
      if(strcmp(cline[1],"\0")==0){
	chdir(home);
      }
      else{
	chdir(cline[1]);
      }
    }
    else  if(strcmp(cline[0],"exit")==0){
      exit(0);}
    else  if(strcmp(cline[0],"quit")==0){
      exit(0);}
    else{
      //if pipe counter == 0 no pipes
      if(pipe_counter == 0 && count == 0 && counte == 0 && counter == 0){	
	pid = fork();
	if(pid < 0){
	  fprintf(stderr,"Failed to Fork\n");
	}
	else if(pid){
	  pid = wait(&status);
	  fprintf(stderr,"\nDead Child = %d : Status = %x\n",pid,status);
	}
	else{
	  myargv[0] = cline[0];
	  myargv[1] = cline[1];
	  myargv[2] = 0;
	  k = 0;

	  fprintf(stderr,"Trying %s in each PATH dir\n",cline[0]);
	  while(r == -1){
	    strcpy(line,path[k]);
	    strcat(line,"/");
	    strcat(line,myargv[0]);
	    fprintf(stderr,"PATH[%d] = %s\n",k,line);
	    r = execve(line,myargv,env);
	    k++;
	  }
	}
      } //end of if pipe_counter == 0
      else if(pipe_counter != 0){
	int cid = fork();
	if(cid < 0){
	  fprintf(stderr,"Failed to Fork a Child Process\n");
	  exit(1);
	}
	if(cid){
	  cid = wait(&status);
	  fprintf(stderr,"\nDead Child = %d : Status = %x\n",cid,status);
	}
	else{
	  //If I had more time I definitely would've Implemented this better.
	  if(pipe_counter == 1){
	    myargv[0] = cmd[0];
	    myargv[1] = param[0];
	    myargv[2] = 0;

	    myargv2[0] = cmd[1];
	    myargv2[1] = param[1];
	    myargv2[2] = 0;
	    piped(env);
	  }
	  else if(pipe_counter == 2){
	    myargv[0] = cmd[0];
	    myargv[1] = param[0];
	    myargv[2] = 0;

	    myargv2[0] = cmd[1];
	    myargv2[1] = param[1];
	    myargv2[2] = cline[5];
	    myargv2[3] = 0;

	    myargv3[0] = cmd[2];
	    myargv3[1] = param[2];
	    myargv3[2] = 0;
	    piped2(env);
	  }
	  else if(pipe_counter == 3){
	    myargv[0] = cmd[0];
	    myargv[1] = param[0];
	    myargv[2] = 0;

	    myargv2[0] = cmd[1];
	    myargv2[1] = 0;

	    myargv3[0] = cmd[2];
	    myargv3[1] = param[2];
	    myargv3[2] = cline[7];
	    myargv3[3] = 0;

	    myargv4[0] = cmd[3];
	    myargv4[1] = param[3];
	    myargv4[2] = 0;
	    	    
	    piped3(env);
	  }
	  exit(1);
	}
      }// end of pipe_counter > 0
      else if(count != 0){
	// > write

	int pid2 = fork();
	if(pid2 < 0){
	  fprintf(stderr,"Failed to Fork a Child Process\n");
	}
	if(pid2){
	  pid2 = wait(&status);
	  fprintf(stderr,"\nDead Child = %d : Status = %x\n",pid2,status);
	}
	else{
	  close(1);
	  //change file descriptor 1 to point to open file to create and replace
	  open(cline[3],O_WRONLY|O_CREAT|O_TRUNC,0644);
	  myargv[0] = cline[0];
	  myargv[1] = cline[1];
	  myargv[2] = 0;
	  k = 0;
	  r = -1;
	  fprintf(stderr,"Trying %s in each PATH dir\n",cline[0]);
	  while(r == -1){
	    strcpy(line,path[k]);
	    strcat(line,"/");
	    strcat(line,myargv[0]);
	    fprintf(stderr,"PATH[%d] = %s\n",k,line);
	    r = execve(line,myargv,env);
	    k++;
	  }
	}
	
      }// end of count != 0
      else if(counte != 0){
	// >> append
	int pid2 = fork();
	if(pid2 < 0){
	  fprintf(stderr,"Failed to Fork a Child Process\n");
	}
	if(pid2){
	  pid2 = wait(&status);
	  fprintf(stderr,"\nDead Child = %d : Status = %x\n",pid2,status);
	}
	else{
	  close(1);
	  //change file descriptor 1 to point to open file to wrtie, and append and create
	  open(cline[3],O_WRONLY|O_APPEND|O_CREAT,0644);
	  myargv[0] = cline[0];
	  myargv[1] = cline[1];
	  myargv[2] = 0;
	  k = 0;
	  r = -1;
	  fprintf(stderr,"Trying %s in each PATH dir\n",cline[0]);
	  while(r == -1){
	    strcpy(line,path[k]);
	    strcat(line,"/");
	    strcat(line,myargv[0]);
	    fprintf(stderr,"PATH[%d] = %s\n",k,line);
	    r = execve(line,myargv,env);
	    k++;
	  }
	}
      }//end of counte != 0
      else if(counter != 0){
	// < read
	int pid2 = fork();
	if(pid2 < 0){
	  fprintf(stderr,"Failed to Fork a Child Process\n");
	}
	if(pid2){
	  pid2 = wait(&status);
	  fprintf(stderr,"\nDead Child = %d : Status = %x\n",pid2,status);
	}
	else{
	  close(0);
	  //change file descriptor 0 to point to open file for read
	  open(cline[2],O_RDONLY); 
	  myargv[0] = cline[0];
	  myargv[1] = 0;
	  k = 0;
	  r = -1;
	  fprintf(stderr,"Trying %s in each PATH dir\n",cline[0]);
	  while(r == -1){
	    strcpy(line,path[k]);
	    strcat(line,"/");
	    strcat(line,myargv[0]);
	    fprintf(stderr,"PATH[%d] = %s\n",k,line);
	    r = execve(line,myargv,env);
	    k++;
	  }
	}
      }//end of counter != 0
    } //end of else
  } //end sh while loop
}

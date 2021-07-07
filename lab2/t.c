#include <stdio.h>             // for I/O
#include <stdlib.h>            // for I/O
#include <libgen.h>            // for dirname()/basename()
#include <string.h>

typedef struct node{
  char  name[64];       // node's name string
  char  type;
  struct node *child, *sibling, *parent;
}NODE;

NODE *root, *cwd, *start;
char line[128];
char command[16], pathname[64];
char dname[64], bname[64];
char gpath[256];
char pname[16][64];           // OR   char *name[16];
int n;

//               0       1      2    3     4     5      6     7       8      9        10
char *cmd[] = {"mkdir","rmdir","ls","cd","pwd","creat","rm","reload","save","menu", "quit", 0};

int dbname(char *name)
{
  char temp[128];
  strcpy(temp, pathname);
  strcpy(dname, dirname(temp));
  strcpy(temp, pathname);
  strcpy(bname,basename(temp));
}

int tokenize(char *pathname)
{
  char *s;
  strcpy(gpath, pathname);
  n = 0;
  s = strtok(gpath, "/");
  while(s){
    strcpy(pname[n++], s);    // OR  name[n++] = s;
    s = strtok(0, "/");
  }
}

int findCmd(char *command)
{
  int i = 0;
  while(cmd[i]){
    if (strcmp(command, cmd[i])==0)
      return i;
    i++;
  }
  return -1;
}

NODE *search_child(NODE *parent, char *name)
{
  NODE *p;
  printf("Search for %s in %s \n", name, parent->name);
  p = parent->child;
  if (p==0){
    return 0;}
  while(p){
    if (strcmp(p->name, name)==0)
      return p;
    p = p->sibling;
  }
  return 0;
}

NODE *search(NODE *parent, char *name){

NODE *p;
  p = parent->child;
  if (p==0){
    return 0;}
  while(p){
    if (strcmp(p->name, name)==0)
      return p;
    p = p->sibling;
  }
  return 0;
}

int insert_child(NODE *parent, NODE *q)
{
  NODE *p;
  p = parent->child;
  if (p==0)
    parent->child = q;
  else{
    while(p->sibling)
      p = p->sibling;
    p->sibling = q;
  }
  q->parent = parent;
  q->child = 0;
  q->sibling = 0;
}

int delete_child(NODE *child)
{
  strcpy(child->name,"\0");
  child->type = '\0';
  child->parent = 0;
  child->sibling = 0;
  free(child);
}

int mkdir(char *name)
{
  int i = 0;
  NODE *p, *q;
  printf("***mkdir: name=%s\n", name);

  if (!strcmp(name, "/") || !strcmp(name, ".") || !strcmp(name, "..")){
    printf("can't mkdir with %s\n", name);
    return -1;
  }

  //break up pathname into dirname and basename
  dbname(name);
  if(!strcmp(dname,"."))
    {
      strcpy(dname,"/");
    }
  printf("dirname = %s\tbasename = %s\n",dname, bname);
  tokenize(dname);
  printf("pathname=%s : n = %d ==> ", dname, n);
  for(int j = 0; j < n; j++)
    {
      printf("%s ",pname[j]);
    }
  printf("\n");
  if (name[0] == '/'){
    start = root;}
  else{
    start = cwd;}

  if(!strcmp(dname,"/"))
    {
      p = search_child(start, bname);
      if (p){
	printf("%s already exists, mkdir FAILED\n", bname);
	return -1;}
    }
  else
    {
      for(i = 0; i < n; i++){
	p = search_child(start, pname[i]);
	if(p->type == 'F'){
	printf("%s is not a DIR type, mkdir FAILED\n",pname[i]);
	return -1;
      } 
	if(p == 0)
	  {
	    printf("%s does not exist, mkdir FAILED\n",pname[i]);
	    return -1;
	  }
	start = p;}
      p = search_child(start, bname);   
       if (p){
	printf("%s already exists, mkdir FAILED\n", bname);
	return -1;}
    }
  
  q = (NODE *)malloc(sizeof(NODE));
  q->type = 'D';
  strcpy(q->name, bname);
  insert_child(start, q);
  printf("----mkdir %s OK----\n", bname);
  return 0;
}

int rmdir(char *name)
{
  NODE *p;
  printf("***rmdir: name=%s\n", name);
  dbname(name);
  tokenize(dname);
  if(!strcmp(dname,"."))
    {
      strcpy(dname,"/");
    }
  if(name[0] == '/'){
    start = root;}
  else{
    start = cwd;}
  if(!strcmp(dname,"/"))
    { 
      p = search_child(start, bname);
      if(p == 0){
	printf("DIR %s does not exist in %s\n",bname, name);
	return -1;}
      if(p->child != 0){
	printf("DIR not empty\n");
	return -1;
      }
      if(p->type == 'F'){
	printf("%s is not a DIR type, rmdir FAILED\n",bname);
	return -1;
      } 
     
    }
  else
    {
      for(int i = 0; i < n; i++){
	p = search_child(start, pname[i]);
	if(p == 0){
	  printf("DIR %s does not exist in %s\n",pname[i], name);
	  return -1;}
	if(p->type == 'F'){
	printf("%s is not a DIR type, rmdir FAILED\n",pname[i]);
	return -1;
      } 
	start = p;
      }
      	p = search_child(start, bname);
	if(p == 0){
	  printf("DIR %s does not exist in %s\n",bname, name);
	  return -1;}
	if(p->type == 'F'){
	printf("%s is not a DIR type, rmdir FAILED\n",bname);
	return -1;
      } 
    }
  
  start = p;
  if(start->child != 0 && start->child->type == 'F'){
	printf("DIR not empty\n");
	return -1;
      }
  else if(start->child != 0 && start->child->type == 'D'){
	printf("DIR not empty\n");
	return -1;
      }
  delete_child(start);
  printf("---- rmdir ok ----\n");
  return 0;
}


int ls(char *pathname)
{
  NODE *p;
  tokenize(pathname);
  if(pathname[0] == '\0') //if no pathname specified assume printing cwd
    {
      p = cwd->child;
      printf("---- cwd contents ----\n");
      while(p){
	if(p->type != '\0'){
	  printf("[%c %s]\n", p->type, p->name);
	}
        p = p->sibling;

      }
  }
  else
    {
      start = root;
      for(int i = 0; i < n; i++){
	p = search_child(start,pname[i]);
	if(p == 0){
	  printf("DIR %s does not exist in %s\n",pname[i], pathname);
	  return -1;}
	if(p->type == 'F'){
	printf("%s is not a DIR type, ls FAILED\n",pname[i]);
	return -1;
      } 
	
	start = p;
      }
      p = p->child;
      printf("---- cwd contents ----\n");
      while(p){
	if(p->type != '\0'){
	  printf("[%c %s]\n", p->type, p->name);
	}
        p = p->sibling;

      }
    }
  return 0;
}

int cd(char *name){
  NODE *p;
  tokenize(name);
  if(!strcmp(pname[0],"..")){
    for(int i = 0; i < n; i++){
      cwd = cwd->parent;
    }
    printf("---- cd to %s ok ----\n",cwd->name);
    return -1;
    }
  if(name[0] == '/'){
    start = root;}
  else{ start = cwd;}
  for(int i = 0; i < n; i++){
    p = search(start,pname[i]);
    if(p == 0){
      printf("DIR %s does not exist in %s\n",pname[i],name);
      return -1;}
    if(p->type == 'F'){
	printf("%s is not a DIR type, cd FAILED\n",pname[i]);
	return -1;
      } 
    start = p;
  }
  cwd = start;
  printf("---- cd to %s ok ----\n",name);
  return 0;
  
}

int pwd(){

  NODE *p = cwd;
  char temp[16][64];
  int i = 0;
  printf("**** pwd ****\n");
  if(!strcmp(cwd->name,"/")){
    printf("/\n");
      return -1;}
  while(strcmp(p->name,"/")){
    strcpy(temp[i++],p->name);
    p = p->parent;
  }

  for(int j = i - 1; j != -1; j--){
    printf("/%s",temp[j]);
  }
  
  printf("\n");
  return 0;
}

int fpwd(NODE *curr, FILE *fp){

  NODE *p = curr;
  char temp[16][64];
  int i = 0;
  while(strcmp(p->name,"/")){
    strcpy(temp[i++],p->name);
    p = p->parent;
  }
  fprintf(fp,"%c,",curr->type);
  printf("%c\t",curr->type);
  for(int j = i - 1; j != -1; j--){
  fprintf(fp,"/%s",temp[j]);
  printf("/%s",temp[j]);
  }
  if(!strcmp(curr->name,"/")){
     fprintf(fp,"/");
     printf("/");
  }
  fprintf(fp,"\n");
  printf("\n");
  return 0;
}

int preorder(NODE *curr, FILE *fp){

  if(curr == 0)
    {
      return -1;
    }
  fpwd(curr, fp);
  preorder(curr->child, fp);
  preorder(curr->sibling, fp);
}

int creat(char *name)
{
  int i = 0;
  NODE *p, *q;
  printf("***creat: name=%s\n", name);

  if (!strcmp(name, "/") || !strcmp(name, ".") || !strcmp(name, "..")){
    printf("can't creat with %s\n", name);
    return -1;
  }

  //break up pathname into dirname and basename
  dbname(name);
  if(!strcmp(dname,"."))
    {
      strcpy(dname,"/");
    }
  printf("dirname = %s\tbasename = %s\n",dname, bname);
  tokenize(dname);
  printf("pathname=%s : n = %d ==> ", dname, n);
  for(int j = 0; j < n; j++)
    {
      printf("%s ",pname[j]);
    }
  printf("\n");
  if (name[0] == '/'){
    start = root;}
  else{
    start = cwd;}

  if(!strcmp(dname,"/"))
    {
      p = search_child(start, bname);
      if (p){
	printf("%s already exists, creat FAILED\n", bname);
	return -1;}
    }
  else
    {
      for(i = 0; i < n; i++){
	p = search_child(start, pname[i]);
	if(p == 0)
	  {
	    printf("%s does not exist, creat FAILED\n",bname);
	    return -1;
	  }
	if(p->type == 'F'){
	printf("%s is not a DIR type, creat FAILED\n",pname[i]);
	return -1;
      } 
	start = p;}
      p = search_child(start, bname);
      if (p){
	printf("%s already exists, creat FAILED\n", bname);
	return -1;}
    }
  q = (NODE *)malloc(sizeof(NODE));
  q->type = 'F';
  strcpy(q->name, bname);
  insert_child(start, q);
  printf("----creat %s OK----\n", bname);
  return 0;
}

int rm(char *name)
{
  NODE *p;
  printf("***rm: name=%s\n", name);
  dbname(name);
  tokenize(dname);
  if(!strcmp(dname,"."))
    {
      strcpy(dname,"/");
    }
  if(name[0] == '/'){
    start = root;}
  else{
    start = cwd;}
  if(!strcmp(dname,"/"))
    { 
      p = search_child(start, bname);
      if(p == 0){
	printf("DIR %s does not exist in %s\n",bname, name);
	return -1;}
      if(p->child != 0){
	printf("DIR not empty\n");
	return -1;
      }
      if(p->type == 'D'){
	printf("%s is not a F type, rm FAILED\n",bname);
	return -1;
      } 
     
    }
  else
    {
      for(int i = 0; i < n; i++){
	p = search_child(start, pname[i]);
	if(p == 0){
	  printf("DIR %s does not exist in %s\n",pname[i], name);
	  return -1;}
	if(p->type == 'F'){
	printf("%s is not a DIR type, invalid pathname, rm FAILED\n",pname[i]);
	return -1;
      } 
	start = p;
      }
      	p = search_child(start, bname);
	if(p == 0){
	  printf("DIR %s does not exist in %s\n",bname, name);
	  return -1;}
	if(p->type == 'D'){
	printf("%s is not an DIR type, rm FAILED\n",bname);
	return -1;
      } 
    }
  
  start = p;
  delete_child(start);
  printf("---- rm ok ----\n");
  return 0;
}

int reload(){
  FILE *fp = fopen("myfile.csv","r");
  char *s;
  int i = 0;
  char t[16][64];
  char path[16][64];
  char buffer[128];
  printf("---- LOADING ----\n\n");
  while(fgets(buffer,128,fp) != 0){
    int len = strlen(buffer);
    if(buffer[len - 1] == '\n')
      {
	buffer[len -1] = '\0';
      }
    s = strtok(buffer,",");
    strcpy(t[i],s);
    s = strtok(0,",");
    if(s != 0){
      
      strcpy(path[i],s);
    }
      if(!strcmp(t[i],"D")){
	strcpy(pathname,path[i]);
	mkdir(pathname);
      }
      else if(!strcmp(t[i],"F")){
	strcpy(pathname,path[i]);
	creat(pathname);
      }
    i++;
  }
  printf("\n");
  printf("---- LOAD OK ----\n");
  fclose(fp);
}

int save(){
  FILE *fp = fopen("myfile.csv", "w+");
  printf("---- SAVING! ----\n");
  printf("TYPE\tPATHNAME\n");
  preorder(root,fp);
  printf("---- SAVE OK ----\n");
  fclose(fp);
}

int quit()
{
  printf("Program exit\n");
  save();
  exit(0);
}

int initialize()
{
  root = (NODE *)malloc(sizeof(NODE));
  strcpy(root->name, "/");
  root->parent = root;
  root->sibling = 0;
  root->child = 0;
  root->type = 'D';
  cwd = root;
  printf("Root initialized OK\n");
}

int main()
{
  int index;

  initialize();

  printf("Enter menu for list of valid commands\n");

  while(1){
    printf("Enter command line : ");
    fgets(line, 128, stdin);
    line[strlen(line)-1] = 0;

    command[0] = pathname[0] = 0;
    sscanf(line, "%s %s", command, pathname);
    printf("\ncommand=%s pathname=%s\n", command, pathname);
      
    if (command[0]==0) 
      continue;

    index = findCmd(command);

    switch (index){
    case 0: mkdir(pathname); break;
    case 1: rmdir(pathname); break;
    case 2: ls(pathname);    break;
    case 3: cd(pathname);    break;
    case 4: pwd();           break;
    case 5: creat(pathname); break;
    case 6: rm(pathname);    break;
    case 7: reload();        break;
    case 8: save();          break;
    case 9:
      printf("========================Menu========================\n");
      printf("= mkdir - make new directory, for a given pathname =\n");
      printf("= rmdir - remove the directory                     =\n");
      printf("= cd - change CWD to pathname or to root           =\n");
      printf("= ls - list the directory contents                 =\n");
      printf("= pwd - print the absolute pathname of CWD         =\n");
      printf("= creat - create a FILE node                       =\n");
      printf("= rm - remove the FILE node                        =\n");
      printf("= save - save the current file system tree         =\n");
      printf("= reload - construct a file system from a file     =\n");
      printf("= menu - show a menu of valid commands             =\n");
      printf("= quit - save the file system and terminate        =\n");
      printf("====================================================\n");
      break;
    case 10: quit();          break;
    default: printf("Not a valid command, enter menu for list of valid commands\n"); break;
    }
  }
}

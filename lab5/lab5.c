#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

#define BLKSIZE 1024

typedef struct ext2_group_desc GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode INODE;
typedef struct ext2_dir_entry_2 DIR;

GD *gp;
SUPER *sp;
INODE *ip;
DIR *dp;

char *dev;
char buf[1024], ibuf[1024], cbuf[1024];
int dbuf[1024];
char gpath[256];
char pname[16][64];
int n;

int fd;
int bmap,imap,iblock;
int rootblock;

int get_block(int fd, char *buf, int blk)
{
  int n;
  lseek(fd,blk*1024,SEEK_SET);
  n = read(fd,buf,1024);
  if(n < 0){
    printf("get block error %d\n", blk);
    return -1;
  }
  return n;
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

void show_dir(INODE *ip)
{
  char sbuf[BLKSIZE], temp[256];
  DIR *dp;
  char *cp;
  int i;
  printf("| i_number | rec_len | name_len | name \n");
  for (i=0; i < 12; i++){  // assume DIR at most 12 direct blocks
    if (ip->i_block[i] == 0){
      break;
    }
    
    // YOU SHOULD print i_block[i] number here
    get_block(fd, sbuf,ip->i_block[i]);

    dp = (DIR *)sbuf;
    cp = sbuf;

    while(cp < sbuf + BLKSIZE){
    strncpy(temp, dp->name, dp->name_len);
    temp[dp->name_len] = 0;
    printf("%6d %10d %9d        %s\n",  dp->inode, dp->rec_len, dp->name_len, temp);
    cp += dp->rec_len;
    dp = (DIR *)cp;
    }
    
  }
}


int search(INODE *ip, char *name)
{
  char sbuf[BLKSIZE], temp[256];
  DIR *dp;
  char *cp;
  int i = 0;
  printf("i=%d i_block[0]=%d\n",i,ip->i_block[0]);
  printf("| i_number | rec_len | name_len | name \n");
  for (i=0; i < 12; i++){  // assume DIR at most 12 direct blocks
    if (ip->i_block[i] == 0){
      break;
    }
    
    // YOU SHOULD print i_block[i] number here
    get_block(fd, sbuf,ip->i_block[i]);

    dp = (DIR *)sbuf;
    cp = sbuf;

    while(cp < sbuf + BLKSIZE){
    strncpy(temp, dp->name, dp->name_len);
    temp[dp->name_len] = 0;
    printf("%6d %10d %9d        %s\n",  dp->inode, dp->rec_len, dp->name_len, temp);

    if(strcmp(temp,name) == 0){
      printf("found %s : ino = %d\n", name, dp->inode);
      return dp->inode;
    }
    cp += dp->rec_len;
    dp = (DIR *)cp;
    }
  }
  return 0;
}


int main(int argc, char*argv[]){
  char pathname[1024];
  if(argc > 2){
    dev = argv[1]; // get device
    strcpy(pathname,argv[2]); // copy pathname
  }
  else{
    printf("Give device and pathname argument: argv[1] = device, argv[2] = pathname\n");
    exit(1);
  }
  fd = open(dev,O_RDONLY);
  if(fd < 0){
    printf("Failed to open %s\n",dev);
    exit(2);
  }

  printf("(1). Read in SUPER block, verify if %s is an ext2 filesystem: ", dev);
  get_block(fd,buf,1);
  sp = (SUPER *)buf;
  if(sp->s_magic != 0xEF53){
    printf("%s is not an ext2 FS\n",dev);
    exit(3);
  }
  else{
    printf("OK\n");
  }
  
  printf("(2). Read Group descriptor 0 to get block number of bmap imap inodes_start\n");
  get_block(fd,buf,2);
  gp = (GD *)buf;
  printf("bmap = %d, imap = %d, inodes_start = %d\n",gp->bg_block_bitmap, gp->bg_inode_bitmap, gp->bg_inode_table);
  bmap = gp->bg_block_bitmap;
  imap = gp->bg_inode_bitmap;
  iblock = gp->bg_inode_table;

  printf("*********** get root inode ***********\n");

  printf("(3). Show root DIR contents\n");
  get_block(fd,buf,iblock);
  ip = (INODE *)buf;
  ip++;
  rootblock = ip->i_block[0];
  printf("root inode data block = %d\n",rootblock);

  show_dir(ip);

  
  printf("hit a key to continue: ");
  getchar();
  printf("tokenize %s\n",pathname);

  tokenize(pathname);

  
  printf("\n");
  printf("\n");
  
  int ino, blk, offset;
  int i = 0;

  for (i=0; i < n; i++){
    ino = search(ip, pname[i]);
        
    if (ino==0){
      printf("can't find %s\n", pname[i]); 
      exit(1);
    }
  
    // Mailman's algorithm: Convert (dev, ino) to INODE pointer
    blk    = (ino - 1) / 8 + iblock; 
    offset = (ino - 1) % 8;        
    get_block(fd, ibuf, blk);
    ip = (INODE *)ibuf + offset;   // ip -> new INODE
  }

  //now ip points at inode of pathname

  printf("\n");
  printf("ino = %d\n",ino);
  printf("size = %d\n",ip->i_size);

  //direct block numbers
  for(int j = 0; j < 14; j++){

    printf("i_block[%d] = %d\n",j,ip->i_block[j]);   
  }
  printf("enter a key to continue: ");
  getchar();
  //indirect block numbers

  get_block(fd,buf,ip->i_block[12]);
  int *up = (int *)buf;
  printf("----------- INDIRECT BLOCKS -----------\n");
  while(up < buf + BLKSIZE){
    if(*up == 0){
      break;
    }
    printf("%d ", *up);
    up++;
  }
  printf("\n");

  //double indirect block numnbers
  printf("----------- DOUBLE INDIRECT BLOCKS -----------\n");
  get_block(fd,dbuf,ip->i_block[13]); 

  for(int u = 0; u < 256; u++)
    {

      get_block(fd,cbuf,dbuf[u]);
      int *op = (int *)cbuf;

      while(op < cbuf + BLKSIZE){

	if(*op != 0){
	  
	  printf("%d ", *op);
	}
	else{

	}
	op++;
      }
    } 
  
  printf("\n");
  
}

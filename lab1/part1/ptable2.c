#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

char buf[512];
int fd;
u32 extended_start_sector = 0,  counter = 1, sector_end = 0, e_end = 0,p4_start_sector = 0, new_start = 0;

struct partition{

  u8 drive;

  u8 head;
  u8 sector;
  u8 cylinder;
  
  u8 sys_type;
  
  u8 end_head;
  u8 end_sector;
  u8 end_cylinder;

  u32 start_sector;
  u32 nr_sectors;
};


int main(int argc, char *argv[], char *env[])
{
  fd = open("vdisk", O_RDONLY); //open vdisk for read
  read(fd, buf, 512); //read MBR(masterboot record) into buf[512]
  struct partition *p = (struct partition *)&buf[0x1be];
  printf("Device\tBoot Start\tEnd\tSectors\tId\n");

 p++;

p++;
  
      sector_end = (p->nr_sectors) + (p->start_sector) - 1; //this will print out end of current sector
      int sector_end = (p->nr_sectors) + (p->start_sector) - 1; 
      printf("vdisk[3]\t%d\t%d\t%d\t\%2x\n",p->start_sector,sector_end,p->nr_sectors,(unsigned int)p->sys_type);

      p++;
    
  p--;
  p4_start_sector = p->start_sector;
  if(p->sys_type == 5) //if type = 5 it is an extended type
   {
     lseek(fd,(long)p->start_sector*512,SEEK_SET); //lseek to p4 begin
     read(fd,buf,512); //read local mbr
     p = (struct partition *)&buf[0x1be]; //access local p table

     while(p->sys_type != 5) //print p table
       {
	 extended_start_sector = p->start_sector + p4_start_sector; //calculate the start sector of the current sector in extended partition
	 e_end = (p->nr_sectors) + extended_start_sector - 1; //calculate end for current sector in extended partition
	 printf("vdisk[%d]\t%d\t%d\t%d\t\%2x\n", counter,extended_start_sector,e_end,p->nr_sectors,(unsigned int)p->sys_type);
	 p++;
	 counter++;
       }
     
     extended_start_sector = p->start_sector + p4_start_sector;
     if(p->sys_type == 5) //if 5 then it is an mbr sector
       {
	 new_start = extended_start_sector;

	 lseek(fd,(long) extended_start_sector*512, SEEK_SET); //lseek to p5 mbr sector
	 read(fd,buf,512); //read local mbr
	 p = (struct partition *)&buf[0x1be]; //access local p table
	 while(p->sys_type != 5)
       {
	 extended_start_sector = p->start_sector + new_start;
	 e_end = (p->nr_sectors) + extended_start_sector - 1;
	 printf("vdisk[%d]\t%d\t%d\t%d\t\%2x\n", counter,extended_start_sector,e_end,p->nr_sectors,(unsigned int)p->sys_type);
	 p++;
	 counter++;
       }
	 extended_start_sector = p->start_sector + p4_start_sector;
	 if(p->sys_type == 5) //if 5 then it is an mbr sector
	   {
	     new_start = extended_start_sector;
	     lseek(fd,(long) extended_start_sector*512, SEEK_SET); //lseek to p6 mbr sector
	     read(fd,buf,512); //read loacl mbr
	     p = (struct partition *)&buf[0x1be]; //access local p table
	     while(p->sys_type != 0) //if sys type is 0 then it is the end of the extended partition
	       {
		 extended_start_sector = p->start_sector + new_start;
		 e_end = (p->nr_sectors) + extended_start_sector - 1;
		 printf("vdisk[%d]\t%d\t%d\t%d\t\%2x\n", counter,extended_start_sector,e_end,p->nr_sectors,(unsigned int)p->sys_type);
		 p++;
		 counter++;
	       } 
	   }	 
       } 
   } 
}


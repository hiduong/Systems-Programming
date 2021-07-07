 #include <stdio.h>
#include <stdio.h>
#include <fcntl.h>

typedef unsigned int u32;
char *ctable = "0123456789ABCDEF";
int BASE = 10;
int BASEHEX = 16;
int BASEOCT = 8;

int prints(char *s)
{
  while(*s != '\0')
    {
      putchar(*s);
      s++;
    }
}

int rpu(u32 x)
{
  char c;
  if(x)
    {
      c = ctable[x %BASE];
      rpu(x / BASE);
      putchar(c);
    }
}

int rpu_hex(u32 x)
{
  char c;
  if(x)
    {
      c = ctable[x %BASEHEX];
      rpu_hex(x / BASEHEX);
      putchar(c);
    }
}

int rpu_oct(u32 x)
{
  char c;
  if(x)
    {
      c = ctable[x % BASEOCT];
      rpu_oct(x / BASEOCT);
      putchar(c);

    }
}

  
int printu(u32 x)
{
  (x==0)?putchar('0'):rpu(x);
}

int printd(int x)
{
  if (x < 0)
    {
      putchar('-');
      printu(-x);
    }
  else
    {
      printu(x);
    }
}

int printx(u32 x)
{
  putchar('0'); putchar('x');
  rpu_hex(x);
}

int printo(u32 x)
{
  putchar('0');
  rpu_oct(x);
}

int myprintf(char *fmt, ...)
{
  char *cp = fmt; //cp points at the format string
  int *ip = &fmt+1;  // ip points to first item to be printed on stack
  while(*cp != '\0')
    {
      if(*cp == '\n')
	{
	  putchar('\n');putchar('\r');
	}
      else if(*cp == '\t')
	{
	  putchar('\t');
	}
      else if(*cp == '%')
	{
	  cp = cp + 1; // will get character after %
	  if(*cp == 'c')
	    { putchar(*ip);}
	  else if(*cp == 's')
	    {
	      prints(*ip);
	    }
	  else if(*cp == 'u')
	    {
	      printu(*ip);
	    }
	  else if(*cp == 'd')
	    {
	      printd(*ip);
	    }
	  else if(*cp  == 'o')
	    {
	      printo(*ip);
	    }
	  else if(*cp == 'x')
	    {
	      printx(*ip);
	    }
	  ip++;
	}
      else
	{
	  putchar(*cp);
	}

      cp++;
    }
}

int main(int argc, char *argv[], char *env[])
{
  myprintf("argc value = %d\n", argc);

  for(int i = 0; i < argc; i++)
    {
      myprintf("argv[%d] = %s\n",i,argv[i]);
    }
  for(int j = 0;env[j] != NULL; j++)
    {
      myprintf("\n%s", env[j]);
    }

  myprintf("\n");
  myprintf("\n");
  
  myprintf("-------------TESTING MYPRINTF---------------\n");
  myprintf("cha=%c string=%s dec=%d hex=%x oct=%o neg=%d\n", 'A', "this is a test", 100, 100, 100, -100);

  myprintf("\n-----------TESTING TAB--------------------\n");
  myprintf("HELLO\t WORLD!\n");
 
}








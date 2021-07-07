#include <stdio.h>
#include <stdlib.h>

int *FP;

int main(int argc, char *argv[], char *env[])
{
  int a,b,c;
  printf("enter main\n");
  printf("&argc=%x argv=%x env=%x\n", &argc, argv, env);
  printf("&a=%8x &b=%8x &c=%8x\n", &a, &b, &c);

  // (1). Write C code to print values of argc and argv[] entries
  int i;
  printf("Value of argc = %d\n", argc);
  for( i =0; i < argc; i++)
    {
      printf("Value of argv[%d] = %s\n",i, argv[i]);
    }

  a=1; b=2; c=3;
  A(a,b);
  printf("exit main\n");

}

int A(int x, int y)
{
  int d,e,f;
  printf("enter A\n");
  // write C code to PRINT ADDRESS OF d,e,f
  printf("-------------Address of d,e,f-----------\n");
  printf("&d=%8x &e=%8x &f=%8x\n", &d, &e, &f);
  
  d=4; e=5; f=6;
  B(d,e);
  printf("exit A\n");
}

int B(int x, int y)
{
  int g,h,i;
  printf("enter B\n");
  //Write C code to PRINT ADDRESS OF g,h,i
  printf("---------Address of g,h,i--------------\n");
  printf("&g=%8x &h=%8x &i=%8x\n", &g, &h, &i);
  
  g=7;h=8;i=9;
  C(g,h);
  printf("exit B\n");
}

int C(int x, int y)
{
  int u, v,w,i,*p;
  printf("enter C\n");
  //Write C code to PRINT ADDRESS OF u,v,w,i,p
  printf("----------Address of u,v,w,i,p----------\n");
  printf("&u=%8x &v=%8x &w=%8x &i=%8x p=%x\n", &u, &v, &w, &i, &p);

  u=10; v=11;w=12;i=13;

  FP = (int *)getebp();
  printf("-----------Stack Frame Linked List------\n");
  // (2) Write C code to print the stack frame link list.
  while (FP != 0)
    {
      printf("%8x => ",FP);
      FP = *FP;
    }
  if (FP == 0)
    {printf("0");}
 
  printf("\n");
  
  p = (int *)&p;

  int q = 0;

  // (3) Print the stack contents from p to the frame of main() YOU MAY JUST PRINT 128 entries of the stack contents.
  printf("-----------STACK CONTENTS--------------\n");
  while(q  < 128)
    {
      printf("%8x\n",*p);
      p++;
      q++;
    }

  // (4) On a hard copy of the print out, identify the stack contents as LOCAL VARIABLES, PARAMETERS, stack frame pointer of each function.
}
  

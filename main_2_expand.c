#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#define BUFF 1<<10
void hChild1();
void hMain();
void hChild2();
  pid_t one;
  pid_t two;
  int main(){
      int Pi[2];
      int result=pipe(Pi);
      if(result==-1)exit(-1);
      if((one=fork())==-1)return -1;
      if(one==0){
        int counter=1;
        close(Pi[0]);
        int sum=0;
        while(1){
          sum+=counter;
          counter++;
          write(Pi[1],&sum,sizeof(sum));
          if(counter==101)exit(0);
        }
      }
      if((two=fork())==-1)return -1;
      if(two==0){
        close(Pi[1]);
        int sum;
        while(1){
         read(Pi[0],&sum,sizeof(sum));
         printf("sum==%d\n",sum);
         if(sum==5050)exit(0);
        }
    }
    int status;
    waitpid(one,&status,0);
    waitpid(two,&status,0);
    printf("process 1 over\n");
    printf("process 2 over\n");
    close(Pi[0]);
    close(Pi[1]);
    return 0;


  }
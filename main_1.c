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

pid_t child_1_pid;//sub-PROCESS_1_pid
pid_t child_2_pid;//sub-PROCESS_2_pid

int  main(){
	int Pi[2];//file descript，Pi[0]:read，Pi[1]:write
	int result=pipe(Pi);//initial pipe without name,success return 0 else return -1,
	if(result==-1)exit(-1);
	 
	alarm(10);
	signal(SIGALRM,hMain);

	signal(SIGINT,hMain);//catch the interruptKey to kill sub_process_1 and sub_process_2
	if((child_1_pid=fork())==-1)return -1;//Creat sub_process_1
	if(child_1_pid==0){//in sub_process_1
		signal(SIGINT,SIG_IGN);//ignore the InterruptKey,cause only caught by the main_process
		signal(SIGUSR1,hChild1);//default kill the process
		char buf[BUFF];//buf
       int counter = 1;//counter
	   close(Pi[0]);//close the pipe 1,no permite to read
    while(1) {
        sleep(1);
        sprintf(buf, "I send you %u times\n", counter);
        write(Pi[1], buf, strlen(buf));//Use The pipe 2 send message to sub_process_2
		counter++;
    }
	
	}
	if((child_2_pid=fork())==-1)return -1;//creat sub_process_2
	if(child_2_pid==0){//in sub_process_2
		signal(SIGINT,SIG_IGN);//ignore the InterruptKey,cause only caught by the main_process
		signal(SIGUSR2,hChild2);//default kill the process
		char buf[BUFF];//buf
       close(Pi[1]);//close the pipe 0 no permite to write
    while (1) {
        memset(buf, '\0', BUFF);
        read(Pi[0], buf, BUFF);
        printf("%s\n", buf);
	}

	}

	 int status_pid;
     waitpid(child_1_pid, &status_pid, 0);//Wait The sub_process_1 finished,main_process accknowledge pid as the number
     waitpid(child_2_pid, &status_pid, 0);//Wait The sub_process_2 finished,main_process accknowledge pid as the number
     printf("Parent Process is Killed!\n");
	 
	close(Pi[0]);//close the pipe
    close(Pi[1]);//close the pipe
    return 0;
}
void hChild1(){//sub_process_1 finish
	printf("Child Process 1 is killed by Parent\n");
	exit(0);
}

void hChild2(){//sub_process_2 finish
	printf("Child Process 2 is killed by Parent\n");
	exit(0);
}
void  hMain(){//main_process finish
	 kill(child_1_pid, SIGUSR1);//Kill the sub_process_1
     kill(child_2_pid, SIGUSR2);//Kill the sub_process_1
}
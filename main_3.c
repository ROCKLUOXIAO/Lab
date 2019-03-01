/*
file name:main_3.c
as the main process
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<fcntl.h>
#include<sys/stat.h>
#define BUFFERNUM 10
#define Buf_size 10
 union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

 struct Buffer_union {//common memory
    char data[10];
	int  len;
};
 int create_Sem(int key,int size);
 void destroy_Sem(int semid);
 int get_Sem(int key,int size);
 void set_N(int semid,int index,int n);
 
 int main(int argc,char **argv){
     int shimd;//共享内存标识符
    int semid;//信号灯标识符
	 if (argc != 3) {
        printf("：./running in.txt out.txt\n");
        return 1;
    }
	pid_t pid_read,pid_write;//process character
	shimd=shmget((key_t)233,10*sizeof(struct Buffer_union),IPC_CREAT|0666);
	 struct Buffer_union *S;
	S=(struct Buffer_union *)shmat(shimd,NULL,SHM_R|SHM_W);
	semid=create_Sem(112,2);
	set_N(semid, 0, 0);//full
    set_N(semid, 1, 10);//length
	if((pid_read=fork())==0){//process one:read
	   printf("process one\n");
	   execl("./read","read",argv[1],NULL);
	   exit(0);
	}
	else if((pid_write=fork())==0){
	   printf("process two\n");
	   execl("./write","write",argv[2],NULL);
	   exit(0);
	}
    int status;
    waitpid(pid_read, &status, 0);
    waitpid(pid_write, &status, 0);
    destroy_Sem(semid);
	shmctl(shimd,IPC_RMID,0);//删除共享内存
    return 0;
}

int create_Sem(int key, int size)//
{
	int id;
	id = semget(key, size, IPC_CREAT|0666);//创建size个信号量,return the IPC 
	if(id ==-1) {//creat failed,return -1
		printf("create sem %d,%d error\n", key, size);//failed to creat.print the error.
	}
	return id;//return IPC of the signal.
}

void destroy_Sem(int semid)
{
	int res = semctl(semid,0,IPC_RMID,0);//从系统中删除信号量,IPC_RMID indicate remove the signal from sys.
	if (res <0) {//判断是否删除成功
		printf("destroy sem %d error\n", semid);//信号量删除失败，输出信息
	}
	return;
}
void set_N(int semid, int index, int n)
{
	union semun semopts; 
	semopts.val = n;//设定SETVAL的值为n
	semctl(semid,index,SETVAL,semopts);//初始化信号量，信号量编号为index
	return;
}

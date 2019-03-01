

//file name :write.c
//as the comsumer

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<fcntl.h>
#include<sys/stat.h>
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};
 struct Buffer_union {//common memory
    char data[10];
    int len;
};

int create_Sem(int key,int size);
 void P(int semid, int index);
 void V(int semid, int index);
int main(int argc,char **argv){
	int shimd;//biashifu
    int semid;//xinhaodeng
    shimd=shmget((key_t)233,10*sizeof(struct Buffer_union),IPC_CREAT|0666);
    semid=create_Sem(112,2);
    struct Buffer_union *messageTail;
    struct Buffer_union BUFF;
	messageTail=(struct Buffer_union *)shmat(shimd,NULL,SHM_R|SHM_W);
    FILE *file=fopen(argv[1],"wb");
     BUFF.len=1;
     int i=0;
     while(1){
        P(semid,0);
       BUFF= messageTail[i];
		V(semid,1); 
		if(BUFF.len==0) break;
        fwrite(BUFF.data,BUFF.len,1,file);
		i++;
		i=i%10;
	}
	fclose(file);
    shmdt(messageTail);
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

void P(int semid, int index)
{
	struct sembuf sem;//信号量操作数组
	sem.sem_num = index;//信号量编号
	sem.sem_op = -1;//信号量操作，-1为P操作
	sem.sem_flg = 0;//操作标记：0或IPC_NOWAIT等
	semop(semid,&sem,1);//1:表示执行命令的个数
	return;
}

void V(int semid, int index)
{
	struct sembuf sem;//信号量操作数组
	sem.sem_num = index;//信号量编号
	sem.sem_op =  1;//信号量操作，1为V操作
	sem.sem_flg = 0; //操作标记
    semop(semid,&sem,1);//1:表示执行命令的个数
	return;
}

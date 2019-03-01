#include<stdio.h>
#include<sys/types.h>
#include<sys/sem.h>
#include <sys/ipc.h>
#include<pthread.h>
 
 int create_Sem(int key,int size);
 void destroy_Sem(int semid);
 int get_Sem(int key,int size);
 void set_N(int semid,int index,int n);
 void P(int semid,int index);
 void V(int semid,int index);
 void pthread_one(void);//thread one ,to sum up from 1 to 100
 void pthread_two(void);//thraed two,to print the consequence during the calculation

 union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};
    int semid;//signal 
    int sum=0;
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

 void pthread_one(void){
     int i=0;
     for(i=1;i<=100;i++){
         P(semid,0);
         printf("thread 1,sum： %d,i： %d\n",sum,i);
         sum+=i;//add from 1 to 100
         V(semid,1);
     }

 } 
 void pthread_two(void){
      while(1){
          P(semid,1);
          printf("Thread 2,sum： %d\n\n",sum);
          if(sum==5050)return;
          V(semid,0);
      }
 }


int main(){
      int key=8200;
      int id;
      pthread_t p1,p2;
      semid=create_Sem(key,2);//Creat two signal;
      set_N(semid,0,1);
      set_N(semid,1,0);
      id=pthread_create(&p1,NULL,(void *)pthread_one,NULL);
      if(id!=0){
          printf("failed to creat thread one!\n");
          return -1;
      }
      id=pthread_create(&p2,NULL,(void *)pthread_two,NULL);
      if(id!=0){
          printf("failed to creat thread two!\n");
          return -1;
      }
      pthread_join(p1,NULL);
      pthread_join(p2,NULL);
      destroy_Sem(semid);
      //destroy_Sem(semid);
      return 0;

}

#include <stdio.h>
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"

#define LEFT  30000000
#define RIGHT 30000200

#define TOTAL_NUM (RIGHT-LEFT+1)
#define THRNUM 4


static int num=0;
static pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;


static void* thr_func(void *p)
{
	int i,j,mark;
	while(1)
	{
		pthread_mutex_lock(&mut);
		//检测是否有任务
		while(num==0)
		{//当前没有任务
			pthread_mutex_unlock(&mut);
			sched_yield();
			pthread_mutex_lock(&mut);
		}
		//任务空
		if(num==-1)
		{
			pthread_mutex_unlock(&mut);
			break;
		
		}
		//抢到任务
		i=num;
		//将num值置位0 告诉上级重新发放任务给线程
		num=0;
		pthread_mutex_unlock(&mut);
		//线程开始任务  临界区任务完成
		//
		mark=1;
		for(j=2;j<i/2;j++)
		{
			if(i%j==0)
			{
				mark=0;
				break;
			}	
		
		}
		if(mark)
		{
		
			printf("thread[%d]:%d\n",(int)p,i);
		
		}
	
	
	
	}
	pthread_exit(NULL);



}


int main()
{
	int i,err;
	pthread_t tid[THRNUM];
	for(i=0;i<THRNUM;i++)
	{
		err=pthread_create(tid+i,NULL,thr_func,(void*)i);
		if(err)
		{
			fprintf(stderr,"pthread_create():%s\n",strerror(err));
			exit(1);
		
		}
	
	
	
	}

	//下发任务
	for(i=LEFT;i<=RIGHT;i++)
	{
		//下发任务时防止线程操作
		pthread_mutex_lock(&mut);
		//检测任务是否被领走
		while(num!=0)//没有领走任务
		{
			pthread_mutex_unlock(&mut);
			sched_yield();
			pthread_mutex_lock(&mut);
		}
		//设置num 下发任务
		 num=i;
		 pthread_mutex_unlock(&mut);
	}
	//下发任务完成
	//设置num=-1 代表任务全部结束
	pthread_mutex_lock(&mut);
	//循环检测最后一个任务是否完成
	while(num!=0)
	{
		pthread_mutex_unlock(&mut);
		sched_yield();
		pthread_mutex_lock(&mut);
	
	}//最后任务已完成
	num=-1;
	pthread_mutex_unlock(&mut);
	//收尸



	for(i=0;i<THRNUM;i++)
		pthread_join(tid[i],NULL);


	pthread_mutex_destroy(&mut);









exit(0);

}

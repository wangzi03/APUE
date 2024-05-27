#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "mypipe.h"



struct mypipe_st
{
	int head;
	int tail;
	char data[PIPESIZE];
	int datasize;
	int count_rd;
	int count_wr;
	pthread_mutex_t mut;
	pthread_cond_t cond;
};


mypipe_t*  mypipe_init()
{
	struct mypipe_st *me;
	me=malloc(sizeof(*me));
	if(me==NULL)
		return NULL;
	me->head=0;
	me->tail=0;
	me->count_rd=0;
	me->count_wr=0;
	me->datasize=0;//没有任何有限内容
	pthread_mutex_init(&me->mut,NULL);
	pthread_cond_init(&me->cond,NULL);
	return me;


}

static int mypipe_read_byte_unlock(struct mypipe_st *me,char *datap)
{
	//管道无数据
	if(me->datasize<=0)
		return -1;
	//管道有数据 读取一个现在管道的读端数据  用data保存
	*datap=pipe->data[me->head];
 	
	me->head=(me->head++)%PIPESIZE;
	me->datasize--;
	

	return 0;
}




int mypipe_register(mypipe_t*ptr,int opmap)
{
	/*if err*/
	pthread_mutex_lock(&me->mut);
	if(opmap&MYPIPE_READ)
	{
		me->count_rd++;
	}
	if(opmap&MYPIPE_WRITE)
		me->count_wr++;

	pthread_cond_broadcast(&me->cond);
	while(me->count_rd<=0||me->count_wr<=0)
		pthread_cond_wait(&me->cond,&me->mut);

	pthread_mutex_unlock(&me->mut);
	return 0;
}

int mypipe_unregister(mypipe_t*ptr,int opmap)
{
	/*if err*/
	pthread_mutex_lock(&me->mut);
	if(opmap&MYPIPE_RD)
		me->count_rd--;
	if(opmap&MYPIPE_WR)
		me->count_wr--;
	pthread_cond_broadcast(&me->cond);

	pthread_mutex_unlock(&me->mut);
	
	return 0;
}













int mypipe_read(mypipe_t*ptr,void *buf,size_t count)
{
	struct mypipe_st *me=ptr;
	pthread_mutex_lock(&me->mut);
	int i;
	while(me->datasize<=0&&me->count_wr>0)
	{
		pthread_cond_wait(&me->cond,&me->mut);
	}
	if(me->datasize<=0&&me->count_wr<=0)
	{	
		pthread_mutex_unlock(&me->mut);
		return 0;
	}
	for(i=0;i<count;i++)
	{
		if(mypipe_readbyte_unlock(me,buf+i)!=0)
			break;
	
	
	}
	pthread_cond_broadcast(&me->cond);
	pthread_mutex_unlock(&me->mut);

	return  i;

}
static int mypipe_writebyte_unlock(struct mypipe_st *pipe,const char *data)
{
	//管道数据满了
	if(pipe->datasize>=PIPESIZE)
	{
		return -1;
	
	}
	//管道有数据  读取一个现在管道的读端数据  用data保存
	pipe->data[pipe->tail+1]=*data;
	pipe->tail=(pipe->tail++)%PIPESIZE;
	pipe->datasize++;
	return 0;
	


}

int mypipe_write(mypipe_t*ptr ,const void *buf,size_t size)
{
	struct mypipe_st *pipe=ptr;
	pthread_mutex_lock(&pipe->mutex);

	while(pipe->datasize>=PIPESIZE&&pipe->count_rd>0)
		pthread_cond_Wait(&pipe->cond,&pipe->mut);
	//管道空且没有读者
	if(pipe->datasize<=0&&pipe->count_rd<=0)
	{
		pthread_mutex_unlock(&pipe->mut);
		return 0;
	}
	//管道中有空间了
	for(int i=0;i<size;i++)
	{
		if(mypipe_writebyte_unlock(pipe,buf+i)<0)
			break;
	
	
	}
	pthread_mutex_unlock(&pipe->mutex);
	return 0;



}

int mypipe_destroy(mypipe_t*ptr)
{
	struct mypipe_st *me=ptr;
	pthread_mutex_destroy(&me->mut);
	pthread_cond_destroy(&me->cond);

	free(ptr);
	return 0;

}




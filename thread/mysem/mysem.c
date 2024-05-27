#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "unistd.h"
#include "string.h"
#include "errno.h"


#include "mysem.h"



struct mysem_st
{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;

};

mysem_t *mysem_init(int initval)
{
	struct mysem_st *sem;
	sem=malloc(sizeof(*sem));


	if(sem==NULL)
	{
		return NULL;
	}
	sem->value=initval;
	pthread_mutex_init(&sem->mutex,NULL);
	pthread_cond_init(&sem->cond,NULL);
	return sem;	
}

int mysem_sub(mysem_t*ptr,int n)
{
	struct mysem_st *sem=ptr;
	pthread_mutex_lock(&sem->mutex);
	while(sem->value<n)
	{
		pthread_cond_wait(&sem->cond,&sem->mutex);
	
	}
	sem->value-=n;
	pthread_mutex_unlock(&sem->mutex);
	return n;
}
int mysem_add(mysem_t *ptr,int n)
{
	struct mysem_st *sem=ptr;
	pthread_mutex_lock(&sem->mutex);
	sem->value+=n;
	pthread_cond_broadcast(&sem->cond);
	pthread_mutex_unlock(&sem->mutex);

	return n;
}



int mysem_destory(mysem_t*ptr)
{

	struct mysem_st *sem=ptr;
	pthread_mutex_destroy(&sem->mutex);
	pthread_cond_destroy(&sem->cond);
	free(sem);
	return 0;




}

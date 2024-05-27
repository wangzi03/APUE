#ifndef RELAYER_H_
#define RELAYER_H_



#define REL_JOBMAX 1000
enum
{
	STATE_RUNNING=1,
	STATE_CANCELED,
	STATE_OVER



};
struct rel_stat_st
{
	int state;
	int fd1;
	int fd2;
	int64_t count12,count21;
	//	struct timerval start,end;

};

int rel_addjob(int fd1,int fd2);
/*
 *return >=0  成功 返回当前任务的id
 *	==-EINVAL   失败 参数非法
 *	==-ENOSPC  失败  任务数组已满
 *	==-ENOMEM  	失败,内存分配有误
 *	
 *
 * */


//int rel_canceljob(int id);
/*
 *	return ==0  成功 指定任务成功取消
 *	 	==-EINVAL  失败   参数非法
 *	 	==-EBUSY 	失败 任务早已被取消
 *	 
 *
 *
 *
 * */

//int rel_waitjob(int id,struct rel_stat_st *);
/*
 *	return ==0 成功 指定任务已终止并且返回状态
 *	      ==-EINVAL 失败  参数非法
 *
 *
 * */

//rel_statjob(int id,struct rel_stat_st *)
/*i
 *return ==0   成功  指定任务状态已经返回
 *      == -EINVAL   失败  参数非法
 *
 *
 *
 * */


#endif

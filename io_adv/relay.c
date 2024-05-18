#include "stdio.h"
#include "stdlib.h"


#define  TTY1  "/dev/tty11"
#define TTY2  "/dev/tty12"
enum
{
	STATE_R=1;
	STATE_W,
	STATE_EX,
	STATE_t


};

struct fsm_st
{

	int state;
	int sfd;
	int dfd;

};


 relay(int fd1,fd2)
{
	int fd1_save,fd2_save;
	fd1_save=fcntl(fd1,F_GETFL);
	fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);

	fd2_save=fcntl(fd2,F_SETFL);
	fnctl(fd2,F_SETFL,fd2_save|O_NONBLOCK);

}


int main()
{

	int fd1,fd2;
	 fd1=open(TTY1,O_RDWR);
	if(fd1<0)
	{
		perror("open()");
		exit(1);
	
	
	}
	fd2=open(TTY2,O_RDWR|O_NONBLOCK);
	if(fd2<0)
	{
		perror("open()");
		exit(1);
	
	}
	relay(fd1,fd2);



	close(fd2);
	close(fd1);





	exit(0);
}

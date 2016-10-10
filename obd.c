#include <stdio.h>
#include <unistd.h>
#include <signal.h>
pid_t pid_temp;
void child_handler(int sig)
{
	if(sig == SIGINT)
		kill(getppid(),SIGUSR1);
	if(sig == SIGQUIT)
		kill(getppid(),SIGUSR2);
	if(sig == SIGUSR1)
	{
		printf("please get off the bus\n");
		kill(getppid(),SIGKILL);
		raise(SIGKILL);
	}

}
void father_handler(int sig)
{
	if(sig == SIGUSR1)
		printf("let's gogogog\n");
	if(sig == SIGUSR2)
		printf("stop the bus\n");
	if(sig == SIGTSTP)
	{
		kill(pid_temp,SIGUSR1);
		//raise(SIGKILL);
	}
}
int main(int argc, const char *argv[])
{
	pid_t p;
	p = fork();
	if(p == 0)//子进程代表售票员
	{
		signal(SIGINT,child_handler);
		signal(SIGQUIT,child_handler);
		signal(SIGUSR1,child_handler);
		signal(SIGTSTP,SIG_IGN);
		while(1)
		{
			pause();
			//printf("im child,im the ticket seller\n");
			//sleep(3);
		}
	}
	else//父进程代表司机
	{
		signal(SIGUSR1,father_handler);
		signal(SIGUSR2,father_handler);
		signal(SIGTSTP,father_handler);
		signal(SIGQUIT,SIG_IGN);
		signal(SIGINT,SIG_IGN);
		pid_temp = p;
		while(1)
		{
			pause();
			//printf("im father,im the bus driver\n");
			//sleep(2);

		}
	}
	return 0;
}

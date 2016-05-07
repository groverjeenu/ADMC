// Class Test Revisited
// Asynchronous Distributed Mean Computation(ADM)

// Objective
// In this question we will compute the mean in a graph of nodes using an synchronous distributed algorithm. 

// Group Details
// Group No: 22
// Member 1: Jeenu Grover (13CS30042)
// Member 2: Ashish Sharma (13CS30043)

// Filename: node.c


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#define MAXN 1000
#define MAXM 1000
#define STR sizeof(int)

int pipe_in[MAXN];
int pipe_out[MAXN];

int list_val[MAXN];
int list_pid[MAXN];

bool isPresent(int pid, int entires)
{
	int i = 0;

	for(i = 0;i<entires;i++)
		if(list_pid[i] == pid)
			return true;
	return false;
}


char* str(int data)
{
	char * s = (char*)malloc(STR*sizeof(char));
	sprintf(s,"%d",data);
	return s;
}

int getInt(int fd)
{
	//printf("WAITING to read\n");
	char * s = (char*)malloc(STR*sizeof(char));
	if(read(fd,s,STR)<0)printf("READ ERROR\n");
	//printf("Read COMPLETE\n");

	return atoi(s);
}

int main(int arc, char* argv[])
{

	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	int val = atoi(argv[3]);

	int mypid = getpid();

	//printf("pid %d done 1\n",mypid);

	int cnt,i,j,k;

	// scanf("%d ",&cnt);
	cnt = getInt(STDIN_FILENO);
	//printf("pid %d val %d, cnt %d\n",mypid,val,cnt);



	//printf("pid %d done 2\n",mypid);

	for(i = 0; i<cnt;i++)
	{
		//scanf("%d ",&pipe_out[i]);
		pipe_out[i] = getInt(STDIN_FILENO);
		//printf("Process %d: %d\tout = %d\n",mypid,i,pipe_out[i]);
		//scanf("%d ",&pipe_in[i]);
		pipe_in[i] = getInt(STDIN_FILENO);
		//printf("Process %d: %d\tin = %d\n",mypid,i,pipe_in[i]);

	}

	int entires = 0, prev = 0,num;
	list_val[entires] = val;
	list_pid[entires] = mypid;
	entires++;
	

	int rval,rpid;
	int sum  = 0,mean = 0;
	sum += val;

	//printf("pid %d done 3\n",mypid);

	for(i = 0 ;i<n;i++)
	{
		
		num = entires-prev;
		//printf("pid %d iter %d\n",mypid,i);

		for(j = 0 ;j <cnt;j++)
		{
			if(write(pipe_out[j], str(num),STR)<0)printf("Write Error\n");
			for(k =0;k<num;k++)
			{
				write(pipe_out[j], str(list_val[prev + k]),STR);
				write(pipe_out[j], str(list_pid[prev + k]),STR);
			}

		}

		//printf("pid %d done 4\n",mypid);

		prev = entires;

		for(j=0 ;j <cnt;j++)
		{
			num  = getInt(pipe_in[j]);
			for(k = 0;k<num;k++)
			{
				rval = getInt(pipe_in[j]);
				rpid = getInt(pipe_in[j]);

				if(!isPresent(rpid,entires))
				{
					//printf("Process %d, val %d, from %d\n",mypid,rval,rpid);

					list_val[entires]=rval;
					list_pid[entires] = rpid;
					sum += rval;
					entires++;
				}
			}

		}

	}

	mean = (sum/n);
	//printf("Process %d: Mean = %d\n",mypid,mean );
	exit(mean);




	return 0;
}
// Class Test Revisited
// Asynchronous Distributed Mean Computation(ADM)

// Objective
// In this question we will compute the mean in a graph of nodes using an synchronous distributed algorithm. 

// Group Details
// Group No: 22
// Member 1: Jeenu Grover (13CS30042)
// Member 2: Ashish Sharma (13CS30043)

// Filename: master.c


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define MAXN 1000
#define MAXM 1000
#define STR sizeof(int)

int pipe_in[MAXN][MAXN][2];

int pipe_out[MAXN][MAXN][2];

int com[MAXN][MAXN][2];

int edge[MAXN][MAXN];

int masterNode[MAXN][2];
int value[MAXN];

int pid[MAXN],val[MAXN];


char* str(int data)
{
	char * s = (char*)malloc(STR*sizeof(char));
	sprintf(s,"%d",data);
	return s;
}


int getInt(int fd)
{
	char * s = (char*)malloc(STR*sizeof(char));
	if(read(fd,s,STR)<0)printf("READ ERROR\n");

	return atoi(s);
}





int main()
{

	int n,m,i,j,k,x,y;

	scanf("%d %d",&n,&m);

	for(i = 0 ;i < n; i++)
	{
		for(j = 0;j< n;j++)
			edge[i][j] = 0;
	}

	for(i = 0;i<m;i++)
	{
		scanf("%d %d",&x,&y);
		x--;
		y--;

		edge[x][y] = 1;
		edge[y][x] = 1;

	}




	for( i = 0;i<n;i++)
	{
		for( j = 0;j<n;j++)
		{
			if(edge[i][j] == 1)
			{
				pipe(com[i][j]);

			}
				

			
		}
	}



	int pid_n;
	printf("Initial values are : ");

	for( i= 0;i<n;i++)
	{
		pipe(masterNode[i]);
		srand(time(NULL)+i);
		value[i] = rand()%100 + 1;
		pid_n = fork();
		if(pid_n == 0)
		{

			dup2(masterNode[i][0],STDIN_FILENO);
			close(masterNode[i][0]);
			close(masterNode[i][1]);
			// for(j = 0;i<n;j++)
			// {
			// 		if(edge[x][y])
			// 		{
			// 			dup2(STDIN_FILENO,pipe_in[x][y][0]);
			// 			dup2(STDOUT_FILENO,pipe_in[y][x][1]);
			// 			close(pipe_in[x][y][1]);
			// 			close(pipe_in[y][x][0]);

			// 			execlp("./node","./node",str(n),str(m),str(value[i]),NULL);
			// 		}
			// }
			execlp("./node","./node",str(n),str(m),str(value[i]),NULL);
			 
		}
		else
		{
			printf("(pid %d, %d); ",pid_n,value[i]);
			close(masterNode[i][0]);

			// char send[10000];
			// char temp[10000];

			int cnt = 0;
			for(k = 0;k<n;k++)
				if(edge[i][k])cnt++;
			//printf("Writing To %d: %d\tcnt = %d\n",pid_n,i,cnt);

			//strcpy(send,"");
			//strcpy
			//sprintf(send,"%d ",cnt);
			write(masterNode[i][1],str(cnt),STR);
			for(k = 0;k<n;k++)
			{
				if(edge[i][k])
				{
					//printf("Writing To %d: %d\tout = %d\n",pid_n,i,com[i][k][1]);
					write(masterNode[i][1],str(com[i][k][1]),STR);
					//printf("Writing To %d: %d\tin = %d\n",pid_n,i,com[i][k][0]);
					write(masterNode[i][1],str(com[k][i][0]),STR);
				}
			}

			

		}

	}
	printf("\n");


	

	int sum = 0;
	for(i = 0;i <n;i++)
		sum += value[i];

	int mean = (sum/n) ;

	int cpid,flag = 0;

	printf("Means received are : ");
	for(i = 0;i<n;i++)
	{
		pid[i] = wait(&val[i]);
		if(WEXITSTATUS(val[i]) != mean)
		{
			flag = 1;
			
		}

		printf("(pid %d, %d); ",pid[i],WEXITSTATUS(val[i]));
	}
	printf("\n");

	if(!flag)
		printf("\n-------------- Successful : Means received are same %d -----------\n",mean);
	else
	{
		printf("\n************** NOT Succesful ***************\n");
	}


	for( i = 0;i<n;i++)
	{
		for( j = 0;j<n;j++)
		{
			if(edge[i][j] == 1)
			{
				close(com[i][j][0]);
				close(com[i][j][1]);
			}
		}
	}


	return 0;
}
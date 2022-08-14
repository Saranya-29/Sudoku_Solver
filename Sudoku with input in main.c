#include <stdio.h>
#include<pthread.h>
#include<string.h>
#include<math.h>

int N=9;
int grid[9][9];  //change the size of the grid as you like

void printgrid(int N)
{
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			printf("%d ",grid[i][j]);
		}
		printf("\n");
	}
}
int check(int row,int col, int num)
{
	for(int i=0;i<N;i++)
	{
		if(grid[row][i]==num)
		return 0;
	}
	for(int j=0;j<N;j++)
	{
		if(grid[j][col]==num)
		return 0;
	}
	int miniRow=row-row%((int)sqrt(N));
	int miniCol=col-col%((int)sqrt(N));
	
	for(int i=0;i<sqrt(N);i++)
	{
		for(int j=0;j<sqrt(N);j++)
		{
			if(grid[i+miniRow][j+miniCol]==num)
			return 0;
		}
	}
	return 1;
}

void* counter(void* s)
{
	int *size=(int*)s;
	for(int i=0;i<*size;i++)
	{
		for(int j=0;j<*size;j++)
		{
			if(grid[i][j]==0)
			{
				int count=0;
				int n=0;
				for(int num=1;num<=*size;num++)
				{
					if(check(i,j,num)==1)
					count++;
					
					if(count>1)
					break;
				}
				if(count==1)
				grid[i][j]=n;
			}
		}
	}
}
int solver(int row,int col)
{
	if(row==N-1 && col==N)
	return 1;
	
	if(col==N)
	{
		row++;
		col=0;
	}
	for(int num=1;num<=N;num++)
	{
		if(check(row,col,num)==1)
		{
			grid[row][col]=num;
			if(solver(row,col+1)==1)
			return 1;
		}
		grid[row][col]=0;
	}
	return 0;
	
}

int main() {
	int sudoku[9][9]={ {3, 0, 6, 5, 0, 8, 4, 0, 0},   //change the input grid in main function as you like
         {5, 2, 0, 0, 0, 0, 0, 0, 0}, 
         {0, 8, 7, 0,  0, 0, 0, 3, 1}, 
         {0, 0, 3, 0, 1, 0, 0, 8, 0}, 
         {9, 0, 0, 8, 6, 3, 0, 0, 5}, 
         {0, 5, 0, 0, 9, 0, 6, 0, 0}, 
         {1, 3, 0, 0, 0, 0, 2, 5, 0}, 
         {0, 0, 0, 0, 0, 0, 0, 7, 4}, 
         {0, 0, 5, 2, 0, 6, 3, 0, 0} };
         
         memcpy(grid,sudoku,N*N*sizeof(int));
         pthread_t th[N*2];
         for(int i=0;i<N*2;i++)
         {
         	pthread_create(&th[i],NULL,counter,&N);
         }
         for(int i=0;i<N*2;i++)
         {
         	pthread_join(th[i],NULL);
         }
         solver(0,0);
         printgrid(N);
	return 0;
}

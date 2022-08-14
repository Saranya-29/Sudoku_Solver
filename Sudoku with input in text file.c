#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void read_grid_from_file(int size, char *ip_file, int grid[36][36]) {  //read the sudoku grid from the file and store it in a grid
	FILE *fp;
	int i, j;
	fp = fopen(ip_file, "r");
	for (i=0; i<size; i++)
		for (j=0; j<size; j++) {
			fscanf(fp, "%d", &grid[i][j]);
	}
}

void print_grid(int size, int grid[36][36]) { //print the final grid 
	int i, j;
	/* The segment below prints the grid in a standard format. Do not change */
	for (i=0; i<size; i++) {
		for (j=0; j<size; j++)
			printf("%d\t", grid[i][j]);
		printf("\n");
	}
}

typedef struct {
  int sudoku[36][36];
  int N, row, col;
}args;

int sud[36][36];



int check(int N, int row, int sudoku[36][36], int num, int col)
{
//check for num in the corresponding row, corresponding column and the corresponding mini grid. If any of the 3 have num already present it means the present num cannot come in the curr cell sud[row][col]
	for (int x = 0; x <= N-1; x++) {   //check columns for a particular number num
		if (sudoku[x][col] == num)
            return 0;
	}

	for (int x = 0; x <= N-1; x++) {  //check in rows for a particular number num
		if (sudoku[row][x] == num)
            return 0;
	}

	int miniRow = row - row % (int)(sqrt(N));  //check in the mini grids for a particular number num
	int miniCol = col - col % (int)(sqrt(N));

 	for (int i = 0; i < (int)(sqrt(N)); i++) {
		for (int j = 0; j < (int)(sqrt(N)); j++) {
			if (sudoku[i + miniRow][j +miniCol] == num)
							return 0;
		}
	}

	return 1; //if num not present in the corresponding row, col, or mini grid of the current cell
}

void *counter (void *s) {   //fill in those cells where only one possibility is there using threads

	int *size = (int *) s;
		for (int i = 0; i <= *size-1; i++) {
			for (int j = 0; j <= *size-1; j++) {
				if (sud[i][j] == 0) {
					int count = 0, n=0;
					for (int num = 1; num <= *size; num++)   //for every cell, check if every num from 1 to 9 (or n) is possible
				    {  
							if (check(*size,i,sud,num,j)==1) {
				            count++;   //for every num, check if the num is possible in the current cell, if possible then increase count
										if (count>1) break;  //if count more than 1 it means that the current cell has more than 1 possibility so break the loop and go to next cell
										n = num;  //if count =1 , it means only 1 element possible in this cell and store it 
								}
				    }
						if (count==1) {
							sud[i][j] = n; //update the current cell to stored value if the count is 1
						}
					}
			}
		}
}


	int solver(int row,  int sudoku[36][36], int col, int N)
	{

	    if (row == N - 1 && col == N)  //the last row and last col+1 , it means successfully the table has been filled so return 1
	        return 1;

	    if (col == N) //if we have reached last column and row is not the last row, we have to go to the first col of next row, that is first cell of next row
	    {
	        row++;
	        col = 0;
	    }
	    if (sudoku[row][col] > 0)  //if the cell has a value greater than 0, it means the cell has been filled already so we directly go to the next row
	       return solver(row,sudoku,col+1,N);

	 	for (int num = 1; num <= N; num++) //if row has not been filled
	    {

		if(check(N,row,sudoku,num,col)==1) //we check for the possibility of every num from 1 to 9 in the current cell using check function, if it is safe or possible for current num, we filled the current cell with num and go to the next cell
	        {
	            sudoku[row][col] = num;
	    				if (solver(row,sudoku,col+1,N)==1) //we solve the remaining grid using the recursive call 
	                return 1;
	        }
	        sudoku[row][col] = 0; //if at any point we are not getting a solution by taking the current cell as num, we backtrack by making the cell back to 0 and start checking the next possible number
	    }
	    return 0;
	}

int main(int argc, char *argv[]) {
	int grid[36][36], N, i, j;

	if (argc != 3) {
		printf("Usage: ./sudoku.out grid_size inputfile");
		exit(-1);
	}

	N = atoi(argv[1]); //size of the sudoku matrix; atoi- string to integer


	read_grid_from_file(N, argv[2], sud);
	/* Do your thing here */


	pthread_t th[N*2];

	for (i=0; i<N*2; i++){
		pthread_create(&th[i], NULL, counter, &N );   // 2*N threads will work on the counter function simultaneously 
	}


	for (i=0; i<N*2; i++){
		pthread_join(th[i],NULL);  //join all the threads after the execution of counter function and then call the solver function to fill the remaining cells in the matrix
	}

	solver(0,sud,0,N);
	print_grid(N, sud);
 	
 	return 0;

}

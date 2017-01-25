/*
 * Sudoku_prog.cpp
 *
 *  Created on: Oct 29, 2016
 *      Author: Sam
 */

#include "Sudoku.h"


int main(int argc, char *argv[])
{
	using namespace std;

    int **Sudoku = createSudokuPuzzle(9, 9);

    sudokuValidation(Sudoku);

    return 0;

}

void sudokuValidation(int** Sudoku)
{
	HANDLE ThreadHandle[THREADS_SIZE];
	parameters* subSetValidation[THREADS_SIZE];

	// define a parameter object for Sudoku row validation
	subSetValidation[0] = (parameters *) malloc(sizeof(parameters));
	subSetValidation[0]->validation = false;
	subSetValidation[0]->sudokuVal = Sudoku;

	// define a parameter object for Sudoku column validation
	subSetValidation[1] = (parameters *) malloc(sizeof(parameters));
	subSetValidation[1]->validation = false;
	subSetValidation[1]->sudokuVal = Sudoku;

	// define 9 parameter objects for Sudoku subgrids validation
	// the starting points of these subgrids are:
	// (0, 0), (0, 3), (0, 6), (3, 0), (3, 3), (3, 6), (6, 0), (6, 3), (6, 6)
	for (int i = 2, j = 0; j < 3; j++)
	{
		for (int k = 0; k < 3; k++)
		{
			subSetValidation[i] = (parameters *) malloc(sizeof(parameters));
			subSetValidation[i]->row = j * 3;
			subSetValidation[i]->col = k * 3;
			subSetValidation[i]->validation = false;
			subSetValidation[i]->sudokuVal = Sudoku;
			i++;
		}
	}

	/* create the thread */
	ThreadHandle[0] = CreateThread(0, 0, sudokuRowValidation, (void *)subSetValidation[0], 0, NULL);
	ThreadHandle[1] = CreateThread(0, 0, sudokuColValidation, (void *)subSetValidation[1], 0, NULL);

	for (int i = 0; i < 9; i++)
	{
		ThreadHandle[i + 2] = CreateThread(0, 0, sudokuSubgridValidation, (void *)subSetValidation[i + 2], 0, NULL);
	}

	// Wait until all threads have terminated.
    WaitForMultipleObjects(THREADS_SIZE, ThreadHandle, TRUE, INFINITE);

	/* close the thread handle */
    for (int i = 0; i < THREADS_SIZE; i++)
    {
    	CloseHandle(ThreadHandle[i]);
    }

	if (subSetValidation[0]->validation && subSetValidation[1]->validation
			&& subSetValidation[2]->validation
			&& subSetValidation[3]->validation
			&& subSetValidation[4]->validation
			&& subSetValidation[5]->validation
			&& subSetValidation[6]->validation
			&& subSetValidation[7]->validation
			&& subSetValidation[8]->validation
			&& subSetValidation[9]->validation
			&& subSetValidation[10]->validation)
	{
		printf("valid Sudoku!");
	}
	else
	{
		printf("invalid Sudoku!");
	}
}

int** createSudokuPuzzle(unsigned height, unsigned width)
{
	int validSudoku[9][9] = {
				{5, 3, 4, 6, 7, 8, 9, 1, 2},
				{6, 7, 2, 1, 9, 5, 3, 4, 8},
				{1, 9, 8, 3, 4, 2, 5, 6, 7},
				{8, 5, 9, 7, 6, 1, 4, 2, 3},
				{4, 2, 6, 8, 5, 3, 7, 9, 1},
				{7, 1, 3, 9, 2, 4, 8, 5, 6},
				{9, 6, 1, 5, 3, 7, 2, 8, 4},
				{2, 8, 7, 4, 1, 9, 6, 3, 5},
				{3, 4, 5, 2, 8, 6, 1, 7, 9}
			};

	int** Sudoku = 0;
	Sudoku = new int*[height];

	for (unsigned h = 0; h < height; h++)
	{
		Sudoku[h] = new int[width];

		for (unsigned w = 0; w < width; w++)
		{
			  Sudoku[h][w] = validSudoku[h][w];
		}
	}

	return Sudoku;
}


// Row validation function
DWORD WINAPI sudokuRowValidation(LPVOID lpParameter)
{
	parameters *data = (parameters*) lpParameter;

	for(int i = 0; i < 9; i++)
	{
		int marker [9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

		for(int j = 0; j < 9; j++)
		{
			int temp = data->sudokuVal[i][j];
			marker[temp - 1] = marker[temp - 1] + 1;
		}

		for(int i = 0; i < 9; i++)
		{
			if (marker[i] != 1)
			{
				data->validation = false;
				return -1;
			}
		}
	}
	data->validation = true;
	return 0;
}


// Column validation function
DWORD WINAPI sudokuColValidation(LPVOID lpParameter)
{
	parameters *data = (parameters*) lpParameter;

	for(int i = 0; i < 9; i++)
	{
		int marker [9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

		for(int j = 0; j < 9; j++)
		{
			int temp = data->sudokuVal[j][i];
			marker[temp - 1] = marker[temp - 1] + 1;
		}

		for(int i = 0; i < 9; i++)
		{
			if (marker[i] != 1)
			{
				data->validation = false;
				return -1;
			}
		}
	}
	data->validation = true;
	return 0;
}


// Subgrids validation function
DWORD WINAPI sudokuSubgridValidation(LPVOID lpParameter)
{
	parameters *data = (parameters*) lpParameter;

	int marker [9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int row = data->row;
	int col = data->col;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			int temp = data->sudokuVal[row + i][col + j];
			marker[temp - 1] = marker[temp - 1] + 1;
		}
	}

	for(int i = 0; i < 9; i++)
	{
		if (marker[i] != 1)
		{
			data->validation = false;
			return -1;
		}
	}
	data->validation = true;
	return 0;
}










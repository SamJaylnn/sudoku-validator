/*
 * Sudoku.h
 *
 *  Created on: Oct 29, 2016
 *      Author: Sam
 */

#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <windows.h>
#include <iostream>
#include <stdio.h>

#define THREADS_SIZE 11

//generate a sudoku puzzle
int** createSudokuPuzzle(unsigned height, unsigned width);
void sudokuValidation(int** Sudoku);

/* the thread runs in this separate function */
DWORD WINAPI sudokuRowValidation(LPVOID lpParameter);
DWORD WINAPI sudokuColValidation(LPVOID lpParameter);
DWORD WINAPI sudokuSubgridValidation(LPVOID lpParameter);

typedef struct
{
	int row;
	int col;
	bool validation;
	int **sudokuVal;
}parameters;



#endif /* SUDOKU_H_ */

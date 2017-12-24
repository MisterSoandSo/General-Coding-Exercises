// Matrix Reduction Version 2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <iomanip>

using namespace std;

float **Matrix;
int row_x, col_y;
int pivot_point_x = 0, pivot_point_y = 0;

void pause()
{
	cin.sync(); // Flush The Input Buffer Just In Case
	cin.ignore(); // There's No Need To Actually Store The Users Input
}

void printMat()
{
	// printing of element for matrix
	cout << endl << "Now printing the element for the matrix..." << endl;
	for (int i = 0; i < row_x; i++)
	{
		for (int j = 0; j < col_y; j++)
		{
			cout << Matrix[i][j] << "   ";
		}
		cout << endl;
	}
}

void inputMat()
{
	// input element for matrix
	cout << endl << "Now enter the element for the matrix..." << endl;
	for (int i = 0; i < row_x; i++)
	{
		for (int j = 0; j < col_y; j++)
		{
			cout << "Row " << (i + 1) << " Col " << (j + 1) << " :";
			cin >> Matrix[i][j];
		}
	}
}

void buildMat()
{
	//Ask user for Matrix size
	cout << " Please input row size: ";
	cin >> row_x;
	cout << " Please input column size: ";
	cin >> col_y;

	// dynamically allocate an array
	Matrix = new float *[row_x];
	for (int count = 0; count < row_x; count++)
	{
		Matrix[count] = new float[col_y];
	}
}

void delMat()
{
	// free dynamically allocated memory
	for (int i = 0; i < row_x; i++)
	{
		delete[] Matrix[i];
	}
	delete[] Matrix;	// done 
}

bool swap()
{
	int swi = pivot_point_x;
	float temp_a;

	for (int i = pivot_point_x + 1; i < row_x; i++)	//find out which row to swap with that is valid
	{
		if (Matrix[i][pivot_point_y] != 0)
		{
			swi = i;
			break;
		}
		else if (Matrix[row_x - 1][pivot_point_y] == 0 && i == row_x - 1)
		{
			pivot_point_y += 1;		// shift over to the next col		
			return false;					////find pivot again
		}
	}
	for (int l = 0; l < col_y;l++)	//switch rows
	{
		temp_a = Matrix[pivot_point_x][l];
		Matrix[pivot_point_x][l] = Matrix[swi][l];
		Matrix[swi][l] = temp_a;
	}
	
	return true;
}

void find_pivot()
{
	if (Matrix[pivot_point_x][pivot_point_y] != 0)	//1 or greater
	{
		if (Matrix[pivot_point_x][pivot_point_y] != 1)
		{
			//greater than 1
			//if (modulo_check(Matrix[pivot_point_x][pivot_point_y], pivot_point_x))
			//{
				//if true ... accuracy will not be lost.
				float pivot_value = Matrix[pivot_point_x][pivot_point_y];
				for (int i = 0; i < col_y;i++)
					if (Matrix[pivot_point_x][i] != 0 && pivot_value != 0)
						Matrix[pivot_point_x][i] /= pivot_value;
			//}
			//if mudulo return false

		}
		return;
		// continue with row reduction
	}
	else if (Matrix[pivot_point_x][pivot_point_y] == 0) // is 0
	{
		if (pivot_point_x == row_x - 1)
		{
			cout << "Matrix has no soultion" << endl;
			return;
		}
		if (!swap()) find_pivot();	//if true swap complete
									//if false, find pivot again with new y pivot value
	}
}

void reduce_row()
{
	for (int row = pivot_point_x + 1; row < row_x; row++)
	{
		if (Matrix[row][pivot_point_y] != 0)
		{
			float temp_value = Matrix[row][pivot_point_y];
			for (int j = 0; j < col_y; j++)
			{
				Matrix[row][j] = Matrix[row][j] - temp_value*Matrix[pivot_point_x][j];
			}
		}
	}
}

int main()
{
	//initial build dynamic matrix
	buildMat();
	inputMat();
	printMat();

	//cycle thru entire matrix to find Echlon form
	while (pivot_point_x < row_x)
	{
		find_pivot();
		if (pivot_point_x != row_x - 1)
			reduce_row();	//assuming pivot found is the value of 1

		pivot_point_x += 1;	//increment pivot points
		pivot_point_y += 1;
	}
	cout << "---------  Matrix Echlon Form  ---------" << endl;
	printMat();

	pause();	pause();	//temporary hold display 

							//dealocate dynamic memory
	delMat();


	return 0;
}

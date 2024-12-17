///scanf_s has a increased security for characters/strings/arrays.
///We don't use scanf for characters/strings/arrays so it's really don't matter.
#define USE_SCANF_S

#define VALUE_TYPE int
#define VALUE_TYPE_SPECIFIER "%d"
#define COUNTER_TYPE int
#define COUNTER_TYPE_SPECIFIER "%d"

///1 for Laplace Expansion
///2 for Leibniz Formula
///3 for Gaussian Elimination
#define DETERMINANT_ALG 2

///0 for False
///1 for True
#define USE_MATH_LIB_FOR_GAUSSIAN 1

#include <stdio.h>
#include <stdlib.h>

#if DETERMINANT_ALG == 3
#if USE_MATH_LIB_FOR_GAUSSIAN == 1
#include <math.h>
#else
double fabs(double x)
{
	if(x < 0){
		return -x;
	}
	return x;
}
#endif
#endif

/// @brief It's get defined VALUE_TYPE from Input
/// @return defined VALUE_TYPE
VALUE_TYPE GetValueTypeInputRaw()
{
	VALUE_TYPE value;
	#if defined(USE_SCANF_S)
		scanf_s(VALUE_TYPE_SPECIFIER,&value);
	#else
		scanf(VALUE_TYPE_SPECIFIER, &value);
	#endif
	return value;
}

/// @brief It's get defined COUNTER_TYPE from Input
/// @return defined COUNTER_TYPE
COUNTER_TYPE GetCounterInputRaw()
{
	COUNTER_TYPE value;
	#if defined(USE_SCANF_S)
		scanf_s(COUNTER_TYPE_SPECIFIER,&value);
	#else
		scanf(COUNTER_TYPE_SPECIFIER, &value);
	#endif
	return value;
}

/// @brief It's get defined COUNTER_TYPE from Input with Question (print)
/// @param question A text which printed before input scan
/// @return defined COUNTER_TYPE
COUNTER_TYPE GetCounterInput(char* question)
{
	printf(question);
	return GetCounterInputRaw();
}

/// @brief Allocating VALUE_TYPE Array which represented as Matrix
/// @param row Row Count of the Matrix which is wanted to allocate
/// @param column Column Count of the Matrix which is wanted to allocate
/// @return VALUE_TYPE Array as pointer
VALUE_TYPE* AllocateMatrix(COUNTER_TYPE row, COUNTER_TYPE column)
{
    //It's getting space according to (element count * element size)
    //Element count can be calculated as (Row * Column)
    //We are used calloc other than malloc because it's initializing data (with zeros)
	VALUE_TYPE* matrix = (VALUE_TYPE*)calloc(row * column, sizeof(VALUE_TYPE));

	return matrix;
}

/// @brief Calculating the Index of VALUE_TYPE in VALUE_TYPE Array (which is our Matrix)
/// @param currentRow Current Row
/// @param column Column Count of Matrix
/// @param currentColumn Current Column
/// @return (currentRow * column) + currentColumn as a COUNTER_TYPE
COUNTER_TYPE CalculateIndex(COUNTER_TYPE currentRow, COUNTER_TYPE column, COUNTER_TYPE currentColumn)
{
	COUNTER_TYPE value = (currentRow * column) + currentColumn;
	return value;
}

/// @brief Filling the Matrix according to inputs
/// @param matrix Matrix which is a VALUE_TYPE Array as pointer
/// @param row Row Count of Matrix
/// @param column Column Count of Matrix
void ScanMatrix(VALUE_TYPE* matrix, COUNTER_TYPE row, COUNTER_TYPE column)
{
	COUNTER_TYPE _row, _column; //Pre-initialize for variables because before C99 you can't use for initializers
	for(_row = 0; _row < row; ++_row){
		for(_column = 0; _column < column; ++_column){
			printf("Input Value (%d, %d): ",_row + 1, _column + 1);
			VALUE_TYPE value = GetValueTypeInputRaw();
			
			matrix[CalculateIndex(_row,column,_column)] = value;
		}
	}
}

/// @brief Printing the Matrix
/// @param matrix Matrix which is a VALUE_TYPE Array as pointer
/// @param row Row Count of Matrix
/// @param column Column Count of Matrix
void PrintMatrix(VALUE_TYPE* matrix, COUNTER_TYPE row, COUNTER_TYPE column)
{
	COUNTER_TYPE _row, _column; //Pre-initialize for variables because before C99 you can't use for initializers
    for(_row = 0; _row < row; ++_row)
	{
		for(_column = 0; _column < column; ++_column)
		{
			printf(VALUE_TYPE_SPECIFIER,matrix[CalculateIndex(_row,column,_column)]);
		}
	}
}

/// @brief Fill Sub Rectangle Matrix according to Main Rectangle Matrix
/// @param matrix Main Matrix
/// @param subMatrix Allocated Sub Matrix which wanted too filled up
/// @param size Size Of The Matrix
/// @param excludeRow Excluded Row
/// @param excludeCol Excluded Column
void GetSubMatrix(VALUE_TYPE* matrix, VALUE_TYPE* subMatrix, COUNTER_TYPE size, COUNTER_TYPE excludeRow, COUNTER_TYPE excludeCol) 
{
    COUNTER_TYPE NewSize = size - 1;
	COUNTER_TYPE row = 0, col = 0;
	COUNTER_TYPE _row, _col;
    for (_row = 0; _row < size; ++_row) 
	{
        if (_row == excludeRow) 
		{
			continue;
		}
        for (_col = 0; _col < size; ++_col) 
		{
            if (_col == excludeCol)
			{
				continue;
			}
            subMatrix[CalculateIndex(row, NewSize, col)] = matrix[CalculateIndex(_row, size, _col)];
            col++;
            if (col == NewSize) 
			{
                col = 0;
                row++;
            }
        }
    }
}

#if DETERMINANT_ALG == 1
/// @brief Calculating Determinant of Rectangle Matrixes
/// @param matrix Matrix which is a VALUE_TYPE Array as pointer
/// @param size Size of the Matrix for example 1 for 1x1, 2 for 2x2, 3 for 3x3 etc.
/// @return Determinant of Matrix (it's scalar) as Pointer or NULL if error accoured
VALUE_TYPE* DeterminateRectangleMatrix(VALUE_TYPE* matrix, COUNTER_TYPE size)
{	
	//Determinant of the 1x1 matrix is equal to itself
	if (size == 1) 
	{
        return &matrix[0]; 
    }

	//We allocate it because we want to return it's pointer
	//If we don't allocate it, it will be removed from stack in function end
	//And when we try to access this pointer from other wheres it will be throw error
	VALUE_TYPE* determinant = (VALUE_TYPE*)malloc(sizeof(VALUE_TYPE));

	//2x2 Matrix Determinant
	//Actually we don't need this condition, for loop can process 2x2 Matrixes too
    if (size == 2) 
	{
		*determinant = (matrix[0] * matrix[3]) - (matrix[1] * matrix[2]);
        return determinant;
    }

    
    VALUE_TYPE* subMatrix = (VALUE_TYPE*)malloc((size - 1) * (size - 1) * sizeof(VALUE_TYPE));

	COUNTER_TYPE col; //Pre-initialize for variables because before C99 you can't use for initializers
    for (col = 0; col < size; col++) 
	{
        GetSubMatrix(matrix, subMatrix, size, 0, col);
        VALUE_TYPE* subDeterminant = DeterminateRectangleMatrix(subMatrix, size - 1);
		if(subDeterminant == NULL)
		{
			free(subMatrix);
			free(determinant);
			return NULL;
		}
        *determinant += (col % 2 == 0 ? 1 : -1) * matrix[col] * (*subDeterminant);
    }

    free(subMatrix);
    return determinant;
}
#elif DETERMINANT_ALG == 2
/// @brief Determines the sign of a permutation by counting the number of inversions. An inversion occurs when a pair of indices (i, j) satisfies i < j and perm[i] > perm[j]
/// @param perm Pointer to an array representing the permutation
/// @param n The size of the permutation (number of elements)
/// @return Returns 1 if the number of inversions is even (even permutation), or -1 if the number of inversions is odd (odd permutation)
COUNTER_TYPE PermutationSign(COUNTER_TYPE* perm, COUNTER_TYPE n) 
{
    COUNTER_TYPE inversions = 0;

	// Count the number of inversions
	COUNTER_TYPE i; //Pre-initialize for variables because before C99 you can't use for initializers
    for (i = 0; i < n; i++) 
	{
		COUNTER_TYPE j; //Pre-initialize for variables because before C99 you can't use for initializers
        for (j = i + 1; j < n; j++) 
		{
            if (perm[i] > perm[j]) 
			{
                inversions++;
            }
        }
    }

	// Return the sign: 1 for even inversions, -1 for odd inversions
    return (inversions % 2 == 0) ? 1 : -1;
}

/// @brief Generates all permutations of row indices recursively and computes the determinant of an n x n matrix using the Leibniz formula
/// @param perm Pointer to an array of row indices to be permuted
/// @param used Pointer to an array that tracks whether a row index has been used
/// @param n The dimension of the square matrix (number of rows/columns)
/// @param depth The current depth of recursion (index position being filled)
/// @param matrix Pointer to the n x n matrix elements stored in a 1D array (row-major order)
/// @param determinant Pointer to a variable that accumulates the determinant value
void GeneratePermutations(COUNTER_TYPE* perm, COUNTER_TYPE* used, COUNTER_TYPE n, COUNTER_TYPE depth, VALUE_TYPE* matrix, VALUE_TYPE* determinant) 
{
    // Static variable to store the current permutation
	static COUNTER_TYPE* currentPerm = NULL;

	// On first call (non recrusive call) allocate currentPerm
	if(depth == 0)
	{
		//It will be NULL because it's NULL before call and freed after every completed call
		currentPerm = (COUNTER_TYPE*)malloc(n * sizeof(COUNTER_TYPE));
	}

	COUNTER_TYPE i; //Pre-initialize for variables because before C99 you can't use for initializers

	// Base case: If the permutation is complete
    if (depth == n) 
	{
        VALUE_TYPE product = 1;
        for (i = 0; i < n; i++) 
		{
            product *= matrix[i * n + currentPerm[i]];
        }
        *determinant += PermutationSign(currentPerm, n) * product;

		// Freeing currentPerm
		free(currentPerm);
		currentPerm = NULL;
        return;
    }

	// Recursive case: Generate permutations
    for (i = 0; i < n; i++) 
	{
        // Check if the index has not been used yet
		if (!used[i]) 
		{
            // Mark the index as used
			used[i] = 1; 
			// Add the current index to the permutation
            currentPerm[depth] = perm[i];
			// Recurse to fill the next position
            GeneratePermutations(perm, used, n, depth + 1, matrix, determinant);
			// Backtrack: Mark the index as unused
            used[i] = 0;
        }
    }
}

/// @brief Calculating Determinant of Rectangle Matrixes
/// @param matrix Matrix which is a VALUE_TYPE Array as pointer
/// @param size Size of the Matrix for example 1 for 1x1, 2 for 2x2, 3 for 3x3 etc.
/// @return Determinant of Matrix (it's scalar) as Pointer or NULL if error accoured
VALUE_TYPE* DeterminateRectangleMatrix(VALUE_TYPE* matrix, COUNTER_TYPE size) 
{
    VALUE_TYPE* determinant = (VALUE_TYPE*)malloc(sizeof(VALUE_TYPE));
    COUNTER_TYPE* perm = (COUNTER_TYPE*)malloc(size * sizeof(COUNTER_TYPE));
    COUNTER_TYPE* used = (COUNTER_TYPE*)calloc(size, sizeof(COUNTER_TYPE));

	COUNTER_TYPE i; //Pre-initialize for variables because before C99 you can't use for initializers
    for (i = 0; i < size; i++) 
	{
        perm[i] = i;
    }

    GeneratePermutations(perm, used, size, 0, matrix, determinant);

    free(perm);
    free(used);
    return determinant;
}
#elif DETERMINANT_ALG == 3
/// @brief Swapping Rows
/// @param matrix Matrix which is a VALUE_TYPE Array as pointer
/// @param size Size of the Matrix for example 1 for 1x1, 2 for 2x2, 3 for 3x3 etc.
/// @param row1 Row 1 Index, Start From 0
/// @param row2 Row 2 Index, Start From 0
void SwapRows(VALUE_TYPE* matrix, COUNTER_TYPE size, COUNTER_TYPE row1, COUNTER_TYPE row2) 
{
	COUNTER_TYPE i; //Pre-initialize for variables because before C99 you can't use for initializers
    for (i = 0; i < size; i++) 
	{
        VALUE_TYPE temp = matrix[row1 * size + i];
        matrix[row1 * size + i] = matrix[row2 * size + i];
        matrix[row2 * size + i] = temp;
    }
}

/// @brief Calculating Determinant of Rectangle Matrixes
/// @param matrix Matrix which is a VALUE_TYPE Array as pointer
/// @param size Size of the Matrix for example 1 for 1x1, 2 for 2x2, 3 for 3x3 etc.
/// @return Determinant of Matrix (it's scalar) as Pointer or NULL if error accoured
VALUE_TYPE* DeterminateRectangleMatrix(VALUE_TYPE* matrix, COUNTER_TYPE size) 
{
    VALUE_TYPE* determinant = (VALUE_TYPE*)malloc(sizeof(VALUE_TYPE));
    VALUE_TYPE* tempMatrix = (VALUE_TYPE*)malloc(size * size * sizeof(VALUE_TYPE));

    // Copying original matrix
	COUNTER_TYPE i; //Pre-initialize for variables because before C99 you can't use for initializers
    for (i = 0; i < size * size; i++) 
	{
        tempMatrix[i] = matrix[i];
    }

	COUNTER_TYPE k; //Pre-initialize for variables because before C99 you can't use for initializers
    for (k = 0; k < size; k++) 
	{
        // Find the Maximum Absolute Value and Change the Row
        COUNTER_TYPE maxRow = k;
        for (COUNTER_TYPE i = k + 1; i < size; i++) 
		{
            if (fabs(tempMatrix[i * size + k]) > fabs(tempMatrix[maxRow * size + k])) 
			{
                maxRow = i;
            }
        }
        if (maxRow != k) 
		{
            SwapRows(tempMatrix, size, k, maxRow);
            *determinant *= -1; // Changing row will change sign
        }

		// If main element is zero determinant is zero
        if (tempMatrix[k * size + k] == 0.0) 
		{
            free(tempMatrix);
			*determinant = 0;
            return determinant;
        }

        // Update sub rows
        for (i = k + 1; i < size; i++) 
		{
            VALUE_TYPE factor = tempMatrix[i * size + k] / tempMatrix[k * size + k];
            COUNTER_TYPE j; //Pre-initialize for variables because before C99 you can't use for initializers
			for (j = k; j < size; j++) 
			{
                tempMatrix[i * size + j] -= factor * tempMatrix[k * size + j];
            }
        }
    }

    // Main Diagonal Product - Determinant
    for (i = 0; i < size; i++) 
	{
        *determinant *= tempMatrix[i * size + i];
    }

    free(tempMatrix);
    return determinant;
}
#endif

VALUE_TYPE* DeterminateMatrix(VALUE_TYPE* matrix, COUNTER_TYPE row, COUNTER_TYPE column)
{
	if(row != column)
	{
		printf("Only rectangle matrixes (like 2x2, 3x3, 4x4, NxN) can be calculated.");
		return NULL;
	}

	if(row <= 0)
	{
		printf("Matrix can't have negative size.");
		return NULL;
	}

	return DeterminateRectangleMatrix(matrix, row);
}

/// @brief Main Function, Entrypoint of the App
/// @return Exit Code, 0 for succesfull 
int main()
{
	while(1 == 1) //Infinite Loop, before C99 there is no bool definition so condition used
	{
		COUNTER_TYPE row = GetCounterInput("Input Row Count: ");
		COUNTER_TYPE column = GetCounterInput("Input Column Count: ");
		
		/* 
		//We can warn user from start
		if(row != column){
			printf("Only rectangle matrixes (like 2x2, 3x3, 4x4, NxN) can be calculated.");
			continue;
		}
		*/

		VALUE_TYPE* matrix = AllocateMatrix(row, column);
		
		ScanMatrix(matrix, row, column);
		
		PrintMatrix(matrix, row, column);
		
		VALUE_TYPE* determinant = DeterminateMatrix(matrix, row, column);
		
		free(matrix); //We don't need it more

		if(determinant == NULL)
		{
			printf("Matrix Determinant Calculation not finished succesfully, error accoured");
			continue;
		}

		printf("Determinant of the matrix is ");
		printf(VALUE_TYPE_SPECIFIER, *determinant);
		printf("\n");

		free(determinant); //We don't need it more
	}
}
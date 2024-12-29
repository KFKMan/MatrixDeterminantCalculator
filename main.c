#define NewLineSpecifier "\n"

///scanf_s has a increased security for characters/strings/arrays.
///We don't use scanf for characters/strings/arrays so it's really don't matter.
#define USE_SCANF_S

#define VALUE_TYPE int
#define VALUE_TYPE_SPECIFIER "%lf"
#define COUNTER_TYPE int
#define COUNTER_TYPE_SPECIFIER "%d"

///1 for Laplace Expansion
///2 for Leibniz Formula
///3 for Gaussian Elimination (there is a divide operation because of that you need to use floating types like double, float etc.)
#define DETERMINANT_ALG 1

///0 for False
///1 for True
#define USE_MATH_LIB_FOR_GAUSSIAN 1

#define USE_STOPWATCH 1

#if USE_STOPWATCH == 1
#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>

const int Algorithm = DETERMINANT_ALG;

void PrintAlgorithm(){
	if(Algorithm == 1){
		printf("Selected Algorithm is Laplace Expansion \n");
	}
	else if(Algorithm == 2){
		printf("Selected Algorithm is Leibniz Formula \n");
	}
	else if (Algorithm == 3)
	{
		printf("Selected Algorithm is Gaussian Elimination \n");
	}
	else{
		printf("Unknow Algorithm Detected \n");
	}
}

#if DETERMINANT_ALG == 3
#if USE_MATH_LIB_FOR_GAUSSIAN == 1
#include <math.h>
#else
double fabs(double x)
{
	if(x < 0)
	{
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
/// @return VALUE_TYPE Array as or NULL if error accoured
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
		printf("| ");
		for(_column = 0; _column < column; ++_column)
		{
			printf(VALUE_TYPE_SPECIFIER,matrix[CalculateIndex(_row,column,_column)]);
			printf(" | ");
		}
		printf(NewLineSpecifier);
	}
}

/// @brief Fill Sub Rectangle Matrix according to Main Rectangle Matrix
/// @param matrix Main Matrix
/// @param subMatrix Allocated Sub Matrix which wanted too filled up
/// @param size Size Of The Matrix N for NxN Matrix
/// @param excludeRow Excluded Row Index (start from 0)
/// @param excludeCol Excluded Column Index (start from 0)
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
	//We allocate it because we want to return it's pointer
	//If we don't allocate it, it will be removed from stack in function end
	//And when we try to access this pointer from other wheres it will be throw error
	VALUE_TYPE* determinant = (VALUE_TYPE*)malloc(sizeof(VALUE_TYPE));
	if(!determinant){
		return NULL;
	}
	*determinant = 0;

	//Determinant of the 1x1 matrix is equal to itself
	if (size == 1) 
	{
		*determinant = matrix[0];
		return determinant;
    }

	//2x2 Matrix Determinant
	//Actually we don't need this condition, for loop can process 2x2 Matrixes too
    if (size == 2) 
	{
		*determinant = (matrix[0] * matrix[3]) - (matrix[1] * matrix[2]);
        return determinant;
    }

    
    VALUE_TYPE* subMatrix = (VALUE_TYPE*)malloc((size - 1) * (size - 1) * sizeof(VALUE_TYPE));
	if(!subMatrix){
		return NULL;
	}

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

/// @brief Get Sign of Permutation
/// @param perm Array (n=size) for storing permutations
/// @param size Size of the Matrix for example 1 for 1x1, 2 for 2x2, 3 for 3x3 etc.
/// @return Sign of Permutation, if even +1 else -1
int signOfPermutation(COUNTER_TYPE *perm, int size) {
    int inversions = 0;
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (perm[i] > perm[j]) {
                inversions++;
            }
        }
    }
    return (inversions % 2 == 0) ? 1 : -1;
}

/// @brief Create Permutations and Calculate Determinant with them
/// @param matrix Matrix which is a VALUE_TYPE Array as pointer
/// @param perm Array (n=size) for storing permutations
/// @param used Array (n=size) with for determining used or not, use calc or fill it with zeros
/// @param depth It's for recursing set it zero
/// @param size Size of the Matrix for example 1 for 1x1, 2 for 2x2, 3 for 3x3 etc.
/// @param det For setting determinant value
void permute(VALUE_TYPE *matrix, COUNTER_TYPE *perm, COUNTER_TYPE *used, COUNTER_TYPE depth, COUNTER_TYPE size, VALUE_TYPE *det) {
    if (depth == size) {
        int product = 1;
        for (int i = 0; i < size; i++) {
            product *= matrix[i * size + perm[i]];
        }
        *det += signOfPermutation(perm, size) * product;
    } else {
        for (int i = 0; i < size; i++) {
            if (!used[i]) {
                used[i] = 1;
                perm[depth] = i;
                permute(matrix, perm, used, depth + 1, size, det);
                used[i] = 0;
            }
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
	if(!determinant){
		return NULL;
	}
	*determinant = 0;
    COUNTER_TYPE* perm = (COUNTER_TYPE*)malloc(size * sizeof(COUNTER_TYPE));
	if(!perm){
		free(determinant);
		return NULL;
	}
    COUNTER_TYPE* used = (COUNTER_TYPE*)calloc(size, sizeof(COUNTER_TYPE));
	if(!used){
		free(determinant);
		free(perm);
		return NULL;
	}

    permute(matrix, perm, used, 0, size, determinant);

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
	if(!determinant){
		return NULL;
	}
	*determinant = 1;

    VALUE_TYPE* tempMatrix = (VALUE_TYPE*)malloc(size * size * sizeof(VALUE_TYPE));
	if (!tempMatrix)
    {
        free(determinant);
        return NULL;
    }

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
		COUNTER_TYPE j;
        for (j = k + 1; j < size; j++) 
		{
            if (fabs(tempMatrix[j * size + k]) > fabs(tempMatrix[maxRow * size + k])) 
			{
                maxRow = j;
            }
        }
        if (maxRow != k) 
		{
            SwapRows(tempMatrix, size, k, maxRow);
            *determinant *= -1; // Changing row will change sign
        }

		// If main element is zero determinant is zero
        if (tempMatrix[k * size + k] == 0) 
		{
            free(tempMatrix);
			*determinant = 0;
            return determinant;
        }

        // Update sub rows
		COUNTER_TYPE x;
        for (x = k + 1; x < size; x++) 
		{
            VALUE_TYPE factor = tempMatrix[x * size + k] / tempMatrix[k * size + k];
            COUNTER_TYPE y; //Pre-initialize for variables because before C99 you can't use for initializers
			for (y = k; y < size; y++) 
			{
                tempMatrix[x * size + y] -= factor * tempMatrix[k * size + y];
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
		printf(NewLineSpecifier);
		return NULL;
	}

	if(row <= 0)
	{
		printf("Matrix can't have negative or zero size.");
		printf(NewLineSpecifier);
		return NULL;
	}

	return DeterminateRectangleMatrix(matrix, row);
}

/// @brief Main Function, Entrypoint of the App
/// @return Exit Code, 0 for succesfull 
int main()
{
	PrintAlgorithm();

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
		if(matrix == NULL){
			printf("Allocation Error Accoured");
			printf(NewLineSpecifier);
			continue;
		}
		
		ScanMatrix(matrix, row, column);
		
		PrintMatrix(matrix, row, column);
		
		#if USE_STOPWATCH == 1
		clock_t start = clock();
		#endif
		VALUE_TYPE* determinant = DeterminateMatrix(matrix, row, column);
		#if USE_STOPWATCH == 1
		clock_t stop = clock();
		long miliseconds = stop-start;
		printf("Calculated in %ld ms", miliseconds);
		printf(NewLineSpecifier);
		#endif

		free(matrix); //We don't need it more

		if(determinant == NULL)
		{
			printf("Matrix Determinant Calculation not finished succesfully, error accoured");
			printf(NewLineSpecifier);
		}
		else{
			printf("Determinant of the matrix is ");
			printf(VALUE_TYPE_SPECIFIER, *determinant);
			printf(NewLineSpecifier);

			free(determinant); //We don't need it more
		}

		int point;
		printf("Do you want to continue (1) or exit (0 or any other value): ");

		#if defined(USE_SCANF_S)
		scanf_s("%d",&point);
		#else
		scanf("%d", &point;
		#endif

		if(point != 1){
			break;
		}
	}

	printf("Application Closing...");
	printf(NewLineSpecifier);

	return 0;
}
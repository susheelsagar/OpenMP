/*****************************************************************************
 *
 * Gaussian Elimination (Parallel version - OpenMP)
 * 
 *             File : gaussian_openmp.c
 *        Author(s) : Susheel Sagar, Biswajeet Mohanty
 *          Created : 2015-05-22
 *    Last Modified : 2015-06-12
 * Last Modified by :  Susheel Sagar
****************************************************************************/
 
 
#include <stdio.h>

#define NUMB_CORES 8
#define MAX_SIZE 4096

typedef double matrix[MAX_SIZE][MAX_SIZE];

int	N;		            // matrix size	
int	maxnum;		        // max number of element
char	*Init;		    // matrix init type	
int	PRINT;		        // print switch
matrix	A;		        // matrix A
double	b[MAX_SIZE];	// vector b 
double	y[MAX_SIZE];	// vector y 
int chunksize;          // chunk size 

/* forward declarations */
void work(void);
void Init_Matrix(void);
void Print_Matrix(void);
void Init_Default(void);
int Read_Options(int, char **);

int 
main(int argc, char **argv)
{
    int i, timestart, timeend, iter;
	
    Init_Default();		        // This function is to  Initialize the  default values 
    Read_Options(argc,argv);	// Read the user arguements	
    Init_Matrix();		        // Initializing the matrix	
	work();
    if (PRINT == 1)
	Print_Matrix();
}

void
work(void)   /* Gaussian Elimination algorithm */
{
    int i, j, k;
	
    for (k = 0; k < N; k++) {   

		
		/* Creates a new group of threads */
		#pragma omp parallel for num_threads(NUMB_CORES) schedule(dynamic, chunksize)            
		for (j = k+1; j < N; j++)                                 
			A[k][j] = A[k][j] / A[k][k];                          

		y[k] = b[k] / A[k][k];	
		A[k][k] = 1.0;
		
		
		#pragma omp parallel for num_threads(NUMB_CORES) schedule(dynamic, chunksize) collapse(2)              
		for (i = k+1; i < N; i++) {		                           								       
			for (j = k+1; j < N; j++)		                  
				A[i][j] = A[i][j] - A[i][k]*A[k][j];              // Elimination step 
		}

		#pragma omp parallel for num_threads(NUMB_CORES) schedule(dynamic, chunksize)
		for (i = k+1; i < N; i++) {		
			b[i] = b[i] - A[i][k]*y[k];			
			A[i][k] = 0.0;
		}
    }
}

void
Init_Matrix()
{
    int i, j;
 
    printf("\nsize      = %dx%d ", N, N);
    printf("\nmaxnum    = %d \n", maxnum);
    printf("Init	  = %s \n", Init);
    printf("Initializing matrix...");
 
	chunksize = N/NUMB_CORES;   // chunk size of each chunk that is allocated to a thread by omp
	
    if (strcmp(Init,"rand") == 0) {
	    #pragma omp parallel for num_threads(NUMB_CORES) schedule(dynamic, chunksize) collapse (2)
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if (i == j)                                      // diagonal dominance 
					A[i][j] = (double)(rand() % maxnum) + 5.0;
				else
					A[i][j] = (double)(rand() % maxnum) + 1.0;
			}
		}
    }
	
    if (strcmp(Init,"fast") == 0) {
	    #pragma omp parallel for num_threads(NUMB_CORES) schedule(dynamic, chunksize) collapse (2)
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if (i == j)                                      // diagonal dominance 
					A[i][j] = 5.0;
				else
					A[i][j] = 2.0;
			}
		}
    }

    #pragma omp parallel for num_threads(NUMB_CORES) schedule(dynamic, chunksize)
	for (i = 0; i < N; i++) {                                    // Initialize vectors b and y
		b[i] = 2.0;
		y[i] = 1.0;
    }

    printf("done \n\n");
    if (PRINT == 1)
	Print_Matrix();
}

void
Print_Matrix()
{
    int i, j;
 
    printf("Matrix A:\n");
    for (i = 0; i < N; i++) {
	printf("[");
	for (j = 0; j < N; j++)
	    printf(" %5.2f,", A[i][j]);
	printf("]\n");
    }
    printf("Vector b:\n[");
    for (j = 0; j < N; j++)
	printf(" %5.2f,", b[j]);
    printf("]\n");
    printf("Vector y:\n[");
    for (j = 0; j < N; j++)
	printf(" %5.2f,", y[j]);
    printf("]\n");
    printf("\n\n");
}

void 
Init_Default()
{
    N = 2048;
    Init = "rand";
    maxnum = 15.0;
    PRINT = 0;
}
 
int
Read_Options(int argc, char **argv)
{
    char    *prog;
 
    prog = *argv;
    while (++argv, --argc > 0)
	if (**argv == '-')
	    switch ( *++*argv ) {
	    case 'n':
		--argc;
		N = atoi(*++argv);
		break;
	    case 'h':
		printf("\nHELP: try gomp -u \n\n");
		exit(0);
		break;
	    case 'u':
		printf("\nUsage: gomp [-n problemsize]\n");
		printf("           [-D] show default values \n");
		printf("           [-h] help \n");
		printf("           [-I init_type] fast/rand \n");
		printf("           [-m maxnum] max random no \n");
		printf("           [-P print_switch] 0/1 \n");
		exit(0);
		break;
	    case 'D':
		printf("\nDefault:  n         = %d ", N);
		printf("\n          Init      = rand" );
		printf("\n          maxnum    = 5 ");
		printf("\n          P         = 0 \n\n");
		exit(0);
		break;
	    case 'I':
		--argc;
		Init = *++argv;
		break;
	    case 'm':
		--argc;
		maxnum = atoi(*++argv);
		break;
	    case 'P':
		--argc;
		PRINT = atoi(*++argv);
		break;
	    default:
		printf("%s: ignored option: -%s\n", prog, *argv);
		printf("HELP: try %s -u \n\n", prog);
		break;
	    } 
}

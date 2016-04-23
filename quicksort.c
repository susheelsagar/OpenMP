/***************************************************************************
 *
 *  Parallel implementation of Quick Sort using OpenMP
 * 
 *             File : q_sort.c
 *        Author(s) : Susheel Sagar, Biswajeet Mohanty
 *          Created : 2015-05-22
 *    Last Modified : 2015-06-13
 * Last Modified by : Susheel Sagar
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define PRINT 1

static int *list;
static int *index;
static int size;
static int threads;

static void
initialization(char** argv)                     /* function to initilize the values */
{
    int i;
    size = atoi(argv[1]);
    list = (int *) malloc(size * sizeof(int));
    for (i = 0; i < size; i++)
        list[i] = rand()%100;
	
	threads = atoi(argv[2]);
	omp_set_num_threads(threads);
	index = (int *)malloc((threads+1) * sizeof(int));
	for(i=0; i<threads; i++) 
		index[i]=i*size/threads;           
		index[threads]=size;                   
	
}

static void
print_elements(void)   /*function to print the values in the array*/
{
    printf("\n\n");
	int i;

    for (i = 0; i < size; i++)
        printf("%d ", list[i]);
    printf("\n\n");
}

static int Comparision(const void *a, const void *b)
{
	return ( *(int*)a - *(int*)b );
}

static void 
sort(int *list, int size, int *index, int s) /*function to sort the elements in the array*/
{
	int i;
	while ( s > 1 ) {	    
		for( i = 0; i < s; i++ )
			index[i] = i*size/s;             
		index[s]=size;                       
		
		#pragma omp parallel for private(i) 
	    for( i = 0; i < s; i=i+2 ) {
			merge(list+index[i], list+index[i+1], index[i+1]-index[i], index[i+2]-index[i+1]);				
	    }
	    s = s/2;
	}
}

void merge(int* A, int* B, int size1, int size2) 
{
	int i = 0, j = 0, k = 0,t;
	int size = size1 + size2;
	int *temp = (int *)malloc(size * sizeof(int));
	
	
	while (i < size1 && j < size2) {
		if (A[i] <= B[j]) 
			temp[k] = A[i++];
		else temp[k] = B[j++];
		k++;
	}
	
	if (i < size1) {                           
		for ( t = i; t < size1; t++, k++)   // Copy the other elements of  sorted array A to the temp array
			temp[k] = A[t];
	}
	else {                                     
		for ( t = j; t < size2; t++, k++)   
			temp[k] = B[t];
	}
	
	for( i=0; i<size; i++ )    
		A[i] = temp[i];
	
	free(temp);
}

int main(int argc, char **argv)
{
    int i;
	
	initialization(argv);
    if(PRINT)
		print_elements(); 

	
	#pragma omp parallel for private(i)
	for(i = 0; i < threads; i++) 
		qsort(list+index[i], index[i+1]-index[i], sizeof(int), Comparision);
	
	if( threads > 1 ) 
		sort(list, size, index, threads);	
	if(PRINT)
		print_elements();
}




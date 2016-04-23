/**********************************************************************************
 *                                                                                *
 *                                                                                *
 *            Instructions to compile and execute the source code files           *
 *                                                                                *
 *                                                                                *
 **********************************************************************************/


/*********************************************COMPILING THE SOURCE CODE**********************************/

1) Compiling the quicksort.c file

		gcc -fopenmp -o qs quicksort.c

2) Compiling the Gaussian Elimination File
		
		gcc -fopenmp -o gauss gaussian_openmp.c



/************************************EXECUTING THE EXE FILES*******************************************/

(1) Execution of the Quicksort program, type:
	
	./qs [numb_elements] [numb_threads]

        numb_elements    are the number of elements to be sorted. 

        numb_threads     are the number of threads that the program is desired to 
                         be run on (the number of threads should be a power of 2).
		
(2) To execute the Gaussian Elimination program:    
    
        ./gauss [-n problemsize] [-I init_type] [-m max_random_num] [-P print_flag]
 
 
	If -n, -I and -m are used, init_type can be either "rand" or "fast"
				   
	problemsize       is the size of the matrix (Example: 16 indicates a 16 X 16 matrix)     
					  
	init_type         Initialization type can be random (rand) or fast (fast)              
					  [rand/fast]

	max_random_num    maximum value that a generated random number can take.                  
					  [a positive integer]
        
        print_flag        Default value is 0. A value of 1 prints the output matrix.
                                          [0/1]
 


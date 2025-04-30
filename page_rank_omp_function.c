#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>
#include <string.h>

//original code by authors Gaia O. and Gianluca C
///refactored and made parallel by nicholas hibbard
// with added features 



/* Step One: 
Most of the overhead of the dynamic distribution comes from the locking step performed by the threads to acquire the new iteration to work with. It just looks to me that there is not much thread locking contention going on, and even if it is, it is being compensated by better loading balancing achieved with the dynamic scheduler. I have seen this exact pattern before there is not wrong with it.
*/
typedef struct {
	int n;
    float **a;
	float **at;
	float *p;
	int *out_link;
	float d;
	float *p_new;

} pageRank;
void initializationOfAt(pageRank page);
void initializationOfOutLink(pageRank page);
void initializationOfP(pageRank page);
void matrixStochasticFied(pageRank page);
void transpose(pageRank page);
float calc_p(pageRank page);

int main(int argc, char* argv[]){

	/*************************** TIME, VARIABLES ***************************/

	// Keep track of the execution time
	double time_spent;
	if (argc != 3) 
	{ 
        printf("\nEnter Thread count and input file please\n");
    	return 1;
    }
	int thread_count = atoi(argv[1]);
	omp_set_num_threads(thread_count);

	double start_time = omp_get_wtime();

	// Set the damping factor 'd'

	/******************* OPEN FILE + NUM OF NODES **************************/

	// Open the data set
     char *filename = malloc(strlen(argv[1]) + 1);
	strcpy(filename, argv[2]);
    FILE *fp;
    if((fp = fopen(filename,"r")) == NULL) {
        fprintf(stderr,"[Error] Cannot open the file");
        exit(1);
    }

    // Read the data set and get the number of nodes (n)
    int n;
    char ch;
    char str[100];
    ch = getc(fp);
    while (ch =='#') {
		
        if (fgets(str, 100 - 1, fp) == NULL) 
		{
		}
        sscanf (str,"%*s %d %*s %*d", &n); 
        ch = getc(fp);
    }
    ungetc(ch, fp);
	if(thread_count > n){
		printf("input thread count lower then n\n");
		return 1;
	}
    // DEBUG: Print the number of nodes 
    printf("\nNumber of nodes = %d\n",n);

   	/********************** INITIALIZATION OF A **************************/

	int i, j, node1, node2;
	pageRank page;
	page.a = malloc(sizeof *page.a * n);
	page.at = malloc(sizeof *page.at * n);
	page.d = 0.85;
	page.n = n;
	
	// Preallocate the adjacency matrix 'a'   
#pragma omp parallel num_threads(thread_count)
{
	#pragma omp for schedule(dynamic)
	for (int i = 0; i < n; i++) {
	  	page.a[i] = malloc(sizeof *page.a[i] * n);
	}
	#pragma omp for nowait
	for (int i = 0; i < n; i++) {
	    page.at[i] = malloc(sizeof *page.at[i] * n);
	}

	// Initialize all the adjacency matrix to 0.0
    #pragma omp for schedule(dynamic,1000)
	for(i = 0; i < n; i++){ 
        for(j = 0; j < n; j++){ 
        	page.a[i][j] = 0.0;
        }
    }

}

	while(!feof(fp)){//scans till end of file
	if (fscanf(fp, "%d%d", &node1, &node2) != 2) {
    // Handle error, such as unexpected format or end-of-file
}
	    page.a[node1][node2] = 1.0;
	}

	page.p = malloc(sizeof(float) * page.n);
	page.out_link = malloc(sizeof(int) * page.n);
	#pragma omp parallel num_threads(thread_count)//split work up kind of like mpi 
	{
		initializationOfAt(page);
		initializationOfP(page);
		initializationOfOutLink(page);

	/******************* INITIALIZATION OF OUTPUT LINK ********************/
	#pragma omp barrier
 	#pragma omp single
 	{
	printf("\nPer-node out link: \n\n     [");
	for (i=0; i<n; i++){
		printf("%d", page.out_link[i]);
		if(i != (n-1)){
			printf(", "); 
		}
	}
	printf("]\n\n");
 	}

	matrixStochasticFied(page);
	transpose(page);

	}
	/*************************** PageRank LOOP  **************************/
	int looping = 1;
	int k = 0;

	page.p_new= malloc(sizeof(int) * page.n);
	while (looping) {
	float error = 0.0;
		#pragma omp parallel num_threads(thread_count) reduction(+:error)
		error = calc_p(page);

	    if (error < 0.000001){
	        looping = 0;
	    }
	    for (i=0; i<n;i++){
	    	page.p[i] = page.p_new[i];
		}
	    k = k + 1;
	}
	/*************************** CONCLUSIONS *******************************/

	// Stop the timer and compute the time spent
	double end_time = omp_get_wtime();
	time_spent = (end_time - start_time);

	// Sleep a bit so stdout is not messed up
	sleep(1);
		
	// Print results
	printf ("\nNumber of iteration to converge: %d \n\n", k); 
	printf ("Final Pagerank values:\n\n[");
	for (i=0; i<n; i++){
		printf("%f ", page.p[i]);
		if(i!=(n-1)){ printf(", "); }
	}
	printf("]\n\nTime spent: %f seconds. thread count %d\n", time_spent,thread_count);
	return 0;
}

void initializationOfAt(pageRank page)
{
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int startrange = page.n / thread_count;
	int endrange = page.n / thread_count;
	int residual = page.n % thread_count;
	if(my_rank==thread_count-1)
		endrange=endrange+residual;
	for (int i=(my_rank*startrange); i<((my_rank*startrange)+endrange); i++){
		for (int j=0; j<page.n; j++){
			page.at[i][j] = 0.0;
		}
	}

}
void initializationOfOutLink(pageRank page)
{
	int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int startrange = page.n / thread_count;
	int endrange = page.n / thread_count;
	int residual = page.n % thread_count;
	if(my_rank==thread_count-1)
		endrange=endrange+residual;

	for (int i=(my_rank*startrange); i<((my_rank*startrange)+endrange); i++) {
		for (int j=0; j<page.n; j++) {
			if (page.a[i][j] != 0.0) {
				page.out_link[i] = page.out_link[i] + 1;// will add up the outgoing links from a page
			}
		}
	}

}
void initializationOfP(pageRank page)
{
	int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int startrange = page.n / thread_count;
	int endrange = page.n / thread_count;
	int residual = page.n % thread_count;
	if(my_rank==thread_count-1){
		endrange=endrange+residual;
	}

		for(int i=(my_rank*startrange); i<((my_rank*startrange)+endrange); i++) {
		page.p[i] = 1.0 / page.n;//initial values of pages will be 1/num of all pages
        page.out_link[i] = 0;// we start with no out going links because we are just initializing it all 
	}

}
void matrixStochasticFied(pageRank page)
{
	int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int startrange = page.n / thread_count;
	int endrange = page.n / thread_count;
	int residual = page.n % thread_count;

	if(my_rank==thread_count-1)
		endrange=endrange+residual;

	for (int i=(my_rank*startrange); i<((my_rank*startrange)+endrange); i++){
		if (page.out_link[i] == 0){
			// Deal with dangling nodes
			for (int j=0; j<page.n; j++){
				page.a[i][j] = 1.0 / page.n;
			}
		} else {
			for (int j=0; j<page.n; j++){
				if (page.a[i][j] != 0) {//this will going through all pages that have links pointed page and update the values 
					page.a[i][j] = page.a[i][j] / page.out_link[i];
				}
			}
		}
	}


}

void transpose(pageRank page)
{
	int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int startrange = page.n / thread_count;
	int endrange = page.n / thread_count;
	int residual = page.n % thread_count;

	if(my_rank==thread_count-1)
		endrange=endrange+residual;

	for (int i=(my_rank*startrange); i<((my_rank*startrange)+endrange); i++){
		for (int j=0; j<page.n; j++){
			page.at[j][i] = page.a[i][j];//transpose
		}
	}
}

float calc_p(pageRank page)
{
	int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int startrange = page.n / thread_count;
	int endrange = page.n / thread_count;
	int residual = page.n % thread_count;

	if(my_rank==thread_count-1)
		endrange=endrange+residual;


		
		// Update p_new (without using the damping factor)
			float error = 0.0;
		for (int i=(my_rank*startrange); i<((my_rank*startrange)+endrange); i++){
			page.p_new[i] = 0.0;
			for (int j=0; j<page.n; j++){
				page.p_new[i] = page.p_new[i] + (page.at[i][j] * page.p[j]);
			}
			page.p_new[i] = page.d * page.p_new[i] + (1.0 - page.d) / page.n;
			error =  error + fabs(page.p_new[i] - page.p[i]);
		} 



		return error;

}
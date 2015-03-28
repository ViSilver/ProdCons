#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#if defined (__cplusplus) && __cplusplus > 199711L
/* C++ 11 */
#include <thread>
#include <mutex>     
#include <condition_variable>
using namespace std;
#endif /* __cplusplus */



struct TRect
 {
	 int             m_X;
	 int             m_Y;
	 int             m_W;
	 int             m_H;
 };

struct TCostProblem
 {
	 int          ** m_Values;
	 int             m_Size;
	 int             m_MaxCost;
	 void         (* m_Done) ( const TCostProblem *, const TRect * );
 };

struct TCrimeProblem
 {
	 double       ** m_Values;
	 int             m_Size;
	 double          m_MaxCrime;
	 void         (* m_Done) ( const TCrimeProblem *, const TRect * );
 };

#endif /* __PROGTEST__ */


// My global variables

enum cell_type
{
	dummy,
	cost,
	crime
};

struct ProcData  // Processed data structer
{
	cell_type type;
	void * data;
};

struct ProdCostStruct
{
	intptr_t thrID;
	const TCostProblem * (* costFunc) ( void ); // Pointer to costFunc
};

struct ProdCrimeStruct
{
	intptr_t thrID;
	const TCrimeProblem * (* crimeFunc) ( void ); // Pointer to crimeFunc
};

PROC_DATA 			* g_Queue;
int 				g_Pos, g_Prods, g_Conrs;

pthread_mutex_t  	g_Mtx;
sem_t 				g_Full, g_Free;

void * prodFunc ( intptr_t  id ) // To be implemented a prodFunc for each type of producer
 {
	int i;
	 
	for ( i = 0; ; i ++ )
		{
			// Call the problem generating function and store the data into a local variable
			// Check if the received value is NULL

			var = 

			sem_wait ( &g_Free );
			pthread_mutex_lock ( &g_Mtx );   //first erorr
			
			// set the current cell of the queue to the problem pointer
			// increment g_Pos

			pthread_mutex_unlock ( &g_Mtx );
			sem_post ( &g_Full );
			
		}
	return ( NULL );
 }

void * consFunc ( intptr_t  id )
 {
	while ( 1 )
		{
			//sleep(1);
			sem_wait ( &g_Full );
			pthread_mutex_lock ( &g_Mtx );

			// decrement g_Pos
			// get the structure of data and set the value of the array to NULL

			pthread_mutex_unlock ( &g_Mtx );
			sem_post ( &g_Free );

			// Check if it is dummy - check if it is the last consumer
			// If it is dummy - deallocate the dynamic variables

			// Call the problem function and solve it
			// call the m_Done() function
			
		}
	return ( NULL );
 }

void MapAnalyzer ( int threads, const TCostProblem * (* costFunc) ( void ), const TCrimeProblem* (* crimeFunc) ( void ) )
 {
	//    
	int prod, cons, i;
	pthread_t      * thrID;
	pthread_attr_t   attr;

	thrID = (pthread_t *) malloc ( sizeof ( *thrID ) * ( prod + cons ) );

	// Initialization of global variables
	g_Queue = (PROC_DATA *) malloc ( sizeof ( *g_Queue ) * ( threads + 6 ) );
	g_Conrs = threads;
	g_Prods = 2;
	g_Pos = 0;

	// Creating the data for the producers
	ProdCostStruct 		s_ProdCostData;
	ProdCrimeStruct		s_ProdCrimeData; 

	pthread_attr_init ( &attr );
	pthread_attr_setdetachstate ( &attr, PTHREAD_CREATE_JOINABLE );

	pthread_mutex_init ( &g_Mtx, NULL );
	sem_init ( &g_Free, 0, threads );        //fourth problem - the size of the initial free space is the size of the entire buffer
	sem_init ( &g_Full, 0, 0 );                  //fourth problem

	if ( pthread_create (&thrID[0], &attr, prodCostFunc, (void*) (&s_ProdCostData)) )
	{
		perror ( "pthread_create" );
		return ( 1 ); 
	}
	if ( pthread_create (&thrID[1], &attr, prodCrimeFunc, (void*) (&s_ProdCrimeData)) )
	{
		perror ( "pthread_create" );
		return ( 1 ); 
	}

	// for (int i = 0; i < 2; i++)
	// {
	// 	if ( pthread_create ( &thrID[i], &attr, (void*(*)(void*)) prodFunc, (void*)(intptr_t) (i + 1) ) )
	// 	{
	// 		perror ( "pthread_create" );
	// 		return ( 1 );    
	// 	}
	// }

	for ( i = 0; i < threads; i ++ )
		if ( pthread_create ( &thrID[i + threads], &attr, (void*(*)(void*)) consFunc, (void*)(intptr_t) (i + 1) ) )
		{
			perror ( "pthread_create" );
			return ( 1 );    
		}
	pthread_attr_destroy ( &attr );  
		 
	for ( i = 0; i < prod + cons; i ++ )
		pthread_join ( thrID[i], NULL );  

	pthread_mutex_destroy ( &g_Mtx );
	sem_destroy ( &g_Free );
	sem_destroy ( &g_Full );

	free ( thrID );
	return ( 0 );
	}
 }

bool FindByCost ( int ** values, int size, int maxCost, TRect * res )
 {
	long ** precalc, A, B, C, D, sum;
	struct TRect * max_rect;
	int maxArea = 0;

	max_rect = new TRect;

	max_rect -> m_X = 0;
	max_rect -> m_Y = 0;
	max_rect -> m_W = 0;
	max_rect -> m_H = 0;

	// memory allocation for temporary array
	precalc = (long **) malloc ( sizeof( long * ) * size );

	for (int i = 0; i < size; i ++)
	{
		precalc[i] = (long *) malloc ( sizeof( long ) * size );
	}

	// reading the input data
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++ )	
		{
				precalc[y][x] = (long) values[y][x];    // values[y][x]
		}
	}

	// precalculating the first row and the first column
	for (int i = 1; i < size; i++)
	{
		precalc[i][0] += precalc[i-1][0];
		precalc[0][i] += precalc[0][i-1];
	}

	// precalculating the rest of the matrix
	for (int i = 1; i < size; i++)
	{
		for (int j = 1; j < size; j++)
		{
			precalc[i][j] += precalc[i-1][j] + precalc[i][j-1] - precalc[i-1][j-1]; 
		}
	}

	for (int j = size - 1; j >= 0; j--) 
	{
		for (int l = size - 1; l >= 0; l--)
		{
			for (int i = 0; i < size - j; i++)
			{
				for (int k = 0; k < size - l; k++)
				{
					if(i == 0 && k == 0)
					{
						A = 0;
						B = 0;
						C = 0;
						D = precalc[k + l][i + j];
					} else {
						if(i == 0){
							A = 0;
							B = precalc[k - 1][i + j];
							C = 0;
							D = precalc[k + l][i + j];
						} else {
							if (k == 0){
								A = 0;
								B = 0;
								C = precalc[k + l][i - 1];
								D = precalc[k + l][i + j];
							} else {
								A = precalc[k - 1][i - 1];
								B = precalc[k - 1][i + j];
								C = precalc[k + l][i - 1];
								D = precalc[k + l][i + j];
							}
						}
					}

					sum = D + A - B - C;
					int area = (l + 1) * (j + 1);

					if (sum <= maxCost && area > maxArea)
					{
						max_rect -> m_X = i;
						max_rect -> m_Y = k;
						max_rect -> m_W = j + 1;
						max_rect -> m_H = l + 1;
						maxArea = area;
					}
				}
			}
		} 
	}

	///// Returning the result 

	res -> m_Y = max_rect -> m_Y;
	res -> m_X = max_rect -> m_X;
	res -> m_H = max_rect -> m_H;
	res -> m_W = max_rect -> m_W;

	// cleanup
	for (int i = 0; i < size; i++)
	{
		free (precalc[i]);
	}

	free (precalc);
	delete max_rect;

	 return (!(res -> m_Y == 0 && res -> m_X == 0 && (res -> m_W == 0 || res -> m_H == 0)));
 }

void recomputeCrimeMatrix(int ** m1, int ** m2, int size, int col)
 {
	for (int i = 0; i < size; i++)
	{
		m2[i][col] = m1[i][col];
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = col + 1; j < size; j++)
		{
			m2[i][j] = m2[i][j - 1] + m1[i][j];
		}
	}

	return;
 }

bool FindByCrime ( double ** values, int size, double maxCrime, TRect * res )
 {  
	int ** tmp, ** s_tmp, h, prev, last;
	struct TRect * max_rect;

	max_rect = new TRect;

	max_rect -> m_X = 0;
	max_rect -> m_Y = 0;
	max_rect -> m_W = 0;
	max_rect -> m_H = 0;

	// memory allocation for the binary temp array
	tmp = (int **) malloc ( sizeof (int *) * size);

	for (int i = 0; i < size; i++)
	{
		tmp[i] = (int *) malloc ( sizeof (int) * size);
	}

	for (int y = 0; y < size; y++)
	{
			for (int x = 0; x < size; x++)
			{
				if (values[y][x] > maxCrime) 
				{
					tmp[y][x] = 1;
				} else 
				{
					tmp[y][x] = 0;
				}
			}
	}

	// memory allocation for the sum temp array
	s_tmp = (int **) malloc ( sizeof (int *) * size);

	for (int i = 0; i < size; i++)
	{
		s_tmp[i] = (int *) malloc ( sizeof (int) * size);
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			s_tmp[i][j] = tmp[i][j];
		}
	}

	for (int i = 0; i < size; i++)
	{
		// recompute crime matrix
		recomputeCrimeMatrix(tmp, s_tmp, size, i);

		for (int j = i; j < size; j++)
		{
			prev = -1;
			h = 0;
			last = -1;

			for (int k = 0; k < size; k++)
			{
				if (s_tmp[k][j] >= 1) 
				{
					if (k - prev - 1 > h)
					{
						h = k - prev - 1;
						last = k;
					}					
					prev = k; 
				}
			}	

			if (prev < size - 1 && size - prev - 1 > h)
			{	
				last = size;
				h = size - prev - 1;  
			} 

			if (last == -1)
			{
				last = size;
			}

			int w = j - i + 1;

			if (h * w > (max_rect -> m_W * max_rect -> m_H) )
			{
				max_rect -> m_W = w;
				max_rect -> m_H = h;
				max_rect -> m_Y = last - h;
				max_rect -> m_X = i;
			}	
		}
	}

	///// Returning the result 
	res -> m_Y = max_rect -> m_Y;
	res -> m_X = max_rect -> m_X;
	res -> m_H = max_rect -> m_H;
	res -> m_W = max_rect -> m_W;

	// cleanup
	for (int i = 0; i < size; i++)
	{
		free (tmp[i]);
		free (s_tmp[i]);
	}

	free (tmp);
	free (s_tmp);
	delete max_rect;

	 return (!(res -> m_Y == 0 && res -> m_X == 0 && (res -> m_W == 0 || res -> m_H == 0)));
 }






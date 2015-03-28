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

#define __MAC__


// My global variables

enum cell_type
{
	dummy,
	cost,
	crime
};

typedef struct ProcData  // Processed data structer
{
	cell_type type;
	void * data;
}PROC_DATA;

struct ProdCostStruct
{
	//intptr_t thrID;
	const TCostProblem * (* tCostFunc) ( void ); // Pointer to costFunc
};

struct ProdCrimeStruct
{
	//intptr_t thrID;
	const TCrimeProblem * (* tCrimeFunc) ( void ); // Pointer to crimeFunc
};

PROC_DATA 			** g_Queue;
int 				g_Pos, g_Prods, g_Conrs;

pthread_mutex_t  	g_Mtx;
#ifndef __MAC__
sem_t 				g_Full, g_Free;
#else
sem_t 				*g_Full, *g_Free;
#endif


bool FindByCost ( int ** values, int size, int maxCost, TRect * res );
bool FindByCrime ( double ** values, int size, double maxCrime, TRect * res );



void * prodCostFunc ( void * data) // To be implemented a prodFunc for each type of producer
 {
	int i;
	 
	for ( i = 0; ; i ++ )
		{
			// Call the problem generating function and store the data into a local variable
			// Check if the received value is NULL

            const TCostProblem * tmp = ( ( ProdCostStruct * ) data ) -> tCostFunc();
            PROC_DATA * pData = (PROC_DATA *) malloc ( sizeof ( * pData ) );
            
            pData -> data = (void *) tmp;
            
            if (tmp == NULL) {
                pData -> type = dummy;
            } else
            {
                pData -> type = cost;
            }

#ifndef __MAC__
            sem_wait ( &g_Free );
#else  // LINUX
            sem_wait ( g_Free );
#endif  // MAC
			pthread_mutex_lock ( &g_Mtx );   //first erorr
			
			// set the current cell of the queue to the problem pointer
            g_Queue[g_Pos] = pData;
			// increment g_Pos
            g_Pos++;

			pthread_mutex_unlock ( &g_Mtx );
#ifndef __MAC__
			sem_post ( &g_Full );
#else  // LINUX
            sem_post ( g_Full );
#endif
			
		}
	return ( NULL );
 }

 void * prodCrimeFunc ( void * data) // To be implemented a prodFunc for each type of producer
 {
	int i;
	 
	for ( i = 0; ; i ++ )
		{
			// Call the problem generating function and store the data into a local variable
			// Check if the received value is NULL

            const TCrimeProblem * tmp =  ( ( ProdCrimeStruct * ) data ) -> tCrimeFunc();
            PROC_DATA * pData = (PROC_DATA *) malloc ( sizeof ( * pData ) );
            
            pData -> data = (void *) tmp;
            
            if (tmp == NULL) {
                pData -> type = dummy;
            } else
            {
                pData -> type = crime;
            }

#ifndef __MAC__
            sem_wait ( &g_Free );
#else  // LINUX
            sem_wait ( g_Free );
#endif  // MAC
			pthread_mutex_lock ( &g_Mtx );   //first erorr
			
            // set the current cell of the queue to the problem pointer
            g_Queue[g_Pos] = pData;
            // increment g_Pos
            g_Pos ++;

			pthread_mutex_unlock ( &g_Mtx );
#ifndef __MAC__
            sem_post ( &g_Full );
#else  // LINUX
            sem_post ( g_Full );
#endif
			
		}
	return ( NULL );
 }

void * consFunc ( void )
 {
     TCrimeProblem * tCrime;
     TCostProblem * tCost;
     PROC_DATA * pData;
     TRect * tRect;
     tRect = (TRect *) malloc ( sizeof ( * tRect ) );
     
	while ( 1 )
		{
            tCrime = NULL;
            tCost = NULL;
            pData = NULL;
            tRect -> m_W = 0;
            tRect -> m_H = 0;
            tRect -> m_X = 0;
            tRect -> m_Y = 0;
            
			//sleep(1);
#ifndef __MAC__
            sem_wait ( &g_Full );
#else  // LINUX
            sem_wait ( g_Full );
#endif  // MAC
			pthread_mutex_lock ( &g_Mtx );

			// decrement g_Pos
            g_Pos--;
			// get the structure of data and set the value of the array to NULL
            pData = g_Queue[g_Pos];

			pthread_mutex_unlock ( &g_Mtx );
#ifndef __MAC__
            sem_post ( &g_Free );
#else  // LINUX
            sem_post ( g_Free );
#endif
            
            // Check if it is dummy - check if it is the last consumer
            // If it is dummy - deallocate the dynamic variables
            
            // Call the problem function and solve it
            // call the m_Done() function
            
            switch (pData -> type) {
                case crime:
                    tCrime = (TCrimeProblem *) (pData -> data);
                    if ( FindByCrime ( tCrime -> m_Values, tCrime -> m_Size, tCrime -> m_MaxCrime, tRect ) ) {
                        tCrime -> m_Done ( tCrime, tRect );
                    } else {
                        tCrime -> m_Done ( tCrime, NULL );
                    }
                    free ( pData );
                    break;
                    
                case cost:
                    tCost = (TCostProblem *) (pData -> data);
                    if ( FindByCost ( tCost -> m_Values, tCost -> m_Size, tCost -> m_MaxCost, tRect ) ) {
                        tCost -> m_Done ( tCost, tRect );
                    } else {
                        tCost -> m_Done ( tCost, NULL );
                    }
                    free ( pData );
                    break;
                    
                default:
                    // If this is a dummy data
                    free ( pData );
                    break;
            }
		}
	return ( NULL );
 }

void MapAnalyzer ( int threads, const TCostProblem * (* costFunc) ( void ), const TCrimeProblem* (* crimeFunc) ( void ) )
 {
	//    
     int i;
     pthread_t      * thrID;
     pthread_attr_t   attr;

	thrID = (pthread_t *) malloc ( sizeof ( *thrID ) * ( threads + 2 ) );

	// Initialization of global variables
     g_Queue = (PROC_DATA **) malloc ( sizeof ( *g_Queue ) * ( threads + 6 ) );
     g_Conrs = threads;
     g_Prods = 2;
     g_Pos = 0;

	// Creating the data for the producers
     ProdCostStruct 		s_ProdCostData;
     ProdCrimeStruct		s_ProdCrimeData;

     s_ProdCostData.tCostFunc = costFunc;
     s_ProdCrimeData.tCrimeFunc = crimeFunc;
     
     pthread_attr_init ( &attr );
     pthread_attr_setdetachstate ( &attr, PTHREAD_CREATE_JOINABLE );

     pthread_mutex_init ( &g_Mtx, NULL );
#ifndef __MAC__
     sem_init ( &g_Free, 0, threads );        //fourth problem - the size of the initial free space is the size of the entire buffer
     sem_init ( &g_Full, 0, 0 );                  //fourth problem
#else   // LINUX
     if ((g_Free = sem_open("/semaphore_free", O_CREAT, 0666, threads)) == SEM_FAILED)
     {
        perror("sem_open");
        return;
     }
     
     if ((g_Full = sem_open("/semaphore_full", O_CREAT, 0666, 0)) == SEM_FAILED)
     {
         perror("sem_open");
         return;
     }
#endif   // MAC

     if ( pthread_create (&thrID[0], &attr, prodCostFunc, (void*) (&s_ProdCostData)) )
     {
         perror ( "pthread_create" );
         return;
     }
     if ( pthread_create (&thrID[1], &attr, prodCrimeFunc, (void*) (&s_ProdCrimeData)) )
     {
         perror ( "pthread_create" );
         return;
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
         if ( pthread_create ( &thrID[2 + i], &attr, (void *(*)(void *)) consFunc, NULL))
         {
             perror ( "pthread_create" );
             return;
         }
     
     pthread_attr_destroy ( &attr );
		 
     for ( i = 0; i < threads + 2; i ++ )
         pthread_join ( thrID[i], NULL );

     pthread_mutex_destroy ( &g_Mtx );
#ifndef __MAC__
     sem_destroy ( &g_Free );
     sem_destroy ( &g_Full );
#else   // LINUX
     if(sem_close(g_Free) == -1)
     {
        perror("sem_close");
        return;
     }
     if (sem_close(g_Full) == -1)
     {
        perror("sem_close");
        return;
     }
     
     if (sem_unlink("/semaphore_free") == -1)
     {
        perror("sem_unlink");
        return;
     }
     
     if (sem_unlink("/semaphore_full") == -1) {
        perror("sem_unlink");
        return;
     }
#endif  /* __MAC__ */

     free ( thrID );
     return;
}

bool FindByCost ( int ** values, int size, int maxCost, TRect * res )
 {
	long ** precalc, A, B, C, D, sum;
	struct TRect * max_rect;
	int maxArea = 0;
    bool found = false;

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
                        found = true;
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

	 return (found);
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
    bool found = false;

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
                found = true;
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

	 return (found);
 }






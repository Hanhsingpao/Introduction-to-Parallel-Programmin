#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


// setting the number of threads:
#ifndef NUMT
#define NUMT		    4
#endif

// setting the number of trials in the monte carlo simulation:
#ifndef NUMNODES
#define NUMNODES	1000
#endif

#ifndef NUMTIMES
#define NUMTIMES   20
#endif
// how many tries to discover the maximum performance:


#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

const float N = 2.5f;
const float R = 1.2f;

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float rn = pow( fabs(R), (double)N );
	float r = rn - xn - yn;
	if( r <= 0. )
	        return 0.;
	float height = pow( r, 1./(double)N );
	return height;
}


int main( int argc, char *argv[ ] )
{
  #ifndef _OPENMP
    fprintf( stderr, "No OpenMP support!\n" );
    return 1;
  #endif
    
    
    omp_set_num_threads( NUMT );    // set the number of threads to use in parallelizing the for-loop:`
    
    // get ready to record the maximum performance and the probability:
    double  maxPerformance = 0.;    // must be declared outside the NUMTIMES loop
    double sum_volume = 0.;
    int  numHits;                // must be declared outside the NUMTIMES loop
    float fullTileArea = 
    (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );
    
    float TileAr1 = fullTileArea /2.0;
    float TileAr2 = fullTileArea /4.0;
    
    // looking for the maximum performance:
    
      
      double volume = 0.;
      double time0 = omp_get_wtime( );
      //numHits = 0;
      
     //pragma omp parallel for collapse(2) default(none) . . .
      for( int i = 0; i < NUMNODES*NUMNODES; i++ )
      {
         int iu = i % NUMNODES;
         int iv = i / NUMNODES;
         float z = Height( iu, iv );
         
         if ( (iu == 0 && iv == 0) || (iu == 0 && iv == NUMNODES - 1) ||
             (iu == NUMNODES - 1 && iv == NUMNODES - 1) || (iu == NUMNODES - 1 && iv == 0) ) {
            volume += z * TileAr2;
         }
         
         else if ( iu == 0 || iu == NUMNODES - 1 || iv == 0 || iv == NUMNODES - 1 ) {
            volume += z * TileAr1;
         }
         
         else {
            volume += z * fullTileArea;
         }
      }
      volume *= 2.;
      double time1 = omp_get_wtime( );
      double megaHeightPerSecond = (double)(NUMNODES*NUMNODES) / ( time1 - time0 ) / 1000000.;
      if( megaHeightPerSecond > maxPerformance )
      maxPerformance = megaHeightPerSecond;
      sum_volume = volume;
      
   
    fprintf(stderr, "%d NUMT ; %d NUMNODES ; volume = %6.2f ; megaHeights/sec = %6.2lf\n",
                NUMT, NUMNODES, sum_volume, maxPerformance);

}
  































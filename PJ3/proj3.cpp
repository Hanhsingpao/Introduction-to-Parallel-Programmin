#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>

int	NowYear;		// 2022 - 2027
int	NowMonth;		// 0 - 11
int NowAddMonth = 1;

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
int	NowNumHunter;

const float GRAIN_GROWS_PER_MONTH = 9.0;
const float ONE_DEER_EATS_PER_MONTH = 1.0;
const float ONE_PEOPLE_HUNTS_PER_MONTH = 0.5;

const float AVG_PRECIP_PER_MONTH = 7.0;	// average
const float AMP_PRECIP_PER_MONTH = 6.0;	// plus or minus
const float RANDOM_PRECIP = 2.0;	// plus or minus noise

const float AVG_TEMP = 60.0;	// average
const float AMP_TEMP = 20.0;	// plus or minus
const float RANDOM_TEMP = 10.0;	// plus or minus noise

const float MIDTEMP = 40.0;
const float MIDPRECIP = 10.0;

unsigned int seed = 0;

float Ranf(unsigned int* seedp, float low, float high)
{
    float r = (float)rand_r(seedp);              // 0 - RAND_MAX

    return(low + r * (high - low) / (float)RAND_MAX);
}

float
SQR(float x)
{
    return x * x;
}

void Deer() {
    while (NowYear < 2028)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        int nextNumDeer = NowNumDeer;
        int carryingCapacity = (int)(NowHeight);
        if (nextNumDeer < carryingCapacity)
            nextNumDeer++;
        else
            if (nextNumDeer > carryingCapacity)
                nextNumDeer--;

        if (nextNumDeer < 0)
            nextNumDeer = 0;

        // DoneComputing barrier:
#pragma omp barrier
        NowNumDeer = nextNumDeer;


        // DoneAssigning barrier:
#pragma omp barrier

// DonePrinting barrier:
#pragma omp barrier
    }
}

void Grain() {
    while (NowYear < 2028)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        float tempFactor = exp(-SQR((NowTemp - MIDTEMP) / 10.));
        float precipFactor = exp(-SQR((NowPrecip - MIDPRECIP) / 10.));

        float nextHeight = NowHeight;

        nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

        if (nextHeight < 0.)
            nextHeight = 0.;


        // DoneComputing barrier:
#pragma omp barrier
        NowHeight = nextHeight;


        // DoneAssigning barrier:
#pragma omp barrier

// DonePrinting barrier:
#pragma omp barrier
    }
}

void Watcher() {
    while (NowYear < 2028)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:


        // DoneComputing barrier:
#pragma omp barrier

// DoneAssigning barrier:
#pragma omp barrier
        float ang = (30. * (float)NowMonth + 15.) * (M_PI / 180.);

        float temp = AVG_TEMP - AMP_TEMP * cos(ang);
        NowTemp = temp + Ranf(&seed, -RANDOM_TEMP, RANDOM_TEMP);

        float CelTemp;
        CelTemp = (NowTemp - 32.) * (5. / 9.);

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);

        NowPrecip = precip + Ranf(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);
        if (NowPrecip < 0.)
            NowPrecip = 0.;

        printf("%d,%d,%d,%lf,%f,%f\n", NowAddMonth, NowNumDeer, NowNumHunter, (NowHeight*2.54), CelTemp, (NowPrecip*2.54));


        if (NowMonth == 11) {
            NowMonth = -1;
            NowYear++;
        }

        NowAddMonth++;
        NowMonth++;




        // DonePrinting barrier:
#pragma omp barrier

    }
}

void Prohibation() {
    while (NowYear < 2028)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        int prohibit_hunter = NowNumHunter;
        int survive_deer = NowNumDeer;

        // DoneComputing barrier:
#pragma omp barrier	

        if (NowMonth % 12 == 0 || NowMonth  % 12 == 6) {
            int total = rand() % 100;
            if (total <= 55) {
                prohibit_hunter--;
                survive_deer++;
            }
            else if (total > 55) {
                prohibit_hunter++;
                survive_deer--;
            }
        }

        int food = (float)NowNumHunter * ONE_PEOPLE_HUNTS_PER_MONTH;
        if (food < survive_deer) {

            prohibit_hunter--;
            survive_deer++;
        }
        // DoneAssigning barrier:
#pragma omp barrier
        NowNumDeer = survive_deer;
        NowNumHunter = prohibit_hunter;

        // DonePrinting barrier:
#pragma omp barrier	
    }
}





int
main(int argc, char* argv[])
{
#ifndef _OPENMP
    fprintf(stderr, "No OpenMP support!\n");
    return 1;
#endif

    NowMonth = 0;

    NowYear = 2022;

    NowNumDeer = 1;
    NowHeight = 1.;
    NowNumHunter = 30;

    omp_set_num_threads(4);	// same as # of sections
#pragma omp parallel sections
    {
#pragma omp section
        {
            Deer();
        }

#pragma omp section
        {
            Grain();
        }

#pragma omp section
        {
            Watcher();
        }

#pragma omp section
        {
            Prohibation();	// your own
        }
    }       // implied barrier -- all functions must return in order
               // to allow any of them to get past here
    return 0;

}







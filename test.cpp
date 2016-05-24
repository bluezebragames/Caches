#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cmath>

#include <mach/mach.h>
#include <mach/mach_time.h>

using namespace std;

const int TESTS = 100;
const int BIGARRAYSCALE = 250;
const int NUMWARMUP = 50000;
const int ACCESSESPERTEST = 5;
const int BUCKETS = 100;
const uint64_t SIZE = 128;

// CALCULATION TIME!
// 8MB ~ 8000000B
// 8000000B / (16B/int) = 500000ints
const int NUMBLOCKS = 500000, NUMSETS = NUMBLOCKS/4;

// uint64_t newarray[128 * BIGARRAYSCALE];
uint64_t newarray[64*TESTS*128*ACCESSESPERTEST];
uint64_t now, fini;
double times[TESTS][128];
uint64_t t;
int countbucket[BUCKETS]; // 0 to 1000?



double variance(double* nums, uint64_t length) {
  double sum2 = 0, ret = 0;
  for(uint64_t i = 0; i<length; ++i) {
    sum2 += nums[i];
    ret += nums[i] * nums[i];
  }
  ret *= length;
  sum2 *= sum2;
  return (ret-sum2) / ((long long)length * (long long)length);
}

double variance(uint64_t* nums, uint64_t length) {
  double sum2 = 0, ret = 0;
  for(uint64_t i = 0; i<length; ++i) {
    sum2 += nums[i];
    ret += nums[i] * nums[i];
  }
  ret *= length;
  sum2 *= sum2;
  return (ret-sum2) / ((long long)length * (long long)length);
}

int getBucket(double index) {
  return min((int)(index / 10), BUCKETS-1);
}

unsigned long long startTSC()
{
  unsigned int hiword = 0;
  unsigned int loword = 0;

  __asm__ volatile ("cpuid\n\t"
                    "rdtsc\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t" : "=r" (hiword), "=r" (loword)::
                    "%rax", "%rbx", "%rcx", "%rdx");

  return (((unsigned long long)hiword) << 32) | loword;
}

unsigned long long endTSC()
{
  unsigned int hiword = 0;
  unsigned int loword = 0;

  __asm__ volatile ("rdtscp\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t"
                    "cpuid\n\t" : "=r" (hiword), "=r" (loword)::
                    "%rax", "%rbx", "%rcx", "%rdx");

  return (((unsigned long long)hiword) << 32) | loword;
}

int main() {
  cout << "Run option: 0 to instrument, other numbers to generate a stream\n";
  int ro;
  cin >> ro;
  const bool q = (ro != 0);

  ofstream fout("out.txt");
  fout << NUMBLOCKS << endl << NUMSETS << endl;
  srand(50);

  // TO WARM UP!
  for(uint64_t i = 0; i<NUMWARMUP; ++i) {
    uint64_t temp = (uint64_t)(((float)rand() / RAND_MAX) * SIZE * BIGARRAYSCALE);
    // t = newarray[temp];
    t = newarray[0];
  }
  startTSC();
  endTSC();
  startTSC();
  endTSC();


  int bucket;
  for(int j = 0; j<TESTS; ++j) {
    if(!q) {
      for(uint64_t i = 0; i<SIZE; ++i) {
        now = startTSC();
        for(int k = 0; k<ACCESSESPERTEST; ++k) {
          uint64_t temp = (uint64_t)(((float)rand() / RAND_MAX) * SIZE * BIGARRAYSCALE);
          // t = newarray[temp];
          t = newarray[0];
          // t = newarray[64*(k + i*ACCESSESPERTEST + j*SIZE*ACCESSESPERTEST)];
        }
        fini = endTSC();

        times[j][i] = ((double)fini-now-12)/ACCESSESPERTEST - 4;// - 14;
        bucket = getBucket(times[j][i]);
        countbucket[bucket]++;
        // 12 is overhead in calling start and end tsc()!
        // 4 is overhead in running loop 100 times!  and 500 also, and 5, apparently
        //   weird, right?
        // 14 is overhead in calculating temp!
        // hits take 0 cycles

        // 4mb (scale = 20005)
        // ll cache hit rate of about 91%
        // cycle time for 4mb array size = 25
        // 100ns*(1-.91)*2.7(cyc/ns) = 24.3 ~ 25?  maybe

        // 32k, now (scale = 250)
        // ll cache hit rate of 100%
        // cycle time for 32k array size = 2



        // we got it!!!  miss penalty is ~270-300 cycles

        // hmmm 100 * 2.7 = 270
        // coincidence?  I think not.
      }
    }
  }


  srand(50); // Seed with the same seed, to ensure the random numbers are the same
  // do the same warming up, too
  // for(uint64_t i = 0; i<NUMWARMUP; ++i) {
  //   uint64_t temp = (uint64_t)(((float)rand() / RAND_MAX) * SIZE * BIGARRAYSCALE);
  //   t = newarray[temp];
  //   fout << 0 << " " << ((long long)&newarray[temp]/8 % NUMBLOCKS) / 16 << endl;
  // }
  fout << "3 3" << endl;
  for(int j = 0; j<TESTS; ++j) {
    for(uint64_t i = 0; i<SIZE; ++i) {
      for(int k = 0; k<ACCESSESPERTEST; ++k) {
        uint64_t temp = (uint64_t)(((float)rand() / RAND_MAX) * SIZE * BIGARRAYSCALE);
        // uint64_t temp = 64*(k + i*ACCESSESPERTEST + j*SIZE*ACCESSESPERTEST);
        // ALSO
        fout << 0 << " " << ((long long)&newarray[temp]/32) / 16 << endl;
        // fout << 0 << " " << ((long long)&newarray[0]/32) / 16 << endl;
        // 32 IS FOR CONVERTING THE ADDRESSES TO SEQUENTIAL
        // 16 IS FOR THE FACT THAT CACHE LINES ARE 64 BYTES LONG, SO THEY CAN FIT 16 INTS
      }
    }
  }
  fout << "2 2 -1\n";




  if(!q) {
    uint64_t maxrange = 0, totalvar = 0, varvar = 0, varmin = 0;
    uint64_t mins[TESTS], ranges[TESTS], vars[TESTS];
    for(int i = 0; i<TESTS; ++i) {
      uint64_t range = 0, mn = -1, mx = 0;
      for(uint64_t j = 0; j<SIZE; ++j) {
        if(times[i][j] < mn || mn == (uint64_t)(-1))
          mn = times[i][j];
        if(times[i][j] > mx)
          mx = times[i][j];
      }
      range = mx - mn;
      maxrange = max(maxrange, range);
      mins[i] = mn;
      ranges[i] = range;
      vars[i] = variance(times[i], SIZE);
      totalvar += vars[i];
      printf("loop_size:%d >>>> variance(cycles): %llu; max_deviation: %llu; min time: %llu\n", i, vars[i], maxrange, mn);
    }
    varvar = variance(vars, TESTS);
    varmin = variance(mins, TESTS);

    printf("\n total variance = %llu", (totalvar/TESTS));
    printf("\n absolute range = %llu", maxrange);
    printf("\n variance of variances = %llu", varvar);
    printf("\n variance of minimum values = %llu", varmin);
  }

  cout << endl;
  int acc = 0;
  for(int i = 0; i<BUCKETS; ++i) {
    if(countbucket[i] != 0) {
      cout << i << "\t" << countbucket[i] << endl;
      acc += countbucket[i];
    }
  }
  cout << acc << endl;

  return 0;
}

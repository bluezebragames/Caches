#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cmath>

#include <mach/mach.h>
#include <mach/mach_time.h>

using namespace std;

const int TESTS = 100;

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

int main() {
  ofstream fout("out.txt");
  fout << 16 << endl << 4 << endl;
  srand(time(0));

  // // RANDOM ACCESSES
  // for(int i = 0; i<250; ++i) {
  //   double temp = rand();
  //   temp /= RAND_MAX;
  //   temp *= 20;
  //   fout << 0 << endl << (int)temp << endl;
  // }
  // // is ~75%
  // // 16/20 = .75, which is good
  // fout << 2 << " " << 2 << endl;
  // fout << "0 1 0 2 0 3 0 4 2 2 " << endl;

  // for(int i = 0; i<50; ++i) {
  //   fout << 0 << " " << i << " ";
  // }
  // // 0%, which is good
  // fout << 2 << " " << 2 << endl;


  // for(int i = 0; i<50; ++i) {
  //   fout << 0 << " " << i*i % 20 << endl;
  // }
  // // 88% (44 / 50), which is good
  // fout << 2 << " " << 2 << endl;



  // for(int i = 0; i<50; ++i) {
  //   double temp = rand();
  //   fout << 0 << " " << (int)sqrt(sqrt(sqrt(temp))) << endl;
  // }
  // fout << 2 << " " << 2 << endl;

  // int array[15];
  // for(int i = 0; i<15; ++i) {
  //   fout << 0 << " " << (long long)&array[i] % 64 << endl;
  // }
  // // 0% by itself, which is right
  // for(int i = 0; i<15; ++i) {
  //   fout << 0 << " " << (long long)&array[i] % 64 << endl;
  // }
  // // 0% together, which is right
  // fout << 2 << " " << 2 << endl;


  // RANDOM ARRAY
  uint64_t size; cin >> size;
  uint64_t newarray[size];
  uint64_t now, fini;
  uint64_t times[TESTS][size];
  unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
  uint64_t t;


  // TO WARM UP!
  asm volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                "%rax", "%rbx", "%rcx", "%rdx");
  asm volatile("RDTSCP\n\t"
               "mov %%edx, %0\n\t"
               "mov %%eax, %1\n\t"
               "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
               "%rbx", "%rcx", "%rdx");
  asm volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                "%rax", "%rbx", "%rcx", "%rdx");
  asm volatile("RDTSCP\n\t"
               "mov %%edx, %0\n\t"
               "mov %%eax, %1\n\t"
               "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
               "%rbx", "%rcx", "%rdx");



  for(int j = 0; j<TESTS; ++j) {
    for(uint64_t i = 0; i<size; ++i) {
      uint64_t temp = (uint64_t)(((float)rand() / RAND_MAX) * 20);
      // now = mach_absolute_time();
      asm volatile (
                    "CPUID\n\t"
                    "RDTSC\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r"
                    (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
      // t = newarray[temp];
      asm volatile("RDTSCP\n\t"
                   "mov %%edx, %0\n\t"
                   "mov %%eax, %1\n\t"
                   "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
                   "%rax", "%rbx", "%rcx", "%rdx");

      now = (((uint64_t)cycles_high  << 32) | cycles_low  );
      fini =(((uint64_t)cycles_high1 << 32) | cycles_low1 );
      times[j][i] = fini-now;
      // ALSO
      fout << 0 << endl << (long long)&newarray[temp]/8 % 64 << endl;
    }
  }



  uint64_t maxrange = 0, totalvar = 0, varvar = 0, varmin = 0;
  uint64_t mins[TESTS], ranges[TESTS], vars[TESTS];
  for(int i = 0; i<TESTS; ++i) {
    uint64_t range = 0, mn = -1, mx = 0;
    for(uint64_t j = 0; j<size; ++j) {
      if(times[i][j] < mn || mn == (uint64_t)(-1))
        mn = times[i][j];
      if(times[i][j] > mx)
        mx = times[i][j];
    }
    range = mx - mn;
    maxrange = max(maxrange, range);
    mins[i] = mn;
    ranges[i] = range;
    vars[i] = variance(times[i], size);
    totalvar += vars[i];
    printf("loop_size:%d >>>> variance(cycles): %llu; max_deviation: %llu; min time: %llu\n", i, vars[i], maxrange, mn);
  }
  varvar = variance(vars, TESTS);
  varmin = variance(mins, TESTS);

  printf("\n total variance = %llu", (totalvar/TESTS));
  printf("\n absolute range = %llu", maxrange);
  printf("\n variance of variances = %llu", varvar);
  printf("\n variance of minimum values = %llu", varmin);





  fout << 2 << " " << 2 << endl;

  // for(int i = 0; i<size; ++i) {
  //   fout << 0 << endl << 0 << endl;
  // }
  // fout << 2 << " " << 2 << endl;

  fout << -1;

  return 0;
}

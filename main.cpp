#include <iostream>
#include <fstream>

#include "cache.h"

#include <mach/mach.h>
#include <mach/mach_time.h>

using namespace std;

static mach_timebase_info_data_t timebase_info;

int main() {
  ifstream fin("out.txt");

  // MEMORY SIZE IS FIXED AT 150
  int numblocks, numsets;
  cout << "number of blocks?\n";
  fin >> numblocks;

  cout << "number of sets?\n";
  fin >> numsets;

  if(numblocks % numsets != 0) {
    cout << "that number of sets in those blocks isn't going to work, you know\nyour blocks should be divisible by your sets\n";
    return 0;
  }

  Cache ch = Cache(numblocks, numsets);
  ch.print();

  long long address;
  int rw = 0;
  // START THE TIME!!!
  mach_timebase_info(&timebase_info);
  uint64_t now = mach_absolute_time();
  uint64_t fini;
  int counter = 0;
  while(rw != -1) {
    cout << rw << " ";
    // cout << "input 0 for read, 1 for write, -1 to quit\n";
    fin >> rw;
    // cout << "input some addresses\n";
    fin >> address;
    counter++;
    while(address >= ch.MEMSIZE)
      {cout << "try again\n"; fin >> address;}
    switch (rw) {
    case 0:
      // cout << ch.FindA(address) << " ";
      ch.FindA(address);
      break;
    // case 1:
    //   cout << "what do you want to write?";
    //   int toWrite; fin >> toWrite;
    //   ch.WriteA(address, toWrite);
    //   break;
    case 2:
      fini = mach_absolute_time();
      cout << endl << fini - now << " nanoseconds = " << (1.0 * fini - 1.0 * now) / 1000000 << " milliseconds\n";
      cout << "so, the average cache access time is " << 1.0 * (fini - now) / counter << " nanoseconds";
      cout << endl;
      ch.print();
      ch.clear();
      now = mach_absolute_time();
      cout << "we're back!" << endl;
      break;
    default:
      break;
    }
    cout << "we're back!" << endl;
    // ch.print();
  }

  return 0;
}

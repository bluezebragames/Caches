#include <iostream>
#include <fstream>

#include "cache.h"

using namespace std;

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
  int counter = 0;
  while(rw != -1) {
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
      ch.print();
      ch.clear();
      break;
    default:
      break;
    }
    // ch.print();
  }

  return 0;
}

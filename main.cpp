#include <iostream>
#include <fstream>

#include "cache.hpp"

using namespace std;

int main() {
  ifstream fin("out.txt");

  // MEMORY SIZE IS FIXED AT 150
  long long numblocks, numsets;
  cout << "number of blocks?\n";
  fin >> numblocks;

  cout << "number of sets?\n";
  fin >> numsets;

  if(numblocks % numsets != 0) {
    cout << "that number of sets in those blocks isn't going to work, you know\nyour blocks should be divisible by your sets\n";
    return 0;
  }

  // create the cache model
  Cache ch = Cache(numblocks, numsets);

  long long address;
  long long rw = 0;
  long long counter = 0;
  while(rw != -1) {
    fin >> rw >> address;
    address /= 16; // compression!
    counter++;
    // if(counter % 4000 == 0) {cout << counter << "!\n";}
    switch (rw) {
    case 0:
      // cout << ch.FindA(address) << " ";
      ch.FindA(address);
      break;
    // case 1:
    //   cout << "what do you want to write?";
    //   long long toWrite; fin >> toWrite;
    //   ch.WriteA(address, toWrite);
    //   break;
    case 2:
      ch.print();
      ch.clear();
      break;
    case 3:
      // ch.print();
      ch.hmzero();
    default:
      break;
    }
    // ch.print();
  }

  return 0;
}

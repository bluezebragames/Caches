#include <iostream>

#include "cache.h"

using namespace std;

int main() {

  int numblocks, numsets;

  // take cache size
  cout << "number of blocks?\n";
  cin >> numblocks;

  cout << "number of sets?\n";
  cin >> numsets;

  if(numblocks % numsets != 0) {
    cout << "that number of sets in those blocks isn't going to work, you know\nyour blocks should be divisible by your sets\n";
    return 0;
  }

  Cache ch = Cache(numblocks, numsets);

  // take addresses

  // if key-value pairs

  int address = 0, rw, value, numindexes, index;
  cout << "number of indexes?\n";
  cin >> numindexes;
  ch.indexes.resize(numindexes);
  while(true) {
    ch.print();
    cout << "input 0 for read, 1 for write, -1 to quit\n";
    cin >> rw;
    if(rw == 0) {
      cout << "input address\n";
      cin >> address;
      cout << ch.FindDMk(address) << endl;
      // cout << ch.FindAk(address) << endl;
    }
    else if(rw == 1) {
      cout << "input address\n";
      cin >> address;
      cout << "input value\n";
      cin >> value;
      // writethrough or writeback?
      index = ch.FindDMk(address);
      if(ch.hit) {
        cout << "writethrough (0) or writeback (1)?\n";
        bool tb;
        cin >> tb;
        if(tb == 0) {
          // writethrough
          // set cache index to value as well
          ch.setAddressesk(address, index, value);
        }
        else {
          // writeback
          // don't have to do anything else
        }
      }
      ch.indexes[address] = value;
    }
    else {break;}
  }


  // if pointers

  // cout << "input addresses, -1 to quit\n";
  // // long long address;
  // int address;
  // cin >> address;
  // while(address != -1) {
  //   cout << ch.FindA(&address) << endl;
  //
  //   // //cout << ch.FindDM((int *)address) << endl;
  //   // cout << address << endl;
  //   // cout << (int *) address << endl;
  //   // cout << *(int *) address << endl;
  //   cin >> address;
  // }

  return 0;
}

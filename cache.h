#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <vector>

using namespace std;

struct entry {
  int tag;
  int index;
  bool valid;
};

class Cache {
 public:
  Cache(int, int);
  int FindDM(int);
  int FindA(int);
  void WriteDM(int, int);
  void WriteA(int, int);

  void print();
  static const int MEMSIZE = 150;

 private:
  int MOD, numblocks, numsets, hits = 0, misses = 0;
  vector<entry> cache;
  vector<int> data;
  int memory[MEMSIZE];
};

#endif

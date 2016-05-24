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
  ~Cache();
  // RESTART
  void clear();
  void hmzero();
  // FIND
  int FindDM(int);
  int FindA(int);
  // WRITE
  void WriteDM(int, int);
  void WriteA(int, int);
  // REPLACEMENT
  int RAND(int, entry);
  int NMRU(int, entry);
  int LRU(int, entry);
  void touchLRU(int, int);

  void print();
  static const int MEMSIZE = 1500;

private:
  int MOD, numblocks, numsets, hits, misses;
  vector<entry> cache;
  vector<int> data;
  vector<int> mru;
  // usage: mru[set] = index
  vector<vector<int> > lru;
  // usage: lru[set][index] = which place it occupies
  int memory[MEMSIZE];

  inline int convAddress(int address, int i) {
    return address%MOD*(numblocks/numsets) + i;
  }
};

#endif

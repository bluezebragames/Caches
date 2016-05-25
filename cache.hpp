#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <vector>

using namespace std;

struct entry {
  long long tag;
  long long index;
  bool valid;
};

class Cache {
public:
  Cache(long long, long long);
  ~Cache();
  // RESTART
  void clear();
  void hmzero();
  // FIND
  long long FindDM(long long);
  long long FindA(long long);
  // WRITE
  void WriteDM(long long, long long);
  void WriteA(long long, long long);
  // REPLACEMENT
  long long RAND(long long, entry);
  long long NMRU(long long, entry);
  long long LRU(long long, entry);
  void touchLRU(long long, long long);

  void print();
  static const long long MEMSIZE = 1500;

private:
  long long MOD, numblocks, numsets, hits, misses;
  vector<entry> cache;
  vector<long long> data;
  vector<long long> mru;
  // usage: mru[set] = index
  vector<vector<long long> > lru;
  // usage: lru[set][index] = which place it occupies
  long long memory[MEMSIZE];

  inline long long convAddress(long long address, long long i) {
    return address%MOD*(numblocks/numsets) + i;
  }
};

#endif

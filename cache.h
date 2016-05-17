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
  // FIND
  int FindDM(int);
  int FindA(int);
  // WRITE
  void WriteDM(int, int);
  void WriteA(int, int);

  // REPLACEMENT
  int RAND(int, entry);
  int NMRU (int, entry);

  void print();
  static const int MEMSIZE = 150;

 private:
  int MOD, numblocks, numsets, hits, misses;
  vector<entry> cache;
  vector<int> data;
  vector<int> mru;
  int memory[MEMSIZE];
};

#endif

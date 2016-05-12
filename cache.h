#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <vector>

using namespace std;

class Cache {
 public:
  Cache(int, int);
  int FindDMk(int);
  int FindAk(int);

  void setAddressesk(int, int, int);

  void print();

  vector<int> indexes;
  bool hit;
 private:
  int numblocks, numsets, hits = 0, misses = 0;
  vector<pair<int, int> > addressesk;
  vector<int> repls;
};

#endif

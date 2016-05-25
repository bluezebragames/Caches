#include "cache.hpp"

#include <cmath>
#include <cstdlib>
#include <ctime>
// initialize the cache, set up the vectors for addresses and repls
Cache::Cache(long long nb, long long ns) {
  srand(time(0));
  numblocks = nb;
  numsets = ns;
  hits = 0;
  misses = 0;
  entry temp;
  temp.tag = -1;
  temp.index = -1;
  temp.valid = false;
  cache.resize(numblocks, temp);
  mru.resize(numsets, -1);
  vector<long long> tempv;
  tempv.resize(numblocks/numsets, 0);
  lru.resize(numsets, tempv);
  data.resize(numblocks, -1);
  memset(memory, 0, MEMSIZE * sizeof(long long));
}

Cache::~Cache() {
}


void Cache::clear() {
  hits = 0;
  misses = 0;
  cache.clear();
  mru.clear();
  for(long long i = 0; i<(long long)lru.size(); ++i) {
    lru[i].clear();
  }
  lru.clear();
  data.clear();
  entry temp;
  temp.tag = -1;
  temp.index = -1;
  temp.valid = false;
  cache.resize(numblocks, temp);
  mru.resize(numsets, -1);
  vector<long long> tempv;
  tempv.resize(numblocks/numsets, 0);
  lru.resize(numsets, tempv);
  data.resize(numblocks, -1);
  memset(memory, -1, MEMSIZE * sizeof(long long));
}

void Cache::hmzero() {
  hits = 0;
  misses = 0;
}


// find an element in the cache, assuming that the cache is direct-mapped
long long Cache::FindDM(long long address) {
  MOD = numblocks;
  // just in case numsets gets filled out while doing direct-map
  // if the tags match and the entry is valid, you've hit
  if(cache[address%MOD].tag == address/MOD && cache[address%MOD].valid == true) {
    // cout << "Hit!\n";
    hits++;
    return address%MOD;
  }
  // cout << "Miss\n";
  misses++;
  // create a new entry with the address info, and put it in the cache
  entry temp;
  temp.tag = address/MOD;
  temp.index = address%MOD;
  temp.valid = true;
  cache[address%MOD] = temp;
  // data[address%MOD] = memory[address];
  data[address%MOD] = 0;
  return address%MOD;
}

// find an element in the cache, assuming the cache is set-associative
long long Cache::FindA(long long address) {
  MOD = numsets;  // it's different for DM and A
  long long invalid = -1; // if there is an invalid block in the cache and we miss, we can replace the invalid with the new block
  // check all of the possible places it could be
  for(long long i = 0; i<numblocks/numsets; ++i) {
    // blocks are separated by a distance of 1 and are at an 'offset' of address%MOD * numblocks/numsets
    if(cache[convAddress(address,i)].tag == address/MOD && cache[convAddress(address,i)].valid == true) {
      // HIT!
      // cout << "Hit!\n";
      hits++;
      // update mru with this new block
      mru[address%MOD] = i;
      touchLRU(address%MOD, i);
      return convAddress(address, i);
    }
    // if we haven't found an invalid block and this one is invalid, then this is the invalid one that we've now found
    if(invalid == -1 && cache[convAddress(address, i)].valid == false) {
      invalid = convAddress(address, i);
    }
  }
  // Nope, it wasn't there, we'd better add it
  // cout << "Miss\n";
  misses++;

  entry temp;
  temp.tag = address/MOD;
  temp.index = address%MOD;
  temp.valid = true;

  if(invalid != -1) {
    // we found an invalid entry
    cache[invalid] = temp;
    // data[invalid] = memory[address];
    data[invalid] = 0;
    mru[address%MOD] = invalid/MOD;
    touchLRU(address%MOD, invalid/MOD);
    return invalid;
  }

  return LRU(address, temp);
}

void Cache::touchLRU(long long i, long long j) {
  long long curr = lru[i][j];
  for(long long k = 0; k<(long long)lru[i].size(); ++k) {
    if(lru[i][k] > curr)
      lru[i][k]--;
  }
  lru[i][j] = numblocks / numsets - 1;
}

long long Cache::LRU(long long address, entry temp) {
  long long i;
  for(i = 0; i<numblocks / numsets; ++i) {
    if(lru[address%MOD][i] == 0) break;
  }
  cache[convAddress(address, i)] = temp;
  // data[convAddress(address, i)] = memory[address];
  data[convAddress(address, i)] = 0;
  touchLRU(address%MOD, i);
  return convAddress(address, i);
}

long long Cache::NMRU(long long address, entry temp) {
  double i;
  long long counter = 0;
  do {
    i = rand();
    i /= (long long)RAND_MAX+1;
    i *= numblocks/numsets;
    i = (long long)i;
    counter++;
  } while (i == mru[address%MOD] && numsets != 1);
  // the only problem is that I have no clue if this works
  cache[convAddress(address, i)] = temp;
  // data[convAddress(address, i)] = memory[address];
  data[convAddress(address, i)];
  mru[address%MOD] = i;
  return convAddress(address, i);
}

// replace a random
long long Cache::RAND(long long address, entry temp) {
  double i = rand();
  i /= (long long)RAND_MAX+1;
  i *= numsets;
  i = (long long)i;

  cache[convAddress(address, i)] = temp;
  // data[convAddress(address, i)] = memory[address];
  data[convAddress(address, i)] = 0;
  return convAddress(address, i);
}


void Cache::WriteDM (long long address, long long toWrite) {
  FindDM(address);
  data[address%MOD] = toWrite;
  memory[address] = toWrite;
}

void Cache::WriteA (long long address, long long toWrite) {
  long long place = FindA(address);
  data[place] = toWrite;
  memory[address] = toWrite;
}




// DEBUG
void Cache::print() {
  // prlong longs the contents of the cache: tag, index, data
  // for(long long i = 0; i<(long long)cache.size(); ++i) {
  //   cout << cache[i].tag << "\t" << cache[i].index << "\t" << data[i] << "\n";
  // }
  // // outputs mrus
  // for(long long i = 0; i<(long long)mru.size(); ++i) {
  //   cout << "MRU " << i << ": " << mru[i] << endl;
  // }
  // // outputs lrus
  // for(long long i = 0; i<(long long)lru.size(); ++i) {
  //   cout << "LRU " << i << ": ";
  //   for(long long j = 0; j<(long long)lru[i].size(); ++j) {
  //     cout << lru[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // prlong longs general information about the cache
  cout << "This cache is " << (numsets == numblocks ? ("direct-mapped ") : (numsets == 1 ? ("fully associative ") : (to_string(numblocks/numsets) + "-way associative "))) << "with " << numsets << " sets and " << numblocks << " blocks." << endl;
  // prlong longs hit rate
  cout << "Current hit rate is: " << (100.0 * hits / (hits + misses)) << "%\n";
  cout << hits << " hits, " << misses << " misses, for a total of " << hits+misses << " queries\n";
  // prlong longs cache utilization
  long long used = 0;
  for(long long i = 0; i<(long long)cache.size(); ++i) {
    if(cache[i].tag == -1){used++;}
  }
  cout << "Cache utilization: " << cache.size() - used << " cache lines were used, out of " << cache.size() << endl;
}

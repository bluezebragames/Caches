#include "cache.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
// initialize the cache, set up the vectors for addresses and repls
Cache::Cache(int nb, int ns) {
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
  vector<int> tempv;
  tempv.resize(numblocks/numsets, 0);
  lru.resize(numsets, tempv);
  data.resize(numblocks, -1);
  memset(memory, -1, MEMSIZE * sizeof(int));
}

Cache::~Cache() {
}


void Cache::clear() {
  cout << "Starting clear\n";
  hits = 0;
  misses = 0;
  cache.clear();
  mru.clear();
  for(int i = 0; i<(int)lru.size(); ++i) {
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
  vector<int> tempv;
  tempv.resize(numblocks/numsets, 0);
  lru.resize(numsets, tempv);
  data.resize(numblocks, -1);
  memset(memory, -1, MEMSIZE * sizeof(int));
  cout << "Ending clear\n";
  print();
}


// find an element in the cache, assuming that the cache is direct-mapped
int Cache::FindDM(int address) {
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
  // create a new entry with the address info (and fake data)
  entry temp;
  temp.tag = address/MOD;
  temp.index = address%MOD;
  temp.valid = true;
  cache[address%MOD] = temp;
  data[address%MOD] = memory[address];
  return address%MOD;
}

// find an element in the cache, assuming it's set-associative
int Cache::FindA(int address) {
  MOD = numblocks / numsets;  // it's different for DM and A
  int invalid = -1; // if there is an invalid block in the cache and we miss, we can replace the invalid with the new block
  // check all of the possible places it could be
  for(int i = 0; i<numsets; ++i) {
    // blocks are separated by a distance of (numblocks / numsets) and are at an 'offset' of address%MOD
    if(cache[i * numblocks / numsets + address%MOD].tag == address/MOD && cache[i * numblocks / numsets + address%MOD].valid == true) {
      // HIT!
      // cout << "Hit!\n";
      hits++;
      // update mru with this new block
      mru[i] = i * numblocks / numsets + address%MOD;
      touchLRU(i, address%MOD);
      return i * numblocks / numsets + address%MOD;
    }
    // if we haven't found an invalid block and this one is invalid, then this is the invalid one that we've now found
    if(invalid == -1 && cache[i * numblocks / numsets + address%MOD].valid == false) {
      invalid = i * numblocks / numsets + address%MOD;
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
    data[invalid] = memory[address];
    mru[invalid / MOD] = invalid;
    touchLRU(invalid / MOD, invalid%MOD);
    return invalid;
  }

  // TODO: CORRECTLY IMPLEMENT LRU
  return LRU(address, temp);
}

void Cache::touchLRU(int i, int j) {
  int curr = lru[i][j];
  for(int k = 0; k<(int)lru[i].size(); ++k) {
    if(lru[i][k] > curr)
      lru[i][k]--;
  }
  lru[i][j] = numblocks / numsets - 1;
}

int Cache::LRU(int address, entry temp) {
  int i;
  for(i = 0; i<numblocks / numsets; ++i) {
    if(lru[address%MOD][i] == 0) break;
  }
  cache[i*numblocks/numsets+address%MOD] = temp;
  data[i*numblocks/numsets+address%MOD] = memory[address];
  touchLRU(address%MOD, i);
  return i*numblocks/numsets+address%MOD;
}

int Cache::NMRU(int address, entry temp) {
  double i;
  int counter = 0;
  do {
    i = rand();
    i /= (long long)RAND_MAX+1;
    i *= numsets;
    i = (int)i;
    counter++;
  } while (i*numblocks/numsets+address%MOD == mru[address%MOD] &&
           numsets != 1);
  // the only problem is that I have no clue if this works
  cache[i * numblocks / numsets + address%MOD] = temp;
  data[i * numblocks / numsets + address%MOD] = memory[address];
  mru[i] = i * numblocks / numsets + address%MOD;
  return i * numblocks / numsets + address%MOD;
}

// replace a random
int Cache::RAND(int address, entry temp) {
  double i = rand();
  i /= (long long)RAND_MAX+1;
  i *= numsets;
  i = (int)i;

  cache[i * numblocks / numsets + address%MOD] = temp;
  data[i * numblocks / numsets + address%MOD] = memory[address];
  return i * numblocks / numsets + address%MOD;
}


void Cache::WriteDM (int address, int toWrite) {
  FindDM(address);
  data[address%MOD] = toWrite;
  memory[address] = toWrite;
}

void Cache::WriteA (int address, int toWrite) {
  int place = FindA(address);
  data[place] = toWrite;
  memory[address] = toWrite;
}






// DEBUG
void Cache::print() {
  // prints the contents of the cache: tag, index, data
  int used = 0;
  for(int i = 0; i<(int)cache.size(); ++i) {
    cout << cache[i].tag << "\t" << cache[i].index << "\t" << data[i] << "\n";
    if(cache[i].tag == -1){used++;}
  }
  // outputs mrus
  for(int i = 0; i<(int)mru.size(); ++i) {
    cout << "MRU " << i << ": " << mru[i] << endl;
  }
  // outputs lrus
  for(int i = 0; i<(int)lru.size(); ++i) {
    cout << "LRU " << i << ": ";
    for(int j = 0; j<(int)lru[i].size(); ++j) {
      cout << lru[i][j] << " ";
    }
    cout << endl;
  }
  // prints general information about the cache
  cout << "This cache is " << (numsets == 1 ? ("direct-mapped ") : (numsets == numblocks ? ("fully associative ") : (to_string(numsets) + "-way associative "))) << "with " << numsets << " sets and " << numblocks << " blocks." << endl;
  // prints hit rate
  cout << "Current hit rate is: " << (100.0 * hits / (hits + misses)) << "%\n";
  cout << hits << " hits, " << misses << " misses, for a total of " << hits+misses << " queries\n";
  // prints cache utilization
  cout << "Cache utilization: " << cache.size() - used << " cache lines were used, out of " << cache.size() << endl;
}

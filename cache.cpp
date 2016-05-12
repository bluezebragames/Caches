#include "cache.h"

#include <cstdlib>
#include <ctime>

// initialize the cache, set up the vectors for addresses and repls
Cache::Cache(int nb, int ns) {
  srand(time(0));
  numblocks = nb;
  numsets = ns;
  addressesk.resize(nb);
  repls.resize(ns, 0);
}

// see if the cache has the address -- using direct-map
int Cache::FindDMk(int ind) {
  pair<int, int> temp = addressesk[ind % numblocks];
  if(temp.first == ind) {
    cout << "Hit!\n";
    hit = true;
    hits++;
    return temp.first;
  }
  cout << "Miss\n";
  hit = false;
  misses++;
  addressesk[ind % numblocks] = make_pair(ind, indexes[ind]);
  return ind % numblocks;
}

// see if the cache has the address -- using set-associative
int Cache::FindAk(int ind) {
  int temp = ind % numsets;
  // HIT?
  for(int i = 0; i<numblocks / numsets; ++i) {
    if(addressesk[temp + i].first == ind) {
      cout << "Hit!\n";
      hit = true;
      hits++;
      return addressesk[temp + i].second;
    }
  }
  // MISS
  cout << "Miss\n";
  hit = false;
  misses++;
  int ret = indexes[ind];
  if(repls[temp] < numsets) {
    // the set isn't full yet
    addressesk[temp * numsets + repls[temp]] = make_pair(ind, ret);
    repls[temp]++;
  }
  else {
    // the set's full, use lru or not mru or random or vic-next vic
    // but I'm lazy right now
    addressesk[temp * numsets + repls[temp] - numsets] = make_pair(ind, ret);
    repls[temp] = (int)(rand() * numsets) + numsets;
  }

  return ret;
}



// SETTERS/GETTERS
void Cache::setAddressesk(int address, int index, int value) {
  addressesk[index] = make_pair(address, value);
}




// DEBUG
void Cache::print() {
  for(int i = 0; i<(int)addressesk.size(); ++i) {
    cout << addressesk[i].first << " " << addressesk[i].second << "\t";
  }
  cout << endl;
  cout << "Current hit rate is: " << (100.0 * hits / (hits + misses)) << "%\n";
}

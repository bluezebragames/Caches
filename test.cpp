#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cmath>

using namespace std;

int main() {
  ofstream fout("out.txt");
  fout << 16 << endl << 4 << endl;
  srand(time(0));
  // // RANDOM ACCESSES
  // for(int i = 0; i<250; ++i) {
  //   double temp = rand();
  //   temp /= RAND_MAX;
  //   temp *= 20;
  //   fout << 0 << endl << (int)temp << endl;
  // }
  // // is ~75%
  // // 16/20 = .75, which is good
  // fout << 2 << " " << 2 << endl;

  // for(int i = 0; i<50; ++i) {
  //   fout << 0 << " " << i << " ";
  // }
  // // 0%, which is good
  // fout << 2 << " " << 2 << endl;


  // for(int i = 0; i<50; ++i) {
  //   fout << 0 << " " << i*i % 20 << endl;
  // }
  // // 88% (44 / 50), which is good
  // fout << 2 << " " << 2 << endl;



  // for(int i = 0; i<50; ++i) {
  //   double temp = rand();
  //   fout << 0 << " " << (int)sqrt(sqrt(sqrt(temp))) << endl;
  // }
  // fout << 2 << " " << 2 << endl;

  // int array[15];
  // for(int i = 0; i<15; ++i) {
  //   fout << 0 << " " << (long long)&array[i] % 64 << endl;
  // }
  // // 0% by itself, which is right
  // for(int i = 0; i<15; ++i) {
  //   fout << 0 << " " << (long long)&array[i] % 64 << endl;
  // }
  // // 0% together, which is right
  // fout << 2 << " " << 2 << endl;


  // RANDOM ARRAY
  int size; cin >> size;
  int newarray[size];
  for(int i = 0; i<size; ++i) {
    double temp = rand();
    temp /= RAND_MAX;
    temp *= 20;
    fout << 0 << endl << (long long)&newarray[(int)temp] % 64 << endl;
  }
  fout << 2 << " " << 2 << endl;

  for(int i = 0; i<size; ++i) {
    fout << 0 << endl << 0 << endl;
  }
  fout << 2 << " " << 2 << endl;

  fout << -1;

  return 0;
}

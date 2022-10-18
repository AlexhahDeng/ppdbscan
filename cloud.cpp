#include "cloud.h"

vector<cpp_int> cloud::calculateEF(cpp_int x, cpp_int y, int beaverIdx){
  // e = x - a, f = y - b
  return vector<cpp_int>{(x-beaverlist[beaverIdx][0] + RING) % RING, (y-beaverlist[beaverIdx][1] + RING) % RING };
}

cpp_int cloudOne::calculateXmulY(cpp_int e, cpp_int f, int bIdx) {
  //(xy)A = f*a + e*b + c
  return (f*beaverlist[bIdx][0] + e*beaverlist[bIdx][1] + beaverlist[bIdx][2] + RING) % RING;
}

cpp_int cloudTwo::calculateXmulY(cpp_int e, cpp_int f, int bIdx) {
  //(xy)B = e*f + f*a + e*b + c
  return (e*f + f*beaverlist[bIdx][0] + e*beaverlist[bIdx][1] + beaverlist[bIdx][2] + RING) % RING;
}

vector<vector<cpp_int>> cloud::calculateEFPairs(vector<cpp_int>&xs, vector<cpp_int>&ys, int bIdx) {
  int len = xs.size();
  for(int i=0;i<len;++i){
    xs[i] = (xs[i] - beaverlist[bIdx][0] + RING) % RING;
    ys[i] = (ys[i] - beaverlist[bIdx][1] + RING) % RING;
  }
  return vector<vector<cpp_int>>{xs, ys};
}

vector<cpp_int> cloudOne::calculateXmulYPairs(vector<vector<cpp_int>> &efPairs, int bIdx) {
  vector<cpp_int>res(efPairs.size());
  for(int i=0;i<efPairs.size();++i){
    cpp_int e = efPairs[i][0];
    cpp_int f = efPairs[i][1];
    res[i] = f*beaverlist[bIdx][0] + e*beaverlist[bIdx][1] + beaverlist[bIdx][2];
  }
  return res;
}

vector<cpp_int> cloudTwo::calculateXmulYPairs(vector<vector<cpp_int>> &efPairs, int bIdx) {
  vector<cpp_int>res(efPairs.size());
  for(int i=0;i<efPairs.size();++i){
    cpp_int e = efPairs[i][0];
    cpp_int f = efPairs[i][1];
    res[i] = e*f + f*beaverlist[bIdx][0] + e*beaverlist[bIdx][1] + beaverlist[bIdx][2];
  }
  return res;
}


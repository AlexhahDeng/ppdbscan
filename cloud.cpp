#include "cloud.h"

vector<long long> cloud::calculateEF(long long x, long long y, int beaverIdx){
  // e = x - a, f = y - b
  return vector<long long>{x-beaverlist[beaverIdx][0], y-beaverlist[beaverIdx][1]};
}

long long cloudOne::calculateXmulY(long long e, long long f, int bIdx) {
  //(xy)A = f*a + e*b + c
  return f*beaverlist[bIdx][0] + e*beaverlist[bIdx][1] + beaverlist[bIdx][2];
}

long long cloudTwo::calculateXmulY(long long e, long long f, int bIdx) {
  //(xy)B = e*f + f*a + e*b + c
  return e*f + f*beaverlist[bIdx][0] + e*beaverlist[bIdx][1] + beaverlist[bIdx][2];
}
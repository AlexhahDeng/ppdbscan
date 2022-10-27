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

vector<vector<long long>> cloud::calculateEFPairs(vector<long long>&xs, vector<long long>&ys, int bIdx) {
  int len = xs.size();
  for(int i=0;i<len;++i){
    xs[i] -= beaverlist[bIdx][0];
    ys[i] -= beaverlist[bIdx][1];
  }
  return vector<vector<long long>>{xs, ys};
}

vector<long long> cloudOne::calculateXmulYPairs(vector<vector<long long>> &efPairs, int bIdx) {
  vector<long long>res(efPairs.size());
  for(int i=0;i<efPairs.size();++i){
    long long e = efPairs[i][0];
    long long f = efPairs[i][1];
    res[i] = f*beaverlist[bIdx][0] + e*beaverlist[bIdx][1] + beaverlist[bIdx][2];
  }
  return res;
}

vector<long long> cloudTwo::calculateXmulYPairs(vector<vector<long long>> &efPairs, int bIdx) {
  vector<long long>res(efPairs.size());
  for(int i=0;i<efPairs.size();++i){
    long long e = efPairs[i][0];
    long long f = efPairs[i][1];
    res[i] = e*f + f*beaverlist[bIdx][0] + e*beaverlist[bIdx][1] + beaverlist[bIdx][2];
  }
  return res;
}

vector<vector<int>> cloud::calculate_v0v1_ef(vector<vector<int>> vec)
{
    // vec 第一行为x, 第二行为y
    vector<vector<int>> ef(vec);

    for (int i = 0; i < vec[0].size(); i++)
    {
        ef[0][i] = vec[0][i] - beaverlist[i][0];
        ef[1][i] = vec[1][i] - beaverlist[i][1];
    }

    return ef;
}

vector<int> cloudOne::calculate_v0v1_final(vector<vector<int>> ef)
{
    vector<int> result(ef[0].size());

    for (int i = 0; i < ef[0].size(); i++)
    {
        result[i] = ef[0][i] * beaverlist[i][1] + ef[1][i] * beaverlist[i][0] + beaverlist[i][2];
    }

    return result;
}

vector<int> cloudTwo::calculate_v0v1_final(vector<vector<int>> ef)
{
    vector<int> result(ef[0].size());

    for (int i = 0; i < ef[0].size(); i++)
    {
        result[i] = ef[0][i] * ef[1][i] + ef[0][i] * beaverlist[i][1] + ef[1][i] * beaverlist[i][0] + beaverlist[i][2];
    }

    return result;
}
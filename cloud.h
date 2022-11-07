#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <sstream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <climits>
#include <list>
#include <map>
#include <algorithm>

 #include "BuildingBlocks/aux-protocols.h"
 #include "utils/emp-tool.h"

#include <limits>

// #include <boost/multiprecision/cpp_int.hpp>

// using namespace boost::multiprecision;

using namespace std;
using namespace sci;

const long long RING = pow(2, 48); // 秘密共享环的大小

struct dataset {
  string filepath;
  long long eps;
  int minPts;
  string filename;
};

class pairInfo {
public:
  int cluid1;
  int cluid2;
  int isConnected;

  pairInfo(int c1, int c2, int isc):cluid1(c1), cluid2(c2), isConnected(isc){}
};

class point
{
public:
    vector<long long> data; // enc: secret share data
    int cluIdx;             // enc: cluster index
    int type = 0;               // plx: 类型，如果有的话
    int isCorePoint = 0;        // enc: whether is a core point
    int isMark = 0;             // enc: whether it is marked
    int resCluIdx = 0;          // enc: dfs以后获取的结果

    point(vector<long long> tdata, int tcluIdx) : data(tdata), cluIdx(tcluIdx)
    {

    }
};

class cloud
{
public:
    vector<point*> plist;          // point list
    int minpts;                     // params
    int dim;                        // dimension of data
    long long eps;                  // params
    list<pairInfo*>resPairs;
    vector<vector<int>> beaverlist; // multiplication triple sets
    vector<vector<int>> distinfo;   // whether two point is close to each other

    cloud(vector<point*> tplist, int tminpts, long long teps) : plist(tplist), minpts(tminpts), eps(teps)
    {
    }
    cloud() {}

    vector<long long> calculateEF(long long x, long long y, int beaverIdx);
    long long calculateXmulY(long long e, long long f, int bIdx);

    vector<vector<long long>> calculateEFPairs(vector<long long>&xs, vector<long long>&ys, int bIdx);
    vector<long long> calculateXmulYPairs(vector<vector<long long>> &efPairs, int bIdx);

    vector<vector<int>> calculate_v0v1_ef(vector<vector<int>> vec);

};

class cloudOne: public cloud
{
public:
  cloudOne(){}
  long long calculateXmulY(long long e, long long f, int bIdx);

    vector<vector<long long>> calculateEFPairs(vector<long long>&xs, vector<long long>&ys, int bIdx);
        vector<long long> calculateXmulYPairs(vector<vector<long long>> &efPairs, int bIdx);

        vector<int> calculate_v0v1_final(vector<vector<int>> ef);


};

class cloudTwo: public cloud {
public:
  cloudTwo(){}
  long long calculateXmulY(long long e, long long f, int bIdx);

      vector<vector<long long>> calculateEFPairs(vector<long long>&xs, vector<long long>&ys, int bIdx);
          vector<long long> calculateXmulYPairs(vector<vector<long long>> &efPairs, int bIdx);

    vector<int> calculate_v0v1_final(vector<vector<int>> ef);
};
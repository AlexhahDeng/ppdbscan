#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <sstream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <climits>
#include <list>
#include <map>
#include <limits>

#include <boost/multiprecision/cpp_int.hpp>

// #include "BuildingBlocks/aux-protocols.h"
// #include "utils/emp-tool.h"

using namespace std;
using namespace boost::multiprecision;
// using namespace sci;

const cpp_int RING = (long long int)(pow(2, 48)); // 秘密共享环的大小

struct dataset {
  string filename;
  cpp_int eps;
  int minPts;
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
    vector<cpp_int> data; // enc: secret share data
    int cluIdx;             // enc: cluster index
    int type = 0;               // plx: 类型，如果有的话
    int isCorePoint = 0;        // enc: whether is a core point
    int isMark = 0;             // enc: whether it is marked
    int resCluIdx = 0;          // enc: dfs以后获取的结果

    point(vector<cpp_int> tdata, int tcluIdx) : data(tdata), cluIdx(tcluIdx)
    {

    }
};

class cloud
{
public:
    vector<point*> plist;          // point list
    int minpts;                     // params
    int dim;                        // dimension of data
    cpp_int eps;                  // params
    list<pairInfo*>resPairs;
    vector<vector<int>> beaverlist; // multiplication triple sets
    vector<vector<int>> distinfo;   // whether two point is close to each other

    cloud(vector<point*> tplist, int tminpts, cpp_int teps) : plist(tplist), minpts(tminpts), eps(teps)
    {
    }
    cloud() {}

    vector<cpp_int> calculateEF(cpp_int x, cpp_int y, int beaverIdx);
    cpp_int calculateXmulY(cpp_int e, cpp_int f, int bIdx);

    vector<vector<cpp_int>> calculateEFPairs(vector<cpp_int>&xs, vector<cpp_int>&ys, int bIdx);
    vector<cpp_int> calculateXmulYPairs(vector<vector<cpp_int>> &efPairs, int bIdx);

};

class cloudOne: public cloud
{
public:
  cloudOne(){}
  cpp_int calculateXmulY(cpp_int e, cpp_int f, int bIdx);

    vector<vector<cpp_int>> calculateEFPairs(vector<cpp_int>&xs, vector<cpp_int>&ys, int bIdx);
        vector<cpp_int> calculateXmulYPairs(vector<vector<cpp_int>> &efPairs, int bIdx);


};

class cloudTwo: public cloud {
public:
  cloudTwo(){}
  cpp_int calculateXmulY(cpp_int e, cpp_int f, int bIdx);

      vector<vector<cpp_int>> calculateEFPairs(vector<cpp_int>&xs, vector<cpp_int>&ys, int bIdx);
          vector<cpp_int> calculateXmulYPairs(vector<vector<cpp_int>> &efPairs, int bIdx);


};
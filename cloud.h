#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <sstream>
#include <fstream>
#include <math>
#include <time.h>
#include <climits>

#include "BuildingBlocks/aux-protocols.h"
#include "utils/emp-tool.h"

using namespace std;
using namespace sci;

const long long RING = pow(2, 64);
class point
{
public:
    vector<long long> data; // secret share data
    int cluIdx;             // cluster index
    int isCorePoint;        // whether is a core point
    int isMark;             // whether it is marked

    point(vector<long long> tdata, int tcluIdx, int tisCoreP, int tisM) : data(tdata), cluIdx(tcluIdx), isCorePoint(tisCoreP), isMark(tisM)
    {
    }
}

class cloud
{
public:
    vector<point *> plist;          // point list
    int minpts;                     // params
    long long eps;                  // params
    vector<vector<int>> beaverlist; // multiplication triple sets
    vector<vector<int>> distinfo;   // whether two point is close to each other

    cloud(vector<point *> tplist, int tminpts, long long teps) : plist(tplist), minpts(tminpts), eps(teps)
    {
    }
    cloud() {}
}
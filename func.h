#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include <random>
#include <map>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <time.h>
#include "cloud.h"

using namespace std;


void secretShareLL(long long data, long long &s1, long long &s2);

void secretShareInt(long long data, int &d1, int &d2);

vector<vector<long long>> readData(string filename);

vector<point*> data2Point(vector<vector<long long>>&dataList, vector<point*>&pList1, vector<point*>&pList2);

void generateBeaverSet(vector<vector<int>>&set1, vector<vector<int>>&set2);

void calculateEcliDist(cloud *c1, cloud *c2);

map<int, int> liantongtu(vector<vector<int>> &mat);

void dfs(int idx, int flag, vector<vector<int>> &mat, vector<bool> &visited, map<int, int> &mymap);

void writeCsv(vector<point *> &plist);

int plainCompare(long long x, long long y);

vector<long long> mulAllVecElem(cloudOne *c1, cloudTwo *c2,vector<long long> v1, vector<long long> v2);

map<int, int> getResult(cloudOne *c1, cloudTwo *c2);
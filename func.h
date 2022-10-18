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


void secretShareLL(cpp_int data, cpp_int &s1, cpp_int &s2);

void secretShareInt(cpp_int data, int &d1, int &d2);

vector<vector<cpp_int>> readData(string filename);

vector<point*> data2Point(vector<vector<cpp_int>>&dataList, vector<point*>&pList1, vector<point*>&pList2);

void generateBeaverSet(vector<vector<int>>&set1, vector<vector<int>>&set2);

void calculateEcliDist(cloud *c1, cloud *c2);

map<int, int> liantongtu(vector<vector<int>> &mat);

void dfs(int idx, int flag, vector<vector<int>> &mat, vector<bool> &visited, map<int, int> &mymap);

void writeCsv(vector<point *> &plist);
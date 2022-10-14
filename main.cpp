#include <iostream>
#include <time.h>
#include "func.h"

using namespace std;
dataset lsun{"../data/Lsun.csv", 200000000000, 4}, s1{"../data/s1.csv", 625000000000, 4};

// if x>y, return 1
int plainCompare(int x, int y) {
  return x>y;
}
/**
 * @brief 秘密共享数据，构造双云
 * 1. 读取数据并转换为point数据结构
 * 2. 构造beaver三元组
 * 3. 秘密共享minPts和eps
 * @param filename
 */
void initialization(dataset dtset, cloudOne* c1, cloudTwo* c2) {
    srand(time(NULL));

    vector<vector<long long>> rawData = readData(dtset.filename);
    c1->dim = rawData[0].size();
    c2->dim = rawData[0].size();

    data2Point(rawData, c1->plist, c2->plist);
    generateBeaverSet(c1->beaverlist, c2->beaverlist);
    secretShareInt(dtset.minPts, c1->minpts, c2->minpts);
    secretShareLL(dtset.eps, c1->eps, c2->eps);

    // 计算欧式距离
    int num = c1->plist.size();
    c1->distinfo = vector<vector<int>>(num, vector<int>(num));
    c2->distinfo = vector<vector<int>>(num, vector<int>(num));

    for(int i=0;i<num;++i){
      for(int j=i+1;j<num;++j){
        long long d1 = 0, d2 = 0;
        for(int k=0;k<c1->dim;++k){
          d1 = c1->plist[i]->data[k] - c1->plist[j]->data[k];
          d2 = c2->plist[i]->data[k] - c2->plist[j]->data[k];

          vector<long long>ef1 = c1->calculateEF(d1, d1, 0);
          vector<long long>ef2 = c2->calculateEF(d2, d2, 0);

          for(int m=0;m<ef1.size();++m)ef1[m] += ef2[m]; // 合并ef

          d1 += c1->calculateXmulY(ef1[0], ef1[1], 0);
          d2 += c2->calculateXmulY(ef1[0], ef1[1], 0);
        }
        c1->distinfo[i][j] = plainCompare(c1->eps+c2->eps, d1+d2);
        c1->distinfo[j][i] = c1->distinfo[i][j];
      }
    }

    return;
}

void clustering(cloud *c1, cloud *c2) {

}
/*
 * func:
 */
int main()
{
    cout<<"hell0!"<<endl;
    char *rr = new char(20);
    cloudOne *c1 = new cloudOne();
    cloudTwo *c2 = new cloudTwo();
    initialization(lsun, c1, c2);


    cout<<"bye!"<<endl;
    return 0;
}

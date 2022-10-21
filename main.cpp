#include "func.h"
#include <iostream>
#include <list>
#include <time.h>

using namespace std;
dataset lsun{"../data/Lsun.csv", 200000000000, 4},
    s1{"../data/s1.csv", 625000000000, 4};

// if x>y, return 1
int plainCompare(long long x, long long y)
{
  //  cout<<"cmp"<<" " <<x<<" "<<y<<endl;
  if (x > y)
    return 1;
  return 0;
}

/**
 * @brief 将vector内所有元素乘起来
 *
 * @param v1
 * @param v2
 * @return vector<long long> 秘密共享结果
 */
vector<long long> mulAllVecElem(cloudOne *c1, cloudTwo *c2,
                                vector<long long> v1, vector<long long> v2)
{
  int len = v2.size();

  while (len != 1)
  {
    vector<long long> tmp1, tmp2;
    for (int i = 0; i < (len / 2); ++i)
    {
      vector<long long> ef1 = c1->calculateEF(v1[2*i], v1[2 * i + 1], 0);
      vector<long long> ef2 = c2->calculateEF(v2[2*i], v2[2 * i + 1], 0);
      for (int j = 0; j < ef1.size(); ++j)
        ef1[j] += ef2[j];
      long long val1 = c1->calculateXmulY(ef1[0], ef1[1], 0);
      long long val2 = c2->calculateXmulY(ef1[0], ef1[1], 0);

      tmp1.push_back(val1);
      tmp2.push_back(val2);
    }
    if ((len%2)==1)
    {
      tmp1.push_back(v1.back());
      tmp2.push_back(v2.back());
    }
    v1 = tmp1;
    v2 = tmp2;
    len = v1.size();
  }
  return vector<long long>{v1[0], v2[0]};
}

/**
 * @brief 方案初始化：1.读取数据、2.构建beaver sets、3.秘密共享中间参数、4.计算距离，构造u[i,j]
 *
 * @param dtset 所用数据集
 * @param c1
 * @param c2
 * @return vector<point *>
 */
vector<point *> initialization(dataset dtset, cloudOne *c1, cloudTwo *c2)
{
  srand(time(NULL));
  vector<point *> plnPointList;

  vector<vector<long long>> rawData = readData(dtset.filename);
  c1->dim = rawData[0].size();
  c2->dim = rawData[0].size();

  plnPointList = data2Point(rawData, c1->plist, c2->plist);
  generateBeaverSet(c1->beaverlist, c2->beaverlist);
  secretShareInt(dtset.minPts, c1->minpts, c2->minpts);
  secretShareLL(dtset.eps, c1->eps, c2->eps);

  // 计算欧式距离
  int num = c1->plist.size();
  c1->distinfo = vector<vector<int>>(num, vector<int>(num));
  c2->distinfo = vector<vector<int>>(num, vector<int>(num));

  for (int i = 0; i < num; ++i)
  {
    int neiCnt1, neiCnt2;
    secretShareInt(0, neiCnt1, neiCnt2);
    for (int j = 0; j < num; ++j)
    {
      if(j <= i){
        neiCnt1 += c1->distinfo[i][j];
        neiCnt2 += c2->distinfo[i][j];
        continue;
      }
      long long d1 = 0, d2 = 0;
      for (int k = 0; k < c1->dim; ++k)
      {
        long long tmp1 = c1->plist[i]->data[k] - c1->plist[j]->data[k];
        long long tmp2 = c2->plist[i]->data[k] - c2->plist[j]->data[k];

        vector<long long> ef1 = c1->calculateEF(tmp1, tmp1, 0);
        vector<long long> ef2 = c2->calculateEF(tmp2, tmp2, 0);

        for (int m = 0; m < ef1.size(); ++m)
          ef1[m] += ef2[m]; // 合并ef

        d1 += c1->calculateXmulY(ef1[0], ef1[1], 0);
        d2 += c2->calculateXmulY(ef1[0], ef1[1], 0);
      }
      c1->distinfo[i][j] = plainCompare(c1->eps + c2->eps, d1 + d2);
      c1->distinfo[j][i] = c1->distinfo[i][j];
      neiCnt1 += c1->distinfo[i][j];
      neiCnt2 += c2->distinfo[i][j];
    }
    int res = plainCompare(neiCnt1+neiCnt2, c1->minpts+c2->minpts);
    c1->plist[i]->isCorePoint = res;
  }

  return plnPointList;
}

/**
 * @brief 聚类过程
 *
 * @param c1
 * @param c2
 */
void clustering(cloudOne *c1, cloudTwo *c2)
{
  int num = c1->plist.size();
  vector<long long> res, res1, res2;

  for (int i = 0; i < num; ++i)
  {
    int tmp1, tmp2, tmpCluId1, tmpCluId2;
    secretShareInt(1, tmp1, tmp2);           // 秘密共享1
    secretShareInt(0, tmpCluId1, tmpCluId2); // 秘密共享0


    for (int j = 0; j < num; ++j)
    {
      if (i == j)
        continue;
      // tmp * j.isMark * j.CluId * u[i,j]
      res = mulAllVecElem(
          c1, c2,
          vector<long long>{tmp1, c1->plist[j]->isMark, c1->plist[j]->cluIdx,
                            c1->distinfo[i][j]},
          vector<long long>{tmp2, c2->plist[j]->isMark, c2->plist[j]->cluIdx,
                            c2->distinfo[i][j]});

      // tmpCluId = tmpCluId + tmp * j.isMark * j.CluId * u[i,j]
      tmpCluId1 = res[0] + res[1];
      tmpCluId2 = 0;

      // tmp * j.isMark * u[i,j]
      res = mulAllVecElem(
          c1, c2,
          vector<long long>{tmp1, c1->plist[j]->isMark, c1->distinfo[i][j]},
          vector<long long>{tmp2, c2->plist[j]->isMark, c2->distinfo[i][j]});

      // tmp = tmp - tmp * j.isMark*u[i,j] = tmp(1 - j.isMark*u[i,j])
      tmp1 = tmp1 - res[0];
      tmp2 = tmp2 - res[1];
    }

    // 更新isMark
    // i.isMark = sc(i.isMark+i.isCorePoint, 0)
    int cmpRes1 =
        plainCompare(c1->plist[i]->isMark + c2->plist[i]->isMark +
                         c1->plist[i]->isCorePoint + c2->plist[i]->isCorePoint,
                     0);
    int cmpRes2 = 0;

    c1->plist[i]->isMark = cmpRes1;
    c2->plist[i]->isMark = cmpRes2;

    // 更新 CluId
    // tmp * i.isMark
    res = mulAllVecElem(c1, c2, vector<long long>{tmp1, c1->plist[i]->isMark},
                        vector<long long>{tmp2, c2->plist[i]->isMark});
    // i.isMark + tmp - tmp * i.isMark
    long long part1 = c1->plist[i]->isMark + tmp1 - res[0];
    long long part2 = c2->plist[i]->isMark + tmp2 - res[1];

    // i.CluId * (i.isMark + tmp - tmp * i.isMark)
    res1 = mulAllVecElem(c1, c2, vector<long long>{c1->plist[i]->cluIdx, part1},
                         vector<long long>{c2->plist[i]->cluIdx, part2});
    c1->plist[i]->cluIdx = res1[0];
    c2->plist[i]->cluIdx = res1[1];

    // 1 - tmp - i.isMark + tmp * i.isMark （常数特别注意下
    part1 = 1 - tmp1 - c1->plist[i]->isMark + res[0];
    part2 = -tmp2 - c2->plist[i]->isMark + res[1];

    // i.CluId = (i.CluId * (i.isMark + tmp - tmp * i.isMark)) + (1 - tmp -
    // i.isMark + tmp * i.isMark) * tmpCluId
    res = mulAllVecElem(c1, c2, vector<long long>{part1, tmpCluId1},
                        vector<long long>{part2, tmpCluId2});
    c1->plist[i]->cluIdx += res[0];
    c2->plist[i]->cluIdx += res[1];

//    // FIXME delete later
//    c1->plist[i]->cluIdx += c2->plist[i]->cluIdx;
//    c2->plist[i]->cluIdx = 0;

    for (int j = 0; j < num; ++j)
    {
      // debug only
//      if(i==213 && j == 209){
//        cout<<"?"<<endl;
//      }
      if (i == j)
        continue;

      // 更新j的cluid
      // (i.cluId-j.cluId)*(1-j.isMark)*i.isMark*u[i,j]
      res = mulAllVecElem(
          c1, c2,
          vector<long long>{c1->plist[i]->cluIdx - c1->plist[j]->cluIdx,
                            1 - c1->plist[j]->isMark, c1->plist[i]->isMark,
                            c1->distinfo[i][j]},
          vector<long long>{c2->plist[i]->cluIdx - c2->plist[j]->cluIdx,
                            -c2->plist[j]->isMark, c2->plist[i]->isMark,
                            c2->distinfo[i][j]});

      c1->plist[j]->cluIdx += res[0];
      c2->plist[j]->cluIdx += res[1];

//      // FIXME for debug, delete later
//      c1->plist[j]->cluIdx += c2->plist[j]->cluIdx;
//      c2->plist[j]->cluIdx = 0;

      // 更新j的标记
      // u[i,j]*i.isMark
      res = mulAllVecElem(
          c1, c2, vector<long long>{c1->distinfo[i][j], c1->plist[i]->isMark},
          vector<long long>{c2->distinfo[i][j], c2->plist[i]->isMark});
      // j.isMark+u[i,j]*i.isMark
      tmp1 = plainCompare(
          res[0] + res[1] + c1->plist[j]->isMark + c2->plist[j]->isMark, 0);

      c1->plist[j]->isMark = tmp1;
      c2->plist[j]->isMark = 0;

      // 标记是否相连的信息
      // i.isMark * j.isMark * u[i,j]
      res = mulAllVecElem(
          c1, c2,
          vector<long long>{c1->plist[i]->isMark, c1->plist[j]->isMark,
                            c1->distinfo[i][j]},
          vector<long long>{c2->plist[i]->isMark, c2->plist[j]->isMark,
                            c2->distinfo[i][j]});
      pairInfo *p1 =
          new pairInfo(c1->plist[i]->cluIdx, c1->plist[j]->cluIdx, res[0]);
      pairInfo *p2 =
          new pairInfo(c2->plist[i]->cluIdx, c2->plist[j]->cluIdx, res[1]);
      c1->resPairs.push_back(p1);
      c2->resPairs.push_back(p2);
    }
  }
}

/**
 * @brief 还原双云信息，获取结果
 *
 * @param c1
 * @param c2
 * @return map<int, int> int1: curr clu id  int2: final clu id
 */
map<int, int> getResult(cloudOne *c1, cloudTwo *c2)
{
  int num = c1->plist.size() + 1;
  list<pairInfo *>::iterator p1, p2;

  vector<vector<int>> matrix(num, vector<int>(num));
  for (p1 = c1->resPairs.begin(), p2 = c2->resPairs.begin();
       p1 != c1->resPairs.end() && p2 != c2->resPairs.end(); p1++, p2++)
  {
    int cluid1 = (*p1)->cluid1 + (*p2)->cluid1;
    int cluid2 = (*p1)->cluid2 + (*p2)->cluid2;
    int isConn = (*p1)->isConnected + (*p2)->isConnected;
    matrix[cluid1][cluid2] += isConn;
    matrix[cluid2][cluid1] += isConn;
  }

  cout << "开始查询连通图" << endl;
  map<int, int> resMap = liantongtu(matrix);

  return resMap;
}

int main()
{
  cout << "hell0!" << endl;

  cloudOne *c1 = new cloudOne();
  cloudTwo *c2 = new cloudTwo();

  cout<<"part one: Initialization" << endl;
  vector<point *> pointList = initialization(lsun, c1, c2);

  cout<<"part two: Clustering" <<endl;
  clustering(c1, c2);

  cout << "part three: obtain cluster res" << endl;
  map<int, int> mymap = getResult(c1, c2);
  int num = c1->plist.size();
  for (int i = 0; i < num; ++i)
  {
    int cluidx = c1->plist[i]->cluIdx+c2->plist[i]->cluIdx;
    if (mymap[cluidx] != 0)pointList[i]->resCluIdx = mymap[cluidx];
    else pointList[i]->resCluIdx = cluidx;

  }
  writeCsv(pointList);

  cout << "bye!" << endl;
  return 0;
}

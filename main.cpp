//
//                       _oo0oo_
//                      o8888888o
//                      88" . "88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \\|     |// '.
//                 / \\|||  :  |||// \
//                / _||||| -:- |||||- \
//               |   | \\\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          ."" '<  `.___\_<|>_/___.' >' "".
//         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//         \  \ `_.   \_ __\ /__ _/   .-` /  /
//     =====`-.____`.___ \_____/___.-`___.-'=====
//                       `=---='
//
//
//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//               佛祖保佑         实验顺利
#include "func.h"
#include <iostream>
#include <list>
#include <time.h>
int party, port = 8200;
int MAX_CMP_LEN = 500;
// int dim = 1 << 16;
int dim = 5500; // 只有在kd tree构造部分才需要取很大
string address = "127.0.0.1";
NetIO *io;
OTPack<NetIO> *otpack;
MillionaireProtocol<sci::NetIO> *mill;
int bw_x = 64;

clock_t starttime, endtime;

dataset dataSetList[] = {{"../data/Lsun.csv", 200000000000, 4,"lsun"}, {"../data/s1.csv", 625000000000, 4,"s1"}};

/**
 * @brief 判断d1>d2?
 *
 * @param d1 alice方的数据
 * @param d2 bob方的数据
 * @return vector<vector<int>> = 2*dim
 */
vector<vector<int>> cmp(vector<int> d1, vector<int> d2, cloudOne *c1, cloudTwo *c2)
{
  assert(d1.size() == d2.size());

  vector<vector<int>> result(2, vector<int>(dim));

  uint64_t *x = new uint64_t[dim];
  uint64_t *y = new uint64_t[dim];

  uint8_t *alice_res = new uint8_t[dim];
  uint8_t *bob_res = new uint8_t[dim];

  y[0] = d1.size(); // 待比较数组第一位存放长度
  for (int i = 0; i < d1.size(); i++)
  {
    x[i] = d1[i];
    y[i + 1] = d2[i];
  }

  io->send_data(y, dim * sizeof(uint64_t));            // 将待比较的数据发送给bob
  mill->compare(alice_res, x, d1.size(), bw_x, true);  // 比较大小
  io->recv_data(bob_res, d1.size() * sizeof(uint8_t)); // 读取bob的比较结果

  vector<vector<int>> tmp1(2, vector<int>(d1.size())), tmp2(tmp1);
  int a = rand() % 10, b = rand() % 10;

  for (int i = 0; i < d1.size(); i++)
  {
    tmp1[0][i] = a;
    tmp1[1][i] = b;

    tmp2[0][i] = int(alice_res[i]) - a;
    tmp2[1][i] = int(bob_res[i]) - b;
  } // 秘密共享v0, v1

  // 计算v0*v1
  vector<vector<int>> ef1 = c1->calculate_v0v1_ef(tmp1);
  vector<vector<int>> ef2 = c2->calculate_v0v1_ef(tmp2);

  for (int i = 0; i < d1.size(); i++)
  {
    ef1[0][i] += ef2[0][i];
    ef1[1][i] += ef2[1][i];
  }

  vector<int> r1 = c1->calculate_v0v1_final(ef1);
  vector<int> r2 = c2->calculate_v0v1_final(ef1);

  // 计算 vi-(2v0v1)i
  for (int i = 0; i < r1.size(); i++)
  {
    r1[i] = int(alice_res[i]) - 2 * r1[i];
    r2[i] = int(bob_res[i]) - 2 * r2[i];
    // cout << r1[i] << " " << r2[i] << endl;
  }

  delete[] x;
  delete[] y;

  delete[] alice_res;
  delete[] bob_res;

  return vector<vector<int>>{r1, r2};
}

vector<vector<int>> securecmp(vector<int>d1, vector<int>d2, cloudOne *c1, cloudTwo *c2){
  assert(d1.size() == d2.size());
  vector<vector<int>>res;

  for(int i=0;i*500<d1.size();i++){
    int start = i*MAX_CMP_LEN;
    int end = (i+1)*MAX_CMP_LEN<d1.size()?(i+1)*MAX_CMP_LEN:d1.size();
    vector<vector<int>>tmp = cmp(vector<int>(d1.begin()+start, d1.begin()+end),
        vector<int>(d2.begin()+start, d2.begin()+end),
            c1, c2);
    res.insert(res.end(), tmp.begin(), tmp.end());
  }

  return res;
}

/**
 * @brief 判断d1>d2?
 *
 * @param d1 alice方的数据
 * @param d2 bob方的数据
 * @return vector<vector<int>> = 2*dim
 */
vector<vector<int>> cmpLL(vector<long long> d1, vector<long long> d2, cloudOne *c1, cloudTwo *c2)
{
  assert(d1.size() == d2.size());

  vector<vector<int>> result(2, vector<int>(dim));

  uint64_t *x = new uint64_t[dim];
  uint64_t *y = new uint64_t[dim];

  uint8_t *alice_res = new uint8_t[dim];
  uint8_t *bob_res = new uint8_t[dim];

  y[0] = d1.size(); // 待比较数组第一位存放长度
  for (int i = 0; i < d1.size(); i++)
  {
    x[i] = d1[i];
    y[i + 1] = d2[i];
  }

  io->send_data(y, dim * sizeof(uint64_t));            // 将待比较的数据发送给bob
  mill->compare(alice_res, x, d1.size(), bw_x, true);  // 比较大小
  io->recv_data(bob_res, d1.size() * sizeof(uint8_t)); // 读取bob的比较结果

  vector<vector<int>> tmp1(2, vector<int>(d1.size())), tmp2(tmp1);
  int a = rand() % 10, b = rand() % 10;

  for (int i = 0; i < d1.size(); i++)
  {
    tmp1[0][i] = a;
    tmp1[1][i] = b;

    tmp2[0][i] = int(alice_res[i]) - a;
    tmp2[1][i] = int(bob_res[i]) - b;
  } // 秘密共享v0, v1

  // 计算v0*v1
  vector<vector<int>> ef1 = c1->calculate_v0v1_ef(tmp1);
  vector<vector<int>> ef2 = c2->calculate_v0v1_ef(tmp2);

  for (int i = 0; i < d1.size(); i++)
  {
    ef1[0][i] += ef2[0][i];
    ef1[1][i] += ef2[1][i];
  }

  vector<int> r1 = c1->calculate_v0v1_final(ef1);
  vector<int> r2 = c2->calculate_v0v1_final(ef1);

  // 计算 vi-(2v0v1)i
  for (int i = 0; i < r1.size(); i++)
  {
    r1[i] = int(alice_res[i]) - 2 * r1[i];
    r2[i] = int(bob_res[i]) - 2 * r2[i];
    // cout << r1[i] << " " << r2[i] << endl;
  }

  delete[] x;
  delete[] y;

  delete[] alice_res;
  delete[] bob_res;

  return vector<vector<int>>{r1, r2};
}

vector<vector<int>> securecmpLL(vector<long long>d1, vector<long long>d2, cloudOne *c1, cloudTwo *c2){
  assert(d1.size() == d2.size());
  vector<vector<int>>res;

  for(int i=0;i*500<d1.size();i++){
    int start = i*MAX_CMP_LEN;
    int end = (i+1)*MAX_CMP_LEN<d1.size()?(i+1)*MAX_CMP_LEN:d1.size();
    vector<vector<int>>tmp = cmpLL(vector<long long>(d1.begin()+start, d1.begin()+end),
        vector<long long>(d2.begin()+start, d2.begin()+end),
            c1, c2);
    res.insert(res.end(), tmp.begin(), tmp.end());
  }

  return res;
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

  vector<vector<long long>> rawData = readData(dtset.filepath);
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

  vector<int> neighPointCnt(num, 0);
  for (int i = 0; i < num; ++i)
  {
    vector<long long> distVec(num, 0);
    for (int j = 0; j < num; ++j)
    {
      if (i == j)
      {
        distVec[j] = c1->eps + c2->eps;
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
      distVec[j] = d1 + d2;
    } // j

    vector<vector<int>> cmpres = securecmpLL(vector<long long>(num, c1->eps + c2->eps), distVec, c1, c2);
    c1->distinfo[i] = vector<int>(cmpres[0].begin(), cmpres[0].begin() + num);
    c2->distinfo[i] = vector<int>(cmpres[1].begin(), cmpres[1].begin() + num);

    for (int p = 0; p < num; ++p)neighPointCnt[i] = neighPointCnt[i] + (cmpres[0][p] + cmpres[1][p]);
  }

  vector<vector<int>> cmpres = securecmp(neighPointCnt, vector<int>(num, c1->minpts + c2->minpts), c1, c2);
  for (int i = 0; i < num; ++i)
  {
    c1->plist[i]->isCorePoint = cmpres[0][i] + cmpres[1][i];
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
  vector<vector<int>>cmpres;

  for (int i = 0; i < num; ++i)
  {
    // 更新isMark
    // i.isMark = sc(i.isMark+i.isCorePoint, 0)
    cmpres = securecmp(vector<int>{c1->plist[i]->isMark + c2->plist[i]->isMark +
                         c1->plist[i]->isCorePoint + c2->plist[i]->isCorePoint}, vector<int>{0}, c1, c2);

    c1->plist[i]->isMark = cmpres[0].front();
    c2->plist[i]->isMark = cmpres[1].front();

    vector<int> markInfo(num, 0);
    for (int j = 0; j < num; ++j)
    {
      if (i == j)
        continue;

      // 更新j的cluid
      // (i.cluId-j.cluId)*(1-j.isMark)*i.isMark*u[i,j]
      res = mulAllVecElem(
          c1, c2,
          vector<long long>{c1->plist[i]->cluIdx - c1->plist[j]->cluIdx,
                            1 - c1->plist[j]->isMark, c1->plist[i]->isCorePoint,
                            c1->distinfo[i][j]},
          vector<long long>{c2->plist[i]->cluIdx - c2->plist[j]->cluIdx,
                            -c2->plist[j]->isMark, c2->plist[i]->isCorePoint,
                            c2->distinfo[i][j]});

      c1->plist[j]->cluIdx += res[0];
      c2->plist[j]->cluIdx += res[1];

      // 更新j的标记
      // u[i,j]*i.isMark
      res = mulAllVecElem(
          c1, c2, vector<long long>{c1->distinfo[i][j], c1->plist[i]->isCorePoint},
          vector<long long>{c2->distinfo[i][j], c2->plist[i]->isCorePoint});

      markInfo[j] = res[0] + res[1] + c1->plist[j]->isMark + c2->plist[j]->isMark;
    }

    cmpres = securecmp(markInfo, vector<int>(num, 0), c1, c2);
    for(int j=0;j<num;++j){
      c1->plist[j]->isMark = cmpres[0][j] + cmpres[1][j];
      c2->plist[j]->isMark = 0;

      // 标记是否相连的信息
      // i.isMark * j.isMark * u[i,j]
      res = mulAllVecElem(
          c1, c2,
          vector<long long>{c1->plist[i]->isCorePoint, c1->plist[j]->isCorePoint,
                            c1->distinfo[i][j]},
          vector<long long>{c2->plist[i]->isCorePoint, c2->plist[j]->isCorePoint,
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

dataset chooseDataset(string setname) {
  for(auto dataset : dataSetList){
    if(dataset.filename == setname){
      return dataset;
    }
  }
  cout<<"no related dataset"<<endl;
  return {};
}
/**
 * @brief 负责主要过程
 * 
 */
void alice(string &setname)
{

  cloudOne *c1 = new cloudOne();
  cloudTwo *c2 = new cloudTwo();

  starttime = clock();
  cout << "part one: Initialization" << endl;
  dataset set = chooseDataset(setname);
  vector<point *> pointList = initialization(set, c1, c2);

  cout<<"part two: Clustering" <<endl;
  clustering(c1, c2);
  endtime = clock();
  double tottime = (double)(endtime-starttime)/CLOCKS_PER_SEC;
  cout<<"total time of clustering:"<<tottime*1000<<"ms"<<endl;
  
  starttime = clock();
  cout << "part three: obtain cluster res" << endl;
  map<int, int> mymap = getResult(c1, c2);
  
  int num = c1->plist.size();
  for (int i = 0; i < num; ++i)
  {
    int cluidx = c1->plist[i]->cluIdx+c2->plist[i]->cluIdx;
    if (mymap[cluidx] != 0)pointList[i]->resCluIdx = mymap[cluidx];
    else pointList[i]->resCluIdx = cluidx;

  }
  endtime = clock();
  tottime = (double)(endtime-starttime)/CLOCKS_PER_SEC;
  cout<<"total time of clustering:"<<tottime*1000<<"ms"<<endl;
  writeCsv(pointList);
}

/**
 * @brief 负责比较
 * 
 */
void bob()
{
  uint64_t *recv_data = new uint64_t[dim];
  uint8_t *bob_res = new uint8_t[dim];

  while (true)
  {
    io->recv_data(recv_data, dim * sizeof(uint64_t)); // 接收待比较的数据

    int cmp_arr_len = recv_data[0]; // 第一个数据存放数组长度
    uint64_t *y = new uint64_t[cmp_arr_len];
    memcpy(y, recv_data + 1, sizeof(uint64_t) * cmp_arr_len); // 复制数组

    mill->compare(bob_res, y, cmp_arr_len, bw_x, true);    // 开始比较
    io->send_data(bob_res, cmp_arr_len * sizeof(uint8_t)); // 将结果发送给alice
                                                           // delete[] y;
  }

  delete[] recv_data;
  delete[] bob_res;
}


int main(int argc, char **argv)
{
  cout << "hell0!" << endl;
  // 初始化设置
  setbuf(stdout, 0);
  srand(time(NULL));
  // 读取命令行数据
  ArgMapping amap;
  int dimension, data_num, n_of_k, iter_times;
  string dataset;
  amap.arg("r", party, "Role of party: ALICE = 1; BOB = 2");
  amap.arg("d", dataset, "choose which dataset");
//  amap.arg("m", dimension, "dimension of dataset");
//  amap.arg("n", data_num, "number of data");
//  amap.arg("k", n_of_k, "number of cluster");
//  amap.arg("i", iter_times, "iteration rounds");
//  amap.arg("p", port, "Port Number");
//  amap.arg("d", dim, "Size of vector");
//  amap.arg("ip", address, "IP Address of server (ALICE)");
  amap.parse(argc, argv);

  io = new NetIO(party == 1 ? nullptr : address.c_str(), port);
  otpack = new OTPack<NetIO>(io, party);
  mill = new MillionaireProtocol<sci::NetIO>(party, io, otpack);

  cout<<dataset<<endl;
  // 开始方案
  if (party == ALICE)
    alice(dataset);
  else
    bob();

  cout << "bye!" << endl;
  return 0;
}
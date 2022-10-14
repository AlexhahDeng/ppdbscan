#include "func.h"

/**
 *
 * @param data 原始明文数据
 * @param s1 分割数据1
 * @param s2 分割数据2
 */
void secretShareLL(long long data, long long &s1, long long &s2) {
    int l = 0, r = 1000;

    s1 = (rand() % (r-l+1) + l) % RING;
    s2 = (data - s1) % RING;
}

void secretShareInt(long long data, int &d1, int &d2) {
      int l = 0, r = 1000;
    d1 = (rand() % (r-l+1) + l) % RING;
    d2 = (data - d1) % RING;
}
/**
 *
 * @param filename 读取文件位置
 * @return 返回原始数据
 */
vector<vector<long long>> readData(string filename)
{
    vector<vector<long long>> res;
    ifstream fp(filename);
    string line;

    while (!fp.eof())
    {
        getline(fp, line);
        if (line == "")
            break;
        vector<long long> lineData;
        string numStr;

        istringstream readStr(line);

        while (getline(readStr, numStr, ','))
        {
            lineData.push_back(atol(numStr.c_str()));
        }
        res.push_back(lineData);
    }
    return res;
}
/**
 * data 转 point
 * @param dataList 原始数据集
 * @param pList1 秘密共享数据集1
 * @param pList2 秘密共享数据集2
 */
void data2Point(vector<vector<long long>>&dataList, vector<point*>&pList1, vector<point*>&pList2){
    int n = dataList.size(), m = dataList[0].size();

    pList1.resize(n);
    pList2.resize(n);

    for(int i=0;i<n;++i){
        vector<long long>v1(m), v2(m);
        for(int j=0;j<m;++j){
            long long d1, d2;
            secretShareLL(dataList[i][j], d1, d2);
            v1[j] = d1;
            v2[j] = d2;
        }
        long long t1, t2;
        secretShareLL(i+1, t1, t2);
        pList1[i] = new point(v1, t1);
        pList2[i] = new point(v2, t2);
    }
    return;
}

void generateBeaverSet(vector<vector<int>>&set1, vector<vector<int>>&set2) {
  const int setNum = 10000;
  const int dataRange = 1000;
  set1.resize(setNum);
  set2.resize(setNum);

  for(int i=0;i<setNum;++i){
    int a = rand() % dataRange + 1;
    int b = rand() % dataRange + 1;
    int c = a * b;

    int c1_a = rand() % a;
    int c2_a = a - c1_a;

    int c1_b = rand() % b;
    int c2_b = b - c1_b;

    int c1_c = rand() % c;
    int c2_c = c - c1_c;

    set1[i] = vector<int>{c1_a, c1_b, c1_c};
    set2[i] = vector<int>{c2_a, c2_b, c2_c};
  }
}

void calculateEcliDist(cloud *c1, cloud *c2){
  int num = c1->plist.size();
  for(int i=0;i<num;++i){
    for(int j=i+1;j<num;++j){

    }
  }
}


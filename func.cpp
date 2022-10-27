#include "func.h"

/**
 *
 * @param data 原始明文数据
 * @param s1 分割数据1
 * @param s2 分割数据2
 */
void secretShareLL(long long data, long long &s1, long long &s2) {
    long long l = 0, r = 50;
    long long rnd = rand();
    s1 = rnd % (r-l);
    s2 = data - s1;
}

void secretShareInt(long long data, int &d1, int &d2) {
    long long l = 0, r = 50;
    long long rnd = rand();
    d1 = rnd % (r-l);
    d2 = data - d1;
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
vector<point*> data2Point(vector<vector<long long>>&dataList, vector<point*>&pList1, vector<point*>&pList2){
    int n = dataList.size(), m = dataList[0].size();

    pList1.resize(n);
    pList2.resize(n);
    vector<point *> pointList(n);

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
        pointList[i] = new point(dataList[i], i+1);
    }
    return pointList;
}
/**
 * @brief 生成1w个beaver 三元组
 * @param set1
 * @param set2
 */
void generateBeaverSet(vector<vector<int>>&set1, vector<vector<int>>&set2) {
  const int setNum = 10000;
  const int dataRange = 500;
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

//    set1[i] = vector<int>{c1_a, c1_b, c1_c};
//    set2[i] = vector<int>{c2_a, c2_b, c2_c};
    set1[i] = vector<int>{100, 100, 10000};
    set2[i] = vector<int>{0, 0, 0};
  }
}


void dfs(int idx, int flag, vector<vector<int>> &mat, vector<bool> &visited, map<int, int> &mymap) {
    int num = mat.size();
    for (int i = 1; i < num; ++i) {
        if (visited[i])continue;
        if (mat[idx][i] != 0) {
            visited[i] = true;
            mymap[i] = flag;
            dfs(i, flag, mat, visited, mymap);
        }
    }
}

map<int, int> liantongtu(vector<vector<int>> &mat) {
    int num = mat.size();
    int flag = 500;
    map<int, int> mymap;
    vector<bool> visited(num, false);

    for (int i = 1; i < num; ++i) {
        if (visited[i])continue;
        visited[i] = true;
        mymap[i] = flag;
        for (int j = 1; j < num; ++j) {
            if (visited[j])continue;
            if (mat[i][j] != 0) {
                mymap[j] = flag;
                visited[j] = true;
                dfs(j, flag, mat, visited, mymap);
            }
        }
        flag++;
    }
    return mymap;
}

void writeCsv(vector<point *> &plist) {
    ofstream outFile;
    outFile.open("../data/res.csv");
    for (int i = 0; i < plist.size(); ++i) {
        outFile << plist[i]->data[0] << "," << plist[i]->data[1] << "," << plist[i]->type << "," << plist[i]->resCluIdx
                << endl;
    }
    outFile.close();
}


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
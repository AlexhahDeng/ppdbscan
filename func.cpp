#include "func.h"

/**
 * @brief 从指定文件读取数据
 *
 * @param filename
 * @return vector<vector<long long>>
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

vector<int> ssInt(int data)
{
    vector<int> res(2);
    srand(time(NULL));
    res[0] = rand() % data;
    res[1] = data - res[0];
}

vector<vector<point *>> data2point(vector<long long> &data)
{
    int len = data.size();
    vector<vector<point *>> res(2, vector<point *>(len, nullptr));
    for ()
}
void sendData2Cloud(cloud *c1, cloud *c2, vector<vector<long long>> &data)
{
}

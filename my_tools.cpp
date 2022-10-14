#include "tools.h"

vector<vector<long long>> readData(string filename)
{
    cout << "read data from " << filename << endl;
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
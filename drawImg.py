import csv

import matplotlib.pyplot as plt
import numpy as np

def extraNoise(datalist):
    dataset = set()
    repeatset = set()
    for data in  datalist:
        if data[-1] in dataset:
            repeatset.add(data[-1])
            continue
        dataset.add(data[-1])
    return dataset-repeatset # 就是只出现一次的noise

csv_reader = csv.reader(open("./data/res.csv"))
datalist = []
for line in csv_reader:
    cur = []
    for item in line:
        cur.append(int(item))
    datalist.append(cur)

colors = np.array(
    ['g', 'r', 'c', 'm', 'y', 'k', 'w', '#2ecc71', '#ff7f50', '#2ed573', '#f0932b', '#82589f', '#7efff5',
     '#ea5514', '#6a8d52',
     '#12507b', '#e94829',
     '#ddbb99', '#2c4f54', '#c3d825', '#cc7eb1', '#c9171e', '#ea5506'])
datanp = np.asarray(datalist)

noiseset = extraNoise(datalist)
print("noist set is ", str(noiseset))

# 无重复的簇标记
label = set(datanp[:, -1])
# 创建映射
label2color = {}
for idx, val in enumerate(label):
    if val in noiseset:
        label2color[val] = 'black' # 噪声节点都用黑色标记
        continue
    label2color[val] = colors[idx % len(colors)]

print("num of cluster ", str(len(label2color) - len(noiseset)))
x = datanp[:, 0]
y = datanp[:, 1]

for i in range(len(datanp)):
    plt.scatter(x[i], y[i], s=10, c=label2color[datanp[i, -1]])

plt.savefig("./image/res.png")

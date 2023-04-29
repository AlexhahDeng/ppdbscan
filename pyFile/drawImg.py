import csv
import matplotlib.pyplot as plt
import numpy as np
import sys
from time import strftime, localtime

# 预定义颜色
colors = np.array(
    ['#b6a014','#80a492','#422517','#c12c1f','#a76283','#4f6f46','#c67915','#12264f','#422256','#cb5c83','#68945c','#e94829','#106898','#108b96','#ba79b1','#d12920','#d24735','#420b2f'])

# 提取噪声点
def extraNoise(datalist):
    dataset = set()
    repeatset = set()
    for data in  datalist:
        if data[-1] in dataset:
            repeatset.add(data[-1])
            continue
        dataset.add(data[-1])
    return dataset-repeatset 

# 获取文件名
setname = sys.argv[1]
prefix = strftime("%m-%d", localtime())
filename = "/home/dd/Ubu20/ppdbscan/res/" +prefix+ "-" + setname+".csv"

# 读取数据
csv_reader = csv.reader(open(filename))
datalist = []
for line in csv_reader:
    cur = []
    for item in line:
        cur.append(int(item))
    datalist.append(cur)
datanp = np.asarray(datalist)

# 查找噪声点
noiseset = extraNoise(datalist)
print("noist set is ", str(noiseset))

# 无重复的簇
label = set(datanp[:, -1])

# 创建映射
label2color = {}
count = 0
label = sorted(label)
for idx, val in enumerate(label):
    if val in noiseset:
        label2color[val] = 'black' # 噪声节点都用黑色标记
        continue
    label2color[val] = colors[count % len(colors)]
    count+= 1
print("num of cluster ", str(len(label) - len(noiseset)))
x = datanp[:, 0]
y = datanp[:, 1]

#开始绘图
for i in range(len(datanp)):
    plt.scatter(x[i], y[i], s=10, c=label2color[datanp[i, -1]])

filename = "/home/dd/Ubu20/ppdbscan/image/" + prefix + setname + ".png"
plt.savefig(filename)

from matplotlib import pyplot as plt
import numpy as np
import matplotlib
# matplotlib.rc("font",family='yahei')


# ppdb = [[688.983,5641785],[1747.77,12966841],[3389.34,22420285],[4980.98,33606541],[6561.58,47078785]]
# redb = [[6047.27,37781385],[12768.5,88094841],[22226.3,151385485],[30758.2,209342541],[40719.6,271082785]]
# d1 = np.array(ppdb)
# d2 = np.array(redb)
# plt.yscale('log')
ppdbclurecove=[[17734,12],[72557,47],[161882,111],[397451,380]]
ppdbclurecove = np.array(ppdbclurecove)
# x_axis = [100,200,300,400,500]
x_axis = [1000,2000,3000,4000]
# y1=d1[:,0]
# y2=d2[:,0]
yclu=ppdbclurecove[:,0]
yrecover = ppdbclurecove[:,1]
# plt.rc('font',family='YouYuan')
plt.plot(x_axis, yclu, label="vlu")
plt.plot(x_axis, yrecover, label="recover")

for a,b in zip(x_axis, yclu):
    plt.text(a,b,str(b)+"ms")
    
for a,b in zip(x_axis, yrecover):
    plt.text(a,b,str(b)+"ms")
# plt.xlabel("数量")
# plt.ylabel("时间（ms）")
plt.savefig("testrc.png")
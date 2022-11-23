import numpy as np


class point:
    def __init__(self) -> None:
        self.data = []
        self.pid = 0 # 明文聚类所属簇id
        self.cid = 0 # 密文聚类所属簇id

with open("./data/res.csv") as f:
    line = f.readline()
    
    p_list = []
    while line:
        split_item = line.split(",")
        tmp = []
        for item in split_item:
            tmp.append(int(item))
        p_list.append(tmp)
        line = f.readline()
p_list = np.asarray(p_list)
# print(p_list[:, -1])
# 计算明文簇相关信息
p_id_set = set(p_list[:, -2])
p_id_list = list(p_id_set)
p_id_list.sort()
print("明文计算簇分类："+str(p_id_list))



# 计算密文簇相关信息
all_id_set = set()
c_id_set = set()
for item in p_list[:, -1]:
    if item in all_id_set:
        c_id_set.add(item)
    all_id_set.add(item)
noise_id_set = all_id_set - c_id_set
c_id_list = list(c_id_set)
noise_id_list = list(noise_id_set)

c_id_list.sort()
print("密文聚类簇分类："+str(c_id_list))
print("噪声簇分类："+str(noise_id_list))
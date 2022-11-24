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

# 计算明文簇相关信息
p_id_set = set(p_list[:, -2])
print("明文计算簇分类："+str(p_id_set))

p_id_dict = dict()
for item in p_list[:, -2]:
    if item in p_id_dict:
        p_id_dict[item] = p_id_dict[item]+1
    else:
        p_id_dict[item] = 0
    
print(p_id_dict)

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

print("密文聚类簇分类："+str(c_id_list))
print("噪声簇分类："+str(noise_id_list))

c_id_dict_tmp = dict()
for item in p_list[:, -1]:
    if item in noise_id_set:
        continue
    if item in c_id_dict_tmp:
        c_id_dict_tmp[item] = c_id_dict_tmp[item] + 1
    else:
        c_id_dict_tmp[item] = 0
        
sort_tmp = sorted(c_id_dict_tmp)
key = 1
c_id_dict = dict()
for item in sort_tmp:
    c_id_dict[key] = c_id_dict_tmp[item]
    key = key + 1

print(c_id_dict)
import collections

RoadDic = {'A':['B','C'], 'B':['A','D'], 'C':['A','E','F'], 'D':['B','E'], 'E':['C','D','G'], 'F':['C','G'], 'G':['E','F']}
#RoadDic = {'A':['B','C','D'], 'B':['A','C','E'], 'C':['A','B','D','E'], 'D':['A','C','F'], 'E':['B','C','F'], 'F':['D','E']}

RoadOrDic = collections.OrderedDict(sorted(RoadDic.items()))
RoadValueDic = {'AB':7, 'AC':5, 'BD':14, 'CE':17, 'DE':6, 'CF':6, 'FG':14, 'EG':6}
#RoadValueDic = {'AB':7,'AC':9, 'AD':14, 'BC':10,'BE':15, 'CD':2, 'CE':11, 'DF':9, 'EF':6}

NodeValue = {'A':0, 'B':0, 'C':0, 'D':0, 'E':0, 'F':0, 'G':0}
#NodeValue = {'A':0, 'B':0, 'C':0, 'D':0, 'E':0, 'F':0}
#result = a b c d f

key = list(RoadDic.keys())

def get_items(dic, key):
    tmp = list(dic.keys())
    for i in range(0, len(tmp)):
        if key in tmp[i]:
            return list(dic.values())[i]

def calculate_node(_RoadValueDic, _NodeValue, _flag, _WayNode):
    tlist = []
    for i in range(0, len(_WayNode)):
        tmp = _flag + _WayNode[i]
        reverseTmp = _WayNode[i] + _flag
        if tmp in _RoadValueDic:
            if _NodeValue[_WayNode[i]] > _RoadValueDic[tmp] + _NodeValue[_WayNode[i]] or _NodeValue[_WayNode[i]] == 0 :
                _NodeValue[_WayNode[i]] = _RoadValueDic[tmp] + _NodeValue[_WayNode[i]]
                tlist.append(_NodeValue[_WayNode[i]])
            else:
                tlist.append(_NodeValue[_WayNode[i]])
        elif reverseTmp in _RoadValueDic:
            if _NodeValue[_WayNode[i]] > _RoadValueDic[reverseTmp] + _NodeValue[_WayNode[i]] or _NodeValue[_WayNode[i]] == 0 :
                _NodeValue[_WayNode[i]] = _RoadValueDic[reverseTmp] + _NodeValue[_WayNode[i]]
                tlist.append(_NodeValue[_WayNode[i]])
            else:
                tlist.append(_NodeValue[_WayNode[i]])

    tnum = tlist[0]
    for i in range(1, len(tlist)):
        if tnum > tlist[i]:
            tnum = tlist[i]
    return _WayNode[tlist.index(tnum)]



flag = 'A'
result = []
result.append(flag)
while not flag == 'F':
    for i in range(0, len(RoadDic)):
        tmp = RoadDic[key[i]]
        if flag in tmp:
            tmp.pop(tmp.index(flag))
            RoadDic[flag] = tmp
    tmp = get_items(RoadOrDic, flag)
    flag = calculate_node(RoadValueDic, NodeValue, flag, tmp)
    result.append(flag)


""""
    RoadDic = {'A':['B','C'], 'B':['A','D'], 'C':['A','E','F'], 'D':['B','E'], 'E':['C','D','G'], 'F':['C','G'], 'G':['E','F']}
    tmp = list(RoadDic['E'])
    tmp.pop(tmp.index('C'))
    RoadDic['E'] = tmp
    print(RoadDic['E'])
"""

print(result)


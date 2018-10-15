import collections
import RoadData

#==========================================================================
# 다리 별 가중치 입력
#==========================================================================
RoadValueDic = {'AB':7,'AC':9, 'AD':14, 'BC':10,'BE':15, 'CD':2, 'CE':11, 'DF':9, 'EF':6}

#==========================================================================
# 노드간의 연결 정보 입력
#==========================================================================
RoadDic = {}
RoadData.make_nodedic(RoadValueDic.keys(), RoadDic)
RoadOrDic = collections.OrderedDict(sorted(RoadDic.items()))
#==========================================================================
# 노드 구성 및 노드값 초기화
#==========================================================================
NodeValue = {}
RoadData.make_nodeValueDic(RoadOrDic, NodeValue)


#==========================================================================
# 해당하는 키의 값을 가져오는 함수
#==========================================================================
# dic = 노드의 다리 연결 정보
# key = 찾고자하는 노드

def get_items(dic, key):
    tmp = list(dic.keys())
    for i in range(0, len(tmp)):
        if key in tmp[i]:
            return list(dic.values())[i]

#==========================================================================
# 노드를 이동할 때 마다 연결된 노드와 가중치 계산
#==========================================================================
# _RoadValueDic = 위에서 초기화한 다리별 가중치 값의 리스트
# _NodeValue = 노드들의 값이 저장되있는 리스트
# _flag = 지금 현재 머물러있는 노드
# _WayNode = _flag 노드의 다리 연결 정보
def calculate_node(_RoadValueDic, _NodeValue, _flag, _WayNode):
    # 각각의 가중치들을 저장하는 리스트
    tlist = []

    # _flag에서 연결된 다리 갯수 만큼 순환
    for i in range(0, len(_WayNode)):
        #딕셔너리에서 해당 가중치를 찾기 위해 문자열 조합
        # ex) A와 B 사이의 다리 = AB , BA
        tmp = _flag + _WayNode[i]
        reverseTmp = _WayNode[i] + _flag

        # ==========================================================================
        # 가중치 계산 시작
        # ==========================================================================
        if tmp in _RoadValueDic:
            #초기 노드이거나 해당 노드의 값이 지금 계산한 값보다 작을 때
            #노드값을 계산 값으로 대체 후  가중치 리스트에 저장
            if _NodeValue[_WayNode[i]] > _RoadValueDic[tmp] + _NodeValue[_WayNode[i]] or _NodeValue[_WayNode[i]] == 0 :
                _NodeValue[_WayNode[i]] = _RoadValueDic[tmp] + _NodeValue[_WayNode[i]]
                tlist.append(_NodeValue[_WayNode[i]])
            #그것이 아니라면 기존의 값 유지
            #이 후 마찮가지로 가중치 리스트에 저장
            else:
                tlist.append(_NodeValue[_WayNode[i]])
        #계산 중 이름이 바뀌어 있을 경우를 대비한 루틴
        #ex) AB, AC, DG의 정방향이 아닌 BA, CA, GD의 경우
        elif reverseTmp in _RoadValueDic:
            if _NodeValue[_WayNode[i]] > _RoadValueDic[reverseTmp] + _NodeValue[_WayNode[i]] or _NodeValue[_WayNode[i]] == 0 :
                _NodeValue[_WayNode[i]] = _RoadValueDic[reverseTmp] + _NodeValue[_WayNode[i]]
                tlist.append(_NodeValue[_WayNode[i]])
            else:
                tlist.append(_NodeValue[_WayNode[i]])

    #초기값을 0으로 잡고 가중치값 비교 시작
    tnum = tlist[0]
    for i in range(1, len(tlist)):
        if tnum > tlist[i]:
            tnum = tlist[i]

    #결과를 리턴
    return _WayNode[tlist.index(tnum)]

def main():
    #노드를 미리 리스트로 반환해서 넣어둠
    key = list(RoadDic.keys())

    #출발 경로 설정
    flag = 'A'
    result = []
    result.append(flag)

    # 출발지를 'F'로 설정(바꾸고싶으면 해당 알파벳을 변경하면 됨
    # 탐색 시작
    while not flag == 'F':
        for i in range(0, len(RoadDic)):
            tmp = RoadDic[key[i]]
            if flag in tmp:
                tmp.pop(tmp.index(flag))
                RoadDic[flag] = tmp
        tmp = get_items(RoadOrDic, flag)
        flag = calculate_node(RoadValueDic, NodeValue, flag, tmp)
        result.append(flag)
    print(result)
    
main()

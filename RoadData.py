def node_add(_dic={}, _node=""):
    if not _node in _dic.keys():
        _dic[_node] = []


def check_node(_dic={}, _node="", _insert=""):
    if not _insert in _dic[_node]:
        _dic[_node].append(_insert)

def make_nodedic(_key, _resultNode):
    for i in _key:
        node_add(_resultNode, i[0])
        node_add(_resultNode, i[1])
        check_node(_resultNode, i[0], i[1])
        check_node(_resultNode, i[1], i[0])

def make_nodeValueDic(_key, _node_value):
    for i in _key.keys():
        _node_value[i] = 0

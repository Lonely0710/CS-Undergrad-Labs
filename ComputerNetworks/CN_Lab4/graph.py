import collections

class Graph:
    def __init__(self):
        """
        初始化图对象。
        节点在添加边时被隐式添加。
        边以邻接表的形式存储：
        self.edges = {
            'A': {'B': 10, 'C': 3},
            'B': {'A': 10, 'D': 2, 'C': 1},
            ...
        }
        self.nodes 存储所有唯一节点名的集合。
        """
        self.edges = collections.defaultdict(dict)
        self.nodes = set()

    def add_edge(self, from_node, to_node, cost):
        """
        添加一条有向边到图中。
        如果你想要无向边，应调用此函数两次，
        一次从 from_node 到 to_node，另一次从 to_node 到 from_node。
        """
        self.edges[from_node][to_node] = cost
        self.nodes.add(from_node)
        self.nodes.add(to_node)

    def get_neighbors(self, node):
        """
        返回给定节点的所有邻居及其到达成本。
        如果该节点没有出边或不存在，则返回一个空字典。
        """
        return self.edges[node]

    def get_nodes(self):
        """
        返回图中的所有节点列表。
        """
        return list(self.nodes)

    def __str__(self):
        """
        图的字符串表示形式。
        """
        output = []
        for from_node in self.edges:
            for to_node, cost in self.edges[from_node].items():
                output.append(f"{from_node} -> {to_node} (cost: {cost})")
        if not output:
            return "图为空。"
        return "\n".join(output)


def load_graph_from_file(filepath):
    """
    从文本文件加载图。
    文件中的每一行应表示一条边，格式如下：
    from_node to_node cost
    示例：
    A B 10
    A C 3
    B D 2
    对于无向图，如果算法需要，应在两个方向都列出边。
    对于 Dijkstra 算法，通常我们根据输入定义有向边。
    如果输入暗示是无向连接，请自行在两个方向添加边。
    """
    graph = Graph()
    try:
        with open(filepath, 'r') as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith('#'):  # 跳过空行或注释
                    continue
                parts = line.split()
                if len(parts) == 3:
                    from_node, to_node, cost_str = parts
                    try:
                        cost = int(cost_str)  # 假设成本为整数
                        if cost < 0:
                            print(f"警告：从 {from_node} 到 {to_node} 的边成本为负 ({cost})。Dijkstra 算法假设权重非负。")
                        graph.add_edge(from_node, to_node, cost)
                        # 如果链接是无向的，请取消下一行注释以添加反向边
                        # graph.add_edge(to_node, from_node, cost)
                    except ValueError:
                        print(f"警告：无法将边 {from_node}-{to_node} 的成本 '{cost_str}' 解析为整数。跳过此边。")
                else:
                    print(f"警告：格式错误的行 '{line}'。期望格式为 'from_node to_node cost'。跳过此行。")
    except FileNotFoundError:
        print(f"错误：未找到文件 '{filepath}'。")
        return None
    except Exception as e:
        print(f"读取文件时发生错误：{e}")
        return None
    return graph
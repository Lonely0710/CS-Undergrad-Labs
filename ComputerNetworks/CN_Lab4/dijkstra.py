import heapq

def dijkstra(graph, start_node):
    """
    实现 Dijkstra 算法，以查找从起始节点到图中所有其他节点的最短路径。

    参数：
        graph (Graph): 图对象（来自 graph.py）。
        start_node (str): 起始节点的标识符。

    返回：
        tuple: 一个包含两个字典的元组：
               - distances (dict): 从起始节点到每个节点的最短距离。
                                   格式：{'node': distance}
               - previous_nodes (dict): 每个节点在最短路径中的前驱节点。
                                        格式：{'node': 'previous_node'}
               如果未找到起始节点，则返回 (None, None)
    """
    if start_node not in graph.get_nodes():
        print(f"错误：图中未找到起始节点 '{start_node}'。")
        return None, None

    # 初始化距离：所有节点的距离为无穷大，起始节点为0
    distances = {node: float('inf') for node in graph.get_nodes()}
    distances[start_node] = 0

    # 存储每个节点在最短路径中的前驱节点
    previous_nodes = {node: None for node in graph.get_nodes()}

    # 优先队列用于存储 (距离, 节点)
    # 我们使用最小堆，这样距离较小的节点会被优先处理
    priority_queue = [(0, start_node)]  # (距离, 节点名)

    while priority_queue:
        # 获取当前距离最小的节点
        current_distance, current_node = heapq.heappop(priority_queue)

        # 如果我们已经找到了更短的路径，跳过当前节点
        if current_distance > distances[current_node]:
            continue

        # 遍历当前节点的所有邻居
        for neighbor, cost in graph.get_neighbors(current_node).items():
            new_distance = current_distance + cost

            # 如果找到了到邻居节点的更短路径
            if new_distance < distances[neighbor]:
                distances[neighbor] = new_distance
                previous_nodes[neighbor] = current_node
                heapq.heappush(priority_queue, (new_distance, neighbor))

    return distances, previous_nodes


def get_shortest_path(previous_nodes, start_node, end_node):
    """
    使用 Dijkstra 算法返回的 previous_nodes 字典重建从起始节点到目标节点的最短路径。

    参数：
        previous_nodes (dict): 前驱节点字典。
        start_node (str): 起始节点。
        end_node (str): 目标节点。

    返回：
        list: 表示从起始节点到目标节点最短路径的节点列表。
              如果找不到路径或目标节点不可达，返回空列表。
    """
    path = []
    current_node = end_node

    # 从终点回溯到起点
    while current_node is not None and current_node in previous_nodes:
        path.append(current_node)
        if current_node == start_node:
            break
        current_node = previous_nodes[current_node]

    # 如果没有成功回溯到起始节点（且它不是起始节点本身）
    # 或者该节点从未被访问过（前驱为 None 且不是起始节点）
    if not path or path[-1] != start_node:
        return []  # 未找到路径或目标节点不可达

    return path[::-1]  # 反转路径，使其从起点到终点排列
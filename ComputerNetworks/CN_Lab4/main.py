from graph import Graph, load_graph_from_file
from dijkstra import dijkstra, get_shortest_path

def main():
    # 定义网络拓扑文件的路径
    # 选项1：从文件加载
    network_file = "network.txt"
    print(f"正在从 '{network_file}' 加载图...")
    graph = load_graph_from_file(network_file)

    if graph is None:
        print("无法加载图。程序退出。")
        return
    
    if not graph.get_nodes():
        print("加载后图为空。没有可处理的内容。程序退出。")
        return

    print("\n图加载成功：")
    print(graph)

    # --- 替代方案：手动定义一个图用于快速测试 ---
    # print("手动定义图...")
    # graph = Graph()
    # graph.add_edge('A', 'B', 1)
    # graph.add_edge('A', 'C', 4)
    # graph.add_edge('B', 'C', 2)
    # graph.add_edge('B', 'D', 5)
    # graph.add_edge('C', 'D', 1)
    # graph.add_edge('D', 'A', 7) # 示例：回边或不同成本的返回路径
    # ------------------------------------------------------------

    all_nodes = graph.get_nodes()
    if not all_nodes:
        print("图中没有节点可供选择起点。")
        return
        
    start_node_input = input(f"\n请输入起始节点（从 {all_nodes} 中选择）: ").strip()

    if start_node_input not in all_nodes:
        print(f"错误：节点 '{start_node_input}' 不在加载的图节点列表中：{all_nodes}")
        # 如果输入无效，则尝试使用第一个节点作为默认值，或者退出程序
        if all_nodes:
            start_node = all_nodes[0]
            print(f"使用第一个节点 '{start_node}' 作为默认起始节点。")
        else:
            print("没有可用节点来选择起始节点。程序退出。")
            return
    else:
        start_node = start_node_input


    print(f"\n正在使用 Dijkstra 算法计算从节点 '{start_node}' 出发的最短路径...")
    distances, previous_nodes = dijkstra(graph, start_node)

    if distances is None or previous_nodes is None:
        print("无法运行 Dijkstra 算法（例如，未找到起始节点）。")
        return

    print("\n--- 结果 ---")
    print(f"从 '{start_node}' 出发的最短距离：")
    for node, distance in distances.items():
        if distance == float('inf'):
            print(f"  到 {node}：不可达")
        else:
            print(f"  到 {node}：{distance}")

    print(f"\n从 '{start_node}' 出发的最短路径：")
    for node in graph.get_nodes():
        if node == start_node:
            print(f"  到 {node}：[{start_node}] （成本：0）")
            continue
        
        path = get_shortest_path(previous_nodes, start_node, node)
        if path:
            cost = distances[node]
            print(f"  到 {node}：{' -> '.join(path)} （成本：{cost}）")
        else:
            # 此条件应与 distance == float('inf') 对齐
            # 但如果图中有不连通的部分，previous_nodes 可能不完整，get_shortest_path 也能处理这种情况。
            if distances[node] == float('inf'):
                print(f"  到 {node}：未找到路径（不可达）")
            else:
                # 如果 Dijkstra 运行正常且节点在 distances 中，此情况理论上不应发生
                print(f"  到 {node}：无法构造路径，但距离为 {distances[node]}。请检查图或算法逻辑。")


if __name__ == "__main__":
    main()
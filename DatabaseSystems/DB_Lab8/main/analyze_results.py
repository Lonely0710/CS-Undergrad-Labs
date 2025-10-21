# File: analyze_results.py
import pandas as pd
import matplotlib.pyplot as plt

# 读取日志数据
df = pd.read_csv('operations.log', sep='|', names=['Time', 'Operation', 'Duration'])
df['Duration'] = df['Duration'].str.replace('s', '').astype(float)

# 按操作类型分组
query_no_index = df[df['Operation'] == 'QUERY_NO_INDEX']
query_with_index = df[df['Operation'] == 'QUERY_WITH_INDEX']
insertions = df[df['Operation'] == 'INSERT']

# 绘制查询时间对比
plt.figure(figsize=(10, 6))
plt.plot(query_no_index['Time'], query_no_index['Duration'], label='No Index')
plt.plot(query_with_index['Time'], query_with_index['Duration'], label='With Index')
plt.title('Query Performance Comparison')
plt.xlabel('Time')
plt.ylabel('Duration (s)')
plt.legend()
plt.savefig('query_performance.png')

# 绘制插入时间趋势
plt.figure(figsize=(10, 6))
plt.plot(insertions['Time'], insertions['Duration'])
plt.title('Insertion Time Trend')
plt.xlabel('Time')
plt.ylabel('Duration (s)')
plt.savefig('insertion_trend.png')
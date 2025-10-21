# File: stress_test_no_index.py
import time
import threading
import mysql.connector
from basic_operations import DB_CONFIG, log_operation

# 并发配置
INSERT_THREADS = 3    # 插入线程数
QUERY_INTERVAL = 1    # 查询间隔（秒）
INSERT_RATE = 100     # 每个线程每秒插入量

def insert_data():
    """并发插入数据"""
    conn = mysql.connector.connect(**DB_CONFIG)
    cursor = conn.cursor()
    for _ in range(INSERT_RATE):
        try:
            cursor.execute("""
                INSERT INTO Menu (name, price, category_id)
                VALUES (%s, %s, %s)
            """, (f"Dummy_{time.time()}", 10.00, 2))
            conn.commit()
        except:
            conn.rollback()
    cursor.close()
    conn.close()

def query_data():
    """定期查询（无索引）"""
    while True:
        start = time.time()
        conn = mysql.connector.connect(**DB_CONFIG)
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM Menu WHERE name = '宫保鸡丁'")  # 非主属性查询
        cursor.fetchall()
        duration = time.time() - start
        log_operation("QUERY_NO_INDEX", duration)
        time.sleep(QUERY_INTERVAL)

if __name__ == "__main__":
    # 启动插入线程
    for _ in range(INSERT_THREADS):
        threading.Thread(target=insert_data, daemon=True).start()
    # 启动查询线程
    threading.Thread(target=query_data, daemon=True).start()
    
    # 运行 60 秒
    start_time = time.time()
    while time.time() - start_time < 60:
        time.sleep(1)
    
    print("压力测试完成：运行时间 1 分钟")
# File: basic_operations.py 基础操作与耗时记录
import time
import mysql.connector
from mysql.connector import Error

# 数据库配置
DB_CONFIG = {
    "host": "localhost",
    "database": "restaurant_test",
    "user": "root",
    "password": "yll20040710"
}

def log_operation(op_type, duration):
    """记录操作耗时到日志文件"""
    with open('operations.log', 'a') as f:
        f.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')} | {op_type} | {duration:.4f}s\n")

def execute_query():
    """查询操作：根据菜名查询"""
    try:
        start = time.time()
        conn = mysql.connector.connect(**DB_CONFIG)
        cursor = conn.cursor()
        query = "SELECT * FROM Menu WHERE name = '宫保鸡丁'"  # 非主属性A=name
        cursor.execute(query)
        cursor.fetchall()
        duration = time.time() - start
        log_operation("QUERY", duration)
    except Error as e:
        print(f"Query Error: {e}")
    finally:
        if conn.is_connected():
            cursor.close()
            conn.close()

def execute_insert():
    """插入操作：插入新菜品"""
    try:
        start = time.time()
        conn = mysql.connector.connect(**DB_CONFIG)
        cursor = conn.cursor()
        insert_sql = """
        INSERT INTO Menu (name, price, category_id, style, description)
        VALUES (%s, %s, %s, %s, %s)
        """
        data = ('糖醋排骨', 35.00, 2, '江浙菜', '经典酸甜口味')
        cursor.execute(insert_sql, data)
        conn.commit()
        duration = time.time() - start
        log_operation("INSERT", duration)
    except Error as e:
        print(f"Insert Error: {e}")
    finally:
        if conn.is_connected():
            cursor.close()
            conn.close()

if __name__ == "__main__":
    execute_query()
    execute_insert()
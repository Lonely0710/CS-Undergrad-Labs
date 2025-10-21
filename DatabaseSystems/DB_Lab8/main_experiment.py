# File: main_experiment.py
import time
import threading
import argparse
import pandas as pd
import matplotlib.pyplot as plt
from mysql.connector import Error, pooling
from datetime import datetime
import re
from io import StringIO
import seaborn as sns

# 全局配置
DB_CONFIG = {
    "host": "localhost",
    "database": "restaurant_test",
    "user": "root",
    "password": "yll20040710"
}
LOG_FILE = "operations.log"
INSERT_THREADS = 3       # 并发插入线程数
INSERT_RATE_PER_THREAD = 100  # 每个线程每秒插入量
TEST_DURATION = 60       # 压力测试持续时间（秒）
ROWS_PER_INSERT_BATCH = 10

# 连接池配置
conn_pool = pooling.MySQLConnectionPool(
    pool_name="mypool",
    pool_size=32,                # 根据并发量调整
    pool_reset_session=True,
    **DB_CONFIG
)

# ------------------------- 工具函数 -------------------------
def log_operation(op_type, duration):
    """记录操作耗时"""
    with open(LOG_FILE, 'a') as f:
        f.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')}|{op_type}|{duration:.4f}\n")

def reset_database(create_index=False):
    """安全清空数据并管理索引"""
    try:
        conn = conn_pool.get_connection()
        cursor = conn.cursor()
        
        # 1. 禁用外键检查
        cursor.execute("SET FOREIGN_KEY_CHECKS = 0")
        
        # 2. 清空关联的子表数据
        cursor.execute("DELETE FROM OrderItem")  # 先清空子表
        
        # 3. 清空主表数据
        cursor.execute("TRUNCATE TABLE Menu")    # 再清空主表
        
        # 4. 管理索引
        if create_index:
            cursor.execute("CREATE INDEX idx_name ON Menu(name)")
        else:
            cursor.execute("DROP INDEX idx_name ON Menu")  # 确保无索引
        
        # 5. 恢复外键检查
        cursor.execute("SET FOREIGN_KEY_CHECKS = 1")
        conn.commit()
        
    except Error as e:
        print(f"Reset Error: {e}")
    finally:
        if conn.is_connected():
            cursor.close()
            conn.close()

# ------------------------- 模块1：基础操作 -------------------------
def basic_operations():
    """单次查询和插入"""
    # 查询
    start = time.time()
    try:
        conn = conn_pool.get_connection()
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM Menu WHERE name = '宫保鸡丁'")
        cursor.fetchall()
        log_operation("QUERY_BASIC", time.time() - start)
    except Error as e:
        print(f"Query Error: {e}")

    # 插入
    start = time.time()
    try:
        conn = conn_pool.get_connection()
        cursor = conn.cursor()
        cursor.execute(
            "INSERT INTO Menu (name, price, category_id) VALUES (%s, %s, %s)",
            ("糖醋排骨", 35.00, 2)
        )
        conn.commit()
        log_operation("INSERT_BASIC", time.time() - start)
    except Error as e:
        print(f"Insert Error: {e}")

# ------------------------- 模块2/3：压力测试 -------------------------
def stress_test(use_index=False):
    """压力测试核心函数"""
    reset_database(create_index=use_index)
    test_type = "WITH_INDEX" if use_index else "NO_INDEX"

    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    separator = f"\n{'=' * 30} {timestamp} {'=' * 30}\n"
    with open(LOG_FILE, 'a', encoding='utf-8') as f:
        f.write(separator)   

    # 插入线程
    def insert_worker():
        while running:
            conn = None
            cursor = None
            try:
                conn = conn_pool.get_connection()
                cursor = conn.cursor()
                start = time.time()
                
                # 批量插入优化（一次插入10条）
                batch_data = [(f"Dummy_{time.time()}_{i}", 10.00, 2) for i in range(10)]
                cursor.executemany(
                    "INSERT INTO Menu (name, price, category_id) VALUES (%s, %s, %s)",
                    batch_data
                )
                conn.commit()
                
                # 记录单批次耗时（10条总时间）
                duration = time.time() - start
                log_operation(f"INSERT_{test_type}", duration)
                
                # 精确控制速率（100条/秒 = 10批次/秒）
                time.sleep(max(0, 0.1 - duration))  # 每批次间隔0.1秒
                
            except Error as e:
                print(f"Insert Worker Error: {e}")
            finally:
                # 强制释放资源
                if cursor:
                    cursor.close()
                if conn:
                    conn.close()  # 关键！必须显式归还连接

    # 查询线程
    def query_worker():
        while running:
            conn = None
            cursor = None
            try:
                start = time.time()
                conn = conn_pool.get_connection()
                cursor = conn.cursor()
                cursor.execute("SELECT * FROM Menu WHERE name = '宫保鸡丁'")
                cursor.fetchall()
                log_operation(f"QUERY_{test_type}", time.time() - start)
            except Error as e:
                print(f"Query Worker Error: {e}")
            finally:
                if cursor:
                    cursor.close()
                if conn:
                    conn.close()
            time.sleep(1)

    # 启动线程
    global running
    running = True
    threads = []
    for _ in range(INSERT_THREADS):
        t = threading.Thread(target=insert_worker)
        threads.append(t)
        t.start()
    
    qt = threading.Thread(target=query_worker)
    threads.append(qt)
    qt.start()

    # 运行指定时间
    time.sleep(TEST_DURATION)
    running = False
    for t in threads:
        t.join()

# ------------------------- 模块4：数据分析 -------------------------
def analyze_results():
    """
    生成性能对比图表。
    能够处理包含多个测试段（由分隔符标记）的日志文件，
    并以数据库中的近似总行数作为X轴。
    """
    try:
        with open(LOG_FILE, 'r', encoding='utf-8') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Error: Log file '{LOG_FILE}' not found.")
        return
    except Exception as e:
        print(f"Error reading log file: {e}")
        return

    # Regex to split log content by separator
    # Separator pattern: '======== YYYY-MM-DD HH:MM:SS ========'
    segments_content = re.split(r'\n={20,}\s*\d{4}-\d{2}-\d{2}\s\d{2}:\d{2}:\d{2}\s*={20,}\n', content)
    segments_content = [s.strip() for s in segments_content if s.strip()]

    if not segments_content:
        print("No valid data segments found in the log file.")
        return

    all_processed_data = []

    for i, segment_str in enumerate(segments_content):
        if not segment_str:
            continue

        try:
            first_op_line = segment_str.split('\n', 1)[0]
            segment_test_type = "UNKNOWN"
            if 'WITH_INDEX' in first_op_line:
                segment_test_type = "WITH_INDEX"
            elif 'NO_INDEX' in first_op_line:
                segment_test_type = "NO_INDEX"
            else:
                print(f"Warning: Could not determine test type from the first line of segment {i+1}.")

            df_segment = pd.read_csv(StringIO(segment_str), sep='|', names=['Time', 'Operation', 'Duration'], on_bad_lines='skip')
            if df_segment.empty:
                print(f"Warning: Log segment {i+1} is empty or could not be parsed.")
                continue

            df_segment['Timestamp'] = pd.to_datetime(df_segment['Time'], errors='coerce')
            df_segment = df_segment.dropna(subset=['Timestamp']).sort_values(by='Timestamp').reset_index(drop=True)

            if df_segment.empty:
                print(f"Warning: Log segment {i+1} is empty after timestamp processing.")
                continue

            df_segment['IsInsert'] = df_segment['Operation'].str.startswith('INSERT_')
            df_segment['CumulativeInsertBatches'] = df_segment['IsInsert'].cumsum()
            df_segment['ActualRowsInDB'] = df_segment['CumulativeInsertBatches'] * ROWS_PER_INSERT_BATCH
            
            # Determine TestType from Operation column for more accuracy if needed
            df_segment['OperationTestType'] = df_segment['Operation'].apply(
                lambda x: 'WITH_INDEX' if 'WITH_INDEX' in x else ('NO_INDEX' if 'NO_INDEX' in x else 'UNKNOWN')
            )
            
            # If segment_test_type was UNKNOWN, try to infer from majority in OperationTestType
            if segment_test_type == "UNKNOWN":
                mode_types = df_segment[df_segment['OperationTestType'] != 'UNKNOWN']['OperationTestType'].mode()
                if not mode_types.empty:
                    segment_test_type = mode_types[0]
            
            df_segment['SegmentTestType'] = segment_test_type # Assign a consistent type for the whole segment
            df_segment['SegmentID'] = i 

            all_processed_data.append(df_segment)

        except pd.errors.ParserError as e:
            print(f"Error parsing log segment {i+1}: {e}")
        except Exception as e:
            print(f"Unknown error processing log segment {i+1}: {e}")

    if not all_processed_data:
        print("No data segments could be processed from the log file.")
        return

    df_combined = pd.concat(all_processed_data, ignore_index=True)

    if df_combined.empty:
        print("Combined data is empty. Cannot generate charts.")
        return

    # --- Chart 1: Performance Comparison by Index Type ---
    fig1, axes1 = plt.subplots(2, 1, figsize=(14, 12), sharex=True)
    fig1.suptitle("Performance Comparison vs. Number of Rows in DB", fontsize=16)

    # Subplot 1: Query Performance
    ax1_query = axes1[0]
    for test_type_group in ['NO_INDEX', 'WITH_INDEX']:
        segments_for_type = df_combined[df_combined['SegmentTestType'] == test_type_group]
        for segment_id in segments_for_type['SegmentID'].unique():
            subset = segments_for_type[
                (segments_for_type['SegmentID'] == segment_id) & 
                (segments_for_type['Operation'] == f'QUERY_{test_type_group}')
            ]
            if not subset.empty:
                ax1_query.plot(subset['ActualRowsInDB'], subset['Duration'], marker='.', linestyle='-', label=f'Query {test_type_group} (Run {segment_id+1})')
    ax1_query.set_title("Query Performance Comparison")
    ax1_query.set_ylabel("Duration (seconds)")
    ax1_query.legend(loc='upper left')
    ax1_query.grid(True)

    # Subplot 2: Insert Performance
    ax1_insert = axes1[1]
    for test_type_group in ['NO_INDEX', 'WITH_INDEX']:
        segments_for_type = df_combined[df_combined['SegmentTestType'] == test_type_group]
        for segment_id in segments_for_type['SegmentID'].unique():
            subset = segments_for_type[
                (segments_for_type['SegmentID'] == segment_id) & 
                (segments_for_type['Operation'] == f'INSERT_{test_type_group}')
            ]
            if not subset.empty:
                ax1_insert.plot(subset['ActualRowsInDB'], subset['Duration'], marker='.', linestyle='-', label=f'Insert {test_type_group} (Run {segment_id+1})')
    ax1_insert.set_title("Insert Performance Comparison")
    ax1_insert.set_xlabel("Approx. Total Rows in DB")
    ax1_insert.set_ylabel(f"Batch Duration (seconds) - {ROWS_PER_INSERT_BATCH} rows/batch")
    ax1_insert.legend(loc='upper left')
    ax1_insert.grid(True)

    plt.tight_layout(rect=[0, 0, 1, 0.96]) # Adjust layout to make space for suptitle
    plt.savefig("performance_comparison_by_rows.png")
    print("Chart 'performance_comparison_by_rows.png' saved.")
    # plt.show() # Uncomment to display charts immediately

    # --- Chart 2: Insert vs. Query Performance for Each Index Strategy ---
    fig2, axes2 = plt.subplots(1, 2, figsize=(18, 7), sharey=False) # Y-axes can have different scales
    fig2.suptitle("Insert vs. Query Performance vs. Number of Rows in DB (by Index Strategy)", fontsize=16)

    # Subplot 1: NO_INDEX
    ax2_no_index = axes2[0]
    segments_no_index = df_combined[df_combined['SegmentTestType'] == 'NO_INDEX']
    for segment_id in segments_no_index['SegmentID'].unique():
        subset_insert = segments_no_index[(segments_no_index['SegmentID'] == segment_id) & (segments_no_index['Operation'] == 'INSERT_NO_INDEX')]
        subset_query = segments_no_index[(segments_no_index['SegmentID'] == segment_id) & (segments_no_index['Operation'] == 'QUERY_NO_INDEX')]
        if not subset_insert.empty:
            ax2_no_index.plot(subset_insert['ActualRowsInDB'], subset_insert['Duration'], label=f'INSERT_NO_INDEX (Run {segment_id+1})', marker='.', linestyle='-')
        if not subset_query.empty:
            ax2_no_index.plot(subset_query['ActualRowsInDB'], subset_query['Duration'], label=f'QUERY_NO_INDEX (Run {segment_id+1})', marker='o', linestyle='--')
    ax2_no_index.set_title("NO_INDEX: Insert vs. Query")
    ax2_no_index.set_xlabel("Approx. Total Rows in DB")
    ax2_no_index.set_ylabel("Duration (seconds)")
    ax2_no_index.legend(loc='upper left')
    ax2_no_index.grid(True)

    # Subplot 2: WITH_INDEX
    ax2_with_index = axes2[1]
    segments_with_index = df_combined[df_combined['SegmentTestType'] == 'WITH_INDEX']
    for segment_id in segments_with_index['SegmentID'].unique():
        subset_insert = segments_with_index[(segments_with_index['SegmentID'] == segment_id) & (segments_with_index['Operation'] == 'INSERT_WITH_INDEX')]
        subset_query = segments_with_index[(segments_with_index['SegmentID'] == segment_id) & (segments_with_index['Operation'] == 'QUERY_WITH_INDEX')]
        if not subset_insert.empty:
            ax2_with_index.plot(subset_insert['ActualRowsInDB'], subset_insert['Duration'], label=f'INSERT_WITH_INDEX (Run {segment_id+1})', marker='.', linestyle='-')
        if not subset_query.empty:
            ax2_with_index.plot(subset_query['ActualRowsInDB'], subset_query['Duration'], label=f'QUERY_WITH_INDEX (Run {segment_id+1})', marker='o', linestyle='--')
    ax2_with_index.set_title("WITH_INDEX: Insert vs. Query")
    ax2_with_index.set_xlabel("Approx. Total Rows in DB")
    ax2_with_index.set_ylabel("Duration (seconds)")
    ax2_with_index.legend(loc='upper left')
    ax2_with_index.grid(True)

    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.savefig("insert_vs_query_by_rows.png")
    print("Chart 'insert_vs_query_by_rows.png' saved.")
    # plt.show() # Uncomment to display charts immediately

# ------------------------- 模块4：数据分析 -------------------------
def plot_additional_metrics_from_log(log_file_path, rows_per_insert_batch, output_filename="additional_performance_metrics_from_log.png"):
    """
    Reads a log file, processes the data, and generates a plot with additional performance metrics:
    1. Average operation duration (bar chart).
    2. Operation duration distribution (box plot).
    3. Insert throughput vs. Number of Rows in DB.
    4. Query throughput vs. Number of Rows in DB.

    Args:
        log_file_path (str): Path to the log file.
        rows_per_insert_batch (int): Number of rows inserted in each INSERT batch.
        output_filename (str): Name of the file to save the plot to.
    """
    try:
        with open(log_file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Error: Log file '{log_file_path}' not found.")
        return
    except Exception as e:
        print(f"Error reading log file: {e}")
        return

    # Regex to split log content by separator
    # Separator pattern: '======== YYYY-MM-DD HH:MM:SS ========'
    segments_content = re.split(r'\n={20,}\s*\d{4}-\d{2}-\d{2}\s\d{2}:\d{2}:\d{2}\s*={20,}\n', content)
    segments_content = [s.strip() for s in segments_content if s.strip()]

    if not segments_content:
        print("No valid data segments found in the log file.")
        return

    all_processed_data = []
    for i, segment_str in enumerate(segments_content):
        if not segment_str:
            continue

        try:
            # Determine SegmentTestType from the first operation in the segment string
            first_data_line = segment_str.split('\n', 1)[0]
            segment_test_type = "UNKNOWN" # Default

            if not first_data_line:
                 print(f"Warning: Log segment {i+1} appears empty after splitting first data line.")
                 continue

            # Infer from first data line's content (assuming it's an operation log)
            # The first part of first_data_line before '|' is the timestamp,
            # the second part is the Operation name.
            try:
                operation_name_in_first_line = first_data_line.split('|', 2)[1]
                if 'WITH_INDEX' in operation_name_in_first_line:
                    segment_test_type = "WITH_INDEX"
                elif 'NO_INDEX' in operation_name_in_first_line:
                    segment_test_type = "NO_INDEX"
            except IndexError:
                 print(f"Warning: Could not parse operation name from first line of segment {i+1} to determine test type: '{first_data_line[:50]}...'")


            df_segment = pd.read_csv(StringIO(segment_str), sep='|', names=['Time', 'Operation', 'Duration'], on_bad_lines='skip')
            if df_segment.empty:
                print(f"Warning: Log segment {i+1} (inferred type: '{segment_test_type}') is empty or could not be parsed.")
                continue

            # If segment_test_type is still UNKNOWN, try to infer from majority in Operation column
            if segment_test_type == "UNKNOWN":
                df_segment['OpTypeInferred'] = df_segment['Operation'].apply(
                    lambda x: 'WITH_INDEX' if isinstance(x, str) and 'WITH_INDEX' in x else ('NO_INDEX' if isinstance(x, str) and 'NO_INDEX' in x else 'UNKNOWN')
                )
                mode_types = df_segment[df_segment['OpTypeInferred'] != 'UNKNOWN']['OpTypeInferred'].mode()
                if not mode_types.empty:
                    segment_test_type = mode_types[0]
                else:
                    # Fallback: if all are UNKNOWN, keep segment_test_type as UNKNOWN
                    print(f"Warning: Could not determine a dominant test type for segment {i+1} from its operations. Proceeding as '{segment_test_type}'.")
                df_segment = df_segment.drop(columns=['OpTypeInferred'])


            df_segment['Timestamp'] = pd.to_datetime(df_segment['Time'], errors='coerce')
            df_segment = df_segment.dropna(subset=['Timestamp']).sort_values(by='Timestamp').reset_index(drop=True)

            if df_segment.empty:
                print(f"Warning: Log segment {i+1} (type: '{segment_test_type}') is empty after timestamp processing.")
                continue

            df_segment['IsInsert'] = df_segment['Operation'].str.startswith('INSERT_')
            
            # Calculate ActualRowsInDB per segment
            current_inserts_in_segment = 0
            actual_rows_list = []
            for _, row_data in df_segment.iterrows():
                if row_data['IsInsert']:
                    current_inserts_in_segment += 1
                    actual_rows_list.append(current_inserts_in_segment * rows_per_insert_batch)
                else: 
                    actual_rows_list.append(current_inserts_in_segment * rows_per_insert_batch)
            df_segment['ActualRowsInDB'] = actual_rows_list
            
            df_segment['SegmentTestType'] = segment_test_type
            df_segment['SegmentID'] = i 

            all_processed_data.append(df_segment)

        except pd.errors.ParserError as e:
            print(f"Error parsing log segment {i+1}: {e}")
        except Exception as e:
            print(f"Unknown error processing log segment {i+1}: {e}")

    if not all_processed_data:
        print("No data segments could be processed from the log file.")
        return

    df_combined = pd.concat(all_processed_data, ignore_index=True)

    if df_combined.empty:
        print("Combined data is empty. Cannot generate charts.")
        return

    # --- Create Figure and Subplots ---
    fig, axes = plt.subplots(2, 2, figsize=(20, 16))
    fig.suptitle("Additional Performance Metrics Analysis (from Log File)", fontsize=18, y=0.98)

    # --- 1. Average Operation Duration (Bar Chart) ---
    ax0 = axes[0, 0]
    avg_durations = df_combined.groupby('Operation')['Duration'].mean().sort_index()
    if not avg_durations.empty:
        avg_durations.plot(kind='bar', ax=ax0, colormap='viridis')
        ax0.set_title("Average Operation Duration", fontsize=14)
        ax0.set_ylabel("Average Duration (seconds)", fontsize=12)
        ax0.set_xlabel("Operation Type", fontsize=12)
        # MODIFIED LINE: Removed ha='right'
        ax0.tick_params(axis='x', rotation=45, labelsize=10)
        ax0.tick_params(axis='y', labelsize=10)
        ax0.grid(True, axis='y', linestyle='--', alpha=0.7)
        # If specific alignment is still needed after rotation,
        # you might need to do something like:
        # for label in ax0.get_xticklabels():
        #     label.set_horizontalalignment('right')
    else:
        ax0.text(0.5, 0.5, "No data for average durations", ha='center', va='center', fontsize=12)
        ax0.set_title("Average Operation Duration", fontsize=14)


    # --- 2. Operation Duration Distribution (Box Plot) ---
    ax1 = axes[0, 1]
    if not df_combined.empty and 'Operation' in df_combined.columns and 'Duration' in df_combined.columns:
        unique_ops = df_combined['Operation'].unique()
        if len(unique_ops) > 0:
            operation_order = sorted(unique_ops)
            sns.boxplot(x='Operation', y='Duration', hue='Operation', data=df_combined, ax=ax1, order=operation_order, palette="Set2", fliersize=3, legend=False)
            ax1.set_title("Operation Duration Distribution", fontsize=14)
            ax1.set_ylabel("Duration (seconds)", fontsize=12)
            ax1.set_xlabel("Operation Type", fontsize=12)
            # MODIFIED LINE: Removed ha='right'
            ax1.tick_params(axis='x', rotation=45, labelsize=10)
            ax1.tick_params(axis='y', labelsize=10)
            ax1.grid(True, axis='y', linestyle='--', alpha=0.7)

    # --- 3. Insert Throughput vs. Number of Rows in DB ---
    ax2 = axes[1, 0]
    if 'IsInsert' in df_combined.columns:
        df_inserts = df_combined[df_combined['IsInsert']].copy()
        if not df_inserts.empty and 'Duration' in df_inserts.columns and df_inserts['Duration'].notna().all() and (df_inserts['Duration'] > 0).all():
            df_inserts['Throughput'] = rows_per_insert_batch / df_inserts['Duration']
            
            for test_type in sorted(df_inserts['SegmentTestType'].unique()):
                segments_for_type = df_inserts[df_inserts['SegmentTestType'] == test_type]
                for segment_id in sorted(segments_for_type['SegmentID'].unique()):
                    subset = segments_for_type[segments_for_type['SegmentID'] == segment_id]
                    if not subset.empty:
                        subset = subset.sort_values(by='ActualRowsInDB')
                        operation_name = subset['Operation'].iloc[0] 
                        ax2.plot(subset['ActualRowsInDB'], subset['Throughput'], marker='.', linestyle='-', 
                                 label=f'{operation_name} (Run {segment_id+1})')
            if ax2.has_data():
                ax2.legend(loc='best', fontsize=10)
        else:
            ax2.text(0.5, 0.5, "No valid INSERT data for throughput\n(check durations > 0)", ha='center', va='center', fontsize=12)
    else:
         ax2.text(0.5, 0.5, "Column 'IsInsert' not found", ha='center', va='center', fontsize=12)

    ax2.set_title("Insert Throughput vs. Rows in DB", fontsize=14)
    ax2.set_xlabel("Approx. Total Rows in DB", fontsize=12)
    ax2.set_ylabel(f"Insert Throughput (rows/second)", fontsize=12)
    ax2.tick_params(axis='both', labelsize=10)
    ax2.grid(True, alpha=0.7)


    # --- 4. Query Throughput vs. Number of Rows in DB ---
    ax3 = axes[1, 1]
    if 'IsInsert' in df_combined.columns:
        df_queries = df_combined[~df_combined['IsInsert']].copy()
        if not df_queries.empty and 'Duration' in df_queries.columns and df_queries['Duration'].notna().all() and (df_queries['Duration'] > 0).all():
            df_queries['Throughput'] = 1 / df_queries['Duration']

            for test_type in sorted(df_queries['SegmentTestType'].unique()):
                segments_for_type = df_queries[df_queries['SegmentTestType'] == test_type]
                for segment_id in sorted(segments_for_type['SegmentID'].unique()):
                    subset = segments_for_type[segments_for_type['SegmentID'] == segment_id]
                    if not subset.empty:
                        subset = subset.sort_values(by='ActualRowsInDB')
                        operation_name = subset['Operation'].iloc[0] 
                        ax3.plot(subset['ActualRowsInDB'], subset['Throughput'], marker='.', linestyle='-',
                                 label=f'{operation_name} (Run {segment_id+1})')
            if ax3.has_data():
                ax3.legend(loc='best', fontsize=10)
        else:
            ax3.text(0.5, 0.5, "No valid QUERY data for throughput\n(check durations > 0)", ha='center', va='center', fontsize=12)
    else:
        ax3.text(0.5, 0.5, "Column 'IsInsert' not found", ha='center', va='center', fontsize=12)
        
    ax3.set_title("Query Throughput vs. Rows in DB", fontsize=14)
    ax3.set_xlabel("Approx. Total Rows in DB", fontsize=12)
    ax3.set_ylabel("Query Throughput (queries/second)", fontsize=12)
    ax3.tick_params(axis='both', labelsize=10)
    ax3.grid(True, alpha=0.7)

    # --- Finalize and Save ---
    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig(output_filename)
    print(f"Chart '{output_filename}' saved.")
    # plt.show() # Uncomment to display charts immediately


def plot_additional_metrics_from_log_separate(log_file_path, rows_per_insert_batch, output_prefix="metric_plot"):
    """
    Reads a log file, processes the data, and generates FOUR SEPARATE plots for additional performance metrics:
    1. Average operation duration (bar chart).
    2. Operation duration distribution (box plot).
    3. Insert throughput vs. Number of Rows in DB.
    4. Query throughput vs. Number of Rows in DB.

    Args:
        log_file_path (str): Path to the log file.
        rows_per_insert_batch (int): Number of rows inserted in each INSERT batch.
        output_prefix (str): Prefix for the output image filenames.
                             Files will be saved as:
                             - {output_prefix}_avg_duration.png
                             - {output_prefix}_duration_dist.png
                             - {output_prefix}_insert_throughput.png
                             - {output_prefix}_query_throughput.png
    """
    try:
        with open(log_file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Error: Log file '{log_file_path}' not found.")
        return
    except Exception as e:
        print(f"Error reading log file: {e}")
        return

    segments_content = re.split(r'\n={20,}\s*\d{4}-\d{2}-\d{2}\s\d{2}:\d{2}:\d{2}\s*={20,}\n', content)
    segments_content = [s.strip() for s in segments_content if s.strip()]

    if not segments_content:
        print("No valid data segments found in the log file.")
        return

    all_processed_data = []
    for i, segment_str in enumerate(segments_content):
        if not segment_str:
            continue
        try:
            first_data_line = segment_str.split('\n', 1)[0]
            segment_test_type = "UNKNOWN"
            if not first_data_line:
                 print(f"Warning: Log segment {i+1} appears empty after splitting first data line.")
                 continue
            try:
                operation_name_in_first_line = first_data_line.split('|', 2)[1]
                if 'WITH_INDEX' in operation_name_in_first_line: segment_test_type = "WITH_INDEX"
                elif 'NO_INDEX' in operation_name_in_first_line: segment_test_type = "NO_INDEX"
            except IndexError:
                 print(f"Warning: Could not parse operation name from first line of segment {i+1}: '{first_data_line[:50]}...'")

            df_segment = pd.read_csv(StringIO(segment_str), sep='|', names=['Time', 'Operation', 'Duration'], on_bad_lines='skip')
            if df_segment.empty:
                print(f"Warning: Log segment {i+1} (inferred type: '{segment_test_type}') is empty or could not be parsed.")
                continue

            if segment_test_type == "UNKNOWN":
                df_segment['OpTypeInferred'] = df_segment['Operation'].apply(
                    lambda x: 'WITH_INDEX' if isinstance(x, str) and 'WITH_INDEX' in x else ('NO_INDEX' if isinstance(x, str) and 'NO_INDEX' in x else 'UNKNOWN')
                )
                mode_types = df_segment[df_segment['OpTypeInferred'] != 'UNKNOWN']['OpTypeInferred'].mode()
                if not mode_types.empty: segment_test_type = mode_types[0]
                else: print(f"Warning: Could not determine a dominant test type for segment {i+1}. Proceeding as '{segment_test_type}'.")
                df_segment = df_segment.drop(columns=['OpTypeInferred'])

            df_segment['Timestamp'] = pd.to_datetime(df_segment['Time'], errors='coerce')
            df_segment = df_segment.dropna(subset=['Timestamp']).sort_values(by='Timestamp').reset_index(drop=True)
            if df_segment.empty:
                print(f"Warning: Log segment {i+1} (type: '{segment_test_type}') is empty after timestamp processing.")
                continue

            df_segment['IsInsert'] = df_segment['Operation'].str.startswith('INSERT_')
            current_inserts_in_segment = 0
            actual_rows_list = []
            for _, row_data in df_segment.iterrows():
                if row_data['IsInsert']:
                    current_inserts_in_segment += 1
                    actual_rows_list.append(current_inserts_in_segment * rows_per_insert_batch)
                else: 
                    actual_rows_list.append(current_inserts_in_segment * rows_per_insert_batch)
            df_segment['ActualRowsInDB'] = actual_rows_list
            df_segment['SegmentTestType'] = segment_test_type
            df_segment['SegmentID'] = i 
            all_processed_data.append(df_segment)
        except pd.errors.ParserError as e: print(f"Error parsing log segment {i+1}: {e}")
        except Exception as e: print(f"Unknown error processing log segment {i+1}: {e}")

    if not all_processed_data:
        print("No data segments could be processed from the log file.")
        return
    df_combined = pd.concat(all_processed_data, ignore_index=True)
    if df_combined.empty:
        print("Combined data is empty. Cannot generate charts.")
        return

    # --- Plot 1: Average Operation Duration (Bar Chart) ---
    fig1, ax1_plot = plt.subplots(figsize=(10, 7)) # Adjusted figsize for single plot
    avg_durations = df_combined.groupby('Operation')['Duration'].mean().sort_index()
    if not avg_durations.empty:
        avg_durations.plot(kind='bar', ax=ax1_plot, colormap='viridis')
        ax1_plot.set_title("Average Operation Duration", fontsize=16)
        ax1_plot.set_ylabel("Average Duration (seconds)", fontsize=12)
        ax1_plot.set_xlabel("Operation Type", fontsize=12)
        ax1_plot.tick_params(axis='x', rotation=45, labelsize=10) # Removed ha
        ax1_plot.tick_params(axis='y', labelsize=10)
        ax1_plot.grid(True, axis='y', linestyle='--', alpha=0.7)
    else:
        ax1_plot.text(0.5, 0.5, "No data for average durations", ha='center', va='center', fontsize=12)
        ax1_plot.set_title("Average Operation Duration", fontsize=16)
    plt.tight_layout()
    filename1 = f"{output_prefix}_avg_duration.png"
    plt.savefig(filename1)
    print(f"Chart '{filename1}' saved.")
    plt.close(fig1) # Close the figure to free memory

    # --- Plot 2: Operation Duration Distribution (Box Plot) ---
    fig2, ax2_plot = plt.subplots(figsize=(10, 7))
    if not df_combined.empty and 'Operation' in df_combined.columns and 'Duration' in df_combined.columns:
        unique_ops = df_combined['Operation'].unique()
        if len(unique_ops) > 0:
            operation_order = sorted(unique_ops)
            sns.boxplot(x='Operation', y='Duration', hue='Operation', data=df_combined, ax=ax2_plot, 
                        order=operation_order, palette="Set2", fliersize=3, legend=False) # Added hue and legend=False
            ax2_plot.set_title("Operation Duration Distribution", fontsize=16)
            ax2_plot.set_ylabel("Duration (seconds)", fontsize=12)
            ax2_plot.set_xlabel("Operation Type", fontsize=12)
            ax2_plot.tick_params(axis='x', rotation=45, labelsize=10) # Removed ha
            ax2_plot.tick_params(axis='y', labelsize=10)
            ax2_plot.grid(True, axis='y', linestyle='--', alpha=0.7)
        else:
            ax2_plot.text(0.5, 0.5, "No operations found for distribution plot", ha='center', va='center', fontsize=12)
            ax2_plot.set_title("Operation Duration Distribution", fontsize=16)
    else:
        ax2_plot.text(0.5, 0.5, "No data for duration distribution", ha='center', va='center', fontsize=12)
        ax2_plot.set_title("Operation Duration Distribution", fontsize=16)
    plt.tight_layout()
    filename2 = f"{output_prefix}_duration_dist.png"
    plt.savefig(filename2)
    print(f"Chart '{filename2}' saved.")
    plt.close(fig2)

    # --- Plot 3: Insert Throughput vs. Number of Rows in DB ---
    fig3, ax3_plot = plt.subplots(figsize=(12, 7)) # Slightly wider for line plot legend
    if 'IsInsert' in df_combined.columns:
        df_inserts = df_combined[df_combined['IsInsert']].copy()
        if not df_inserts.empty and 'Duration' in df_inserts.columns and df_inserts['Duration'].notna().all() and (df_inserts['Duration'] > 0).all():
            df_inserts['Throughput'] = rows_per_insert_batch / df_inserts['Duration']
            
            for test_type in sorted(df_inserts['SegmentTestType'].unique()):
                segments_for_type = df_inserts[df_inserts['SegmentTestType'] == test_type]
                for segment_id in sorted(segments_for_type['SegmentID'].unique()):
                    subset = segments_for_type[segments_for_type['SegmentID'] == segment_id]
                    if not subset.empty:
                        subset = subset.sort_values(by='ActualRowsInDB')
                        operation_name = subset['Operation'].iloc[0] 
                        ax3_plot.plot(subset['ActualRowsInDB'], subset['Throughput'], marker='.', linestyle='-', 
                                 label=f'{operation_name} (Run {segment_id+1})')
            if ax3_plot.has_data():
                ax3_plot.legend(loc='best', fontsize=10)
        else:
            ax3_plot.text(0.5, 0.5, "No valid INSERT data for throughput\n(check durations > 0)", ha='center', va='center', fontsize=12)
    else:
         ax3_plot.text(0.5, 0.5, "Column 'IsInsert' not found", ha='center', va='center', fontsize=12)
    ax3_plot.set_title("Insert Throughput vs. Rows in DB", fontsize=16)
    ax3_plot.set_xlabel("Approx. Total Rows in DB", fontsize=12)
    ax3_plot.set_ylabel(f"Insert Throughput (rows/second)", fontsize=12)
    ax3_plot.tick_params(axis='both', labelsize=10)
    ax3_plot.grid(True, alpha=0.7)
    plt.tight_layout()
    filename3 = f"{output_prefix}_insert_throughput.png"
    plt.savefig(filename3)
    print(f"Chart '{filename3}' saved.")
    plt.close(fig3)

    # --- Plot 4: Query Throughput vs. Number of Rows in DB ---
    fig4, ax4_plot = plt.subplots(figsize=(12, 7))
    if 'IsInsert' in df_combined.columns:
        df_queries = df_combined[~df_combined['IsInsert']].copy()
        if not df_queries.empty and 'Duration' in df_queries.columns and df_queries['Duration'].notna().all() and (df_queries['Duration'] > 0).all():
            df_queries['Throughput'] = 1 / df_queries['Duration']

            for test_type in sorted(df_queries['SegmentTestType'].unique()):
                segments_for_type = df_queries[df_queries['SegmentTestType'] == test_type]
                for segment_id in sorted(segments_for_type['SegmentID'].unique()):
                    subset = segments_for_type[segments_for_type['SegmentID'] == segment_id]
                    if not subset.empty:
                        subset = subset.sort_values(by='ActualRowsInDB')
                        operation_name = subset['Operation'].iloc[0] 
                        ax4_plot.plot(subset['ActualRowsInDB'], subset['Throughput'], marker='.', linestyle='-',
                                 label=f'{operation_name} (Run {segment_id+1})')
            if ax4_plot.has_data():
                ax4_plot.legend(loc='best', fontsize=10)
        else:
            ax4_plot.text(0.5, 0.5, "No valid QUERY data for throughput\n(check durations > 0)", ha='center', va='center', fontsize=12)
    else:
        ax4_plot.text(0.5, 0.5, "Column 'IsInsert' not found", ha='center', va='center', fontsize=12)
    ax4_plot.set_title("Query Throughput vs. Rows in DB", fontsize=16)
    ax4_plot.set_xlabel("Approx. Total Rows in DB", fontsize=12)
    ax4_plot.set_ylabel("Query Throughput (queries/second)", fontsize=12)
    ax4_plot.tick_params(axis='both', labelsize=10)
    ax4_plot.grid(True, alpha=0.7)
    plt.tight_layout()
    filename4 = f"{output_prefix}_query_throughput.png"
    plt.savefig(filename4)
    print(f"Chart '{filename4}' saved.")
    plt.close(fig4)

# ------------------------- 主函数 -------------------------
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Database Experiment Controller")
    parser.add_argument('--phase', type=int, required=True, 
                       help="1: Basic Ops, 2: Stress Test (No Index), 3: Stress Test (With Index), 4: Analyze")
    args = parser.parse_args()

    if args.phase == 1:
        basic_operations()
    elif args.phase == 2:
        stress_test(use_index=False)
    elif args.phase == 3:
        stress_test(use_index=True)
    elif args.phase == 4:
        # analyze_results()
        # plot_additional_metrics_from_log(LOG_FILE, ROWS_PER_INSERT_BATCH)
        plot_additional_metrics_from_log_separate(LOG_FILE, ROWS_PER_INSERT_BATCH)
        
    else:
        print("Invalid phase number (1-4)")
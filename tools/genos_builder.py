import os
import re
from collections import defaultdict

def scan_task_manifest(specs_dir):
    """
    扫描 specs 文件夹，将 phase1 和 phase2 文件按层级(L1/L2/L3)配对。
    返回值格式: { 'L1': {'p1': 'path/to/L1_phase1.txt', 'p2': 'path/to/L1_phase2.txt'}, ... }
    """
    if not os.path.exists(specs_dir):
        print(f"Error: Specs directory '{specs_dir}' not found.")
        return {}

    # 使用字典存储层级关系
    # 结构: tasks[level][phase_key] = full_path
    tasks = defaultdict(dict)
    
    # 获取目录下所有 .txt 文件 [cite: 2, 60, 123]
    files = [f for f in os.listdir(specs_dir) if f.endswith('.txt')]
    
    for file_name in files:
        # 匹配文件名格式，例如: L1_phase1.txt
        match = re.match(r'(L\d)_phase(\d)', file_name)
        if match:
            level = match.group(1)   # 例如: L1
            phase = match.group(2)   # 例如: 1 或 2
            full_path = os.path.join(specs_dir, file_name)
            
            # 将路径存入对应的层级和阶段
            tasks[level][f'p{phase}'] = full_path

    # 验证配对完整性
    valid_tasks = {}
    for level, phases in tasks.items():
        if 'p1' in phases and 'p2' in phases:
            valid_tasks[level] = phases
            print(f"[OK] Detected Level {level}: Phase 1 & 2 paired.")
        else:
            print(f"[Warning] Incomplete phases for {level}. Skipping.")

    return valid_tasks

# --- 验证代码 ---
if __name__ == "__main__":
    # 假设当前在 ~/GenOS 目录下运行
    specs_path = "./specs"
    manifest = scan_task_manifest(specs_path)
    print("\nFinal Manifest for LLM Processing:")
    for lvl, paths in manifest.items():
        print(f"{lvl}: {paths}")
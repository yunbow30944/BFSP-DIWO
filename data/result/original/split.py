import pandas as pd
import re

# 读取CSV文件
df = pd.read_csv('BestSeq_ARPD.csv')

# 检查列数
if len(df.columns) < 7:
    print(f"警告：文件只有 {len(df.columns)} 列，将使用所有列进行分组。")
    last_seven_cols = df.columns.tolist()  # 使用所有列
else:
    last_seven_cols = df.columns[-7:].tolist()  # 使用最后7列

print("尝试用于分组的列:", last_seven_cols)

# 检查这些列是否确实存在于DataFrame中
missing_cols = [col for col in last_seven_cols if col not in df.columns]
if missing_cols:
    print(f"错误：以下列不存在于DataFrame中: {missing_cols}")
    exit()

# 根据列分组
for key_values, group in df.groupby(last_seven_cols):
    # 生成唯一哈希值
    unique_hash = abs(hash(key_values)) % (10 ** 10)

    # 清理文件名中的非法字符
    key_str = '_'.join(map(str, key_values))
    safe_key = re.sub(r'[\\/*?:"<>|]', '_', key_str)  # 替换特殊字符
    safe_key = re.sub(r'\s+', '_', safe_key)  # 替换空格
    safe_key = safe_key.strip('_')[:50]  # 截断至50字符

    # 生成文件名
    filename = f'{safe_key}.csv'

    # 保存分组数据
    group.to_csv(filename, index=False)

print(f"分割完成！共生成 {len(df.groupby(last_seven_cols))} 个文件。")
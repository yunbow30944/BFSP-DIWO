import csv
import os
import glob
import pandas as pd

def process_csv_files(input_folder='.', output_folder='cleaned_csv'):
    # 创建输出文件夹
    #os.makedirs(output_folder, exist_ok=True)

    # 遍历输入文件夹中的所有CSV文件
    mean_arpd = []
    for csv_file in glob.glob(os.path.join(input_folder, '*.csv')):
        df = pd.read_csv(csv_file)
        arpd = df.iloc[:,4].mean()
        mean_arpd.append({csv_file:arpd})

    print(mean_arpd)
    with open('ans.txt','w') as f:
        for arpd in mean_arpd:
            f.write(str(arpd))
            print('',file=f)


if __name__ == '__main__':
    # 使用示例（按需修改参数）
    process_csv_files(
        input_folder='.',  # 要处理的CSV文件所在目录
        output_folder='cleaned'  # 处理后的文件保存目录
    )
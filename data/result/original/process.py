import csv
import os
import glob


def process_csv_files(input_folder='.', output_folder='cleaned_csv'):
    # 创建输出文件夹
    os.makedirs(output_folder, exist_ok=True)

    # 遍历输入文件夹中的所有CSV文件
    for csv_file in glob.glob(os.path.join(input_folder, '*.csv')):
        # 构造输出路径
        filename = os.path.basename(csv_file)
        output_path = os.path.join(output_folder, filename)

        # 读取并处理CSV文件
        with open(csv_file, 'r', newline='', encoding='utf-8') as infile, \
                open(output_path, 'w', newline='', encoding='utf-8') as outfile:

            reader = csv.reader(infile)
            writer = csv.writer(outfile)

            # 过滤并写入符合要求的行
            for row in reader:
                if len(row) == 14:
                    writer.writerow(row)


if __name__ == '__main__':
    # 使用示例（按需修改参数）
    process_csv_files(
        input_folder='.',  # 要处理的CSV文件所在目录
        output_folder='cleaned'  # 处理后的文件保存目录
    )
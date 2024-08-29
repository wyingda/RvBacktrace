import os  
import re  
  
# 用户输入目录路径  
directory_input = input("请输入txt文件的目录路径（例如：E:\\RvBacktrace\\tools\\txt）: ")  
directory = os.path.abspath(directory_input)  
  
# 确保路径存在且为目录  
if not os.path.exists(directory) or not os.path.isdir(directory):  
    print("指定的路径不存在或不是一个目录。")  
    exit()  
  
# 栈帧计数器  
frame_number = 0  
  
# 输出文件的完整路径  
output_dir = 'obj'  
if not os.path.exists(output_dir):  
    os.makedirs(output_dir)  # 如果文件夹不存在，则创建它  
output_file_path = os.path.join(output_dir, 'function_pc.txt')  
  
# 尝试打开（或创建）输出文件  
with open(output_file_path, 'w', encoding='utf-8') as output_file:  
    # 遍历目录下的所有.txt文件  
    for filename in os.listdir(directory):  
        if filename.endswith('.txt'):  
            file_path = os.path.join(directory, filename)  
  
            try:  
                with open(file_path, 'r', encoding='utf-8') as file:  
                    for line in file:  
                        # 检查行中是否包含"pc"（不区分大小写）  
                        if 'pc' in line.lower():  
                            # 在"pc"之后搜索以"0x"开头的十六进制数据  
                            hex_match = re.search(r'pc\s*0x[0-9a-fA-F]+', line)  
                            if hex_match:  
                                # 提取十六进制数据部分  
                                hex_data = hex_match.group(0).split(' ', 1)[-1].strip()  
                                # 去除前导空格和非十六进制字符（这里其实已经由上一个正则保证了）  
                                # 但为了清晰，保留这一行，实际上可能不需要再次搜索  
                                # hex_data = re.search(r'\b0x[0-9a-fA-F]+\b', hex_data).group(0)  
                                # 打印栈帧级别和栈帧地址（可选，用于控制台输出）  
                                print(f"第{frame_number + 1}级栈帧，栈帧地址: {hex_data}")  
                                # 将栈帧地址写入输出文件  
                                output_file.write(hex_data + '\n')  
                                # 更新栈帧计数器  
                                frame_number += 1  
  
            except Exception as e:  
                print(f"无法读取文件 {file_path}: {e}")  
  
# 程序执行完毕  
print("所有txt文件中的栈帧地址已写入当前路径下obj文件夹的function_pc.txt文件。")
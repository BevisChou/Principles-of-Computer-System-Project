# Principles-of-Computer-System-Project

## 编译

```bash
# 安装依赖
apt update
apt install -y build-essential cmake libncurses5-dev

# 编译本项目
./build.sh
```

## 运行

```bash
./run.sh
```

## 使用

启动程序后，用户可以输入以下命令使用程序的功能。

1. assemble `input_filename` `output_filename`
2. disassemble `input_filename` `output_filename`
3. simulate assembly `file_name`
4. simulate binary `file_name`
5. convert to int `bits`
6. convert to float `bits`
7. convert int `number`
8. convert float `number`
9. exit

在模拟器模式下，用户可以输入以下指令。

1. s
2. t

## 功能

支持的MIPS指令包括：

- mv
- lw
- sw
- and
- or
- add
- addi
- sub
- sll
- slt
- beq
- bne
- j
- jr
- jal

### 汇编

输入汇编码文件名与输出文件名，将结果写入指定文件。默认文件名与输入汇编码合法。

### 反汇编

输入机器码文件名与输出文件名，将结果写入指定文件，默认文件名与输入机器码合法。

### MIPS模拟器

模拟的系统状态包括：
- 32个寄存器
- 内存

### 数的表示和运算

功能包括：
- 将十进制整数转化为二进制补码
- 将十进制浮点数转化为符合IEEE标准的32位机器码
- 将二进制补码转化为十进制整数
- 将符合IEEE标准的32位浮点数表示转化为十进制浮点数
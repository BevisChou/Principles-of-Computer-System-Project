# Principles-of-Computer-System-Project

## 编译

```bash
# 安装依赖
sudo apt update
sudo apt install -y build-essential cmake libncurses5-dev

# 进入项目根目录编译本项目
./build.sh
```

## 运行

```bash
# 进入项目根目录运行本程序
./run.sh
```

具体使用方法将在后文介绍。

## 功能

### 程序功能

- 汇编

  输入存有汇编代码的绝对路径和期望输出的文件的绝对路径，程序将完成汇编并将结果存在指定的文件中。

- 反汇编

  输入存有二进制码的绝对路径和期望输出的文件的绝对路径，程序将完成反汇编并将结果存在指定的文件中。

- MIPS模拟器

  输入存有需要模拟执行的汇编代码或二进制码的文件的绝对路径，完成模拟，用户可以全程查看32个寄存器以及PC寄存器的状态以及内存的状态。

- 数的表示和运算

  具体功能包括：

  - 将十进制整数转化为二进制补码
  - 将十进制浮点数转化为符合IEEE标准的32位机器码
  - 将二进制补码转化为十进制整数
  - 将符合IEEE标准的32位浮点数表示转化为十进制浮点数

### MIPS指令支持

以上功能支持的MIPS指令共15个。这些指令涵盖了绝大部分常用指令，包括：

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

## 使用方法

程序分常规模式和模拟器模式。启动程序后，程序将进入常规模式，常规模式下支持的命令如下所示。

1. assemble `input_filename` `output_filename`

   完成汇编。

2. disassemble `input_filename` `output_filename`

   完成反汇编。

3. simulate assembly `file_name`

   进入模拟器模式，模拟指定汇编码的执行。

4. simulate binary `file_name`

   进入模拟器模式，模拟指定二进制码的执行。

5. convert to int `bits`

   将32位二进制补码转化为十进制整数并输出。

6. convert to float `bits`

   将32位二进制码转化为十进制浮点数并输出。

7. convert int `number`

   将十进制整数转化为32位二进制补码。

8. convert float `number`

   将十进制浮点转化为32位二进制码。

9. exit

   退出程序。

在模拟器模式下，用户可以输入以下命令。

1. s

   单步执行。

2. t

   退出模拟器模式。

## 注意事项

请确保输入的命令格式正确，并确保需要汇编的汇编码、需要反汇编的二进制码、需要模拟的汇编码或二进制码正确。
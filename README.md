# Principles-of-Computer-System-Project

## 目标

- 提交物
    - 技术报告
        - 需求
        - 设计
        - 实现
    - 源代码包
    - 展示幻灯片
- 其他
    - 展示
        - 幻灯片放映与讲解
        - 程序演示

## 功能

支持的命令如下：

1. assemble `input_filename` `output_filename`
2. disassemble `input_filename` `output_filename`
3. simulate assembly `file_name`
4. simulate binary `file_name`
5. step
6. set `register_number` `number`
7. show registers
8. show memory
9. terminate
10. convert to int `bits`
11. convert to float `bits`
12. convert int `number`
13. convert float `number`
14. exit

支持的指令包括：
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
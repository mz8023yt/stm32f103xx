# 秉火指南者 - 工程模板

## 创建工程步骤

1. 建立好工程文件夹，在 git 命令行敲命令更快：mkdir doc driver library listing output project startup user
2. 使用 keil 新建工程，工程文件保存在 project 目录下
3. 指定工程 target device 为 stm32f103ve
4. 将文件加入工程中，注意 startup_stm32f10x_hd.s 启动代码要必须要加入工程
5. 设置链接和编译输出文件路径
6. 指定头文件查找路径
7. 设置默认开启的宏定义: STM32F10X_HD, USE_STDPERIPH_DRIVER
8. 仅需要添加使用到的库文件进工程
9. 修改 stm32f10x_conf.h 文件，只需要包含使用到的头文件
10. 新增 stm32f10x_main.c 文件，实现主函数
11. 编译工程，工程可以编译通过
12. 设置 DAP/J-LINK 下载配置，需要配置两个地方：Debug、Utilites

# stm32f103c8t6 核心板 - 工程模板

1. 复制一份指南者的工程, 重命名为 stm32f103c8.gpio.led
2. 修改工程 device 为 stm32f103c8
3. 修改宏定义 STM32F10X_HD 为 STM32F10X_MD
4. 更换启动文件 startup_stm32f10x_md.s
5. 配置 DAP 或者 JLINK 下载
6. 修改其中的 bsp_led 驱动文件, 配置 PC13 为输出, 并通过位带抛出 LED 接口

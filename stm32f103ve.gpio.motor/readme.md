# 秉火指南者 - 步进电机控制驱动

## 工程概述
使用 4 个通用的 GPIO 端口分别控制步进电机的 4 路输入信号, 从而控制步进电机的运转.
使用的步进电机如图:
![image](https://github.com/mz8023yt/stm32f103/blob/master/stm32f103ve.gpio.motor/image/step_motor_1.jpg)

![image](https://github.com/mz8023yt/stm32f103/blob/master/stm32f103ve.gpio.motor/image/step_motor_2.jpg)

![image](https://github.com/mz8023yt/stm32f103/blob/master/stm32f103ve.gpio.motor/image/step_motor_3.jpg)

步进电机的使用的 GPIO 引脚对应表
PB7  -- IN1
PA8  -- IN2
PA11 -- IN3
PA12 -- IN4

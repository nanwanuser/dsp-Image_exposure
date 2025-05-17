# DSP-Image_exposure  
**Code Repository for DSP Course Design of Southwest University of Science and Technology**

## 项目简介
本项目为基于TI C6748 DSP处理器的图像处理应用系统，实现了图像二次曝光功能。图像二次曝光将两幅图像按照特定权重（`C=0.5×A+0.5×B`）合成为一幅新图像，展示了DSP图像处理的基本流程和优化方法。

## 功能特性
- 支持BMP格式图像的读取和处理
- 实现图像二次曝光算法：`C=0.5×A+0.5×B`
- 提供三种实现方式：
  - C语言实现
  - Intrinsic指令优化实现
  - 线性汇编优化实现
- 自动测量和比较不同实现方式的性能（时钟周期数）

## 系统要求
- 开发环境：Code Composer Studio 5.0
- 目标硬件：TI C6748 DSP处理器  
- 输入图像：两幅相同尺寸的BMP格式图像

## 项目结构
```
Image_exposure/
├── ImageDoubleExposure.h      # 算法接口头文件
├── ImageDoubleExposure.c      # 算法接口实现
├── DoubleExposureCore.h       # 核心算法头文件
├── DoubleExposureCore.c       # 核心算法实现（C语言和Intrinsic优化）
├── process_pixels_dblexp_asm.sa # 线性汇编优化实现
├── RGB2RGB.h                  # RGB格式转换头文件
├── RGB2RGB.c                  # RGB格式转换实现
├── bmp_head_struct.h          # BMP文件格式定义
├── main.c                     # 主程序
└── C6748.cmd                  # 链接器命令文件
```

## 优化方法比较
| 实现方式 | 特点 | 性能提升 |
|----------|------|----------|
| C语言实现 | 基础实现，逐像素处理，无特殊优化 | 基准 |
| Intrinsic指令优化 | 使用C6x DSP内联指令，如`_amem4`进行内存访问优化，每次处理4个像素 | 1-3倍 |
| 线性汇编优化 | 使用线性汇编，通过SIMD指令（如`AVGU4`）进行并行处理，每次处理8个像素 | 3-5倍 |

## 运行方法
1. 在Code Composer Studio 5.0中创建新项目
2. 将所有源文件添加到项目中
3. 在项目属性中设置目标为TI C6748 DSP
4. 在bmps文件夹中放置两幅名为`ImageA.bmp`和`ImageB.bmp`的测试图像
5. 编译并运行程序
6. 查看控制台输出的各种实现方法的性能对比
7. 在bmps文件夹中查看生成的结果图像（`Result_Mode0.bmp`、`Result_Mode1.bmp`和`Result_Mode2.bmp`）

## 性能分析
程序会自动测量各种实现方式的处理时间（以时钟周期为单位），并在控制台输出。一般来说，性能提升效果如下：
- Intrinsic指令优化相比C语言实现约提升1-3倍性能
- 线性汇编优化相比C语言实现约提升3-5倍性能

具体性能提升效果取决于图像大小和处理器配置。

## 注意事项
- 确保输入的两幅图像大小相同
- 确保有足够的内存用于图像处理
- 在修改代码时注意DSP架构的特点和限制
- 测试时尽量使用不同大小和内容的图像以验证算法的稳定性

## 课程设计要求完成情况
- 使用了DSP优化技术：Intrinsic和线性汇编
- 给出了优化前后的时钟占用对比
- 实现了图像二次曝光：`C=0.5×A+0.5×B`
- 使用三种不同方式（C语言、Intrinsic、线性汇编）进行了实现

# 多版本并发控制（MVCC）实现
## 简介

MVCC（多版本并发控制，Multi-Version Concurrency Control）是一种用于数据库管理系统的并发控制方法。它允许多个事务在不阻塞的情况下同时读取和写入数据，从而提高了数据库的并发性能和吞吐量。本项目实现了一个简单的MVCC系统，以便在学习和研究中使用。
## 特性

    支持多版本数据存储
    提供读未提交、读已提交、可重复读和可串行化的隔离级别
    支持事务的开始、提交和回滚
    提供简单易用的API

## 目录结构
```
.
├── Makefile
├── include
│   ├── database.h
│   ├── row.h
│   └── transaction.h
└── main.cpp
```
## 安装与使用
### 前提条件

    C++11 编译器
    CMake 或 GNU Make

### 编译

使用Makefile进行编译：

`make`

### 运行示例

编译完成后，运行生成的可执行文件：

`./main`

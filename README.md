# 人脸考勤检测

## 一.开发环境

1. 开发软件：Qt5.14.2
2. 编译器：msvc2017_64
3. 数据库：mysql 8.0

## 二.文件构成

1. model:模型文件
2. new_client: 客户端
3. new_manager: 管理员端
4. opencv: 4.8.0的opencv库，已使用vs2019编译，只能在msvc编译器下运行，带CUDA和扩展模块的版本
5. server:服务器，可以在linux上使用./server运行，如果不行在linux安装Qt或者用windows上的Qt重新编译成可执行文件。连接时把客户端和管理员端的network.cpp中的ip和端口改成服务器的ip和端口
6. sql:数据库文件

# 文件说明

```c++
src
|
 ---main.cpp
 ---berth.h
 ---berth.cpp
 ---robot.h
 ---robot.cpp
 ---ship.h
 ---ship.cpp
 ---map.h
 ---map.cpp
```

# ROBOT

​	当机器人处于未携带货物的状态的时候。第一次扫描整体地图（刚买机器人的时候和机器人在泊位上交货的时候或者锁定的货物消失），然后确定最优的货物路径时锁定该货物。后续在该机器人附近一定曼哈顿距离内扫描，搜寻更优的货物。

​	当机器人处于携带货物的状态的时候，返回开放的最近的泊位。

​	当机器人之间要发生碰撞的时候，优先携带货物的机器人，要碰撞的机器人触发避障系统，向附近不会发生碰撞的最近的躲避点移动。如果在狭路上，也就是没有躲避点的是时候，返回。



## variable

+ Robot: struct
  + x, y: 坐标
  + goods: 是否携带货物
  + goods_x, goods_y: 要去取货的货物坐标x和y
  + berth: 机器人要去的泊位标号
  + pre_berth: 机器人从哪个泊位出发的
  + dis: 当前机器人行走的距离
  + goods_value: 机器人携带的货物价值
  + robot_find_goods_move: 到货物的移动路径

## func

### read_robot()

读取机器人状态

### check(int x, int y)

路径搜索时候的边界检测

### int_pad2str

### int2str

### bfs_find_goods

搜寻货物

​        在第一次搜索的时候根据当前位置计算出最合适的货物位置，在后续每一帧的计算中，计算机器人附近范围内的货物，有更优的取更优

### robot

机器人控制

### collision_judge

碰撞检测

### robot_buy

根据场上的货物以及港口的货物剩余判断是否需要再购买机器人

+ 当场上没有机器人的时候 -> buy

### control_robot

控制机器人，robot系统的主函数

# BERTH

## variable

## func



# SHIP



# MAP

## 预处理

+ 计算地图上所有点到泊位的最短路径
+ 计算地图上所有泊位到交货点的最短路径



## variable

+ grid: 原始的地图，不可修改
+ grid_copy: 可修改的地图，更新货物信息，用于搜索
+ goods: 存储货物信息
+ goods_value_mp: map, 用于映射货物坐标和价值
+ goods_time_mp: map, 用于映射货物坐标和货物出现时间
+ Goods: struct
  + x, y: 坐标
  + value: 货物价值
+ grid_berth_id: 地图上泊位坐标
+ PATH_POINT: struct -> 记录路径
  - x, y: 坐标
  - move: 如何移动



## func

### read_map()

读取map信息，用于init阶段

### Init_shotpath_berth

计算地图上所有点到泊位的最短路径

### Init_shotpath_delivery



### Init_grid_berth_id

确定地图上泊位对应的id



# 运行

```shell
g++ main.cpp ship.cpp robot.cpp map.cpp berth.cpp -o main
```

```shell
.\SemiFinalJudge.exe -m maps\map1.txt .\src\main.exe
```


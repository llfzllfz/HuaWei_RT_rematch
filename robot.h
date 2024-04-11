#ifndef ROBOT_H
#define ROBOT_H
#include <bits/stdc++.h>
#include "map.h"
using namespace std;

extern const int N;
extern int berth_num;
extern int robot_num;

extern char grid[][200];
extern char grid_copy[][200];



// 机器人
struct ROBOT
{
    int id, x, y, goods; // x, y 坐标，goods 是否携带货物
    int goods_x, goods_y; // 锁定货物的坐标
    int status;  // 机器人状态
    int berth=-1; // 当前机器人负责的港口
    int dis; // 距离
    int goods_values; //携带的货物价值
    int is_first=1;
    stack<int> collision_move;
    // stack<Point> best_goods; //最优的十个货物选择
    stack<int> robot_find_goods_move;
    vector<POINT> best_goods;
    POINT pre_best_goods = POINT(-1,-1);
    int visit[200][200];
    PATH_POINT path[200][200];
    ROBOT() {}
    ROBOT(int startX, int startY) {
        x = startX;
        y = startY;
    }
};

// 机器人移动
struct ROBOT_MOVE{
    int x, y, move;
    int change = 0;
    ROBOT_MOVE() {}
    ROBOT_MOVE(int x1, int y1, int move1){
        x = x1;
        y = y1;
        move = move1;
    }
};

extern ROBOT robot[50];
extern int robot_price;

// 读取机器人状态
void read_robot();

// 边界检测
int check(int x, int y);

// int转string
string int_pad2str(int x);
string int2str(int x, int y);

// 寻路系统-寻找货物路径
void Init_robot(ROBOT &x, int is_first);
void bfs_find_goods(int robot_index, int zhen, double pow_index, int region);
int sort_best_goods(POINT x1, POINT x2);
double xjb_func(POINT x1);

// 碰撞系统检测
// vector<ROBOT_MOVE> collision_judge(vector<ROBOT_MOVE> v);

// 判断是否要购买机器人
int robot_buy();

// 控制机器人
void control_robot(int zhen);

void Init_containers();

int select2best_point(POINT x1, POINT x2, double pow_index);



vector<ROBOT_MOVE> collision_judge(vector<ROBOT_MOVE> v);
// int collision_move_opposite(ROBOT x, ROBOT y, int y_move);

// // 机器人路径清空


// // 计算初始时候的所有点到港口的距离
// void cal_dis(int x, int y);
// void Init_map_dis();


// // 寻路系统-寻找港口路径
// void bfs_find_berth(int x, int y, int ux, int uy, int robot_index, int zhen);




// int check_berth(int x, int y);
// int find_nearst_berth(int x, int y, int robot_index);
// int select2best_point(Point x1, Point x2, double pow_index);

// int find_pzset_move(vector<int> pz_set, vector<string> pz_v, int x, int y, int ux, int uy);


#endif
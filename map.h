#ifndef MAP_H
#define MAP_H
#include <bits/stdc++.h>
using namespace std;


struct Goods{
    int x, y;
    int value;
    int time;
    Goods(){}
    Goods(int startx, int starty){
        x = startx;
        y = starty;
    }
    Goods(int startx, int starty, int startvalue, int starttime){
        x = startx;
        y = starty;
        value = startvalue;
        time = starttime;
    }
};

struct Map_Point{
    int x, y;
    Map_Point() {}
    Map_Point(int _, int __){
        x = _;
        y = __;
    }
};

struct PATH_POINT{
    int x, y, move;
    PATH_POINT() {}
    PATH_POINT(int _, int __, int ___){
        x = _;
        y = __;
        move = ___;
    }
};

struct SHOTPATH_BERTH{
    int path[200][200];
};

struct SHOTPATH_DELIVERY{
    int path[200][200];
};

// 地图上的货物点
struct POINT{
    int x, y, dis, value;
    int goods2berth_dis = 10000;
    int pre_dis;
    int berth = -1;
    POINT() {}
    POINT(int startX, int startY) {
        x = startX;
        y = startY;
    }
    POINT(int startX, int startY, int startvalue, int startdis){
        x = startX;
        y = startY;
        value = startvalue;
        dis = startdis;
    }
};

struct SEA_POINT{
    int x, y, direction; // 核心点位置和当前的航向， 0表示向右，1表示向下，2表示向左，3表示向上
    int dis; // 船移动的步骤
    int pre_move = -1;
    int now_move = -1;
    SEA_POINT() {}
    SEA_POINT(int _, int __, int ___, int ____, int _____){
        x = _;
        y = __;
        dis = ___;
        direction = ____;
        pre_move = _____;
    }
};


extern const int N;
extern char grid[200][200];
extern char grid_copy[200][200];
extern char grid_goods[200][200];
extern int grid_copy_berth_id[200][200];
// extern vector<SHOTPATH_BERTH> shotpath_berth;
// extern vector<SHOTPATH_DELIVERY> shotpath_delivery;
extern int shotpath_berth[10][200][200];
extern int shotpath_berth_dis[10][200][200];
extern int shotpath_delivery[10][200][200];
extern int shotpath_delivery_dis[10][200][200];
extern vector<Goods> goods;
extern unordered_map<string, int> goods_value_mp;
extern unordered_map<string, int> goods_time_mp;
extern vector<pair<int, int>> robot_purchase_point;
extern vector<pair<int, int>> boat_purchase_point;
extern vector<pair<int, int>> delivery_point;





extern int grid_berth_id[200][200];
extern int robot_move_x[4];
extern int robot_move_y[4];
extern int ship_move_x[3];
extern int ship_move_y[3];


void read_map();
void Init_shotpath_berth();
void Init_shotpath_delivery();
void ProcessMap();
int get_move_reverse(int move);
int check_path2delivery(int x, int y, int direction);
void Init_berth_id();


extern char grid_goods[200][200];

extern int goods_density[200][200];
extern int goods_vector_index;




#endif
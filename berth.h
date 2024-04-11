#ifndef BERTH_H
#define BERTH_H
#include <bits/stdc++.h>
using namespace std;



extern int berth_num;
extern vector<int> transport_berth;

struct Berth
{
    int x, y; // 当前坐标
    int loading_speed; //装载速度
    int goods_num; //港口货物量
    int efficiency; //单词满载运输的效率
    int id; // 港口编号
    int goods_values;
    double goods_efficiency;
    int all_goods_num;
    int sub_goods_num;
    int is_transport;
    int goods_num_time[16000];
    bool idle=true;
    Berth(){}
    Berth(int x, int y, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> loading_speed = loading_speed;
    }
};

extern Berth berth[10 + 10];
extern vector<int> robot_berth;

void read_berth();
void find_transport_berth();
void Update_berth_goods_efficiency(int zhen);
vector<int> Occupid_berth();
vector<int> Occupid_berth(vector<int> v);
int select_suitable_berth();
int check_move_or_stop(int boat_id, int zhen);
int check_is_last(int boat_id, int zhen);
int select_suitable_berth(int zhen);
// void cal_betch(int zhen, int tz, int boat_capacity);

#endif
#include <bits/stdc++.h>
#include "robot.h"
#include "map.h"
#include "berth.h"
#include "ship.h"
using namespace std;
const int N = 200;

int robot_num = 0;
int boat_num = 0;
int berth_num = 0;

int goods_num = 0;
int frame_id = 0;
int money;

void Init()
{
    read_map();
    ProcessMap();
    read_berth();
    Init_shotpath_berth();
    Init_berth_id();
    // Init_shotpath_delivery();
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}


void Input()
{
    scanf("%d", &money);
    scanf("%d", &goods_num);
    for(int i = 0; i < goods_num; i ++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        if(val == 0) {
            grid_copy[x][y] = grid[x][y];
            grid_goods[x][y] = grid[x][y];
            goods_value_mp[int2str(x, y)] = 0;
        }
        else{
            goods.push_back(Goods(x, y, val, frame_id));
            grid_copy[x][y] = 'g';
            grid_goods[x][y] = 'g';
            goods_value_mp[int2str(x, y)] = val;
            goods_time_mp[int2str(x, y)] = frame_id;
        }
    }
    read_robot();
    read_boat();
    char okk[100];
    scanf("%s", okk);
}

// -----------------------------------------------start----------------------------------------
int main()
{
    Init();
    while(scanf("%d", &frame_id) != EOF)
    {
        // cerr << frame_id << endl;
        Input();
        if(money >= robot_price && robot_num < 20){
            printf("lbot %d %d\n", robot_purchase_point[0].first, robot_purchase_point[0].second);
            Init_containers();
            // money = money - 2000;
        }

        if(money >= boat_price && boat_num < 5){
            printf("lboat %d %d\n", boat_purchase_point[0].first, boat_purchase_point[0].second);
            auto it = find(robot_berth.begin(), robot_berth.end(), boat_num);
            if(it == robot_berth.end()) robot_berth.push_back(boat_num);
            // if(robot_berth.size() < berth_num) robot_berth.push_back(boat_num);
        }
        cerr << "Robot" << endl;
        control_robot(frame_id);
        cerr << "Ship" << endl;
        control_ship(frame_id);
        cerr << 0 << ' ' << robot[0].best_goods.size() << endl;
        for(int i = 0; i < boat_num; i++){
            cerr << boat[i].goods_num << ' ';
        }
        cerr << endl;
        for(int i = 0; i < robot_berth.size(); i++){
            cerr << robot_berth[i] << ' ';
        }
        cerr << endl;

        // for(int i = 0; i < robot_num; i ++)
        //     printf("move %d %d\n", i, rand() % 4);

        // for(int i = 0; i < boat_num; i ++){
        //     int status = abs(rand()) % 2;
        //     if(status == 1)
        //         printf("ship %d\n", i);
        //     else
        //         printf("rot %d %d\n", i, rand() % 2);
        // }
        puts("OK");
        fflush(stdout);
    }
    return 0;
}
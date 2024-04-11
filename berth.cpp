#include "berth.h"
#include "ship.h"
#include <bits/stdc++.h>
using namespace std;


Berth berth[10 + 10];
vector<int> robot_berth;
vector<int> transport_berth;

void read_berth(){
	scanf("%d", &berth_num);
    for(int i = 0; i < berth_num; i ++)
    {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d", &berth[id].x, &berth[id].y, &berth[id].loading_speed);
        robot_berth.push_back(i);
    }
}

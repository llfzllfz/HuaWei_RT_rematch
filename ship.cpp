#include "ship.h"
#include "berth.h"
#include <bits/stdc++.h>
using namespace std;

Boat boat[10];
int boat_capacity;
int boat_price=8000;

void read_boat(){
	scanf("%d", &boat_num);
    for(int i = 0; i < boat_num; i ++)
        scanf("%d%d%d%d%d%d\n", &boat[i].id, &boat[i].goods_num, &boat[i].x, &boat[i].y, &boat[i].dir, &boat[i].status);
}

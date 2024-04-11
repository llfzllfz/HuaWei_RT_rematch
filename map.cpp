#include "map.h"
#include "berth.h"
#include <bits/stdc++.h>

char grid[200][200];
char grid_copy[200][200];
vector<Goods> goods;
unordered_map<string, int> goods_value_mp;
unordered_map<string, int> goods_time_mp;
// vector<SHOTPATH_BERTH> shotpath_berth;
// vector<SHOTPATH_DELIVERY> shotpath_delivery;
int shotpath_berth[10][200][200];
int shotpath_berth_dis[10][200][200];
int grid_berth_id[200][200];
int robot_move_x[] = {0, 0, -1, 1};
int robot_move_y[] = {1, -1, 0, 0};



int visits[200][200];


char grid_goods[200][200];
int goods_density[200][200];
int goods_vector_index = 0;


vector<pair<int, int>> robot_purchase_point;
vector<pair<int, int>> boat_purchase_point;
vector<pair<int, int>> delivery_point;
void ProcessMap()
{
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            if(grid[i][j] == 'R')
                robot_purchase_point.push_back(make_pair(i, j));
            else if(grid[i][j] == 'S')
                boat_purchase_point.push_back(make_pair(i, j));
            else if(grid[i][j] == 'T')
                delivery_point.push_back(make_pair(i, j));
        }
    }
}


void read_map(){
	for(int i = 0; i < N; i ++){
        scanf("%s", grid[i]);
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            grid_copy[i][j] = grid[i][j];
        }
    }
}

void Init_grid_berth_id(){
	memset(grid_berth_id, -1, sizeof(grid_berth_id));
	int berth_id = 0;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if(grid[i][j] == 'B'){
				if((i-1 >= 0 && grid_berth_id[i-1][j] > -1) || (j-1 >= 0 && grid_berth_id[i][j-1] > -1)){
					grid_berth_id[i][j] = max(grid[i-1][j], grid[i][j-1]);
				}
				else{
					grid_berth_id[i][j] = berth_id;
					berth_id++;
				}
			}
		}
	}
}

int check_path2berth(int x, int y){
    if(x < 0 || x >= N) return -1;
    if(y < 0 || y >= N) return -1;
    char tmp = grid_copy[x][y];
    if(tmp == '.' || tmp == '>' || tmp == 'R' || tmp == 'B' || tmp == 'C' || tmp == 'c' || tmp == 'g') return 1;
    return -1;
}

int get_move_reverse(int move){
	switch(move){
		case 0: {return 1;}
		case 1: {return 0;}
		case 2: {return 3;}
		case 3: {return 2;}
		default: {break;}
	}
	return -1;
}

void Init_shotpath_berth_cal_path(int x, int y, int berth_id){
	memset(visits, 0, sizeof(visits));
	queue<POINT> q;
	q.push(POINT(x, y, 0, 0));
	visits[x][y] = 1;
	while(!q.empty()){
		POINT now = q.front();
		q.pop();
		for(int i = 0; i < 4; i++){
			int next_x = now.x + robot_move_x[i];
            int next_y = now.y + robot_move_y[i];
            if(check_path2berth(next_x, next_y) == -1) continue;
            if(visits[next_x][next_y] == 1) continue;
            POINT next = POINT(next_x, next_y);
            next.dis = now.dis + 1;
            q.push(next);
            visits[next.x][next.y] = 1;
            shotpath_berth_dis[berth_id][next.x][next.y] = next.dis;
            shotpath_berth[berth_id][next.x][next.y] = get_move_reverse(i);
		}
	}
}

void Init_shotpath_berth(){
	memset(shotpath_berth_dis, -1, sizeof(shotpath_berth_dis));
	memset(shotpath_berth, -1, sizeof(shotpath_berth));
	for(int i = 0; i < berth_num; i++){
		Init_shotpath_berth_cal_path(berth[i].x, berth[i].y, i);
	}
}

// void Init_shotpath_delivery(){

// }

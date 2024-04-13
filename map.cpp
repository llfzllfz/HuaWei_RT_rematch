#include "map.h"
#include "berth.h"
#include <bits/stdc++.h>

char grid[200][200];
char grid_copy[200][200];
int grid_copy_berth_id[200][200];
vector<Goods> goods;
unordered_map<string, int> goods_value_mp;
unordered_map<string, int> goods_time_mp;
// vector<SHOTPATH_BERTH> shotpath_berth;
// vector<SHOTPATH_DELIVERY> shotpath_delivery;
int shotpath_berth[10][200][200];
int shotpath_berth_dis[10][200][200];
int shotpath_delivery[10][200][200];
int shotpath_delivery_dis[10][200][200];
int grid_berth_id[200][200];
int robot_move_x[] = {0, 0, -1, 1};
int robot_move_y[] = {1, -1, 0, 0};
int ship_move_x[] = {0, 1, 0};
int ship_move_y[] = {2, 1, 0};



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

int check_path2delivery_point(int x, int y){
	if(x < 0 || x >= N) return -1;
    if(y < 0 || y >= N) return -1;
    char tmp = grid_copy[x][y];
    // cerr << tmp << endl;
    // cerr << x << ' ' << y << ' ' << tmp << endl;
    if(tmp == '*' || tmp == '~' || tmp == 'S' || tmp == 'B' || tmp == 'C' || tmp == 'c' || tmp == 'T' || tmp == 'K') return 1;
    return -1;
}

int check_path2delivery(int x, int y, int direction){
	if(direction == 0){
		for(int i = x; i <= x+1; i++){
			for(int j = y; j <= y+2; j++){
				if(check_path2delivery_point(i, j) == -1) return -1;
			}
		}
	}
	else if(direction == 1){
		for(int i = x - 1; i <= x; i++){
			for(int j = y-2; j <= y; j++){
				if(check_path2delivery_point(i, j) == -1) return -1;
			}
		}
	}
	else if(direction == 2){
		for(int i = x-2; i <= x; i++){
			for(int j = y; j <= y+1; j++){
				// cerr << check_path2delivery_point(i, j) << endl;
				if(check_path2delivery_point(i, j) == -1) return -1;
			}
		}
	}
	else if(direction == 3){
		for(int i = x; i <= x + 2; i++){
			for(int j = y - 1; j <= y; j++){
				if(check_path2delivery_point(i, j) == -1) return -1;
			}
		}
	}
	return 1;
}

// 根据核心点的坐标和航向判断是否有船体进入主航道
int check_main_channel(int x, int y, int direction){
	if(direction == 0){
		for(int i = x; i <= x+1; i++){
			for(int j = y; j <= y+2; j++){
				if(grid_copy[i][j] == '~' || grid_copy[i][j] == 'B' || grid_copy[i][j] == 'K') return 1;
			}
		}
	}
	else if(direction == 1){
		for(int i = x; i <= x+2; i++){
			for(int j = y-1; j <= y; j++){
				if(grid_copy[i][j] == '~' || grid_copy[i][j] == 'B' || grid_copy[i][j] == 'K') return 1;
			}
		}
	}
	else if(direction == 2){
		for(int i = x-2; i <= x; i++){
			for(int j = y-1; j <= y; j++){
				if(grid_copy[i][j] == '~' || grid_copy[i][j] == 'B' || grid_copy[i][j] == 'K') return 1;
			}
		}
	}
	else{
		for(int i = x-2; i <= x; i++){
			for(int j = y; j <= y+1; j++){
				if(grid_copy[i][j] == '~' || grid_copy[i][j] == 'B' || grid_copy[i][j] == 'K') return 1;
			}
		}
	}
	return 0;
}

int change_move2direction(int move){
	if(move == 0) return 0;
	if(move == 1) return 2;
	if(move == 2) return 3;
	if(move == 3) return 1;
	return -1;
}

void Init_shotpath_delivery_cal_path(int x, int y, int delivery_id){
	memset(visits, 0, sizeof(visits));
	queue<SEA_POINT> q;
	q.push(SEA_POINT(x, y, 0, 0, -1));
	visits[x][y] = 1;
	while(!q.empty()){
		SEA_POINT now = q.front();
		q.pop();
		if(visits[now.x][now.y] == 1 && now.dis < shotpath_delivery_dis[delivery_id][now.x][now.y]){
			shotpath_delivery_dis[delivery_id][now.x][now.y] = now.dis;
			shotpath_delivery[delivery_id][now.x][now.y] = get_move_reverse(now.pre_move);
		}
		else if(visits[now.x][now.y] == 1) continue;
		for(int i = 0; i < 4; i++){
			if(now.pre_move != -1 && get_move_reverse(now.pre_move) == i) continue;
			int next_x = now.x + robot_move_x[i];
			int next_y = now.y + robot_move_y[i];
			if(check_path2delivery(next_x, next_y, change_move2direction(i)) == -1) continue;
			SEA_POINT next = SEA_POINT(next_x, next_y, now.dis + 1, change_move2direction(i) ,i);
			if(next.pre_move != i) next.dis = next.dis + 1;
			if(check_main_channel(next_x, next_y, change_move2direction(i)) == 1) next.dis = next.dis + 1;
			if(visits[next_x][next_y] == 0){
				shotpath_delivery_dis[delivery_id][next_x][next_y] = next.dis;
				shotpath_delivery[delivery_id][next_x][next_y] = get_move_reverse(i);
			}
			visits[next_x][next_y] = 1;
		}
	}
}

void Init_shotpath_delivery(){
	memset(shotpath_delivery_dis, -1, sizeof(shotpath_delivery_dis));
	memset(shotpath_delivery, -1, sizeof(shotpath_delivery));
	for(int i = 0; i < delivery_point.size(); i++){
		Init_shotpath_delivery_cal_path(delivery_point[i].first, delivery_point[i].second, i);
	}
}

int check_berth_id_point(int x, int y){
	if(x < 0 || x >= N) return -1;
    if(y < 0 || y >= N) return -1;
    char tmp = grid_copy[x][y];
    if(tmp == 'B' || tmp == 'K') return 1;
    return -1;
}

void bfs_berth_id(int x, int y, int berth_id){
	memset(visits, 0, sizeof(visits));
	queue<POINT> q;
	q.push(POINT(x, y));
	visits[x][y] = 1;
	grid_copy_berth_id[x][y] = berth_id;
	while(!q.empty()){
		POINT now = q.front();
		q.pop();
		for(int i = 0; i < 4; i++){
			int next_x = now.x + robot_move_x[i];
			int next_y = now.y + robot_move_y[i];
			if(check_berth_id_point(next_x, next_y) == -1) continue;
			if(visits[next_x][next_y] == 1) continue;
			grid_copy_berth_id[next_x][next_y] = berth_id;
			q.push(POINT(next_x, next_y));
			visits[next_x][next_y] = 1;
		}
	}
}

void Init_berth_id(){
	memset(grid_copy_berth_id, -1, sizeof(grid_copy_berth_id));
	for(int i = 0; i < berth_num; i++){
		bfs_berth_id(berth[i].x, berth[i].y, i);
	}
}
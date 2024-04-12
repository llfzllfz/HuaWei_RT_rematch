#include "ship.h"
#include "berth.h"
#include "map.h"
#include <bits/stdc++.h>
using namespace std;

BOAT boat[30];
int boat_capacity;
int boat_price=8000;
int visit_ship[200][200];

void read_boat(){
	scanf("%d", &boat_num);
    for(int i = 0; i < boat_num; i ++)
        scanf("%d%d%d%d%d%d\n", &boat[i].id, &boat[i].goods_num, &boat[i].x, &boat[i].y, &boat[i].direction, &boat[i].status);
}

int direction2direction_real(int direction){
	if(direction == 0) return 0;
	if(direction == 1) return 3;
	if(direction == 2) return 1;
	if(direction == 3) return 2;
	return -1;
}

int direction2direction(int direction){
	if(direction == 0) return 0;
	if(direction == 1) return 2;
	if(direction == 2) return 3;
	if(direction == 3) return 1;
	return -1;
}

void collision_point_visit(int x, int y, int direction, int visit_){
	if(direction == 0){
		for(int i = x; i <= x+1; i++){
			for(int j = y; j <= y+2; j++){
				visit_ship[i][j] = visit_;
			}
		}
	}
	else if(direction == 1){
		for(int i = x; i <= x+2; i++){
			for(int j = y-1; j <= y; j++){
				visit_ship[i][j] = visit_;
			}
		}
	}
	else if(direction == 2){
		for(int i = x-2; i <= x; i++){
			for(int j = y-1; j <= y; j++){
				visit_ship[i][j] = visit_;
			}
		}
	}
	else{
		for(int i = x-2; i <= x; i++){
			for(int j = y; j <= y+1; j++){
				visit_ship[i][j] = visit_;
			}
		}
	}
}

// 检测是不是所有点都在主航道，如果有一个点不在主航道，则输出1
int collision_point_main_channel(int x, int y, int direction){
	if(direction == 0){
		for(int i = x; i <= x+1; i++){
			for(int j = y; j <= y+2; j++){
				if(grid_copy[i][j] != '~' && grid_copy[i][j] != 'B' && grid_copy[i][j] != 'K') return 1;
			}
		}
	}
	else if(direction == 1){
		for(int i = x; i <= x+2; i++){
			for(int j = y-1; j <= y; j++){
				if(grid_copy[i][j] != '~' && grid_copy[i][j] != 'B' && grid_copy[i][j] != 'K') return 1;
			}
		}
	}
	else if(direction == 2){
		for(int i = x-2; i <= x; i++){
			for(int j = y-1; j <= y; j++){
				if(grid_copy[i][j] != '~' && grid_copy[i][j] != 'B' && grid_copy[i][j] != 'K') return 1;
			}
		}
	}
	else{
		for(int i = x-2; i <= x; i++){
			for(int j = y; j <= y+1; j++){
				if(grid_copy[i][j] != '~' && grid_copy[i][j] != 'B' && grid_copy[i][j] != 'K') return 1;
			}
		}
	}
	return 0;
}

// 判断所有点是否发生碰撞
int collision_point_(int x, int y, int direction){
	if(direction == 0){
		for(int i = x; i <= x+1; i++){
			for(int j = y; j <= y+2; j++){
				if(visit_ship[i][j] == 1) return 1;
			}
		}
	}
	else if(direction == 1){
		for(int i = x; i <= x+2; i++){
			for(int j = y-1; j <= y; j++){
				if(visit_ship[i][j] == 1) return 1;
			}
		}
	}
	else if(direction == 2){
		for(int i = x-2; i <= x; i++){
			for(int j = y-1; j <= y; j++){
				if(visit_ship[i][j] == 1) return 1;
			}
		}
	}
	else{
		for(int i = x-2; i <= x; i++){
			for(int j = y; j <= y+1; j++){
				if(visit_ship[i][j] == 1) return 1;
			}
		}
	}
	return 0;
}

vector<BOAT_MOVE> boat_collision_judge(vector<BOAT_MOVE> v){
	memset(visit_ship, 0, sizeof(visit_ship));
	for(int i = 0; i < boat_num; i++){
		collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 1);
	}
	// 前往交货点的船舶优先
	for(int i = 0; i < boat_num; i++){
		if(boat[i].status != 0) continue;
		
		// 前往交货点的船舶优先
		if(grid_copy[boat[i].target_x][boat[i].target_y] != 'T') continue;
		int move = v[i].move;
		int next_x = boat[i].x;
		int next_y = boat[i].y;
		if(move == -1) continue;
		collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 0);
		// 如果当前船舶所有点和船舶移动后所有点都在主航道，则跳过碰撞检测
		if(collision_point_main_channel(boat[i].x, boat[i].y, boat[i].direction) == 0){
			change_point(next_x, next_y, boat[i].direction, move);
			if(collision_point_main_channel(next_x, next_y, get_rot_direction(boat[i].direction, move)) == 0){
				collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 0);
				collision_point_visit(next_x, next_y, get_rot_direction(boat[i].direction, move), 0);
			}
		}
		next_x = boat[i].x;
		next_y = boat[i].y;
		change_point(next_x, next_y, boat[i].direction, move);
		// 计算船舶移动后所有点是否有碰撞，没有就确认当前移动方式，有碰撞则转移角度。（顺时针旋转，移动，逆时针旋转）如果所有方式均会发生碰撞，则停帧
		if(collision_point_(next_x, next_y, get_rot_direction(boat[i].direction, move)) == 0){
			// 无碰撞
			collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 0);
			collision_point_visit(next_x, next_y, get_rot_direction(boat[i].direction, move), 1);
		}
		else{
			// 发生了碰撞
			// 当前ship换方式
			for(int j = 0; j < 3; j++){
				if(j == v[i].move) continue;
				next_x = boat[i].x;
				next_y = boat[i].y;
				change_point(next_x, next_y, boat[i].direction, j);
				if(collision_point_(next_x, next_y, get_rot_direction(boat[i].direction, j)) == 0){
					if(check_path2delivery(next_x, next_y, get_rot_direction(boat[i].direction, j)) == 1){
						move = j;
						break;
					}
				}
			}
			// 如果三种都不行，那就保持不动
			if(move == v[i].move)
			{
				v[i].move = -1;
				collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 1);
			}
			else{
				v[i].move = move;
				collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 0);
				collision_point_visit(next_x, next_y, get_rot_direction(boat[i].direction, move), 1);
			}
		}
	}
	// 判断剩余的船舶
	for(int i = 0; i < boat_num; i++){
		if(boat[i].status == 0) continue;
		if(grid_copy[boat[i].target_x][boat[i].target_y] == 'T') continue;
		int move = v[i].move;
		int next_x = boat[i].x;
		int next_y = boat[i].y;
		if(move == -1) continue;
		collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 0);
		// 如果当前船舶所有点和船舶移动后所有点都在主航道，则跳过碰撞检测
		if(collision_point_main_channel(boat[i].x, boat[i].y, boat[i].direction) == 0){
			change_point(next_x, next_y, boat[i].direction, move);
			if(collision_point_main_channel(next_x, next_y, get_rot_direction(boat[i].direction, move)) == 0){
				collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 0);
				collision_point_visit(next_x, next_y, get_rot_direction(boat[i].direction, move), 0);
			}
		}
		next_x = boat[i].x;
		next_y = boat[i].y;
		change_point(next_x, next_y, boat[i].direction, move);
		// 计算船舶移动后所有点是否有碰撞，没有就确认当前移动方式，有碰撞则转移角度。（顺时针旋转，移动，逆时针旋转）如果所有方式均会发生碰撞，则停帧
		if(collision_point_(next_x, next_y, get_rot_direction(boat[i].direction, move)) == 0){
			// 无碰撞
			collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 0);
			collision_point_visit(next_x, next_y, get_rot_direction(boat[i].direction, move), 1);
		}
		else{
			// 发生了碰撞
			// 当前ship换方式
			for(int j = 0; j < 3; j++){
				if(j == v[i].move) continue;
				next_x = boat[i].x;
				next_y = boat[i].y;
				change_point(next_x, next_y, boat[i].direction, j);
				if(collision_point_(next_x, next_y, get_rot_direction(boat[i].direction, j)) == 0){
					if(check_path2delivery(next_x, next_y, get_rot_direction(boat[i].direction, j)) == 1){
						move = j;
						break;
					}
				}
			}
			// 如果三种都不行，那就保持不动
			if(move == v[i].move)
			{
				v[i].move = -1;
				collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 1);
			}
			else{
				v[i].move = move;
				collision_point_visit(boat[i].x, boat[i].y, boat[i].direction, 0);
				collision_point_visit(next_x, next_y, get_rot_direction(boat[i].direction, move), 1);
			}
		}
	}
	return v;
}

// x代表当前的航向，y代表目的地应该需要的航向
int get_direction_rot(int x, int y){
	if(x == 0 && y == 2) return 1;
	if(x == 0 && y == 3) return 0;
	if(x == 3 && y == 0) return 1;
	if(x == 3 && y == 1) return 0;
	if(x == 1 && y == 3) return 1;
	if(x == 1 && y == 2) return 0;
	if(x == 2 && y == 1) return 1;
	if(x == 2 && y == 0) return 0; 
	return -1;
}

// 通过当前航向和旋转方向得到下一个航向
int get_rot_direction(int direction, int rot){
	if(rot == 2) return direction;
	if(direction == 0){
		if(rot == 0) return 3;
		else if(rot == 1) return 2;
	}
	else if(direction == 1){
		if(rot == 0) return 2;
		else if(rot == 1) return 3;
	}
	else if(direction == 2){
		if(rot == 0) return 0;
		else if(rot == 1) return 1;
	}
	else if(direction == 3){
		if(rot == 0) return 1;
		else if(rot == 1) return 0;
	}
	return -1;
}

void change_point(int &x, int &y, int direction, int rot){
	if(rot == 0){
		if(direction == 0){
			y = y + 2;
		}
		else if(direction == 1){
			x = y - 2;
		}
		else if(direction == 2){
			x = x - 2;
		}
		else if(direction == 3){
			x = x + 2;
		}
	}
	else if(rot == 1){
		if(direction == 0){
			x = x + 1;
			y = y + 1;
		}
		else if(direction == 1){
			x = x - 1;
			y = y - 1;
		}
		else if(direction == 2){
			x = x - 1;
			y = y + 1;
		}
		else if(direction == 3){
			x = x + 1;
			y = y - 1;
		}
	}
	else if(rot == 2){
		if(direction == 0){
			y = y + 1;
		}
		else if(direction == 1){
			y = y - 1;
		}
		else if(direction == 2){
			x = x - 1;
		}
		else if(direction == 3){
			x = x + 1;
		}
	}
}

int get_move(int x, int y){
	for(int i = 0; i < 4; i++){
		if(robot_move_x[i] == x && robot_move_y[i] == y) return i;
	}
	return -1;
}

int check_direction(int x, int y, int ux, int uy, int direction){
	int dx = (ux - x) == 0 ? 0 : ((ux - x) > 0 ? 1 : -1);
    int dy = (uy - y) == 0 ? 0 : ((uy - y) > 0 ? 1 : -1);
    if(abs(ux - x) == 1 && abs(uy - y) == 1){
    	int next_x = x;
    	int next_y = y;
    	change_point(next_x, next_y, direction, 1);
    	if(check_path2delivery(next_x, next_y, get_rot_direction(direction, 1)) == 1) return 1;
    }
    if(dx != 0 && dy != 0){
	    // cerr << dx << ' ' << dy << endl;
	    // cerr << x << ' ' << y << ' ' << ux << ' ' << uy << ' ' << direction << endl;
	    for(int i = 0; i < 4; i++){
	    	if(robot_move_x[i] == dx || robot_move_y[i] == dy){
	    		if(direction == i) {
	    			int next_x = x + robot_move_x[i];
	    			int next_y = y + robot_move_y[i];
	    			// 沿着当前方向开
	    			if(check_path2delivery(next_x, next_y, direction) == 1) return 2;
	    		}
	    	}
	    }
	    // cerr << x << ' ' << y << ' ' << ux << ' ' << uy << ' ' << direction << endl;
	    int dic1 = get_move(dx, 0);
	    int dic2 = get_move(0, dy);
	    if(direction == dic1){
	    	int rot = get_direction_rot(direction, dic2);
			int next_x = x;
	    	int next_y = y;
	    	change_point(next_x, next_y, direction, rot);
	    	if(check_path2delivery(next_x, next_y, dic2) == 1) return rot;
	    }
	    else if(direction == dic2){
	    	int rot = get_direction_rot(direction, dic1);
			int next_x = x;
	    	int next_y = y;
	    	change_point(next_x, next_y, direction, rot);
	    	if(check_path2delivery(next_x, next_y, dic1) == 1) return rot;
	    }
	    // for(int i = 0; i < 4; i++){
	    // 	if(robot_move_x[i] == dx || robot_move_y[i] == dy){
	    // 		if(robot_move_x[direction] == dx || robot_move_y[direction] == dy){
		   //  		// cerr << i << endl;
		   //  		if(direction == i) continue;
		   //  		// 换到另一个方向
					// int rot = get_direction_rot(direction, i);
					// int next_x = x;
			  //   	int next_y = y;
			  //   	change_point(next_x, next_y, direction, rot);
			  //   	if(check_path2delivery(next_x, next_y, get_move_reverse(i)) == 1) return rot;
			  //   }
	    // 	}
	    // }
	    // cerr << x << ' ' << y << ' ' << ux << ' ' << uy << ' ' << direction << endl;
	    for(int i = 0; i < 4; i++){
	    	if(robot_move_x[i] == dx || robot_move_y[i] == dy){
	    		continue;
	    	}
	    	if(i == direction) continue;
	    	// cerr << i << endl;
	    	// cerr << direction << ' ' << get_move_reverse(i) << endl;
	    	int rot = get_direction_rot(direction, get_move_reverse(i));
	    	int next_x = x;
	    	int next_y = y;
	    	change_point(next_x, next_y, direction, rot);
	    	// cerr << i << ' ' << next_x << ' ' << next_y << ' ' << get_move_reverse(i) << ' ' << check_path2delivery(next_x, next_y, get_move_reverse(i)) << endl;
	    	if(check_path2delivery(next_x, next_y, get_move_reverse(i)) == 1) return rot;
	    }
	}
	else{
		int move = get_move(dx, dy);
		if(direction == move){
			int next_x = x + robot_move_x[move];
			int next_y = y + robot_move_y[move];
			// 沿着当前方向开
			if(check_path2delivery(next_x, next_y, direction) == 1) return 2;
		}
		else if(get_rot_direction(direction, 0) == move){
			int next_x = x;
	    	int next_y = y;
	    	change_point(next_x, next_y, direction, 0);
	    	if(check_path2delivery(next_x, next_y, get_rot_direction(direction, 0)) == 1) return 0;
		}
		else if(get_rot_direction(direction, 1) == move){
			int next_x = x;
	    	int next_y = y;
	    	change_point(next_x, next_y, direction, 1);
	    	if(check_path2delivery(next_x, next_y, get_rot_direction(direction, 1)) == 1) return 1;
		}
		else{
			int next_x = x;
	    	int next_y = y;
	    	change_point(next_x, next_y, direction, 0);
	    	if(check_path2delivery(next_x, next_y, get_rot_direction(direction, 0)) == 1) return 0;
	    	next_x = x;
	    	next_y = y;
	    	change_point(next_x, next_y, direction, 1);
	    	if(check_path2delivery(next_x, next_y, get_rot_direction(direction, 1)) == 1) return 1;
		}
	}
    return -1;
}

// 简易版 不考虑bfs，直接使用曼哈顿距离
void control_ship(int frame_id){
	// 修改目的地
	for(int i = 0; i < boat_num; i++){
		if(boat[i].status == 1) continue;
		if(boat[i].goods_num == boat_capacity && grid_copy[boat[i].x][boat[i].y] == 'B'){
			boat[i].target_x = delivery_point[0].first;
			boat[i].target_y = delivery_point[0].second;
			boat[i].to_delivery = 1;
		}
		else if(grid_copy[boat[i].x][boat[i].y] == 'B'){
			boat[i].target_x = -1;
			boat[i].target_y = -1;
		}
		if(grid_copy[boat[i].x][boat[i].y] == 'T'){
			boat[i].to_delivery = 0;
			boat[i].target_x = berth[0].x;
			boat[i].target_y = berth[0].y;
		}
		if(grid_copy[boat[i].x][boat[i].y] == 'S' && boat[i].target_x == -1){
			boat[i].target_x = berth[0].x;
			boat[i].target_y = berth[0].y;
		}
	}
	for(int i = 0; i < boat_num; i++){
		cerr << boat[i].x << ' ' << boat[i].y << ' ' << boat[i].target_x << ' ' << boat[i].target_y << ' ' << boat[i].direction << endl;
	}
	vector<BOAT_MOVE> boat_collision_move;
	// 船舶航行路线
	for(int i = 0; i < boat_num; i++){
		if(boat[i].status == 1){
			boat_collision_move.push_back(BOAT_MOVE(boat[i].x, boat[i].y, boat[i].direction, -1));
			continue;
		}
		if(boat[i].target_x == -1){
			boat_collision_move.push_back(BOAT_MOVE(boat[i].x, boat[i].y, boat[i].direction, -1));
			continue;
		}
		// 矫正航向
		cerr << check_direction(boat[i].x, boat[i].y, boat[i].target_x, boat[i].target_y, boat[i].direction) << endl;
		boat_collision_move.push_back(BOAT_MOVE(boat[i].x, boat[i].y, boat[i].direction, check_direction(boat[i].x, boat[i].y, boat[i].target_x, boat[i].target_y, boat[i].direction)));
		// cerr << check_direction(boat[i].x, boat[i].y, boat[i].target_x, boat[i].target_y, boat[i].direction) << endl;
		// cerr << boat_collision_move[i].move << endl;
		// if(boat[i].status == 1) {
		// 	boat_collision_move.push_back(BOAT_MOVE(boat[i].x, boat[i].y, -1));
		// 	continue;
		// }
		// // 矫正航向
		// int next_move = shotpath_delivery[boat[i].delivery][boat[i].x][boat[i].y];
		// if(get_move_reverse(boat[i].direction) == next_move){
		// 	int fale_direction = direction2direction(boat[i].direction);

		// 	if(check_path2delivery(boat[i].x, boat[i].y, (fale_direction + 1) % 4) == 1) boat_collision_move.push_back(BOAT_MOVE(boat[i].x, boat[i].y, 0));
		// 	else if(check_path2delivery(boat[i].x, boat[i].y, (fale_direction - 1 + 4) % 4) == 1) boat_collision_move.push_back(BOAT_MOVE(boat[i].x, boat[i].y, 1));
		// 	else if(check_path2delivery(boat[i].x, boat[i].y, fale_direction) == 1) boat_collision_move.push_back(BOAT_MOVE(boat[i].x, boat[i].y, 2));
		// 	else boat_collision_move.push_back(BOAT_MOVE(boat[i].x, boat[i].y, -1));
		// }
		// else if(){

		// }
		// else if(){

		// }
		// else{
		// 	boat_collision_move.push_back(BOAT_MOVE(boat[i].x, boat[i].y, 2));
		// }
	}
	// 船舶碰撞
	vector<BOAT_MOVE> new_boat_collision_move = boat_collision_judge(boat_collision_move);

	// 船舶更新线路
	for(int i = 0; i < boat_num; i++){
		cerr << boat_collision_move[i].move << ' ' << new_boat_collision_move[i].move << endl;
		boat_collision_move[i].move = new_boat_collision_move[i].move;
	}

	// 船舶控制
	for(int i = 0; i < boat_num; i++){
		if(boat[i].status == 1) continue;
		if(grid_copy[boat[i].x][boat[i].y] == 'K' && boat[i].to_delivery == 0){
			printf("berth %d\n", i);
		}
		// else if(grid_copy[boat[i].x][boat[i].y] == 'B' && boat[i].goods_num == 0){
		// 	printf("berth %d\n", i);
		// }
		else if(boat_collision_move[i].move == 2){
			printf("ship %d\n", i);
		}
		else if(boat_collision_move[i].move == 1){
			printf("rot %d %d\n", i, 1);
		}
		else if(boat_collision_move[i].move == 0){
			printf("rot %d %d\n", i, 0);
		}
	}
}
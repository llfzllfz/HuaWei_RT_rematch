#include "robot.h"
#include "berth.h"
#include "map.h"
#include <bits/stdc++.h>
using namespace std;

ROBOT robot[50];
int robot_price=2000;

double pow_index_base = 1;
vector<double> containers;
int visit_collision[200][200];

void read_robot(){
	scanf("%d", &robot_num);
    for(int i = 0; i < robot_num; i ++)
        scanf("%d%d%d%d", &robot[i].id, &robot[i].goods, &robot[i].x, &robot[i].y);
}

int check(int x, int y){
    if(x < 0 || x >= N) return -1;
    if(y < 0 || y >= N) return -1;
    char tmp = grid_copy[x][y];
    if(tmp == '.' || tmp == '>' || tmp == 'R' || tmp == 'B' || tmp == 'C' || tmp == 'c' || tmp == 'g') return 1;
    return -1;
}

string int_pad2str(int x){
    string sx = "";
    while(x){
        int mod = x % 10;
        x /= 10;
        sx = sx + char(mod + '0');
    }
    while(sx.size() < 3){
        sx = '0' + sx;
    }
    return sx;
}

string int2str(int x, int y){
    string sx = int_pad2str(x);
    string sy = int_pad2str(y);
    return sx + sy;
}

void Init_robot(ROBOT &x, int is_first){
    // while(!x.robot_find_goods_move.empty()) x.robot_find_goods_move.pop();
    if(!x.best_goods.empty()){
        if(is_first == 1) {
            x.best_goods.clear();
            x.pre_best_goods = POINT(-1, -1);
        }
        else{
            POINT bestPoint = x.best_goods[0];
            x.best_goods.clear();
            x.best_goods.push_back(bestPoint);
            x.pre_best_goods = bestPoint;
        }
    }
}

void bfs_find_goods(int robot_index, int frame_id, double pow_index, int region){
    int x = robot[robot_index].x;
    int y = robot[robot_index].y;
    memset(robot[robot_index].visit, 0, sizeof(robot[robot_index].visit));
    POINT bestPoint = POINT(-1, -1, 0, 0);
    queue<POINT> q;
    q.push(POINT(x, y, 0, 0));
    robot[robot_index].visit[x][y] = 1;
    while(!q.empty()){
        POINT now = q.front();
        q.pop();
        if(now.dis >= region) continue;
        for(int i = 0; i < 4; i++){
            int next_x = now.x + robot_move_x[i];
            int next_y = now.y + robot_move_y[i];
            if(check(next_x, next_y) != 1) continue;
            if(robot[robot_index].visit[next_x][next_y] == 1) continue;
            POINT next = POINT(next_x, next_y);
            next.dis =  now.dis + 1;
            q.push(next);
            robot[robot_index].visit[next.x][next.y] = 1;
            robot[robot_index].path[next.x][next.y].x = now.x;
            robot[robot_index].path[next.x][next.y].y = now.y;
            robot[robot_index].path[next.x][next.y].move = i;
            if(grid_copy[next_x][next_y] == 'g'){
                if(next.dis > (1000 - (frame_id - goods_time_mp[int2str(next_x, next_y)] + 1))) continue;
                int value = goods_value_mp[int2str(next_x, next_y)];
                if(bestPoint.x == -1){
                    // 锁定最近距离的货物到货物最近距离的港口
                    // if(region == 400)
                    //     cerr << ' ' << next_x << ' ' << next_y << endl;
                    for(int j = 0; j < robot_berth.size(); j++){
                        int tmp_berth_index = robot_berth[j];
                        if(shotpath_berth_dis[tmp_berth_index][next.x][next.y] == -1) continue;
                        if(bestPoint.goods2berth_dis > shotpath_berth_dis[tmp_berth_index][next.x][next.y]){
                            bestPoint.x = next.x;
                            bestPoint.y = next.y;
                            bestPoint.value = value;
                            bestPoint.dis = next.dis;
                            bestPoint.goods2berth_dis = shotpath_berth_dis[tmp_berth_index][next.x][next.y];
                            bestPoint.berth = tmp_berth_index;
                            bestPoint.pre_dis = robot[robot_index].dis;
                        }
                    }
                    // cerr << bestPoint.x << endl;
                    // cerr << bestPoint.goods2berth_dis << endl;
                    // if(region == 400)
                    //     cerr << double(bestPoint.dis + bestPoint.pre_dis) - containers[robot_index] * double(bestPoint.goods2berth_dis) << endl;
                    if((double(bestPoint.dis + bestPoint.pre_dis) < containers[robot_index] * double(bestPoint.goods2berth_dis)) && bestPoint.x != -1)
                        {robot[robot_index].best_goods.push_back(bestPoint);}
                }
                if(bestPoint.x == -1) continue;
                bestPoint.x = -1;
                bestPoint.goods2berth_dis = 10000;
            }
        }
    }
    // cerr << robot[robot_index].best_goods.size() << endl;
    if(robot[robot_index].best_goods.size() > 0)
        sort(robot[robot_index].best_goods.begin(), robot[robot_index].best_goods.end(), sort_best_goods);

}

int sort_best_goods(POINT x1, POINT x2){
    return xjb_func(x1) > xjb_func(x2);
}

double xjb_func(POINT x1){
    int g2b = x1.goods2berth_dis;
    int r2g = x1.dis + x1.pre_dis;
    int value = goods_value_mp[int2str(x1.x, x1.y)];
    double pow_index = pow_index_base;
    return pow(double(value), pow_index) / double(x1.dis + g2b);
}

// x机器人基准, y机器人移动
int collision_move_opposite(int robot_x, int robot_y, int y_move){
    // cerr << "Start check" << endl;
    int move = -1;
    int x_next_move = -1;
    // cerr << "Start x_next_move" << endl;
    // cerr << robot_x << endl;
    // cerr << robot[robot_x].berth << endl;
    // cerr << robot[robot_x].robot_find_goods_move.size() << ' ' << robot[robot_x].berth << endl;
    if(!robot[robot_x].collision_move.empty()) x_next_move = robot[robot_x].collision_move.top();
    else if(!robot[robot_x].robot_find_goods_move.empty()) x_next_move = robot[robot_x].robot_find_goods_move.top();
    else if(robot[robot_x].berth >= 0) x_next_move = shotpath_berth[robot[robot_x].berth][robot[robot_x].x][robot[robot_x].y];
    int y_next_move = -1;
    // cerr << "Start y_next_move" << endl;
    if(!robot[robot_y].collision_move.empty()) y_next_move = robot[robot_y].collision_move.top();
    else if(!robot[robot_y].robot_find_goods_move.empty()) y_next_move = robot[robot_y].robot_find_goods_move.top();
    else if(robot[robot_y].berth >= 0) y_next_move = shotpath_berth[robot[robot_y].berth][robot[robot_y].x][robot[robot_y].y];
    // cerr << "Start check 1" << endl;
    for(int i = 0; i < 4; i++){
        if(i == y_move) continue;
        if(i == x_next_move) continue;
        int next_x = robot[robot_y].x + robot_move_x[i];
        int next_y = robot[robot_y].y + robot_move_y[i];
        if(check(next_x, next_y) == -1) continue;
        if(i == y_next_move) return y_next_move;
    }
    // cerr << "Start check 2" << endl;
    for(int i = 0; i < 4; i++){
        if(i == y_move) continue;
        if(i == x_next_move) continue;
        int next_x = robot[robot_y].x + robot_move_x[i];
        int next_y = robot[robot_y].y + robot_move_y[i];
        if(check(next_x, next_y) == -1) continue;
        return i;
    }
    // cerr << "Start check 3" << endl;
    for(int i = 0; i < 4; i++){
        if(i == y_move) continue;
        int next_x = robot[robot_y].x + robot_move_x[i];
        int next_y = robot[robot_y].y + robot_move_y[i];
        if(check(next_x, next_y) == -1) continue;
        return i;
    }
    return -1;
}

void collision_move_opposite2(int x, ROBOT &y){
    return;
}

// 优先顺序 返回泊位->寻找货物
vector<ROBOT_MOVE> collision_judge(vector<ROBOT_MOVE> v){
    // // vector<string> collision_v;
    unordered_map<string, int> collision_add;
    // 先check返回泊位的机器人
    for(int i = 0; i < robot_num; i++){
        // cerr << i << endl;
        if(robot[i].goods == 0) continue;
        int next_x = v[i].x;
        int next_y = v[i].y;
        if(v[i].move != -1){
            next_x = next_x + robot_move_x[v[i].move];
            next_y = next_y + robot_move_y[v[i].move];
        }
        // 发生碰撞，两个机器人交换位置
        if(collision_add[int2str(robot[i].x, robot[i].y)] > 0){
            if(next_x == robot[collision_add[int2str(robot[i].x, robot[i].y)] - 1].x && next_y == robot[collision_add[int2str(robot[i].x, robot[i].y)] - 1].y){
                // cerr << collision_add[int2str(robot[i].x, robot[i].y)] - 1 << ' ' << i << endl;
                int new_move = collision_move_opposite(collision_add[int2str(robot[i].x, robot[i].y)] - 1, i, v[i].move);
                if(new_move == -1){
                    // cerr << collision_add[int2str(robot[i].x, robot[i].y)] - 1 << ' ' << i << endl;
                    new_move = collision_move_opposite(i, collision_add[int2str(robot[i].x, robot[i].y)] - 1, v[collision_add[int2str(robot[i].x, robot[i].y)] - 1].move);
                    if(new_move == -1) {
                        v[collision_add[int2str(robot[i].x, robot[i].y)] - 1].move = -1;
                        collision_add[int2str(robot[i].x, robot[i].y)] = 0;
                        v[i].move = new_move;
                    }
                    else{
                        int tmp_idx = collision_add[int2str(next_x, next_y)] - 1;
                        collision_add[int2str(next_x, next_y)] = 0;
                        v[tmp_idx].move = new_move;
                        int next_x_ = next_x + robot_move_x[new_move];
                        int next_y_ = next_y + robot_move_y[new_move];
                        collision_add[int2str(next_x_, next_y_)] = tmp_idx + 1;
                    }
                }
                else{
                    v[i].move = new_move;
                }
            }
        }
        // 发生碰撞, 抢占同一个点
        else if(collision_add[int2str(next_x, next_y)] > 0){
            // cerr << collision_add[int2str(robot[i].x, robot[i].y)] - 1 << ' ' << i << endl;
            int new_move = collision_move_opposite(collision_add[int2str(next_x, next_y)] - 1, i, v[i].move);
            // cerr << new_move << endl;
            if(new_move == -1){
                new_move = collision_move_opposite(i, collision_add[int2str(next_x, next_y)] - 1, v[collision_add[int2str(next_x, next_y)] - 1].move);
                // cerr << "  " << new_move << endl;
                if(new_move == -1) {
                    v[collision_add[int2str(next_x, next_y)] - 1].move = -1;
                    collision_add[int2str(next_x, next_y)] = 0;
                    v[i].move = new_move;
                }
                else{
                    int tmp_idx = collision_add[int2str(next_x, next_y)] - 1;
                    collision_add[int2str(next_x, next_y)] = 0;
                    v[tmp_idx].move = new_move;
                    int next_x_ = next_x + robot_move_x[new_move];
                    int next_y_ = next_y + robot_move_y[new_move];
                    collision_add[int2str(next_x_, next_y_)] = tmp_idx + 1;
                }
            }
            else{
                v[i].move = new_move;
            }
        }
        next_x = v[i].x;
        next_y = v[i].y;
        if(v[i].move != -1){
            next_x = next_x + robot_move_x[v[i].move];
            next_y = next_y + robot_move_y[v[i].move];
        }
        // 机器人抢占该点
        collision_add[int2str(next_x, next_y)] = i + 1;
    }

    // check 寻找货物的机器人
    for(int i = 0; i < robot_num; i++){
        // cerr << i << endl;
        if(robot[i].goods == 1) continue;
        int next_x = v[i].x;
        int next_y = v[i].y;
        if(v[i].move != -1){
            next_x = next_x + robot_move_x[v[i].move];
            next_y = next_y + robot_move_y[v[i].move];
        }
        // 发生碰撞，两个机器人交换位置
        if(collision_add[int2str(robot[i].x, robot[i].y)] > 0){
            if(next_x == robot[collision_add[int2str(robot[i].x, robot[i].y)] - 1].x && next_y == robot[collision_add[int2str(robot[i].x, robot[i].y)] - 1].y){
                // cerr << collision_add[int2str(robot[i].x, robot[i].y)] - 1 << ' ' << i << endl;
                int new_move = collision_move_opposite(collision_add[int2str(robot[i].x, robot[i].y)] - 1, i, v[i].move);
                if(new_move == -1){
                    // cerr << collision_add[int2str(robot[i].x, robot[i].y)] - 1 << ' ' << i << endl;
                    new_move = collision_move_opposite(i, collision_add[int2str(robot[i].x, robot[i].y)] - 1, v[collision_add[int2str(robot[i].x, robot[i].y)] - 1].move);
                    if(new_move == -1) {
                        v[collision_add[int2str(robot[i].x, robot[i].y)] - 1].move = -1;
                        collision_add[int2str(robot[i].x, robot[i].y)] = 0;
                        v[i].move = new_move;
                    }
                    else{
                        int tmp_idx = collision_add[int2str(next_x, next_y)] - 1;
                        collision_add[int2str(next_x, next_y)] = 0;
                        v[tmp_idx].move = new_move;
                        int next_x_ = next_x + robot_move_x[new_move];
                        int next_y_ = next_y + robot_move_y[new_move];
                        collision_add[int2str(next_x_, next_y_)] = tmp_idx + 1;
                    }
                }
                else{
                    v[i].move = new_move;
                }
            }
        }
        // 发生碰撞, 抢占同一个点
        else if(collision_add[int2str(next_x, next_y)] > 0){
            // cerr << collision_add[int2str(robot[i].x, robot[i].y)] - 1 << ' ' << i << endl;
            int new_move = collision_move_opposite(collision_add[int2str(next_x, next_y)] - 1, i, v[i].move);
            // cerr << "robot2 " << v[i].move << ' ' << new_move << endl;
            if(new_move == -1){
                new_move = collision_move_opposite(i, collision_add[int2str(next_x, next_y)] - 1, v[collision_add[int2str(next_x, next_y)] - 1].move);
                // cerr << "  " << new_move << endl;
                if(new_move == -1) {
                    v[collision_add[int2str(next_x, next_y)] - 1].move = -1;
                    collision_add[int2str(next_x, next_y)] = 0;
                    v[i].move = new_move;
                }
                else{
                    int tmp_idx = collision_add[int2str(next_x, next_y)] - 1;
                    collision_add[int2str(next_x, next_y)] = 0;
                    v[tmp_idx].move = new_move;
                    int next_x_ = next_x + robot_move_x[new_move];
                    int next_y_ = next_y + robot_move_y[new_move];
                    collision_add[int2str(next_x_, next_y_)] = tmp_idx + 1;
                }
            }
            else{
                v[i].move = new_move;
            }
        }
        next_x = v[i].x;
        next_y = v[i].y;
        if(v[i].move != -1){
            next_x = next_x + robot_move_x[v[i].move];
            next_y = next_y + robot_move_y[v[i].move];
        }
        // 机器人抢占该点
        collision_add[int2str(next_x, next_y)] = i + 1;
    }
    return v;
}

int robot_buy(){
    if(robot_num == 0) return 1;
    if(robot_num <= berth_num * 2) return 1;
    Init_containers();
    return 0;
}

void cal_best_path(int robot_index){
    int x = robot[robot_index].best_goods[0].x;
    int y = robot[robot_index].best_goods[0].y;
    while(x != robot[robot_index].x || y != robot[robot_index].y){
        int move = robot[robot_index].path[x][y].move;
        robot[robot_index].robot_find_goods_move.push(move);
        int now_xx = robot[robot_index].path[x][y].x;
        int now_yy = robot[robot_index].path[x][y].y;
        x = now_xx;
        y = now_yy;
    }
    robot[robot_index].berth = robot[robot_index].best_goods[0].berth;
    robot[robot_index].goods_x = robot[robot_index].best_goods[0].x;
    robot[robot_index].goods_y = robot[robot_index].best_goods[0].y;
}

int select2best_point(POINT x1, POINT x2, double pow_index){
    if(x1.x == x2.x && x1.y == x2.y){
        int x1_dis = x1.dis + x1.goods2berth_dis + x1.pre_dis;
        int x2_dis = x2.dis + x2.goods2berth_dis + x2.pre_dis;
        int x1_value = goods_value_mp[int2str(x1.x, x1.y)];
        int x2_value = goods_value_mp[int2str(x2.x, x2.y)];
        // if(pow(double(x1_value), pow_index) / double(x1_dis) + g2b_alpha * x1.goods2berth_dis - b2g_alpha * (x1.dis + x1.pre_dis) < pow(double(x2_value), pow_index) / double(x2_dis) + g2b_alpha * x2.goods2berth_dis - b2g_alpha * (x2.dis + x2.pre_dis)){
        //     return 1;
        // }
        if(xjb_func(x1) < xjb_func(x2)) return 1;
        else return 2;
    }
    return 0;
}

void control_robot(int frame_id){
    vector<thread> threads;
    // cerr << "Robot bfs" << endl;
    for(int i = 0; i < robot_num; i++){
        // cerr << i << ' ' << robot[i].best_goods.size() << ' ' << robot[i].is_first << endl;
        if(robot[i].goods == 1) continue;
        if(robot[i].best_goods.empty()) robot[i].is_first = 1;
        
        Init_robot(robot[i], robot[i].is_first);
        // if()
        int region = (robot[i].is_first == 1 ? 400 : 50);
        // cerr << i << ' ' << region << ' ' ;
        // if(!robot[i].robot_find_goods_move.empty()) continue;
        // threads.emplace_back(thread(bfs_find_goods, i, frame_id, pow_index_base, region));
        bfs_find_goods(i, frame_id, pow_index_base, region);
    }
    // cerr << endl;
    // for(auto& t : threads){
    //     t.join();
    // }
    int flg = 1;
    // cerr << "Find goods" << endl;
    while(flg){
        flg = 0;
        for(int i = 0; i < robot_num; i++){
            if(robot[i].best_goods.empty() || robot[i].goods == 1) continue;
            for(int j = 0; j < robot_num; j++){
                if(i == j || robot[j].best_goods.empty() || robot[j].goods == 1) continue;
                int select = select2best_point(robot[i].best_goods[0], robot[j].best_goods[0], pow_index_base);
                if(select == 0) continue;
                else if(select == 1) {
                    robot[i].best_goods.erase(robot[i].best_goods.begin());
                    flg = 1;
                    break;
                }
                else if(select == 2) {
                    robot[j].best_goods.erase(robot[j].best_goods.begin());
                    flg = 1;
                    break;
                }
            }
        }
    }
    // cerr << 0 << ' ' << robot[0].best_goods.size() << endl;
    // cerr << "Cal best path" << endl;
    for(int i = 0; i < robot_num; i++){
        if(robot[i].best_goods.empty() || robot[i].goods == 1) continue;
        // cerr << "CAL " << robot[i].best_goods.size() << endl;
        if(robot[i].pre_best_goods.x == robot[i].best_goods[0].x && robot[i].pre_best_goods.y == robot[i].best_goods[0].y) continue;
        while(!robot[i].collision_move.empty()) robot[i].collision_move.pop();
        while(!robot[i].robot_find_goods_move.empty()) robot[i].robot_find_goods_move.pop();
        // cerr << robot[i].best_goods.size() << endl;
        cal_best_path(i);
        // cerr << "cal best path " << i << ' ' << robot[i].robot_find_goods_move.size() << endl;
    }
    // 计算路线
    // cerr << 0 << ' ' << robot[0].best_goods.size() << endl;
    // cerr << "Cal the path" << endl;
    vector<ROBOT_MOVE> Robot_move;
    for(int i = 0; i < robot_num; i++){
        robot[i].is_first = 0;
        if(robot[i].goods == 0){
            int move = -1;
            // cerr << robot[i].collision_move.size() << ' ' << robot[i].robot_find_goods_move.size() << endl;
            if(!robot[i].collision_move.empty()){
                move = robot[i].collision_move.top();
                move = get_move_reverse(move);
                // cerr << move << endl;
                robot[i].collision_move.pop();
            }
            else if(!robot[i].robot_find_goods_move.empty()){
                move = robot[i].robot_find_goods_move.top();
                robot[i].robot_find_goods_move.pop();
            }
            Robot_move.push_back(ROBOT_MOVE(robot[i].x, robot[i].y, move));
            // cerr << move << endl;
            // cerr << i << ' ' << move << endl;
        }
        else{
            Robot_move.push_back(ROBOT_MOVE(robot[i].x, robot[i].y, shotpath_berth[robot[i].berth][robot[i].x][robot[i].y]));
        }
    }
    // cerr << Robot_move.size() << endl;
    // 碰撞检测
    if(Robot_move.size() > 0){
        // cerr << 0 << ' ' << robot[0].best_goods.size() << endl;
        // cerr << "Collision judge" << endl;
        // cerr << Robot_move.size() << endl;
        vector<ROBOT_MOVE> new_Robot_move = collision_judge(Robot_move);


    // 路线更新
        // cerr << 0 << ' ' << robot[0].best_goods.size() << endl;
        // cerr << "Update path" << endl;
        for(int i = 0; i < robot_num; i++){
            if(Robot_move[i].move == new_Robot_move[i].move) {
                if(Robot_move[i].move != -1) robot[i].dis = robot[i].dis + 1;
                continue;
            }
            // if(robot[i].goods == 1) continue;
            if(new_Robot_move[i].move != -1)
                robot[i].collision_move.push(new_Robot_move[i].move);
            if(Robot_move[i].move != -1)
                robot[i].robot_find_goods_move.push(Robot_move[i].move);
            Robot_move[i].move = new_Robot_move[i].move;
        }
    }

    // 机器人控制
    for(int i = 0; i < robot_num; i++){
        int move = Robot_move[i].move;
        if(move == -1) continue;
        // cerr << move << endl;
        printf("move %d %d\n", i, move);
        int next_x = robot[i].x + robot_move_x[move];
        int next_y = robot[i].y + robot_move_y[move];
        if(robot[i].goods == 0 && next_x == robot[i].goods_x && next_y == robot[i].goods_y && grid_copy[next_x][next_y] == 'g'){
            printf("get %d\n", i);
            cerr << "get" << endl;
            while(!robot[i].collision_move.empty()) robot[i].collision_move.pop();
            while(!robot[i].robot_find_goods_move.empty()) robot[i].robot_find_goods_move.pop();
            robot[i].dis = 0;
        }
        // 还差一个判断条件，判断当前到达的港口是不是期望港口
        if(robot[i].goods == 1 && grid_copy[next_x][next_y] == 'B'){
            printf("pull %d\n", i);
            cerr << "pull" << endl;
            robot[i].is_first = 1;
            while(!robot[i].collision_move.empty()) robot[i].collision_move.pop();
            while(!robot[i].robot_find_goods_move.empty()) robot[i].robot_find_goods_move.pop();
            // cerr << i << ' ' << robot[i].best_goods.size() << endl;
            while(!robot[i].best_goods.empty()) {
                robot[i].best_goods.clear();
                robot[i].pre_best_goods = POINT(-1, -1);
                robot[i].dis = 0;
            }
        }
    }
    // cerr << 0 << ' ' << robot[0].best_goods.size() << endl;
    // // 判断购买机器人的地点
    // if(robot_buy() == 1){
        
    // }
}

void Init_containers(){
    containers.push_back(2);
    // for(int i = 0; i < robot_num; i++) containers.push_back(2);
}
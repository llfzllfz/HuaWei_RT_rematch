#ifndef SHIP_H
#define SHIP_H


extern int boat_num;
extern int boat_capacity;

struct BOAT
{
    int id, x, y, direction;
    int goods_num, status;
    int delivery = -1;
    int berth = -1;
    int target_x = -1, target_y = -1;
    int to_delivery = 0;
    BOAT() {}
    BOAT(int startX, int startY) {
        x = startX;
        y = startY;
    }
};

struct BOAT_MOVE{
	int x, y, direction, move;
	BOAT_MOVE() {}
	BOAT_MOVE(int _, int __, int ___, int ____){
		x = _;
		y = __;
		direction = ___;
		move = ____;
	}
};

void read_boat();
void control_ship(int frame_id);
int get_direction_rot(int x, int y);
int get_rot_direction(int direction, int rot);
void change_point(int &x, int &y, int direction, int rot);


extern BOAT boat[30];
extern int boat_price;
#endif
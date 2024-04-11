#ifndef SHIP_H
#define SHIP_H


extern int boat_num;
extern int boat_capacity;

struct Boat
{
    int id, x, y, dir;
    int goods_num, status;
    Boat() {}
    Boat(int startX, int startY) {
        x = startX;
        y = startY;
    }
};

void read_boat();

extern Boat boat[10];
extern int boat_price;
#endif

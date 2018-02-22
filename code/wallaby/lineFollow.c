#include "lineFollow.h"

void lineFollow(int speed, int ms, int side) {
    //speed is in %. If speed is 100, then it runs at 100%
    int time = 0;
    while (time < ms) {
        if (side == LEFT) {
            //follow on the left of the line
            mav(MOT_LEFT, .01*speed*(BLACK - analog(0)));
            mav(MOT_RIGHT, .01*speed*(analog(0) - WHITE));
            msleep(1);
            time++;
        } else if (side == RIGHT) {
        	//follow on the right of the line
            mav(MOT_LEFT, .01*speed*(analog(0) - WHITE));
            mav(MOT_RIGHT, .01*speed*(BLACK - analog(0)));
            msleep(1);
            time++;
        }
    }
}

#include <kipr/botball.h>

#define MOT_LEFT 0
#define MOT_RIGHT 1

#define LEFT 0
#define RIGHT 1

#define WHITE 250	//Make this value GREATER THAN the sensor reading for white
#define BLACK 2200 	//Make this value LESS than the sensor reading for black
#define AVG WHITE+BLACK/2

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

int main()
{
    int selection = 0;
    while(selection == 0) {
    	if (left_button()) {
        	selection = 1;
        } else if (right_button()) {
        	selection = 2;
        }
    }
    if (selection == 1)
    	lineFollow(50, 3000, LEFT);
    else 
        lineFollow(100, 1500, RIGHT);
    
    return 0;
}

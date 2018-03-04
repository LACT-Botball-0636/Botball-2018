#include <kipr/botball.h>
#include "createDrive.h"
#include "lineFollow.h"

#define SERVO_LEFT 0
#define SERVO_RIGHT 1
#define SERVO_GATE 2
#define SERVO_WRIST 3

#define SERVO_RIGHT_UP 0
#define SERVO_RIGHT_MIDDLE 1000
#define SERVO_RIGHT_DOWN 1335
#define SERVO_LEFT_UP 1412
#define SERVO_LEFT_MIDDLE SERVO_LEFT_UP-(SERVO_RIGHT_MIDDLE-SERVO_RIGHT_UP)
#define SERVO_LEFT_DOWN SERVO_LEFT_UP-(SERVO_RIGHT_DOWN-SERVO_RIGHT_UP)
#define SERVO_GATE_CLOSED 557
#define SERVO_GATE_OPEN 1800
#define SERVO_WRIST_DUMP 1375
#define SERVO_WRIST_FLAT 20+50
#define SERVO_WRIST_DOWN 1598

void servo_slow(int port, int dest);
void servo_slow_2(int port1, int dest1, int port2, int dest2);
void arm_up();
void arm_down();
void arm_middle();
void right_up();
void left_up();
void right_down();
void left_down();
void left_middle();
void right_middle();
void push_poms_in();

int main()
{
    //start setup

    //initial servo positions
    set_servo_position(SERVO_GATE, SERVO_GATE_CLOSED);
    set_servo_position(SERVO_LEFT, SERVO_LEFT_UP);
    set_servo_position(SERVO_RIGHT, SERVO_RIGHT_UP);

    //initialize servos and create
    enable_servos();
    create_connect();
    
    /*set_servo_position(SERVO_WRIST, SERVO_WRIST_FLAT+70);
    arm_down();
    
    msleep(1000);
    
    create_forward(50, 100);
    
    msleep(1000);
    
    arm_middle();
    
    return 0;*/
    
    //IMPORTANT: might want to uncomment out the two set servo pos's above if the below malfunctions
    arm_up();
	
    //start arm up, move claw down, slowly move arm down
    servo_slow(SERVO_WRIST, SERVO_WRIST_DOWN);
    servo_slow_2(SERVO_LEFT, SERVO_LEFT_MIDDLE-50, SERVO_RIGHT, SERVO_RIGHT_MIDDLE+50);
    //end setup

    msleep(2000);
    
    //square up against right wall and face towards poms
    arm_middle();
    create_forward(5, 100);
    create_left(90, 100);
    create_square_up(1300, -100);
    create_forward(25, 100);
    create_right(90, 100);
    create_square_up(1000, -100);
    create_forward(2, 100);
    create_right(25, 100);

    //lift arm up, move claw to flat position, then put arm back down
    arm_up();
    set_servo_position(SERVO_WRIST, SERVO_WRIST_FLAT);
    set_servo_position(SERVO_GATE, SERVO_GATE_OPEN);
    //arm_middle();

    //move forward to collect poms
    //create_forward(5, 100);
    arm_down();
    push_poms_in();
    //servo_slow(SERVO_CLAW, SERVO_CLAW_CLOSED);

    //move forward and turn to face rest of poms
    create_forward(25, 100);
    create_left(25, 100);
    arm_middle();
    create_forward(25, 100);
    create_left(90, 100);
    create_square_up(2000, -100);
    arm_down();

    //continue along line (line following) and collect poms
    create_forward(45, 100);
    arm_middle();
    create_forward(30, 100);
    arm_down();
    create_forward(45, 100);

    //de-initialize
    create_disconnect();
    disable_servos();
    return 0;
}

//all these arm functions move the servo slowly so we can make sure the function ends once the servo has reached its desired location
void arm_up()
{
    servo_slow_2(SERVO_LEFT, SERVO_LEFT_UP, SERVO_RIGHT, SERVO_RIGHT_UP);
}

void arm_down() 
{
    servo_slow_2(SERVO_LEFT, SERVO_LEFT_DOWN, SERVO_RIGHT, SERVO_RIGHT_DOWN);
}

void arm_middle() 
{
    servo_slow_2(SERVO_LEFT, SERVO_LEFT_MIDDLE, SERVO_RIGHT, SERVO_RIGHT_MIDDLE);
}

void push_poms_in() {
	//pushes poms in to the carrier
    set_servo_position(SERVO_GATE, SERVO_GATE_CLOSED);
    msleep(300);
    set_servo_position(SERVO_GATE, SERVO_GATE_OPEN);
    msleep(300);
}

void servo_slow(int port, int dest)
{
    int pos = get_servo_position(port);
    if (dest > pos) 
    {
        while (pos < dest) 
        {
            pos++;
            set_servo_position(port, pos);
            msleep(1);
        }
    }
    else
    {
        while (pos > dest) 
        {
            pos--;
            set_servo_position(port, pos);
            msleep(1);
        }
    }
}

void servo_slow_2(int port1, int dest1, int port2, int dest2)
{
    //slow servo with 2 servos
    int pos1 = get_servo_position(port1);
    int dir1 = dest1 > pos1 ? 1 : -1;
    int pos2 = get_servo_position(port2);
    int dir2 = dest2 > pos2 ? 1 : -1;

    while (dir1*pos1 < dir1*dest1 || dir2*pos2 < dir2*dest2) 
    {
        if (dir1*pos1 < dir1*dest1) {
            pos1 += dir1*1;
            set_servo_position(port1, pos1);
        }

        if (dir2*pos2 < dir2*dest2) {
            pos2 += dir2*1;
            set_servo_position(port2, pos2);

        }

        msleep(1);
    }
}

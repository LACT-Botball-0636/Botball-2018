#include <kipr/botball.h>
#include "createDrive.h"

#define SERVO_LEFT 0
#define SERVO_RIGHT 1
#define SERVO_CLAW 2
#define SERVO_WRIST 3

#define SERVO_RIGHT_UP 0
#define SERVO_RIGHT_MIDDLE 1205
#define SERVO_RIGHT_DOWN 1301
#define SERVO_LEFT_UP 1412
#define SERVO_LEFT_MIDDLE SERVO_LEFT_UP-(SERVO_RIGHT_MIDDLE-SERVO_RIGHT_UP)
#define SERVO_LEFT_DOWN SERVO_LEFT_UP-(SERVO_RIGHT_DOWN-SERVO_RIGHT_UP)
#define SERVO_CLAW_CLOSED 1900
#define SERVO_CLAW_OPEN 300
#define SERVO_WRIST_DUMP 700
#define SERVO_WRIST_FLAT 0
#define SERVO_WRIST_DOWN 1778

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

int main()
{
    //start setup
    
    //initial servo positions
    set_servo_position(SERVO_CLAW, SERVO_CLAW_CLOSED);
    set_servo_position(SERVO_LEFT, SERVO_LEFT_UP);
    set_servo_position(SERVO_RIGHT, SERVO_RIGHT_UP);
    
    //initialize servos and create
    enable_servos();
    create_connect();
    msleep(1000);
   
    //start arm up, move claw down, slowly move arm down
    servo_slow(SERVO_WRIST, SERVO_WRIST_DOWN);
    arm_middle();
    //end setup
    
    msleep(2000);
    
    //move forward and square up with the two walls closest to base
    create_drive_direct(100, 100);
    msleep(300);
    create_drive_direct(-100, 100);
    msleep(1000);
    create_drive_direct(-100, -100);
    msleep(1300);
    create_drive_direct(100, 100);
    msleep(500);
    create_drive_direct(100, -100);
    msleep(1500);
    create_drive_direct(-75, -75);
    msleep(1500);
    create_stop();
    
	//lift arm up, move claw to flat position, then put arm back down
    arm_up();
    set_servo_position(SERVO_WRIST, SERVO_WRIST_FLAT);
    set_servo_position(SERVO_CLAW, SERVO_CLAW_OPEN);
    arm_middle();
    
    //move forward to collect poms
    create_drive_direct(100, 100);
    msleep(500);
    create_stop();
    arm_down();
    servo_slow(SERVO_CLAW, SERVO_CLAW_CLOSED);
    
    //de-initialize
    create_disconnect();
    disable_servos();
    return 0;
}

//all these arm functions move the servo slowly so we can make sure the function ends once the servo has reached it's desired location
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
            msleep(1);
        }
        
        if (dir2*pos2 < dir2*dest2) {
        	pos2 += dir2*1;
            set_servo_position(port2, pos2);
            msleep(1);
        }
    }
}

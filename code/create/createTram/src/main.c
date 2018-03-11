#include <kipr/botball.h>
#include "createDrive.h"
#include "lineFollow.h"

#define GREEN 0
#define RED 1

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
#define SERVO_GATE_CLOSED 550
#define SERVO_GATE_OPEN 2047
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
void open_gate();
void arm_pickup_poms();
int camera_see_poms();

int main()
{
    //start setup

    //initial servo positions
    set_servo_position(SERVO_GATE, SERVO_GATE_CLOSED);
    //set_servo_position(SERVO_LEFT, SERVO_LEFT_UP);
    //set_servo_position(SERVO_RIGHT, SERVO_RIGHT_UP);

    //initialize servos and create and camera
    enable_servos();
    create_connect();
    camera_open_black();
    camera_load_config("red_green");

    //IMPORTANT: might want to uncomment out the two set servo pos's above if the below malfunctions
    arm_up();

    //start arm up, move claw down, slowly move arm down
    servo_slow(SERVO_WRIST, SERVO_WRIST_DOWN);
    arm_middle();
    //end setup

    msleep(2000);

    //square up against right wall and face towards poms
    servo_slow_2(SERVO_LEFT, SERVO_LEFT_MIDDLE+200, SERVO_RIGHT, SERVO_RIGHT_MIDDLE-200);
    create_forward(5, 100);
    create_left(100, 100);
    create_square_up(1300, -100);

    //square up on the black line with the tophat sensors
    while (analog(LEFT_LINE) < 3200 || analog(RIGHT_LINE) < 3200) 
    {
        if (analog(LEFT_LINE) < 3200 && analog(RIGHT_LINE) < 3200) 
        {
            create_drive_direct(100, 100);
        } 
        else if (analog(RIGHT_LINE) < 3200) 
        {
            create_drive_direct(0, 50);
        }
        else if (analog(LEFT_LINE) < 3200) 
        {
            create_drive_direct(50, 0);
        }
    }
    create_stop();

    arm_up();

    //turn in general direction of poms
    set_servo_position(SERVO_GATE, SERVO_GATE_OPEN);
    arm_middle();
    create_right(60, 100);
    int see_poms = camera_see_poms();
    while (see_poms == -1) //while it doesn't see any poms, move forward 
    {
    	create_drive_direct(25, 25);
        see_poms = camera_see_poms();
    }
    while (see_poms != 0) //while poms aren't centered, move until it is centered
    {
    	if (see_poms == 2) //move forward if poms in second quadrant
            create_drive_direct(25, 25);
        else if (see_poms == 4) //move backward if poms in fourth quadrant
            create_drive_direct(-25, -25);
        else if (see_poms == 1) //turn right if poms in first quadrant
            create_drive_direct(25, -25);
        else if (see_poms == 3) //turn left if poms in third quadrant
            create_drive_direct(-25, 25);
            
        see_poms = camera_see_poms();
    }
    create_stop();
    
    
    //OLD
    /*int see_poms = camera_see_poms();
    while (see_poms != 1)  //go until it says we don't need to move anymore
    {
        if (see_poms == 0)
            create_drive_direct(50, -50);
        else if (see_poms == 2)
            create_drive_direct(25, -25);
        else if (see_poms == 3)
            create_drive_direct(-25, 25);

        see_poms = camera_see_poms();
    }*/
    //create_stop();

    //turn back and pick up 3rd group of poms
    //create_right(10, 50);
    arm_pickup_poms();
    create_forward(35, 100);
    push_poms_in();

    disable_servos();
    create_disconnect();
    camera_close();

    return 0;
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
    camera_close();
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

void push_poms_in()
{
    //pushes poms in to the carrier
    set_servo_position(SERVO_GATE, SERVO_GATE_CLOSED);
    msleep(300);
    //set_servo_position(SERVO_GATE, SERVO_GATE_OPEN);
    //msleep(300);
}

void open_gate()
{
    set_servo_position(SERVO_GATE, SERVO_GATE_OPEN);
    msleep(300);
}

void arm_pickup_poms() 
{
    arm_up();
    set_servo_position(SERVO_WRIST, SERVO_WRIST_FLAT);
    set_servo_position(SERVO_GATE, SERVO_GATE_OPEN);
    arm_down();
}

int in_between(int val, int min, int max) 
{
	//checks if a value is inbetween two values, 1=true, 0=false
    if (val > min && val < max) 
    {
    	return 1;
    }
    return 0;
}

int camera_see_poms() 
{
    msleep(1);
    camera_update();

    int i, max_red, max_green, red_count, green_count = 0;
    float tolerance = .5;
    point2 red_center, green_center;
    for (i = 0; i < get_object_count(RED); i++) 
    {
        if (get_object_area(RED, i) > max_red && get_object_confidence(RED, i) > tolerance)
        {
            max_red = get_object_area(RED, i);
            red_center = get_object_center(RED, i);
            red_count++;
        }
    }

    for (i = 0; i < get_object_count(GREEN); i++)
    {
        if (get_object_area(GREEN, i) > max_green && get_object_confidence(GREEN, i) > tolerance)
        {
            max_green = get_object_area(GREEN, i);
            green_center = get_object_center(GREEN, i);
            green_count++;
        }
    }

    if (green_count != 0 || red_count != 0) {
        point2 cur_center; //used to determine which center to use: the one for green or one for red
        if (red_count == 1 || max_red > max_green)
        {
            cur_center = red_center; 
        }
        else if (green_count == 1 || max_green > max_red)
        {
            cur_center = green_center;
        }
        
        int tolerance = 15;
        int camera_center_x = get_camera_width()/2;
        int camera_center_y = get_camera_height()/2;
        if (in_between(cur_center.x, camera_center_x-tolerance, camera_center_x+tolerance) && in_between(cur_center.y, camera_center_y-tolerance, camera_center_y+tolerance))
        	return 0;
        else if (cur_center.x > camera_center_x-tolerance || cur_center.y < camera_center_y-tolerance)
            return 1; //first quadrant
        else if (cur_center.x < camera_center_x-tolerance || cur_center.y < camera_center_y-tolerance)
            return 2; //second quadrant
        else if (cur_center.x < camera_center_x-tolerance || cur_center.y > camera_center_y-tolerance)
            return 3; //third quadrant
        else if (cur_center.x > camera_center_x-tolerance || cur_center.y > camera_center_y-tolerance)
            return 4; //fourth quadrant
    }
	
    return -1;
}

int camera_see_poms2() 
{
    msleep(1);
    //returns 3 if red/green poms are detected and we need to move left, 2 if need to move right, 1 if centered, 0 otherwise
    int i;
    for (i = 0; i < 1; i++)
    {
        camera_update();
    }

    int max_red, max_green = 0;
    point2 red_center, green_center;
    for (i = 0; i < get_object_count(RED); i++) 
    {

        if (get_object_area(RED, i) > max_red)
        {
            max_red = get_object_area(RED, i);
            red_center = get_object_center(RED, i);
        }
        //printf("RED OBJECT SIZE: %d\n", get_object_area(RED, i));

        //printf("Max red: %d. Max green: %d\n", max_red, max_green);
    }

    for (i = 0; i < get_object_count(GREEN); i++)
    {
        if (get_object_area(GREEN, i) > max_green)
        {
            max_green = get_object_area(GREEN, i);
            green_center = get_object_center(GREEN, i);
        }
    }
    printf("Max red: %d. Max green: %d\n", max_red, max_green);
    float dist = sqrt( pow(red_center.x-green_center.x, 2) + pow(red_center.y-green_center.y, 2) );
    printf("DISTANCE BTW RED AND GREEN: %f\n", dist);


    int threshold = 15;
    int cameraCenterX = get_camera_width()/2 - 15;
    if (dist < 50 && max_red > 20 && max_green > 20) {
        //detects red green clump
        if ( (red_center.x > cameraCenterX - threshold && red_center.x < cameraCenterX + threshold) || (green_center.x > cameraCenterX - threshold && green_center.x < cameraCenterX + threshold) )
            return 1;
        else if (red_center.x < cameraCenterX || green_center.x < cameraCenterX - threshold)
            return 2;
        else if (red_center.x > cameraCenterX + threshold || green_center.x > cameraCenterX +threshold)
            return 3;
    }
    return 0;
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

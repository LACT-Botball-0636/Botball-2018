//THIS IS THE CORRECT ONE!!!!

#include <kipr/botball.h>
#include "createDrive.h"

#define GREEN 0
#define RED 1

#define ET 0

#define SERVO_LEFT 3
#define SERVO_RIGHT 2
#define SERVO_GATE 1
#define SERVO_WRIST 0

#define MOTOR_GATE 0
#define MOTOR_GATE_OPEN_DIST 400

#define SERVO_RIGHT_UP 280
#define SERVO_RIGHT_MIDDLE 1321
#define SERVO_RIGHT_DOWN 1530
#define SERVO_LEFT_UP SERVO_LEFT_DOWN+(SERVO_RIGHT_DOWN-SERVO_RIGHT_UP)
#define SERVO_LEFT_MIDDLE SERVO_LEFT_DOWN+(SERVO_RIGHT_DOWN-SERVO_RIGHT_MIDDLE)
#define SERVO_LEFT_DOWN 795
//#define SERVO_LEFT_MIDDLE SERVO_LEFT_UP-(SERVO_RIGHT_MIDDLE-SERVO_RIGHT_UP)
//#define SERVO_LEFT_DOWN SERVO_LEFT_UP-(SERVO_RIGHT_DOWN-SERVO_RIGHT_UP)
#define SERVO_GATE_CLOSED 550
#define SERVO_GATE_OPEN 2047
#define SERVO_WRIST_DUMP 400
#define SERVO_WRIST_FLAT 1750
#define SERVO_WRIST_UP 650


void servo_slow(int port, int dest);
void servo_slow_2(int port1, int dest1, int port2, int dest2);
void move_servo1();
void move_servo2();
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
void arm_pickup_poms();
int camera_see_poms();
void grab_poms();
void setupTophat();
void gate_close();
void gate_open();
void pom_release(); 

//Set up instructions:
//move gate to closed
//create should be facing middle of the board
//arm should be in down
//wrist should be flat(about)
int main()
{
    /*set_servo_position(SERVO_RIGHT, SERVO_RIGHT_DOWN);
    enable_servos();
    create_connect();
    gate_open();
    msleep(1000);
    set_servo_position(SERVO_WRIST, SERVO_WRIST_FLAT);
    msleep(100);
    msleep(100);
    arm_up();
    msleep(5000);
    create_forward(15,100);
    msleep(100);
    gate_close();
    msleep(100);
    arm_up();
    msleep(5000);
    create_forward(15, 100);
    msleep(100);*/
    /*set_servo_position(SERVO_LEFT, SERVO_LEFT_DOWN);
    set_servo_position(SERVO_RIGHT, SERVO_RIGHT_DOWN);
    set_servo_position(SERVO_WRIST, SERVO_WRIST_FLAT);
    enable_servos();
    create_connect();
    arm_middle();
    msleep(100);
    servo_slow(SERVO_WRIST, SERVO_WRIST_UP);
    arm_up();
    msleep(1000);
    create_forward(15,50); //42
    msleep(300);
    create_right(30, 50);
    msleep(300);
    create_line_follow2(27,25);
    gate_open();
    pom_release(); //THIS IS A TEST. DO NOT REMOVE
    pom_release();
    pom_release();
    pom_release();
    msleep(100);
    create_disconnect();
    disable_servos();
    return(0);*/
    //----------start setup----------//
    setupTophat();

    //initial servo positions
    set_servo_position(SERVO_RIGHT, SERVO_RIGHT_DOWN);
    set_servo_position(SERVO_LEFT, SERVO_LEFT_DOWN);

    set_servo_position(SERVO_WRIST, SERVO_WRIST_FLAT);

    //initialize servos and create and camera
    enable_servos();
    create_connect();

    //square up after calibration

    arm_middle();
    create_square_up(2000, -100);

    //start arm up, move claw down, slowly move arm down
    arm_down();

    //----------end setup----------//

    //<INSERT start light HERE>
    msleep(2000);

    //square up against right wall and face towards poms
    arm_middle();
    create_forward(10, 100);
    create_left(100, 100);
    create_square_up(1300, -100);

    //go forward until middle of board then square up on outer wall and the middle line
    create_forward(115, 100);
    create_right(90, 100);
    create_square_up(1500, -100);
    create_forward(10, 100);
    create_square_up_line(50);
    msleep(500);

    //go forward and turn so on the line
    create_forward(20, 100);
    //create_left(90, 100);
    while (analog(RIGHT_LINE) < rightBlack-300)
    {
        create_left(1,50);
        msleep(10);
    }
    while (analog(RIGHT_LINE) > rightWhite+300) 
    {
        create_right(1,50);
        msleep(10);
    }
    msleep(1000);

    //Line follow to the three groups of poms and collect them
    arm_down();
    gate_open();

    create_line_follow(30, 50);
    gate_close();
    msleep(1000);
    gate_open();

    create_line_follow(15, 50);
    gate_close();
    msleep(1000);
    gate_open();
    
    create_line_follow(15, 50);
    gate_close();
    msleep(1000);
    gate_open();

    create_line_follow(15, 50);
    gate_close();
    msleep(1000);
    gate_open();
    
    create_line_follow(15, 50);
    gate_close();
    msleep(1000);
    gate_open();
    
    //create_line_follow(15, 50);
    gate_close();
    msleep(1000);
    //set_servo_position(SERVO_GATE, SERVO_GATE_OPEN);  
	
    //go towards middle of board and line up on middle bump
    arm_up();
    create_right(200, 100);
    create_line_follow(50, 100);
    msleep(100);
    create_drive_direct(50, 50);
    int ms = 0;
    while (ms < 5000) 
    {   
        if (get_create_lbump() || get_create_rbump())
        {
            create_stop();
        	break;
        }
    	msleep(1);
        ms++;
    }

    //going towards tram and dumping after getting the first round of poms
    create_right(90,50);
    msleep(100);
    create_backward(25, 100);
    create_square_up_line(50);
    msleep(100);
    servo_slow(SERVO_WRIST, SERVO_WRIST_UP);
    arm_up();
    msleep(1000);
    create_forward(15,50); //42
    msleep(300);
    create_right(30, 50);
    msleep(300);
    create_line_follow2(27,25);
    gate_open();
    pom_release(); //THIS IS A TEST. DO NOT REMOVE
    pom_release();
    pom_release();
    pom_release();
    msleep(100);
    disable_servos();
    create_disconnect();
    return 0;

}

void setupTophat() 
{
    int accept = 0;
    while (!accept) 
    {
        printf("\n\n------------------------\n\nMove tophat sensors over white area of board.\nPress right button to set.\n");
        while (!right_button()) {}
        leftWhite = analog(LEFT_LINE);
        rightWhite = analog(RIGHT_LINE);
        printf("Left tophat value: %d, Right tophat value: %d\n", leftWhite, rightWhite);
        msleep(1000);
        printf("Press right button to accept, left button to reject.\n");
        while(!right_button() && !left_button()) {}
        if (right_button()) 
        {
            accept = 1;
        }
    }

    msleep(1000);
    accept = 0;
    while (!accept) 
    {
        printf("\n\n------------------------\n\nMove tophat sensors over black area of board.\nPress right button to set.\n");
        while (!right_button()) {}
        leftBlack = analog(LEFT_LINE);
        rightBlack = analog(RIGHT_LINE);
        printf("Left tophat value: %d, Right tophat value: %d\n", leftBlack, rightBlack);
        msleep(1000);
        printf("Press right button to accept, left button to reject.\n");
        while(!right_button() && !left_button()) {}
        if (right_button()) 
        {
            accept = 1;
        }
    }
}

//all these arm functions move the servo slowly so we can make sure the function ends once the servo has reached its desired location
void arm_up()
{
    //servo_slow(SERVO_RIGHT, SERVO_RIGHT_UP);
    servo_slow_2(SERVO_LEFT, SERVO_LEFT_UP, SERVO_RIGHT, SERVO_RIGHT_UP);
}

void arm_down() 
{
    //servo_slow(SERVO_RIGHT, SERVO_RIGHT_DOWN);
    servo_slow_2(SERVO_LEFT, SERVO_LEFT_DOWN, SERVO_RIGHT, SERVO_RIGHT_DOWN);
}

void arm_middle() 
{
    //servo_slow(SERVO_RIGHT, SERVO_RIGHT_MIDDLE);
    servo_slow_2(SERVO_LEFT, SERVO_LEFT_MIDDLE, SERVO_RIGHT, SERVO_RIGHT_MIDDLE);
}

void gate_close()
{
    //pushes poms in to the carrier
    cmpc(MOTOR_GATE);
    motor(MOTOR_GATE, -40);
    while (gmpc(MOTOR_GATE) > -MOTOR_GATE_OPEN_DIST)
    {
        msleep(1);
    }
    off(MOTOR_GATE);
}

void gate_open() 
{
    cmpc(MOTOR_GATE);
    motor(MOTOR_GATE, 40);
    while (gmpc(MOTOR_GATE) < MOTOR_GATE_OPEN_DIST)
    {
        msleep(1);
    }
    off(MOTOR_GATE);
}

void open_gate()
{
    set_servo_position(SERVO_GATE, SERVO_GATE_OPEN);
    msleep(300);
}

void arm_pickup_poms() 
{
    arm_up();
    servo_slow(SERVO_WRIST, SERVO_WRIST_FLAT);
    servo_slow(SERVO_GATE, SERVO_GATE_OPEN);
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
            pos += 1;
            set_servo_position(port, pos);
            msleep(1);
        }
    }
    else
    {
        while (pos > dest) 
        {
            pos -= 1;
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

    void move_servo1() {
        while (dir1*pos1 < dir1*dest1) {
            pos1 += dir1*1;
            set_servo_position(port1, pos1);
            msleep(1);
        }
    }

    void move_servo2() {
        while (dir2*pos2 < dir2*dest2) {
            pos2 += dir2*1;
            set_servo_position(port2, pos2);
            msleep(1);
        }
    }

    thread move_servo1_thread = thread_create(move_servo1);
    thread_start(move_servo1_thread);

    move_servo2();

    thread_wait(move_servo1_thread);
    thread_destroy(move_servo1_thread);

}



void grab_poms()
{
    msleep(3000);
    servo_slow(SERVO_GATE, SERVO_GATE_CLOSED);
    set_servo_position(SERVO_WRIST, SERVO_WRIST_UP);
    msleep(1000);
    servo_slow(SERVO_WRIST, SERVO_WRIST_FLAT);
    servo_slow(SERVO_GATE, SERVO_GATE_OPEN);
}

void pom_release()
{
    servo_slow(SERVO_WRIST, SERVO_WRIST_UP);
    msleep(100);
    servo_slow(SERVO_WRIST, SERVO_WRIST_DUMP);
    msleep(100);
}


#ifndef LINEFOLLOW_H
#define LINEFOLLOW_H

#include <kipr/botball.h>

//MOT_LEFT and MOT_RIGHT must be defined for the lineFollow() function to work
/*
#define MOT_LEFT 0
#define MOT_RIGHT 1
*/

#define LEFT 0
#define RIGHT 1

#define WHITE 250	//Make this value GREATER THAN the sensor reading for white
#define BLACK 2200 	//Make this value LESS than the sensor reading for black
#define AVG WHITE+BLACK/2

void lineFollow(int speed, int ms, int side);

#endif

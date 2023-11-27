/*
 * controller.c
 */

#include "main.h"
#include "controller.h"
#include "pid.h"
#include <stdio.h>


/*
 * We recommend you implement this function so that move(1) will move your rat 1 cell forward.
 */
void move(int8_t n) {
	setPIDGoalD(n * 525);
}

/*
 * We recommend you implement this function so that turn(1) turns your rat 90 degrees in your positive rotation
 * direction and turn(-1) turns the other way.
 */
void turn(int8_t n) {
	setPIDGoalA(n * 500);
}

/*
 * pid.c
 */

#include "main.h"
#include "motors.h"
#include "encoders.h"

const float MAX_A_CORRECTION = 0.4;
const float MAX_D_CORRECTION = 0.5;

int angleError = 0;
int oldAngleError = 0;
float distanceError = 0;
float oldDistanceError = 0;
float angleCorrection = 0;
float distanceCorrection = 0;

int distanceGoal = 0;
int angleGoal = 0;
int ticksElapsed = 0;

float kPw = 0.5;
float kDw = 0.5;

float kPx = 0.5;
float kDx = 0.5;

int done = 0;

void resetPID() {
	angleError = 0;
	oldAngleError = 0;
	distanceError = 0;
	oldDistanceError = 0;
	angleCorrection = 0;
	distanceCorrection = 0;

	distanceGoal = 0;
	angleGoal = 0;
	ticksElapsed = 0;
	resetEncoders();
}

void updatePID() {
	angleError = angleGoal - (getLeftEncoderCounts() - getRightEncoderCounts());
	angleCorrection = kPw * angleError + kDw * (angleError - oldAngleError);
	oldAngleError = angleError;

	distanceError = distanceGoal - (getLeftEncoderCounts() + getRightEncoderCounts()) / 2;
	distanceCorrection = kPx * distanceError + kDx * (distanceError - oldDistanceError);
	oldDistanceError = distanceError;

	limitAngleCorrection();
	limitDistanceCorrection();

	setMotorLPWM(distanceCorrection + angleCorrection);
	setMotorRPWM(distanceCorrection - angleCorrection);

	if(angleError == 0 && distanceError == 0) {
		ticksElapsed++;

		if(ticksElapsed == 50) {
			resetPID();
			done = 1;
		}
	} else {
		ticksElapsed = 0;
	}
}

void limitAngleCorrection() {
	if(angleCorrection >= MAX_A_CORRECTION) {
		angleCorrection = MAX_A_CORRECTION;
	}

	if(angleCorrection <= -MAX_A_CORRECTION) {
		angleCorrection = -MAX_A_CORRECTION;
	}
}

void limitDistanceCorrection() {
	if(distanceCorrection >= MAX_D_CORRECTION) {
		distanceCorrection = MAX_D_CORRECTION;
	}

	if(distanceCorrection <= -MAX_D_CORRECTION) {
		distanceCorrection = -MAX_D_CORRECTION;
	}
}

void setPIDGoalD(int16_t distance) {
	distanceGoal = distance;
	while(done == 0) {}
	done = 0;
}

void setPIDGoalA(int16_t angle) {
	angleGoal = angle;
	while(done == 0) {}
	done = 0;
}

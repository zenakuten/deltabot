
#ifndef DELTA_H
#define DELTA_H
#include "Arduino.h"
//Adapted from http://forums.trossenrobotics.com/tutorials/introduction-129/delta-robot-kinematics-3276/
class Delta 
{	 
	public:
		Delta(float e, float f, float rf, float re);
 
		 // forward kinematics: (theta1, theta2, theta3) -> (x0, y0, z0)
		 // returned status: 0=OK, -1=non-existing position
		int CalcForward(float theta1, float theta2, float theta3, float &x0, float &y0, float &z0);
		// inverse kinematics: (x0, y0, z0) -> (theta1, theta2, theta3)
		// returned status: 0=OK, -1=non-existing position
		int CalcInverse(float x0, float y0, float z0, float &theta1, float &theta2, float &theta3); 
		
	private:
		// inverse kinematics
		// helper functions, calculates angle theta1 (for YZ-pane)
		int CalcAngleYZ(float x0, float y0, float z0, float &theta);

		float e, f, rf, re;
};

#endif

#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           left,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           right,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           arm,           tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port5,           claw,          tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// ****************************************************************************
// ****************************************************************************
// NOTE: NOTE: NOTE:
// The following code is essentially the middle schooler's code re-wrapped into
// the correct competition templates to ensure that it will play well with the
// the VEX field system
//
// Additional advanced changes:
//   All joystick values are scaled to -1.0 to +1.0, squared and then rescaled
//   back to integer range -127 to +127. This makes the joysticks easier to
//   use since the kids don't always have the fine motor control to move a little bit.
//
//   Because of the scaling the built-in arcade drive function could not be
//   used so we copied code from the high school team.
// ****************************************************************************
// ****************************************************************************

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"
#include "NatLang_CORTEX.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
	resetMotorEncoder(arm);

}

/*---------------------------------------------------------------------------*/
/* Arm Control Task - makes arm follow joystick and then holds position      */
/*---------------------------------------------------------------------------*/
task PosArmControl()
{
	float setPoint=270.0 * 7.0;
	float position=0.0;
	float k=2.0;
	float minVal=0;
	float maxVal=270.0 * 7.0;
	float error=0.0;


	for(;;)
	{
		float joyFloat = (float)getJoystickValue(Ch2Xmtr2) / 127.0;
		joyFloat *= fabs(joyFloat);
		int joy = (int)(joyFloat * 127.0);

		long enc = getMotorEncoder(arm);

		if( (abs(joy)>10)
		&&
			 (((setPoint >= maxVal) && (joy<0))
		||  ((setPoint <= minVal) && (joy>0))
		||  ((setPoint > minVal) && (setPoint<maxVal))))
		{
			setPoint=(float)enc/1.742;
		  motor[arm]=-joy;
		}
    else
    {
		  if(setPoint>maxVal)
		  {
		  	setPoint=maxVal;
		  }
		  else if(setPoint<minVal)
		  {
				setPoint=minVal;
			}
			position = (float) enc/1.742;
			error=setPoint-position;
			motor[arm]=-error*k;
		}

		wait1Msec(5);
	}
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
	// ..........................................................................
	// Insert user code here.
	// ..........................................................................

	// We will treat this user control task as the lowest priority
	// thing in the system
	short autonomousPriority = getTaskPriority(autonomous);

	// In RobotC, higher number is higher priority (reversed from popular RTOSes)
	// Make sure our control loop tasks always have high priority than user polling
	// and other background activity
	startTask(PosArmControl,autonomousPriority+1);

	// Just wait for the auto period to stop
	for (;;)
	{
		wait1Msec(1000); // Time here does not really matter, just don't exit until they tell us
	}


}

// Useful code to compare two numbers and return the one that is more positive
float max(float x, float y)
{
	float retval = x;
	if (x < y)
	{
		retval = y;
	}
	return retval;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
	// User control code here, inside the loop

	// We will treat this user control task as the lowest priority
	// thing in the system
	short userControlPriority = getTaskPriority(usercontrol);

	// In RobotC, higher number is higher priority (reversed from popular RTOSes)
	// Make sure our control loop tasks always have high priority than user polling
	// and other background activity
	startTask(PosArmControl,userControlPriority+1);

	for (;;)
	{
			// Read the joysticks
		  int drive = vexRT[Ch3];
		  int turn  = -vexRT[Ch1]; // Z-axis points down so positive joystick needs to be negative turn

		  // The joystick range is -127 to +127
		  // But we want to scale the joystick as -1.0 to +1.0
		  // so we can scale on parabola (y = x*x) so the control
		  // is softer in the middle
		  float driveFloat = (float)drive / 127.0;
		  float turnFloat = (float)turn / 127.0;

		  // Square but keep sign
		  driveFloat *= fabs(driveFloat);
		  turnFloat *= fabs(turnFloat);

			// Now we just implement the arcade logic (similar to the WPI library
			// used on the FRC robot)
			float leftMotorSpeed = 0.0;
			float rightMotorSpeed = 0.0;
			if (driveFloat > 0.0)
			{
				if (turnFloat > 0.0)
				{
					leftMotorSpeed = driveFloat - turnFloat;
					rightMotorSpeed = max(driveFloat, turnFloat);
				}
				else
				{
					leftMotorSpeed = max(driveFloat, -turnFloat);
					rightMotorSpeed = driveFloat + turnFloat;
				}
			}
			else
			{
				if (turnFloat > 0.0)
				{
					leftMotorSpeed = -max(-driveFloat, turnFloat);
					rightMotorSpeed = driveFloat + turnFloat;
				}
				else
				{
					leftMotorSpeed = driveFloat - turnFloat;
					rightMotorSpeed = -max(-driveFloat, -turnFloat);
				}
			}

		  // Now rescale the left and right motors to the integers
			// the motors need (it is also -127 to +127)
			int leftMotorCommand = (int)(leftMotorSpeed * 127.0);
			int rightMotorCommand = (int)(rightMotorSpeed * 127.0);

			hogCPU();
			motor[left] = leftMotorCommand;
			motor[right] = rightMotorCommand;
			releaseCPU();

			// This button controllers the motor on the claw
			buttonControl(claw, Btn5UXmtr2, Btn5DXmtr2, 120);

			wait1Msec(5);
	}
}

#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           RightWithEnc,  tmotorVex393_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port3,           Right,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           LeftWithEnc,   tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port5,           Left,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           MobileManipLeft, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           MobileManipRight, tmotorVex393_MC29, openLoop, encoderPort, I2C_3)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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
#include "DriverInterface.c" //equivalent to java class interaction
#include "Goal_Interface.c"
#include "Cone_Lifter_Interface.c"
#include "Cone_Manipulator_Interface.c"
#include "Utilities.h"

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

	/// MOK: Add motor slaves here and pass only the master motor ID to
	/// and interface for each subsystem control loop or state machine
	/// http://help.robotc.net/WebHelpVEX/index.htm#Resources/topics/VEX_Cortex/ROBOTC/IME_Commands/slaveMotor.htm
	slaveMotor(Left, LeftWithEnc);
	slaveMotor(Right, RightWithEnc);
	slaveMotor(MobileManipRight, MobileManipLeft);
	initializeDriveMotors(LeftWithEnc, RightWithEnc);
	initializeGoalLifterMotors(MobileManipLeft, GOAL_ENCODER_SCALE_TICKS_PER_DEG);
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

	// Remove this function call once you have "real" code.
	AutonomousCodePlaceholderForTesting();
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
	startTask(driveControlTask, userControlPriority + 1);
	startTask(goalLifterControlTask);

	// Don't stop the usercontrol task, just loop forever
	// displaying status and time... this is our low priority
	// status loop
	for EVER
	{
		displayStatusAndTime();

		// There is no need to run this more than once per second
		wait1Msec(1000);
	}
}

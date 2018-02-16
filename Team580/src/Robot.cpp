#include <wpilib.h>
#include <IterativeRobot.h>
#include <Joystick.h>
#include <Buttons/JoystickButton.h>
#include <LiveWindow/LiveWindow.h>
#include <RobotDrive.h>
#include <Timer.h>

class Robot: public frc::IterativeRobot {
public:
	Robot(): myRobot(kFrontLeftChannel, kRearLeftChannel, kFrontRightChannel, kRearRightChannel),
	stick(0),
	lw(frc::LiveWindow::GetInstance()),
	timer(),
	leftGrabber(4), // port 4
	rightGrabber(5) // port 5
{
		myRobot.SetExpiration(0.1);
		timer.Start();
}

private:
	frc::RobotDrive myRobot;
	frc::Joystick stick;
	frc::LiveWindow* lw;
	frc::Timer timer;
	frc::Talon leftGrabber; // cube intake left motor
	frc::Talon rightGrabber; // cube intake right motor
	static constexpr int kFrontLeftChannel = 2;
	static constexpr int kRearLeftChannel = 3;
	static constexpr int kFrontRightChannel = 1;
	static constexpr int kRearRightChannel = 0;
	bool buttonEnabled = false; // checks if the cube intake motors are on

	void AutonomousInit() override {
		timer.Reset();
		timer.Start();
		//FMS data
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
	}

	void AutonomousPeriodic() override {
		if (timer.Get() < 2.0) {
			myRobot.MecanumDrive_Cartesian((0.0),(0.5),(0.0));
			//Percentage of Power along a specific Axis
			//X = left & right, Y = forward & backward, Twist = rotate
		} else {
			myRobot.MecanumDrive_Cartesian((0.0),(0.0),(0.5));
		}
	}

	void TeleopInit() override {

	}

	void TeleopPeriodic() override {
		//myRobot.ArcadeDrive(stick);
		myRobot.MecanumDrive_Cartesian(stick.GetX(), stick.GetY(), stick.GetTwist());

		// cube intake motor's "on" button
		if(stick.GetRawButton(7)) {
			buttonEnabled = true;
		}
		// "off" button
		else if(stick.GetRawButton(9)) {
			buttonEnabled = false;
		}

		// Set to joystick slider's manual control (-1.0 - +1.0)
		if(buttonEnabled) {
			leftGrabber.Set(stick.GetThrottle());
			rightGrabber.Set(stick.GetThrottle());
		} else { // otherwise, set the speed to 0 (turn off the motors)
			leftGrabber.Set(0);
			rightGrabber.Set(0);
		}
	}

	void TestPeriodic() override {
		lw->Run();
	}
};
START_ROBOT_CLASS(Robot)

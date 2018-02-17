#include <wpilib.h>
#include <IterativeRobot.h>
#include <Joystick.h>
#include <Buttons/JoystickButton.h>
#include <LiveWindow/LiveWindow.h>
#include <RobotDrive.h>
#include <Timer.h>

class Robot: public frc::IterativeRobot {
public:
	Robot(): myRobot(frontLeft, rearLeft, frontRight, rearRight),
	stick(0),
	lw(frc::LiveWindow::GetInstance()),
	timer(),
	frontLeft(2),
	rearLeft(3),
	frontRight(1),
	rearRight(0),
	leftGrabber(4), // port 4
	rightGrabber(5) // port 5
{
		myRobot.SetExpiration(0.1);
		timer.Start();
		//frontLeft.SetInverted(true);
		//rearLeft.SetInverted(true);
}

private:
	frc::RobotDrive myRobot;
	frc::Joystick stick;
	frc::LiveWindow* lw;
	frc::Timer timer;
	frc::Talon leftGrabber; // cube intake left motor
	frc::Talon rightGrabber; // cube intake right motor
	frc::Talon frontLeft;
	frc::Talon rearLeft;
	frc::Talon frontRight;
	frc::Talon rearRight;
	// static constexpr int kFrontLeftChannel = 2;
	// static constexpr int kRearLeftChannel = 3;
	// static constexpr int kFrontRightChannel = 1;
	// static constexpr int kRearRightChannel = 0;
	bool buttonEnabled = false; // checks if the cube intake motors are on
	int station = frc::DriverStation::GetInstance().GetLocation(); // returns 1-3, representing driver station number
	std::string gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage(); // is the switch in the left or right position?

	void AutonomousInit() override {
		timer.Reset();
		timer.Start();
	}

	//TODO: Calculate measurements!
	// X = left & right, Y = forward & backward, Twist = rotate
	// myRobot.MecanumDrive_Cartesian(X, Y, Twist)
	void AutonomousPeriodic() override {
		if(gameData[0] == 'L') { // left switch
			if(station == 1) { // left driver station
				if(timer.Get() < 4.0) {
					myRobot.MecanumDrive_Cartesian((0.0),(0.5),(0.0));
				} else {
					myRobot.MecanumDrive_Cartesian((0.0),(0.0),(0.0));

				}
			} else if(station == 2) { // middle driver station
				if(timer.Get() < 2.0) {
					myRobot.MecanumDrive_Cartesian((0.0),(0.5),(0.0));
				} else {
					myRobot.MecanumDrive_Cartesian((0.0),(0.0),(0.0));
				}
			} else if(station == 3) { // right driver station
				if(timer.Get() < 3.0) {
					myRobot.MecanumDrive_Cartesian((0.0),(0.5),(0.0));
				} else {
					myRobot.MecanumDrive_Cartesian((0.0),(0.0),(0.0));
				}
			}
		} else if(gameData[0] == 'R') { // right switch
			if(station == 1) { // left driver station
				if(timer.Get() < 4.0) {
					myRobot.MecanumDrive_Cartesian((0.0),(0.5),(0.0));
				} else {
					myRobot.MecanumDrive_Cartesian((0.0),(0.0),(0.0));
				}
			} else if(station == 2) { // middle driver station
				if(timer.Get() < 2.0) {
					myRobot.MecanumDrive_Cartesian((0.0),(0.5),(0.0));
				} else {
					myRobot.MecanumDrive_Cartesian((0.0),(0.0),(0.0));
				}
			} else if(station == 3) { // right driver station
				if(timer.Get() < 3.0) {
					myRobot.MecanumDrive_Cartesian((0.0),(0.5),(0.0));
				} else {
					myRobot.MecanumDrive_Cartesian((0.0),(0.0),(0.0));
				}
			}
		}
	}

	void TeleopInit() override {

	}

	void TeleopPeriodic() override {
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

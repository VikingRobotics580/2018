#include <IterativeRobot.h>
#include <Joystick.h>
#include <Buttons/JoystickButton.h>
#include <LiveWindow/LiveWindow.h>
#include <RobotDrive.h>
#include <Timer.h> // System directory

class Robot: public frc::IterativeRobot {
public:
	Robot(): myRobot(kFrontLeftChannel, kRearLeftChannel, kFrontRightChannel, kRearRightChannel),
             stick(0),
			 lw(frc::LiveWindow::GetInstance()),
			 timer(),
			 button1(&stick, 0) //address to stick, id: 0
	{
		myRobot.SetExpiration(0.1);
		timer.Start();
	}

private:
	frc::RobotDrive myRobot;  // Robot drive system
	frc::Joystick stick;         // Only joystick
	frc::LiveWindow* lw;
	frc::Timer timer;
	frc::JoystickButton button1;

	static constexpr int kFrontLeftChannel = 2;
	static constexpr int kRearLeftChannel = 3;
	static constexpr int kFrontRightChannel = 1;
	static constexpr int kRearRightChannel = 0;


	void AutonomousInit() override {
		timer.Reset();
		timer.Start();
	}

	void AutonomousPeriodic() override {
		// Drive for 2 seconds
		if (timer.Get() < 2.0) {
			myRobot.MecanumDrive_Cartesian((0.0),(0.5),(0.0));
			//Percentage of Power along a specific Axis
			//X - Left & Right, Y - Forward & Back, Twist - Rotate
		} else {
			myRobot.MecanumDrive_Cartesian((0.0),(0.0),(0.5));
		}
	}

	void TeleopInit() override {

	}

	void TeleopPeriodic() override {
		// Drive with arcade style (use right stick)
		//myRobot.ArcadeDrive(stick);
		myRobot.MecanumDrive_Cartesian(stick.GetX(), stick.GetY(), stick.GetTwist());
	}

	void TestPeriodic() override {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)

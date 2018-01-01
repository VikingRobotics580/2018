#include <IterativeRobot.h>
#include <Joystick.h>
#include <Buttons/JoystickButton.h>
#include <LiveWindow/LiveWindow.h>
#include <RobotDrive.h>
#include <Timer.h>

class Robot: public frc::IterativeRobot {
public:
	Robot(): myRobot(0, 1),
             stick(0),
			 lw(frc::LiveWindow::GetInstance()),
			 timer(),
			 button1(&stick, 0)
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

	void AutonomousInit() override {
		timer.Reset();
		timer.Start();
	}

	void AutonomousPeriodic() override {
		// Drive for 2 seconds
		if (timer.Get() < 2.0) {
			myRobot.Drive(-0.5, 0.0);  // Drive forwards half speed
		} else {
			myRobot.Drive(0.0, 0.0);  // Stop robot
		}
	}

	void TeleopInit() override {

	}

	void TeleopPeriodic() override {
		// Drive with arcade style (use right stick)
		myRobot.ArcadeDrive(stick);
	}

	void TestPeriodic() override {
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)

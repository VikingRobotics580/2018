#include <WPILib.h>
class Robot: public IterativeRobot {
public:
	Robot():
		IterativeRobot(),
		timer(),
		frontLeft(0),
		backLeft(1),
		frontLeft(2),
		backRight(3),
		driveStation(DriverStation::GetInstance())
{
		stick = new Joystick(0);
		myRobot = NULL;
		timer.Start();
}

	void RobotInit(){
		myRobot = new MecanumDrive(frontLeft,backLeft,frontRight,backRight);
		myRobot -> SetSafetyEnabled(false);
	}

	void AutonomousInit() override {
		timer.Reset();
		timer.Start();
		// Read in FMS data
		// gameData = DriverStation::GetInstance().GetGameSpecificMessage();

		// Get station number: 1, 2 or 3
		// driveStation -> GetInstance().GetLocation();
	}

	//Autonomous skeleton
	/*
	//Determines field switch placements: left or right?
    void getSwitch() {
        if (gameData[0] == 'L') {
            if (gameData[1] == 'L') { //LL

            } else if (gameData[1] == 'R') { //LR

            }
        } else if (gameData[0] == 'R') {
            if (gameData[1] == 'R') { //RR

            } else if (gameData[1] == 'L') { //RL

            }
        }
    }
	 */

	void AutonomousPeriodic() override {
		if (timer.Get() < 2.0) {
			myRobot->DriveCartesian((0.0), (0.5), (0.0));
			// Percentage of power along a specific axis
			// X - left & right, Y - forward & back, Twist - rotate
		} else {
			myRobot->DriveCartesian((0.0), (0.0), (0.5));
		}
	}

	void TeleopInit() override {

	}

	void TeleopPeriodic() override {
		// Drive with arcade style (use right stick)
		// myRobot.ArcadeDrive(stick);
		myRobot->DriveCartesian(stick->GetX(), stick->GetY(), stick->GetTwist());
	}

	void TestPeriodic() override {

	}

private:
	MecanumDrive* myRobot; // Robot drive system
	Timer timer;
	Talon frontLeft; // front left wheel
	Talon backLeft; // back left wheel
	Talon frontRight; // front right wheel
	Talon backRight; // back right wheel
	Joystick* stick;
	std::string gameData; // FMS data
	DriverStation& driveStation;
	static constexpr int kFrontLeftChannel = 2;
	static constexpr int kRearLeftChannel = 3;
	static constexpr int kFrontRightChannel = 1;
	static constexpr int kRearRightChannel = 0;
};
START_ROBOT_CLASS(Robot)

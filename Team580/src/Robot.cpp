#include <Talon.h>
#include <Timer.h>
#include <DriverStation.h>
#include <Drive/MecanumDrive.h>
#include <IterativeRobot.h>
#include <Joystick.h>
#include <Spark.h>
#include <DigitalInput.h>
class Robot : public frc::IterativeRobot {
public:
	Robot():
		timer(),
		leftGrabber(4),
		rightGrabber(5),
		leftClimber(6),
		rightClimber(7),
		fluxCapacitor(8),
		winch(9),
		// DIO ports
		input1(0),
		input2(1),
		input3(2)
{
}
	void RobotInit() {
		robotDrive.SetSafetyEnabled(false);
	}

	// Robot moves forward at a rate of power that is passed in
	void moveForward(double speed) {
		robotDrive.DriveCartesian((0.0),(speed),(0.0));
	}

	// Rotates right at 50% power
	void rotateRight() {
		robotDrive.DriveCartesian((0.0),(0.0),(-0.5));
	}

	// Rotates left at 50% power
	void rotateLeft() {
		robotDrive.DriveCartesian((0.0),(0.0),(0.5));
	}

	// Intake grabbers spin at 100% power and flux opens at 50% power
	void shootCube() {
		leftGrabber.Set(-1);
		rightGrabber.Set(1);
		fluxCapacitor.Set(0.5);
	}

	// Grabber motors and flux turn off
	void stopShooter() {
		leftGrabber.Set(0);
		rightGrabber.Set(0);
		fluxCapacitor.Set(0);
	}

	// All wheel motors turn off
	void motionless() {
		robotDrive.DriveCartesian((0.0),(0.0),(0.0));
	}

	void AutonomousInit() override {
		timer.Reset();
		timer.Start();
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage(); // left or right switch?

		if(gameData[0] == 'L') { // checks if switch is left
			leftSwitch = true;
		}

		if(input1.Get()) { // 100
			leftPos = true;
		} else if(input2.Get()) { // 010
			middlePos = true;
		} else if(input3.Get()) { // 001
			rightPos = true;
		}
	}

	void AutonomousPeriodic() override {
		if(gameData.length() > 0) {
			if(leftSwitch) {	// Left switch logic
				if(leftPos) {
					if(timer.Get() < 3.0) {
						moveForward(-0.5);
					} else if(timer.Get() > 4 && timer.Get() < 4.75) {
						rotateRight();
					} else if(timer.Get() > 5.5 && timer.Get() < 6.5) {
						moveForward(-0.4);
					} else if(timer.Get() > 7 && timer.Get() < 9) {
						shootCube();
					} else {
						motionless();
						stopShooter();
					}
				} else if(middlePos) {
					if(timer.Get() < 1.0) {
						moveForward(-0.5);
					} else if(timer.Get() > 1.25 && timer.Get() < 2) {
						rotateLeft();
					} else if(timer.Get() > 2.5 && timer.Get() < 3.5) {
						moveForward(-0.5);
					} else if(timer.Get() > 4 && timer.Get() < 4.75) {
						rotateRight();
					} else if(timer.Get() > 5 && timer.Get() < 6) {
						moveForward(-0.4);
					} else if(timer.Get() > 6 && timer.Get() < 8) {
						shootCube();
					} else {
						motionless();
						stopShooter();
					}
				} else if(rightPos) {
					if(timer.Get() < 3) {
						moveForward(-0.5);
					} else {
						motionless();
					}
				}
			} else { // leftSwitch = false
				if(leftPos) {
					if(timer.Get() < 3) {
						moveForward(-0.5);
					} else {
						motionless();
					}
				} else if(middlePos) {
					if(timer.Get() < 1) {
						moveForward(-0.5);
					} else if(timer.Get() > 1.25 && timer.Get() < 2) {
						rotateRight();
					} else if(timer.Get() > 2.5 && timer.Get() < 3.5) {
						moveForward(-0.5);
					} else if(timer.Get() > 4 && timer.Get() < 4.75) {
						rotateLeft();
					} else if(timer.Get() > 5 && timer.Get() < 6) {
						moveForward(-0.4);
					} else if(timer.Get() > 6 && timer.Get() < 8) {
						shootCube();
					} else {
						motionless();
						stopShooter();
					}
				} else if(rightPos) {
					if(timer.Get() < 3) {
						moveForward(-0.5);
					} else if(timer.Get() > 4 && timer.Get() < 4.75) {
						rotateLeft();
					} else if(timer.Get() > 5.5 && timer.Get() < 6.5) {
						moveForward(-0.4);
					} else if(timer.Get() > 7 && timer.Get() < 9) {
						shootCube();
					} else {
						motionless();
						stopShooter();
					}
				}
			}
		}
	}

	void TeleopInit() override {
		//m_robotDrive.DriveCartesian(rightJoystick.GetX(), rightJoystick.GetY(), -(rightJoystick.GetZ()));
	}

	void TeleopPeriodic() override {
		robotDrive.DriveCartesian(rightJoystick.GetX(), rightJoystick.GetY(), -(rightJoystick.GetZ()));

		if(leftJoystick.GetRawButton(9)) { // open cube grabber
			fluxCapacitor.Set(0.5);
		} else if(leftJoystick.GetRawButton(10)) { // close cube grabber
			fluxCapacitor.Set(-0.5);
		} else { // flux motor off
			fluxCapacitor.Set(0);
		}

		if(leftJoystick.GetRawButton(2)) { // suck in cube
			leftGrabber.Set(0.5);
			rightGrabber.Set(-0.5);
		} else if(leftJoystick.GetRawButton(1)) { // shoot out cube
			leftGrabber.Set(-1);
			rightGrabber.Set(1);
		} else { // grabber motors off
			leftGrabber.Set(0);
			rightGrabber.Set(0);
		}

		if(rightJoystick.GetRawButton(11)) { // climb down
			rightClimber.Set(1);
			leftClimber.Set(1);
		} else if(rightJoystick.GetRawButton(12)) { // climb up
			rightClimber.Set(-1);
			leftClimber.Set(-1);
		} else if(rightJoystick.GetRawButton(10)) { // climber motor off
			rightClimber.Set(0);
			leftClimber.Set(0);
		}

		if(leftJoystick.GetRawButton(3)) { // winch climb up
			winch.Set(1);
		} else if(leftJoystick.GetRawButton(4)) { // winch climb down
			winch.Set(-1);
		} else { // winch motor off
			winch.Set(0);
		}
	}

private:
	static constexpr int frontLeftChannel = 0;
	static constexpr int rearLeftChannel = 1;
	static constexpr int frontRightChannel = 2;
	static constexpr int rearRightChannel = 3;
	static constexpr int joyChannel = 0;
	static constexpr int joyChannel2 = 1;
	frc::Spark frontLeft{frontLeftChannel};
	frc::Spark rearLeft{rearLeftChannel};
	frc::Spark frontRight{frontRightChannel};
	frc::Spark rearRight{rearRightChannel};
	frc::MecanumDrive robotDrive{frontLeft, rearLeft, frontRight, rearRight};
	frc::Joystick leftJoystick{joyChannel};
	frc::Joystick rightJoystick{joyChannel2};
	frc::Talon leftClimber;
	frc::Talon rightClimber;
	frc::Talon leftGrabber;
	frc::Talon rightGrabber;
	frc::Talon fluxCapacitor; // Back to the Future easter egg - controls the grabber's width
	frc::Talon winch;
	frc::Timer timer;
	std::string gameData;
	DigitalInput input1;
	DigitalInput input2;
	DigitalInput input3;
	bool leftSwitch = false;
	bool leftPos = false;
	bool middlePos = false;
	bool rightPos = false;
};
START_ROBOT_CLASS(Robot)

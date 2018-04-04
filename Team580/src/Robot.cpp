#include <Talon.h>
#include <Timer.h>
#include <DriverStation.h>
#include <Drive/MecanumDrive.h>
#include <IterativeRobot.h>
#include <Joystick.h>
#include <Spark.h>
//#include <ADXRS450_Gyro.h>

class Robot : public frc::IterativeRobot {
public:
	Robot():
		timer(),
		leftGrabber(4),
		rightGrabber(5),
		leftClimber(6),
		rightClimber(7),
		fluxCapacitor(8),
		winch(9)
{
}
	void moveForward(double speed) {
		m_robotDrive.DriveCartesian((0.0),(speed),(0.0));
	}

	void rotateRight() {
		m_robotDrive.DriveCartesian((0.0),(0.0),(-0.5));
	}

	void rotateLeft() {
		m_robotDrive.DriveCartesian((0.0),(0.0),(0.5));
	}

	void shootCube() {
		leftGrabber.Set(-1);
		rightGrabber.Set(1);
		fluxCapacitor.Set(0.5);
	}

	void stopShooter() {
		leftGrabber.Set(0);
		rightGrabber.Set(0);
	}

	void motionless() {
		m_robotDrive.DriveCartesian((0.0),(0.0),(0.0));
	}

	void RobotInit() {
		m_robotDrive.SetSafetyEnabled(false);
	}

	void AutonomousInit() override {
		timer.Reset();
		timer.Start();
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage(); // left or right switch?
	}

	void AutonomousPeriodic() override {
		station = 1; // test purposes
		if(gameData.length() > 0) {
			if(gameData[0] == 'L') { // left scale
				if(station == 1) { // left station
					if(timer.Get() < 3.0) {
						moveForward(-0.5);
						//winch.Set(0.35f); //4" @ .75 per second, 5.5" @ 1.0 per second
					} else if(timer.Get() > 4.0 && timer.Get() < 4.75) {
						rotateRight();
					} else if(timer.Get() > 5.5 && timer.Get() < 6.5) {
						moveForward(-0.4);
					} else if(timer.Get() > 7.0 && timer.Get() < 9.0) {
						shootCube();
					} else {
						motionless();
					}
				} else if(station == 2) { // middle station
					if(timer.Get() < 1.0) {
						moveForward(-0.5);
					} else if(timer.Get() > 1.25 && timer.Get() < 2.0) {
						rotateLeft();
					} else if(timer.Get() > 2.5 && timer.Get() < 3.5) {
						moveForward(-0.5);
					} else if(timer.Get() > 4.0 && timer.Get() < 4.75) {
						rotateRight();
					} else if(timer.Get() > 5.0 && timer.Get() < 6.0) {
						moveForward(-0.4);
					} else if(timer.Get() > 6.0 && timer.Get() < 8.0) {
						shootCube();
					} else {
						motionless();
					}

				} else if(station == 3) { // right station
					if(timer.Get() < 3.0) {
						moveForward(-0.5);
					} else {
						motionless();
					}
				}

			} else if(gameData[0] == 'R') { // right scale
				if(station == 1) { // left switch
					if(timer.Get() < 3.0) {
						moveForward(-0.5);
					} else {
						motionless();
					}

				} else if(station == 2) { // middle station
					if(timer.Get() < 1.0) {
						moveForward(-0.5);
					} else if(timer.Get() > 1.25 && timer.Get() < 2.0) {
						rotateRight();
					} else if(timer.Get() > 2.5 && timer.Get() < 3.5) {
						moveForward(-0.5);
					} else if(timer.Get() > 4.0 && timer.Get() < 4.75) {
						rotateLeft();
					} else if(timer.Get() > 5.0 && timer.Get() < 6.0) {
						moveForward(-0.4);
					} else if(timer.Get() > 6.0 && timer.Get() < 8.0) {
						shootCube();
					} else {
						motionless();
					}

				} else if(station == 3) { // right station
					if(timer.Get() < 3.0) {
						moveForward(-0.5);
						//winch.Set(0.35f); //4" @ .75 per second, 5.5" @ 1.0 per second
					} else if(timer.Get() > 4.0 && timer.Get() < 4.75) {
						rotateLeft();
					} else if(timer.Get() > 5.5 && timer.Get() < 6.5) {
						moveForward(-0.4);
					} else if(timer.Get() > 7.0 && timer.Get() < 9.0) {
						shootCube();
					} else {
						motionless();
					}
				}
			}
		}
	}

	void TeleopInit() override {
		//m_robotDrive.DriveCartesian(rightJoystick.GetX(), rightJoystick.GetY(), -(rightJoystick.GetZ()));
	}

	void TeleopPeriodic() override {
		// Use the joystick X axis for lateral movement, Y axis for forward movement, and Z axis for rotation.
		m_robotDrive.DriveCartesian(rightJoystick.GetX(), rightJoystick.GetY(), -(rightJoystick.GetZ()));

		if(leftJoystick.GetRawButton(9)) { // open cube grabber
			fluxCapacitor.Set(0.5);
		} else if(leftJoystick.GetRawButton(10)) { // close cube grabber
			fluxCapacitor.Set(-0.5);
		} else {
			fluxCapacitor.Set(0);
		}

		if(leftJoystick.GetRawButton(2)) { // "suck in" cube
			leftGrabber.Set(1);
			rightGrabber.Set(-1);
		} else if(leftJoystick.GetRawButton(1)) { // shoot out cube
			leftGrabber.Set(-1);
			rightGrabber.Set(1);
		} else { // turn off motors
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
			winch.Set(0.0f);
		}
	}

private:
	static constexpr int kFrontLeftChannel = 0;
	static constexpr int kRearLeftChannel = 1;
	static constexpr int kFrontRightChannel = 2;
	static constexpr int kRearRightChannel = 3;
	static constexpr int joyChannel = 0;
	static constexpr int joyChannel2 = 1;
	frc::Spark m_frontLeft{kFrontLeftChannel};
	frc::Spark m_rearLeft{kRearLeftChannel};
	frc::Spark m_frontRight{kFrontRightChannel};
	frc::Spark m_rearRight{kRearRightChannel};
	frc::MecanumDrive m_robotDrive{m_frontLeft, m_rearLeft, m_frontRight, m_rearRight};
	frc::Joystick leftJoystick{joyChannel};
	frc::Joystick rightJoystick{joyChannel2};
	frc::Talon leftClimber;
	frc::Talon rightClimber;
	frc::Talon leftGrabber;
	frc::Talon rightGrabber;
	frc::Talon fluxCapacitor;
	frc::Talon winch;
	frc::Timer timer;
	int station;
	std::string gameData;
	//ADXRS450_Gyro gyro;
};
START_ROBOT_CLASS(Robot)

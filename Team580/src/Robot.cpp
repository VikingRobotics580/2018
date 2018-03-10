#include <Talon.h>
#include <Timer.h>
#include <DriverStation.h>
#include <Drive/MecanumDrive.h>
#include <IterativeRobot.h>
#include <Joystick.h>
#include <Spark.h>

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
	void RobotInit() {
		// Invert the left side motors
		m_frontLeft.SetInverted(true);
		m_rearLeft.SetInverted(true);
		m_robotDrive.SetSafetyEnabled(false);
	}

	void AutonomousInit() override {
		timer.Reset();
		timer.Start();
	}

	//TODO: Calculate measurements!
	// X = left & right, Y = forward & backward, rotation = Z value
	// m_robotDrive.DriveCartesian(X, Y, rotation)
	void AutonomousPeriodic() override {
		std::string gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage(); // left or right switch?
		int station = frc::DriverStation::GetInstance().GetLocation(); // returns 1-3, representing driver station number

		if(gameData.length() > 0) {
			if(gameData[0] == 'L') { // left switch
				if(timer.Get() < 1.0) {
					m_robotDrive.DriveCartesian((0.0),(-0.5),(0.0));
				} else {
					m_robotDrive.DriveCartesian((0.0),(0.0),(0.0));
				}
			} else if(gameData[0] == 'R') { // right switch
				if(timer.Get() < 2.0) {
					m_robotDrive.DriveCartesian((0.0),(-0.25),(0.0));
				} else {
					m_robotDrive.DriveCartesian((0.0),(0.0),(0.0));
				}
			}
		}
		/*
		        if(gameData[0] == 'L') { // left switch
		            if(station == 1) { // left driver station
		                if(timer.Get() < 4.0) {
		                    myRobot->MecanumDrive_Cartesian((0.0),(0.5),(0.0));
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
		        } */
	}

	void TeleopInit() override {

	}

	void TeleopPeriodic() override {
		// Z axis is flipped so the direction it is tilted in is also the same direction as the robot's movement
		m_robotDrive.DriveCartesian(rightJoystick.GetX(), rightJoystick.GetY(), -(rightJoystick.GetZ()));

		if(leftJoystick.GetRawButton(1)) { // open cube grabber
			fluxCapacitor.Set(0.5);
		} else if(leftJoystick.GetRawButton(2)) { // close cube grabber
			fluxCapacitor.Set(-0.5);
		} else {
			fluxCapacitor.Set(0);
		}

		if(leftJoystick.GetRawButton(5)) { // shoot out cube
			leftGrabber.Set(1);
			rightGrabber.Set(-1);
		} else if(leftJoystick.GetRawButton(6)) { // receive cube
			leftGrabber.Set(-1);
			rightGrabber.Set(1);
		} else { // turn off motors
			leftGrabber.Set(0);
			rightGrabber.Set(0);
		}

		/* if(m_stick.GetRawButton(7)) { // cube intake motor's "on" button
			buttonEnabled = true;
		} else if(m_stick.GetRawButton(9)) { // "off" button
			buttonEnabled = false;
		}

		if(buttonEnabled) { // Set to joystick slider's manual control (-1.0 - +1.0)
			leftGrabber.Set(m_stick.GetThrottle());
			rightGrabber.Set(m_stick.GetThrottle());
		} else { // turn off the motors
			leftGrabber.Set(0);
			rightGrabber.Set(0);
		} */

		if(rightJoystick.GetRawButton(11)) { // climber down
			rightClimber.Set(0.5);
			leftClimber.Set(0.5);
		} else if(rightJoystick.GetRawButton(12)) { // climber up
			rightClimber.Set(-0.5);
			leftClimber.Set(-0.5);
		} else if(rightJoystick.GetRawButton(10)) { // climber motor off
			rightClimber.Set(0);
			leftClimber.Set(0);
		}

		if(leftJoystick.GetRawButton(3)) { // Winch climb up
			winch.Set(0.75f);
		} else if(leftJoystick.GetRawButton(4)) { // Winch climb down
			winch.Set(-0.75f);
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
	bool buttonEnabled = false;
};

START_ROBOT_CLASS(Robot)

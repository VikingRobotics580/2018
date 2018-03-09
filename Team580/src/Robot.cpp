#include <DriverStation.h>
#include <Drive/MecanumDrive.h>
#include <IterativeRobot.h>
#include <Joystick.h>
#include <Spark.h>
#include <Talon.h>
#include <Timer.h>
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
		frontLeft.SetInverted(true);
		rearLeft.SetInverted(true);
		robotDrive.SetSafetyEnabled(false);
	}
	void AutonomousInit() override {
		timer.Reset();
		timer.Start();
	}

	//TODO: Calculate autonomous measurements!
	/*
	 * X = left & right, Y = forward & backward, rotation = Z value
	 * robotDrive.DriveCartesian(X, Y, rotation)
	 */
	void AutonomousPeriodic() override {
		std::string gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage(); // left or right switch?
		int station = 1; // for now, we will manually change the station number for testing purposes
		//frc::DriverStation::GetInstance().GetLocation(); // returns 1-3, representing driver station number

		//TODO: start raising climbing motors at autonomous' start!

		if(gameData.length() > 0) {
			if(gameData[0] == 'L') { // left switch
				if(station == 1) { // leftmost driver station; aim for first switch!
					//pass auto line
					//turn to face switch
					//place cube using motors
				} else if(station == 2) { // middle driver station
					//pass auto line
					//turn left
					//place cube using motors
				}
				else if(station == 3) { // farthest driver station from switch; aim for scale!
					// it's too complicated to reach the switch, so we'll try the scale if it's on our station's same side
					/*
                     if(gameData[1] == 'R') {
                         // drive forward to scale
                         // place cube
                     } else { // cross auto line
                         if(timer.Get() < 2.0) {
                             robotDrive.DriveCartesian((0.0),(-0.75),(0.0));
                         } else {
                             robotDrive.DriveCartesian((0.0),(0.0),(0.0));
                     }
					 */
				}
				/*
				 * USE FOR FMS FAILURE ONLY (crosses auto line so we can get ranking point)
				 *  if(timer.Get() < 2.0) {
                        robotDrive.DriveCartesian((0.0),(-0.75),(0.0));
                    } else {
                        robotDrive.DriveCartesian((0.0),(0.0),(0.0));
                    }
				 */
			} else if(gameData[0] == 'R') { // right switch
				if(station == 1) { // leftmost driver station; aim for scale!
					// it's too complicated to reach the switch, so we'll try the scale if it's on our station's same side
					/*
                     if(gameData[1] == 'L') {
                         // drive forward to scale
                         // place cube
                     } else { // cross auto line
                         if(timer.Get() < 2.0) {
                             robotDrive.DriveCartesian((0.0),(-0.75),(0.0));
                         } else {
                             robotDrive.DriveCartesian((0.0),(0.0),(0.0));
                     }
					 */
				}
				else if(station == 2) { // middle driver station
					//pass the auto line
					//turn right
					//then place cube
				}
				else if(station == 3) { // rightmost driver station; aim for first switch!
					//pass auto line
					//turn left
					//then place a cube
				}
				/*
				 * USE FOR FMS FAILURE ONLY (crosses auto line so we can get ranking point)
				 *  if(timer.Get() < 2.0) {
                        robotDrive.DriveCartesian((0.0),(-0.75),(0.0));
                    } else {
                        robotDrive.DriveCartesian((0.0),(0.0),(0.0));
                    }
				 */
			}
		}
	}

	void TeleopInit() override {
	}

	void TeleopPeriodic() override {
		// Z axis is flipped so the direction it is tilted in is also the same direction as the robot's movement
		robotDrive.DriveCartesian(stick.GetX(), stick.GetY(), -(stick.GetZ()));

		if(stick.GetRawButton(1)) { // open cube grabber
			fluxCapacitor.Set(0.5);
		}
		else if(stick.GetRawButton(2)) { // close cube grabber
			fluxCapacitor.Set(-0.5);
		} else { // turn off motor
			fluxCapacitor.Set(0);
		}

		if(stick.GetRawButton(5)) { // shoot out cube
			leftGrabber.Set(0.5);
			rightGrabber.Set(-0.5);
		} else if(stick.GetRawButton(6)) { // receive cube
			leftGrabber.Set(-0.5);
			rightGrabber.Set(0.5);
		} else { // turn off motors
			leftGrabber.Set(0);
			rightGrabber.Set(0);
		}

		if(stick.GetRawButton(7)) { // cube intake motor's "on" button
			buttonEnabled = true;
		} else if(stick.GetRawButton(9)) { // "off" button
			buttonEnabled = false;
		}

		if(buttonEnabled) { // Set to joystick slider's manual control (-1.0 - +1.0)
			leftGrabber.Set(stick.GetThrottle());
			rightGrabber.Set(stick.GetThrottle());
		} else { // turn off the motors
			leftGrabber.Set(0);
			rightGrabber.Set(0);
		}

		// Control cube mechanism's height
		if(stick.GetRawButton(11)) { // climb down
			rightClimber.Set(0.5);
			leftClimber.Set(0.5);
		} else if(stick.GetRawButton(12)) { // climb up
			rightClimber.Set(-0.5);
			leftClimber.Set(-0.5);
		} else if(stick.GetRawButton(10)) { // climber motor off
			rightClimber.Set(0);
			leftClimber.Set(0);
		}

		if(stick.GetRawButton(3)) { // Winch climb up
			winch.Set(0.75f);
		} else if(stick.GetRawButton(4)) { // Winch climb down
			winch.Set(-0.75f);
		} else { // winch motor off
			winch.Set(0.0f);
		}
	}

private: // The port channels initialized are passed into the brackets, giving the motor its channel number
	static constexpr int kFrontLeftChannel = 0;
	static constexpr int kRearLeftChannel = 1;
	static constexpr int kFrontRightChannel = 2;
	static constexpr int kRearRightChannel = 3;
	static constexpr int kJoystickChannel = 0;
	frc::Spark frontLeft{kFrontLeftChannel};
	frc::Spark rearLeft{kRearLeftChannel};
	frc::Spark frontRight{kFrontRightChannel};
	frc::Spark rearRight{kRearRightChannel};
	frc::MecanumDrive robotDrive{frontLeft, rearLeft, frontRight, rearRight};
	frc::Joystick stick{kJoystickChannel};

	frc::Talon leftClimber;
	frc::Talon rightClimber;
	frc::Talon leftGrabber; // cube intake left motor
	frc::Talon rightGrabber; // cube intake right motor
	frc::Talon fluxCapacitor; // this is really the "middle grabber motor," but why not?
	frc::Talon winch;
	frc::Timer timer;
	bool buttonEnabled = false; // checks if the cube intake button is on
};
START_ROBOT_CLASS(Robot)

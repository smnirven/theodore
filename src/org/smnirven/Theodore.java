package org.smnirven;

import lejos.robotics.subsumption.*;
import lejos.nxt.*;
import lejos.nxt.comm.*;
import lejos.util.Timer;


/**
 * Example leJOS Project with an ant build file
 *
 */

public class Theodore {

  public static void main(String[] args) {
    String connected = "Connected";
    String waiting = "Waiting...";
    String closing = "Closing...";

    LCD.drawString(waiting, 0, 0);
    LCD.refresh();
    BTConnection btc = Bluetooth.waitForConnection();
    LCD.clear();
    LCD.drawString(connected, 0, 0);
    LCD.refresh();

    Commo c = new Commo(btc);
    c.start();

    // Calibrate the Compass
    CompassCalibrator.run();

    Timer timer = new Timer(1000, new TelemetryMonitor());

//    Behavior b1 = new DriveForward(c);
//    Behavior b2 = new DetectObstacle();
//    Behavior b3 = new DoExit();
//
//    Behavior[] behaviorList =
//    {
//       b1, b2, b3
//    };

//    Arbitrator arbitrator = new Arbitrator(behaviorList);
    LCD.drawString("Press button to start",0,1);
    Button.waitForPress();

    LCD.clear();
    LCD.drawString("And we're off...", 0, 0);
    LCD.refresh();
    
    timer.start();
//    arbitrator.start();
  }
}

class DetectObstacle implements Behavior {
  private UltrasonicSensor sonar;

  public DetectObstacle() {
    sonar = new UltrasonicSensor(SensorPort.S3);
  }

  public boolean takeControl() {
    sonar.ping();
    Sound.pause(20);
    return sonar.getDistance() < 35;
  }

  public void suppress() {
    // Looking for an obstacle is kind of important, so don't allow suppression
  }

  public void action() {
    Motor.A.rotate(-1080, true);// start Motor.A rotating backward
    Motor.C.rotate(-1800);  // rotate C farther to make the turn
  }
}

class DriveForward implements Behavior {
  private Commo commo;
  private Action command;

  public DriveForward(Commo c) {
    this.commo = c;
    this.command = Action.STOP;
  }

  public boolean takeControl() {
    return true;  // this behavior always wants control.
  }

  public void suppress() {
    Motor.A.stop();
    Motor.C.stop();
  }

  public void action() {
    
    if (this.commo.getNewCommand()) {
      this.command = this.commo.getCommand();
    }

    if (this.command == Action.STOP) {
      // ALL STOP!!!
      Motor.A.stop();
      Motor.C.stop();
    } else if (this.command == Action.AHEAD_STANDARD) {
      // All ahead standard
      Motor.A.setSpeed(180);
      Motor.C.setSpeed(180);
      Motor.A.forward();
      Motor.C.forward();
    } else if (this.command == Action.AHEAD_FULL) {
      Motor.A.setSpeed(720);
      Motor.C.setSpeed(720);
      Motor.A.forward();
      Motor.C.forward();
    } else if (this.command == Action.BACK_FULL) {
      Motor.A.setSpeed(720);
      Motor.C.setSpeed(720);
      Motor.A.backward();
      Motor.C.backward();
    }
  }
}

class DoExit implements Behavior {

	public boolean takeControl() {
		return Button.ESCAPE.isPressed();
	}

	public void suppress() {
		// We don't have code running, so nothing to suppress..
	}

	public void action() {
		System.exit(0);
	}
}

package org.smnirven;

import lejos.nxt.*;
import lejos.nxt.addon.*;
/**
 *
 * @author smnirven
 */
public class CompassCalibrator {
  public static void run() {
    CompassSensor cs = new CompassSensor(SensorPort.S4);

    Motor.A.setSpeed(90);
    Motor.C.setSpeed(90);

    cs.startCalibration();

    Motor.A.forward();
    Motor.C.backward();

    try {
      Thread.sleep(60000);
    } catch (Exception ex) {}

    Motor.A.stop();
    Motor.C.stop();

    cs.stopCalibration();
  }
}

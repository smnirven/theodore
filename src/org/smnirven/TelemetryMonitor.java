package org.smnirven;

import lejos.nxt.*;
import lejos.util.TimerListener;
import lejos.nxt.addon.CompassSensor;
import lejos.nxt.addon.TiltSensor;

/**
 *
 * @author smnirven
 */
public class TelemetryMonitor implements TimerListener {
  private CompassSensor compass;
  private TiltSensor tilt;

  public TelemetryMonitor() {
    this.compass = new CompassSensor(SensorPort.S4);
    this.tilt = new TiltSensor(SensorPort.S1);
  }

  public void timedOut() {
    float heading = this.compass.getDegrees();
    int x = this.tilt.getXTilt();
    int y = this.tilt.getYTilt();
    int z = this.tilt.getZTilt();

    LCD.clear();
    LCD.drawString("hdg: " + Float.toString(heading), 0, 0);
    LCD.drawString("x: " + Integer.toString(x), 0, 2);
    LCD.drawString("y: " + Integer.toString(y), 0, 4);
    LCD.drawString("z: " + Integer.toString(z), 0, 6);
    LCD.refresh();
  }

}

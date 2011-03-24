package org.smnirven;

import java.io.DataInputStream;
import lejos.nxt.comm.BTConnection;

/**
 *
 * @author smnirven
 */
public class Commo extends Thread {
  DataInputStream dis;
	private boolean newCommand = true;
	private Action command = Action.NOOP;

	public Commo(BTConnection btc) {
		dis = btc.openDataInputStream();
	}

	/**
	 * BluetoothConnection thread which constantly waiting for new commands from PC
	 */
	public void run(){
		command = Action.NOOP;
		while(true){
			try{
				newCommand = false;
				command = Action.getActionById(dis.readInt());
				newCommand = true;
				try{
					Thread.sleep(100);
				}catch (Exception e) {
					// Do nothing
				}
			}catch (Exception e) {
				// Do nothing - we failed to open I/O stream above.
			}
		}
	}

	/*
	 * @return the current command sent from bluetooth
	 */
	public Action getCommand(){
		return command;
	}

	/*
	 * @return true if we have received new command from Bluetooth else false
	 */
	public boolean getNewCommand(){
		return newCommand;
	}
}

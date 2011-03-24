package org.smnirven;

/**
 *
 * @author smnirven
 */
public enum Action {
STOP(0), AHEAD_STANDARD(1), AHEAD_FULL(2), LEFT(3), RIGHT(4), BACK_FULL(5), NOOP(9);

	private int id;

	Action(int id) {
		this.id = id;
	}

	public int getId() {
		return this.id;
	}

	public static Action getActionById(int id) {
		switch (id) {
			case 0:
				return Action.STOP;
			case 1:
				return Action.AHEAD_STANDARD;
      case 2:
        return Action.AHEAD_FULL;
			case 3:
				return Action.LEFT;
			case 4:
				return Action.RIGHT;
      case 5:
        return Action.BACK_FULL;
			default:
				return null;
		}
	}
}

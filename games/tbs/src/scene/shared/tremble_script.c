/* The trembling of Imil's freezing young man and of the Tolbi ship's lookout
   once the sea monster has landed beside him. */

/* An actor trembles: squashed narrow and a little tall, then sprung back over
   three ticks, every four ticks for as long as the script runs. */
Sint32 ActorTrembleScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_ADD(SW_SCALE_X, -toFIXED(5) / 16),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

// Kolima and Kolima Forest: the party quivers under Tret's curse as it falls, and is set back to
// its own size as each collapses.

// Each party member squashes and wobbles until another script takes over.
Sint32 KolimaPartySquashScript[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

// Back to full size as the party member collapses.
Sint32 KolimaPartyRestoreScaleScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_WAIT(1),
	SC_END,
};

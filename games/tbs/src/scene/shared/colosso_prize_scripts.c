// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3): the scripts of the
// raised prize and its sparkles (colosso_finish.c).

// A sparkle beside the raised prize sinks for a second, then is released; a copy of the script of
// the sparkles by a found item (FieldEffectParticleScript).
Sint32 ColossoPrizeSparkleScript[] = {
	SC_ADD(SW_Y, -toFIXED(1) / 4),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_RELEASE,
};

// The raised prize swells and shrinks back, twice, with sparkles about it
// (scColossoSparkleAroundPrize).
Sint32 ColossoPrizeScript[] = {
	SC_SET(SW_HOOK, (Sint32)scColossoSparkleAroundPrize),
	SC_LABEL(1),
	SC_WAIT(10),
	SC_SET(SW_SCALE_X, (toFIXED(11)) / 8),
	SC_SET(SW_SCALE_Y, (toFIXED(11)) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, (toFIXED(3)) / 2),
	SC_SET(SW_SCALE_Y, (toFIXED(3)) / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, (toFIXED(9)) / 8),
	SC_SET(SW_SCALE_Y, (toFIXED(9)) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, (toFIXED(11)) / 8),
	SC_SET(SW_SCALE_Y, (toFIXED(11)) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, (toFIXED(3)) / 2),
	SC_SET(SW_SCALE_Y, (toFIXED(3)) / 2),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, (toFIXED(9)) / 8),
	SC_SET(SW_SCALE_Y, (toFIXED(9)) / 8),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_WAIT(2),
	SC_WAIT(50),
	SC_LOOP(2, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_SET(SW_HOOK, 0),
	SC_END,
};

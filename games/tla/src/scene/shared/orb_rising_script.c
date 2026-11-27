// A rising orb's script, which the Jupiter and Mars Lighthouses keep a copy of for the particles
// that rise in their beams: at 0.8 of its size, it rises a pixel a tick for 16 ticks and is gone.
Uint32 OrbRisingScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(0.8)),
	SC_SET(SW_SCALE_Y, toFIXED(0.8)),
	SC_ANIMATION(1),
	SC_LABEL(1),
	SC_ADD(SW_Y, toFIXED(1)),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};

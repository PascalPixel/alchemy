// The script of a ripple widening on the water over an actor (scActorEmitRipples), which the Apojii
// Islands' and Aqua Rock's overlays keep a copy of.

// A ripple rises a quarter of a pixel a tick for 31 ticks, then is released.
Uint32 RippleScript[] = {
	SC_ADD(SW_Y, toFIXED(1) / 4),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_RELEASE,
};

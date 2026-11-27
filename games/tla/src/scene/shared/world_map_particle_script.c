// Unused: a particle's script on the World Map, which the staff credits keep a copy of: it lasts 39
// ticks, is put back at x and z 0, and is gone.
UNUSED Uint32 WorldMapParticleScript[] = {
	SC_WAIT(39),
	SC_SET(SW_X, 0),
	SC_SET(SW_Z, 0),
	SC_WAIT(1),
	SC_RELEASE,
};

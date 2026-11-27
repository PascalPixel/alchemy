// The frames of the drops of rain fdGroundParticlesDraw moves about a doused stone, which the
// Apojii Islands and Aqua Rock keep a copy of.
//
// An includer whose overlay places the table after its pictures defines
// GROUND_PARTICLE_FRAMES_SECTION as LATE_DATA first.

#ifndef GROUND_PARTICLE_FRAMES_SECTION
#	define GROUND_PARTICLE_FRAMES_SECTION
#endif

// A drop's frames, five halfwords each, played from the last to the first: its offset from where it
// lands, x and y in pixels, its first tile, its OAM shape and its size. Nine frames of a streak
// falling slantwise down to the ground, 8 pixels across and 16 down a tick, then seven of its
// splash.
GROUND_PARTICLE_FRAMES_SECTION Sint16 GroundParticleFrames[] = {
	-8,  -12, 20,  0,   1,    -8,  -12, 20,   0,   1,  -8,  -12, 20,   0,   1,    -8,  -12,  16,
	0,   1,   -8,  -12, 16,   0,   1,   -8,   -12, 12, 0,   1,   -8,   -12, 8,    0,   1,    0,
	-32, 0,   2,   2,   8,    -48, 0,   2,    2,   16, -64, 0,   2,    2,   24,   -80, 0,    2,
	2,   32,  -96, 0,   2,    2,   40,  -112, 0,   2,  2,   48,  -128, 0,   2,    2,   56,   -144,
	0,   2,   2,   64,  -160, 0,   2,   2,    -2,  12, -4,  12,  -6,   12,  -8,   8,   -10,  8,
	-12, 8,   -14, 8,   -16,  8,   -19, 4,    -21, 4,  -24, 4,   -27,  4,   -30,  4,   -33,  4,
	-36, 4,   -40, 4,   -44,  4,   -48, 0,    -52, 0,  -56, 0,   -60,  0,   -64,  0,   -68,  0,
	-72, 0,   -76, 0,   -80,  0,   -85, 0,    -90, 0,  -95, 0,   -100, 0,   -110, 0,   -120, 0,
};

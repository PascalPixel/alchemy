/* @file */
/* The script of a thrown bit of rock, ice or earth, which the scenes that */
/* scatter debris keep a copy of: Gaia Rock's stone heads, Taopo Swamp's */
/* tremor, Mars Lighthouse's ice and rubble. */

/* A piece of debris flies for 40 frames, then blinks ten times and is gone. */
u32 Debris_Script[] = {
	SC_WAIT(40),
	SC_LABEL(1),
	SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_NONE),
	SC_WAIT(1),
	SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_ONE),
	SC_WAIT(1),
	SC_LOOP(10, SC_TO(1)),
	SC_RELEASE,
};

/* @file */
/* Twenty-four scenes: the three lifetimes fdEffectSpawn */
/* (field_effect_spawn.c) gives its effects through */
/* FieldEffect_DurationScripts. An effect lasts 24, 46 or 128 frames: it */
/* waits a frame 22, 44 or 126 times, then twice more, placed at 0, 0, 0 and */
/* stopped between the two, and is released. The repeated one-frame wait keeps
 */
/* the count in each script's fourth word, by which fdEffectSpawn divides */
/* a change of scale so the effect reaches its scale by the end of the repeats.
 */
/* */
/* An includer whose scripts are read-only defines */
/* FIELD_EFFECT_DURATION_SCRIPTS_READ_ONLY first. */

#ifdef FIELD_EFFECT_DURATION_SCRIPTS_READ_ONLY
#define FIELD_EFFECT_DURATION_SCRIPT_QUALIFIER const
#else
#define FIELD_EFFECT_DURATION_SCRIPT_QUALIFIER
#endif

FIELD_EFFECT_DURATION_SCRIPT_QUALIFIER s32 FieldEffect_ShortScript[] = {
	SC_WAIT(1),			  SC_LOOP(22, SC_TOP), SC_WAIT(1),
	SC_POSITION(0, 0, 0), SC_WAIT(1),		   SC_RELEASE,
};

FIELD_EFFECT_DURATION_SCRIPT_QUALIFIER s32 FieldEffect_MediumScript[] = {
	SC_WAIT(1),			  SC_LOOP(44, SC_TOP), SC_WAIT(1),
	SC_POSITION(0, 0, 0), SC_WAIT(1),		   SC_RELEASE,
};

FIELD_EFFECT_DURATION_SCRIPT_QUALIFIER s32 FieldEffect_LongScript[] = {
	SC_WAIT(1),			  SC_LOOP(126, SC_TOP), SC_WAIT(1),
	SC_POSITION(0, 0, 0), SC_WAIT(1),			SC_RELEASE,
};

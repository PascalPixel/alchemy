/* @file */
/* The three lifetimes scFieldEffectSpawn (../field/field_effect_spawn.c) gives
 */
/* its effects through each scene's FieldEffect_DurationScripts, which most */
/* scenes keep a copy of, as ☀️'s do (its */
/* shared/field_effect_duration_scripts.c). An effect lasts 24, 46 or 128 */
/* frames: it waits a frame 22, 44 or 126 times, then twice more, placed at 0,
 */
/* 0, 0 and stopped between the two, and is released. The repeated one-frame */
/* wait keeps the count in each script's fourth word, by which */
/* scFieldEffectSpawn divides a change of scale so the effect reaches its scale
 */
/* by the end of the repeats. */
/* */
/* An includer whose overlay places the scripts after its pictures defines */
/* FIELD_EFFECT_DURATION_SCRIPT_SECTION as LATE_DATA first. */

#ifndef FIELD_EFFECT_DURATION_SCRIPT_SECTION
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION
#endif

FIELD_EFFECT_DURATION_SCRIPT_SECTION u32 FieldEffect_ShortScript[] = {
	SC_WAIT(1), SC_LOOP(22, SC_TOP), SC_WAIT(1), SC_POSITION(0, 0, 0),
	SC_WAIT(1), SC_RELEASE,			 SC_END,
};

FIELD_EFFECT_DURATION_SCRIPT_SECTION u32 FieldEffect_MediumScript[] = {
	SC_WAIT(1), SC_LOOP(44, SC_TOP), SC_WAIT(1), SC_POSITION(0, 0, 0),
	SC_WAIT(1), SC_RELEASE,			 SC_END,
};

FIELD_EFFECT_DURATION_SCRIPT_SECTION u32 FieldEffect_LongScript[] = {
	SC_WAIT(1), SC_LOOP(126, SC_TOP), SC_WAIT(1), SC_POSITION(0, 0, 0),
	SC_WAIT(1), SC_RELEASE,			  SC_END,
};

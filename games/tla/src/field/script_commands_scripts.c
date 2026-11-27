/* The particle scripts of the field commands after the rope and vine pictures,
   Mind Read's colour cycle and the pad's headings, in the order of The Broken
   Seal's field_mode_tables.c. */

#include "fixed_math.h"
#include "types.h"
#include "field/actor_script.h"
#include "field/script_commands.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "field/sprite.h"

/* Unused: one word, 17, which no code names. */
const Sint32 FieldEffect_UnusedWord[1] = {17};

/* [0] only releases the actor, +1 waits for it to arrive and releases it. */
const Sint32 FieldEffect_ReleaseScript[] = {
	SC_RELEASE,
	SC_SYNC,
	SC_RELEASE,
};

/* Faces the watched actor every frame, forever: the caster holding its target
   while Move, Carry or Lift has it (each gives the caster its target first). */
const Sint32 FieldEffect_SourceHoldScript[] = {
	SC_FACE_WATCHED,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* A shard or particle: waits to arrive, slows to 1/16 pixel a frame, and
   releases itself 10 frames later. */
const Sint32 FieldEffect_FragmentScript[] = {
	SC_SYNC,
	SC_SPEED(0.0625),
	SC_ANIMATION(0),
	SC_WAIT(10),
	SC_RELEASE,
};

/* The colour modes Mind Read's caster cycles through, two frames each
   (fdActorSetLayersColMode): its colour channels swapped one way after another
   and back to its own; The Broken Seal's MindRead_CasterPaletteCycle. */
const Uint8 MindRead_CasterColorCycle[8] = {
	SPRITE_COLOR_GRB,
	SPRITE_COLOR_RBG,
	SPRITE_COLOR_BGR,
	SPRITE_COLOR_GBR,
	SPRITE_COLOR_BGR,
	SPRITE_COLOR_RBG,
	SPRITE_COLOR_GRB,
	SPRITE_COLOR_NORMAL,
};

/* clang-format off */

/* An angle for each value of the right, left, up and down bits of the */
/* +Control Pad; DPAD_ANGLE_NONE for none (fdInputGetDpadAng, */
/* script_commands_mind_read.c). */
const Uint16 Input_DpadAngles[16] = {
	DPAD_ANGLE_NONE, FACING_EAST,     FACING_WEST,     DPAD_ANGLE_NONE,
	FACING_NORTH,    DPAD_ANGLE_NONE, DPAD_ANGLE_NONE, FACING_NORTH,
	FACING_SOUTH,    DPAD_ANGLE_NONE, DPAD_ANGLE_NONE, FACING_SOUTH,
	DPAD_ANGLE_NONE, FACING_EAST,     FACING_WEST,     DPAD_ANGLE_NONE,
};
/* clang-format on */

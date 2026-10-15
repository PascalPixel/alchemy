#ifndef ALCHEMY_ALTIN_PEAK_MOTION_H
#define ALCHEMY_ALTIN_PEAK_MOTION_H

#include "types.h"
#include "layout_guard.h"

struct SceneMotion
{
	u8	unk_00[12];
	s32 y;
	u8	unk_10[4];
	s32 ground;
	u8	unk_18[16];
	s32 velocity;
	u8	unk_2c[47];
	u8	state;
	u8	unk_5c[8];
	s16 timer, delay;
	s32 active;
};

LAYOUT_OFFSET_GUARD(SceneMotion_Velocity, struct SceneMotion, velocity, 0x28);
LAYOUT_OFFSET_GUARD(SceneMotion_State, struct SceneMotion, state, 0x5b);
LAYOUT_OFFSET_GUARD(SceneMotion_Timer, struct SceneMotion, timer, 0x64);
LAYOUT_OFFSET_GUARD(SceneMotion_Delay, struct SceneMotion, delay, 0x66);
LAYOUT_OFFSET_GUARD(SceneMotion_Active, struct SceneMotion, active, 0x68);

#endif

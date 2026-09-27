#ifndef ALCHEMY_ARUTIN_H
#define ALCHEMY_ARUTIN_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* Timed actor state shared by the mountain scene and its update callback. */
struct SceneMotion {
    u8 unk_00[12]; s32 y; u8 unk_10[4]; s32 ground;
    u8 unk_18[16]; s32 velocity; u8 unk_2c[28]; s32 gravity;
    u8 unk_4c[15]; u8 state;
    u8 unk_5c[8]; s16 timer, delay; s32 active;
};

LAYOUT_OFFSET_GUARD(SceneMotion_Velocity, struct SceneMotion, velocity, 0x28);
LAYOUT_OFFSET_GUARD(SceneMotion_Gravity, struct SceneMotion, gravity, 0x48);
LAYOUT_OFFSET_GUARD(SceneMotion_State, struct SceneMotion, state, 0x5b);
LAYOUT_OFFSET_GUARD(SceneMotion_Timer, struct SceneMotion, timer, 0x64);
LAYOUT_OFFSET_GUARD(SceneMotion_Delay, struct SceneMotion, delay, 0x66);
LAYOUT_OFFSET_GUARD(SceneMotion_Active, struct SceneMotion, active, 0x68);

void SceneMotion_UpdateTimedActor(struct SceneMotion *work);

#endif

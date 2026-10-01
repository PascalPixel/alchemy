#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

#include "EFFECT_STEP.H"
#include "LAYOUT_GUARD.H"
#include "TYPES.H"

struct BattleEffectArgument;

struct BattleEffectWork {
    u8 unknown_0000[0x6980];
    s32 bg2_x[160];
    u8 unknown_6c00[0x480];
    struct EffectStep particles[64];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x20];
    s32 shake_frames;
    s32 camera_delta; /* Camera_ApplyPhasedDelta adds it to the camera */
    s32 camera_phase;
    s32 fade_frames;
    s32 fade_step;
    u8 unknown_77bc[0x68];
    s32 transfer_pending;
    struct BattleEffectArgument *effect;
};

struct BattleEffectArgument {
    s32 kind;
    s32 side;
    s32 actor; /* Acting unit; actors[] contains the affected units. */
    s32 unknown_000c;
    s32 unknown_0010;
    s32 count;
    s32 variant;
    s32 unknown_001c;
    s32 unknown_0020;
    s16 actors[8];
};

LAYOUT_OFFSET_GUARD(BattleEffectWork_Bg2X,
    struct BattleEffectWork, bg2_x, 0x6980);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Particles,
    struct BattleEffectWork, particles, 0x7080);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferMode,
    struct BattleEffectWork, transfer_mode, 0x7780);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferValue,
    struct BattleEffectWork, transfer_value, 0x7784);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ShakeFrames,
    struct BattleEffectWork, shake_frames, 0x77a8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_CameraDelta,
    struct BattleEffectWork, camera_delta, 0x77ac);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FadeFrames,
    struct BattleEffectWork, fade_frames, 0x77b4);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FadeStep,
    struct BattleEffectWork, fade_step, 0x77b8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferPending,
    struct BattleEffectWork, transfer_pending, 0x7824);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Effect,
    struct BattleEffectWork, effect, 0x7828);
LAYOUT_OFFSET_GUARD(BattleEffectArgument_Count,
    struct BattleEffectArgument, count, 0x14);
LAYOUT_OFFSET_GUARD(BattleEffectArgument_Actors,
    struct BattleEffectArgument, actors, 0x24);

#endif

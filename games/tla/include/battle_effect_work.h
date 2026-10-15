#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

// The Lost Age's copy of the battle effect work: The Broken Seal's block with
// the 64 particles in front of the sheet, the two affine scales after the BG2
// reference point, 64 object slots and one more word before the effect.

#include "effect_step.h"
#include "layout_guard.h"
#include "types.h"

struct BattleEffectArgument;

struct BattleEffectWork
{
    struct EffectStep particles[64];
    u8 sheet[0x6980];
    s32 bg2_x[160];
    u8 unknown_7300[0x480];
    s32 transfer_mode;
    s32 transfer_value;
    s32 unknown_7788;
    s32 frame;
    s32 scroll_timer;
    s32 scroll_interval;
    s32 scroll_step_x;
    s32 scroll_step_y;
    s32 saved_bg1_x;
    s32 saved_bg1_y;
    s32 shake_frames;
    s32 camera_delta;
    s32 camera_phase;
    s32 fade_frames;
    s32 fade_step;
    u16 win0h;
    u16 win0v;
    u16 win1h;
    u16 win1v;
    u16 winin;
    u16 winout;
    u16 dispcnt;
    u16 bldcnt;
    u16 bldalpha;
    u16 padding_77ce;
    s32 bg2x;
    s32 bg2y;
    u16 bg2pa;
    u16 bg2pd;
    void *objects[64];
    u8 actor_timers[8];
    s32 frames_since_transfer;
    s32 transfer_pending;
    s32 unknown_78ec;
    struct BattleEffectArgument *effect;
};

struct BattleEffectArgument
{
    s32 kind;
    s32 side;
    s32 actor;
    s32 unknown_000c;
    s32 unknown_0010;
    s32 count;
    s32 variant;
    s32 prelude;
    s32 unknown_0020;
    s16 actors[8];
};

LAYOUT_OFFSET_GUARD(BattleEffectWork_Sheet, struct BattleEffectWork, sheet, 0x700);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Bg2X, struct BattleEffectWork, bg2_x, 0x7080);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferMode, struct BattleEffectWork, transfer_mode, 0x7780);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ShakeFrames, struct BattleEffectWork, shake_frames, 0x77a8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FadeStep, struct BattleEffectWork, fade_step, 0x77b8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Win0H, struct BattleEffectWork, win0h, 0x77bc);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Bg2RefX, struct BattleEffectWork, bg2x, 0x77d0);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Bg2Pa, struct BattleEffectWork, bg2pa, 0x77d8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Objects, struct BattleEffectWork, objects, 0x77dc);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ActorTimers, struct BattleEffectWork, actor_timers, 0x78dc);
LAYOUT_OFFSET_GUARD(
    BattleEffectWork_TransferPending, struct BattleEffectWork, transfer_pending, 0x78e8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Effect, struct BattleEffectWork, effect, 0x78f0);
LAYOUT_OFFSET_GUARD(BattleEffectArgument_Count, struct BattleEffectArgument, count, 0x14);
LAYOUT_OFFSET_GUARD(BattleEffectArgument_Actors, struct BattleEffectArgument, actors, 0x24);

#endif

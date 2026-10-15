#ifndef ALCHEMY_BATTLE_FX_WORK_H
#define ALCHEMY_BATTLE_FX_WORK_H

#include "types.h"
#include "layout_guard.h"
#include "effect_step.h"

struct BattleEffectArgument;

// The battle effect scenes' work (heap slot 23), as The Lost Age lays it
// out: The Broken Seal's BattleEffectWork with the particles moved ahead of
// the sheet and 0xc4 more bytes ahead of the actor timers.
struct BattleFxWork {
    struct EffectStep particles[64];
    u8 sheet[0x6980];
    u8 unknown_7080[0x700];
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
    u8 unknown_77bc[0x20];
    void *objects[16];
    u8 unknown_781c[0xc0];
    u8 actor_timers[8];
    s32 frames_since_transfer;
    s32 transfer_pending;
    s32 unknown_78ec;
    struct BattleEffectArgument *effect;
};

LAYOUT_OFFSET_GUARD(BattleFxWork_Sheet, struct BattleFxWork, sheet, 0x700);
LAYOUT_OFFSET_GUARD(BattleFxWork_TransferMode, struct BattleFxWork, transfer_mode, 0x7780);
LAYOUT_OFFSET_GUARD(BattleFxWork_ShakeFrames, struct BattleFxWork, shake_frames, 0x77a8);
LAYOUT_OFFSET_GUARD(BattleFxWork_Objects, struct BattleFxWork, objects, 0x77dc);
LAYOUT_OFFSET_GUARD(BattleFxWork_ActorTimers, struct BattleFxWork, actor_timers, 0x78dc);
LAYOUT_OFFSET_GUARD(BattleFxWork_TransferPending, struct BattleFxWork, transfer_pending, 0x78e8);
LAYOUT_OFFSET_GUARD(BattleFxWork_Effect, struct BattleFxWork, effect, 0x78f0);

#endif

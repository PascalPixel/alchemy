#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

#include "EFFECT_STEP.H"
#include "LAYOUT_GUARD.H"
#include "TYPES.H"

struct BattleEffectArgument;

/*
 * The battle effects' work block, heap slot 39 (gBattleFxWork[0]).  An
 * effect keeps its sheets and particles here, and leaves word here for the
 * callbacks that run beside it every frame: when to send the canvas to the
 * screen, how to move the background and the camera, what the window and
 * blend registers should hold.
 */
struct BattleEffectWork {
    /* Decompressed sprite sheets, laid out by each effect as it pleases. */
    u8 sheet[0x6980];
    /* One BG2X for every screen line, fed to the register line by line. */
    s32 bg2_x[160];
    u8 unknown_6c00[0x480];
    struct EffectStep particles[64];
    /* What the transfer does with the canvas (1 copy and refill, 2 fade,
       3 darken, 4 brighten) and the value it does it with. */
    s32 transfer_mode;
    s32 transfer_value;
    s32 unknown_7788;
    /* Counted up from 0 by the effect's frame callback. */
    s32 frame;
    /* Every scroll_interval frames the background (or BG2's reference
       point) moves by one step; scroll_timer counts up to the interval. */
    s32 scroll_timer;
    s32 scroll_interval;
    s32 scroll_step_x;
    s32 scroll_step_y;
    /* BG1's scroll as it was before the effect took the layer. */
    s32 saved_bg1_x;
    s32 saved_bg1_y;
    s32 shake_frames;
    /* What Camera_ApplyPhasedDelta adds to the scene camera: all of it
       once when camera_phase is 1, half of it every frame otherwise. */
    s32 camera_delta;
    s32 camera_phase;
    s32 fade_frames;
    s32 fade_step;
    /* Copied to the window, display and blend registers once a frame. */
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
    /* BG2's reference point, copied to its registers the same way. */
    s32 bg2x;
    s32 bg2y;
    /* The battle objects the effect spawned. */
    void *objects[16];
    /* A countdown for each of effect->actors; at zero the actor's objects
       are updated. */
    u8 actor_timers[8];
    /* 1 on the frame the canvas went to the screen, one more each frame
       after it. */
    s32 frames_since_transfer;
    /* Set to 1 by the effect when its canvas is ready to be sent. */
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
LAYOUT_OFFSET_GUARD(BattleEffectWork_Frame,
    struct BattleEffectWork, frame, 0x778c);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ScrollTimer,
    struct BattleEffectWork, scroll_timer, 0x7790);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ScrollStepX,
    struct BattleEffectWork, scroll_step_x, 0x7798);
LAYOUT_OFFSET_GUARD(BattleEffectWork_SavedBg1X,
    struct BattleEffectWork, saved_bg1_x, 0x77a0);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ShakeFrames,
    struct BattleEffectWork, shake_frames, 0x77a8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_CameraDelta,
    struct BattleEffectWork, camera_delta, 0x77ac);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FadeFrames,
    struct BattleEffectWork, fade_frames, 0x77b4);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FadeStep,
    struct BattleEffectWork, fade_step, 0x77b8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Win0H,
    struct BattleEffectWork, win0h, 0x77bc);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Bg2RefX,
    struct BattleEffectWork, bg2x, 0x77d0);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Objects,
    struct BattleEffectWork, objects, 0x77d8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ActorTimers,
    struct BattleEffectWork, actor_timers, 0x7818);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FramesSinceTransfer,
    struct BattleEffectWork, frames_since_transfer, 0x7820);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferPending,
    struct BattleEffectWork, transfer_pending, 0x7824);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Effect,
    struct BattleEffectWork, effect, 0x7828);
LAYOUT_SIZE_GUARD(BattleEffectWork_Size,
    struct BattleEffectWork, 0x782c);
LAYOUT_OFFSET_GUARD(BattleEffectArgument_Count,
    struct BattleEffectArgument, count, 0x14);
LAYOUT_OFFSET_GUARD(BattleEffectArgument_Actors,
    struct BattleEffectArgument, actors, 0x24);

#endif

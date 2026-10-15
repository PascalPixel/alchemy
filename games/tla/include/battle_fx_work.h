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
    u8 unk7080[0x700];
    s32 transferMode;
    s32 transferValue;
    s32 unk7788;
    s32 frame;
    s32 scrollTimer;
    s32 scrollInterval;
    s32 scrollStepX;
    s32 scrollStepY;
    s32 savedBg1X;
    s32 savedBg1Y;
    s32 shakeFrames;
    s32 cameraDelta;
    s32 cameraPhase;
    s32 fadeFrames;
    s32 fadeStep;
    u8 unk77bc[0x20];
    void *objects[16];
    u8 unk781c[0xc0];
    u8 actorTimers[8];
    s32 framesSinceTransfer;
    s32 transferPending;
    s32 unk78ec;
    struct BattleEffectArgument *effect;
};

LAYOUT_OFFSET_GUARD(BattleFxWork_Sheet, struct BattleFxWork, sheet, 0x700);
LAYOUT_OFFSET_GUARD(BattleFxWork_TransferMode, struct BattleFxWork, transferMode, 0x7780);
LAYOUT_OFFSET_GUARD(BattleFxWork_ShakeFrames, struct BattleFxWork, shakeFrames, 0x77a8);
LAYOUT_OFFSET_GUARD(BattleFxWork_Objects, struct BattleFxWork, objects, 0x77dc);
LAYOUT_OFFSET_GUARD(BattleFxWork_ActorTimers, struct BattleFxWork, actorTimers, 0x78dc);
LAYOUT_OFFSET_GUARD(BattleFxWork_TransferPending, struct BattleFxWork, transferPending, 0x78e8);
LAYOUT_OFFSET_GUARD(BattleFxWork_Effect, struct BattleFxWork, effect, 0x78f0);

#endif

#ifndef ALCHEMY_BATTLE_FX_WORK_H
#define ALCHEMY_BATTLE_FX_WORK_H

#include "types.h"
#include "static_assert.h"
#include "effect_step.h"

struct BattleEffectArgument;

/// The battle effect scenes' work (heap slot 23), as The Lost Age lays it
/// out: The Broken Seal's BattleEffectWork with the particles moved ahead of
/// the sheet and 0xc4 more bytes ahead of the actor timers.
struct BattleFxWork {
  struct EffectStep particles[64];
  u8 sheet[0x6980];
  u8 unk7080[0x700];
  s32 transferMode;
  s32 transferValue;
  s32 unk_7788;
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
  u16 win0h;
  u16 win0v;
  u16 win1h;
  u16 win1v;
  u16 winin;
  u16 winout;
  u16 dispcnt;
  u16 bldcnt;
  u16 bldalpha;
  u16 unk_77ce;
  s32 bg2x;
  s32 bg2y;
  u16 bg2pa;
  u16 bg2pd;
  void *objects[16];
  u8 unk781c[0xc0];
  u8 actorTimers[8];
  s32 framesSinceTransfer;
  s32 transferPending;
  s32 unk78ec;
  struct BattleEffectArgument *effect;
};

STATIC_ASSERT(offsetof(struct BattleFxWork, sheet) == 0x700);
STATIC_ASSERT(offsetof(struct BattleFxWork, transferMode) == 0x7780);
STATIC_ASSERT(offsetof(struct BattleFxWork, shakeFrames) == 0x77a8);
STATIC_ASSERT(offsetof(struct BattleFxWork, objects) == 0x77dc);
STATIC_ASSERT(offsetof(struct BattleFxWork, actorTimers) == 0x78dc);
STATIC_ASSERT(offsetof(struct BattleFxWork, transferPending) == 0x78e8);
STATIC_ASSERT(offsetof(struct BattleFxWork, effect) == 0x78f0);

#endif

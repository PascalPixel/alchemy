#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

#include "effect_step.h"
#include "battle_effect_argument.h"
#include "static_assert.h"
#include "types.h"

struct BattleEffectWork {
  u8 sheet[0x6980];
  s32 bg2X[160];
  u8 unk6c00[0x480];
  struct EffectStep particles[64];
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
  u16 win0h;
  u16 win0v;
  u16 win1h;
  u16 win1v;
  u16 winin;
  u16 winout;
  u16 dispcnt;
  u16 bldcnt;
  u16 bldalpha;
  u16 padding77ce;
  s32 bg2x;
  s32 bg2y;
  void *objects[16];
  u8 actorTimers[8];
  s32 framesSinceTransfer;
  s32 transferPending;
  struct BattleEffectArgument *effect;
};

STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2X) == 0x6980);
STATIC_ASSERT(offsetof(struct BattleEffectWork, particles) == 0x7080);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferMode) == 0x7780);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferValue) == 0x7784);
STATIC_ASSERT(offsetof(struct BattleEffectWork, frame) == 0x778c);
STATIC_ASSERT(offsetof(struct BattleEffectWork, scrollTimer) == 0x7790);
STATIC_ASSERT(offsetof(struct BattleEffectWork, scrollStepX) == 0x7798);
STATIC_ASSERT(offsetof(struct BattleEffectWork, savedBg1X) == 0x77a0);
STATIC_ASSERT(offsetof(struct BattleEffectWork, shakeFrames) == 0x77a8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, cameraDelta) == 0x77ac);
STATIC_ASSERT(offsetof(struct BattleEffectWork, fadeFrames) == 0x77b4);
STATIC_ASSERT(offsetof(struct BattleEffectWork, fadeStep) == 0x77b8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, win0h) == 0x77bc);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2x) == 0x77d0);
STATIC_ASSERT(offsetof(struct BattleEffectWork, objects) == 0x77d8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, actorTimers) == 0x7818);
STATIC_ASSERT(offsetof(struct BattleEffectWork, framesSinceTransfer) == 0x7820);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferPending) == 0x7824);
STATIC_ASSERT(offsetof(struct BattleEffectWork, effect) == 0x7828);
STATIC_ASSERT(sizeof(struct BattleEffectWork) == 0x782c);

#endif

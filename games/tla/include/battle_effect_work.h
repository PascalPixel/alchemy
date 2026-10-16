#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

// The Lost Age's copy of the battle effect work: The Broken Seal's block with
// the 64 particles in front of the sheet, the two affine scales after the BG2
// reference point, 64 object slots and one more word before the effect.

#include "effect_step.h"
#include "static_assert.h"
#include "types.h"

struct BattleEffectArgument;

struct BattleEffectWork {
  struct EffectStep particles[64];
  u8 sheet[0x6980];
  s32 bg2X[160];
  u8 unk7300[0x480];
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
  u16 bg2pa;
  u16 bg2pd;
  void *objects[64];
  u8 actorTimers[8];
  s32 framesSinceTransfer;
  s32 transferPending;
  s32 unk78ec;
  struct BattleEffectArgument *effect;
};

struct BattleEffectArgument {
  s32 kind;
  s32 side;
  s32 actor;
  s32 unk000c;
  s32 unk0010;
  s32 count;
  s32 variant;
  s32 prelude;
  s32 unk0020;
  s16 actors[8];
};

STATIC_ASSERT(offsetof(struct BattleEffectWork, sheet) == 0x700);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2X) == 0x7080);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferMode) == 0x7780);
STATIC_ASSERT(offsetof(struct BattleEffectWork, shakeFrames) == 0x77a8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, fadeStep) == 0x77b8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, win0h) == 0x77bc);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2x) == 0x77d0);
STATIC_ASSERT(offsetof(struct BattleEffectWork, bg2pa) == 0x77d8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, objects) == 0x77dc);
STATIC_ASSERT(offsetof(struct BattleEffectWork, actorTimers) == 0x78dc);
STATIC_ASSERT(offsetof(struct BattleEffectWork, transferPending) == 0x78e8);
STATIC_ASSERT(offsetof(struct BattleEffectWork, effect) == 0x78f0);
STATIC_ASSERT(offsetof(struct BattleEffectArgument, count) == 0x14);
STATIC_ASSERT(offsetof(struct BattleEffectArgument, actors) == 0x24);

#endif

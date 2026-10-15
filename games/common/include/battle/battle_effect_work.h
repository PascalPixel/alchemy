#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

#include "effect_step.h"
#include "layout_guard.h"
#include "types.h"

struct BattleEffectArgument;

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

LAYOUT_OFFSET_GUARD(BattleEffectWork_Bg2X, struct BattleEffectWork, bg2X,
                    0x6980);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Particles, struct BattleEffectWork,
                    particles, 0x7080);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferMode, struct BattleEffectWork,
                    transferMode, 0x7780);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferValue, struct BattleEffectWork,
                    transferValue, 0x7784);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Frame, struct BattleEffectWork, frame,
                    0x778c);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ScrollTimer, struct BattleEffectWork,
                    scrollTimer, 0x7790);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ScrollStepX, struct BattleEffectWork,
                    scrollStepX, 0x7798);
LAYOUT_OFFSET_GUARD(BattleEffectWork_SavedBg1X, struct BattleEffectWork,
                    savedBg1X, 0x77a0);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ShakeFrames, struct BattleEffectWork,
                    shakeFrames, 0x77a8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_CameraDelta, struct BattleEffectWork,
                    cameraDelta, 0x77ac);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FadeFrames, struct BattleEffectWork,
                    fadeFrames, 0x77b4);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FadeStep, struct BattleEffectWork,
                    fadeStep, 0x77b8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Win0H, struct BattleEffectWork, win0h,
                    0x77bc);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Bg2RefX, struct BattleEffectWork, bg2x,
                    0x77d0);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Objects, struct BattleEffectWork, objects,
                    0x77d8);
LAYOUT_OFFSET_GUARD(BattleEffectWork_ActorTimers, struct BattleEffectWork,
                    actorTimers, 0x7818);
LAYOUT_OFFSET_GUARD(BattleEffectWork_FramesSinceTransfer,
                    struct BattleEffectWork, framesSinceTransfer, 0x7820);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferPending, struct BattleEffectWork,
                    transferPending, 0x7824);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Effect, struct BattleEffectWork, effect,
                    0x7828);
LAYOUT_SIZE_GUARD(BattleEffectWork_Size, struct BattleEffectWork, 0x782c);
LAYOUT_OFFSET_GUARD(BattleEffectArgument_Count, struct BattleEffectArgument,
                    count, 0x14);
LAYOUT_OFFSET_GUARD(BattleEffectArgument_Actors, struct BattleEffectArgument,
                    actors, 0x24);

#endif

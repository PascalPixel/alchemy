#ifndef ALCHEMY_MODULE_SCENE_GONDOWAN_CLIFFS_H
#define ALCHEMY_MODULE_SCENE_GONDOWAN_CLIFFS_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct EffectOptions;

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

struct SwingActor {
  s16 id;
  s16 phase;
  s16 height;
  s16 period;
  s16 riseAt;
  s16 sinkAt;
  s16 top;
  s16 bottom;
  s16 unk10;
  s16 speed;
  s16 unk14;
  s16 bob;
  struct Actor *shadow;
};

struct GondowanCliffsWork {
  s16 entry;
  u16 value;
  u8 unk004[0x1c0];
  void *block;
};

void Func_020005bc(void);
void Func_02000600(void);

extern s32 gData_0200168c[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void Func_02001304(s32 a, s32 b, s32 c);

void Func_020012f4(void);

void Func_02000fcc(s32 a, s32 b);

void Func_020012fc(void);

#endif

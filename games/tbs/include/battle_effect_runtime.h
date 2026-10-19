#ifndef ALCHEMY_BATTLE_EFFECT_RUNTIME_H
#define ALCHEMY_BATTLE_EFFECT_RUNTIME_H

/// @file
/// The battle animation state, render objects and effect entries, and the
/// palette buffer work.

#include "unit/object/object.h"
#include "object_lookup.h"
#include "types.h"
#include "static_assert.h"
#include "palbuf.h"

struct BattleAnimationState {
  u8 unk00[0x28];
  s16 *value28;
};

struct BattleRenderObject {
  u8 unk00[0x24];
  s32 unk24;
  u8 unk28[4];
  s32 unk2c;
  s32 unk30;
  s32 unk34;
  s32 unk38;
  u8 unk3c[4];
  s32 unk40;
  u8 unk44[0x0c];
  struct BattleAnimationState *animation;
  u8 kind;
};

struct BattleEffectEntry {
  u16 id;
  u8 value;
  u8 flags;
};

extern struct BattleEffectBuffers *gPaletteBufferWork;

#endif

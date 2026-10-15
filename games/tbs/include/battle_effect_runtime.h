#ifndef ALCHEMY_BATTLE_EFFECT_RUNTIME_H
#define ALCHEMY_BATTLE_EFFECT_RUNTIME_H

#include "unit/object/object.h"
#include "object_lookup.h"
#include "types.h"
#include "layout_guard.h"
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

struct BattleRuntime {
  u8 unk000[0x1c8];
  s32 unk1c8;
  s32 mode1cc;
  u8 unk1d0[0x0a];
  u16 unk1da;
  s16 unk1dc;
  s16 unk1de;
  u8 unk1e0[0x14];
  s32 objectId;
  s32 unk1f8;
  u8 unk1fc[0xaba];
  s16 unkcb6;
  u8 unkcb8[0x0a];
  s16 unkcc2;
  s16 unkcc4;
};

struct BattleEffectEntry {
  u16 id;
  u8 value;
  u8 flags;
};

extern struct BattleEffectBuffers *gPaletteBufferWork;

#endif

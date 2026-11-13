#ifndef ALCHEMY_MODULE_FIELD_SCREEN_TRANSITION_H
#define ALCHEMY_MODULE_FIELD_SCREEN_TRANSITION_H

/// @file
/// What games/tla/src/field/object/screen_transition.c declares for the files
/// that use it, written once.

#include "types.h"
#include "battle/battle.h"

struct ObjectAnimationState;

struct BattleFxDefinition {
  u16 id;
  u8 value;
  u8 flags;
};

struct BattleRenderObject {
  u8 unk00[0x50];
  struct ObjectAnimationState *animation;
  u8 kind;
};

struct ParticleEmitter {
  u8 padding[8];
  struct ParticlePosition position;
  u8 padding2[20];
  s32 travelOffset;
  u8 padding3[41];
  u8 active;
};

struct RevealWork {
  u8 unk000[0xca4];
  s8 revealActive;
};

struct EmitterWork {
  u8 unk000[0x14];
  struct Actor *actors[(0xca4 - 0x14) / 4];
  s8 revealActive;
};

struct EffectVisual {
  u8 unk00[5];
  u8 flags05;
  u8 unk06[2];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[6];
  u8 tileSlot;
  u8 unk11[9];
  u8 value1a;
  u8 value1b;
};

struct ParticleSource {
  u8 padding[8];
  struct ParticlePosition position;
};

struct FadeActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u16 unk14;
  s16 floor;
  u8 unk18[10];
  u8 terrainId;
};

struct FadeView {
  u8 unk00[228];
  s16 camera[4];
};

struct TransitionSpan {
  u16 count;
  u16 value;
  u8 x0;
  u8 x1;
  u8 y0;
  u8 y1;
};

struct TransitionShape {
  u16 first;
  u16 second;
  struct TransitionSpan spans[3];
};

struct MapLayerFlags {
  u8 unk00[0x14];
  u16 flags;
};

extern struct BattleFxDefinition gData_080f0308[];

extern const u8 gData_080effd8[];

extern const u8 Data_080f01b8[];

extern const u8 gData_080f0074[];

extern u16 Data_080f0246[];

extern u16 Data_080f0286[];

extern u16 Data_080f02c6[];

extern const u8 Data_080f01c4[];

extern const s8 Data_080f01e6[];

extern struct TransitionShape Data_080f2e9c[];

extern const u8 gData_080f0206[];

extern char ResourceId_Headings;

#endif

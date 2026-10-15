#include "resource.h"
#ifndef KOLIMA_FOREST_TREE_H
#define KOLIMA_FOREST_TREE_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

#include "palette_scene.h"

struct EffectSprite {
  u8 pad00[9];
  u8 flags;
  u8 pad0a[28];
  u8 state;
};

struct PaletteEffect {
  u8 pad00[0x18];
  s32 progress;
  u8 pad1c[7];
  u8 flags;
  u8 pad24[12];
  s32 rateX;
  s32 rateY;
  u8 pad38[24];
  struct EffectSprite *sprite;
  u8 pad54[1];
  u8 mode;
};

struct OrbitingPaletteEffect {
  s32 pad00[2];
  s32 x;
  s32 y;
  s32 z;
  s32 pad14;
  s32 angleX;
  s32 angleY;
  s32 pad20[6];
  s32 anchorX;
  s32 anchorY;
  s32 anchorZ;
  s32 pad44[3];
  u8 *owner;
  s32 pad54[4];
  s16 step;
  s16 heading;
};

struct PaletteSceneRecord {
  u8 pad00[216];
  u16 values[1];
};

extern s32 gKolimaForestTreeSparkOrigin[3];
extern s32 gKolimaForestTreeSparkCount;
extern s32 gKolimaForestTreeSparkSound;
extern s32 gKolimaForestTreeTransitionStep;

#endif

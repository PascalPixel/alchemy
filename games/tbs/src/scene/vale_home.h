#include "resource.h"
#ifndef VALE_HOME_H
#define VALE_HOME_H

#include "types.h"
#include "vram_block.h"
#include "callback_scheduler.h"

#define NULL ((void *)0)

#include "field_event.h"
#include "field_scene.h"
#include "facing_object.h"
#include "field_effect.h"

#include "battle_effect_runtime.h"

struct Obj {
  u8 pad00[6];
  u16 f06;
  u8 pad08[0x30];
  s32 f38;
  s32 f3c;
  s32 f40;
};

struct VerticalEffectAnchor {
  u8 pad00[8];
  s32 x;
  u8 pad0c[4];
  s32 z;
};

struct SceneVerticalEffect {
  u8 pad00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 pad14[4];
  s32 amplitudeX;
  s32 amplitudeY;
  u8 pad20[0x44];
  u16 frame;
  u8 pad66[2];
  struct VerticalEffectAnchor *anchor;
};

struct PairDetail {
  u8 unk00[22];
  u8 field16;
};

struct PairSprite {
  struct FieldSprite sprite;
  struct PairDetail *detail;
};

union PairObject {
  union FieldObject object;
  s32 words[28];
  struct {
    u8 unk00[0x68];
    union PairObject *parent;
  } link;
};

struct SceneVerticalEffect;

struct PairWork {
  u8 unk00[70];
  u16 vramBlock;
};

struct WorldMapOam {
  u8 unk00[4];
  u16 attr0;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

#include "global_cells.h"

s32 Math_RemainderUnsigned();
void Engine_ActorSetSpriteFlags();

LAYOUT_OFFSET_GUARD(PairSprite_Detail, struct PairSprite, detail, 0x28);

LAYOUT_OFFSET_GUARD(PairObject_Parent, union PairObject, link.parent, 0x68);

#endif

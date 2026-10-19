#ifndef ALCHEMY_ANIMSPR_H
#define ALCHEMY_ANIMSPR_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "render_placement.h"

// AnimationEntry.param: how Render_DecodeFrame colours a layer's pixels.
// Modes 1 to 6 and 9 to 11 run them through a 256-byte table (the five
// gResourceSlot_ConversionTables, then the four gRuntime_ByteRemapTable
// ones); the solid modes paint every drawn pixel one colour of the basic
// sprite palette. Mode 8 is a second white and mode 12 reads past the tables.
enum SpriteColorMode {
  SPRITE_COLOR_NORMAL = 0,
  SPRITE_COLOR_GRB = 1,         // red and green swapped
  SPRITE_COLOR_RBG = 2,         // green and blue swapped
  SPRITE_COLOR_BGR = 3,         // red and blue swapped
  SPRITE_COLOR_GBR = 4,         // red to blue, green to red, blue to green
  SPRITE_COLOR_BRG = 5,         // red to green, green to blue, blue to red
  SPRITE_COLOR_GREY = 6,        // greyscale
  SPRITE_COLOR_WHITE = 7,       // solid colour 15, white
  SPRITE_COLOR_BLUE_TINT = 9,   // blue raised to full
  SPRITE_COLOR_RED_TINT = 10,   // red raised to full
  SPRITE_COLOR_GREEN_TINT = 11, // green raised to full
  SPRITE_COLOR_FILL = 13,       // solid gDecodeFillByte
  SPRITE_COLOR_BLACK = 14,      // solid colour 1, black
  SPRITE_COLOR_HIDDEN = 15      // solid colour 0, transparent
};

struct AnimationEntry {
  s16 animId;
  u16 timer;
  u8 kind;
  u8 param;
  u8 priority;
  u8 mode;
  void **frames;
  u8 **scripts;
  u8 *script;
  u8 pos;
  u8 step;
  u8 frame;
  u8 frameBase;
};

struct ResourceSlot {
  s32 tag;
  void *buffer;
};

struct ObjectSystemWork {
  u16 used;
  u8 unk02[2];
  s16 suspended;
  u8 edge;
  u8 fill;
  u8 unk08[0x10];
  s32 count;
  struct ResourceSlot slots[8];
};

STATIC_ASSERT(offsetof(struct ObjectSystemWork, suspended) == 4);
STATIC_ASSERT(offsetof(struct ObjectSystemWork, count) == 0x18);
STATIC_ASSERT(offsetof(struct ObjectSystemWork, slots) == 0x1c);
STATIC_ASSERT(sizeof(struct ObjectSystemWork) == 0x5c);

void Map_BuildProbeRing(s32 x, s32 z, struct SpritePlacement *points);

struct AnimationSpritePart {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
};

struct AnimationObject {
  struct AnimationSpritePart image;
  struct AnimationSpritePart shadow;
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk1d2 : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  struct AnimationEntry *entries[4];
};

STATIC_ASSERT(sizeof(struct AnimationEntry) == 0x18);
STATIC_ASSERT(sizeof(struct AnimationSpritePart) == 0x0c);
STATIC_ASSERT(offsetof(struct AnimationObject, scale) == 0x18);
STATIC_ASSERT(offsetof(struct AnimationObject, rotation) == 0x1e);
STATIC_ASSERT(offsetof(struct AnimationObject, width) == 0x20);
STATIC_ASSERT(sizeof(struct AnimationObject) == 0x38);
STATIC_ASSERT(offsetof(struct AnimationObject, entries) == 0x28);

#endif

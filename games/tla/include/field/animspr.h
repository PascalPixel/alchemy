#ifndef ALCHEMY_ANIMSPR_H
#define ALCHEMY_ANIMSPR_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "render_placement.h"

/// AnimationEntry.param: how Render_DecodePicture colours a layer's pixels.
/// Modes 1 to 6 and 9 to 11 run them through a 256-byte table (the five
/// gResourceSlot_ConversionTables, then the four gRuntime_ByteRemapTable
/// ones); the solid modes paint every drawn pixel one colour of the basic
/// sprite palette. Mode 8 is a second white and mode 12 reads past the tables.
enum SpriteColorMode {
  SPRITE_COLOR_NORMAL = 0,
  SPRITE_COLOR_GRB = 1,         ///< red and green swapped
  SPRITE_COLOR_RBG = 2,         ///< green and blue swapped
  SPRITE_COLOR_BGR = 3,         ///< red and blue swapped
  SPRITE_COLOR_GBR = 4,         ///< red to blue, green to red, blue to green
  SPRITE_COLOR_BRG = 5,         ///< red to green, green to blue, blue to red
  SPRITE_COLOR_GREY = 6,        ///< greyscale
  SPRITE_COLOR_WHITE = 7,       ///< solid colour 15, white
  SPRITE_COLOR_BLUE_TINT = 9,   ///< blue raised to full
  SPRITE_COLOR_RED_TINT = 10,   ///< red raised to full
  SPRITE_COLOR_GREEN_TINT = 11, ///< green raised to full
  SPRITE_COLOR_FILL = 13,       ///< solid gDecodeFillByte
  SPRITE_COLOR_BLACK = 14,      ///< solid colour 1, black
  SPRITE_COLOR_HIDDEN = 15      ///< solid colour 0, transparent
};

/// One animation layer of a sprite: its animation id, pictures, running script
/// and timer.
struct AnimationEntry {
  s16 animId; ///< the sprite id whose metadata and pictures it uses
  /// Time left on the current picture in sixteenths of a frame, read signed:
  /// each timed command adds its argument times 16, each frame takes off step.
  u16 timer;
  /// The facing table its pictures follow (from the metadata, or
  /// animation command 240).
  u8 kind;
  u8 param;      ///< SpriteColorMode its pixels are drawn with
  u8 priority;   ///< drawing order 0 to 3 among the layers; above 3 not drawn
  u8 mode;       ///< picture codec: 1 type-01, 3 LZ, anything else raw
  void **frames; ///< picture pointers, from the metadata or a resource slot
  u8 **scripts;  ///< the metadata's command scripts, one per animation number
  u8 *script;    ///< the running script; null once command 239 ends it
  u8 pos;        ///< byte offset of the next command in script
  u8 step;       ///< taken off timer each frame; 16 is normal speed
  u8 frame;      ///< picture shown, frameBase plus the facing offset; 255 none
  u8 frameBase;  ///< picture the script chose, before facing; 255 blank
};

/// A resource slot: the tag of the animation loaded into it (slot number and
/// animation id) and the buffer holding its pictures.
struct ResourceSlot {
  s32 tag;      ///< (slot << 12) | animation id
  void *buffer; ///< the decoded pictures, their offsets made pointers
};

/// The object system's work: the flag that suspends sprite projection, the
/// outline's edge and fill colours, and the resource slots.
struct ObjectSystemWork {
  u16 used;
  u8 unk02[2];
  s16 suspended; ///< nonzero while a menu has suspended sprite projection
  u8 edge;       ///< outline edge colour
  u8 fill;       ///< outline fill colour
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

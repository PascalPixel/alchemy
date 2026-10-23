#ifndef ALCHEMY_ANIMSPR_H
#define ALCHEMY_ANIMSPR_H

/// @file
/// Animated sprites: their layers, frames and colour modes, and the resource
/// slots they draw from.

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "render_placement.h"

/// SpriteAnim.colorMode: how Render_DecodeFrame colours a layer's pixels.
/// Modes 1 to 6 and 9 to 11 run them through a 256-byte table (the five
/// gSpriteColorTables, then the four gRuntime_ByteRemapTable
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

/// One of a sprite's four sprite anims: its animation id, frames, running
/// script and timer.
struct SpriteAnim {
  s16 animationId; ///< the sprite id whose metadata and frames it uses
  /// Time left on the current frame in sixteenths of a frame, read signed:
  /// each timed command adds its argument times 16, each frame takes off step.
  u16 timer;
  /// The facing table its frames follow (from the metadata, or
  /// ANIMCMD_SET_KIND); zero marks a free sprite anim.
  u8 kind;
  u8 colorMode;    ///< SpriteColorMode its pixels are drawn with; 0 when taken
  u8 priority;     ///< drawing order 0 to 3 among the anims; above 3 not drawn
  u8 mode;         ///< frame codec: 1 type-01, 3 LZ, anything else raw
  void **frames;   ///< frame pointers, from the metadata or a frame cache
  u8 **scripts;    ///< the metadata's command scripts, one per animation number
  u8 *script;      ///< the running script; null when stopped (ANIMCMD_END)
  u8 scriptOffset; ///< byte offset of the next command in script
  u8 step;         ///< taken off timer each frame; 16 is normal speed
  u8 frame;        ///< frame shown, frameBase plus the facing offset; 255 none
  u8 frameBase;    ///< frame the script chose, before facing; 255 blank
};

/// A sprite frame cache: the tag of the animation loaded into it (slot number
/// and animation id) and the buffer holding its frames.
struct SpriteFrameCache {
  s32 tag;      ///< (slot << 12) | animation id
  void *frames; ///< the decoded frames, their offsets made pointers
};

/// The object system's work: the flag that suspends sprite projection, the
/// outline's edge and fill colours, and the sprite frame caches. Heap slot 6.
struct ObjectSystemWork {
  u16 used;      ///< sprite pixel bytes composed since the camera reset it
  u8 unused[2];  ///< no code names it
  s16 suspended; ///< nonzero while a menu has suspended sprite projection
  u8 edge;       ///< outline edge colour; 15 when the field sets up
  u8 fill;       ///< outline fill colour; 0 when the field sets up
  u8 spriteGroups[0x10]; ///< the sprite group tables, taken in turn by count
  s32 count;             ///< sprite group tables taken from spriteGroups
  struct SpriteFrameCache
      frameCaches[8]; ///< found by tag (SpriteFrameCache_FindByTag)
};

STATIC_ASSERT(offsetof(struct ObjectSystemWork, suspended) == 4);
STATIC_ASSERT(offsetof(struct ObjectSystemWork, count) == 0x18);
STATIC_ASSERT(offsetof(struct ObjectSystemWork, frameCaches) == 0x1c);
STATIC_ASSERT(sizeof(struct ObjectSystemWork) == 0x5c);

/// The hardware sprite attributes one part of an animated sprite (its image or
/// its shadow) is drawn with.
struct AnimationSpritePart {
  /// The link to the next part queued in the same depth slot this frame
  /// (Oam_AddSpriteAtDepth).
  u8 link[4];
  /// The OAM entry's attributes 0 to 2.
  ///@{
  u16 y : 8;          ///< screen pixels
  u16 affine : 2;     ///< 0 plain, 1 affine, 3 affine at double size
  u16 objectMode : 2; ///< an enum ObjMode
  u16 mosaic : 1;
  u16 fullColor : 1;   ///< 256 colours; set for both parts when created
  u16 shape : 2;       ///< 0 square, 1 wide, 2 tall
  u16 x : 9;           ///< screen pixels
  u16 affineIndex : 5; ///< affine matrix, or 8 for a mirrored plain sprite
  u16 size : 2;        ///< with shape, the sprite's size
  u16 tile : 10;       ///< first sprite tile in VRAM
  u16 priority : 2;    ///< 2 when created
  u16 palette : 4;     ///< 16-colour palette; unused at 256 colours
  ///@}
  u8 attr3[2]; ///< OAM attribute 3; zero when created, not named by TBS code
};

/// An animated sprite: its image and shadow, scale, rotation, size and up to
/// four sprite anims.
struct Sprite {
  struct AnimationSpritePart image;
  /// Its shadow, shown while flags bit 0 is set.
  struct AnimationSpritePart shadow;
  s32 scale; ///< 16.16 fixed point, 0x10000 full size
  u8 slot;   ///< VRAM cache entry holding its tiles
  /// Keeps the VRAM cache entry when it is released or off screen.
  u8 retainCacheEntry : 1;
  /// Drawn at its own scale, not scaled by distance (fixed camera).
  u8 useOwnScale : 1;
  u8 unusedFlags : 6; ///< no code names them
  u16 rotation;       ///< 1/65536 turns
  u8 width;           ///< pixels; zero marks a free sprite
  u8 height;          ///< pixels
  s8 offsetX;         ///< pixels
  s8 offsetY;         ///< pixels
  /// The animation number last selected, which selecting again skips.
  u8 lastNo;
  u8 dirty; ///< nonzero redraws its pixels into VRAM when next drawn
  /// bits: 1 outlined in ObjectSystemWork's edge and fill colours |
  /// 0 shadow drawn (set when created)
  u8 flags;
  u8 count;                      ///< layers in use, up to the last one present
  struct SpriteAnim *entries[4]; ///< its layers; empty ones null
};

STATIC_ASSERT(sizeof(struct SpriteAnim) == 0x18);
STATIC_ASSERT(sizeof(struct AnimationSpritePart) == 0x0c);
STATIC_ASSERT(offsetof(struct Sprite, scale) == 0x18);
STATIC_ASSERT(offsetof(struct Sprite, rotation) == 0x1e);
STATIC_ASSERT(offsetof(struct Sprite, width) == 0x20);
STATIC_ASSERT(sizeof(struct Sprite) == 0x38);
STATIC_ASSERT(offsetof(struct Sprite, entries) == 0x28);

#endif

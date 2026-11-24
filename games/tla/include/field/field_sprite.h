#ifndef ALCHEMY_FIELD_SPRITE_H
#define ALCHEMY_FIELD_SPRITE_H

/// @file
/// An actor's sprite as the field's scenes read it.

#include "fixed_math.h"
#include "types.h"

/// struct Sprite (field/sprite.h) as the field's scenes read it: its image's
/// OAM attributes as bit fields, its retain, own-scale and sunk-rows bits as
/// one byte, its layer count and its shadow's first two attributes as bytes,
/// and its shadow's attribute 2 as bit fields; the first layer is record.
struct FieldSprite {
  u8 unused[4]; ///< no code names it
  /// The image's OAM attributes 0 to 2.
  ///@{
  u16 y : 8; ///< screen pixels
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1; ///< 256 colours
  u16 shape : 2;
  u16 x : 9; ///< screen pixels
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10; ///< first sprite tile in VRAM
  u16 priority : 2;
  u16 palette : 4;
  ///@}
  u8 unusedAfterPalette[2]; ///< no code names it
  FIXED scale;              ///< 16.16 fixed point, FIXED_ONE full size
  u8 slot;                  ///< VRAM cache entry holding its tiles
  /// Sprite's retainCacheEntry (bit 0), useOwnScale (bit 1) and sunkRows
  /// (bits 2 to 7) as one byte.
  u8 cacheBits;
  u16 rotation; ///< 1/65536 turns
  u8 width;     ///< pixels; zero marks a free sprite
  u8 height;    ///< pixels
  s8 offsetX;   ///< pixels
  s8 offsetY;   ///< pixels
  /// The animation number last selected, saved with the actor's snapshot.
  u8 lastNo;
  u8 dirty; ///< nonzero redraws its pixels into VRAM when next drawn
  /// The sprite flags (enum SpriteFlag), saved with the actor's snapshot;
  /// zeroed for sprites shown without them.
  u8 flags;
  /// Byte 0 is the layer count (Sprite.count), cleared with the first layer;
  /// then the shadow's link and attributes 0 and 1.
  u8 countAndShadow[9];
  /// The shadow's OAM attribute 2.
  ///@{
  u16 secondTile : 10;
  u16 partPriority : 2;
  u16 secondPalette : 4;
  ///@}
  u8 unusedAfterSecondPalette[2]; ///< no code names it
  void *record;                   ///< released by SpriteAnimation_ClearFar
};

/// The depth a sprite the scene draws itself takes past its screen row, just
/// behind an actor's image (+ PLACEMENT_IMAGE_DEPTH, field/field.h) and shadow
/// (two slots less) at the same spot: light sprites, a hole's mask, map cell
/// sprites, fountains and ropes.
#define SCENE_SPRITE_DEPTH 58

#endif

#ifndef ALCHEMY_SPRITE_METADATA_H
#define ALCHEMY_SPRITE_METADATA_H

/// @file
/// A sprite's animation metadata, its row of gSprite_AnimationMetadata.

#include "types.h"
#include "static_assert.h"
#include "edition.h"

/// An animation's metadata: the sprite's size, scale and offsets, how its
/// pictures are drawn and decoded, its pictures and its animation scripts.
struct AnimationMetadata {
  u8 width;  ///< pixels; 0 marks a row with no sprite
  u8 height; ///< pixels
  u16 scale; ///< 8.8 fixed point, 256 full size (a Sprite's scale over 256)
  /// The AnimationFacingKind: which facing table turns its pictures.
  u8 drawKind;
  u8 animationCount; ///< scripts in animation
  s8 adjustX;        ///< pixels the sprite is drawn off its position
  s8 adjustY;        ///< pixels the sprite is drawn off its position
  /// Pixels from the actor's feet up to the top of its sprite, read signed:
  /// where Mind Read's line starts, and what an actor carried on top of
  /// another is raised by.
  u8 spriteTop;
  u8 footprint; ///< pixels across the actor stands on, twice its radius
  /// How its pictures are packed: 1 type-01, 3 LZ, anything else raw.
  u8 pictureCodec;
  void **pictures; ///< null for a sprite whose pictures a cache loads
  u8 **animation;  ///< its command scripts, one per animation number
};

STATIC_ASSERT(sizeof(struct AnimationMetadata) == 20);
STATIC_ASSERT(offsetof(struct AnimationMetadata, scale) == 2);
STATIC_ASSERT(offsetof(struct AnimationMetadata, animationCount) == 5);
STATIC_ASSERT(offsetof(struct AnimationMetadata, spriteTop) == 8);
STATIC_ASSERT(offsetof(struct AnimationMetadata, footprint) == 9);
STATIC_ASSERT(offsetof(struct AnimationMetadata, pictureCodec) == 10);
STATIC_ASSERT(offsetof(struct AnimationMetadata, pictures) == 12);
STATIC_ASSERT(offsetof(struct AnimationMetadata, animation) == 16);

/// Looks up animation metadata using the descriptor ID in the low twelve bits:
/// that row of gSprite_AnimationMetadata (graphics/sprite/sprites.s), with no
/// bounds check.
struct AnimationMetadata *Resource_GetMetadataRecord(u32 number);
struct AnimationMetadata *Resource_GetMetadataRecordFar(u32 number);
struct AnimationMetadata *Resource_GetMetadataRecordFarFar(u32 number);

#endif

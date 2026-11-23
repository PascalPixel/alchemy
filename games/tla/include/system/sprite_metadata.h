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
  u8 width;
  u8 height;
  u16 scale;
  u8 drawKind;
  u8 animationCount;
  s8 adjustX;
  s8 adjustY;
  u8 boxX;
  u8 boxY;
  u8 pictureCodec;
  void **pictures;
  u8 **animation;
};

STATIC_ASSERT(sizeof(struct AnimationMetadata) == 20);
STATIC_ASSERT(offsetof(struct AnimationMetadata, scale) == 2);
STATIC_ASSERT(offsetof(struct AnimationMetadata, animationCount) == 5);
STATIC_ASSERT(offsetof(struct AnimationMetadata, boxX) == 8);
STATIC_ASSERT(offsetof(struct AnimationMetadata, boxY) == 9);
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

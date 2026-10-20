#ifndef ALCHEMY_METADATA_LOOKUP_H
#define ALCHEMY_METADATA_LOOKUP_H

/// @file
/// Animation metadata and its lookup by resource.

#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "farcall.h"

/// An animation's metadata: the sprite's size, scale and offsets, how its
/// frames are drawn and decoded, its frames and its animation scripts.
struct AnimationMetadata {
  u8 width;    ///< pixels
  u8 height;   ///< pixels
  u16 scale;   ///< 8.8 fixed point, 0x100 full size
  u8 drawKind; ///< AnimationEntry.kind: the facing table its frames follow
  /// The scripts in animation; an animation number at or past it is not
  /// played.
  u8 animationCount;
  s8 adjustX; ///< pixels
  s8 adjustY; ///< pixels
  /// Pixels from the object's position up to the top of its sprite
  /// (Object_GetScreenPosition).
  u8 boxX;
  /// Pixels; half of it is a battle unit's MotionObject.height, and can be a
  /// field object's radius (FieldActor.radius).
  u8 boxY;
  u8 frameCodec; ///< AnimationEntry.mode: 1 type-01, 3 LZ, anything else raw
  /// The frame pointers; null takes them from the resource slot holding the
  /// animation (Animation_LookupValueByKey).
  void **frames;
  u8 **animation; ///< the command scripts, one per animation number
};

STATIC_ASSERT(sizeof(struct AnimationMetadata) == 0x14);
STATIC_ASSERT(offsetof(struct AnimationMetadata, scale) == 0x02);
STATIC_ASSERT(offsetof(struct AnimationMetadata, animationCount) == 0x05);
STATIC_ASSERT(offsetof(struct AnimationMetadata, boxX) == 0x08);
STATIC_ASSERT(offsetof(struct AnimationMetadata, boxY) == 0x09);
STATIC_ASSERT(offsetof(struct AnimationMetadata, frameCodec) == 0x0a);
STATIC_ASSERT(offsetof(struct AnimationMetadata, frames) == 0x0c);
STATIC_ASSERT(offsetof(struct AnimationMetadata, animation) == 0x10);

struct AnimationMetadata *Resource_GetMetadataRecordFarFar(u32 number);

#endif

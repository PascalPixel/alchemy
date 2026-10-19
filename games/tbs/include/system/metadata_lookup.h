#ifndef ALCHEMY_METADATA_LOOKUP_H
#define ALCHEMY_METADATA_LOOKUP_H

#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "farcall.h"

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
  u8 frameCodec;
  void **frames;
  u8 **animation;
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

#ifndef ALCHEMY_METADATA_LOOKUP_H
#define ALCHEMY_METADATA_LOOKUP_H

#include "types.h"
#include "layout_guard.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

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
  u8 reserved0b;
  void **frames;
  u8 **animation;
};

LAYOUT_SIZE_GUARD(AnimationMetadata_Size, struct AnimationMetadata, 0x14);
LAYOUT_OFFSET_GUARD(AnimationMetadata_Scale, struct AnimationMetadata, scale,
                    0x02);
LAYOUT_OFFSET_GUARD(AnimationMetadata_Count, struct AnimationMetadata,
                    animationCount, 0x05);
LAYOUT_OFFSET_GUARD(AnimationMetadata_BoxX, struct AnimationMetadata, boxX,
                    0x08);
LAYOUT_OFFSET_GUARD(AnimationMetadata_BoxY, struct AnimationMetadata, boxY,
                    0x09);
LAYOUT_OFFSET_GUARD(AnimationMetadata_Codec, struct AnimationMetadata,
                    frameCodec, 0x0a);
LAYOUT_OFFSET_GUARD(AnimationMetadata_Frames, struct AnimationMetadata, frames,
                    0x0c);
LAYOUT_OFFSET_GUARD(AnimationMetadata_Animation, struct AnimationMetadata,
                    animation, 0x10);

#if GAME_TLA
struct AnimationMetadata *Resource_GetMetadataRecord(u32 number);
#endif
#if GAME_TLA
struct AnimationMetadata *Resource_GetMetadataRecordFar(u32 number);
#endif
struct AnimationMetadata *Resource_GetMetadataRecordFarFar(u32 number);

#endif

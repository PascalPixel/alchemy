#ifndef ALCHEMY_METADATA_LOOKUP_H
#define ALCHEMY_METADATA_LOOKUP_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* SPRITES.TSV emits these fields and the two list-address words. */
struct AnimationMetadata {
    u8 width;
    u8 height;
    u16 scale;
    u8 draw_kind;
    u8 animation_count;
    s8 adjust_x;
    s8 adjust_y;
    u8 box_x;
    u8 box_y;
    u8 frame_codec;
    u8 reserved_0b;
    /* The native initializers transport these list addresses as words. */
    s32 frames;
    s32 animation;
};

LAYOUT_SIZE_GUARD(AnimationMetadata_Size, struct AnimationMetadata, 0x14);
LAYOUT_OFFSET_GUARD(AnimationMetadata_Scale, struct AnimationMetadata, scale, 0x02);
LAYOUT_OFFSET_GUARD(AnimationMetadata_Count, struct AnimationMetadata, animation_count, 0x05);
LAYOUT_OFFSET_GUARD(AnimationMetadata_BoxX, struct AnimationMetadata, box_x, 0x08);
LAYOUT_OFFSET_GUARD(AnimationMetadata_BoxY, struct AnimationMetadata, box_y, 0x09);
LAYOUT_OFFSET_GUARD(AnimationMetadata_Codec, struct AnimationMetadata, frame_codec, 0x0a);
LAYOUT_OFFSET_GUARD(AnimationMetadata_Frames, struct AnimationMetadata, frames, 0x0c);
LAYOUT_OFFSET_GUARD(AnimationMetadata_Animation, struct AnimationMetadata, animation, 0x10);

struct AnimationMetadata *Resource_GetMetadataRecord(u32 number);
struct AnimationMetadata *Resource_GetMetadataRecordFar(u32 number);
struct AnimationMetadata *Resource_GetMetadataRecordFarFar(u32 number);

#endif

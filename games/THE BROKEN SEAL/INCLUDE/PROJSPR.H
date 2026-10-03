#ifndef ALCHEMY_PROJSPR_H
#define ALCHEMY_PROJSPR_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

struct ProjectedSpritePart {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 object_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 unknown_7 : 2;
    u8 unknown_08[4];
};

struct ProjectedSprite {
    struct ProjectedSpritePart part[2];
    s32 scale;
    u8 vram_block;
    u8 flags;
    u16 rotation;
    u8 width;
    u8 height;
    s8 offset_x;
    s8 offset_y;
    u8 unknown_24;
    u8 hidden;
    u8 shadow_flags;
};

LAYOUT_SIZE_GUARD(ProjectedSpritePart_Size, struct ProjectedSpritePart, 0x0c);
LAYOUT_SIZE_GUARD(ProjectedSprite_PrefixSize, struct ProjectedSprite, 0x28);
LAYOUT_OFFSET_GUARD(ProjectedSprite_Scale, struct ProjectedSprite, scale, 0x18);
LAYOUT_OFFSET_GUARD(ProjectedSprite_Width, struct ProjectedSprite, width, 0x20);

#endif

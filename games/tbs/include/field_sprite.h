#ifndef ALCHEMY_FIELD_SPRITE_H
#define ALCHEMY_FIELD_SPRITE_H

#include "types.h"
#include "layout_guard.h"

struct FieldSprite {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 flip_x : 1;
    u16 flip_y : 1;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[0x0a];
    u16 second_tile : 10;
    u16 second_priority : 2;
    u16 second_palette : 4;
    u8 unknown_16[2];
    s32 scale;
    u8 vram_block;
    u8 unknown_1d;
    u16 rotation;
    u8 unknown_20[6];
    u8 flags;
    u8 part_count;
};

LAYOUT_OFFSET_GUARD(FieldSprite_Scale, struct FieldSprite, scale, 0x18);
LAYOUT_OFFSET_GUARD(FieldSprite_VramBlock, struct FieldSprite, vram_block, 0x1c);
LAYOUT_OFFSET_GUARD(FieldSprite_Rotation, struct FieldSprite, rotation, 0x1e);
LAYOUT_OFFSET_GUARD(FieldSprite_PartCount, struct FieldSprite, part_count, 0x27);

struct FieldSpritePriority {
    u8 unknown_00[9];
    u8 tile_high : 2;
    u8 priority : 2;
    u8 palette : 4;
};

#endif

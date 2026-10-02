#ifndef ALCHEMY_FIELD_SPRITE_H
#define ALCHEMY_FIELD_SPRITE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/*
 * The sprite an object draws. It keeps the three GBA sprite attribute words
 * the engine copies into OAM.
 */
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
    /* Sprites with a lower priority draw above the background layers. */
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[0x0a];
    /* The third attribute word of a two-part sprite's second part. */
    u16 second_tile : 10;
    u16 second_priority : 2;
    u16 second_palette : 4;
    u8 unknown_16[2];
    /* The 16.16 factor an actor's contact radius is scaled by. */
    s32 scale;
    /* The VRAM block that holds the sprite's frames. */
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


#endif

#ifndef TLA_FIELD_SPRITE_H
#define TLA_FIELD_SPRITE_H

#include "TYPES.H"

/* The sprite an object draws; its two parts each carry a priority. */
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
    u8 unknown_0a[6];
    /* The VRAM block that holds the sprite's frames. */
    u8 vram_block;
    u8 unknown_11;
    /* The sprite's angle, in 0x10000ths of a turn. */
    u16 rotation;
    u8 unknown_14[4];
    /* The animation frame and the one after it. */
    u8 frame;
    u8 unknown_19;
    u8 next_frame;
    u8 unknown_1b[9];
    /* The third attribute word of a two-part sprite's second part. */
    u16 second_tile : 10;
    u16 part_priority : 2;
    u16 second_palette : 4;
};

#endif

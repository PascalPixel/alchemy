#ifndef TLA_FIELD_SPRITE_H
#define TLA_FIELD_SPRITE_H

#include "TYPES.H"

/* The sprite an object draws; its two parts each carry a priority. */
struct FieldSprite {
    u8 unknown_00[9];
    u8 unknown_09 : 2;
    u8 priority : 2;
    u8 unknown_09_high : 4;
    u8 unknown_0a[8];
    u16 unknown_12;
    u8 unknown_14[6];
    u8 unknown_1a;
    u8 unknown_1b[10];
    u8 unknown_25 : 2;
    u8 part_priority : 2;
    u8 unknown_25_high : 4;
};

#endif

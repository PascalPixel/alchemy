/* Near miss, differ 2 of 57 (2026-10-15). The shape is (attributes >> 30) |
 * (attributes >> 12), as the listing shifts. One pair: the listing loads the size
 * table's address before it starts the 15 mask; this masks first. A table local, and
 * single and paired barriers, measured no better. */
#include "types.h"
#include "iwram_call.h"

struct EffectSprite {
    u8 unknown_00[4];
    u16 attr0_0 : 13;
    u16 color : 1;
    u16 attr0_14 : 2;
    u16 attr1;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[2];
    s32 x;
    s32 y;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_1c[2];
    u16 depth;
    u16 width;
    u16 height;
    u16 unknown_24;
    u16 unknown_26;
};

extern const u8 Data_080f3bc4[][2];

void Func_080eaf98(struct EffectSprite *sprite, s32 a, s32 b, u32 attributes, s32 tiles)
{
    u32 *words;
    s32 shape;

    if (sprite == NULL)
        return;
    Iwram_FillWords(sprite, 40, 0);
    sprite->scale_x = 0x10000;
    sprite->scale_y = 0x10000;
    sprite->depth = 255;
    sprite->unknown_24 = a;
    sprite->unknown_26 = b;
    shape = ((attributes >> 30) | (attributes >> 12)) & 15;
    sprite->width = Data_080f3bc4[shape][0];
    sprite->height = Data_080f3bc4[shape][1];
    words = (u32 *)&sprite->unknown_00[4];
    *words++ = attributes;
    *words = 0;
    sprite->color = 1;
    sprite->tile = tiles;
}

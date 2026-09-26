/* NONMATCHING: 236 of 232 bytes, 82 differing halfwords, 51 aligned halfword
 * edits (2026-09-26). Same loops
 * as MENU/TITLE/SPRITE_ROW.C. Remaining: the reference loads the counter
 * twice before the test (ldrsh for the test, ldrh for an SImode decrement)
 * where ours decrements in HImode through a pooled 0xffff; ours then threads
 * the first loop iteration past its reload (b into the loop), and the
 * tile/255 registers differ. A signed decrement shrank to 228 bytes but did
 * not recover the unsigned second load. An explicit word cast and goto loop
 * compiled like the original draft; retain its ordinary for loop. */
#include "TYPES.H"

struct VramBlock {
    u16 base;
    u16 offset;
};

struct Sprite {
    u32 words[3];
};

extern struct VramBlock Data_03001b10[];
extern s16 Data_02009c1a;
extern s16 Data_02009c18;
extern u32 Data_02009af8[];

void Main_080001e8(struct Sprite *sprite, s32 value);

void Local_020011c4(void)
{
    u32 *w;
    struct Sprite *p;
    s16 *count;
    s32 tile;
    u32 i;
    s32 v;
    s32 y;

    tile = Data_03001b10[Data_02009c1a].offset >> 5;
    count = &Data_02009c18;
    w = Data_02009af8;
    if (*count != 0) {
        (*(u16 *)count)--;
    }
    for (i = 0; i < 8; i++) {
        v = *count;
        *w++ = 0;
        *w++ = (-v / 2 & 0xff) | (i << 21) | 0x80004000;
        *w++ = tile;
    }
    y = (v / 2 + 0x88) & 0xff;
    for (i = 0; i < 8; i++) {
        w[0] = 0;
        w[1] = (i << 21) | y | 0x80004000;
        w[2] = tile;
        w += 3;
    }
    y = (Data_02009c18 / 2 + 0x98) & 0xff;
    for (i = 0; i < 8; i++) {
        w[0] = 0;
        w[1] = (i << 21) | y | 0x80004000;
        w[2] = tile;
        w += 3;
    }
    p = (struct Sprite *)Data_02009af8;
    for (i = 0; i < 24; i++) {
        Main_080001e8(p++, 255);
    }
}

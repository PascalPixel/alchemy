/* NONMATCHING: 236 of 232 bytes, 114 differing halfwords, 62 aligned edits.
 * 2026-09-27: the complete pool proves the 24 twelve-byte records end at
 * count +0x120, followed by the VRAM id +0x122. Modelling these as one
 * state restores the word decrement but folds the three independent pool
 * addresses into one base and offsets. It still forwards the decremented
 * value into the first iteration instead of reloading. Rejected ownership
 * model; the separate-global baseline is in the preceding commit.
 * Earlier baseline: 236/232 bytes, 82 halfwords, 51 aligned edits. Same loops
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

struct SpriteRowState {
    struct Sprite sprites[24];
    s16 count;
    s16 block;
};

extern struct VramBlock Data_03001b10[];
extern struct SpriteRowState Data_02009af8;

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

    tile = Data_03001b10[Data_02009af8.block].offset >> 5;
    count = &Data_02009af8.count;
    w = Data_02009af8.sprites[0].words;
    if (*count != 0) {
        Data_02009af8.count--;
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
    y = (Data_02009af8.count / 2 + 0x98) & 0xff;
    for (i = 0; i < 8; i++) {
        w[0] = 0;
        w[1] = (i << 21) | y | 0x80004000;
        w[2] = tile;
        w += 3;
    }
    p = Data_02009af8.sprites;
    for (i = 0; i < 24; i++) {
        Main_080001e8(p++, 255);
    }
}

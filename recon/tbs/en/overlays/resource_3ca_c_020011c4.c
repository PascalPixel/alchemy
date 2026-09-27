/* NONMATCHING: H1 ship 2026-09-27, 236 of 232 bytes, 104 differing
 * halfwords, 69 aligned edits. Volatile counter pointer restores the loop
 * reload but adds r8 saves, a second decrement load and a 0xffff pool word;
 * it does not emit the required unconditional ldrsh/ldrh then word subtract.
 * Full normalized diff read. Rejected volatile-pointer ownership model.
 * Previous baseline: 236 of 232 bytes, 82 differing halfwords, 51 aligned edits.
 * 2026-09-27: the complete pool proves the 24 twelve-byte records end at
 * count +0x120, followed by the VRAM id +0x122. Modelling these as one
 * state restores the word decrement but folds the three independent pool
 * addresses into one base and offsets. It still forwards the decremented
 * value into the first iteration instead of reloading. Rejected ownership
 * model preserved in 6fd230b5d. Separate signed/unsigned union views of the
 * counter emit exactly the separate-global baseline bytes (cmp confirmed):
 * the unsigned decrement still uses the pooled 0xffff and forwards its
 * value into the first iteration. Keep separate globals and these views;
 * stop the state-layout/counter-view axis after both structural trials.
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

union RowCounter {
    s16 signed_value;
    u16 value;
};

extern struct VramBlock Data_03001b10[];
extern s16 Data_02009c1a;
extern union RowCounter Data_02009c18;
extern u32 Data_02009af8[];

void Main_080001e8(struct Sprite *sprite, s32 value);

void Local_020011c4(void)
{
    u32 *w;
    struct Sprite *p;
    volatile union RowCounter *count;
    s32 tile;
    u32 i;
    s32 v;
    s32 y;

    tile = Data_03001b10[Data_02009c1a].offset >> 5;
    count = &Data_02009c18;
    w = Data_02009af8;
    if (count->signed_value != 0) {
        count->value--;
    }
    for (i = 0; i < 8; i++) {
        v = count->signed_value;
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
    y = (Data_02009c18.signed_value / 2 + 0x98) & 0xff;
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

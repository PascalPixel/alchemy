/*
 * Draft: Title_RevealSpriteRow for overlay 649, ☀️'s Japanese and English
 * title row (eighteen sprites at y 0x88) writing each record through a
 * sprite pointer. 36 rows differ: ⚓️ rebuilds 0x88 inside the loop and ors
 * it before 0x8400, keeps the tile in r5 and the index in r7; this C holds
 * 0x88 in r8. With the constants written together GCC folds them (39
 * rows). It follows Title_LoadSprites into OVERLAY_649/PREPARE.C.
 * Links as resource_649 .text.x02008278.
 */
#include "TYPES.H"
#include "VRAM_TAB.H"

struct TitleSprite {
    u32 attr[3];
};

extern s16 gTitleVramBlock;
extern s16 gTitleRevealFrame;
extern struct TitleSprite gTitleSprites[];
extern u32 gFrameCount;

void Func_020005ac(struct TitleSprite *sprite, s32 priority);

/* Rebuilds the row of eighteen title sprites; one more shows every two
   frames, and the newest two blink with the frame counter. */
void Title_RevealSpriteRow(void)
{
    struct TitleSprite *w;
    struct TitleSprite *p;
    s32 tile;
    s32 i;
    s32 n;
    s32 x;
    s32 y;

    p = gTitleSprites;
    tile = ResourceTableEntries[gTitleVramBlock].offset >> 5;
    w = p;
    y = 0x88;
    for (i = 0; i <= 17; i++) {
        x = 232 - (18 - i) * 8;
        w->attr[0] = 0;
        w->attr[1] = (x << 16) | y | 0x8400;
        w->attr[2] = 0xf000 | tile;
        w++;
        n = gTitleRevealFrame / 2 - i;
        if (n < 0)
            n = 0;
        if (n <= 2 && (gFrameCount & 1))
            n = 0;
        if (n != 0)
            Func_020005ac(p++, 255);
        tile += 2;
    }
    gTitleRevealFrame++;
}

#include "types.h"
#include "vram_tab.h"

struct TitleSprite {
    u32 attr[3];
};

extern s16 gTitleVramBlock;
extern s16 gTitleRevealFrame;
extern struct TitleSprite gTitleSprites[];
extern u32 gFrameCount;

void Func_020005ac(struct TitleSprite *sprite, s32 priority);

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

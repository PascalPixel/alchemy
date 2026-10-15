#include "types.h"
#include "field_scene.h"
#include "resource.h"
#include "dma.h"
#include "mem.h"
#include "io_reg.h"
#include "vram_tab.h"

struct TitleSprite {
    u32 attr[3];
};

extern char ResourceId_IntroTilesB;
extern s16 gTitleVramBlock;
extern s16 Data_020006dc;
extern struct TitleSprite Data_020006f0[];
extern u32 gFrameCount;

void Func_020005ac(struct TitleSprite *sprite, s32 priority);

s32 Scene_PrepareMap(void)
{
    return 0;
}

void Title_LoadSprites(s32 unused)
{
    u8 *buffer;
    volatile s32 *dma;

    buffer = Runtime_BumpAllocateAlternatePool(0x520);
    if (gTitleVramBlock == -1)
        gTitleVramBlock = Resource_FindFreeEntry();
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_IntroTilesB), buffer);
    Dma_Set(buffer, (void *)0x050003e0, 0x84000008, REG_DMA3);
    VramBlock_LoadCached(gTitleVramBlock, 0x500, buffer + 32);
    dma = (volatile s32 *)REG_DMA3;
    while (dma[2] < 0)
        ;
    MemFree(buffer);
}

// ☀️'s Title_RevealSpriteRow: rebuilds the row of 18 sprites under the logo
// and pushes those the reveal has reached, hiding the two newest on odd
// frames, then counts the reveal on.
void Func_02000278(void)
{
    u32 *w;
    struct TitleSprite *p;
    /* FAKEMATCH: the tile is pinned to r5 and the block's offset read into r3: the native keeps the tile in r5, the cursor in r6 and the index in r7, where plainly the cursor takes r5 and the tile r7 (differ 26); pinned alone the tile is loaded straight into r5 (differ 4). */
    register s32 tile asm("r5");
    s32 i;
    s32 n;
    s32 y;
    s32 x;

    p = Data_020006f0;
    w = Data_020006f0[0].attr;
    {
        register u32 offset asm("r3") = ResourceTableEntries[gTitleVramBlock].offset;

        tile = offset >> 5;
    }
    i = 0;
    y = 0x88;
    do {
        /* FAKEMATCH: the row's y goes through n, the native rebuilding 0x88 in n's register each pass; used directly it stays in r8 (differ 20). */
        n = y;
        x = 232 - (18 - i) * 8;
        *w++ = 0;
        *w++ = (x << 16) | n | 0x8400;
        *w++ = 0xf000 | tile;
        n = Data_020006dc / 2 - i;
        if (n < 0)
            n = 0;
        if (n <= 2 && (gFrameCount & 1))
            n = 0;
        if (n != 0)
            Func_020005ac(p++, 255);
        tile += 2;
    } while (++i <= 17);
    Data_020006dc++;
}

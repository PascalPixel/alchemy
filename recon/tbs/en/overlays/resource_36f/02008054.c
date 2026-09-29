/* Draft of Title_LoadBackground, resource_36f at 0x02008454, written for
 * MENU/TITLE beside REVEAL.C and SPRITES.C (TITLE.H). The listing keeps its
 * rows; Title_Run links as MENU/TITLE/RUN.C.
 * Remaining difference: the ROM loads its resource number 0x1a from its
 * literal pool, as ResourceId_GoldenSunLogo does, and it reloads the map
 * cell buffer's address from the pool after the decode call, where GCC keeps
 * it in r5 across the call (0x2e..0x44 and the pool order after), with the
 * label or with the checked entry Ram_MapCellBuffer alike. CLEAR's
 * background loader in overlay 370 has the same difference. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "DMA.H"
#include "TITLE.H"

struct TitleMapWork {
    u8 unknown_00[20];
    u16 mode;
};

struct ScrollPair {
    u16 x;
    u16 y;
};

extern u32 gKeyState;
extern u32 gKeysHeld;
extern u16 gBgScroll[];
extern u8 gMapCellBuffer[];
extern struct TitleMapWork *gMapWork;

void Event_SetPairWork1c0(s32 scene, s32 entrance);
void RuntimeDispatch_NoOpHook(s32 hook);
void Blend_SetDarkenTarget16(s32 target);
void Blend_WaitForTransition(void);
s32 SaveState_ScanRecordFlags(void);
void Party_ApplyStatePreset(void);
void Title_ShowSplashScreen(s32 mode);
void Func_080f0000(s32 mode);
s32 Func_080f2000(s32 mode);
void Func_080f2020(s32 mode);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Sys_Free(void *buffer);
s32 Resource_FindFreeEntry(void);
u8 *Resource_GetTableEntry(s32 resource);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 VramBlock_LoadCached(s32 block, s32 size, const void *data);

#define DMA3 ((volatile u32 *)0x040000d4)

/* Load the title background: palette, tiles and a 30 x 20 map counting up
 * from tile 0x1a0, then clear the scroll registers. */
void Title_LoadBackground(void)
{
    u8 *res;
    s32 id;
    u16 *map;
    u32 x;
    u32 y;
    s32 tile;
    struct ScrollPair *scroll;
    s32 blank;

    id = 0x1a;
    Blend_SetDarkenTarget16(0);
    *(volatile u16 *)0x0400000c = 0x681;
    gBgScroll[5] = 0;
    blank = 0x1ff;
    res = Resource_GetTableEntry(id);
    Dma_Set(res, (void *)0x05000000, 0x84000070, DMA3);
    res += 0x1c0;
    Resource_DecodeType01(res, gMapCellBuffer);
    Dma_Set(gMapCellBuffer, (void *)0x06006800, 0x84002580, DMA3);
    map = (u16 *)0x06003000;
    tile = 0x1a0;
    y = 0;
col:
    {
        x = 0;
    row:
        {
            s32 old = tile;

            /* FAKEMATCH: keep the signed tile wrap in the high half. */
            tile = ((old << 16) + 0x10000) >> 16;
            *map++ = old;
        }
        if (++x <= 29)
            goto row;
        *map++ = blank;
        *map++ = blank;
    }
    if (++y <= 19)
        goto col;
    scroll = (struct ScrollPair *)gBgScroll;
    for (y = 0; y <= 3; y++) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, DMA3);
    gMapWork->mode = 0x1400;
}

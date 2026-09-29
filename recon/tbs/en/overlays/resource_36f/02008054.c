/* Draft of the title overlay's remaining code, resource_36f: Title_Run at
 * 0x02008054, Title_LoadSprites at 0x020081c0 and Title_LoadBackground at
 * 0x02008454, written for MENU/TITLE beside REVEAL.C (TITLE.H). The listing
 * keeps these rows. Compiled with stand-in symbols for the numbers below,
 * every other instruction and literal matches, except where noted.
 * Remaining differences:
 * - Title_Run: the ROM loads the scenes it sends the party to (0 from the
 *   splash, 1 from the intro and the menu, 4 for a new game) and the hook
 *   number 0xb from its literal pool, as link-time scene and hook numbers
 *   would; the plain numbers below compile to immediate moves. Its imports
 *   0x080f0000, 0x080f2000 and 0x080f2020 are the main image's unnamed far
 *   veneers into the scroll and palette modules.
 * - Title_LoadSprites: the ROM loads its resource number 0x1c from its
 *   literal pool, as a link-time resource number would.
 * - Title_LoadBackground: the ROM loads its resource number 0x1a from its
 *   literal pool, as a link-time resource number would; and it reloads
 *   gMapCellBuffer from the pool after the decode call, where this C keeps
 *   the address in r5 across it (0x2e..0x44 and the pool order after). */
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

/* FAKEMATCH: calling through the inline passes each constant straight into
 * its argument register instead of precomputing it. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

s32 Title_Run(void)
{
    s32 wait;

    if (gGameState.entrance == 10) {
        Engine_ActorGet(gGameState.selected_actor)->motion_flags = 0;
        Engine_AudioPlayCue(75);
        Title_RevealScreen(0);
        Engine_TaskWait(120);
        wait = 0;
        if (gKeyState == 0) {
            do {
                Engine_TaskWait(1);
                if (++wait > 3599)
                    break;
            } while (gKeyState == 0);
        }
        Event_SetPairWork1c0(0, 2);
        return 0;
    }
    if (gGameState.entrance == 9) {
        Engine_AudioPlayCue(67);
        Func_080f0000(0);
        Engine_AudioPlayCue(17);
        Blend_SetDarkenTarget16(60);
        Blend_WaitForTransition();
        Engine_EventWait(240);
        Engine_AudioPlayCue(19);
        Event_SetPairWork1c0(1, 2);
        return 0;
    }
    RuntimeDispatch_NoOpHook(0xb);
    if (gGameState.entrance == 2) {
    menu:
        Engine_AudioPlayCue(19);
        Title_ShowSplashScreen(0);
        Func_080f2020(0);
        if (SaveState_ScanRecordFlags() <= 0)
            goto chosen;
        Engine_AudioPlayCue(70);
        if (Func_080f2000(1) != 0)
            goto chosen;
        Engine_AudioPlayCue(17);
        Blend_SetDarkenTarget16(30);
        Blend_WaitForTransition();
        wait = 0;
        if (gKeysHeld == 0) {
            do {
                Engine_TaskWait(1);
                if (++wait > 119)
                    break;
            } while (gKeysHeld == 0);
        }
        goto menu;
    chosen:
        Event_SetPairWork1c0(1, 1);
    } else {
        Engine_AudioPlayCue(64);
        Func_080f2000(0);
        Party_ApplyStatePreset();
        Event_SetPairWork1c0(4, 16);
        Engine_AudioPlayCue(17);
    }
    Engine_AudioPlayCue(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    Engine_EventWait(60);
    Engine_AudioPlayCue(19);
    return 0;
}

/* Decode the title sprites' tiles and palette and load the tiles into the
 * title's VRAM block, finding one the first time. */
void Title_LoadSprites(s32 unused)
{
    u8 *buffer;
    volatile u32 *dma;

    buffer = Runtime_BumpAllocateAlternatePool(0x520);
    if (gTitleVramBlock == -1)
        gTitleVramBlock = Resource_FindFreeEntry();
    Resource_DecodeType01(Resource_GetTableEntry(0x1c), buffer);
    Dma_Set(buffer, (void *)0x050003e0, 0x84000008, DMA3);
    Call3((void (*)())VramBlock_LoadCached, gTitleVramBlock, 0x500, (s32)(buffer + 32));
    dma = DMA3;
    while (dma[2] & 0x80000000)
        ;
    Sys_Free(buffer);
}

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

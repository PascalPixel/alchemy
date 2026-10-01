/* NONMATCHING: 2026-10-01 brief Wave2 one-device macro attempt.
 * Removing QUEUE_WRITE's one-pass boundary at source line 56 changes:
 * Title_RevealScreen: mov r1, r3 => strh r5, [r5] (177/180 assembly lines).
 * Production source retains and tags this measured scheduling boundary.
 */
/* The title overlay: its scene tables. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "RESOURCE_IDS.H"
#include "../../../../../../../games/THE BROKEN SEAL/SRC/MENU/TITLE/TITLE.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "VRAM_BLOCK.H"
#include "RAM_BUFFER.H"
#include "CALL.H"

extern u8 gTitleEntrances[];
extern u8 gTitleExits[];
extern u8 gTitlePlacements[];
extern u8 gTitleEvents[];

extern u32 gKeyState;
extern u32 gKeysHeld;
void Event_SetPairWork1c0(s32 scene, s32 entrance);
void RuntimeDispatch_NoOpHook(s32 resource);
void Blend_SetDarkenTarget16(s32 target);
void Blend_WaitForTransition(void);
s32 SaveState_ScanRecordFlags(void);
void Party_ApplyStatePreset(void);
void Title_ShowSplashScreen(s32 mode);
void ScrollFar_Entry00(s32 mode);
s32 PaletteFar_Entry00(s32 mode);
void PaletteFar_Entry20(s32 mode);

void *Runtime_BumpAllocateAlternatePool(s32 size);
void Sys_Free(void *buffer);
s32 Resource_FindFreeEntry(void);
u8 *Resource_GetTableEntry(s32 resource);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 VramBlock_LoadCached(s32 block, s32 size, const void *data);
#define DMA3 ((volatile u32 *)0x040000d4)

void Runtime_PushSlotEntry(void *entry, s32 slot);

static __inline__ void RestoreInterrupts(u32 saved)
{
    REG_IME = saved;
}

/* FAKEMATCH: the one-pass IME read keeps the saved copy before masking;
 * the count cast preserves the queue's original publication order. */
#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        {                                                                \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        }                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = 0x20000;                                         \
        }                                                                   \
        RestoreInterrupts(saved);                                           \
    } while (0)

/* FAKEMATCH: the one-halfword record keeps the short-range pool zero, as
 * PALETTE_START.C does. */
static __inline__ void ResetCounter(s16 *destination)
{
    struct { u16 value; } zero;

    zero.value = 0;
    *destination = zero.value;
}

extern struct MapRenderWork *gMapWork;
extern u16 gBgScroll[];

/* FAKEMATCH: an inline call wrapper keeps the decode's source in r4 and
   reloads the cell buffer's address after it, as the game does. */
static __inline__ void DecodeBackground(const u8 *res)
{
    Resource_DecodeType01(res, (void *)Ram_MapCellBuffer);
}

struct TitleWork {
    u8 unknown_00[20];
    u16 mode;
};

struct ScrollPair {
    u16 x;
    u16 y;
};

u8 *Title_GetEntrances(void)
{
    return gTitleEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *Title_GetExits(void)
{
    return gTitleExits;
}

u8 *Title_GetPlacements(void)
{
    return gTitlePlacements;
}

u8 *Title_GetEvents(void)
{
    return gTitleEvents;
}

/* The title scene's driver. Entrance 10 reveals the title and waits up to a
 * minute for a key before returning to it; entrance 9 plays the ending's
 * scroll and returns to the clear screen; entrance 2 runs the title menu until
 * a saved game is chosen; any other entrance starts a new game in Lunpa's
 * village, Haidia. */
s32 Title_Run(void)
{
    s32 wait;

    if (gGameState.entrance == 10) {
        Object_GetById(gGameState.selected_actor)->motion_flags = 0;
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
        Event_SetPairWork1c0((s32)&SceneId_Title, 2);
        return 0;
    }
    if (gGameState.entrance == 9) {
        Engine_AudioPlayCue(67);
        ScrollFar_Entry00(0);
        Engine_AudioPlayCue(17);
        Blend_SetDarkenTarget16(60);
        Blend_WaitForTransition();
        Engine_EventWait(240);
        Engine_AudioPlayCue(19);
        Event_SetPairWork1c0((s32)&SceneId_Clear, 2);
        return 0;
    }
    RuntimeDispatch_NoOpHook((s32)&ResourceId_PaletteFarCalls);
    if (gGameState.entrance == 2) {
    menu:
        Engine_AudioPlayCue(19);
        Title_ShowSplashScreen(0);
        PaletteFar_Entry20(0);
        if (SaveState_ScanRecordFlags() <= 0)
            goto chosen;
        Engine_AudioPlayCue(70);
        if (PaletteFar_Entry00(1) != 0)
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
        Event_SetPairWork1c0((s32)&SceneId_Clear, 1);
    } else {
        Engine_AudioPlayCue(64);
        PaletteFar_Entry00(0);
        Party_ApplyStatePreset();
        Event_SetPairWork1c0((s32)&SceneId_HaidiaMura, 16);
        Engine_AudioPlayCue(17);
    }
    Engine_AudioPlayCue(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    Engine_EventWait(60);
    Engine_AudioPlayCue(19);
    return 0;
}

/* The title sprites: decode their tiles and palette, then load the tiles
 * into the title's VRAM block. */

/* Decode the title sprites' tiles and palette and load the tiles into the
 * title's VRAM block, finding one the first time. */
void Title_LoadSprites(s32 unused)
{
    u8 *buffer;
    volatile u32 *dma;

    buffer = Runtime_BumpAllocateAlternatePool(0x520);
    if (gTitleVramBlock == -1)
        gTitleVramBlock = Resource_FindFreeEntry();
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_IntroTilesB), buffer);
    Dma_Set(buffer, (void *)0x050003e0, 0x84000008, DMA3);
    Call3((void (*)())VramBlock_LoadCached, gTitleVramBlock, 0x500, (s32)(buffer + 32));
    dma = DMA3;
    while (dma[2] & 0x80000000)
        ;
    Sys_Free(buffer);
}

/* The title screen's reveal: the background fades in and a row of eighteen
 * sprites appears one by one. */

/* Rebuild the row of eighteen title sprites; one more shows every two
 * frames, and the newest two blink with the frame counter. */
void Title_RevealSpriteRow(void)
{
    u32 *w;
    struct TitleSprite *p;
    s32 tile;
    s32 i;
    s32 n;
    s32 y;
    s32 x;

    p = gTitleSprites;
    w = gTitleSprites[0].attr;
    tile = gVramBlockCache[gTitleVramBlock].offset >> 5;
    i = 0;
    y = 0x88;
loop:
    {
        x = 232 - (18 - i) * 8;
        *w++ = 0;
        *w++ = (x << 16) | y | 0x8400;
        *w++ = 0xf000 | tile;
        n = gTitleRevealFrame / 2 - i;
        if (n < 0)
            n = 0;
        if (n <= 2 && (gFrameCount & 1))
            n = 0;
        if (n != 0)
            Runtime_PushSlotEntry(p++, 255);
        tile += 2;
    }
    if (++i <= 17)
        goto loop;
    gTitleRevealFrame++;
}

/* Fade the title in: the background, then the sprite row under a blend
 * that clears over sixteen steps. */
void Title_RevealScreen(s32 unused)
{
    s32 i;
    struct IoWriteQueue *q;

    Title_LoadBackground();
    Engine_EventWait(30);
    ResetCounter(&gTitleRevealFrame);
    Title_LoadSprites(0);
    Engine_TaskAddCallback(Title_RevealSpriteRow, 0xc80);
    QUEUE_WRITE(0x4000000, 0x1540);
    QUEUE_WRITE(0x4000050, 0x2fce);
    QUEUE_WRITE(0x4000054, 16);
    QUEUE_WRITE(0x4000052, 0x1010);
    Engine_EventWait(120);
    for (i = 0; i <= 16; i++) {
        QUEUE_WRITE(0x4000054, 16 - i);
        Engine_TaskWait(3);
    }
    gEventWork->start_transition = 0;
    gEventWork->transition_frames = 1;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    gEventWork->transition_frames = 60;
}

/* The title screen's background: the Golden Sun logo's palette, tiles and a
 * 30 x 20 map counting up from tile 0x1a0, then cleared scroll registers.
 * The cell buffer is the fixed RAM buffer, reloaded for each use. */

/* Load the title background: palette, tiles and a 30 x 20 map counting up from
 * tile 0x1a0, then clear the scroll registers. */
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

    id = (s32)&ResourceId_GoldenSunLogo;
    Blend_SetDarkenTarget16(0);
    *(volatile u16 *)0x0400000c = 0x681;
    gBgScroll[5] = 0;
    blank = 0x1ff;
    res = Resource_GetTableEntry(id);
    Dma_Set(res, (void *)0x05000000, 0x84000070, DMA3);
    res += 0x1c0;
    DecodeBackground(res);
    Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, DMA3);
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
    (*(struct TitleWork **)&gMapWork)->mode = 0x1400;
}

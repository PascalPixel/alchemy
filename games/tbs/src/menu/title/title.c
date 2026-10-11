#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "global_cells.h"
#include "types.h"
#include "save_state.h"
#include "mem.h"
#include "field_event.h"
#include "field_service.h"
#include "scene_ids.h"
#include "resource_ids.h"
#include "title.h"
#include "dma.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "vram_block.h"
#include "ram_buffer.h"
#include "call.h"

#define RuntimeDispatch_NoOpHook ((void (*)(s32))RuntimeDispatch_NoOpHook)
#define Title_ShowSplashScreen ((void (*)(s32))Title_ShowSplashScreen)

void Event_SetDestination(s32 scene, s32 entrance);

#define DMA3 (REG_DMA3)

extern u16 gBgScroll[];

static __inline__ void DecodeBackground(const u8 *res)
{
    /* FAKEMATCH: this call boundary keeps the source in r4 and reloads the map-cell buffer after the decode; a direct call retains r5 and changes the DMA literal pool. */
    Resource_DecodeType01(res, (void *)Ram_MapCellBuffer);
}

struct TitleWork {
    u8 unknown_00[20];
    u16 mode;
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
        Event_SetDestination((s32)&SceneId_Title, 2);
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
        Event_SetDestination((s32)&SceneId_Clear, 2);
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
        Event_SetDestination((s32)&SceneId_Clear, 1);
    } else {
        Engine_AudioPlayCue(64);
        PaletteFar_Entry00(0);
        Party_ApplyStatePreset();
        Event_SetDestination((s32)&SceneId_HaidiaMura, 16);
        Engine_AudioPlayCue(17);
    }
    Engine_AudioPlayCue(17);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    Engine_EventWait(60);
    Engine_AudioPlayCue(19);
    return 0;
}

void Title_LoadSprites(s32 unused)
{
    u8 *buffer;
    volatile u32 *dma;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    u8 *res;

    buffer = (u8 *)Runtime_BumpAllocateAlternatePool(0x9600);
    res = Resource_GetTableEntry((s32)&ResourceId_IntroTilesB);
    if (gTitleVramBlock == -1)
        gTitleVramBlock = Resource_FindFreeEntry();
    Dma_Set(res, (void *)0x05000200, 0x84000070, DMA3);
    res += 0x1c0;
    Resource_DecodeType01(res, buffer);
    Vram_Load(gTitleVramBlock, 0x1e00, buffer + 0x7800);
#else
    buffer = (u8 *)Runtime_BumpAllocateAlternatePool(0x520);
    if (gTitleVramBlock == -1)
        gTitleVramBlock = Resource_FindFreeEntry();
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_IntroTilesB), buffer);
    Dma_Set(buffer, (void *)0x050003e0, 0x84000008, DMA3);
    Vram_Load(gTitleVramBlock, 0x500, buffer + 32);
#endif
    dma = DMA3;
    while (dma[2] & 0x80000000)
        ;
    MemFree(buffer);
}

void Title_RevealSpriteRow(void)
{
    u32 *w;
    struct TitleSprite *p;
    s32 tile;
    s32 i;
    s32 n;
    s32 y;
    s32 x;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    s32 row;

    p = gTitleSprites;
    w = gTitleSprites[0].attr;
    tile = gVramBlockCache[gTitleVramBlock].offset >> 5;
    row = 0;
    y = 128;
    for (; row <= 3; row++) {
        i = 0;
localized_sprite:
        {
            /* FAKEMATCH: the explicit inner boundary keeps the record writer independent of the sprite pointer; a nested for loop adds a saved register and twelve bytes. */
            *w++ = 0;
            *w++ = (i * 8 << 16) | y | 0x2400;
            *w++ = tile;
            n = gTitleRevealFrame / 2 - i;
            if (n < 0)
                n = 0;
            if (n <= 2 && (gFrameCount & 1))
                n = 0;
            if (n != 0)
                Runtime_PushSlotEntry(p, 255);
            p++;
            tile += 2;
        }
        if (++i <= 29)
            goto localized_sprite;
        y += 8;
    }
#else

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
#endif
    gTitleRevealFrame++;
}

void Title_RevealScreen(s32 unused)
{
    s32 i;

    Title_LoadBackground();
    Engine_EventWait(30);
    gTitleRevealFrame = 0;
    Title_LoadSprites(0);
    Engine_TaskAddCallback(Title_RevealSpriteRow, 0xc80);
    IO_WRITE_QUEUE_PUSH(0x4000000, 0x1540, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x2fce, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000054, 16, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000052, 0x1010, 0x20000);
    Engine_EventWait(120);
    for (i = 0; i <= 16; i++) {
        IO_WRITE_QUEUE_PUSH(0x4000054, 16 - i, 0x20000);
        Engine_TaskWait(3);
    }
    gEventWork->start_transition = 0;
    gEventWork->transition_frames = 1;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    gEventWork->transition_frames = 60;
}

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
    REG_BG2CNT = 0x681;
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
    ((struct TitleWork *)gMapWork[0])->mode = 0x1400;
}

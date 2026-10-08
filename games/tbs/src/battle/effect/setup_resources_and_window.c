#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "game_state.h"
#include "resource.h"
#include "types.h"
#include "mem.h"
#include "dma.h"
#include "battle_effect_runtime.h"
#include "vram_block.h"
#include "io_reg.h"
#include "field_event.h"
#include "btlsys.h"
#include "ui.h"

struct FxWindowEntry {
    u32 a;
    u32 b;
    u32 attr;
};

struct FxWindowWork {
    u16 tiles;
    u16 window_slot;
    s32 x;
    s32 y;
    u8 padding0c[6];
    u16 unknown_12;
    u8 padding14[4];
    s32 unknown_18;
    s32 window;
    struct FxWindowEntry entries[67];
};

struct FxObject {
    u8 padding00[8];
    s32 x;
    u8 padding0c[6];
    s16 y;
};

extern struct FxWindowWork gMapCellBuffer;


void BattleFx_SetupResourcesAndWindow(void)
{
    u32 *oam;
    volatile u32 fill;
    u32 *pal;
    struct FxWindowWork *work;
    struct FxObject *object;
    s32 attr;
    u32 i;
    u32 tile;
    u32 attr2;

    pal = (u32 *)Runtime_BumpAllocateAlternatePool(32);
    work = &gMapCellBuffer;
    oam = (u32 *)work->entries;
    object = Object_GetById(gGameState.selected_actor);
    work->tiles = Resource_FindFreeEntry();
    fill = 0;
    Dma_Set((const void *)&fill, pal, 0x85000020, REG_DMA3);
    pal[0] = 255;
    pal[1] = 0x1ff;
    pal[2] = 272;
    pal[8] = 68;
    pal[9] = 324;
    pal[10] = 272;
    pal[16] = 119;
    pal[17] = 0x177;
    pal[18] = 272;
    pal[24] = 0xff0;
    pal[25] = 0xffff;
    pal[26] = 0x1ffff;
    pal[27] = 0x11ff0;
    pal[28] = 0x1100;
    attr = VramBlock_LoadCached(work->tiles, 128, pal) | 0x400;
    for (i = 0; i < 66; i++) {
        *oam++ = 0;
        *oam++ = 0;
        *oam++ = attr;
    }
    MemFree(pal);
    if (GameFlag_TestFar(284)) {
        work->x = 240 << 15;
        work->y = 160 << 15;
    } else {
        work->x = (((object->x - 0x10000000) >> 16) * 240 / 4096) << 16;
        work->y = ((object->y * 160) / 4096) << 16;
    }
    work->window_slot = Resource_LoadFixedBlockBIntoFreeSlotFar();
    tile = gVramBlockCache[work->window_slot].offset;
    work->window = UiWindow_CreateFar(0, 0, 0, 0, 2);
    work->unknown_12 = 0xffff;
    work->unknown_18 = 0x10000;
    *oam++ = 0;
    *oam++ = 0x40000000;
    attr2 = 0x400;
    attr2 |= tile >> 5;
    *oam = attr2;
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "system.h"
#include "types.h"
#include "scene.h"
#include "resource.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "btlwork.h"
#include "btldisp.h"
#include "heap_state.h"
#include "io_reg.h"

#define Graphics_BuildSequentialTileTable ((void (*)(void*))Graphics_BuildSequentialTileTable)
#define BattlePresentation_BuildTilemap ((void (*)(void*))BattlePresentation_BuildTilemap)

#define gTransitionWork (*(struct BattleBackgroundView * *)&gTransitionWork)


void BattlePresentation_InitializeWorkAndResetState(void)
{
    struct BattleAffineHdma *work;
    struct BattleBackgroundView *state;
    volatile u32 zero;
    work = Runtime_AllocateBlock(10, sizeof *work);
    state = gTransitionWork;
    zero = 0;
    Dma_Set(&zero, work, 0x850000a8, REG_DMA3);
    state->mode = 0;
}

void Runtime_ReleaseHeapBlock10(void)
{
    Runtime_ReleaseHeapBlock(10);
}

typedef void (*BitDecoder)(const u8 *source, void *destination);



void BattleBackground_Load(s32 mode, s32 resource, s32 level)
{
    struct BattleBackgroundView *view = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
    u8 *data = Resource_GetTableEntry(resource);
    struct BtlWork *session = *(gWorkSlot + HEAP_SLOT_BATTLE);
    u16 *palette;

    /* FAKEMATCH: needed to match: the one-pass block keeps the size load after the session load and the decoder call after the copy */
    do {
        u32 size = (u32)BitDecoder_Size;
        void *decoder = Runtime_AllocateHeapBlock(49, size);

        Dma_Set((void *)BitDecoder_DecodeImage, decoder, 0x84000000 | (size >> 2), REG_DMA3);
    } while (0);
    {
        u32 offset = 0x100;
        void *vram;

        /* FAKEMATCH: the empty statement keeps the tile offset a load of its own, ahead of the VRAM address. */
        __asm__("" : "+r"(offset));
        vram = (void *)0x06008000;
        ((BitDecoder)*(gWorkSlot + 49))(data + offset, vram);
    }
    Runtime_ReleaseHeapBlock(49);
    palette = session->palette;
    Dma_Set(data, palette, 0x84000040, REG_DMA3);
    if (level >= 0)
        Graphics_ScaleRgb555Clamped(palette, (u16 *)0x050000c0, session->brightness = 0x10000 - level * 1092, 128);
    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;
    Graphics_BuildSequentialTileTable((void *)0x06003800);
    BattlePresentation_BuildTilemap((void *)0x0600f800);
    Iwram_ClearWords((void *)0x0600ffc0, 64);
    if (view->mode == 0)
        Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
    view->mode = mode;
    if (mode == 1)
        REG_BG1CNT = 0x1f83;
}

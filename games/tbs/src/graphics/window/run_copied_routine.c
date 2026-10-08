#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "dma.h"
#include "system.h"
#include "mem.h"
#include "glyph.h"
#include "heap_state.h"
#include "io_reg.h"



void Ui_RunCopiedRoutine(GlyphTransfer *work)
{
    u32 size;
    void *code;

    /* FAKEMATCH: needed to match: the do-while keeps the size load after the arguments. */
    do { size = (u32)Tile_Decompress4bppCodeSize; } while (0);
    code = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((const void *)Tile_Decompress4bpp, code, 0x84000000 | (size >> 2), REG_DMA3);
    ((void (*)(s32, u8 *))((union HeapState *)gWorkSlot)->slots[49])(
        (s32)work->encoded, (u8 *)work);
    Runtime_ReleaseHeapBlock(49);
}

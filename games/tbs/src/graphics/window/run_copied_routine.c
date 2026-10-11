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
#include "ramcode.h"
#include "system.h"
#include "mem.h"
#include "glyph.h"
#include "heap_state.h"
#include "io_reg.h"



void Ui_RunCopiedRoutine(GlyphTransfer *work)
{

    RamCode_LoadSlot(49, Tile_Decompress4bpp, Tile_Decompress4bppCodeSize);
    ((void (*)(s32, u8 *))((union HeapState *)gWorkSlot)->slots[49])(
        (s32)work->encoded, (u8 *)work);
    Runtime_ReleaseHeapBlock(49);
}

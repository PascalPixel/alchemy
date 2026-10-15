#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "text_reader.h"
#include "types.h"
#include "dma.h"
#include "mem.h"
#include "io_reg.h"

void UiText_LookupMessage(struct TextReader *reader, s32 message)
{
    void (*routine)(struct TextReader *, s32);
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do
    {
        size = (u32)Text_LookupSymbolCodeSize;
    } while (0);
    routine = (void (*)(struct TextReader *, s32))MemAlloc(size);
    Dma_Set((const void *)Text_LookupSymbolCode, routine, 0x84000000 | (size >> 2), REG_DMA3);
    routine(reader, message);
    MemFree(routine);
}

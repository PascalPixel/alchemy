#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TEXT_READER.H"
#include "TYPES.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"
#include "IO_REG.H"


void UiText_LookupMessage(struct TextReader *reader, s32 message)
{
    void (*routine)(struct TextReader *, s32);
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Text_LookupSymbolCodeSize;
    } while (0);
    routine = (void (*)(struct TextReader *, s32))MemAlloc(size);
    Dma_Set((const void *)Text_LookupSymbolCode, routine, 0x84000000 | (size >> 2), REG_DMA3);
    routine(reader, message);
    MemFree(routine);
}

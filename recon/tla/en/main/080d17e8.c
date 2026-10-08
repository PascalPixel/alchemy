#include "types.h"
#include "palbuf.h"
#include "ram_buffer.h"
#include "iwram_call.h"

void BattleFx_BuildBuffer(s32 mode, struct BattleEffectBuffers *source, u16 *destination, s32 half);

void BattleFx_LoadBufferPalette(const u16 *colours)
{
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block_32;

    if (buffers != NULL) {
        Iwram_CopyWords(buffers->palette, colours, 0x1c0);
        BattleFx_BuildBuffer(0x10000, buffers, buffers->target, 1);
    }
}

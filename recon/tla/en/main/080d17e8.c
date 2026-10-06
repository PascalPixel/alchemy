#include "TYPES.H"
#include "PALBUF.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"

void BattleFx_BuildBuffer(s32 mode, struct BattleEffectBuffers *source, u16 *destination, s32 half);

void BattleFx_LoadBufferPalette(const u16 *colours)
{
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block_32;

    if (buffers != NULL) {
        Iwram_CopyWords(buffers->palette, colours, 0x1c0);
        BattleFx_BuildBuffer(0x10000, buffers, buffers->target, 1);
    }
}

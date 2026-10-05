/* Near miss: differ 2, one swapped load, new in ⚓️ beside ☀️'s battle palette
   blend; it goes after BattleFx_StartBufferInterpolation in
   BATTLE/EFFECT/EFFECT12.C, before COMMON's PALETTE.C. The listing loads
   the IwramCopyWords entry between building 0x1c0 (movs, lsls); this build
   loads it after. */
#include "TYPES.H"
#include "PALBUF.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"

void BattleFx_BuildBuffer(s32 mode, struct BattleEffectBuffers *source, u16 *destination, s32 half);

/* Takes the background half of the palette from colours and rebuilds the
   target from it. */
void BattleFx_LoadBufferPalette(const u16 *colours)
{
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block_32;

    if (buffers != NULL) {
        Iwram_CopyWords(buffers->palette, colours, 0x1c0);
        BattleFx_BuildBuffer(0x10000, buffers, buffers->target, 1);
    }
}

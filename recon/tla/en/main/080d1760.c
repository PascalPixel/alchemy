/* Near miss: differ 12, new in ⚓️ beside ☀️'s battle palette blend; it goes
   after BattleFx_SetPrimaryBufferValue in BATTLE/EFFECT/EFFECT12.C. The
   listing loads the buffer into r3, then the 0x3e0 mask, copies the buffer
   to r0 for the colour pointer, then the 0x1f mask, and steps the channel
   pointer from r0; this build keeps the buffer in r3 and steps the channel
   from it. Pointer walks, a do-while counting down (16-17), a struct of
   three channels, index and u16-pointer forms (12-13) and the
   scheduling-barrier search leave 12 or more. */
#include "TYPES.H"
#include "PALBUF.H"
#include "RAM_BUFFER.H"

/* Packs the target's channels, each a 5-bit value in the top of a
   halfword, back into the palette's BGR555 colours. */
void BattleFx_PackTargetBuffer(void)
{
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block_32;
    u16 *channel = buffers->target;
    s32 i;

    for (i = 0; i < 0x1c0; i++) {
        buffers->palette[i] = (channel[0] & 0x7c00)
            | ((channel[1] >> 5) & 0x3e0)
            | ((channel[2] >> 10) & 0x1f);
        channel += 3;
    }
}

/* Near miss: differ 9, new in ⚓️ beside ☀️'s battle palette blend; it goes
   after BattleFx_SetPrimaryBufferValue in BATTLE/EFFECT/EFFECT12.C. Its loop
   is the packing loop of ☀️'s BattlePalette_UpdateBlend, over the target
   colours into the palette, without the (s16) casts (ldrh, lsrs). The
   listing loads the buffer into r3 and the two pooled masks right after it,
   copying the buffer to r0 between them; this build loads the buffer
   straight into r0 and the masks after the shifts, the load-first
   scheduling the compiler gap leaves. Pointer walks and do-while forms (12
   to 17) and the scheduling-barrier search do no better. */
#include "TYPES.H"
#include "PALBUF.H"
#include "RAM_BUFFER.H"

/* Packs the target's channels, each a 5-bit value in the top of a
   halfword, into the palette's BGR555 colours. */
void BattleFx_PackTargetBuffer(void)
{
    struct BattleEffectBuffers *work = Ram_HeapSlots->block_32;
    u16 *packed = work->palette;
    s32 blue = 0x7c00;
    u16 *current;
    s32 i;

    i = 0x1c0;
    current = work->target;
    for (; i != 0; i--) {
        *packed++ = (current[0] & blue) | ((current[1] >> 5) & 0x3e0) | ((current[2] >> 10) & 0x1f);
        current += 3;
    }
}

/* 2026-10-03 effect-slot suffix probe.
 * Reused the current TBS EFFECT43 body with the canonical resource owner.
 * All six TLA editions have the same 44-byte body and no literal pool;
 * their only difference is the relocation to ResourceObject_ReleaseFar.
 * T0: score 0, complete 44-byte extent and native call target verified.
 * The natural group matches this extent in all six TLA reference ROMs
 * after ordinary BL relocation; the unchanged TBS suffix matches too.
 * No production adoption or all-edition linking credit is claimed.
 */
#include "TYPES.H"
#include "EFFECT_SLOT.H"
#include "RESOURCE.H"
#include "DMA.H"

void BattleFx_ClearOwnedSlot(struct EffectSlot *slot)
{
    volatile u32 zero;

    if (slot->object)
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)slot->object);
    zero = 0;
    Dma_Set((const void *)&zero, slot, 0x85000000 | (sizeof *slot / 4),
        (volatile u32 *)0x040000d4);
}

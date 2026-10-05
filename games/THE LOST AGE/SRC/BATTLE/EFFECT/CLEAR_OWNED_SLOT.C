#include "TYPES.H"
#include "EFFECT_SLOT.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "IO_REG.H"

/* Release the slot's object, if it has one, and clear the whole slot. */
void BattleFx_ClearOwnedSlot(struct EffectSlot *slot)
{
    volatile u32 zero;

    if (slot->object)
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)slot->object);
    zero = 0;
    Dma_Set((const void *)&zero, slot, 0x85000000 | (sizeof *slot / 4),
        REG_DMA3);
}

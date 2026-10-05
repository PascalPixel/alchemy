#include "TYPES.H"
#include "EFFECT_SLOT.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "IO_REG.H"

void BattleFx_ClearOwnedSlot(struct EffectSlot *slot)
{
    if (slot->object)
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)slot->object);
    Dma_Fill32(0, slot, sizeof *slot);
}

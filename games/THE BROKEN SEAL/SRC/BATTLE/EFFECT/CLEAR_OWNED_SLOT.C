#include "DMA.H"
void ResourceObject_ReleaseFar(void *);
struct EffectSlot { void *object; u8 work[68]; };
void BattleFx_ClearOwnedSlot(struct EffectSlot *slot)
{
    volatile u32 zero;
    if (slot->object)
        ResourceObject_ReleaseFar(slot->object);
    zero = 0;
    Dma_Set(&zero, slot, 0x85000012, (volatile u32 *)0x040000d4);
}

#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_SLOT.H"
#include "RAM_BUFFER.H"

/* Heap slot 56: the battle effects' work, 24 screen-space slots after the
   effect's own state, in a 0x7f4-byte block. */
struct BattleFxScene {
    u8 unknown_00[0x50];
    struct EffectSlot slots[24];
    u8 unknown_710[0xe4];
};

/* The heap slot's byte offset in the slot bank. */
#define BATTLE_FX_SLOT 224

void BattleFx_UpdateAllSlots(void)
{
    struct BattleFxScene *scene;
    struct EffectSlot *slot;
    s32 remaining_slots;

    scene = Ram_HeapSlots->effect_work;
    remaining_slots = 0x17;
    /* FAKEMATCH: keeps the count between the work load and its first slot. */
    __asm__("");
    slot = scene->slots;
    do {
        remaining_slots -= 1;
        EffectSlot_Update(slot);
        slot++;
    } while (remaining_slots >= 0);
}

void BattleFx_InitializeSlots(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateHeapBlock(BATTLE_FX_SLOT, sizeof(struct BattleFxScene));
    zero = 0;
    Dma_Set(&zero, work, 0x85000000 | (sizeof(struct BattleFxScene) / 4), (volatile u32 *)0x040000d4);
    Scheduler_AddOrUpdateCallback((s32)BattleFx_UpdateAllSlots, 1152);
}

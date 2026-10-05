#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_SLOT.H"
#include "RAM_BUFFER.H"
#include "MOTION_OBJECT.H"
#include "SYSTEM.H"

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

void BattleFx_ClearOwnedSlot(struct EffectSlot *);

/* Unschedules the slot updates, clears every active slot and frees the
   effects' work. */
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void)
{
    struct BattleFxScene *scene = Ram_HeapSlots->effect_work;
    struct EffectSlot *slot;
    s32 i;

    Scheduler_RemoveCallback((u32)BattleFx_UpdateAllSlots);
    for (i = 0; i < 24; i++) {
        slot = &scene->slots[i];
        if (slot->active != 0)
            BattleFx_ClearOwnedSlot(slot);
    }
    Runtime_ReleaseHeapBlock(BATTLE_FX_SLOT);
    WaitFrames(1);
}

void BattleFx_AdvanceSpinAngle(struct MotionObject *object)
{
    object->angle += 0x2000;
}

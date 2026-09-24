#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "GLOBAL_CELLS.H"
#include "EFFECT_0809B11C.H"
#include "SYSTEM.H"

void *Runtime_AllocateHeapBlock(s32, s32);
void BattleFx_ClearOwnedSlot(struct EffectSlot *);

struct EffectScene {
    u8 unknown_00[0x58];
    struct EffectSlot slots[24];
};

void BattleFx_UpdateAllSlots(void)
{
    s32 slot;
    s32 remaining_slots;

    slot = *(s32 *)ADDR_03001F30 + 0x58;
    remaining_slots = 0x17;
    do {
        remaining_slots -= 1;
        EffectSlot_Update((struct EffectSlot *)slot);
        slot += 0x48;
    } while (remaining_slots >= 0);
}

void BattleFx_InitializeSlots(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateHeapBlock(56, 0x720);
    zero = 0;
    Dma_Set(&zero, work, 0x850001c8, (volatile u32 *)0x040000d4);
    Scheduler_AddOrUpdateCallback((s32)BattleFx_UpdateAllSlots, 3200);
}

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void)
{
    struct EffectScene *scene = *(struct EffectScene **)ADDR_03001F30;
    struct EffectSlot *slot;
    s32 i;

    Scheduler_RemoveCallback((u32)BattleFx_UpdateAllSlots);
    for (i = 0; i < 24; i++) {
        slot = &scene->slots[i];
        if (slot->active != 0)
            BattleFx_ClearOwnedSlot(slot);
    }
    Runtime_ReleaseHeapBlock(56);
    WaitFrames(1);
}

void BattleFx_AdvanceSpinAngle(void *object)
{
    *(u16 *)((u8 *)object + 6) += 0x2000;
}

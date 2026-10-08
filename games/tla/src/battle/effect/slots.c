#include "types.h"
#include "mem.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "effect_slot.h"
#include "ram_buffer.h"
#include "motobj.h"
#include "system.h"
#include "io_reg.h"

struct BattleFxScene {
    u8 unknown_00[0x50];
    struct EffectSlot slots[24];
    u8 unknown_710[0xe4];
};

#define BATTLE_FX_SLOT 224

void BattleFx_UpdateAllSlots(void)
{
    struct BattleFxScene *scene;
    struct EffectSlot *slot;
    s32 remaining_slots;

    scene = Ram_HeapSlots->effect_work;
    remaining_slots = 0x17;
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

    work = Runtime_AllocateHeapBlock(BATTLE_FX_SLOT, sizeof(struct BattleFxScene));
    Dma_Fill32(0, work, sizeof(struct BattleFxScene));
    Scheduler_AddOrUpdateCallback((s32)BattleFx_UpdateAllSlots, 1152);
}

void BattleFx_ClearOwnedSlot(struct EffectSlot *);

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

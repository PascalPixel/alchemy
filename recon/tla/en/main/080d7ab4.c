/*
 * Draft: BattleFx_ClearActiveSlotsAndScheduleUpdates, ☀️'s EFFECT32.C body
 * over ⚓️'s effect work; goes after BattleFx_InitializeSlots in ⚓️'s
 * BATTLE/EFFECT/EFFECT32.C, with BattleFx_AdvanceSpinAngle (080d7af8) after
 * it. Score 2: adds r6, #72 fills the active byte load's delay in the
 * listing. The rest is load scheduling: the native issues loads (and fills
 * their delay) earlier than agscc at -mcpu=arm7tdmi; with -mtune=arm9tdmi
 * added it scores 0. devsearch found nothing; a pointer-walk loop swaps r5
 * and r6.
 */
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

void BattleFx_UpdateAllSlots(void);
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

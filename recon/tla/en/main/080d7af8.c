/*
 * Draft: BattleFx_AdvanceSpinAngle, ☀️'s EFFECT32.C body: matches as it
 * stands; waits for BattleFx_ClearActiveSlotsAndScheduleUpdates (080d7ab4) to
 * join EFFECT32.C.
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

void BattleFx_AdvanceSpinAngle(struct MotionObject *object)
{
    object->angle += 0x2000;
}

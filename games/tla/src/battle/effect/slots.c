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
  u8 unk00[0x50];
  struct EffectSlot slots[24];
  u8 unk710[0xe4];
};

#define BATTLE_EFFECT_SLOT 224

void BattleEffect_UpdateAllSlots(void) {
  struct BattleFxScene *scene;
  struct EffectSlot *slot;
  s32 remainingSlots;

  scene = Ram_HeapSlots->effectWork;
  remainingSlots = 0x17;
  slot = scene->slots;
  do {
    remainingSlots -= 1;
    EffectSlot_Update(slot);
    slot++;
  } while (remainingSlots >= 0);
}

void BattleEffect_InitializeSlots(void) {
  void *work;

  work =
      Runtime_AllocateHeapBlock(BATTLE_EFFECT_SLOT, sizeof(struct BattleFxScene));
  Dma_Fill32(0, work, sizeof(struct BattleFxScene));
  Scheduler_AddOrUpdateCallback((s32)BattleEffect_UpdateAllSlots, 1152);
}

void BattleEffect_ClearOwnedSlot(struct EffectSlot *);

void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct EffectSlot *slot;
  s32 i;

  Scheduler_RemoveCallback((u32)BattleEffect_UpdateAllSlots);
  for (i = 0; i < 24; i++) {
    slot = &scene->slots[i];
    if (slot->active != 0)
      BattleEffect_ClearOwnedSlot(slot);
  }
  Runtime_ReleaseHeapBlock(BATTLE_EFFECT_SLOT);
  WaitFrames(1);
}

void BattleEffect_AdvanceSpinAngle(struct MotionObject *object) {
  object->angle += 0x2000;
}

// Draft, differ 59 of 171: mostly order and register choice. Left: the
// prologue loads (the owner spilled after the 0x80 slot here), the slot
// loop's two pointers swapped (r5/r6), kind >= 17 compiled as cmp #16; ble
// where the native has cmp #17; blt, the wait loop's counter zeroed after
// the test, and the kind's halfword for the != 28 test read again after
// the 0xca5 store where the native reads it with the != 8 test. The bytes
// read as s8 (event, block 32) keep the native's sign extensions; as u8
// with casts, differ 76. (u16)kind != 28 measured 53.
// BattleEffect_CleanupSceneObjects, the field event runner's cleanup
// of the heap slot 0xe0 effect work. Clears every owned effect slot, ends the
// arc effects unless the event work's byte 0xcb6 holds them, waits up to 90
// frames for block 32's busy byte in event mode 3, stops the effect slots'
// callback, hands the effect's position back to the map work and its layer
// back to the owner object, marks the event work's flags 0xca5 and 0xcad
// unless the effect kind was 8 or 28, and releases the work.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

struct CleanupSlot {
  u8 unk00[69];
  s8 owned;
  u8 unk46[2];
};

struct CleanupWork {
  u8 unk000[16];
  struct ObjectRuntime *owner;
  u8 unk014[10];
  s16 kind;
  u8 unk020[36];
  s32 x;
  s32 y;
  s32 z;
  struct CleanupSlot slots[24];
  u8 unk710[8];
  u8 ownerByte;
  u8 layer;
};

struct CleanupAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

struct CleanupMap {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
};

void BattleFx_ClearOwnedSlot(void *slot);
void Func_080dc954(void);
void BattleFx_WaitForArcGroups(void);
void BattleFx_EndArcEffect(void);
void WaitFrames(s32 frames);
void Func_080dbb40(void);
void Scheduler_RemoveCallback(void (*callback)(void));
void BattleFx_UpdateAllEffectSlots(void);
void Event_NoOp(void);
void Runtime_ReleaseHeapBlock(s32 slot);

void BattleEffect_CleanupSceneObjects(void) {
  struct CleanupWork *work = Ram_HeapSlots->effectWork;
  s8 *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *owner = work->owner;
  s8 *block = Ram_HeapSlots->block32;
  struct CleanupMap *map = (struct CleanupMap *)Ram_HeapSlots->mapWork;
  struct CleanupSlot *slot = work->slots;
  s32 i;

  for (i = 23; i >= 0; i--, slot++) {
    if (slot->owned)
      BattleFx_ClearOwnedSlot(slot);
  }
  if (work->kind >= 17 && (work->kind <= 27 || work->kind == 30))
    Func_080dc954();
  if (event[0xcb6] != 0)
    return;
  BattleFx_WaitForArcGroups();
  BattleFx_EndArcEffect();
  if (event[394] == 3 && block != NULL && block[0x2a01] != 0) {
    i = 0;
    do {
      WaitFrames(1);
      if (++i > 89)
        break;
    } while (block[0x2a01] != 0);
  }
  event[0xcb7] = 0;
  Func_080dbb40();
  Scheduler_RemoveCallback(BattleFx_UpdateAllEffectSlots);
  map->x = work->x;
  map->y = work->y;
  map->z = work->z;
  if (owner != NULL) {
    ((u8 *)owner)[35] = work->ownerByte;
    ((struct CleanupAnimation *)owner->animation)->layer = work->layer;
  }
  if (work->kind != 8)
    event[0xca5] = 1;
  if (work->kind != 28)
    event[0xcad] = 1;
  Event_NoOp();
  Runtime_ReleaseHeapBlock(224);
}

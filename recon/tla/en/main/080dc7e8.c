// Draft, differ 30 of 171 (2026-10-26, statements reordered) (2026-10-18; 59 before): order and register choice.
// The held unsigned kind (as in BattleEffect_InitializeSharedScene) and a
// halfword 17 (found by permuter) are tagged devices. Left: the prologue
// loads (the owner spilled after the 0x80 slot here), the slot loop's two
// pointers swapped (r5/r6; counting i up instead measured 49), the wait
// loop's counter zeroed after the test, and register choice round the
// owner's layer. The bytes read as s8 (event, block 32) keep the native's
// sign extensions; as u8 with casts, differ 76.
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

void BattleEffect_ClearOwnedSlot(void *slot);
void Func_080dc954(void);
void BattleEffect_WaitForArcGroups(void);
void BattleEffect_EndArcEffect(void);
void WaitFrames(s32 frames);
void Func_080dbb40(void);
void Scheduler_RemoveCallback(void (*callback)(void));
void BattleEffect_UpdateAllEffectSlots(void);
void Event_NoOp(void);
void Runtime_ReleaseHeapBlock(s32 slot);

void BattleEffect_CleanupSceneObjects(void) {
  struct CleanupWork *work = Ram_HeapSlots->effectWork;
  s8 *event = Ram_HeapSlots->eventWork;
  s8 *block = Ram_HeapSlots->block32;
  struct CleanupMap *map = (struct CleanupMap *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *owner = work->owner;
  struct CleanupSlot *slot = work->slots;
  s32 i;
  s32 raw;
  s16 first;

  for (i = 23; i >= 0; i--, slot++) {
    if (slot->owned)
      BattleEffect_ClearOwnedSlot(slot);
  }
  /* FAKEMATCH: a halfword 17 makes the first test cmp #17; blt as the native
   * (cmp #16; ble with the constant). */
  first = 17;
  if (work->kind >= first && (work->kind <= 27 || work->kind == 30))
    Func_080dc954();
  if (event[0xcb6] != 0)
    return;
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
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
  Scheduler_RemoveCallback(BattleEffect_UpdateAllEffectSlots);
  map->x = work->x;
  map->y = work->y;
  map->z = work->z;
  if (owner != NULL) {
    ((u8 *)owner)[35] = work->ownerByte;
    ((struct CleanupAnimation *)owner->animation)->layer = work->layer;
  }
  /* FAKEMATCH: the kind read unsigned and held, so the != 28 test shifts
   * that copy without reading it again after the 0xca5 store (differ 59
   * without). */
  raw = *(u16 *)&work->kind;
  asm("" : "+r"(raw));
  if (work->kind != 8)
    event[0xca5] = 1;
  if ((s16)raw != 28)
    event[0xcad] = 1;
  Event_NoOp();
  Runtime_ReleaseHeapBlock(224);
}

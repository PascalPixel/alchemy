// Differ 12 of 171 (tla-en, 2026-11-08, -mtune=gba). Left: only the slot loop's
// two pointers, r5 and r6 swapped.
// 2026-11-08 (third pass): the slots are now struct FieldParticle (72 bytes, the
// owned byte is its active flag), as BattleEffect_ClearOwnedSlot takes them; still 12.
// Tried: a while, do-while or continue loop, the step reordered or its own statement,
// the slot block-scoped or set in the for, a separate wait counter (28), slot
// declared before event (26), Task_Remove returning s32: all 12 or worse; a
// 1-minute permute found nothing under 12. Re-measured: the halfword 17 local is
// worth 4 (16 without it); the layer temporary 8 (20 without it).
// 2026-11-08 (second pass): the slot loop as an explicit owned-byte pointer,
// an index loop, a do-while or reordered steps, all 12; an 8-minute permute
// found nothing under 12. The two loop pointers are loop.c's biv and giv,
// and the giv wins r5 on priority. The layer local is now tagged.
// 2026-11-08: the wait loop as nested ifs (mode and block, then i = 0, then the busy
// byte) sets i before the busy test as the native does (16 to 12); i = 0 before the
// whole if, 18. Left: the first loop's slot pointer and owned-byte pointer swap r5
// and r6 (counting up, slot++ in the body, slot set at the loop or declared first,
// the callee typed: all 12; a 6-minute permute found nothing under 12).
// 2026-11-06: the owner's layer goes through an s16
// local declared at the top (layer = work->layer, 24 to 16); declared in the if, 50;
// as s32 22, u8 24. Expect it to be a tagged temporary on adoption.
// Draft, differ 30 of 171 (2026-10-26): the event work is now read through a
// struct, which makes the held unsigned kind device unneeded (the byte
// stores no longer make the kind reload); still 30. Order and register
// choice. A halfword 17 (found by permuter) is a tagged device. Left: the prologue
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
#include "actor.h"
#include "ram_buffer.h"
#include "field_particle.h"

struct CleanupWork {
  u8 unk000[16];
  struct Actor *owner;
  u8 unk014[10];
  s16 kind;
  u8 unk020[36];
  s32 x;
  s32 y;
  s32 z;
  struct FieldParticle slots[24];
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

struct CleanupEventWork {
  u8 unk000[394];
  s8 mode;
  u8 unk18b[0xca5 - 395];
  s8 unkca5;
  u8 unkca6[7];
  s8 unkcad;
  u8 unkcae[8];
  s8 unkcb6;
  s8 unkcb7;
};

void BattleEffect_ClearOwnedSlot(struct FieldParticle *slot);
void Func_080dc954(void);
void BattleEffect_WaitForArcGroups(void);
void BattleEffect_EndArcEffect(void);
void wait(s32 frames);
void Func_080dbb40(void);
void Task_Remove(void (*func)(void));
void BattleEffect_UpdateAllEffectSlots(void);
void Event_NoOp(void);
void HeapSlot_Release(s32 slot);

void BattleEffect_CleanupSceneObjects(void) {
  struct CleanupWork *work = Ram_HeapSlots->effectWork;
  struct CleanupEventWork *event = Ram_HeapSlots->eventWork;
  s8 *block = Ram_HeapSlots->block32;
  struct CleanupMap *map = (struct CleanupMap *)Ram_HeapSlots->mapWork;
  struct Actor *owner = work->owner;
  struct FieldParticle *slot = work->slots;
  s32 i;
  s16 first;
  s16 layer;

  for (i = 23; i >= 0; i--, slot++) {
    if (slot->active)
      BattleEffect_ClearOwnedSlot(slot);
  }
  // A local that holds one constant: as a halfword it makes the first test
  // cmp #17; blt as the native (with the literal, 16 of 171 differ in tla-en).
  first = 17;
  if (work->kind >= first && (work->kind <= 27 || work->kind == 30))
    Func_080dc954();
  if (event->unkcb6 != 0)
    return;
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  if (event->mode == 3 && block != NULL) {
    i = 0;
    if (block[0x2a01] != 0) {
      do {
        wait(1);
        if (++i > 89)
          break;
      } while (block[0x2a01] != 0);
    }
  }
  event->unkcb7 = 0;
  Func_080dbb40();
  Task_Remove(BattleEffect_UpdateAllEffectSlots);
  map->x = work->x;
  map->y = work->y;
  map->z = work->z;
  if (owner != NULL) {
    ((u8 *)owner)[35] = work->ownerByte;
    /* FAKEMATCH(temporary): the layer passes through the s16 local layer; as
     * a plain store, 20 of 171 instructions differ in tla-en. */
    ((struct CleanupAnimation *)owner->animation)->layer = layer = work->layer;
  }
  if (work->kind != 8)
    event->unkca5 = 1;
  if (work->kind != 28)
    event->unkcad = 1;
  Event_NoOp();
  HeapSlot_Release(224);
}

// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja (080dc7a4), tla-de (080e97e8), tla-es (080e57dc),
// tla-fr (080e17dc) and tla-it (080dc7dc), whole-function diff at each
// edition's own address, 171 instructions each. No tagged device remains.
// How it matched (2026-11-08, from 12): the slot loop counts an index up with
// the slot block-scoped (slot = &work->slots[i]). Both pointers are then givs
// of i; loop.c records givs at the head of its list, so the owned byte's
// (found last) is set up first, lives longer and loses r5 to the slot pointer,
// as the native; the dbra reversal gives the 23 down to 0 count. A slot pointer
// stepped with slot++ is the biv and always lives longer than its giv (12).
// The owner's layer is the sprite's OAM attribute 2 priority, through
// field_sprite.h's struct FieldSprite (u16 tile : 10, priority : 2), as
// item_break_mode3.c writes it; a plain store then matches, so the s16 layer
// temporary is gone (a u8 bitfield at byte 9 needed it: 8 without). Left as
// ordinary C (S2): the local holding 17, without which fold makes the
// first test cmp #16; ble (4 of 171 differ).
// BattleEffect_CleanupSceneObjects, the field event runner's cleanup
// of the heap slot 0xe0 effect work. Clears every owned effect slot, ends the
// arc effects unless the event work's byte 0xcb6 holds them, waits up to 90
// frames for block 32's busy byte in event mode 3, stops the effect slots'
// callback, restores the map work's saved screen shake, gives the owner object
// back its priority flags and sprite priority back to the owner object, marks
// the event work's flags 0xca5 and 0xcad unless the effect's animation was 8 or
// 28, and releases the work.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "field_particle.h"
#include "field_sprite.h"
#include "map.h"

struct CleanupWork {
  u8 unk000[16];
  struct Actor *owner;
  u8 unk014[10];
  s16 animation;
  u8 unk020[36];
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  struct FieldParticle slots[24];
  u8 unk710[8];
  u8 ownerByte;
  u8 layer;
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
s32 Task_Remove(void (*func)(void));
void BattleEffect_UpdateAllEffectSlots(void);
void Event_NoOp(void);
void HeapSlot_Release(s32 slot);

void BattleEffect_CleanupSceneObjects(void) {
  struct CleanupWork *work = Ram_HeapSlots->effectWork;
  struct CleanupEventWork *event = Ram_HeapSlots->eventWork;
  s8 *block = Ram_HeapSlots->block32;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *owner = work->owner;
  s32 i;
  s16 first;

  for (i = 0; i < 24; i++) {
    struct FieldParticle *slot = &work->slots[i];
    if (slot->active)
      BattleEffect_ClearOwnedSlot(slot);
  }
  // A local that holds one constant: it makes the first test cmp #17; blt as
  // the native (with the literal, 4 of 171 differ in tla-en).
  first = 17;
  if (work->animation >= first &&
      (work->animation <= 27 || work->animation == 30))
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
  map->shakeX = work->savedShakeX;
  map->shakeY = work->savedShakeY;
  map->shakeDecay = work->savedShakeDecay;
  if (owner != NULL) {
    owner->priorityFlags = work->ownerByte;
    ((struct FieldSprite *)owner->animation)->priority = work->layer;
  }
  if (work->animation != 8)
    event->unkca5 = 1;
  if (work->animation != 28)
    event->unkcad = 1;
  Event_NoOp();
  HeapSlot_Release(224);
}

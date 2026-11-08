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

#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "fx_scene.h"
#include "actor.h"

struct FxGroup {
  u8 unk00[32];
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
};

union FxSprite {
  u16 halves[10];
  u32 words[5];
};

s32 Func_080cdac0(s32 effect, s32 from, s32 to);

/// Gathers up to eight live objects, other than the scene's main object, for
/// which Func_080cdac0 measures a distance from the first object, with each
/// one's sprite value.
void ViewMove_SaveNearbySprites(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct FxGroup *group = (struct FxGroup *)Ram_HeapSlots->battleFxWork[0];
  struct Actor **objects;
  struct Actor *obj;
  s32 i;

  group->count = 0;
  objects = work->objects;
  /* FAKEMATCH: the empty asm barrier keeps the walk pointer's spill ahead of
   * the counter's zero; without it, 2 instructions differ in tla-en. */
  asm("");
  for (i = 0; i < 80; i++) {
    obj = *objects++;
    if (obj == NULL || obj->script == NULL || obj == scene->mainObject)
      continue;
    if (Func_080cdac0((s16)(scene->fieldKind | 0x100), scene->firstObjectId,
                      i) == -1)
      continue;
    group->objects[group->count] = obj;
    group->values[group->count] = ((union FxSprite *)obj->animation)->halves[9];
    if (++group->count == 8)
      break;
  }
}

/// Writes each gathered object's saved sprite value back.
void ViewMove_RestoreNearbySprites(void) {
  struct FxGroup *group = (struct FxGroup *)Ram_HeapSlots->battleFxWork[0];
  s32 i;

  for (i = 0; i < group->count; i++)
    ((union FxSprite *)group->objects[i]->animation)->halves[9] =
        group->values[i];
}

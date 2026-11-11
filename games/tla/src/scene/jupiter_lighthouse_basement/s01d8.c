#include "types.h"

#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "frame.h"
#include "field_sprite.h"
#include "event_inline.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

struct CellTarget {
  u8 unk00[6];
  s16 x;
  s16 z;
};

s32 Math_Cosine(s32 angle);
void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void Func_0200015c(struct Actor *object);
void Func_020004dc(s32 mode);
void Func_02001d3c(s32 id);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

void Func_020001d8(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(object, 0);
    Map_EnableUpdateCallback();
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  Map_DisableUpdateCallback();
  if (OBJECT_BYTE(object, 99) != 0) {
    if ((gFrameCount & 7) == 0)
      OBJECT_BYTE(object, 99)--;
    return;
  }
  if (*(s16 *)((u8 *)object + 102) == 0)
    Func_0200015c(object);
  (*(s16 *)((u8 *)object + 102))--;
  /* FAKEMATCH(dead): an empty once-only block, whose loop note ends the
   * scheduling block, so the timer is stored before z loads as in the native;
   * without it, 2 of 87 instructions differ in tla-en. */
  do {
  } while (0);
  object->x += Math_Cosine(((object->z >> 14) + (object->y >> 15)) << 8) / 2;
  object->y += object->speedLimit;
}

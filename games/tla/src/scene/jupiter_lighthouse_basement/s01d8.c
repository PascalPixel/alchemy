#include "types.h"

#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "scene_ids.h"
#include "frame.h"
#include "field_sprite.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

struct CellTarget {
  u8 unk00[6];
  s16 x;
  s16 z;
};

s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
s32 Math_Cosine(s32 angle);
void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_0200015c(struct Actor *object);
void Func_020004dc(s32 mode);
struct Actor *Func_02001c04(s32 type, s32 x, s32 y, s32 z);
void Func_02001c34(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
struct Actor *Func_02001cdc(void);
void Func_02001d3c(s32 id);
void Func_02001d4c(s32 cue);

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
  /* FAKEMATCH: the loop note keeps the timer load after the reset call (stated
   * plainly it is loaded before the test as well, 13 differ). */
  do {
  } while (0);
  (*(s16 *)((u8 *)object + 102))--;
  /* FAKEMATCH: the loop note ends the scheduling block: the native stores the
   * timer before it loads z (2 differ without it). */
  do {
  } while (0);
  object->x += Math_Cosine(((object->z >> 14) + (object->y >> 15)) << 8) / 2;
  object->y += object->speedLimit;
}

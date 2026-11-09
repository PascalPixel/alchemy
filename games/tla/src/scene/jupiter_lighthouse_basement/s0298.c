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
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void Func_0200015c(struct Actor *object);
void Func_020001d8(struct Actor *object);
void Func_020004dc(s32 mode);
void Func_02001d3c(s32 id);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

void Func_02000298(void) {
  s32 i;
  s32 next;

  struct Actor *object;
  s16 *action;
  s16 *timer;

  i = 0;
  do {
    object = (FieldActor_Create(0x2a0, 0, 0, 0));

    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (i << 12) + 0x8000;
    object->acceleration = (i << 12) + 0x8000;
    object->update = (void (*)(void))Func_020001d8;
    OBJECT_BYTE(object, 99) = i << 3;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetSpriteColor(object, 2);
    ((struct FieldSprite *)object->animation)->priority = 1;
    Animation_SetStateFlags(object, 1);
    action = &object->action;
    timer = (s16 *)&object->unk66;
    next = i + 1;
    if (i & 1) {
      s32 parameter = 212;
      *action = parameter;
      *timer = 0;
    } else {
      s32 parameter = 116;
      *action = parameter;
      *timer = 0;
    }
    i = next;
  } while (i != 16);
}

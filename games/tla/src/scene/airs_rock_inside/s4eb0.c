#include "types.h"
#include "actor.h"
#include "field_sprite.h"

struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Func_02000550(void);

void Func_02004eb0(void) {
  struct Actor *object;
  s32 i = 0;
  struct FieldSprite *sprite;

do {
  object = Import_ActorCreate(672, 0, 0, 0);
  sprite = object->animation;
  sprite->priority = 1;
  ((u8 *)object)[0x63] = i << 4;
  { s32 zero = 0;
  object->motionFlags = zero;
  object->priorityFlags = zero; }
  object->acceleration = object->speedLimit = (i << 12) + 0x8000;
  object->update = Func_02000550;
  Actor_SetSingleChildFlags(object, 0);
  Actor_SetPartAttribute(object, 2);
  if (i & 1) {
    object->action = 212;
    object->unk66 = 0;
  } else {
    object->action = 116;
    object->unk66 = 0;
  }
  Animation_SetStateFlags(object, 1);
  i++;
} while (i != 8);
}

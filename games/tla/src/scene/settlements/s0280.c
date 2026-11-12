#include "local.h"
#include "field_event.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02000280(void) { Func_080d7788(10, 1, 14); }

void Func_02000290(void) { Func_080d7788(11, 2, 14); }

void Func_020002a0(void) { Func_080d7788(23, 3, 19); }

s32 Func_020002b0(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

s32 Func_020002bc(struct FieldActor *object) {
  s32 step = object->unk5a[8] & 63;
  struct FieldSprite *sprite = object->sprite;

  object->motionFlags = 3;
  if (step == 0)
    object->velocityY = PIXELS(3);
  if (step == 16)
    object->velocityY = PIXELS(2);
  if (step == 24)
    object->velocityY = PIXELS(3);
  sprite->rotation = Math_Cosine(step << 12) / 64;
  object->unk5a[8]++;
  return -1;
}

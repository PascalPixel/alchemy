#include "types.h"
#include "field_event.h"
#include "scene_tables.h"

s32 Math_Cosine(s32 angle);

s32 Func_02000058(struct FieldActor *object) {
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

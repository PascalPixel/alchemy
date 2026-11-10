#include "local.h"

void Func_02003e34(struct FieldSprite *sprite, u32 frames, s32 step) {
  u32 count = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    sprite->rotation += step;
    Event_Wait(1);
  }
}

void Func_02003e60(struct Actor *object, u32 frames, s32 step) {
  u32 count = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    object->x += step;
    Event_Wait(1);
  }
}

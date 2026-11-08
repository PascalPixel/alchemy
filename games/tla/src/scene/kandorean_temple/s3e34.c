#include "local.h"

void Func_02003e34(struct FieldSprite *sprite, u32 frames, s32 step) {
  /* FAKEMATCH: the native keeps the frame count in r6 and the sprite in r7;
   * unpinned, the sprite's five weighted uses outrank the count's four and take
   * r6 (differ 12). */
  register u32 count asm("r6") = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    sprite->rotation += step;
    FieldEvent_Wait(1);
  }
}

void Func_02003e60(struct Actor *object, u32 frames, s32 step) {
  /* FAKEMATCH: as Func_02003e34, the count pinned to r6 (differ 12 unpinned).
   */
  register u32 count asm("r6") = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    object->x += step;
    FieldEvent_Wait(1);
  }
}

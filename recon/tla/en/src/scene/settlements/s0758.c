#include "../../../../../../games/tla/src/scene/settlements/local.h"

void Func_02000714(struct Actor *object) {
  s32 step;

  object->x += object->action << 8;

  object->y += 0x8000;

  step = SPLIT_CONSTANT(0x500, 30);
  object->scaleX += step;
  object->scaleY += step;
  object->action += 2;
  if (--*(s32 *)&object->watchedObject == 0)
    Func_02002000(object);
}

void Func_02000758(s32 x, s32 y, s32 z) {
  struct Actor *object = Func_02001ff8(0x11d, x, y, z);

  if (object != NULL) {
    s8 *animation;

    object->unk5c[0] = 2;
    object->motionFlags = 0;
    object->action = 0;
    object->priorityFlags &= 0xfe;
    Actor_SetPartAttribute(object, 7);
    animation = object->animation;
    animation[9] = (animation[9] & -13) | 4;
    Actor_SetSingleChildFlags(object, 0);
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
    *(s32 *)&object->watchedObject = 60;
    object->update = (void (*)(void))Func_02000714;
    Func_02001fe0(object, 5);
    Animation_SetStateFlags(object, 1);
  }
}

void Func_020007d8(void) {
  if ((gFrameCount & 63) == 0)
    Func_02000758(0xf80000, 0x300000, 0x1000000);
}

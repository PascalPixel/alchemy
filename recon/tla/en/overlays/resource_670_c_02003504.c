// Draft of overlay 670's Func_02003504, differ 130 of 116: the flashing of
// actor 17, then sixteen sparks (object 673) spread round it, each with
// Func_02003308 as its update. The behaviour is decoded; the allocation is not.
// The native keeps the centre in r8, the count in r10, the angle in r9 and
// Func_02005c44's chain in r11, builds a fresh zero in r5 in each spark's body,
// and loads the timer's zero from the pool into r6 (a QImode constant reloaded
// at its use, which also puts a literal pool and a branch round it before the
// loop's increment). Here loop.c hoists one zero for all the stores and the
// counter takes r7. A u8, s16 or s32 zero declared for the timer changes
// nothing (118); permute 4 minutes reached differ 88 by register choices only.
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

s32 Func_02005c44(struct FieldSprite *sprite, s32 previous);
void Func_02003308(struct KibomboObject *object);

void Func_02003504(void) {
  struct KibomboObject *center = Actor_GetById(17);
  struct KibomboObject *spark;
  s32 previous = 0;
  u32 i;
  u32 angle;

  Func_02005c9c(154);
  for (i = 0; i <= 7; i++) {
    Actor_SetPartAttribute(Actor_GetById(17), 7);
    wait(2);
    Actor_SetPartAttribute(Actor_GetById(17), 0);
    wait(2);
  }
  Func_02005c9c(209);
  for (i = 0, angle = 0; i <= 15; i++, angle += 0x160000) {
    spark = Func_02005a34(673, center->x, center->y + 0x180000, center->z);
    if (spark != 0) {
      previous = Func_02005c44(spark->sprite, previous);
      spark->motionFlags = 0;
      spark->sprite->priority = 1;
      Actor_SetSingleChildFlags(spark, 0);
      Func_02005a24(spark, 1);
      spark->field64 = 0;
      spark->field4c = 0;
      spark->field66 = angle / 360;
      spark->timer = 0;
      spark->targetX = center->x;
      spark->targetY = center->y + 0x180000;
      spark->targetZ = center->z;
      spark->update = Func_02003308;
    }
  }
}

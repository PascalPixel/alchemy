#include "../../../../../../games/tla/src/scene/kibombo/kibombo.h"

s32 Func_02005c44(struct FieldSprite *sprite, s32 previous);
void Func_02003308(struct KibomboObject *object);

void Func_02003504(void) {
  struct KibomboObject *center = Actor_GetById(17);
  struct KibomboObject *spark;
  s32 previous = 0;
  u32 i;
  u32 angle;
  s32 x;
  s32 y;
  s32 z;

  Func_02005c9c(154);
  for (i = 0; i <= 7; i++) {
    Actor_SetPartAttribute(Actor_GetById(17), 7);
    wait(2);
    Actor_SetPartAttribute(Actor_GetById(17), 0);
    wait(2);
  }
  Func_02005c9c(209);
  i = 0;
  angle = 0;
  do {
    x = center->x;
    y = center->y + 0x180000;
    z = center->z;
    spark = Func_02005a34(673, x, y, z);
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
    angle += 0x160000;
    i++;
  } while (i <= 15);
}

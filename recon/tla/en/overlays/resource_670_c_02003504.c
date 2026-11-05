// Draft, differ 23 of 116, skeleton gap 5/4 (tla-en, 2026-11-01)
// Overlay 670's Func_02003504: the flashing of actor 17, then sixteen sparks
// (object 673) spread round it, each with Func_02003308 as its update.
// What closed it from 130: the spark loop as a goto loop, so loop.c leaves its
// zero in the body and the timer's halfword zero loads from the pool (range 64,
// which is what forces the pool and the b round it mid-function); and x, y and
// z read into locals before the call, so 673's pseudo is not yet live and r0
// serves as a reload register (the spill set becomes the native's r0, r2, r3).
// Left: one site. The native's mask is `movs r0, #13; negs r0, r0; adds r2, r0,
// #0`, a QImode -13 that reload builds, and that extra reload is also why the
// tail's 1 and 176 and the mov r11, r0 sit where they do. In a goto loop
// combine folds the andsi3 copy (set 57 (subreg 55)) into a plain SImode -13.
// loop.c's single-use substitution in scan_loop removes that copy first, so
// the native is a real loop, and there (for or do, differ 130) the zero, life
// 31, is hoisted. Open question, the same as 699's Func_02000298: what keeps
// the zero in a real loop. Needed: reg_in_basic_block_p false, or a life of 4
// or less at loop.c. A dead zero before the loop stops the first zero but
// hoists the pool one (96), and is a device. Not it: u8 zero local in the body
// (130), a chained field4c = field64 = 0. A one-pass block before any of the
// calls or stores does not help either. Lower differs that are the wrong kind:
// a u8 view gets 20 with mask 243, s8 flags 18 with a stray sign extension.
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

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
  i++;
  angle += 0x160000;
} while (i <= 15);
}

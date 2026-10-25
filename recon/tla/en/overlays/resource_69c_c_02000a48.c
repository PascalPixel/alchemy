// Draft, differ 4 of 72: overlay 69c's copy of the function drafted in
// resource_6a4_c_02000de0.c (overlay 6a4's Func_02000de0, differ 4 there), whose
// header says what it is, what still differs and what was tried.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"

s32 Func_02000a48(struct ObjectRuntime *object) {
  struct JupiterLighthouseAerieSpark spark;
  s32 frame = gFrameCount & 7;

  if (frame == 0) {
    s32 x;
    s32 y;
    s32 speed;

    spark.word[0] = 3;
    spark.word[2] = 0xb333;
    spark.word[3] = 0xb333;
    spark.word[1] = 14;
    x = object->x + (((((u32)Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (((u32)Random16Far() * 20) >> 16)) << 16);
    speed = (s32)(((((u32)Random16Far() * 5) >> 16) << 16) + 0x50000) / 10;
    Func_02001788(x, y, object->z, 0, speed, frame, 0xb0000, &spark);
  }
  return 0;
}

#include "../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"

void Func_020010a8(struct SeaOfTimeObject *object) {
  struct SeaOfTimeSpark spark;
  s32 even = gFrameCount & 1;

  if (even == 0) {
    s32 dx = ((Random16Far() & 31) << 16) + -0xe0000;

    spark.scaleX = 0xb333;
    spark.scaleY = 0xb333;
    spark.kind = 7;
    spark.mode = 1;
    Func_0200015c(object->x + dx, 0, object->z + 0x100000, 0, even, even,
                  0xb0000, &spark);
  }
}

#include "local.h"

s32 Func_02001228(struct ObjectRuntime *object) {
  s32 y = object->y;
  struct ObjectRuntime *spark;
  s32 scale;

  if (y + object->velocityY < object->terrainHeight) {
    spark = Func_02002808_(0x10d, object->x, y, object->z);
    scale = 0x8ccc;
    if (spark) {
      spark->scaleX = scale;
      spark->scaleY = scale;
      Func_02002800(spark, gData_02002a48);
      ObjectDispatch_SetSingleChildFlags(spark, 0);
      Func_020027f8(spark, 2);
    }
    Func_020029b0(106);
    object->y = object->terrainHeight;
    object->velocityY = 0;
    return 0;
  }
  return 1;
}

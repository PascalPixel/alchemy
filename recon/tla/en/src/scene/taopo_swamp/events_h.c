#include "../../../../../../games/tla/src/scene/taopo_swamp/local.h"

s32 Func_02001564(struct Actor *object) {
  s32 flag;
  s32 count;
  struct Actor *spark;
  s32 scale;

  if (object->y + object->velocityY < object->terrainHeight) {
    flag = GameFlag_Test(SPLIT_CONSTANT(0x122, 255));
    if (flag) {
      Func_020029b0(106);
      return 0;
    }
    spark = Func_02002808_(0x10d, object->x, object->y, object->z);
    scale = SPLIT_CONSTANT(0x8c00, 204);
    if (spark) {
      spark->scaleX = scale;
      spark->scaleY = scale;
      Func_02002800(spark, gData_02002a48);
      Actor_SetSingleChildFlags(spark, 0);
      Func_020027f8(spark, 2);
    }
    Func_020029b0(106);
    object->y = object->terrainHeight;
    object->velocityY = 0;
    if (GameFlag_Test(0x104) == 0) {
      count = GameFlag_GetByte_(0x230);
      if (count <= 3) {
        count++;
        GameFlag_SetByte(0x230, count);
      }
      if (count != 0)
        Func_02000920(0, 10);
      object = Actor_GetById(10);
      object->scaleX = count << 14;
      object->scaleY = count << 14;
    }
    return 0;
  }
  return 1;
}

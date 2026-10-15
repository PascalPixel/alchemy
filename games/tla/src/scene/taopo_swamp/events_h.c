#include "local.h"

s32 Func_02001564(struct ObjectRuntime *object)
{
    s32 flag;
    s32 count;
    struct ObjectRuntime *spark;
    s32 scale;

    if (object->y + object->velocityY < object->terrainHeight) {
        flag = GameFlag_Test(0x221);
        if (flag) {
            Func_020029b0(106);
            return 0;
        }
        spark = Func_02002808_(0x10d, object->x, object->y, object->z);
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
        if (GameFlag_Test(0x104) == 0) {
            count = GameFlag_GetByte_(0x230);
            if (count <= 3) {
                count++;
                GameFlag_SetByte_(0x230, count);
            }
            if (count != 0)
                Func_02000920(0, 10);
            object = Object_GetById(10);
            object->scaleX = count << 14;
            object->scaleY = count << 14;
        }
        return 0;
    }
    return 1;
}

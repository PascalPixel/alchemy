#include "local.h"

s32 Func_02001228(struct ObjectRuntime *object)
{
    s32 y = object->y;
    struct ObjectRuntime *spark;
    s32 scale;

    if (y + object->velocity_y < object->terrain_height) {
        spark = Func_02002808_(0x10d, object->x, y, object->z);
        scale = 0x8ccc;
        if (spark) {
            spark->scale_x = scale;
            spark->scale_y = scale;
            Func_02002800(spark, Data_02002a48);
            ObjectDispatch_SetSingleChildFlags(spark, 0);
            Func_020027f8(spark, 2);
        }
        Func_020029b0(106);
        object->y = object->terrain_height;
        object->velocity_y = 0;
        return 0;
    }
    return 1;
}

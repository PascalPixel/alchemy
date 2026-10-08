/* Near miss (6 of 41 instructions differ): the native loads the
 * 0xfff20000 offset from the pool before it masks the random value, and
 * stores the spark's address to the stack before the position loads. */
#include "../../../../games/tla/src/field/overlay_687/ov687.h"

void Func_020010a8(struct Ov687Object *object)
{
    struct Ov687Spark spark;
    s32 even = gFrameCount & 1;

    if (even == 0) {
        s32 dx = ((Random16Far() & 31) << 16) + -0xe0000;

        spark.scale_x = 0xb333;
        spark.scale_y = 0xb333;
        spark.kind = 7;
        spark.mode = 1;
        Func_0200015c(object->x + dx, 0, object->z + 0x100000, 0, even, even, 0xb0000, &spark);
    }
}

/* Near miss: differ 4 of 72 (two reordered rows): the native loads the
 * object's x between the two halves of the first random's multiply by nine. */
#include "../../../../games/tla/src/field/overlay_6a4/ov6a4.h"

s32 Func_02000de0(struct ObjectRuntime *object)
{
    struct Ov6a4Spark spark;
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
        Func_02001e78(x, y, object->z, 0, speed, frame, 0xb0000, &spark);
    }
    return 0;
}

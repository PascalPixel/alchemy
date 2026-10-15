/* Near miss, differ 19 of 32: the native loads the address of
 * Data_02006294 into r2 before the sign test and builds the 0xbb80
 * threshold in r4, then loads the address again into r5 for the step; the
 * draft loads it inside each branch and uses r2 for the threshold. A
 * counter pointer local, a division and a one-pass block were tried. */
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"

void Func_02000668(struct ObjectRuntime *object, s32 step)
{
    s32 speed = (object->velocityX + object->velocityZ) >> 1;
    s32 *counter = &Data_02006294;

    if (speed < 0)
        speed = -speed;
    if ((u32)speed > 0xbb80)
        *counter += 4;
    else if (speed != 0)
        *counter += 2;
    Data_02006294 += step;
    if (Data_02006294 > 40) {
        Func_020006b0(object);
        Data_02006294 = 0;
    }
}

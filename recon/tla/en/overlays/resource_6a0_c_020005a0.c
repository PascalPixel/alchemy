/* Near miss, differ 49 of 33: the listing reloads the object's x and z and rebuilds -1 on
 * every pass and indexes the table by two counters; this draft hoists them (ip, lr) and shares one index. */
#include "../../../../games/tla/src/field/overlay_6a0/ov6a0.h"

s32 Func_020005a0(struct ObjectRuntime *object)
{
    s32 i;

    for (i = 0; Data_02002c64[i * 2] != -1; i++) {
        if (Data_02002c64[i * 2] == object->x >> 20 && Data_02002c64[i * 2 + 1] == object->z >> 20)
            return 1;
    }
    return 0;
}

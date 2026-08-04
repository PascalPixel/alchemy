/* Integrate an object's position, presentation offsets and heading. */
#include "types.h"

void Func_02000104(s32 *object)
{
    u16 *owner;

    object[2] += object[17];
    object[3] += object[18];
    object[4] += object[19];
    object[6] += object[12];
    object[7] += object[13];

    owner = (u16 *)object[20];
    owner[15] += *(u16 *)((u8 *)object + 100);
}

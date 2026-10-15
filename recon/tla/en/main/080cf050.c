#include "types.h"
#include "object_runtime.h"

s32 EventRuntime_GetControlledOwner(void);
s32 ArcTan2(s32 y, s32 x);
void Vector_AddPolarOffset(s32 distance, u16 angle, s32 *position);

void Func_080cf050(void)
{
    struct ObjectRuntime *leader;
    struct ObjectRuntime *other;
    s32 dx;
    s32 dz;
    s32 i;

    leader = ObjectTable_Get(EventRuntime_GetControlledOwner());
    for (i = 0; i <= 15; i++) {
        other = ObjectTable_Get(i + 64);
        dx = leader->x - other->x;
        dz = leader->z - other->z;
        if (dx > -0x100000 && dx < 0x100000)
            if (dz > -0x100000 && dz < 0x100000) {
                leader->x = other->x;
                leader->z = other->z;
                Vector_AddPolarOffset(0x140000, ArcTan2(dz, dx), &leader->x);
                leader->targetX = 0x80000000;
                leader->targetY = 0x80000000;
                leader->targetZ = 0x80000000;
                break;
            }
    }
}

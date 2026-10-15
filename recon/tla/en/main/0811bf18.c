/* Near miss, differ 4 of 61: the listing loads the pool zero for the blend register before it
 * builds the register address, which also puts the literal pool before the call to Object_SetPosition;
 * this draft builds the address first. An empty loop note before the store fixes both but moves the
 * load of the target and the add of the second quotient. */
#include "types.h"
#include "object_runtime.h"

void Func_0811bf18(struct ObjectRuntime **from, struct ObjectRuntime **to, s32 percent)
{
    struct ObjectRuntime *object = *from;
    struct ObjectRuntime *target = *to;
    s32 x;
    s32 z;

    x = object->x + (target->x - object->x) * percent / 100;
    z = object->z + (target->z - object->z) * percent / 100;
    *(volatile u16 *)0x04000050 = 0;
    object->acceleration = 0x20000;
    object->speedLimit = 0x80000;
    object->velocityY = 0x40000;
    object->gravity = 0xab85;
    object->bounce = 0;
    object->actionFlags = 1;
    Object_SetPosition(object, x, 0, z);
    Object_SetMode(object, 2);
}

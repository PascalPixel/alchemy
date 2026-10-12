/* Draft, differ 29 of 48: the native reloads the table address (ldr r1) and builds 1 and 15 inside the outer loop, saving only r8 and r10; this form hoists the table address and the 1 out of the loop into r11 and r9. A do-while outer loop measures the same. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

extern const s8 Data_02005a88[];

void Func_020009ac(s32 a, s32 b)
{
    s32 id;

    id = 18;
    do {
        struct ObjectRuntime *object = Object_GetById(id);
        s32 x = object->x >> 20;
        s32 z = object->z >> 20;
        u8 *covered = &OBJECT_BYTE(Object_GetById(Data_02005a88[(id - 18) & 3]), 99);
        s32 other;

        *covered = 0;
        for (other = 15; other <= 17; other++) {
            object = Object_GetById(other);
            if (x == object->x >> 20 && z == object->z >> 20) {
                *covered = 1;
                break;
            }
        }
    } while (++id <= 21);
}

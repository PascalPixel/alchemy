/* Near miss: differ 30 of 86, registers only: the native keeps the id in r5
 * and the wait counter's shifted copy in r7; the draft has them the other way
 * round. With the flag calls through the inline forwarder the object and the
 * id also swap (differ 38). */
#include "../../../../games/tla/src/field/overlay_686/ov686.h"

void Func_02000a68(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s16 frames = 60;

    if (object->z >> 20 == 31) {
        object->motion_flags = 3;
        Func_02002e34(object, 3);
        Func_02002eec(204);
        while (frames != 0) {
            WaitFrames(1);
            if (object->velocity_y == 0)
                break;
            frames--;
        }
        Func_02002e6c(id, 0, 0);
        if (id == 10)
            GameFlag_SetBit(0x9d2);
        if (id == 11)
            GameFlag_SetBit(0x9d3);
        if (id == 12)
            GameFlag_SetBit(0x9d4);
        if (id == 13)
            GameFlag_SetBit(0x9d5);
        if (id == 14)
            GameFlag_SetBit(0x9d6);
        if (id == 15)
            GameFlag_SetBit(0x9d7);
        if (id == 16)
            GameFlag_SetBit(0x9d8);
    }
}

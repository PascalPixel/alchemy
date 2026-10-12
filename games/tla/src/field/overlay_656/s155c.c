#include "ov656.h"

void Func_0200155c(void)
{
    struct Ov656EventWork *work = Ram_HeapSlots->event_work;
    struct Ov656Object *object;

    if (GameFlag_Test(0x202)) {
        object = Object_GetById(10);
        if ((object->x | object->z) == 0) {
            work->field_16a = 200;
            Object_SetModeById(8, 1);
        }
    }
    if (GameFlag_Test(0x203)) {
        object = Object_GetById(11);
        if ((object->x | object->z) == 0) {
            work->field_16a = 201;
            Object_SetModeById(9, 1);
        }
    }
}

void Func_020015c8(void)
{
    s32 position[3];
    struct Ov656Object *object;

    position[0] = 0x3280000;
    position[1] = 0x80000;
    position[2] = 0x8c0000;
    Vector_AddPolarOffsetFar(0x200000, Random16Far(), position);
    object = Func_02002c00(0x2a2, position[0], position[1], position[2]);
    if (object != NULL) {
        object->motion_flags = 0;
        *(s32 *)((u8 *)object + 0x30) = 0x20000;
        *(s32 *)((u8 *)object + 0x34) = 0x8000;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_02002bf0(object, 0);
        Func_02002c18(object, 0x3280000, 0x80000, 0x8c0000);
        Func_02002bf8(object, Data_02003ef8);
    }
}

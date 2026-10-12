#include "ov676.h"

void Func_02001688(void)
{
    struct ObjectRuntime *a = Object_GetById(8);
    struct ObjectRuntime *b = Object_GetById(10);
    struct ObjectRuntime *c = Object_GetById(9);
    struct ObjectRuntime *d = Object_GetById(11);
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s32 value = (s16)a->unknown_66;

    switch (value) {
    case 1:
        BYTE_AT(a, 98) = 0;
        BYTE_AT(b, 98) = 0;
        BYTE_AT(c, 98) = 0;
        BYTE_AT(d, 98) = 0;
        BYTE_AT(leader, 98) = 0;
        break;
    case 2:
        BYTE_AT(a, 98) = 2;
        BYTE_AT(b, 98) = 2;
        BYTE_AT(c, 98) = 2;
        BYTE_AT(d, 98) = 2;
        BYTE_AT(leader, 98) = 2;
        break;
    case 3:
        BYTE_AT(a, 98) = 3;
        BYTE_AT(b, 98) = 3;
        BYTE_AT(c, 98) = 3;
        BYTE_AT(d, 98) = 3;
        BYTE_AT(leader, 98) = 3;
        break;
    }
    a->unknown_66 = 0;
    Func_020006bc(8);
    Func_020006bc(10);
    Func_020006bc(9);
    Func_020006bc(11);
    Func_020006bc(gPartyState.current_owner);
}

void Func_02001730(void)
{
    struct ObjectRuntime *object;

    object = Object_GetById(8);
    Data_02002fc0[0].action = object->action;
    Data_02002fc0[0].mode = (s16)object->unknown_66;
    Data_02002fc0[0].byte_98 = BYTE_AT(object, 98);
    Data_02002fc0[0].byte_99 = BYTE_AT(object, 99);
    object = Object_GetById(10);
    Data_02002fc0[1].action = object->action;
    Data_02002fc0[1].mode = (s16)object->unknown_66;
    Data_02002fc0[1].byte_98 = BYTE_AT(object, 98);
    Data_02002fc0[1].byte_99 = BYTE_AT(object, 99);
    object = Object_GetById(9);
    Data_02002fc0[2].action = object->action;
    Data_02002fc0[2].mode = (s16)object->unknown_66;
    Data_02002fc0[2].byte_98 = BYTE_AT(object, 98);
    Data_02002fc0[2].byte_99 = BYTE_AT(object, 99);
    object = Object_GetById(11);
    Data_02002fc0[3].action = object->action;
    Data_02002fc0[3].mode = (s16)object->unknown_66;
    Data_02002fc0[3].byte_98 = BYTE_AT(object, 98);
    Data_02002fc0[3].byte_99 = BYTE_AT(object, 99);
    object = Object_GetById(gPartyState.current_owner);
    Data_02002fc0[4].action = object->action;
    Data_02002fc0[4].mode = (s16)object->unknown_66;
    Data_02002fc0[4].byte_98 = BYTE_AT(object, 98);
    Data_02002fc0[4].byte_99 = BYTE_AT(object, 99);
    Scheduler_RemoveCallbackFar(Func_02001688);
}

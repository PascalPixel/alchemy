#include "yallam.h"

void Func_02001688(void)
{
    struct ObjectRuntime *a = Object_GetById(8);
    struct ObjectRuntime *b = Object_GetById(10);
    struct ObjectRuntime *c = Object_GetById(9);
    struct ObjectRuntime *d = Object_GetById(11);
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    s32 value = (s16)a->unk66;

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
    a->unk66 = 0;
    Func_020006bc(8);
    Func_020006bc(10);
    Func_020006bc(9);
    Func_020006bc(11);
    Func_020006bc(gPartyState.currentOwner);
}

void Func_02001730(void)
{
    struct ObjectRuntime *object;

    object = Object_GetById(8);
    gData_02002fc0[0].action = object->action;
    gData_02002fc0[0].mode = (s16)object->unk66;
    gData_02002fc0[0].byte98 = BYTE_AT(object, 98);
    gData_02002fc0[0].byte99 = BYTE_AT(object, 99);
    object = Object_GetById(10);
    gData_02002fc0[1].action = object->action;
    gData_02002fc0[1].mode = (s16)object->unk66;
    gData_02002fc0[1].byte98 = BYTE_AT(object, 98);
    gData_02002fc0[1].byte99 = BYTE_AT(object, 99);
    object = Object_GetById(9);
    gData_02002fc0[2].action = object->action;
    gData_02002fc0[2].mode = (s16)object->unk66;
    gData_02002fc0[2].byte98 = BYTE_AT(object, 98);
    gData_02002fc0[2].byte99 = BYTE_AT(object, 99);
    object = Object_GetById(11);
    gData_02002fc0[3].action = object->action;
    gData_02002fc0[3].mode = (s16)object->unk66;
    gData_02002fc0[3].byte98 = BYTE_AT(object, 98);
    gData_02002fc0[3].byte99 = BYTE_AT(object, 99);
    object = Object_GetById(gPartyState.currentOwner);
    gData_02002fc0[4].action = object->action;
    gData_02002fc0[4].mode = (s16)object->unk66;
    gData_02002fc0[4].byte98 = BYTE_AT(object, 98);
    gData_02002fc0[4].byte99 = BYTE_AT(object, 99);
    Scheduler_RemoveCallbackFar(Func_02001688);
}

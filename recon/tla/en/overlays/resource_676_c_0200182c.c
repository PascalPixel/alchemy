/* Draft: differ 48 (listing 96 instructions, draft 96), all reordered rows:
 * after each object's first store the native begins the destination address
 * before it loads the saved word, and sets the next id before the last byte
 * store; the draft loads each saved word first. Func_02001730, the saving
 * half, matches in this form (s1688.c). */
#include "../../../../games/tla/src/scene/yallam/yallam.h"

void Func_0200182c(void)
{
    struct ObjectRuntime *object;

    object = Object_GetById(8);
    object->action = gData_02002fc0[0].action;
    object->unk66 = gData_02002fc0[0].mode;
    BYTE_AT(object, 98) = gData_02002fc0[0].byte98;
    BYTE_AT(object, 99) = gData_02002fc0[0].byte99;
    object = Object_GetById(10);
    object->action = gData_02002fc0[1].action;
    object->unk66 = gData_02002fc0[1].mode;
    BYTE_AT(object, 98) = gData_02002fc0[1].byte98;
    BYTE_AT(object, 99) = gData_02002fc0[1].byte99;
    object = Object_GetById(9);
    object->action = gData_02002fc0[2].action;
    object->unk66 = gData_02002fc0[2].mode;
    BYTE_AT(object, 98) = gData_02002fc0[2].byte98;
    BYTE_AT(object, 99) = gData_02002fc0[2].byte99;
    object = Object_GetById(11);
    object->action = gData_02002fc0[3].action;
    object->unk66 = gData_02002fc0[3].mode;
    BYTE_AT(object, 98) = gData_02002fc0[3].byte98;
    BYTE_AT(object, 99) = gData_02002fc0[3].byte99;
    object = Object_GetById(gPartyState.currentOwner);
    object->action = gData_02002fc0[4].action;
    object->unk66 = gData_02002fc0[4].mode;
    BYTE_AT(object, 98) = gData_02002fc0[4].byte98;
    BYTE_AT(object, 99) = gData_02002fc0[4].byte99;
    Scheduler_AddOrUpdateCallback(Func_02001688, 0x480);
}

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
    object->action = Data_02002fc0[0].action;
    object->unknown_66 = Data_02002fc0[0].mode;
    BYTE_AT(object, 98) = Data_02002fc0[0].byte_98;
    BYTE_AT(object, 99) = Data_02002fc0[0].byte_99;
    object = Object_GetById(10);
    object->action = Data_02002fc0[1].action;
    object->unknown_66 = Data_02002fc0[1].mode;
    BYTE_AT(object, 98) = Data_02002fc0[1].byte_98;
    BYTE_AT(object, 99) = Data_02002fc0[1].byte_99;
    object = Object_GetById(9);
    object->action = Data_02002fc0[2].action;
    object->unknown_66 = Data_02002fc0[2].mode;
    BYTE_AT(object, 98) = Data_02002fc0[2].byte_98;
    BYTE_AT(object, 99) = Data_02002fc0[2].byte_99;
    object = Object_GetById(11);
    object->action = Data_02002fc0[3].action;
    object->unknown_66 = Data_02002fc0[3].mode;
    BYTE_AT(object, 98) = Data_02002fc0[3].byte_98;
    BYTE_AT(object, 99) = Data_02002fc0[3].byte_99;
    object = Object_GetById(gPartyState.current_owner);
    object->action = Data_02002fc0[4].action;
    object->unknown_66 = Data_02002fc0[4].mode;
    BYTE_AT(object, 98) = Data_02002fc0[4].byte_98;
    BYTE_AT(object, 99) = Data_02002fc0[4].byte_99;
    Scheduler_AddOrUpdateCallback(Func_02001688, 0x480);
}

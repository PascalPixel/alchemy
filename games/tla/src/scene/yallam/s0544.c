#include "yallam.h"

void Func_02000544(void)
{
    Func_02002488();
    Func_02002590(0);
    Func_02002520(MsgDoYouWant);
    UiText_OpenMessageAtObject(8, 0);
    if (Inventory_PromptAndSetObjectMode(0, 0) == 0)
        Func_02001944();
    else
        Func_02002538(8, 0);
    Func_02002490();
}

void Func_02000584(void)
{
    u8 *event = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object = Object_GetById(9);
    s16 angle = object->angle;

    Func_02002488();
    Func_02002590(0);
    BYTE_AT(object, 99) = 1;
    if (*(s16 *)(event + 358) == 0) {
        ObjectMotion_SetAngleToward(9, gPartyState.current_owner, 0);
        Func_02002520(MsgRunningAroundIn);
    } else {
        Func_02002520(MsgThereSALot);
    }
    Func_02002538(9, 0);
    if (*(s16 *)(event + 358) == 0)
        object->angle = angle;
    BYTE_AT(object, 99) = 0;
    Func_02002490();
}

void Func_02000614(void)
{
    u8 *event = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object = Object_GetById(10);
    s16 angle = object->angle;

    Func_02002488();
    Func_02002590(0);
    BYTE_AT(object, 99) = 1;
    if (*(s16 *)(event + 358) == 0) {
        ObjectMotion_SetAngleToward(10, gPartyState.current_owner, 0);
        Func_02002520(MsgThereSAGame);
    } else {
        Func_02002520(MsgIOnceHeard);
    }
    Func_02002538(10, 0);
    if (*(s16 *)(event + 358) == 0)
        object->angle = angle;
    BYTE_AT(object, 99) = 0;
    Func_02002490();
}

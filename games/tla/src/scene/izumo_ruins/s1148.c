#include "izumo_ruins.h"

void Func_02001148(void)
{
    struct IzumoRuinsEventWork *work = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object;

    Func_02001488();
    Func_020014e8(0);
    Func_02001500(Data_02001696);
    object = work->object_cbc;
    object->priority_flags |= 1;
    object->priority_flags &= ~2;
    Func_02001490();
}

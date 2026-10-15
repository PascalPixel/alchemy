#include "izumo_ruins.h"

void Func_02001148(void)
{
    struct IzumoRuinsEventWork *work = Ram_HeapSlots->eventWork;
    struct ObjectRuntime *object;

    Func_02001488();
    Func_020014e8(0);
    Func_02001500(gData_02001696);
    object = work->objectCbc;
    object->priorityFlags |= 1;
    object->priorityFlags &= ~2;
    Func_02001490();
}

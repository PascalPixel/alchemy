#include "local.h"

void Func_02001ba0(s32 id)
{
    u8                   *work = Ram_HeapSlots->unk70[1];
    struct ObjectRuntime *object = Object_GetById(id);
    s32                  *slot = (s32 *)(work + 0x808);

    *slot = 0;
    if (id != 0 && object != 0)
        *slot = (s32)&object->x;
}

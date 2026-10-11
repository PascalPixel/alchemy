#include "ov697.h"

void Func_020008a4(void)
{
    if (GameFlag_Test(Func_020059b8(gPartyState.current_owner) + 0x203))
        Func_020059c0();
}

void Func_020008d0(s32 a, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    Func_0200070c(id, 16);
    GameFlag_ClearBit(id + 0x203);
    if (object != NULL)
        object->priority_flags &= 0xfd;
}

void Func_02000904(s32 a, s32 id)
{
    OBJECT_BYTE(Object_GetById(id - 5), 98) = 0;
    GameFlag_SetBit(id + 0x313);
}

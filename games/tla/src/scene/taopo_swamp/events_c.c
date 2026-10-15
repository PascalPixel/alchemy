#include "local.h"

void Func_02000428(s32 a)
{
    ObjectMotion_SetActionVariant_(gPartyState.current_owner, 3);
    Func_020028e0_(gPartyState.current_owner, 6, 0x1000);
    Func_020029b0_(0x136);
    GameFlag_SetBit_(0x122);
    Func_02002930(a);
}

void Func_02000464(s32 a, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (id == 9) {
        if (object->x >> 20 == 17) {
            object->z += 0x10000;
            Func_02002764_(2, 0x80000);
            GameFlag_SetBit_(0xa6d);
        }
    } else if (id == 10) {
        if (object->x >> 20 == 29) {
            object->z += 0x10000;
            Func_02002764_(8, 0x80000);
            GameFlag_SetBit_(0xa6e);
        }
    }
}

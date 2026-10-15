#include "anemos_inner_sanctum_entrance.h"

extern const s8 Data_02000ee0[];

void Func_020004e4(s32 a, s32 b)
{
    s32 i;

    const s8 *x = Data_02000ee0;
    const s8 *z = Data_02000ee0 + 1;

    for (i = 9; i <= 14; i++) {
        struct ObjectRuntime *object = Object_GetById(i);

        if (object->x >> 20 != *x)
            return;
        {
            s32 cell_z = *z;

            z += 2;
            x += 2;
            if (object->z >> 20 != cell_z)
                return;
        }
    }
    GameFlag_SetBit(0x200);
    Func_02000d64(201);
    Func_02000be0(1);
    Func_02000494();
}

void Func_02000534(s32 a)
{
    struct PartyState *party;
    s32 held = Func_02000d4c();

    party = &gPartyState;
    if (held) {
        if (Func_02000d0c(party->current_owner) != -1) {
            Func_02000d54();
            Func_020004e4(0, a);
        }
    } else {
        if (Object_GetById(party->current_owner)->y > -0x1c0000)
            Func_02000d14();
    }
}

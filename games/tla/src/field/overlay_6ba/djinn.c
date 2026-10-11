#include "types.h"

void GameFlag_SetBit(s32 flag);
void Djinn_AddToOwner(s32 owner, s32 element, s32 djinn);
void Djinn_Activate(s32 owner, s32 element, s32 djinn);

void Func_020005fc(s32 owner, s32 element, s32 djinn, s32 activate)
{
    GameFlag_SetBit(element * 20 + djinn + 48);
    Djinn_AddToOwner(owner, element, djinn);
    if (activate == 1)
        Djinn_Activate(owner, element, djinn);
}

#include "TYPES.H"
#include "SCENE.H"
s32 Djinn_AddToOwnerFar(s32, s32, s32);

s32 GameFlag_SetBitFar(s32);

void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index)
{
    GameFlag_SetBitFar((element * 0x14) + index + 0x30);
    Djinn_AddToOwnerFar(owner, element, index);
}

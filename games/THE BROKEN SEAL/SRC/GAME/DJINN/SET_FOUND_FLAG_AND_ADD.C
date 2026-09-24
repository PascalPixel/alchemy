#include "TYPES.H"
#include "SCENE.H"

s32 GameFlag_SetBitFar(s32);

void Djinn_SetFoundFlagAndAdd(s32 arg0, s32 arg1, s32 arg2)
{
    GameFlag_SetBitFar((arg1 * 0x14) + arg2 + 0x30);
    Djinn_AddToOwnerFar(arg0, arg1, arg2);
}

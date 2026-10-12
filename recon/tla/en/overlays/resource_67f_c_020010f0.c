/* Near miss, differ 2 of 32 (and a pool offset): the listing loads the party state address
 * before it copies the second argument to r4; this draft copies first. A barrier on the address moves
 * the copy of the first argument instead. */
#include "../../../../games/tla/src/field/overlay_67f/ov67f.h"

void Func_020010f0(s32 a, s32 b)
{
    struct Ov67fEffectWork *work = Ram_HeapSlots->effect_work;

    if (gPartyState.scene == (s32)&SceneId_Ov67FRoom1)
        Func_020000a4(Data_02001674, a, b);
    else
        Func_020000a4(Data_02001688, a, b);
    if (a == 1)
        Func_02000f30(Object_GetById(work->object_id));
}

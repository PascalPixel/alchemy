/* Draft, differ 26 of 98, rewritten from the listing (the previous draft compared
 * the scene with message symbols and scored 19 only because pool words are not
 * compared). The calls match through call.h, the Japanese branch included
 * (differ 26 there too). Left: in both scene tests ⚓️ builds 0x1ac in r1, loads
 * gPartyState into r3 and indexes the halfword with a zero in r1; this form uses
 * r0 for the offset and the zero and r1 for the base, and the 11, 2 call after
 * the dispatch sets r1 before r0. Tried: a scene local, a work local, a one-pass
 * block after the store, the dispatch through Call2; permute 4 minutes (its best
 * moved a call out of its branch). */
#include "../../../../games/tla/src/field/overlay_677/ov677.h"
#include "flag_call.h"
#include "call.h"

void Func_02000a60(s32 id, s32 a);

s32 Func_0200086c(void)
{
    ((struct Ov677EventWork *)Ram_HeapSlots->event_work)->screen_effect = 0x209;
    if (gPartyState.scene == (s32)&SceneId_Ov677Room1) {
        if (Flag_IsSet(0x301)) {
            Call3((void (*)())Func_020009f8, 14, 0x2580000, 0x1680000);
            Call3((void (*)())Func_020009f8, 16, 0x2780000, 0x15a0000);
            Call3((void (*)())ObjectMotion_ArmCallback, 16, 0x4000, 0);
#if !defined(TLA_EDITION_JA)
            Call3((void (*)())Func_020009f8, 17, 0, 0);
#endif
        } else {
            Call2((void (*)())Func_02000a60, 14, 0);
        }
        ObjectDispatch_SetSingleChildFlags(Object_GetById(17), 0);
        Call2((void (*)())ObjectMotion_SetActionVariant, 11, 2);
    }
    if (gPartyState.scene == (s32)&SceneId_Ov677Room2) {
        Call1((void (*)())Func_02000a48, 0);
        Call2((void (*)())ObjectMotion_SetActionVariant, 12, 2);
        Object_GetById(12)->collision_flags |= 4;
        Call2((void (*)())ObjectMotion_SetActionVariant, 10, 2);
        Object_GetById(10)->collision_flags |= 4;
        Call2((void (*)())ObjectMotion_SetActionVariant, 11, 2);
        Object_GetById(11)->collision_flags |= 4;
    }
    return 0;
}

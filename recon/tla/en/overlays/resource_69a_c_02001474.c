/*
 * Draft of overlay 69a Func_02001474 (twins: 6a3 Func_020040b4, 6a5
 * Func_0200510c). 2 instructions differ: the listing copies the object to r10
 * after the two position reads (ldr r5; ldr r6; mov sl, r0), the draft before
 * them. Tried: the Value1 call layer, a second pointer assigned after the
 * reads (same 2), an empty asm barrier after the reads (6: the owner reload
 * mov r1, r8 then falls behind it).
 */
#include "types.h"
#include "party_state.h"
#include "field_event.h"

struct FieldActor *Object_GetById();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_SetPositionAndCommit();
void WaitFrames();
void Object_SetPositionAndResetMotion();
void Battle_WaitMode0();
struct FieldActor *Func_02001b7c();

void Func_02001474(void)
{
    struct FieldActor *found = Object_GetById(gPartyState.current_owner);
    struct FieldActor *object;
    struct FieldActor *other;
    s32 x = found->x;
    s32 z = found->z;

    object = found;
    asm volatile(""); /* FAKEMATCH: the barrier keeps the copy to r10 behind the two reads; without it the copy comes first (2 differ), with or without a second pointer */
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x40000, 0x20000);
    x >>= 20;
    z >>= 20;
    ObjectMotion_SetPositionAndCommit(gPartyState.current_owner, x * 16 + 8, z * 16 + 8);
    WaitFrames(1);
    Object_SetPositionAndResetMotion(object, (x << 20) + 0x80000, object->y, (z << 20) + 0x80000);
    Battle_WaitMode0(4);
    other = Func_02001b7c();
    other->y -= 0x80000;
}

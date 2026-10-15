/* Near miss, differ 2 of 65: the native loads the party state address between
   building 0 and storing it to the motion flags (movs r3, #0; ldr r5; strb r3);
   this draft stores first. A scene-layer call and a plain party->currentOwner
   leave the same order; the address taken at the top moves the load above the
   first call.
   Tried 2026-10-12: an empty asm barrier after the address puts ldr r5 above the
   flags address as well (differ 2, another row); the flags address taken before
   a barrier and stored through after the party address changes the prologue. */
#include "../../../../../games/tla/src/scene/yampi_desert/scene.h"

void Func_02002d6c(struct FieldActor *object, s32 x, s32 z, s32 mode)
{
    struct PartyState *party;

    Func_02004040();
    Func_020041b8(0);
    party = &gPartyState; /* FAKEMATCH: the party state address is taken before the flag store, where the native loads it; taken in the call below, the store comes first */
    object->motionFlags = 0;
    ObjectMotion_SetSpeedParameters(party->currentOwner, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset(party->currentOwner, x, z);
    Func_020040e8(party->currentOwner, 0xc000);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(party->currentOwner), 0);
    Object_SetModeById(party->currentOwner, 13);
    Func_02003fe0(object, object->x, object->y - 0xa0000, object->z);
    Func_02003fe8(object);
    Object_SetModeById(party->currentOwner, 10);
    Func_020041f8(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004138(mode);
}

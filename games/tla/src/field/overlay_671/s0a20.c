#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

void Func_020056b4(struct ObjectRuntime *object);
void Func_0200580c(s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_02005794(s32 id, s32 value);
void Battle_WaitMode0(s32 frames);
void WaitFrames(s32 frames);
void Func_02005854(s32 sound);
void Event_FinishScreenTransition(void);

static __inline__ void Actor_Cue(s32 id, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005794(id, value);
}

void Func_02000a20(struct ObjectRuntime *object)
{
    u32 i;
    s32 delta;
    struct PartyState *party;

    Func_020056b4(object);
    Func_0200580c(0);
    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->motion_flags = 0;
    object->y = 0x60000;
    object->z = 0xf741cf;
    object->angle = 0x4000;
    party = &gPartyState;
    /* FAKEMATCH: the native loads the party state's address among the stores above and builds the owner's offset after them; the plain order builds the offset before the angle store (differ 4). */
    asm volatile("" : "+l"(party));
    Actor_Cue(party->current_owner, 258);
    Object_SetModeById(party->current_owner, 27);
    Battle_WaitMode0(30);
    Object_SetModeById(party->current_owner, 28);
    Battle_WaitMode0(10);
    for (i = 0; i <= 19; i++) {
        /* FAKEMATCH: one variable set twice in the loop keeps both constants inside it, as the native loads them on every pass; as plain constants loop.c moves -0x300 above the loop (life 4, differ 20). */
        delta = -0x300;
        object->scale_x += delta;
        object->scale_y += delta;
        delta = -0x18000;
        object->y += delta;
        WaitFrames(1);
        if (i == 10)
            Func_02005854(204);
        if (i == 20) {
            *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 521;
            Event_FinishScreenTransition();
        }
    }
}

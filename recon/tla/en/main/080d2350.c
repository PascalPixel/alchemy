/* 2026-10-03 finite trial record (extent/differing bytes, relocations unlinked):
 * Four ordinary forms: baseline66/4, pointer-local66/4, early owner62/45, long-lived pointer66/17. Four initial scheduling forms did not match; three pointer forms: pinned66/4, released pin66/0, retained ordinary pointer handoff66/0. Counts include the empty hook, exclude final two alignment bytes.
 * No whole-edition linked proof is claimed by these object measurements. */
/* Event end trial: ordinary C through maintained PartyState.
 * Native function and pool end at 080d2390; the raw listing then carries
 * a distinct empty routine and its alignment. Full edition proof pending. */
#include "PARTY_STATE.H"
#include "CALLBACK_SCHEDULER.H"

void Func_080d21f4(void);
void Object_AttachWorkTargetToObject(s32 object, s32 enabled);
void Func_080ad2b8(void);

void Func_080d2350(void)
{
    Scheduler_RemoveCallback((u32)Func_080d21f4);
    { struct PartyState *party = &gPartyState;
    /* FAKEMATCH: four ordinary forms constructed the override offset before loading the party pointer; this used pointer handoff preserves the native 64-byte order. */
    __asm__ volatile("" : "+r"(party));
    if (party->owner_override == 0)
        Object_AttachWorkTargetToObject(party->current_owner, 1);
    else
        Object_AttachWorkTargetToObject(8, 1);
    Func_080ad2b8();
    }
}

void Event_EmptyHook(void)
{
}

/* Near misses. Func_02000688 (49 instructions, 2 differ): the native loads the pool address of
 * gPartyState before it stores the screen effect, the draft after; sched2 gives the load
 * priority 17 against the store value 18, and a block-local party pointer measured the same
 * (6B9 Func_02000104 has the same single difference). Func_02000704 (29, 22 differ): written with goto
 * (17 differ) the draft keeps the address of owners in r8 across the calls; the native takes it
 * from sp at each pass. As a for loop (22 differ) it steps a pointer and counts down. */
#include "types.h"
#include "field_scene.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "eventwrk.h"
#include "owner_state.h"

void Object_SetModeById(s32 id, s32 mode);
void UnitCalc(s32 owner);
void Func_02000db0(void);

s32 Func_02000688(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    work->screen_effect = 256;
    {
        struct PartyState *party = &gPartyState;

        party->event_mode = 2;
    }
    Object_SetModeById(16, 5);
    Object_SetModeById(17, 5);
    Object_SetModeById(18, 5);
    Object_SetModeById(19, 5);
    Object_SetModeById(30, 6);
    Object_SetModeById(31, 6);
    Object_SetModeById(32, 6);
    Object_SetModeById(33, 6);
    if (gPartyState.entrance == 99)
        Func_02000db0();
    return 0;
}

void Func_02000704(s32 levels)
{
    s16 owners[16];
    s32 count;
    s32 i;
    s32 owner;

    count = Party_ListActiveOwners(owners);
    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes index owners[i] from sp and count up, and a for loop steps a pointer and counts down (differ 22). */
    for (i = 0; i < count;) {
        owner = (u16)owners[i++];
        Party_AdvanceOwnerCountToTarget(owner, ((u8 *)UnitGet(owner))[15] + levels);
        UnitCalc(owner);
    }
}

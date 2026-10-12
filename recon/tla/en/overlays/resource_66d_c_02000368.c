/* Near miss, overlay 66d Func_02000368 (0x02008368): differ 20 of 60, three
 * places.
 * 1. The loop: the native builds the zero on every pass (movs r3, #0 after
 *    the flag's address in r2); here loop.c moves it out into r7 (the loop
 *    dump: 9 real insns, life 1, savings 1, so 15 >= 9). One variable for
 *    this zero and the later one keeps it inside but in r6 across the
 *    function (differ 28); a "+l" barrier on a local zero keeps it inside
 *    and in r3 but ahead of the address (one pair left there).
 * 2. The native subtracts 4 from the entrance after Func_020005f4 returns;
 *    here the subtraction is scheduled before the call. An empty barrier
 *    after the call fixes it but moves the call's argument ahead of the
 *    entrance's load.
 * 3. The native loads the table's address (ldr r0) before it scales the
 *    index; here after. The table as a temporary behind a "+l" barrier
 *    changes the addressing (adds r5, r5, r3 and three displaced loads).
 * The unsigned counter, the work pointer advanced in place and the call
 * through the inline layer are measured: each removed costs instructions. */
#include "../../../../games/tla/src/field/overlay_66d/local.h"

static __inline__ void Func_02000534_(const void *script, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000534(script, x, z);
}

void Func_02000368(void)
{
    u8 *work = Ram_HeapSlots->event_work;
    s32 index;
    u32 id;

    Func_0200054c();
    Func_020005c4(0);
    for (id = 8; id <= 63; id++) {
        struct ObjectRuntime *object = Object_GetById(id);

        if (object != NULL)
            object->motion_flags = 0;
    }
    work += 0x154;
    index = *(s16 *)work;
    Func_020005f4(158);
    index -= 4;
    Func_02000534_(Data_0200064c[index].script, Data_0200064c[index].x, Data_0200064c[index].z);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    Object_GetById(gPartyState.current_owner)->motion_flags = 0;
    Object_SetModeById(gPartyState.current_owner, 2);
    Func_020005a4(*(s16 *)work);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000554();
}

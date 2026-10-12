/* Draft: differ 37 (listing 74 instructions, draft 75). The object loop is a
 * goto loop now, which keeps its zero inside the pass as the native does
 * (a for loop hoists it: differ 53), and the two camera calls go through
 * forwarders. Left: the native keeps the stop index in r6 and the owner
 * address in r5, the draft the other way round; and it loads the stop's
 * first word last, through the table address in r0 (ldr r0, [r0, r4]). */
#include "../../../../games/tla/src/field/overlay_676/ov676.h"

static __inline__ void Offset(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline sets the arguments up in their own order; the direct call negates the offset last (differ 2). */
    ObjectMotion_OffsetPositionAndResetMotion(id, x, z);
}

static __inline__ void CenterOn(s32 id, s32 column, s32 offset)
{
    /* FAKEMATCH: forwarding through the inline sets the arguments up in their own order; the direct call negates the offset last (differ 2). */
    ObjectMotion_CenterOnColumn(id, column, offset);
}

void Func_02000270(void)
{
    u8 *event = Ram_HeapSlots->event_work;
    u32 i;
    s32 index;

    Func_02002488();
    Func_02002590(0);
    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes rebuild the zero on every pass, and a for loop hoists it. */
    i = 8;
loop:
    {
        struct ObjectRuntime *object = Object_GetById(i);

        if (object != NULL)
            object->motion_flags = 0;
    }
    i++;
    if (i <= 63)
        goto loop;
    index = *(s16 *)(event + 340);
    Func_020025a0(158);
    index--;
    Func_02002460(Data_02002ce4[index].a, Data_02002ce4[index].b, Data_02002ce4[index].c);
    Object_SetModeById(gPartyState.current_owner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0xcccc, 0x6666);
    if (index == 7)
        Offset(gPartyState.current_owner, 0, -4);
    else
        CenterOn(gPartyState.current_owner, 2, -4);
    Battle_WaitMode0(10);
    Func_02002578(*(s16 *)(event + 340));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02002490();
}

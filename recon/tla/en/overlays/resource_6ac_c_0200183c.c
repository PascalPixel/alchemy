/* Near miss, differ 2 of 53 (and a pool offset): the listing loads the event work before it
 * builds the offset 0x1ac; this draft builds the offset first. A barrier on the work pointer changes the registers. */
#include "../../../../games/tla/src/field/overlay_6ac/ov6ac.h"

void Func_0200183c(void)
{
    s32 *field = &((struct Ov6acEventWork *)Ram_HeapSlots->event_work)->field_1ac;
    s32 saved = *field;

    Call2((void (*)())Func_02003248, 0x202108, 1);
    Call2((void (*)())Func_02003248, 0x10000, 2);
    ((struct Ov6acEventWork *)Ram_HeapSlots->event_work)->field_1ac = 0x100;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_0200168c();
    Func_020032a0();
    WaitFrames(40);
    Func_0200166c();
    Call2((void (*)())Func_02003248, 0x10000, 1);
    Func_02003258(16);
    WaitFrames(16);
    gPartyState.unknown_2c2 = 0;
    *field = saved;
}

/* Draft: differ 53 (listing 74 instructions, draft 75). The native builds the
 * zero inside the object loop where the draft hoists it to r6, which also
 * moves the stop index from r6 to r5; and it takes the stop's first word last. */
#include "../../../../games/tla/src/field/overlay_676/ov676.h"

void Func_02000270(void)
{
    u8 *event = Ram_HeapSlots->event_work;
    u32 i;
    s32 index;

    Func_02002488();
    Func_02002590(0);
    for (i = 8; i <= 63; i++) {
        struct ObjectRuntime *object = Object_GetById(i);

        if (object != NULL)
            object->motion_flags = 0;
    }
    index = *(s16 *)(event + 340);
    Func_020025a0(158);
    index--;
    Func_02002460(Data_02002ce4[index].a, Data_02002ce4[index].b, Data_02002ce4[index].c);
    Object_SetModeById(gPartyState.current_owner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0xcccc, 0x6666);
    if (index == 7)
        ObjectMotion_OffsetPositionAndResetMotion(gPartyState.current_owner, 0, -4);
    else
        ObjectMotion_CenterOnColumn(gPartyState.current_owner, 2, -4);
    Battle_WaitMode0(10);
    Func_02002578(*(s16 *)(event + 340));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02002490();
}

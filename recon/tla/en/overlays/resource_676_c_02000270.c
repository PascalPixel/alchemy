#include "../../../../games/tla/src/field/overlay_676/ov676.h"

static __inline__ void Offset(s32 id, s32 x, s32 z)
{
    ObjectMotion_OffsetPositionAndResetMotion(id, x, z);
}

static __inline__ void CenterOn(s32 id, s32 column, s32 offset)
{
    ObjectMotion_CenterOnColumn(id, column, offset);
}

static __inline__ void Func_02002460_(s32 a, s32 b, s32 c)
{
    Func_02002460(a, b, c);
}

void Func_02000270(void)
{
    u8 *event = Ram_HeapSlots->event_work;
    u32 i;
    s32 index;

    Func_02002488();
    Func_02002590(0);
    for (i = 8; i <= 63; i++) {
        u8 *actor = (u8 *)Object_GetById(i);

        if (actor != NULL) {
            u8 *flags = &actor[85];
            register s32 zero asm("r3") = 0;

            *flags = zero;
        }
    }
    index = *(s16 *)(event + 340);
    asm("");
    Func_020025a0(158);
    asm("");
    index--;
    Func_02002460_(Data_02002ce4[index].a, Data_02002ce4[index].b, Data_02002ce4[index].c);
    asm("");
    Object_SetModeById(gPartyState.current_owner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0xcccc, 0x6666);
    {
        s32 owner = gPartyState.current_owner;

        if (index == 7)
            Offset(owner, 0, -4);
        else
            CenterOn(owner, 2, -4);
    }
    Battle_WaitMode0(10);
    Func_02002578(*(s16 *)(event + 340));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02002490();
}

/* Draft, differ 10 of 110: each of the first five list calls builds the id
 * before the value (movs r0, movs r1); the native builds the value first.
 * Direct calls, a forwarding inline, its parameters swapped and narrower
 * parameter types all leave the same five pairs. Func_02000714 beside it is
 * adopted in games/tla/src/field/overlay_6b1/events_c.c. */
#include "../../../../games/tla/src/field/overlay_6b1/ov6b1.h"

struct Ov6b1EventWork {
    u8 unknown_000[0x1ac];
    s32 route;
};

void Func_02000408(void);
void Func_02000b18(void);
void Func_02000ba4(s32 id, s32 value);
void Func_02000bd4(u16 value);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ s32 Flag_Test(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_Test(flag);
}

static __inline__ void Flag_Clear(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_ClearBit(flag);
}

s32 Func_02000740(void)
{
    ((struct Ov6b1EventWork *)Ram_HeapSlots->event_work)->route = 0x204;
    if (gPartyState.scene == (s32)&SceneId_Ov6b1Room1) {
        Func_02000b18();
        Func_02000bd4(2);
        Func_02000ba4(9, 0);
        Func_02000ba4(10, 1);
        Func_02000ba4(11, 2);
        Func_02000ba4(12, 3);
        Func_02000ba4(13, 128);
        Func_02000ba4(14, 129);
        Object_GetById(gPartyState.current_owner)->priority_flags |= 0x40;
        Object_GetById(9)->priority_flags |= 0xc0;
        Object_GetById(10)->priority_flags |= 0x80;
        Object_GetById(11)->priority_flags |= 0x80;
        Object_GetById(12)->priority_flags |= 0x80;
        Object_GetById(13)->priority_flags |= 0x80;
        Object_GetById(14)->priority_flags |= 0x80;
        if (Flag_Test(0x200)) {
            Func_02000be0(1);
            Func_02000494();
        }
    }
    if (Flag_Test(0x201)) {
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Func_02000408();
        Flag_Clear(0x201);
    }
    return 0;
}

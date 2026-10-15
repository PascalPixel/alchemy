/* Near miss, differ 4 of 69, from ☀️'s SceneScript_SetupActors (torebi_machi.c)
 * with its byte-pointer store. Devices so far: the zero pinned to r3 keeps
 * it inside the actor loop (loop.c hoists it into r6 otherwise, differ 24);
 * empty asms on each side of Func_020006f0 keep the decrement after the call
 * (sched2 ranks it 46 against the call's 45). What remains: the native loads
 * the table address before the shift (a "+l" asm on the table pointer
 * re-allocates the whole call, differ 15), and Object_SetModeById's mode
 * before the owner id (plain, inline and value-returning calls all load the
 * owner first: both tie at priority 2 and RTL order puts r0 first). */
#include "../../../../games/tla/src/field/overlay_65d/local.h"

struct WarpEntry {
    s32 map;
    u16 x;
    u16 z;
};

extern struct WarpEntry Data_0200073c[];

void Func_02000610(s32 map, s32 x, s32 z);
void Func_020006b8(s32 value);
void Func_020006f0(s32 value);
void ObjectMotion_CenterOnColumn(s32 id, s32 a, s32 b);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ void Func_02000610_(s32 map, s32 x, s32 z)
{
    Func_02000610(map, x, z);
}

static __inline__ void ObjectMotion_CenterOnColumn_(s32 id, s32 column, s32 offset)
{
    ObjectMotion_CenterOnColumn(id, column, offset);
}

void Func_0200009c(void)
{
    u8 *work = Ram_HeapSlots->event_work;
    u32 id;
    s16 *choice;
    s32 index;

    Func_02000648();
    Func_020006d0(0);
    for (id = 8; id <= 63; id++) {
        u8 *actor = (u8 *)Object_GetById(id);

        if (actor != NULL) {
            u8 *flags = &actor[85];
            register s32 zero asm("r3") = 0;

            *flags = zero;
        }
    }
    choice = (s16 *)(work + 340);
    index = *choice;
    asm("");
    Func_020006f0(158);
    asm("");
    index--;
    Func_02000610_(Data_0200073c[index].map, Data_0200073c[index].x, Data_0200073c[index].z);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    Object_GetById(gPartyState.current_owner)->motion_flags = 0;
    {
        s32 mode = 2;

        __asm__ ("" : "+l"(mode));
        Object_SetModeById(gPartyState.current_owner, mode);
    }
    if (index != 4) {
        ObjectMotion_CenterOnColumn_(gPartyState.current_owner, 2, -8);
        Battle_WaitMode0(10);
    }
    Func_020006b8(*choice);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000650();
}

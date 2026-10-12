/* Near miss, differ 22. What remains: the listing builds the zero inside the
 * actor loop (movs r3, #0 on every pass, work pointer in r6) where this draft
 * moves it out into r6 (a one-pass block around the store did not keep it
 * in); it decrements the choice after Func_020006f0 returns and loads the
 * table address before the shift; and Object_SetModeById loads its mode
 * before the owner id. One-pass blocks at the last two spots each move a
 * neighbouring instruction instead. */
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
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000610(map, x, z);
}

static __inline__ void ObjectMotion_CenterOnColumn_(s32 id, s32 column, s32 offset)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
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
        struct ObjectRuntime *object = Object_GetById(id);

        if (object != NULL)
            object->motion_flags = 0;
    }
    choice = (s16 *)(work + 340);
    index = *choice;
    Func_020006f0(158);
    index--;
    Func_02000610_(Data_0200073c[index].map, Data_0200073c[index].x, Data_0200073c[index].z);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    Object_GetById(gPartyState.current_owner)->motion_flags = 0;
    Object_SetModeById_(gPartyState.current_owner, 2);
    if (index != 4) {
        ObjectMotion_CenterOnColumn_(gPartyState.current_owner, 2, -8);
        Battle_WaitMode0(10);
    }
    Func_020006b8(*choice);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000650();
}

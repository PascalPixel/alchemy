/* Near miss, differ 2 of 69, from ☀️'s SceneScript_SetupActors (torebi_machi.c)
 * and overlay 66d's Func_02000368 (adopted, the same function with other
 * ids). Devices here: the zero pinned to r3 inside the actor loop (differ 24
 * without), empty asms on each side of Func_020006f0 (the decrement after
 * the call) and a "+l" asm on Object_SetModeById's mode (its mode before the
 * owner id; overlay 66d needs none). What remains: the native loads the
 * table address before the shift (ldr r0 then lsls r4); both tie at priority
 * 45 in sched2 and the shift goes first. Index here stays live for the later
 * compare, so the shift writes r4 rather than reusing r5 as in 66d. A "+l"
 * asm on a table pointer and the &table[index], table + index and
 * index[table] forms each re-address the call (differ 13 to 15). */
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

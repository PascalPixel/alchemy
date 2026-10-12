/* Draft, differ 12 of 63. Two places: the native reads the entry's two halfwords through base + (index * 8 + 4) and its word through base + index * 8, this form through three indexed loads; and for the (owner, 0, -4) call it clears r1 between the halves of -4, this form after them (657's Func_02000078 matches the same call with 2 for 0). */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"

struct Ov675Entry {
    s32 value;
    u16 position[2];
};

extern const struct Ov675Entry Data_020019ec[];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001254(void);
void Func_0200125c(void);
void Func_02001324(s32 a);
void Func_02001344(s32 a);
void Func_0200122c(s32 value, s32 x, s32 z);
void Func_020012cc(s32 a);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
s32 ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void Battle_WaitMode0(s32 frames);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_020005fc(void)
{
    u8 *work = Ram_HeapSlots->event_work;
    u32 i;
    u32 index;

    Func_02001254();
    Func_02001324(0);
    /* FAKEMATCH: the loop is written with goto so that it carries no loop notes: the bytes rebuild the zero on every pass, and a for loop hoists it. */
    i = 8;
loop:
    {
        struct ObjectRuntime *object = Object_GetById(i);

        if (object != 0)
            object->motion_flags = 0;
    }
    i++;
    if (i <= 63)
        goto loop;
    index = *(s16 *)(work + 0x154) - 1;
    Func_02001344(158);
    index >>= 1;
    Func_0200122c(Data_020019ec[index].value, Data_020019ec[index].position[0], Data_020019ec[index].position[1]);
    Object_SetModeById(gPartyState.current_owner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    ObjectMotion_OffsetPositionAndResetMotion(gPartyState.current_owner, 0, -4);
    Battle_WaitMode0(6);
    Func_020012cc(*(s16 *)(work + 0x154));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_0200125c();
}

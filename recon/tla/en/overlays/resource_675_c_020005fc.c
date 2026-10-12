/* Draft, differ 10 of 63. Two places: the native keeps the table in r0, reads the first halfword as table[index * 8 + 4], adds the table to that offset for the second and reads the word last as table[index * 8]; this form, an entry pointer and a pointer to its pair, adds the table to the entry first (three indexed loads from the struct: differ 12; the byte offsets written out: differ 12 to 14); and for the (owner, 0, -4) call it clears r1 between the halves of -4, this form after them (657's Func_02000078 matches the same call with 2 for 0). */
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
    {
        const struct Ov675Entry *entry = &Data_020019ec[index];
        const u16 *position = entry->position;

        Func_0200122c(entry->value, position[0], position[1]);
    }
    Object_SetModeById(gPartyState.current_owner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    ObjectMotion_OffsetPositionAndResetMotion(gPartyState.current_owner, 0, -4);
    Battle_WaitMode0(6);
    Func_020012cc(*(s16 *)(work + 0x154));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_0200125c();
}

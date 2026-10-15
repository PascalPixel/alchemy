/* Draft, differ 16 of 61, after overlay 695's Func_02000d84 (goto loop, so loop.c keeps the
 * zero inside). Left: the native addresses the placement table as base + offset registers
 * (data at [table, i * 8], the two cells at [table, i * 8 + 4] and 2 past it), the draft as
 * table + 4 and table + 6 or one entry pointer; the leader's mode and column calls also
 * order their loads differently. Tried: an entry struct with x and y or a cell[2], a local
 * table pointer, byte offsets, (index - 1) * 8 in one expression (folds the subtraction),
 * 3 minutes of alchemy permute (differ 14 with an inline index helper). */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct MadraEventWork {
    u8 unknown_000[0x154];
    s16 value_154;
};

struct MadraPlacement {
    const void *data;
    u16 x;
    u16 y;
};

extern s32 Data_02003308[];

struct ObjectRuntime *Object_GetById(s32 id);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002ae8(const void *data, s32 a, s32 b);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002c30(s32 a);
void Func_02002c58(s32 a);
void Func_02002c90(s32 a);

void Func_0200006c(void)
{
    struct MadraEventWork *work = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object;
    u32 id;

    Func_02002b28();
    Func_02002c58(0);
    id = 8;
next:
    object = Object_GetById(id);
    if (object)
        object->motion_flags = 0;
    id++;
    if (id < 64)
        goto next;
    {
        s32 index = work->value_154;

        Func_02002c90(158);
        index--;
        Func_02002ae8((const void *)Data_02003308[index * 2], ((u16 *)&Data_02003308[index * 2 + 1])[0], ((u16 *)&Data_02003308[index * 2 + 1])[1]);
    }
    Object_GetById(gPartyState.current_owner)->motion_flags = 0;
    Object_SetModeById(gPartyState.current_owner, 2);
    ObjectMotion_CenterOnColumn(gPartyState.current_owner, 2, -8);
    Battle_WaitMode0(10);
    Func_02002c30(work->value_154);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02002b30();
}

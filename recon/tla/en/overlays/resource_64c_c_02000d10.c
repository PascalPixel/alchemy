/* Draft, differ 2 of 33 (twin of Func_02000cbc, on z): the native loads gPartyState's address before it builds
 * the current_owner offset (133 << 2); this form builds the offset first. Tried:
 * an owner temporary, a leader inline, assignments apart, the work pointer
 * inline; a minute of permuter found nothing closer. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork158 {
    u8 unknown_000[0x158];
    s16 next_event;
};

extern struct ObjectRuntime *Data_02003dc0[];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020020b4(s32 a);

void Func_02000d10(s32 id, s32 before, s32 after)
{
    struct ObjectRuntime *target = Data_02003dc0[id - 100];
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    struct EventWork158 *work = Ram_HeapSlots->event_work;

    if (leader->z < target->z)
        work->next_event = before;
    else
        work->next_event = after;
    Func_020020b4(123);
}

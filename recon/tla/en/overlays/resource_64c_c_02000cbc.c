/* Draft, differ 2 of 33: the native loads gPartyState's address before it builds
 * the currentOwner offset (133 << 2); this form builds the offset first. Tried:
 * an owner temporary, a leader inline, assignments apart, the work pointer
 * inline; a minute of permuter found nothing closer. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork158 {
    u8 unk000[0x158];
    s16 nextEvent;
};

extern struct ObjectRuntime *gData_02003dc0[];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020020b4(s32 a);

void Func_02000cbc(s32 id, s32 before, s32 after)
{
    struct ObjectRuntime *target = gData_02003dc0[id - 100];
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    struct EventWork158 *work = Ram_HeapSlots->eventWork;

    if (leader->x < target->x)
        work->nextEvent = before;
    else
        work->nextEvent = after;
    Func_020020b4(123);
}

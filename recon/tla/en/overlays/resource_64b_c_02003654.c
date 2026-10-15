/* Draft, differ 2 of 33: the listing loads gPartyState's address before building the 0x214 offset, as nearly every ⚓️ read of currentOwner does; matched code gets that with a tagged one-pass block, which here moves the copy of near instead (4). Tried: the event work inline (25) or separate, the table offset first (10), the block before the table load (6) or before the test (2). */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventChoice {
    u8 unk000[0x158];
    s16 choice;
};

extern struct ObjectRuntime *gData_02006c08[];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004e30(s32 sound);

void Func_02003654(s32 index, s32 near, s32 far)
{
    struct ObjectRuntime *marker = gData_02006c08[index - 100];
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    struct EventChoice *work = Ram_HeapSlots->eventWork;

    if (leader->z < marker->z)
        work->choice = near;
    else
        work->choice = far;
    Func_02004e30(123);
}

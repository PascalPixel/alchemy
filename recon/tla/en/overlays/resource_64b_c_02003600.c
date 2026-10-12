/* Near misses of Func_02003600 and Func_02003654 of the world map (33 instructions each; the
 * second compares z): as below every instruction is the native's but two rows are swapped,
 * `movs r2, #133` before the pool load of gPartyState where the native loads the address
 * first (alchemy diff prints only a pool offset; alchemy show after a build shows the order).
 * A one-pass block before the owner lookup, and a party pointer behind a "+l" barrier, both
 * give the native pair but move the copy of the second parameter (adds r7, r1) five rows up. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "eventwrk.h"

extern struct ObjectRuntime *Data_02006c08[];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004e30(s32 value);

void Func_02003600(s32 slot, s32 before, s32 after)
{
    struct ObjectRuntime *target = Data_02006c08[slot - 100];
    struct ObjectRuntime *owner = Object_GetById(gPartyState.current_owner);
    struct EventWork *work = Ram_HeapSlots->event_work;

    if (owner->x < target->x)
        work->countdown = before;
    else
        work->countdown = after;
    Func_02004e30(123);
}

void Func_02003654(s32 slot, s32 before, s32 after)
{
    struct ObjectRuntime *target = Data_02006c08[slot - 100];
    struct ObjectRuntime *owner = Object_GetById(gPartyState.current_owner);
    struct EventWork *work = Ram_HeapSlots->event_work;

    if (owner->z < target->z)
        work->countdown = before;
    else
        work->countdown = after;
    Func_02004e30(123);
}

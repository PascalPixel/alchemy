#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);

s32 Func_02000590(void)
{
    u8 *event = Ram_HeapSlots->event_work;

    *(s32 *)(event + 428) = 516;
    *(s32 *)(event + 436) = 24;
    if (gPartyState.entrance <= 89) {
        Object_GetById(11)->scale_y = 0x1b333;
        Object_GetById(11)->scale_x = 0x1b333;
    }
    return 0;
}

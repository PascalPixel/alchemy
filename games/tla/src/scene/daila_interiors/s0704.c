#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

extern const u8 Data_02000e04[];

void Func_02000dcc(const u8 *script);

void Func_02000704(void)
{
    u8 *event = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object;
    s32 flags;

    Func_02000dcc(Data_02000e04);
    object = *(struct ObjectRuntime **)(event + 0xcbc);
    flags = 1;
    flags |= object->priority_flags;
    flags &= 0xfd;
    object->priority_flags = flags;
}

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

extern const u8 gData_02000e04[];

void Func_02000dcc(const u8 *script);

void Func_02000704(void)
{
    u8 *event = Ram_HeapSlots->eventWork;
    struct ObjectRuntime *object;
    s32 flags;

    Func_02000dcc(gData_02000e04);
    object = *(struct ObjectRuntime **)(event + 0xcbc);
    flags = 1;
    flags |= object->priorityFlags;
    flags &= 0xfd;
    object->priorityFlags = flags;
}

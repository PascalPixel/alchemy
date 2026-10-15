#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "event_work.h"

struct ObjectRuntime *Object_GetById(s32 id);

s32 Func_02000064(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    work->screen_effect = 513;
    ObjectMotion_SetActionVariant(8, 2);
    Object_GetById(8)->collision_flags |= 4;
    Object_GetById(8)->collision_flags |= 16;
    return 0;
}

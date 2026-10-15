#include "battle.h"
#include "field.h"
#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "map.h"
#include "object_runtime.h"
#include "mem.h"
#include "iwram_call.h"
#include "callback_scheduler.h"

void WaitFrames(s32);
void Motion_CamBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter);
s32 __divsi3(s32, s32);
extern u32 Data_03001144;

void Func_080d450c(s32 id, s32 use_setter)
{
    struct ObjectRuntime *obj = ObjectTable_Get(id);

    if (obj != NULL)
        Motion_CamBounds(obj->x, -1, obj->z, use_setter);
}

void Func_080d452c(s32 id, s32 use_setter)
{
    struct ObjectRuntime *obj = ObjectTable_Get(id);

    if (obj != NULL)
        Motion_CamBounds(obj->x, obj->y, obj->z, use_setter);
}

void Func_080d4548(void)
{
    Object_CommitPosition(((struct EventWork *)Runtime_AllocateBlock(108, 0xd50))->view_center);
    EventRuntime_Wait(2);
}

struct ObjectRuntime *Func_080d4568(void)
{
    return ((struct EventWork *)Runtime_AllocateBlock(108, 0xd50))->view_center;
}

void Func_080d4580(struct ObjectRuntime *target, s32 keep_current_position)
{
    struct ObjectRuntime *object;

    object = ((struct EventWork *)Runtime_AllocateBlock(108, 0xd50))->view_center;
    if (target != NULL) {
        ObjectDispatch_InitFromTable4WithArgumentFar(object, NULL);
        object->linked_object = target;
        if (keep_current_position == 0) {
            object->x = target->x;
            object->y = target->y;
            object->z = target->z;
        }
    }
}

void Func_080d45b8(s32 min_x, s32 min_z, s32 max_x, s32 max_z)
{
    struct MapWork *map = Ram_HeapSlots->map_work;

    map->min_x = min_x;
    map->min_z = min_z;
    map->max_x = max_x;
    map->max_z = max_z;
}

void Func_080d45d8(void)
{
    struct MapWork *map = Ram_HeapSlots->map_work;
    s16 *duration;
    s32 *from;
    s16 *step;
    s32 offset;
    s32 delta;

    if (((struct EventWork *)Runtime_AllocateBlock(108, 0xd50))->view_center->movement_state != 0)
        return;
    duration = &map->zoom_frames;
    if (*duration == 0)
        return;
    from = &map->zoom_from;
    delta = map->zoom_to - *from;
    step = &map->zoom_step;
    (*step)++;
    offset = *from + __divsi3(delta * *step, *duration);
    map->zoom = Iwram_MulQ16(map->far_plane, offset);
    Data_03001144 = *(u16 *)((u8 *)map + 0x11c) + 1;
    if (*step == *duration) {
        *duration = 0;
        Scheduler_RemoveCallback(Func_080d45d8);
    }
}

void Func_080d46a4(s32 zoom, s32 frames)
{
    struct MapWork *map = Ram_HeapSlots->map_work;
    s32 result;

    if (((struct EventWork *)Runtime_AllocateBlock(108, 0xd50))->control_mode != 3)
        return;
    {
        s32 (*ratio)(s32, s32) = Iwram_RatioMulQ14;
        result = ratio(zoom, 0x10000);
    }
    map->zoom_from = map->zoom_to;
    map->zoom_to = result;
    map->zoom_frames = frames;
    map->zoom_step = 0;
    Scheduler_AddOrUpdateCallback(Func_080d45d8, 0x494);
}

void Func_080d4714(void)
{
    struct MapWork *map = Ram_HeapSlots->map_work;
    s32 frames;

    if (((struct EventWork *)Runtime_AllocateBlock(108, 0xd50))->control_mode == 3) {
        frames = 0;
        if (map->zoom_frames != 0) {
            do {
                WaitFrames(1);
                frames++;
            } while (frames <= 0x12b && map->zoom_frames != 0);
        }
    }
}

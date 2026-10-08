/* differ 8: the native spills the event block and stores the column bounds before the next constant's shift; only ARM9TDMI store scheduling differs */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "map.h"
#include "object_runtime.h"

void WaitFrames(s32);

void Motion_CamBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter)
{
    s32 should_use_setter;
    struct EventWork *runtime_block;
    s32 minimum_x;
    s32 minimum_z;
    s32 maximum_x;
    s32 object_z_offset;
    s32 maximum_z;
    s32 position_x;
    s32 position_z;
    s32 position_y;
    struct MapWork *map;
    struct ObjectRuntime *object;

    position_x = requested_x;
    position_y = requested_y;
    should_use_setter = use_setter;
    position_z = requested_z;
    runtime_block = Runtime_AllocateBlock(108, 0xd50);
    object = runtime_block->view_center;
    map = Ram_HeapSlots->map_work;
    minimum_x = map->min_x + 0x780000;
    object_z_offset = object->y;
    minimum_z = map->min_z + object_z_offset + 0x600000;
    maximum_x = map->max_x + 0xFF880000;
    maximum_z = map->max_z + object_z_offset + 0xFFC00000;
    map->origin = &object->x;
    Object_ResetMotion(object);
    if (position_x == -1) {
        position_x = object->x;
    }
    if (position_y == -1) {
        position_y = object->y;
    }
    if (position_z == -1) {
        position_z = object->z;
    }
    if (position_x < minimum_x) {
        position_x = minimum_x;
    }
    if (position_z < minimum_z) {
        position_z = minimum_z;
    }
    if (position_x > maximum_x) {
        position_x = maximum_x;
    }
    if (position_z > maximum_z) {
        position_z = maximum_z;
    }
    if (should_use_setter == 0) {
        object->x = position_x;
        object->y = position_y;
        object->z = position_z;
        WaitFrames(1);
        if (runtime_block->control_mode != 3) {
            Map_ApplyWorkOriginAndSpanFar();
        }
    } else {
        Object_SetPosition(object, position_x, position_y, position_z);
    }
}

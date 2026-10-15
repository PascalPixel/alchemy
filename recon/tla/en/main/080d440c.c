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
#include "event_work.h"
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
    s32 positionX;
    s32 positionZ;
    s32 positionY;
    struct MapWork *map;
    struct ObjectRuntime *object;

    positionX = requested_x;
    positionY = requested_y;
    should_use_setter = use_setter;
    positionZ = requested_z;
    runtime_block = Runtime_AllocateBlock(108, 0xd50);
    object = runtime_block->viewCenter;
    map = Ram_HeapSlots->mapWork;
    minimum_x = map->minX + 0x780000;
    object_z_offset = object->y;
    minimum_z = map->minZ + object_z_offset + 0x600000;
    maximum_x = map->maxX + 0xFF880000;
    maximum_z = map->maxZ + object_z_offset + 0xFFC00000;
    map->origin = &object->x;
    Object_ResetMotion(object);
    if (positionX == -1) {
        positionX = object->x;
    }
    if (positionY == -1) {
        positionY = object->y;
    }
    if (positionZ == -1) {
        positionZ = object->z;
    }
    if (positionX < minimum_x) {
        positionX = minimum_x;
    }
    if (positionZ < minimum_z) {
        positionZ = minimum_z;
    }
    if (positionX > maximum_x) {
        positionX = maximum_x;
    }
    if (positionZ > maximum_z) {
        positionZ = maximum_z;
    }
    if (should_use_setter == 0) {
        object->x = positionX;
        object->y = positionY;
        object->z = positionZ;
        WaitFrames(1);
        if (runtime_block->controlMode != 3) {
            Map_ApplyWorkOriginAndSpanFar();
        }
    } else {
        Object_SetPosition(object, positionX, positionY, positionZ);
    }
}

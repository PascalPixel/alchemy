#include "types.h"
#include "map.h"
#include "metadata_lookup.h"
#include "object_runtime.h"
#include "ram_buffer.h"

// Writes the object's position on screen, in pixels, to position[0] and
// position[1]; an animated object is raised by its frame box. Returns -1 when
// the object does not exist.
s32 Func_080d5bec(s32 objectId, s32 *position)
{
    struct ObjectRuntime *object = ObjectTable_Get(objectId);
    s32 *camera;
    s32 camera_x;
    s32 camera_z;
    s32 x;
    s32 z;

    if (object == 0)
        return -1;
    /* FAKEMATCH: the view position read as two adjacent words through one pointer, as in The Broken Seal, so both loads share one base; read as map->viewX and map->viewZ it differs 26 (64 instructions for 62). */
    camera = &Ram_HeapSlots->mapWork->viewX;
    camera_x = camera[0] & 0xffff0000;
    camera_z = camera[1] & 0xffff0000;
    x = object->x - camera_x;
    z = object->z - camera_z - object->y;
    *position++ = x / 0x10000;
    *position = z / 0x10000;
    if ((object->animationKind & 15) == 1)
        *position -= (s8)Resource_GetMetadataRecordFarFar(
            *((struct BattleAnimationState *)object->animation)->value28)->boxX;
    return 0;
}

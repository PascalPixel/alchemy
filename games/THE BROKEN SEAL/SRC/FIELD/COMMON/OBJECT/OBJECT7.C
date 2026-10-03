#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "MAP_SCROLL.H"
#include "ANIMSPR.H"
#include "METADATA_LOOKUP.H"


/* Writes the object's position relative to the camera in whole units, x
   then z less height; sprite-mode objects are raised by their metadata
   offset. Returns -1 when there is no such object. */
s32 Object_GetScreenPosition(s32 object_id, s32 *position)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    s32 *camera;
    s32 camera_x;
    s32 camera_z;
    s32 x;
    s32 z;

    if (object == 0)
        return -1;
    /* FAKEMATCH: separate named coordinate bases grow 136 to 140 bytes;
       retain the existing adjacent coordinate-word read through the real owner. */
    camera = &((struct MapScrollWork *)gMapWork[0])->view_x;
    camera_x = camera[0] & 0xffff0000;
    camera_z = camera[1] & 0xffff0000;
    x = object->x - camera_x;
    z = object->z - camera_z - object->y;
    *position++ = x / 0x10000;
    *position = z / 0x10000;
    if ((object->animation_kind & 15) == 1)
        *position -= (s8)Resource_GetMetadataRecordFar(((struct AnimationObject *)object->animation)->entries[0]->anim_id)->box_x;
    return 0;
}

void ObjectEffect_ReservedNoOp941DC(void)
{
}

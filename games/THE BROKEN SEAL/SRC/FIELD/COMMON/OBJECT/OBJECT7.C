#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "SCENE.H"
#include "OBJECT_LOOKUP.H"


struct ScreenObject {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[0x50 - 0x14];
    u8 **sprite;
    u8 flags;
};

s8 *Resource_GetMetadataRecordFar(s16 resource_id);

/* Writes the object's position relative to the camera in whole units, x
   then z less height; sprite-mode objects are raised by their metadata
   offset. Returns -1 when there is no such object. */
s32 Object_GetScreenPosition(s32 object_id, s32 *position)
{
    struct ScreenObject *object = (struct ScreenObject *)ObjectTable_Get(object_id);
    s32 *camera;
    s32 camera_x;
    s32 camera_z;
    s32 x;
    s32 z;

    if (object == 0)
        return -1;
    camera = (s32 *)(((u8 *)gMapWork[0]) + 228);
    camera_x = camera[0] & 0xffff0000;
    camera_z = camera[1] & 0xffff0000;
    x = object->x - camera_x;
    z = object->z - camera_z - object->y;
    *position++ = x / 0x10000;
    *position = z / 0x10000;
    if ((object->flags & 15) == 1)
        *position -= Resource_GetMetadataRecordFar(*(s16 *)object->sprite[10])[8];
    return 0;
}

/* object/effects/ObjectEffect_ReservedNoOp.c */
void ObjectEffect_ReservedNoOp941DC(void)
{
}

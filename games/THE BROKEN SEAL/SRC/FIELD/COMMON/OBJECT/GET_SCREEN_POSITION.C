#include "TYPES.H"

extern u8 *Data_03001e70;

struct ScreenObject {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[0x50 - 0x14];
    u8 **sprite;
    u8 flags;
};

struct ScreenObject *ObjectTable_Get(s32 object_id);
s8 *Resource_GetMetadataRecordFar(s16 resource_id);

/* Writes the object's position relative to the camera in whole units, x
   then z less height; sprite-mode objects are raised by their metadata
   offset. Returns -1 when there is no such object. */
s32 Object_GetScreenPosition(s32 object_id, s32 *position)
{
    struct ScreenObject *object = ObjectTable_Get(object_id);
    s32 *camera;
    s32 camera_x;
    s32 camera_z;
    s32 x;
    s32 z;

    if (object == 0)
        return -1;
    camera = (s32 *)(Data_03001e70 + 228);
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

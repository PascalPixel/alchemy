#include "PROTO.H"
#include "TYPES.H"
#include "METADATA_LOOKUP.H"
#include "SCENE.H"
#include "OBJECT_RUNTIME.H"


s32 BattleFx_CopyLinkedObjectPosition(struct ObjectRuntime *object)
{
    struct ObjectRuntime *link;

    link = object->linked_object;
    if (link != NULL) {
        object->motion_flags = 0;
        object->x = link->x;
        object->y = link->y
            + ((s8)Resource_GetMetadataRecordFarFar(*BattleAction_FindDescriptor(object->unknown_66))->box_x << 16)
            + 0x80000;
        object->terrain_height = link->terrain_height;
        object->z = link->z;
    }
    return 0;
}

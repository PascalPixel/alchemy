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
#include "metadata_lookup.h"
#include "scene.h"
#include "object_runtime.h"


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

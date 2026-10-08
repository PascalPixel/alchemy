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
#include "object_runtime.h"
#include "system.h"
#include "script_object_runtime.h"

struct DispatchObject;

void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *, u32);

void ObjectEvent_SetMode5WithCue(u32 object_id)
{
    Object_SetMode(ObjectTable_Get(object_id), 5);
    Audio_PlayCue(125);
    WaitFrames(12);
}

void ObjectEvent_Spring(u32 object_id)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    WaitFrames(18);
    Object_SetMode(object, 7);
    Audio_PlayCue(146);
    if (object != 0) {
        object->velocity_y = 0x80000;
        ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 1);
    }
}

void ObjectEvent_SetMode4WithCue(u32 object_id)
{
    Object_SetMode(ObjectTable_Get(object_id), 4);
    Audio_PlayCue(124);
    WaitFrames(12);
}

void ObjectEvent_SetMode2(u32 object_id)
{
    Object_SetMode(ObjectTable_Get(object_id), 2);
}

void ObjectEvent_SetMode6WithCue(u32 object_id)
{
    Object_SetMode(ObjectTable_Get(object_id), 6);
    Audio_PlayCue(124);
    WaitFrames(12);
}

struct ObjectRuntime *ObjectEvent_GetObject(u32 object_id)
{
    return ObjectTable_Get(object_id);
}

void ObjectEvent_Remove(u32 object_id)
{
    Func_080caa2c(object_id);
}

void ObjectEvent_PlaceOnGround(u32 object_id, s32 x, s32 z)
{
    struct ScriptObjectRuntime *object;

    object = (struct ScriptObjectRuntime *)ObjectTable_Get(object_id);
    if (object == 0)
        return;
    if (x == -1)
        x = object->home_x << 16;
    if (z == -1)
        z = object->home_z << 16;
    object->x = x;
    object->z = z;
#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    object->y = object->terrain_height = Func_08020190(0, x, z);
#else
    object->y = object->terrain_height = Map_GetTerrainHeightFar(0, x, z);
#endif
}

#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "SYSTEM.H"
#include "SCRIPT_OBJECT_RUNTIME.H"

/* Object commands the event scripts reach through the far-call table: a
   pose with its cue, the object itself, its removal and its placement. */

struct DispatchObject;

s32 Map_GetTerrainHeightFar(u8, s32, s32);
s32 Func_08020190(s32, s32, s32);
void ObjectDispatch_SetSingleChildField26Far(struct DispatchObject *, u32);
void Func_080caa2c(u32 object_id);

void ObjectEvent_SetMode5WithCue(u32 object_id)
{
    Object_SetMode(ObjectTable_Get(object_id), 5);
    Audio_PlayCue(125);
    WaitFrames(12);
}

/* After 18 frames the object takes mode 7 with cue 146 and springs up. */
void ObjectEvent_Spring(u32 object_id)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    WaitFrames(18);
    Object_SetMode(object, 7);
    Audio_PlayCue(146);
    if (object != 0) {
        object->velocity_y = 0x80000;
        ObjectDispatch_SetSingleChildField26Far((struct DispatchObject *)object, 1);
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

/* Places the object at (x, z) on the ground; -1 keeps its home
   coordinate. */
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
    /* These localizations reach the terrain query through this entry, as
       ObjectMotion_SnapToTerrain does. */
    object->y = object->terrain_height = Func_08020190(0, x, z);
#else
    object->y = object->terrain_height = Map_GetTerrainHeightFar(0, x, z);
#endif
}

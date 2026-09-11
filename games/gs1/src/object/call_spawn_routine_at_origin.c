#include "types.h"
#include "scene.h"
#include "abi/object/call_spawn_routine_at_origin.h"

s32 Object_CallSpawnRoutineAtOrigin(s32 value)
{
    return Obj_SetMode(value, 0, 0, 0);
}

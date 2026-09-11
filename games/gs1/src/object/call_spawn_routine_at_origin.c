#include "types.h"
#include "scene.h"

s32 Object_CallSpawnRoutineAtOrigin(s32 value)
{
    return Obj_SetMode(value, 0, 0, 0);
}

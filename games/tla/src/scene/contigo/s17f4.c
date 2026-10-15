#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, u32 value);
void Func_020017bc(s32 id);

// Sets up actors 19 to 33: their modes, Func_020017bc for 19 to 26, and child and collision flags for 27 to 33.
void Func_020017f4(void)
{
    Object_SetModeById(19, 0);
    Object_SetModeById(20, 1);
    Object_SetModeById(21, 2);
    Object_SetModeById(22, 0);
    Object_SetModeById(23, 1);
    Object_SetModeById(24, 2);
    Object_SetModeById(25, 3);
    Object_SetModeById(26, 4);
    Func_020017bc(19);
    Func_020017bc(20);
    Func_020017bc(21);
    Func_020017bc(22);
    Func_020017bc(23);
    Func_020017bc(24);
    Func_020017bc(25);
    Func_020017bc(26);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(27), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(28), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(29), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(30), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(31), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(32), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(33), 0);
    Object_GetById(27)->collision_flags |= 8;
    Object_GetById(28)->collision_flags |= 8;
    Object_GetById(29)->collision_flags |= 8;
    Object_GetById(30)->collision_flags |= 8;
    Object_GetById(31)->collision_flags |= 8;
    Object_GetById(32)->collision_flags |= 8;
    Object_GetById(33)->collision_flags |= 8;
    Object_SetModeById(27, 0);
    Object_SetModeById(32, 0);
    Object_SetModeById(33, 0);
}

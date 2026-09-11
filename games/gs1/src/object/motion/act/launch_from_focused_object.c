#include "scene.h"
#include "abi/object/motion/act/launch_from_focused_object.h"
#include "object_runtime.h"

void Motion_SetSpeed(s32, s32, s32);
struct ObjectRuntime *Object_GetById(u32);
void Motion_SetHPosTerrain(u32, s32, s32);
void Object_SetMode(struct ObjectRuntime *, s32);
void Motion_OffsetPositionAndResetMotion(s32, s32, s32);
void Object_SetCallback(struct ObjectRuntime *, void *);

extern s32 gCell[];
extern u8 gRom;

void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct ObjectRuntime *object = ObjectTable_Get(arg0);

    if (object != NULL) {
        struct ObjectRuntime *other;

        Motion_SetSpeed(arg0, 0x9999, 0x4CCC);
        other = Object_GetById(gCell[125]);
        if (other != NULL)
            Motion_SetHPosTerrain(arg0, other->x, other->z);
        object->movement_state = 0;
        Object_SetMode(object, 2);
        Motion_OffsetPositionAndResetMotion(arg0, arg1, arg2);
        Object_SetCallback(object, &gRom);
        object->action = arg3;
    }
}

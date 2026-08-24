#include "object_runtime.h"

void ObjectMotion_SetSpeedParameters(s32, s32, s32);
struct ObjectRuntime *Object_GetById(u32);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32, s32, s32);
void Object_SetMode(struct ObjectRuntime *, s32);
void Func_0809228c(s32, s32, s32);
void Object_SetCallback(struct ObjectRuntime *, void *);

extern s32 Data_02000240[];
extern u8 Data_0809fbcc;

void Func_0809233c(u32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct ObjectRuntime *object = ObjectTable_Get(arg0);

    if (object != NULL) {
        struct ObjectRuntime *other;

        ObjectMotion_SetSpeedParameters(arg0, 0x9999, 0x4CCC);
        other = Object_GetById(Data_02000240[125]);
        if (other != NULL)
            ObjectMotion_SetHorizontalPositionWithTerrain(arg0, other->x, other->z);
        object->movement_state = 0;
        Object_SetMode(object, 2);
        Func_0809228c(arg0, arg1, arg2);
        Object_SetCallback(object, &Data_0809fbcc);
        object->action = arg3;
    }
}

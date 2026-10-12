#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Task_Wait(s32 frames);

struct ActorSprite {
    u8 unknown_00[18];
    u16 rotation;
};

void Func_02000098(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    Engine_ActorSetAnimation(id, 1);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 1);
    object->x -= 0xc0000;
    ((struct ActorSprite *)object->animation)->rotation = 0;
    Task_Wait(1);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 1);
    object->velocity_y = 0x40000;
    object->motion_flags = 3;
    Task_Wait(40);
}

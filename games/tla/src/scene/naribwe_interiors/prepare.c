#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001158(s32 id);
void Func_02001108(s32 id, s32 x, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);

s32 Func_02000f28(void)
{
    struct ObjectRuntime *object;

    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x209;
    Object_GetById(gPartyState.current_owner)->priority_flags |= 0x20;
    Func_02001158(17);
    Func_02001158(18);
    ObjectMotion_SetActionVariant(10, 2);
    Object_GetById(12)->collision_flags |= 4;
    ObjectMotion_SetActionVariant(16, 2);
    Object_GetById(16)->collision_flags |= 4;

    object = Object_GetById(18);
    object->motion_flags = 0;
    object->y = -0x40000;
    object->terrain_height = -0x40000;
    Object_GetById(18)->priority_flags |= 0x20;
    ObjectMotion_SetActionVariant(18, 1);
    Func_02001108(18, 0, 0);

    object = Object_GetById(19);
    object->motion_flags = 0;
    object->y = 0xc0000;
    object->terrain_height = 0xc0000;
    Object_GetById(19)->priority_flags |= 0x20;
    ObjectMotion_SetActionVariant(19, 1);
    Func_02001108(19, 0, 0);

    object = Object_GetById(20);
    object->motion_flags = 0;
    object->y = 0xc0000;
    object->terrain_height = 0xc0000;
    Object_GetById(20)->priority_flags |= 0x20;
    ObjectMotion_SetActionVariant(20, 1);
    Func_02001108(20, 0, 0);

    ObjectDispatch_SetSingleChildFlags(Object_GetById(19), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(20), 0);
    Object_SetPartAttribute(Object_GetById(20), 4);
    return 0;
}

s32 Scene_PrepareMap(void)
{
    return 0;
}

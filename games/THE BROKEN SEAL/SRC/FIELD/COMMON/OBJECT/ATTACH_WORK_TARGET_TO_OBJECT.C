#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "OBJECT_EFFECT.H"
#include "GLOBAL_CELLS.H"
#include "OBJECT_RUNTIME.H"
#include "MAP_SCROLL.H"
#include "EVENT_RUNTIME.H"
void ObjectDispatch_InitFromTable4WithArgumentFar(void *object, void *effect);
void Map_ApplyWorkOriginAndSpanFar(void);

void WaitFrames(s32);

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Object_AttachWorkTargetToObject(s32 id, s32 flag)
{
    struct ObjectRuntime *obj;
    struct ObjectRuntime *target;
    struct EventWork *work;
    struct MapScrollWork *p;

    obj = ObjectTable_Get(id);
    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    target = (struct ObjectRuntime *)work->view_center;
    p = gMapWork[0];
    if (obj != 0) {
        p->origin = &target->x;
        ObjectDispatch_InitFromTable4WithArgumentFar(target, (void *)obj);
        if (flag == 0) {
            target->x = obj->x;
            target->y = obj->y;
            target->z = obj->z;
            WaitFrames(1);
            if (((struct EventRuntime *)work)->mode_19e != 3) {
                Map_ApplyWorkOriginAndSpanFar();
            }
        }
    }
}

void ObjectTable_AllocateAndSetObjectSpeed(s32 first, s32 second)
{
    struct EventWork *owner = Runtime_AllocateBlock(0x1b, 0xccc);
    ((struct ObjectRuntime *)owner->view_center)->speed_limit = first;
    ((struct ObjectRuntime *)owner->view_center)->acceleration = second;
}

void Object_ResetMotion(void *);

void Object_SetPosition(void *, s32, s32, s32);

void Motion_CamBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter)
{
    s32 should_use_setter;
    struct EventWork *runtime_block;
    s32 minimum_x;
    s32 minimum_z;
    s32 maximum_x;
    s32 object_z_offset;
    s32 maximum_z;
    s32 position_x;
    s32 position_z;
    s32 position_y;
    struct MapScrollWork *camera_state;
    struct ObjectRuntime *object;

    position_x = requested_x;
    position_y = requested_y;
    should_use_setter = use_setter;
    position_z = requested_z;
    runtime_block = Runtime_AllocateBlock(0x1B, 0xCCC);
    object = (struct ObjectRuntime *)runtime_block->view_center;
    camera_state = gMapWork[0];
    minimum_x = camera_state->min_x + 0x780000;
    object_z_offset = object->y;
    minimum_z = camera_state->min_y + object_z_offset + 0x600000;
    maximum_x = camera_state->max_x + 0xFF880000;
    maximum_z = camera_state->max_y + object_z_offset + 0xFFC00000;
    camera_state->origin = &object->x;
    Object_ResetMotion(object);
    if (position_x == -1) {
        position_x = object->x;
    }
    if (position_y == -1) {
        position_y = object->y;
    }
    if (position_z == -1) {
        position_z = object->z;
    }
    if (position_x < minimum_x) {
        position_x = minimum_x;
    }
    if (position_z < minimum_z) {
        position_z = minimum_z;
    }
    if (position_x > maximum_x) {
        position_x = maximum_x;
    }
    if (position_z > maximum_z) {
        position_z = maximum_z;
    }
    if (should_use_setter == 0) {
        object->x = position_x;
        object->y = position_y;
        object->z = position_z;
        WaitFrames(1U);
        if (((struct EventRuntime *)runtime_block)->mode_19e != 3) {
            Map_ApplyWorkOriginAndSpanFar();
        }
    } else {
        Object_SetPosition(object, position_x, position_y, position_z);
    }
}

void Motion_CamBounds(s32, s32, s32, s32);

void Object_PlaceCurrentWithinCameraBounds(s32 arg0, s32 arg1)
{
    struct ObjectRuntime *obj;

    obj = ObjectTable_Get();
    Runtime_AllocateBlock(0x1B, 0xCCC);
    if (obj != NULL) {
        Motion_CamBounds(obj->x, -1, obj->z, arg1);
    }
}

void Object_CommitPosition(struct ObjectRuntime *);
void Battle_WaitMode0(s32 arg0);

void BattleFx_CommitObjectPositionAndWait(void)
{
    Object_CommitPosition((struct ObjectRuntime *)
        ((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->view_center);
    Battle_WaitMode0(2);
}

s32 Battle_GetWorkObject1e0(void)
{
    return (s32)((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->view_center;
}

void BattleFx_LinkObjectToTarget(struct ObjectRuntime *target, s32 keep_current_position)
{
    struct ObjectRuntime *object;

    object = (struct ObjectRuntime *)
        ((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->view_center;
    if (target != NULL) {
        ObjectDispatch_InitFromTable4WithArgumentFar(object, NULL);
        object->linked_object = target;
        if (keep_current_position == 0) {
            object->x = target->x;
            object->y = target->y;
            object->z = target->z;
        }
    }
}

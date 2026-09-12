#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "object_effect.h"
#include "global_cells.h"
#include "object_runtime.h"

/* object/attach_work_target_to_object.c */
/* object/attach_work_target_to_object.c */
void *Runtime_AllocateBlock(s32 id, s32 flag);
void WaitFrames(s32);

void Object_AttachWorkTargetToObject(s32 id, s32 flag)
{
    s32 obj;
    void *target;
    void *work;
    s32 *p;

    obj = ObjectTable_Get(id);
    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    target = FIELD_AT_OFFSET(work, void **, 0x1E0);
    p = *(s32 **)ADDR_03001E70;
    if (obj != 0) {
        *p = (s32)((u8 *)target + 8);
        Obj_Run(target, (void *)obj);
        if (flag == 0) {
            FIELD_AT_OFFSET(target, s32 *, 8) = (s32)FIELD_AT_OFFSET(obj, s32 *, 8);
            FIELD_AT_OFFSET(target, s32 *, 0xC) = (s32)FIELD_AT_OFFSET(obj, s32 *, 0xC);
            FIELD_AT_OFFSET(target, s32 *, 0x10) = (s32)FIELD_AT_OFFSET(obj, s32 *, 0x10);
            WaitFrames(1);
            if (FIELD_AT_OFFSET(work, s16 *, 0x19E) != 3) {
                Map_ApplyWorkOriginAndSpan();
            }
        }
    }
}

/* object/table/allocate_and_set_object_speed.c */
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

struct ObjectOwner_080933d4 {
    u8 unknown_000[0x1e0];
    struct ObjectRuntime *object;
};

void ObjectTable_AllocateAndSetObjectSpeed(s32 first, s32 second)
{
    struct ObjectOwner_080933d4 *owner = Runtime_AllocateBlock(0x1b, 0xccc);
    owner->object->speed_limit = first;
    owner->object->acceleration = second;
}

/* object/motion/pos/place_within_camera_bounds.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Object_ResetMotion(void *);
void WaitFrames(u32);

void Object_SetPosition(void *, s32, s32, s32);

void Motion_CamBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter)
{
    s32 should_use_setter;
    void *runtime_block;
    s32 minimum_x;
    s32 minimum_z;
    s32 maximum_x;
    s32 object_z_offset;
    s32 maximum_z;
    s32 position_x;
    s32 position_z;
    s32 position_y;
    void *camera_state;
    void *object;

    position_x = requested_x;
    position_y = requested_y;
    should_use_setter = use_setter;
    position_z = requested_z;
    runtime_block = Runtime_AllocateBlock(0x1B, 0xCCC);
    object = FIELD_AT_OFFSET(runtime_block, void **, 0x1E0);
    camera_state = *(void **)ADDR_03001E70;
    minimum_x = FIELD_AT_OFFSET(camera_state, s32, 0xEC) + 0x780000;
    object_z_offset = FIELD_AT_OFFSET(object, s32, 0xC);
    minimum_z = FIELD_AT_OFFSET(camera_state, s32, 0xF0) + object_z_offset + 0x600000;
    maximum_x = FIELD_AT_OFFSET(camera_state, s32, 0xF4) + 0xFF880000;
    maximum_z = FIELD_AT_OFFSET(camera_state, s32, 0xF8) + object_z_offset + 0xFFC00000;
    FIELD_AT_OFFSET(camera_state, void **, 0) = (void *)(object + 8);
    Object_ResetMotion(object);
    if (position_x == -1) {
        position_x = FIELD_AT_OFFSET(object, s32, 8);
    }
    if (position_y == -1) {
        position_y = FIELD_AT_OFFSET(object, s32, 0xC);
    }
    if (position_z == -1) {
        position_z = FIELD_AT_OFFSET(object, s32, 0x10);
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
        FIELD_AT_OFFSET(object, s32, 8) = position_x;
        FIELD_AT_OFFSET(object, s32, 0xC) = position_y;
        FIELD_AT_OFFSET(object, s32, 0x10) = position_z;
        WaitFrames(1U);
        if (FIELD_AT_OFFSET(runtime_block, s16, 0x19E) != 3) {
            Obj_Run();
        }
    } else {
        Object_SetPosition(object, position_x, position_y, position_z);
    }
}

/* object/motion/pos/place_current_within_camera_bounds.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Runtime_AllocateBlock(s32, s32);
void Motion_CamBounds(s32, s32, s32, s32);

void Object_PlaceCurrentWithinCameraBounds(s32 arg0, s32 arg1)
{
    void *obj;

    obj = ObjectTable_Get();
    Runtime_AllocateBlock(0x1B, 0xCCC);
    if (obj != NULL) {
        Motion_CamBounds(FIELD_AT_OFFSET(obj, s32 *, 8), -1, FIELD_AT_OFFSET(obj, s32 *, 0x10), arg1);
    }
}

/* battle/fx_commit_object_position_and_wait.c */
/* battle/effects/object_control/commit_position_and_wait.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 Object_CommitPosition(s32);
void Battle_WaitMode0(s32 arg0);

void BattleFx_CommitObjectPositionAndWait(void)
{
    Object_CommitPosition(FIELD_AT_OFFSET(Runtime_AllocateBlock(0x1B, 0xCCC), s32 *, 0x1E0));
    Battle_WaitMode0(2);
}

/* battle/get_work_object_1e0.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

s32 Battle_GetWorkObject1e0(void)
{
    return FIELD_AT_OFFSET(Runtime_AllocateBlock(0x1B, 0xCCC), s32 *, 0x1E0);
}

/* battle/effects/object_control/link_object_to_target.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void BattleFx_LinkObjectToTarget(void *target, s32 keep_current_position)
{
    void *object;

    object = FIELD_AT_OFFSET(Runtime_AllocateBlock(0x1B, 0xCCC), void **, 0x1E0);
    if (target != NULL) {
        Battle_Run(object, NULL);
        FIELD_AT_OFFSET(object, void **, 0x68) = target;
        if (keep_current_position == 0) {
            FIELD_AT_OFFSET(object, s32 *, 8) = FIELD_AT_OFFSET(target, s32 *, 8);
            FIELD_AT_OFFSET(object, s32 *, 0xC) = FIELD_AT_OFFSET(target, s32 *, 0xC);
            FIELD_AT_OFFSET(object, s32 *, 0x10) = FIELD_AT_OFFSET(target, s32 *, 0x10);
        }
    }
}

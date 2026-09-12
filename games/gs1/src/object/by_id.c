#include "object_runtime.h"

void ObjectMotion_SetActionVariant(u32, s32);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32, s32, s32);
void Object_SetCallback(struct ObjectRuntime *, const void *);
void Object_SetAction(struct ObjectRuntime *, s32);
s32 Func_080091a8(u8, s32, s32);
void WaitFrames(s32);
void Battle_WaitMode0(s32);
void Audio_PlayCue(s32);


void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Func_08009148(struct ObjectRuntime *);
void Func_08093a6c(struct ObjectRuntime *, s32);

struct ObjectRuntime *Object_GetById(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object == NULL)
        return NULL;
    return object;
}

void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->acceleration = acceleration;
        object->speed_limit = speed_limit;
    }
}

void ObjectMotion_EnableActionAndSetCallback(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        Func_08093a6c(object, action);
    }
}

void ObjectMotion_EnableActionAndResetMotion(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    u8 enabled;

    if (object != NULL) {
        enabled = 1;
        object->action_flags = enabled | object->action_flags;
        Object_ResetMotion(object);
    }
}

extern const u8 Data_0809ff40[];

void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->linked_object = Object_GetById(linked_object_id);
        Func_08093a6c(object, (s32)Data_0809ff40);
    }
}

void Object_RefreshSelectorById(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        Func_08009148(object);
}

void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        Func_08093a6c(object, action);
        Func_08009148(object);
    }
}

void ObjectMotion_ResetAndSetPosition(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetPosition(object, x << 16, object->y, z << 16);
    }
}

void ObjectMotion_SetPositionAndCommit(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetPosition(object, x << 16, object->y, z << 16);
        Object_CommitPosition(object);
    }
}

void ObjectMotion_ResetAndSetPositionInMode2(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetMode(object, 2);
        Object_SetPosition(object, x << 16, object->y, z << 16);
    }
}

void ObjectMotion_SetPositionAndReset(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetMode(object, 2);
        Object_SetPosition(object, x << 16, object->y, z << 16);
        Object_CommitPosition(object);
        Object_SetMode(object, 1);
    }
}

void ObjectMotion_SnapHeadingAndOffset(u32 object_id, s32 action, s32 z_offset)
{
    struct ObjectRuntime *object;
    s16 current_angle;
    s32 snapped_angle;
    s16 angle_remainder;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        current_angle = *(s16 *)((u8 *)object + 0x0a);
        snapped_angle = current_angle;
        if (current_angle < 0) {
            snapped_angle += 15;
        }
        snapped_angle >>= 4;
        snapped_angle *= 16;
        angle_remainder = current_angle - snapped_angle;
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetMode(object, 2);
        Object_SetPosition(object,
            object->x + ((8 - angle_remainder) << 16),
            object->y, object->z);
        Object_CommitPosition(object);
        ObjectMotion_SetActionVariant(object_id, action);
        Object_SetPosition(object, object->x, object->y,
            object->z + (z_offset << 16));
    }
}

void ObjectMotion_OffsetPositionAndResetMotion(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetPosition(object, object->x + (x_offset << 16),
            object->y, object->z + (z_offset << 16));
    }
}

void ObjectMotion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetMode(object, 2);
        Object_SetPosition(object, object->x + (x_offset << 16),
            object->y, object->z + (z_offset << 16));
    }
}

void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    ObjectMotion_OffsetPositionAndReset(object_id, x_offset, z_offset);
    if (object != NULL) {
        Object_CommitPosition(object);
        Object_SetMode(object, 1);
    }
}

extern s32 Data_02000240[];
extern u8 Data_0809fbcc;

void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3)
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
        ObjectMotion_OffsetPositionAndResetMotion(arg0, arg1, arg2);
        Object_SetCallback(object, &Data_0809fbcc);
        object->action = arg3;
    }
}

void ObjectMotion_CommitCurrentPositionAndActivate(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        Object_CommitPosition(object);
        Object_SetMode(object, 1);
    }
}

void ObjectMotion_SetHorizontalPositionWithTerrain(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object;
    s32 terrain_height;
    s32 tile_x;
    s32 tile_z;
    s32 terrain_id;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        Object_ResetMotion(object);
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
        object->target_y = 0x80000000;
        object->target_x = 0x80000000;
        object->x = x;
        object->z = z;
        if (1 & object->flags) {
            terrain_id = object->terrain_id;
            tile_x = x;
            if (tile_x < 0) {
                tile_x += 0xFFFF;
            }
            tile_x = tile_x >> 0x10;
            tile_z = z;
            if (tile_z < 0) {
                tile_z += 0xFFFF;
            }
            tile_z = tile_z >> 0x10;
            terrain_height = Func_080091a8(terrain_id, tile_x, tile_z) << 0x10;
            object->y = (object->y - object->terrain_height) + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z)
{
    struct ObjectRuntime *object;
    s32 terrain_height;
    s32 tile_x;
    s32 tile_z;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        Object_ResetMotion(object);
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
        object->target_y = 0x80000000;
        object->target_x = 0x80000000;
        object->x = x;
        object->y = y;
        object->z = z;
        if (1 & object->flags) {
            s32 terrain_id = object->terrain_id;
            tile_x = x;
            if (tile_x < 0) {
                tile_x += 0xFFFF;
            }
            tile_x >>= 0x10;
            tile_z = z;
            if (tile_z < 0) {
                tile_z += 0xFFFF;
            }
            terrain_height =
                Func_080091a8(terrain_id, tile_x, tile_z >> 0x10) << 0x10;
            object->y = (object->y - object->terrain_height) + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

void Object_SetModeById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        Object_SetMode(object, action);
}

void Object_SetActionById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        Object_SetAction(object, action);
}

void ObjectMotion_WaitForAnimationChange(u32 object_id)
{
    struct ObjectRuntime *object;
    u8 *ptr;
    volatile s32 saved;
    s32 i;

    object = ObjectTable_Get(object_id);
    if (object != NULL && object->animation_kind == 1) {
        ptr = object->animation;
        saved = ptr[36];
        for (i = 0; i <= 89; i++) {
            WaitFrames(1);
            if (saved != ptr[36]) {
                break;
            }
        }
    }
}

void Motion_SetModeAndWaitAnimation(u32 object_id, s32 action)
{
    Object_SetModeById(object_id, action);
    ObjectMotion_WaitForAnimationChange(object_id);
}

void ObjectMotion_NoOp(void)
{
}

void ObjectMotion_Launch(u32 object_id, s32 speed, s32 event_id)
{
    struct ObjectRuntime *object;
    u8 *object_flags;
    s32 updated_flags;
    s32 vertical_velocity;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        object_flags = &object->flags;
        updated_flags = *object_flags | 2;
        vertical_velocity = speed << 16;
        *object_flags = updated_flags;
        object->velocity_y = vertical_velocity;
        if (speed > 5) {
            Audio_PlayCue(0x99);
        } else {
            Audio_PlayCue(0x98);
        }
        Battle_WaitMode0(event_id);
    }
}

extern const u8 Data_0809ebfc[];

void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        Object_SetCallback(object,
            Data_0809ebfc + ((3 - variant) << 7));
    }
}

void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant)
{
    ObjectMotion_SetVariantCallback(object_id, variant);
    Object_RefreshSelectorById(object_id);
}

#include "scene.h"
#include "object_runtime.h"
#include "object_lookup.h"
#include "types.h"

/* object/get_by_id.c */
struct ObjectRuntime *Object_GetById(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object == NULL)
        return NULL;
    return object;
}

/* object/motion/act/set_speed_parameters.c */
void Motion_SetSpeed(u32 object_id, s32 speed_limit, s32 acceleration)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->acceleration = acceleration;
        object->speed_limit = speed_limit;
    }
}

/* object/motion/act/enable_action_and_set_callback.c */
void Motion_SetActionCallback(struct ObjectRuntime *object, s32 kind);

void Motion_EnableActCb(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        Motion_SetActionCallback(object, action);
    }
}

/* object/motion/act/enable_action_and_reset_motion.c */
s32 Object_ResetMotion();
void Motion_EnableReset(void)
{
  void *object;
  unsigned char action_enabled;
  object = ObjectTable_Get();
  if (object != ((void *) 0))
  {
    action_enabled = 1;
    *((u8 *)(((u8 *)object) + 0x5A)) = (u8)(action_enabled | (*((u8 *)(((u8 *)object) + 0x5A))));
    Object_ResetMotion();
  }
}

/* object/link/link_object_and_set_callback.c */
struct ObjectRuntime *Object_GetById(u32);
void Motion_SetActionCallback(struct ObjectRuntime *, s32);
extern const u8 gRom[];

void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->linked_object = Object_GetById(linked_object_id);
        Motion_SetActionCallback(object, (s32)gRom);
    }
}

/* object/refresh_selector_by_id.c */
void Object_RefreshSelectorById(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        Obj_Do(object);
}

/* object/set_action_callback_and_refresh_by_id.c */
void Motion_SetActionCallback(struct ObjectRuntime *, s32);

void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        Motion_SetActionCallback(object, action);
        Obj_Do(object);
    }
}

/* object/motion/pos/reset_and_set_position.c */
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);

void Motion_ResetAndSetPosition(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetPosition(object, x << 16, object->y, z << 16);
    }
}

/* object/motion/pos/set_position_and_commit.c */
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);

void Motion_SetPositionAndCommit(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetPosition(object, x << 16, object->y, z << 16);
        Object_CommitPosition(object);
    }
}

/* object/motion/pos/reset_and_set_position_in_mode_2.c */
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);

void Motion_ResetPosMode2(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetMode(object, 2);
        Object_SetPosition(object, x << 16, object->y, z << 16);
    }
}

/* object/motion/pos/set_position_and_reset.c */
void Object_SetMode(struct ObjectRuntime *, s32);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);

void Motion_SetPosReset(u32 object_id, s32 x, s32 z)
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

/* object/motion/pos/snap_heading_and_offset.c */
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Motion_SetActionVariant(u32 object_id, s32 action);

void Motion_SnapHeadingAndOffset(u32 object_id, s32 action, s32 z_offset)
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
        Motion_SetActionVariant(object_id, action);
        Object_SetPosition(object, object->x, object->y,
            object->z + (z_offset << 16));
    }
}

/* object/motion/pos/offset_position_and_reset_motion.c */
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);

void Motion_OffsetPositionAndResetMotion(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        Object_ResetMotion(object);
        Object_SetPosition(object, object->x + (x_offset << 16),
            object->y, object->z + (z_offset << 16));
    }
}

/* object/motion/pos/offset_position_and_reset.c */
void Object_SetMode(struct ObjectRuntime *, s32);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);

void Motion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset)
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

/* object/motion/pos/commit_position_and_activate.c */
void Object_SetMode(struct ObjectRuntime *, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Motion_OffsetPositionAndReset(u32, s32, s32);

void Motion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    Motion_OffsetPositionAndReset(object_id, x_offset, z_offset);
    if (object != NULL) {
        Object_CommitPosition(object);
        Object_SetMode(object, 1);
    }
}

/* object/motion/act/launch_from_focused_object.c */
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

/* object/motion/pos/commit_current_position_and_activate.c */
void Object_SetMode(struct ObjectRuntime *, s32);
void Object_CommitPosition(struct ObjectRuntime *);

void Motion_CommitPos(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        Object_CommitPosition(object);
        Object_SetMode(object, 1);
    }
}

/* object/motion/pos/set_horizontal_position_with_terrain.c */
void Object_ResetMotion(struct ObjectRuntime *);

void Motion_SetHPosTerrain(u32 object_id, s32 x, s32 z)
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
            terrain_height = Obj_Place(terrain_id, tile_x, tile_z) << 0x10;
            object->y = (object->y - object->terrain_height) + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

/* object/motion/pos/set_position_with_terrain.c */
void Object_ResetMotion(struct ObjectRuntime *);

void Motion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z)
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
                Obj_Place(terrain_id, tile_x, tile_z >> 0x10) << 0x10;
            object->y = (object->y - object->terrain_height) + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

/* object/set_mode_by_id.c */
void Object_SetMode(struct ObjectRuntime *, s32);

void Object_SetModeById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        Object_SetMode(object, action);
}

/* object/set_action_by_id.c */
void Object_SetAction(struct ObjectRuntime *, s32);

void Object_SetActionById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        Object_SetAction(object, action);
}

/* object/motion/act/wait_for_animation_change.c */
void WaitFrames(s32);

void Motion_WaitForAnimationChange(u32 object_id)
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

/* object/motion/act/set_mode_and_wait_animation.c */
s32 Motion_WaitForAnimationChange(s32);

void Motion_SetModeAndWaitAnimation(s32 arg0)
{
    Obj_Check();
    Motion_WaitForAnimationChange(arg0);
}

/* object/motion/act/launch.c */
void Battle_WaitMode0(s32 arg0);
void Audio_PlayCue(s32 arg0);

void Motion_Launch(u32 object_id, s32 speed, s32 event_id)
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

/* object/motion/act/set_variant_callback.c */
void Object_SetCallback(struct ObjectRuntime *, const void *);
extern const u8 gRom[];

void Motion_SetVarCb(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        Object_SetCallback(object,
            gRom + ((3 - variant) << 7));
    }
}

/* object/motion/act/set_variant_callback_and_refresh.c */
s32 Motion_SetVarCb();

void Motion_SetVarCbAndRefresh(s32 arg0)
{
    Motion_SetVarCb();
    Obj_Check(arg0);
}

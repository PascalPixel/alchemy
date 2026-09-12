#include "types.h"
#include "scene.h"
#include "object_runtime.h"
#include "object_lookup.h"
#include "fixed_math.h"
#include "object_dispatch.h"
#include "event_runtime.h"
#include "global_cells.h"

/* event/set_pair1d4.c */
/* event/set_pair1d4.c */
struct State_08091fa8 {
    u8 filler0[0x170];
    u16 value;
};

struct gRom {
    u8 filler0[0x1C0];
    u16 first_1c0;
    u16 second_1c2;
    u8 filler1c4[14];
    u16 first_1d4;
    u16 second_1d6;
};

extern struct State_08091fa8 *volatile gWork;
extern struct gRom gCell;

void Event_SetPair1d4(u16 first, u16 second)
{
    gCell.first_1d4 = first;
    gCell.second_1d6 = second;
}

void Sys_Run(u16 first, u16 second)
{
    struct State_08091fa8 *state = gWork;
    gCell.first_1c0 = first;
    gCell.second_1c2 = second;
    state->value = 999;
}

/* battle/effects/audio/fx_set_queued_sound_and_play.c */
/* battle/effects/audio/set_queued_sound_and_play.c */
struct BattleEventState {
    u8 padding[0xcc8];
    s16 queued_sound;
};

extern struct BattleEventState *gWork;
void Audio_PlayCue(s32);

void BattleFx_SetQueuedSoundAndPlay(s32 sound_id)
{
    gWork->queued_sound = sound_id;
    if ((s16)sound_id == -1) {
        sound_id = 0x121;
    }
    Audio_PlayCue(0x12a);
    Audio_PlayCue(sound_id);
}

/* battle/effects/audio/play_queued_sound.c */
struct BattleEventState {
    u8 padding[0xcc8];
    s16 queued_sound;
};

extern struct BattleEventState *gWork;
void Audio_PlayCue(s32);

void BattleFx_PlayQueuedSound(void)
{
    s16 sound_id = gWork->queued_sound;

    if (sound_id != -1)
        Audio_PlayCue(sound_id);
}

/* object/get_by_id.c */
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
extern const u8 gRomGetById[];

void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->linked_object = Object_GetById(linked_object_id);
        Motion_SetActionCallback(object, (s32)gRomGetById);
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
extern u8 gRomGetById;

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
        Object_SetCallback(object, &gRomGetById);
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
extern const u8 gRomGetById[];

void Motion_SetVarCb(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        Object_SetCallback(object,
            gRomGetById + ((3 - variant) << 7));
    }
}

/* object/motion/act/set_variant_callback_and_refresh.c */
s32 Motion_SetVarCb();

void Motion_SetVarCbAndRefresh(s32 arg0)
{
    Motion_SetVarCb();
    Obj_Check(arg0);
}

/* battle/effects/fx_update_particle_linear_motion.c */
/* battle/effects/particles/update_linear_motion.c */
s32 FixedPoint_Ratio(s32, s32);
void ParticleEffect_UpdateLinearMotion(void *particle)
{
  s32 velocity_x;
  s32 x;
  s32 velocity_z;
  s32 z;
  s32 velocity_y;
  s32 vz;
  velocity_x = *((s32 *)(((u8 *)particle) + 0x30));
  x = (*((s32 *)(((u8 *)particle) + 8))) + velocity_x;
  *((s32 *)(((u8 *)particle) + 8)) = x;
  *((s32 *)(((u8 *)particle) + 0x38)) = x;
  velocity_z = *((s32 *)(((u8 *)particle) + 0x34));
  z = (*((s32 *)(((u8 *)particle) + 0x10))) + velocity_z;
  *((s32 *)(((u8 *)particle) + 0x10)) = z;
  *((s32 *)(((u8 *)particle) + 0x40)) = z;
  velocity_y = (*((s32 *)(((u8 *)particle) + 0xC))) + 0x400;
  *((s32 *)(((u8 *)particle) + 0xC)) = velocity_y;
  *((s32 *)(((u8 *)particle) + 0x3C)) = velocity_y;
  *((s32 *)(((u8 *)particle) + 0x30)) =
      (s32)(velocity_x - FixedPoint_Ratio(velocity_x, 0x12));
 do {
   vz = velocity_z;
   if (velocity_z < 0) {
     vz += 0xF;
   }
   *((s32 *)(((u8 *)particle) + 0x34)) =
       (s32)(velocity_z - (vz >> 4));
 } while (0);
}

/* battle/effects/particles/spawn_burst.c */
struct Child_08092624 {
    u8 pad_00[9];
    u8 low_09 : 2;
    u8 copied_09 : 2;
    u8 high_09 : 4;
    u8 pad_0a[28];
    u8 field_26;
};

struct Object_08092624 {
    u8 pad_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[28];
    s32 field_30;
    s32 field_34;
    u8 pad_38[24];
    struct Child_08092624 *child;
    u8 pad_54;
    u8 mode_55;
    u8 pad_56[14];
    u16 field_64;
    u8 pad_66[6];
    void (*callback_6c)(void);
};

extern struct Object_08092624 *Battle_RunParticleLinearMotion(s32, s32, s32, s32);
extern s32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
extern void Object_SetMode(struct Object_08092624 *, s32);
extern void Object_SetCallback(struct Object_08092624 *, const void *);
extern void ObjectGroup_SetChildValue(struct Object_08092624 *);

extern const u8 gRomParticleLinearMotion[];
extern const u8 gRom2[];
extern u8 gRom3;

void BattleFx_SpawnBurstParticle(struct Object_08092624 *source, s32 optional)
{
    struct Object_08092624 *object;
    struct Child_08092624 *child;
    s32 value;

    object = Battle_RunParticleLinearMotion(222, source->x, source->y, source->z);
    if (object != 0) {
        child = object->child;
        switch (Rand() & 1) {
        case 1:
            Object_SetMode(object, 2);
            Object_SetCallback(object, gRomParticleLinearMotion);
            break;
        default:
            Object_SetMode(object, 1);
            Object_SetCallback(object, gRom2);
            break;
        }

        if (optional != 0)
            ObjectGroup_SetChildValue(object);

        object->mode_55 = 0;
        value = Battle_ApplyParticleLinearMotion(Rand(), 10) + 5;
        object->field_34 = -0x1999 * value;
        value = Battle_ApplyParticleLinearMotion(Rand(), 15) - 7;
        value <<= 1;
        object->field_30 = 0x1999 * value;
        object->field_64 = 0;
        object->callback_6c = (void (*)(void))&gRom3;
        child->field_26 = 0;
        child->copied_09 = source->child->copied_09;
    }
}

/* battle/effects/objects/run_rising_sequence.c */
#define FIELD_S32(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define OBJECT_X(object) FIELD_S32(object, 0x08)
#define OBJECT_Y(object) FIELD_S32(object, 0x0C)
#define OBJECT_Z(object) FIELD_S32(object, 0x10)
#define OBJECT_TARGET_Y(object) FIELD_S32(object, 0x14)
#define OBJECT_VERTICAL_STEP(object) FIELD_S32(object, 0x28)
#define OBJECT_MIRRORED_Y(object) FIELD_S32(object, 0x3C)

s32 Object_GetById(u32);
void Audio_PlayCue(s32);
void Object_SetMode(void *, s32);
void WaitFrames(u32);
void Object_SetPosition(void *, s32, s32, s32);
void Motion_ArmCb(s32 arg0, s32 arg1, s32 arg2);
void BattleFx_SpawnBurstParticle(void *, s32);
void Object_CommitPosition(void *);
void BattleFx_PlayQueuedSound(void);

void BattleFx_RunRisingObjectSequence(s32 sequence_arg, s32 mode_or_frame, s32 optional_action)
{
    s32 next_y;
    s32 base_z;
    void *object;
    u8 *object_flags;

    object = (void *)Object_GetById(sequence_arg);
    base_z = OBJECT_Z(object);
    if (object != 0) {
        Audio_PlayCue(0x121);
        Object_SetMode(object, mode_or_frame);
        WaitFrames(10);
        object_flags = (u8 *)object + 0x55;
        Object_SetMode(object, 1);
        {
            u8 flags = 2;
            flags |= *object_flags;
            *object_flags = flags;
        }
        OBJECT_VERTICAL_STEP(object) = 0x40000;
        Object_SetPosition(object, OBJECT_X(object), OBJECT_Y(object),
            base_z + 0xC0000);
        WaitFrames(6);
        Audio_PlayCue(0xD9);
        mode_or_frame = 0;
        Motion_ArmCb(sequence_arg, 0x5000, 0);
        *object_flags = 0;
        do {
            next_y = OBJECT_Y(object) + 0xFFFE0000;
            OBJECT_Y(object) = next_y;
            OBJECT_MIRRORED_Y(object) = next_y;
            WaitFrames(1);
            if ((optional_action != -1) && (mode_or_frame & 1)) {
                BattleFx_SpawnBurstParticle(object, optional_action);
            }
            mode_or_frame++;
        } while ((u32)mode_or_frame <= 0xD);
        *object_flags = 3;
        OBJECT_VERTICAL_STEP(object) = 0x30000;
        Object_SetPosition(object, OBJECT_X(object), OBJECT_Y(object),
            base_z + 0x100000);
        Object_CommitPosition(object);
        mode_or_frame = 0;
        if (OBJECT_Y(object) > OBJECT_TARGET_Y(object)) {
wait_for_target_y:
            WaitFrames(1);
            mode_or_frame++;
            if ((u32)mode_or_frame <= 0xB3) {
                if (OBJECT_Y(object) > OBJECT_TARGET_Y(object)) {
                    goto wait_for_target_y;
                }
            }
        }
        WaitFrames(2);
        BattleFx_PlayQueuedSound();
    }
}

/* object/set_angle_toward.c */
/* object/motion/aim/set_angle_toward.c */
struct Object_0809280c {
    u8 padding_00[6];
    s16 angle;
    s32 y;
    u8 padding_0c[4];
    s32 x;
};

s32 ArcTan2(s32, s32);
void Battle_WaitMode0(s32 arg0);

void Motion_SetAngleToward(s32 arg0, s32 arg1, s32 arg2)
{
    struct Object_0809280c *object1;
    struct Object_0809280c *object2;

    object1 = ObjectTable_Get(arg0);
    object2 = ObjectTable_Get(arg1);
    if (object1 != 0 && object2 != 0) {
        object1->angle = ArcTan2(object2->x - object1->x,
                                      object2->y - object1->y);
        Battle_WaitMode0(arg2);
    }
}

/* object/link/link_pair.c */
void ObjectLink_RotatePairToward(void *, void *);
void Battle_WaitMode0(s32 arg0);

void Object_LinkPair(s32 arg0, s32 arg1, s32 arg2)
{
    void *first = ObjectTable_Get(arg0);
    void *second = ObjectTable_Get(arg1);
    if (first != NULL && second != NULL) {
        ObjectLink_RotatePairToward(first, second);
        Battle_WaitMode0(arg2);
    }
}

/* object/table/destroy_by_id.c */
extern u8 *gWork;
/* 生成順維持のため戻り値を整数で受け、直後にオブジェクト番地として扱う。 */
s32 ObjectTable_Get(u32 object_id);
/* Object table: 192 pointers at gWork + 0x14 (object/table/get.c). */
void Object_Destroy(void *);

void ObjectTable_DestroyById(s32 index)
{
    void *object = (void *)ObjectTable_Get(index);
    u8 *base = gWork;
    s32 offset;

    if (object != 0) {
        Object_Destroy(object);
        offset = index * 4;
        offset += 20;
        *(s32 *)(base + offset) = 0;
    }
}

void ObjectTable_ReservedNoOp294C(void)
{
}

/* object/group/configure_child_value.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 gRomSetAngleToward;
void ObjectGroup_SetChildValue(void *, s32);

void ObjectGroup_ConfigureChildValue(s32 arg0, s32 arg1)
{
    s32 mode_flags;
    void *object;

    object = ObjectTable_Get();
    if (object != NULL) {
        mode_flags = 0x100 & arg1;
        if (mode_flags != 0) {
            FIELD_AT_OFFSET(object, s32 *, 0x6C) = (s32)&gRomSetAngleToward;
            return;
        }
        FIELD_AT_OFFSET(object, s32 *, 0x6C) = mode_flags;
        ObjectGroup_SetChildValue(object, arg1);
    }
}

/* object/group/apply_indexed_child_value.c */
extern u32 gIwSetAngleToward;
extern u8 gRomSetAngleToward[];

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object)
{
    if ((object->kind & 0xf) == 1) {
        u8 child_value;
        u8 *container;
        u8 child_count;

        child_value = gRomSetAngleToward[(gIwSetAngleToward >> 1) & 3];
        container = object->target.child;
        child_count = *(container + 0x27);
        if (child_count != 0) {
            u8 **entries = (u8 **)(container + 0x28);
            s32 remaining = child_count;
            do {
                u8 *entry = *entries++;
                if (entry != 0 && *(u32 *)(entry + 0x10) != 0) {
                    *(entry + 5) = child_value;
                }
                remaining--;
            } while (remaining != 0);
        }
        *(container + 0x25) = 1;
    }
}

/* object/group/set_child_value.c */
void ObjectGroup_SetChildValue(struct DispatchObject *object, s32 value)
{
    if ((object->kind & 0xf) == 1) {
        u8 *container = object->target.child;
        u8 raw_count = container[0x27];

        if (raw_count != 0) {
            void **entry = (void **)(container + 0x28);
            u32 count = raw_count;
            do {
                void *item = *entry++;
                if (item != NULL && *(s32 *)((u8 *)item + 0x10) != 0) {
                    *((s8 *)item + 5) = value;
                }
                count--;
            } while (count != 0);
        }
        container[0x25] = 1;
    }
}

/* object/link/set_target_callback.c */
struct Object_08092a1c {
    u8 padding_00[0x30];
    s32 field_30;
    s32 field_34;
    u8 padding_38[0x21];
    u8 field_59;
    u8 padding_5a[0x0a];
    s16 field_64;
    u8 padding_66[2];
    struct Object_08092a1c *field_68;
};

void Object_SetCallback(struct Object_08092a1c *, s32);

void Object_SetTargetAndCallback(u32 arg0, s32 arg1, s32 arg2)
{
    struct Object_08092a1c *first = ObjectTable_Get(arg0);
    struct Object_08092a1c *second = ObjectTable_Get(arg1 & 0xff);

    if (first != 0 && second != 0) {
        first->field_68 = second;
        if (!(arg1 & 0x10000)) {
            first->field_64 = 40;
            first->field_34 = second->field_34 * 2;
            first->field_30 = second->field_30;
            first->field_59 = 0;
        }
        Object_SetCallback(first, arg2);
    }
}

/* object/motion/aim/step_angle.c */
struct ObjectMotionAngleState {
    u8 unknown_000[6];
    u16 current_angle;
    u8 unknown_008[92];
    u16 target_angle;
};

s32 Motion_StepAngle(struct ObjectMotionAngleState *object)
{
    s32 delta = 0;

    if (object != 0) {
        s32 target_angle = object->target_angle;
        s32 current_angle = object->current_angle;
        delta = (s16)(target_angle - current_angle);
        if (delta != 0) {
            if (delta > 4096) {
                delta = 2048;
            }
            if (delta < -4096) {
                delta = -2048;
            }
            object->current_angle = current_angle + delta;
        }
    }
    return delta;
}

/* object/reset_target_and_set_mode1.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_SetMode(void *, s32);
s32 Object_ResetMotion();

void Object_ResetTargetAndSetMode1(void)
{
    void *temp_r0;

    temp_r0 = ObjectTable_Get();
    if (temp_r0 != NULL) {
        FIELD_AT_OFFSET(temp_r0, s32 *, 0x38) = 0x80000000;
        FIELD_AT_OFFSET(temp_r0, s32 *, 0x3C) = 0x80000000;
        FIELD_AT_OFFSET(temp_r0, s32 *, 0x40) = 0x80000000;
        Object_ResetMotion();
        Object_SetMode(temp_r0, 1);
    }
}

/* object/motion/act/arm_callback.c */
void Object_SetCallback(u8 *, void *);
void Battle_WaitMode0(s32 arg0);
extern u8 gRomSetAngleToward[];

void Motion_ArmCb(s32 arg0, s32 arg1, s32 arg2)
{
    u8 *object = ObjectTable_Get(arg0);
    if (object != NULL) {
        *(s16 *)(object + 0x64) = arg1;
        Object_SetCallback(object, gRomSetAngleToward);
        Battle_WaitMode0(arg2);
    }
}

/* object/motion/act/set_action_variant.c */
void Motion_SetActionVariant(s32 arg0, s32 arg1)
{
    u32 object_address;
    u8 *object_state;
    s32 variant_bits;
    s32 state_mask;
    s32 variant_mask;

    variant_mask = 3;
    object_address = (u32)ObjectTable_Get(arg0);
    if (object_address != 0 && (0xF & *(volatile s8 *)(object_address + 0x54)) == 1) {
        variant_bits = (variant_mask & arg1) * 4;
        state_mask = -0xD;
        object_state = *(u8 **)(object_address + 0x50);
        object_state[9] = (state_mask & object_state[9]) | variant_bits;
        object_state[0x15] = (state_mask & object_state[0x15]) | variant_bits;
        *(u8 *)(object_address + 0x23) = 0xFE & *(u8 *)(object_address + 0x23);
    }
}

/* object/visual/copy_attributes.c */
extern s32 Object_GetById(u32);

void ObjectVisual_CopyAttributes(u32 arg0, u32 arg1)
{
    void *p;
    u8 flags;
    u32 shape;
    u32 dstAttr;
    u32 merged;

    p = (void *)Object_GetById(arg1);
    p = *(void **)((u8 *)p + 0x50);
    flags = *(u8 *)((u8 *)p + 0x1C);
    shape = *(u16 *)((u8 *)p + 0x8);

    p = (void *)Object_GetById(arg0);
    p = *(void **)((u8 *)p + 0x50);
    dstAttr = *(u16 *)((u8 *)p + 0x8);
    *(u8 *)((u8 *)p + 0x1C) = flags;
    shape <<= 22;
    shape >>= 22;
    merged = 0xfffffc00;
    merged &= dstAttr;
    merged |= shape;
    *(u16 *)((u8 *)p + 0x8) = merged;
}

void ObjectVisual_ReservedNoOp2B90(void)
{
}

/* event/set_value_1d8.c */
struct EventRuntime1d8 {
    u8 unknown_000[0x1d8];
    s16 value;
};

void Event_SetValue1d8(s16 value)
{
    ((struct EventRuntime1d8 *)gWork)->value = value;
}

/* object/table/read_active_value.c */
/* object/table/read_active_value.c */
struct ObjectValueSource {
    u8 unknown_000[40];
    const s16 *value;
};

struct ObjectTableEntry {
    u8 unknown_000[80];
    struct ObjectValueSource *value_source;
    u8 active;
};

struct ObjectTableState {
    u8 unknown_000[20];
    struct ObjectTableEntry *objects[4096];
};

#define OBJECT_08092BA8_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))
typedef char ObjectValueSource_value_offset[
    OBJECT_08092BA8_OFFSET(struct ObjectValueSource, value) == 0x28
        ? 1 : -1
];
typedef char ObjectTableEntry_value_source_offset[
    OBJECT_08092BA8_OFFSET(struct ObjectTableEntry, value_source) == 0x50 ? 1 : -1
];
typedef char ObjectTableEntry_active_offset[
    OBJECT_08092BA8_OFFSET(struct ObjectTableEntry, active) == 0x54 ? 1 : -1
];
typedef char ObjectTableState_objects_offset[
    OBJECT_08092BA8_OFFSET(struct ObjectTableState, objects) == 0x14 ? 1 : -1
];

extern struct ObjectTableState *gWork;

s32 ObjectTable_ReadActiveValue(s32 key)
{
    s32 result = -1;
    struct ObjectTableEntry *entry =
        gWork->objects[(u32)key & 0x0fff];

    if (entry != 0 && entry->active == 1) {
        result = *entry->value_source->value;
    }
    return result;
}

/* object/table/find_active_by_value.c */
struct Inner {
    u8 unknown_000[40];
    s16 *value;
};

struct Object_08092be0 {
    u8 unknown_000[80];
    struct Inner *inner;
    u8 active;
};

/*
 * This owner's view of gWork. games/gs1/include/battle_effect_runtime.h declares
 * the same global as `struct BattleRuntime` with a different layout; both are
 * per-owner views of one object and only the fields each owner reads are
 * evidence. Named for the owner so the two cannot be mistaken for one type.
 */
struct Work_08092be0 {
    u8 unknown_000[20];
    struct Object_08092be0 *objects[4096];
};

extern struct Work_08092be0 *gWork;

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct Work_08092be0 *state = gWork;
    s32 result = -1;
    s32 index = 8;
    struct Object_08092be0 *object = state->objects[index];

    if (object != 0 && object->active == 1 && *object->inner->value == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 65) {
            object = state->objects[index];
            if (object == 0 || object->active != 1 ||
                *object->inner->value != value) {
                goto next;
            }
            result = index;
        }
    }
    return result;
}

/* battle/ev_run_wait.c */
/* battle/presentation/act/run_wait.c */
extern volatile u32 gIwEvWait;

void BattleEv_RunWait(s32 action)
{
    u8 *runtime = *(u8 **)0x03001ebc;
    s32 wait_token = Battle_RunEvWait();
    s32 resolved_action;
    u32 frames = 0;

    WaitFrames(1);
    resolved_action = read_active_value(action);
    if (action <= 7) {
        s32 masked_action = action & 0x0fff;

        if (BattleAction_FindDescriptor(masked_action) == 0) {
            resolved_action = masked_action;
        }
    }
    Battle_unk5_2(resolved_action);

    if (*(s32 *)(runtime + 0x1cc) == 0) {
        while (Battle_unk6_2(wait_token) == 0) {
            WaitFrames(1);
            frames++;
            if (frames > 600 ||
                ((gIwEvWait & 4) && (gIwEvWait & 0x100) &&
                 (gIwEvWait & 0x200) && (gIwEvWait & 1))) {
                UiWork_FinalizePending();
            }
        }
    }

    WaitFrames(1);
}

/* battle/presentation/act/run_wait_mode0.c */
void Battle_WaitMode0(s32 arg0);
/* 入力r0/r1をそのまま渡すため、引数型は意図的に省略する。 */

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
    Battle_Check();
    Battle_WaitMode0(value);
}

/* battle/event_runtime/process_action.c */
extern u8 gCell;

s32 BattleEventRuntime_ProcessAction(s32 object_id, s32 action_id)
{
    s32 result;
    u8 *runtime;
    u8 *global_table;

    Battle_Check(object_id);
    global_table = &gCell;
    result = Battle_ApplyEvWait(*(void **)(global_table + 500), 0);
    if (result == 0) {
        Battle_Apply2(object_id, action_id);
        runtime = *(u8 **)ADDR_03001EBC;
        *(u16 *)(runtime + 472) += 1;
    } else {
        runtime = *(u8 **)ADDR_03001EBC;
        *(u16 *)(runtime + 472) += 1;
        Battle_Apply2(object_id, action_id);
    }
    return result;
}

void BattleEventRuntime_NoOpCallback(void)
{
}

/* object/table/call_refresh_hook.c */
/* object/table/call_refresh_hook.c */
void ObjectTable_CallRefreshHook(void)
{
    Obj_CheckCallRefreshHook();
}

/* event/show_value_1d8_at_position.c */
struct State_08093168 {
    u8 padding[472];
    s16 counter;
};

extern struct State_08093168 *gWork;
extern s32 UiWork_Create(s32, s32, s32, s32);

extern void WaitFrames(s32);

void Event_ShowValue1d8AtPosition(s32 unused0, s32 unused1, s32 x, s32 y)
{
    s32 x0 = x;
    struct State_08093168 *state = gWork;
    s32 py = y;
    s32 px = x0;
    s32 min_x = 8;
    s32 min_y = 20;
    s32 ret;

    if (py > 119)
        py += 32;
    else
        py -= 32;

    if (x0 < min_x)
        px = min_x;
    if (px > 312)
        px = 312;
    if (py < min_y)
        py = min_y;
    if (py > 220)
        py = 220;

    ret = UiWork_Create(state->counter, px, py, 1);
    while (Sys_Check(ret) == 0)
        WaitFrames(1);
    state->counter++;
}

/* object/table/run_if_active.c */
s32 ObjectTable_ReadActiveValue();

void ObjectTable_RunIfActive(void)
{
    if (ObjectTable_ReadActiveValue() != -1) {
        Obj_CheckIfActive();
    }
}

/* battle/show_paired_unit_work_and_wait.c */
struct Runtime_080931ec {
    u8 unknown_000[0x1d8];
    s16 effect_count;
};

extern struct Runtime_080931ec *gWork;
extern volatile u32 gIw;

s32 ObjectTable_ReadActiveValue(s32);

s32 UiWork_Create(s32, s32, s32, s32);

s32 UiWork_IsCompleteFar(void);

void UiWork_FinalizePending(void);
void WaitFrames(s32);

void Battle_ShowPairedUnitWorkAndWait(
    s32 first, s32 first_x, s32 first_y, s32 first_arg,
    s32 first_extra, s32 second, s32 second_x, s32 second_y,
    s32 second_arg, s32 second_extra)
{
    struct Runtime_080931ec *rt = gWork;
    s32 id0 = ObjectTable_ReadActiveValue(first);
    s32 id1 = ObjectTable_ReadActiveValue(second);
    s32 h0;
    s32 h1;

    h0 = UiWork_Create(
        rt->effect_count++, first_x, first_y,
        Battle_CheckCallRefreshHook(id0) << 16);
    Battle_SetMode(id0, 0, first_arg, first_extra);

    h1 = UiWork_Create(
        rt->effect_count++, second_x, second_y,
        Battle_CheckCallRefreshHook(id1) << 16);
    Battle_SetMode(id1, 0, second_arg, second_extra);

    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);

    WaitFrames(1);
    while ((gIw & 0x303) == 0)
        WaitFrames(1);

    WaitFrames(1);
    Battle_Do(id0);
    Battle_Do(id1);
    UiWork_FinalizePending();
    WaitFrames(1);

    while (Battle_unk2(h0) == 0)
        WaitFrames(1);
    while (Battle_unk2(h1) == 0)
        WaitFrames(1);

    WaitFrames(1);
}

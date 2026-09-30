#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"

struct BattleEventState {
    u8 padding[0xcc8];
    s16 queued_sound;
};

extern struct BattleEventState *gEventWork;
void Audio_PlayCue(s32);

void ObjectMotion_SetActionVariant(u32, s32);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32, s32, s32);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *, const void *);
void ObjectDispatch_ApplyValueToChildrenFar(struct ObjectRuntime *, s32);
s32 Map_GetTerrainHeightFar(u8, s32, s32);
void Battle_WaitMode0(s32);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void ObjectDispatch_WaitForValue16Far(struct ObjectRuntime *);
void ObjectMotion_SetActionCallback(struct ObjectRuntime *, s32);
extern const u8 ObjectMotion_LinkedActionScript[];
extern s32 gGameState[];
extern u8 ObjectMotion_LaunchScript;
extern const u8 ObjectMotion_VariantScripts[];

void WaitFrames(s32);
#define FIELD_S32(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define OBJECT_X(object) FIELD_S32(object, 0x08)
#define OBJECT_Y(object) FIELD_S32(object, 0x0C)
#define OBJECT_Z(object) FIELD_S32(object, 0x10)
#define OBJECT_TARGET_Y(object) FIELD_S32(object, 0x14)
#define OBJECT_VERTICAL_STEP(object) FIELD_S32(object, 0x28)
#define OBJECT_MIRRORED_Y(object) FIELD_S32(object, 0x3C)

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

extern struct Object_08092624 *Object_CreateFar(s32, s32, s32, s32);
extern void ObjectGroup_SetChildValue(struct Object_08092624 *);
extern s32 Math_ModU(s32, s32);
extern const u8 BattleFx_BurstParticleScriptA[];
extern const u8 BattleFx_BurstParticleScriptB[];
void ObjectMotion_ArmCallback(s32 arg0, s32 arg1, s32 arg2);
void BattleFx_PlayQueuedSound(void);
void BattleFx_UpdateParticleLinearMotion(void *particle);

struct ObjectRuntime *Object_GetById(u32 object_id);

void BattleFx_SetQueuedSoundAndPlay(s32 sound_id)
{
    gEventWork->queued_sound = sound_id;
    if ((s16)sound_id == -1) {
        sound_id = 0x121;
    }
    Audio_PlayCue(0x12a);
    Audio_PlayCue(sound_id);
}

void BattleFx_PlayQueuedSound(void)
{
    s16 sound_id = gEventWork->queued_sound;

    if (sound_id != -1)
        Audio_PlayCue(sound_id);
}

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
        ObjectMotion_SetActionCallback(object, action);
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

void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->linked_object = Object_GetById(linked_object_id);
        ObjectMotion_SetActionCallback(object, (s32)ObjectMotion_LinkedActionScript);
    }
}

void Object_RefreshSelectorById(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        ObjectDispatch_WaitForValue16Far(object);
}

void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        ObjectMotion_SetActionCallback(object, action);
        ObjectDispatch_WaitForValue16Far(object);
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

void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct ObjectRuntime *object = ObjectTable_Get(arg0);

    if (object != NULL) {
        struct ObjectRuntime *other;

        ObjectMotion_SetSpeedParameters(arg0, 0x9999, 0x4CCC);
        other = Object_GetById(gGameState[125]);
        if (other != NULL)
            ObjectMotion_SetHorizontalPositionWithTerrain(arg0, other->x, other->z);
        object->movement_state = 0;
        Object_SetMode(object, 2);
        ObjectMotion_OffsetPositionAndResetMotion(arg0, arg1, arg2);
        ObjectDispatch_InitializeFar(object, &ObjectMotion_LaunchScript);
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
            terrain_height = Map_GetTerrainHeightFar(terrain_id, tile_x, tile_z) << 0x10;
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
                Map_GetTerrainHeightFar(terrain_id, tile_x, tile_z >> 0x10) << 0x10;
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
        ObjectDispatch_ApplyValueToChildrenFar(object, action);
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

void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        ObjectDispatch_InitializeFar(object,
            ObjectMotion_VariantScripts + ((3 - variant) << 7));
    }
}

void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant)
{
    ObjectMotion_SetVariantCallback(object_id, variant);
    Object_RefreshSelectorById(object_id);
}

void BattleFx_UpdateParticleLinearMotion(void *particle)
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
      (s32)(velocity_x - Math_Div(velocity_x, 0x12));
 do {
   vz = velocity_z;
   if (velocity_z < 0) {
     vz += 0xF;
   }
   *((s32 *)(((u8 *)particle) + 0x34)) =
       (s32)(velocity_z - (vz >> 4));
 } while (0);
}

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void BattleFx_SpawnBurstParticle(struct Object_08092624 *source, s32 optional)
{
    struct Object_08092624 *object;
    struct Child_08092624 *child;
    s32 value;

    object = Object_CreateFar(222, source->x, source->y, source->z);
    if (object != 0) {
        child = object->child;
        switch (Random16() & 1) {
        case 1:
            Object_SetMode(object, 2);
            ObjectDispatch_InitializeFar(object, BattleFx_BurstParticleScriptA);
            break;
        default:
            Object_SetMode(object, 1);
            ObjectDispatch_InitializeFar(object, BattleFx_BurstParticleScriptB);
            break;
        }

        if (optional != 0)
            ObjectGroup_SetChildValue(object);

        object->mode_55 = 0;
        value = Math_ModU(Random16(), 10) + 5;
        object->field_34 = -0x1999 * value;
        value = Math_ModU(Random16(), 15) - 7;
        value <<= 1;
        object->field_30 = 0x1999 * value;
        object->field_64 = 0;
        object->callback_6c = (void (*)(void))BattleFx_UpdateParticleLinearMotion;
        child->field_26 = 0;
        child->copied_09 = source->child->copied_09;
    }
}

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
        ObjectMotion_ArmCallback(sequence_arg, 0x5000, 0);
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

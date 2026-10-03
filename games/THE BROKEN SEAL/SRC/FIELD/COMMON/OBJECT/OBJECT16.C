#include "TYPES.H"
#include "OBJDISP.H"
#include "OBJECT_RUNTIME.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "GAME_STATE.H"
#include "FIELD_SPRITE.H"
#include "ANIMSPR.H"

struct BattleEventState {
    u8 padding[0xcc8];
    s16 queued_sound;
};

extern struct BattleEventState *gEventWork;
void Audio_PlayCue(s32);

void ObjectMotion_SetActionVariant(u32, s32);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32, s32, s32);
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
extern u8 ObjectMotion_LaunchScript;
extern const u8 ObjectMotion_VariantScripts[];

void WaitFrames(s32);

struct BurstParticle {
    u8 pad_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[28];
    s32 velocity_x;
    s32 velocity_z;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[12];
    struct FieldSprite *child;
    u8 pad_54;
    u8 mode_55;
    u8 pad_56[14];
    u16 field_64;
    u8 pad_66[6];
    void (*callback_6c)(struct BurstParticle *);
};

extern struct BurstParticle *Object_CreateFar(s32, s32, s32, s32);
void ObjectGroup_SetChildValue(struct DispatchObject *object, s32 value);
extern const u8 BattleFx_BurstParticleScriptA[];
extern const u8 BattleFx_BurstParticleScriptB[];
void ObjectMotion_ArmCallback(s32 arg0, s32 arg1, s32 arg2);
void BattleFx_PlayQueuedSound(void);
void BattleFx_UpdateParticleLinearMotion(struct BurstParticle *particle);

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
        other = Object_GetById(gGameState.selected_actor);
        if (other != NULL)
            ObjectMotion_SetHorizontalPositionWithTerrain(arg0, other->x, other->z);
        object->movement_state = 0;
        Object_SetMode(object, 2);
        ObjectMotion_OffsetPositionAndResetMotion(arg0, arg1, arg2);
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)&ObjectMotion_LaunchScript);
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
    struct AnimationObject *sprite;
    volatile s32 saved;
    s32 i;

    object = ObjectTable_Get(object_id);
    if (object != NULL && object->animation_kind == 1) {
        sprite = object->animation;
        saved = sprite->last_no;
        for (i = 0; i <= 89; i++) {
            WaitFrames(1);
            if (saved != sprite->last_no) {
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
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (u32)(ObjectMotion_VariantScripts + ((3 - variant) << 7)));
    }
}

void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant)
{
    ObjectMotion_SetVariantCallback(object_id, variant);
    Object_RefreshSelectorById(object_id);
}

void BattleFx_UpdateParticleLinearMotion(struct BurstParticle *particle)
{
  s32 velocity_x;
  s32 x;
  s32 velocity_z;
  s32 z;
  s32 velocity_y;
  s32 vz;
  velocity_x = particle->velocity_x;
  x = (particle->x) + velocity_x;
  particle->x = x;
  particle->target_x = x;
  velocity_z = particle->velocity_z;
  z = (particle->z) + velocity_z;
  particle->z = z;
  particle->target_z = z;
  velocity_y = (particle->y) + 0x400;
  particle->y = velocity_y;
  particle->target_y = velocity_y;
  particle->velocity_x =
      (s32)(velocity_x - velocity_x / 0x12);

  vz = velocity_z;
  if (velocity_z < 0) {
    vz += 0xF;
  }
  particle->velocity_z =
      (s32)(velocity_z - (vz >> 4));

}

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void BattleFx_SpawnBurstParticle(struct BurstParticle *source, s32 optional)
{
    struct BurstParticle *object;
    struct FieldSprite *child;
    s32 value;

    object = Object_CreateFar(222, source->x, source->y, source->z);
    if (object != 0) {
        child = object->child;
        switch (Random16() & 1) {
        case 1:
            Object_SetMode((struct ObjectRuntime *)object, 2);
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_BurstParticleScriptA);
            break;
        default:
            Object_SetMode((struct ObjectRuntime *)object, 1);
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_BurstParticleScriptB);
            break;
        }

        if (optional != 0)
            ObjectGroup_SetChildValue((struct DispatchObject *)object, optional);

        object->mode_55 = 0;
        value = Random16() % 10 + 5;
        object->velocity_z = -0x1999 * value;
        value = Random16() % 15 - 7;
        value <<= 1;
        object->velocity_x = 0x1999 * value;
        object->field_64 = 0;
        object->callback_6c = BattleFx_UpdateParticleLinearMotion;
        child->flags = 0;
        child->priority = source->child->priority;
    }
}

void BattleFx_RunRisingObjectSequence(s32 sequence_arg, s32 mode_or_frame, s32 optional_action)
{
    s32 next_y;
    s32 base_z;
    struct ObjectRuntime *object;
    u8 *object_flags;

    object = Object_GetById(sequence_arg);
    base_z = object->z;
    if (object != 0) {
        Audio_PlayCue(0x121);
        Object_SetMode(object, mode_or_frame);
        WaitFrames(10);
        object_flags = &object->flags;
        Object_SetMode(object, 1);
        {
            u8 flags = 2;
            flags |= *object_flags;
            *object_flags = flags;
        }
        object->velocity_y = 0x40000;
        Object_SetPosition(object, object->x, object->y,
            base_z + 0xC0000);
        WaitFrames(6);
        Audio_PlayCue(0xD9);
        mode_or_frame = 0;
        ObjectMotion_ArmCallback(sequence_arg, 0x5000, 0);
        *object_flags = 0;
        do {
            next_y = object->y + 0xFFFE0000;
            object->y = next_y;
            object->target_y = next_y;
            WaitFrames(1);
            if ((optional_action != -1) && (mode_or_frame & 1)) {
                BattleFx_SpawnBurstParticle((struct BurstParticle *)object, optional_action);
            }
            mode_or_frame++;
        } while ((u32)mode_or_frame <= 0xD);
        *object_flags = 3;
        object->velocity_y = 0x30000;
        Object_SetPosition(object, object->x, object->y,
            base_z + 0x100000);
        Object_CommitPosition(object);
        mode_or_frame = 0;
        if (object->y > object->terrain_height) {
wait_for_target_y:
            WaitFrames(1);
            mode_or_frame++;
            if ((u32)mode_or_frame <= 0xB3) {
                if (object->y > object->terrain_height) {
                    goto wait_for_target_y;
                }
            }
        }
        WaitFrames(2);
        BattleFx_PlayQueuedSound();
    }
}

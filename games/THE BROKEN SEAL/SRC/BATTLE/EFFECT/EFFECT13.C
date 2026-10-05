#include "MOTION_OBJECT.H"
#include "FX_SCENE.H"
#include "FIXED_MATH.H"
#include "OBJDISP.H"
#include "EFFECT_SLOT.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "ANIMSPR.H"
#include "GAME_STATE.H"
#include "OBJECT_EFX.H"
#include "SOUND_IDS.H"
#include "GLOBAL_CELLS.H"
#include "IWRAM_CALL.H"
#include "IO_REG.H"

u32 BattleFx_HasReachedTarget(struct EffectSlot *);

struct Output_08097f80 {
    s32 x;
    s32 y;
    s32 z;
};

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Vector_AddPolarOffset(s32, s32, struct Output_08097f80 *);


extern struct BattleFxScene *gEffectWork;
void BattleEffect_InitializeSharedScene(void);
void *BattleFx_StartItemBreak(void *object);
void BattleFx_SnapScaleToFull(struct MotionObject *object);
void Object_SetMode(void *object, s32 mode);
void BattleFx_PrepareBufferInterpolation(void);
void UpdateRisingParticleBurst(void *object);

extern void *Object_Spawn(s32, s32, s32, s32);
extern void Motion_SetTargetPositionFromMagnitudeAngle(void *object, s32 magnitude, s32 angle);
extern void Object_SetMode(void *, s32);
extern void BattleFx_UpdateItemBreakFragment(void *);

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
extern void Audio_PlayCue(s32);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
s32 Object_CommitPosition();

extern void Object_Destroy(void *object);

extern u8 gObjectSlots[];

extern u8 *gEventWork;

struct FxObject {
    u8 unknown_00[8];
    s32 x;                      /* 0x08 */
    s32 y;
    s32 z;
    s32 ground;                 /* 0x14 */
    u8 unknown_18[10];
    u8 mode;                    /* 0x22 */
    u8 unknown_23;
    s32 velocity_x;             /* 0x24 */
    s32 velocity_y;             /* 0x28 */
    s32 velocity_z;             /* 0x2c */
    s32 speed;                  /* 0x30 */
    s32 lift;                   /* 0x34 */
    u8 unknown_38[12];
    s32 gravity;                /* 0x44 */
    u8 unknown_48[13];
    u8 flag;                    /* 0x55 */
    u8 unknown_56[4];
    u8 unknown_5a;
    u8 unknown_5b;
    u8 unknown_5c[12];
    struct FxObject *linked;    /* 0x68 */
    void *callback;             /* 0x6c */
};

extern const u8 BattleFx_SourceHoldScript[];
extern const u8 Data_0809f118[];
extern volatile u32 gKeysHeld;
extern volatile u32 gKeyState;
extern volatile u32 gFrameCount;

void WaitFrames(s32 frames);
void Object_SetMoveTargetFar(struct FxObject *object, s32 x, s32 y, s32 z);
void ObjectGroup_ApplyRandomChildValues(void);
void AudioCommand_PlayFar(s32 cue);
s32 BattleFx_GetCycledTableWord(u32 keys);
struct FxObject *ScriptObject_FindOverlappingEntryFar(struct FxObject *object, struct Output_08097f80 *position);
s32 Object_CheckMovementCollision(struct FxObject *object, struct Output_08097f80 *position);
s32 Map_GetCellAttributeLowNibbleFar(s32 layer, s32 x, s32 z);
struct FxObject *Object_GetById(s32 id);
void Animation_ApplyChildValuesFar(struct FxObject *object, s32 value);
void EffectRuntime_StopCurrentObject(void);

/* Lets the player push the target one step: a preview object shows the step
   tried, and a step that fits moves the target, with the source object too
   when the source stands in the way and its next cell is free. */
void BattleFx_SteerPushedTarget(s32 target_id)
{
    struct Output_08097f80 position;
    struct Output_08097f80 nearby;
    struct BattleFxScene *work;
    struct FxObject *source;
    struct FxObject *target;
    struct FxObject *preview;
    struct FxObject *hit;
    s32 x;
    s32 z;
    s32 angle;
    s32 moved;
    s32 direction;
    s32 keys;
    s32 cell_x;
    s32 cell_z;
    struct Output_08097f80 *p;

    work = gEffectWork;
    source = work->main_object;
    target = work->child;
    angle = work->angle + 0x8000;
    moved = 0;
    if (target == NULL)
        return;
    BattleEffect_InitializeSharedScene();
    source->linked = target;
    ObjectDispatch_InitializeFar((struct DispatchObject *)source, (u32)BattleFx_SourceHoldScript);
    preview = BattleFx_StartItemBreak(source);
    if (preview == NULL) {
        BattleFx_PrepareBufferInterpolation();
        return;
    }
    preview->linked = target;
    p = &position;
    p->x = target->x;
    p->y = target->y + 0x100000;
    p->z = target->z;
    Vector_AddPolarOffset(0x100000, angle, p);
    Object_SetMoveTargetFar(preview, p->x, p->y, p->z);
    BattleFx_SnapScaleToFull((struct MotionObject *)preview);
    preview->speed = 0x40000;
    preview->lift = 0x8000;
    preview->flag = 4;
    target->callback = ObjectGroup_ApplyRandomChildValues;
    target->speed = 0x6666;
    target->lift = 0x3333;
    target->unknown_5a = moved;
    target->mode = 2;
    for (;;) {
        WaitFrames(1);
        keys = gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS);
        if (keys != 0)
            break;
        direction = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
        if (direction == 0xffff) {
            position.x = target->x;
            position.y = target->y + 0x100000;
            position.z = target->z;
            Vector_AddPolarOffset(0x100000, angle, &position);
            Object_SetMoveTargetFar(preview, position.x, position.y, position.z);
            Object_SetMode(preview, 1);
            preview->velocity_x = 0;
            preview->velocity_y = 0;
            preview->velocity_z = 0;
            continue;
        }
        position.x = target->x;
        position.y = target->y + 0x100000;
        position.z = target->z;
        Vector_AddPolarOffset(0x100000, angle, &position);
        Vector_AddPolarOffset(0x20000, direction, &position);
        Object_SetMoveTargetFar(preview, position.x, position.y, position.z);
        Object_CommitPosition(preview);
        position.x = target->x;
        position.y = target->y;
        position.z = target->z;
        Vector_AddPolarOffset(0x100000, direction, &position);
        nearby.x = target->x;
        nearby.y = target->y;
        nearby.z = target->z;
        Vector_AddPolarOffset(0x200000, direction, &nearby);
        if (Object_CheckMovementCollision(target, &position) > 0) {
            Object_SetMode(preview, 4);
            if ((gFrameCount & 15) == 0)
                AudioCommand_PlayFar(114);
            continue;
        }
        hit = ScriptObject_FindOverlappingEntryFar(target, p);
        if (hit == NULL)
            goto move;
        if (hit != source) {
            Object_SetMode(preview, 4);
            if ((gFrameCount & 15) == 0)
                AudioCommand_PlayFar(114);
            continue;
        }
        cell_x = source->x & 0xfff00000;
        cell_z = source->z & 0xfff00000;
        if (cell_x == (p->x & 0xfff00000) && cell_z == (p->z & 0xfff00000)) {
            Object_SetMode(preview, 4);
            if ((gFrameCount & 15) == 0)
                AudioCommand_PlayFar(114);
            continue;
        }
        if (cell_x != (nearby.x & 0xfff00000) || cell_z != (nearby.z & 0xfff00000))
            goto move;
        if (Map_GetCellAttributeLowNibbleFar(source->mode, nearby.x, nearby.z) != 0) {
            Object_SetMode(preview, 4);
            if ((gFrameCount & 15) == 0)
                AudioCommand_PlayFar(114);
            continue;
        }
        moved = 1;
    move:
        AudioCommand_PlayFar(175);
        x = position.x;
        z = position.z;
        Object_SetMode(preview, Data_0809f118[(u16)(angle - direction) >> 14]);
        WaitFrames(15);
        target->unknown_5b = 0;
        target->speed = 0x3333;
        target->lift = 0x3333;
        Object_SetMoveTargetFar(target, position.x, position.y, position.z);
        preview->flag = 0;
        preview->speed = 0x3333;
        preview->lift = 0x3333;
        Vector_AddPolarOffset(0x100000, direction, &position);
        Object_SetMoveTargetFar(preview, position.x, position.y + 0x100000, position.z);
        if (moved == 1) {
            Object_GetById(work->first_object_id)->unknown_5a &= 0xfe;
            source->speed = 0x3333;
            source->lift = 0x3333;
            Object_SetMoveTargetFar(source, nearby.x, nearby.y, nearby.z);
        }
        Object_CommitPosition(target);
        target->x = x;
        target->z = z;
        target->velocity_x = 0;
        target->velocity_z = 0;
        break;
    }
    Animation_ApplyChildValuesFar(target, work->saved_palette);
    ObjectDispatch_InitializeFar((struct DispatchObject *)target, (u32)(work->saved_script));
    target->callback = work->saved_callback;
    EffectRuntime_StopCurrentObject();
    if (moved == 1)
        Object_GetById(work->first_object_id)->unknown_5a |= 1;
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(preview);
}

void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *effect)
{
    struct Output_08097f80 position;
    s8 *state_pointer = &effect->state;
    s32 state;

next_state:
    state = *state_pointer;
    if (state == 0) {
        u32 angle;

        position.x = effect->origin_x;
        position.z = effect->origin_z;
        angle = Random16();
        Vector_AddPolarOffset(0x1e0000, (u16)angle, &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x40000;
        effect->max_speed = 0x40000;
        effect->flag42 = state;
        (*state_pointer)++;
        return;
    }

    if (state == 1) {
        if (BattleFx_HasReachedTarget(effect) == 0) {
            (*state_pointer)++;
            goto next_state;
        }
        return;
    }

    if (state == 2) {
        effect->target_x = effect->origin_x;
        effect->target_z = effect->origin_z;
        {
            u32 value = 0x400;

            effect->max_turn_step = value;
        }
        effect->flag42 = 1;
        (*state_pointer)++;
        return;
    }

    if (state == 3 && BattleFx_HasReachedTarget(effect) == 0)
        BattleFx_ClearOwnedSlot(effect);
}

void BattleFx_RunItemBreakSequence(void)
{
    s32 work[3];
    struct BattleFxScene *scene;
    void *object;

    scene = gEffectWork;
    object = scene->main_object;

    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        BattleEffect_InitializeSharedScene();
    } while (0);
    object = BattleFx_StartItemBreak(object);
    BattleFx_SnapScaleToFull(object);
    if (object != 0) {
        Object_SetMode(object, 4);
        WaitFrames(30);
    }
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(object);
}

void *BattleFx_StartItemBreak(void *object)
{
    struct MotionObject *source = object;
    struct MotionObject *parent;
    struct MotionObject *child;
    s32 angle;
    s32 distance;
    s32 speed;
    s32 i;
    u32 spread;
    u32 jitter;

    angle = (source->angle + 0x2000) & 0xc000;
    parent = Object_Spawn(0xd7, source->x, source->y + (16 << 16), source->z);
    if (parent == 0)
        return 0;
    parent->scale_y = 0x4000;
    parent->scale_x = 0x4000;
    parent->update = BattleFx_UpdateItemBreakFragment;
    parent->speed_limit = 0x20000;
    parent->acceleration = 0x20000;
    parent->motion_flags = 0;
    Object_SetMode(parent, 3);
    Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

    for (i = 7; i >= 0; i--) {
        child = Object_Spawn(0x11d, source->x, source->y + (16 << 16), source->z);
        if (child != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)child, (u32)&BattleFx_FragmentScript);
            speed = Random16() + 0x10000;
            child->acceleration = 0x10000;
            child->speed_limit = speed;
            child->motion_flags = 2;
            child->vertical_motion_strength = 0x51e;
            spread = Random16();
            child->velocity_y = spread - Random16();
            distance = Random16() * 24 + (8 << 16);
            jitter = Random16();
            Motion_SetTargetPositionFromMagnitudeAngle(child, distance,
                                                       ((jitter - Random16()) >> 3) + source->angle);
        }
    }
    Audio_PlayCue(SOUND_ITEM_BREAK);
    return parent;
}

void BattleFx_SnapScaleToFull(struct MotionObject *obj)
{
    s32 next;
    s32 scale;

    if (obj != NULL) {
        scale = obj->scale_x;
        if (scale <= 0xFFFF) {
            do {
                next = scale + 0x1000;
                scale = next;
            } while (next <= 0xFFFF);
            obj->scale_x = next;
            obj->scale_y = next;
        }
        Object_CommitPosition();
    }
}

/* UpdateRisingParticleBurst: lift and spin the source for 31 frames, then
   burst eight item-break fragments with random speeds and headings. */
void UpdateRisingParticleBurst(void *object)
{
    struct MotionObject *source = object;
    struct MotionObject *child;
    s32 i;
    s32 distance;
    u32 spread;
    /* FAKEMATCH: holds 0x10000 in sl across the burst loop; a plain local folds to the constant. */
    register s32 unit asm("sl");

    Audio_PlayCue(154);
    for (i = 30; i >= 0; i--) {
        source->y += 1 << 16;
        source->angle += 0x2000;
        source->scale_x += -0x800;
        source->scale_y += -0x800;
        WaitFrames(1);
    }
    i = 7;
    unit = 0x10000;
    for (; i >= 0; i--) {
        child = Object_Spawn(0x11d, source->x, source->y, source->z);
        if (child != 0) {
            s32 speed;

            ObjectDispatch_InitializeFar((struct DispatchObject *)child, (u32)&BattleFx_FragmentScript);
            speed = Random16();
            child->acceleration = unit;
            child->speed_limit = speed + unit;
            child->motion_flags = 2;
            child->vertical_motion_strength = 0xa3d;
            spread = Random16();
            child->velocity_y = spread - Random16();
            distance = Random16() * 24;
            distance += 8 << 16;
            Motion_SetTargetPositionFromMagnitudeAngle(child, distance, Random16());
        }
    }
    Audio_PlayCue(131);
    Object_Destroy(source);
}

/* Sets the draw parameter of every live object whose first animation is
   entry 200, and marks its sprite for redrawing. */
void ObjectDispatch_ApplyValueToKind200Children(s32 value)
{
    struct ObjectRuntime *object = *(struct ObjectRuntime **)gObjectSlots;
    s32 count = 63;

    do {
        if (object->script != NULL) {
            u8 kind = object->animation_kind;

            if (kind == 1) {
                struct AnimationObject *animation = object->animation;
                struct AnimationEntry *entry = animation->entries[0];

                if (entry->anim_id == 200) {
                    entry->param = value;
                    animation->dirty = kind;
                }
            }
        }
        count--;
        object++;
    } while (count >= 0);
}

/* Counts down the scene timer while it runs and raises request 0x2090 once
   the leader strays 60 units from the anchor or the timer runs out. */
void FieldEffect_WatchLeaderDistance(void)
{
    u8 *work;
    struct ObjectRuntime *actor;
    s16 *timer;
    s32 dx;
    s32 dz;

    work = gEventWork;
    actor = ObjectTable_Get(gGameState.selected_actor);
    if (*(s16 *)(work + 0xcc0) != 0) {
        timer = (s16 *)(work + 0xcba);
        if (*timer != 0)
            (*timer)--;
    }
    dx = Iwram_MulQ16(*(s16 *)(work + 0xcbc) - actor->x / 0x10000, 0xd105);
    dz = *(s16 *)(work + 0xcbe) - (actor->z - actor->y) / 0x10000;
    if (dx * dx + dz * dz >= 3600 || *(s16 *)(work + 0xcba) == 0) {
        u16 *slot = (u16 *)(work + 382);
        u32 request = 0x2090;

        *slot = request;
    }
}

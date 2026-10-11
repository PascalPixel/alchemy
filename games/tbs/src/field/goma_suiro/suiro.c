#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#include "fieldobj.h"
#include "types.h"
#include "configured_effect_spawn.h"
#include "field_event.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "field_scene.h"
#include "scene_ids.h"
#include "call.h"
#include "fixed_math.h"

typedef struct {
    u8 pad_to_angle[30];
    u16 angle;
} EffectRecord_020004c4;

typedef struct {
    u8 pad_to_record[80];
    EffectRecord_020004c4 *record;
} EffectWork_020004c4;

void Engine_TaskWait(s32 frames);

void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(void *object);

union MotionWork {
    struct {
        u32 unk_00[2];
        s32 x, y, z;
        u32 unk_14;
        s32 accum_x, accum_y;
        u32 unk_20[4];
        s32 rate_x, rate_y;
        u32 unk_38[3];
        s32 velocity_x, velocity_y, velocity_z;
        u16 *record;
        u8 unk_54[16];
        u16 angle_step;
    } fields;
    u8 bytes[102];
};

void SceneEffect_SetRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *obj = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        struct FieldSprite *rec = obj->sprite;

        rec->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 14);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *result = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        struct FieldSprite *object = result->sprite;

        object->priority = 1;
        result->motion_flags = 0;
        result->collision_flags = 8;
        Engine_ActorSetSpriteFlags(result, 0);
        ObjectGroup_SetChildValue(result, 15);
        result->priority_flags = (result->priority_flags & 0xfe) | 2;
        return result;
    }
    return NULL;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroEntrances2;
    }
    return gGomaSuiroEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return GomaSuiro_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroPlacements2;
    }
    return gGomaSuiroPlacementsOther;
}

void FieldScene_RunActor8AtCell24Sequence(void)
{
    s32 *record;
    struct FieldActor *target;
    s32 value;

    record = (s32 *)Object_GetById(8);
    value = record[2] / 0x100000;
    if (value == 24) {
        SceneEffect_RunActorBurst(8);
        Object_GetById(8)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(19, 74, 9, 3, 19, 17);
        target = Object_GetById(8);
        Engine_ActorSetSpriteFlags((s32)target, 0);
        ((void (*)())Engine_GameFlagSet)(0x864);
    }
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroEvents2;
    }
    return gGomaSuiroEventsOther;
}

s32 FieldScene_PlaceActor8OnEntry(void)
{
    struct FieldActor *record;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_GomaSuiro1) {
        if (gGameState.entrance == 5) {
            Engine_GameFlagClear(0x12f);
        } else {
            Object_GetById(8)->collision_flags |= 16;
            if (Engine_GameFlagIsSet(0x864) != 0) {
                Engine_ActorSetPosition(8, 0x15a0000, 0x1240000);
                record = Object_GetById(8);
                Engine_ActorSetSpriteFlags(record, 0);
                *((u8 *)Object_GetById(8) + 35) |= 2;
                Engine_ActorSetAnimation(8, 2);
                Map_CopyCellAttributes(19, 74, 9, 3, 19, 17);
            }
        }
    }
    return 0;
}

void SceneEffect_RotateRecord(EffectWork_020004c4 *work)
{
    EffectRecord_020004c4 *record = work->record;

    record->angle -= 0x800;
}

void SceneActor_WaitObjectBelowHeight(u8 *object, s32 height)
{
    s32 frames = 60;

    while (frames != 0) {
        Engine_TaskWait(1);
        frames--;
        if (*(s32 *)(object + 12) <= height)
            break;
    }
}

union SceneActor {
    struct {
        u32 unk_00[2];
        s32 x, y, z;
        u32 unk_14[3];
        u8 unk_20[2];
        u8 field_22;
        u8 unk_23[5];
        s32 field_28;
        u32 unk_2c[3];
        u32 field_38;
        u32 unk_3c[3];
        s32 velocity_y;
        u32 unk_4c;
        struct Sprite *sprite;
        u8 unk_54;
        u8 mode;
        u8 unk_56[22];
        u32 callback;
    } fields;
    u8 bytes[112];
};
struct Vector { s32 x, y, z; };

void Engine_ActorWaitForMove(s32);
void Engine_AudioPlayCue(s32);
void Effect_Spawn();
void Engine_ActorSetAnimation(s32, s32);

void SceneEffect_RunActorBurst(s32 no)
{
    union SceneActor *work;
    u32 cnt;
    struct Vector vec;
    union {
        struct ConfiguredEffectOptions fields;
        u8 bytes[sizeof(struct ConfiguredEffectOptions)];
    } opt;

    work = (union SceneActor *)Object_GetById(no);
    work->fields.mode = 0;
    for (cnt = 0; cnt < 18; cnt++) {
        Engine_TaskWait(1);
        work->fields.sprite->angle -= 256;
        work->fields.x -= Trig_Cos(work->fields.sprite->angle) / 2;
        work->fields.field_38 = 0x80000000;
    }
    work->fields.callback = (u32)SceneEffect_RotateRecord;
    Actor_SetSpeed(no, 0x30000, 0x18000);
    Actor_SetDestination(no, 376, 288);
    work->fields.velocity_y = 0xcccc;
    work->fields.mode = 3;
    work->fields.field_22 = 0;
    Engine_ActorWaitForMove(no);
    SceneActor_WaitObjectBelowHeight(work, 0);
    Engine_AudioPlayCue(188);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Engine_AudioPlayCue(141);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    for (cnt = 0; cnt < 17; cnt++) {
        vec.x = Trig_Cos(cnt << 12);
        vec.y = 0;
        vec.z = Trig_Sin(cnt << 12);
        vec.x -= vec.x / 4;
        vec.z -= vec.z / 2;
        Effect_Spawn(work->fields.x, work->fields.y, work->fields.z,
                     vec.x, vec.y, vec.z, 0, NULL);
    }
    work->fields.field_28 = 0x50000;
    Actor_SetDestination(no, 346, 292);
    Engine_ActorWaitForMove(no);
    SceneActor_WaitObjectBelowHeight(work, 0);
    work->fields.callback = 0;
    work->fields.sprite->angle = 0x1000;
    opt.fields.kind = 214;
    opt.fields.accum18 = 0x8000;
    opt.fields.accum1c = 0xcccc;
    opt.fields.target30 = 0x18000;
    opt.fields.target34 = 0x13333;
    Effect_Spawn(work->fields.x, work->fields.y, work->fields.z, 0, 0, 0, 0x1c0000, &opt.fields);
    Engine_AudioPlayCue(154);
    Engine_ActorSetAnimation(no, 3);
    Map_WaitWorkValuesBelow256();
}

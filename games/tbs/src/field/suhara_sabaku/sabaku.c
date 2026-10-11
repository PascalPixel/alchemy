#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fx_scene.h"
#include "global_cells.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "call.h"
#include "field_effect.h"
#include "scene_ids.h"
#include "text/msg_ids.h"

#define NULL ((void *)0)

extern u8 gEffectWork[];

struct Selection_02000400 {
    u8 reserved000[500];
    s32 actor_id;
};

struct Actor_02000400 {
    u8 reserved00[6];
    u16 presentation;
};

struct SceneWork_02000400 {
    u8 reserved000[386];
    u16 state_one_marker;
};

extern u8 gSuharaSabakuExits[];
extern u8 gSuharaSabakuActor12Action[];

extern u16 gSuharaSabakuShownLevel;
s32 GameFlag_GetByte();
void GameFlag_SetByte();
void BattleFx_SetPhaseRequest();
void BattleFx_EmitRandomParticle();

#define ACTOR_ID 13

#define EncounterPalette (*(volatile u16 *)0x0500019e)

void SetEffectRecordMode(struct FieldActor *work, s32 mode);

void *OverlayObject_CreateAndInitialize(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void SceneState_SendRequest15With45(void);

s32 SuharaSabaku_ClearActorSpriteAndCollisionFlags(u8 *obj);

s32 SuharaSabaku_ToggleActorActive(u8 *obj);

s32 SceneData_ReturnZero(void);

u8 *SuharaSabaku_GetExits(void);

void SuharaSabaku_SyncSelectedActorProgress(void);

void FieldScene_RunMiddleAuxiliarySequence(s32 a0);

void FieldScene_RunActor8Step(void);

void FieldScene_RunActor9Step(void);

void FieldScene_RunActor10Step(void);

void FieldScene_RunActor11Step(void);

void FieldScene_RunActor12Step(void);

void FieldScene_RunLateActor8Step(void);

void FieldScene_RunLateActor9Step(void);

void FieldScene_RunLateActor10Step(void);

void FieldScene_RunLateActor11Step(void);

void FieldScene_RunLateActor12Step(void);

void FieldScene_RunActorThirteenRestoration(void);

void PlaceActorTwelveWhenFlagClear(void);

void SuharaSabaku_EnableEffectChildOption(void);

void EncounterPalette_Pulse(void);

s32 FieldScene_RunOpeningAuxiliarySequence(void);

s32 FieldScene_RunScene3c0SequenceA(void);

void SuharaSabaku_ApplyVisitFlagBlend(void);

void SuharaSabaku_ApplyAltarFlagBlend(void);

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_CreateAndInitialize(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *obj = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        struct FieldSprite *sprite = obj->sprite;

        sprite->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 14);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
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

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawnov.c"

// 戦闘開始の種類 (battle/party/party.c と同じ)

void BattleFx_SetWeightedResult(s32 actor, s32 mode);

void Event_SetDestination(s32 scene, s32 entrance);

struct FieldGlobals {
    u8 *map;
    u8 unknown_04[0x48];
    struct EventWork *event;
};

void QueueSceneSound(s32 cue);

void SceneState_SendRequest15With45(void)
{
    BattleFx_SetPhaseRequest(15, 45);
}

s32 SuharaSabaku_ClearActorSpriteAndCollisionFlags(u8 *obj)
{
    Engine_ActorSetSpriteFlags(obj, 0);
    ((struct FieldActor *)obj)->collision_flags = 0;
    return 0;
}

s32 SuharaSabaku_ToggleActorActive(u8 *obj)
{
    ((struct FieldActor *)obj)->active ^= 1;
    return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_SuharaSabaku1) {
        return gSuharaSabakuEntrances1;
    }
    if (selector == (s32)&SceneId_SuharaSabaku2) {
        return gSuharaSabakuEntrances2;
    }
    if (selector == (s32)&SceneId_SuharaSabaku3) {
        return gSuharaSabakuEntrances3;
    }
    return gSuharaSabakuEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SuharaSabaku_GetExits(void)
{
    return gSuharaSabakuExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_SuharaSabaku1) {
        return gSuharaSabakuPlacements1;
    }
    if (selector == (s32)&SceneId_SuharaSabaku2) {
        return gSuharaSabakuPlacements2;
    }
    if (selector == (s32)&SceneId_SuharaSabaku3) {
        return gSuharaSabakuPlacements3;
    }
    return gSuharaSabakuPlacementsOther;
}

void SuharaSabaku_SyncSelectedActorProgress(void)
{
    struct Actor_02000400 *actor;
    struct SceneWork_02000400 *scene;
    s32 progress;

    actor = Object_GetById(((struct Selection_02000400 *)((s16 *)&gGameState))->actor_id);
    scene = *(struct SceneWork_02000400 **)((u8 *)&gEventWork);
    actor->presentation = (u16)(gFrameCount << 12);

    progress = GameFlag_GetByte(0x210);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (Engine_GameFlagIsSet(0x106) == 0) {
            progress -= 1;
        }
    }
    GameFlag_SetByte(0x210, progress);
}

void FieldScene_RunMiddleAuxiliarySequence(s32 a0)
{
    s32 p10;
    s32 rec2;
    struct FieldActor *rec7;
    struct FieldActor *record;
    u8 *p6;
    u8 *base;

    base = (u8 *)((s16 *)&gGameState);
    p6 = *(u8 **)(base + 500);
    p10 = a0;
    rec7 = Object_GetById((s32)p6);
    Actor_Get(p10);
    rec2 = GameFlag_IsSet(0x20f);
    if (rec2 == 0) {
        Engine_EventBegin();
        Actor_SetAttachedEffect((s32)p6, 0x101);
        Engine_ActorSetAnimation((s32)p6, 9);
        record = Object_GetById(p10);
        if (record != 0) {
            Actor_SetDestination((s32)p6, record->x.part.pixel, record->z.part.pixel);
        }
        Engine_ActorWaitForMove((s32)p6);
        Event_PlayCue(244);
        Engine_TaskAddCallback((s32)SuharaSabaku_SyncSelectedActorProgress, 0xc80);
        rec7->motion_flags = rec2;
        Engine_ObjectSetPosition((s32)rec7, rec7->x.fixed, rec7->y.fixed + 0x200000, rec7->z.fixed);
        Engine_ActorWaitForMove((s32)p6);
        rec7->velocity_y = rec2;
        rec7->motion_flags = 4;
        *(u8 *)(base + 498) = 2;
        GameFlag_Set(0x20f);
        GameFlag_SetByte(0x218, p10);
        GameFlag_SetByte(0x210, 180);
        Engine_EventEnd();
        *(u16 *)(*(u8 **)((u8 *)&gEventWork) + 0x17c) = rec2;
    }
}

void FieldScene_RunActor8Step(void) { FieldScene_RunMiddleAuxiliarySequence(8); }

void FieldScene_RunActor9Step(void) { FieldScene_RunMiddleAuxiliarySequence(9); }

void FieldScene_RunActor10Step(void) { FieldScene_RunMiddleAuxiliarySequence(10); }

void FieldScene_RunActor11Step(void) { FieldScene_RunMiddleAuxiliarySequence(11); }

void FieldScene_RunActor12Step(void) { FieldScene_RunMiddleAuxiliarySequence(12); }

void SuharaSabaku_RestoreActorScaleAndBlend(s32 a0)
{
    s32 i;
    s32 next_frame;
    s32 p10;
    s32 p10b;
    /* FAKEMATCH: x precedes z to break their equal allocation priority. */
    s32 p9;
    s32 p11;
    s32 p9b;
    struct FieldActor *rec;
    s32 rec2;
    struct FieldActor *rec7;
    struct FieldActor *record;
    s32 v2;
    s32 queue_pos;
    s32 base6_4000208;
    s32 v0;
    s32 v6;

    p10 = a0;
    rec7 = Object_GetById(gGameState.selected_actor);
    rec = Object_GetById(p10);
    rec2 = Engine_GameFlagIsSet(0x340);
    p9 = rec7->x.part.pixel;
    p11 = rec7->z.part.pixel;
    Engine_EventBegin();
    Engine_AudioPlayCue(244);
    i = 0;
    do {
        rec->scale_x = ((i << 11) + 0x800);
        rec->scale_y = ((i << 12) + 0x1000);
        if (rec2 == 0) {
            next_frame = i + 1;
            IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - next_frame) << 8) | next_frame, 0x20000);
        } else {
            next_frame = i + 1;
        }
        Engine_TaskWait(1);
        i = next_frame;
    } while (i <= 15);
    Engine_GameFlagSet((0x1fe + p10));
    Engine_GameFlagSet(0x340);
    if (Engine_GameFlagIsSet(0x9a0) == 0) {
    } else {
        if (GameFlag_IsSet(0x9b6) != 0) {
        } else {
            Engine_GameFlagSet(0x9b6);
            Actor_SetSpeed(13, 0x10000, 0x8000);
            Actor_SetSpeed(0, 0x18000, 0xc000);
            record = Object_GetById(0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(13, record->x.fixed, record->z.fixed);
            }
            Actor_FaceActor(13, 0x4000, 0);
            Object_GetById(13)->steering_flags &= ~ACTOR_FACES_MOTION;
            Engine_ActorWalkTo(13, p9, (p11 - 16));
            Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
            Engine_ActorWalkToAndWait(0, (p9 + 8), (p11 - 40));
            Engine_EventWait(1);
            Object_GetById(0)->steering_flags |= ACTOR_FACES_MOTION;
            Engine_ActorSetAnimation(13, 1);
            Engine_ActorFaceDirection(0, 0x4000, 0);
            Engine_EventSetMessage((s32)MsgSuharaSandstorm);
            Actor_ShowEmote(13, 0x106, 60);
            Engine_EventShowMessage(13, 0);
            Actor_ShowEmote(13, 0x102, 60);
            Engine_EventShowMessage(13, 0);
            Actor_FaceDirection(13, 0x2000, 0);
            Actor_ShowEmote(13, 0x101, 60);
            Engine_EventShowMessage(13, 0);
            Engine_EventWait(10);
            Actor_FaceDirection(13, 0xc000, 30);
            Engine_ActorRunRepeatedMotion(13, 2);
            Engine_EventShowMessage(13, 0);
            Engine_ActorSetAnimationAndWait(13, 3);
            Engine_EventShowMessage(13, 0);
            Actor_SetSpeed(13, 0x10000, 0x8000);
            Engine_ActorSetAnimation(13, 2);
            record = Object_GetById(0);
            if ((s32)record != 0) {
                Engine_ActorSetDestination(13, record->x.part.pixel, record->z.part.pixel);
            }
            Engine_ActorWaitForMove(13);
            Engine_ActorSetPosition(13, 0, 0);
        }
    }
    Engine_EventEnd();
}

void FieldScene_RunLateActor8Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(8); }

void FieldScene_RunLateActor9Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(9); }

void FieldScene_RunLateActor10Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(10); }

void FieldScene_RunLateActor11Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(11); }

void FieldScene_RunLateActor12Step(void) { SuharaSabaku_RestoreActorScaleAndBlend(12); }

void FieldScene_RunActorThirteenRestoration(void)
{
    u32 i;
    struct FieldActor *record;

    if (GameFlag_IsSet(0x9a0) == 0) {
    } else {
        if (GameFlag_IsSet(0x1b7) != 0) {
        } else {
            if (GameFlag_IsSet(0x9b0) == 0) {
            } else {
                GameFlag_Set(0x9b5);
                Engine_EventBegin();
                Engine_EventSetMessage((s32)MsgSuharaNowhereFound);
                record = Actor_Get(ACTOR_PARTY_LEADER);
                if (record != 0) {
                    Actor_SetPosition(ACTOR_ID, record->x.fixed, record->z.fixed);
                }
                Actor_FaceActor(ACTOR_ID, 0xc000, 0);
                Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b8, 0x4e8);
                Actor_FaceDirection(ACTOR_ID, 0x4000, 0);
                Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1bc, 0x4d8);
                Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 40);
                Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
                Engine_ActorSetAnimationAndWait(ACTOR_ID, 4);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
                Actor_ShowEmote(ACTOR_ID, 0x105, 60);
                Event_ShowMessage(ACTOR_ID, 0);
                Engine_EventWait(30);
                Engine_ActorRunRepeatedMotion(ACTOR_ID, 2);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_FaceDirection(ACTOR_ID, 0xc000, 30);
                Event_AskYesNo(ACTOR_ID, 0);
                Engine_EventWait(30);
                Actor_ShowEmote(ACTOR_ID, 0x106, 60);
                Event_ShowMessage(ACTOR_ID, 0);
                Engine_ActorSetAnimationAndWait(ACTOR_ID, 3);
                Event_ShowMessage(ACTOR_ID, 0);
                Actor_SetSpeed(ACTOR_ID, 0xb333, 0x5999);
                Actor_WalkToAndWait(ACTOR_ID, 0x1b8, 0x4e8);
                Event_ShowMessage(ACTOR_ID, 0);
                Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
                Engine_ActorSetAnimation(ACTOR_ID, 2);
                record = Actor_Get(ACTOR_PARTY_LEADER);
                if (record != 0) {
                    Actor_SetDestination(ACTOR_ID, record->x.part.pixel, record->z.part.pixel);
                }
                Engine_ActorWaitForMove(ACTOR_ID);
                Actor_SetPosition(ACTOR_ID, 0, 0);
                Engine_EventEnd();
            }
        }
    }
}

s32 SuharaSabaku_FollowLeaderWithSparks(struct FieldActor *actor)
{
    struct EffectOptions options;
    struct FieldActor *leader;
    s32 dx;
    s32 dz;
    s32 phase;

    leader = Object_GetById(gGameState.selected_actor);
    actor->acceleration = 0x4000;
    actor->speed = 0x18000;
    actor->motion_flags = 0;
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->active ^= 1;
    if (Engine_GameFlagIsSet(0x104)) {
        actor->target_x = ACTOR_NO_TARGET;
        actor->target_y = ACTOR_NO_TARGET;
        actor->target_z = ACTOR_NO_TARGET;
    } else {
        actor->target_x = leader->x.fixed;
        actor->target_y = leader->terrain_height;
        actor->target_z = leader->z.fixed;
        dx = actor->x.fixed - leader->x.fixed;
        if (dx < 0) {
            dx = leader->x.fixed - actor->x.fixed;
        }
        dz = actor->z.fixed - leader->z.fixed;
        if (dx + (dz >= 0 ? actor->z.fixed - leader->z.fixed : leader->z.fixed - actor->z.fixed) < 0x80000) {
            struct EventWork *work = gEventWork;

            if (leader->motion_flags != 0) {
                work->raised_trigger = 55;
            }
            actor->motion_flags = 3;
            actor->target_x = leader->x.fixed;
            actor->target_y = leader->y.fixed;
            actor->target_z = leader->z.fixed;
        }
    }
    phase = gFrameCount & 7;
    if (phase == 0) {
        options.start_scale_x = 0xcccc;
        options.start_scale_y = 0xcccc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, phase, phase, 0x880001, &options);
    }
    return 1;
}

void PlaceActorTwelveWhenFlagClear(void)
{
    if (GameFlag_IsSet(2487) == 0) {
        GameFlag_Set(526);
        Actor_SetPosition(12, 240 << 15, 206 << 18);
        Engine_ActorEnableActionCallback(12, gSuharaSabakuActor12Action);
    }
}

void SuharaSabaku_EnableEffectChildOption(void)
{
    u8 *state = *(u8 **)gEffectWork;
    ((struct BattleFxScene *)state)->child_option = 1;
}

void SuharaSabaku_MeetActor(s32 a0, s32 actor)
{
    struct GameState *state;

    if (gEventWork->raised_trigger == 99) {
        gEventWork->raised_trigger = 0;
    }
    Engine_GameFlagClear(0x20f);
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku1) {
        Engine_GameFlagSet((actor + 0x2f9));
    } else {
        if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
            Engine_GameFlagSet((actor + 0x309));
        }
    }
    GameFlag_SetByte(0x210, 0);
    BattleFx_SetWeightedResult(98, 5);
    Battle_SetStart(3);
    state = &gGameState;
    if (state->scene == (s32)&SceneId_SuharaSabaku2) {
        if (actor == 11) {
            BattleFx_SetWeightedResult(98, 7);
        } else {
            if (actor == 12) {
                BattleFx_SetWeightedResult(98, 6);
                Engine_ActorStop(12);
                Engine_ActorSetPosition(12, 0, 0);
            }
        }
    }
    Object_GetById(gGameState.selected_actor)->motion_flags = 3;
}

void SuharaSabaku_DropAndLeave(void)
{
    s32 other;
    struct FieldActor *leader;
    struct FieldActor *partner;
    s32 i;

    other = GameFlag_GetByte(0x218);
    leader = Object_GetById(gGameState.selected_actor);
    partner = Object_GetById(other);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_AudioPlayCue(219);
    Engine_ActorSetSpriteFlags((struct FieldActor *)gGameState.selected_actor, 0);
    partner->motion_flags = 0;
    leader->motion_flags = 0;
    leader->velocity_y = 0;
    leader->unknown_5d[4] = 1;
    partner->unknown_5d[4] = 1;
    for (i = 0; i < 60; i++) {
        leader->velocity_y += 0x3333;
        partner->velocity_y += 0x3333;
        Engine_TaskWait(1);
    }
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
    Engine_GameFlagSet(0x122);
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku2 && GameFlag_GetByte(0x218) == 11) {
        Event_SetDestination((s32)&SceneId_WorldMap, 77);
    } else {
        Event_SetDestination((s32)&SceneId_WorldMap, 27);
    }
}

void EncounterPalette_Pulse(void)
{
    u16 phase = gFrameCount & 63;
    s32 level;

    if (phase > 31)
        phase = 64 - phase;
    level = (phase >> 1) + 7;
    level |= (level << 10) | (level << 5);
    EncounterPalette = ((u32)level << 16) >> 16;
}

s32 FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 record;

    if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        Object_GetById(14)->priority_flags = 2;
        Object_GetById(14)->motion_flags = 3;
        Actor_SetPosition(14, 0, 0);
        Map_CopyCellAttributes(16, 44, 1, 1, 15, 44);
        MapObject_SetPosition(100, 0, 0);
        Map_CopyCellAttributes(12, 71, 1, 1, 127, 127);
        /* FAKEMATCH: the void result is discarded; Call6 changes argument allocation. */
        Value6(Engine_MapCopyCellAttributes, 11, 71, 1, 1, 12, 71);
        /* FAKEMATCH: an empty scheduling barrier after the third cell copy gives the game's argument order r1, r2, r3, r0. */
        do {
        } while (0);
        record = Engine_TaskRemoveCallback(EncounterPalette_Pulse);
        {
            /* FAKEMATCH: a forced temporary gives the game's load order. */
            s32 shown = gSuharaSabakuShownLevel;

            EncounterPalette = shown;
        }
        return record;
    }
}

s32 FieldScene_RunScene3c0SequenceA(void)
{
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        Object_GetById(14)->priority_flags = 2;
        Object_GetById(14)->motion_flags = 0;
        Actor_SetPosition(14, 0xf80000, 0x2c80000);
        Map_CopyCellAttributes(31, 95, 1, 1, 15, 44);
        MapObject_SetPosition(100, -1, -1);
        BattleFx_EmitRandomParticle();
        Map_CopyCellAttributes(127, 127, 1, 1, 12, 71);
        return Engine_TaskAddCallback(EncounterPalette_Pulse, 0xc80);
    }
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku3) {
        return gSuharaSabakuEvents3;
    }
    return gSuharaSabakuEventsOther;
}

s32 SuharaSabaku_RunSceneScript(void)
{
    u8 *map;

    map = (*(struct FieldGlobals *)gMapWork).map;
    (*(struct FieldGlobals *)gMapWork).event->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    if (GameFlag_GetByte(0x210) != 0) {
        gGameState.movement_mode = 2;
        Engine_TaskAddCallback(SuharaSabaku_SyncSelectedActorProgress, 0xc80);
    }
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku1
        || gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        gSuharaSabakuShownLevel = EncounterPalette;
        FieldScene_RunOpeningAuxiliarySequence();
    }
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku1) {
        SuharaSabaku_ApplyVisitFlagBlend();
    } else if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        SuharaSabaku_ApplyAltarFlagBlend();
    } else {
        Engine_AudioPlayCue(0x120);
    }
    if (gGameState.entrance == 0) {
        *(u16 *)(map + 20) &= ~0x200;
    }
    return 0;
}

void SuharaSabaku_ApplyVisitFlagBlend(void)
{
    struct FieldActor *actor;
    s32 id;
    s32 alpha = 0;

    if (Engine_GameFlagIsSet(0x301)) {
        Engine_GameFlagSet(0x206);
    }
    if (Engine_GameFlagIsSet(0x302)) {
        Engine_GameFlagSet(0x207);
    }
    if (Engine_GameFlagIsSet(0x303)) {
        Engine_GameFlagSet(0x208);
    }
    if (Engine_GameFlagIsSet(0x304)) {
        Engine_GameFlagSet(0x209);
    }
    if (Engine_GameFlagIsSet(0x305)) {
        Engine_GameFlagSet(0x20a);
    }
    for (id = 8; id <= 12; id++) {
        actor = Object_GetById(id);
        if (actor != 0) {
            if (!Engine_GameFlagIsSet(0x109)) {
                actor->scale_x = 0x800;
                actor->scale_y = 0x800;
            }
            actor->sprite->flags = 0;
        }
    }
    IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
    if (Engine_GameFlagIsSet(0x340)) {
        alpha = 16;
        QueueSceneSound(244);
    }
    IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - alpha) << 8) | alpha, 0x20000);
}

void SuharaSabaku_ApplyAltarFlagBlend(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    s32 id;
    s32 alpha = 0;

    if (Engine_GameFlagIsSet(0x311)) {
        Engine_GameFlagSet(0x206);
    }
    if (Engine_GameFlagIsSet(0x312)) {
        Engine_GameFlagSet(0x207);
    }
    if (Engine_GameFlagIsSet(0x313)) {
        Engine_GameFlagSet(0x208);
    }
    for (id = 8; id <= 10; id++) {
        actor = Object_GetById(id);
        if (actor != 0) {
            if (!Engine_GameFlagIsSet(0x109)) {
                actor->scale_x = 0x800;
                actor->scale_y = 0x800;
            }
            sprite = actor->sprite;
            sprite->flags = 0;
        }
    }
    actor = Object_GetById(11);
    if (actor != 0) {
        u8 *part;

        sprite = actor->sprite;
        part = *(u8 **)((u8 *)sprite + 40);
        if (part != 0) {
            part[5] = 10;
        }
        sprite->unknown_20[5] = 1;
        sprite->flags = 0;
    }
    if (Engine_GameFlagIsSet(0x315)) {
        Engine_GameFlagSet(0x9b7);
    }
    IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
    if (Engine_GameFlagIsSet(0x340)) {
        alpha = 16;
        QueueSceneSound(244);
    }
    IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - alpha) << 8) | alpha, 0x20000);
}

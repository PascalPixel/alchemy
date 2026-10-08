#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "hashira.h"
#include "types.h"
#include "field_event.h"
#include "call.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "field_service.h"

s32 FieldScene_RunScene3b3SequenceD(void);
void FieldEffect_UpdateGridPlacement(void);

void Engine_MapCopyCellAttributes();

void ObjectMotion_WaitForAnimationChange();
void SceneEffect_SpawnRandomizedParticle();

struct Probe {
    s32 word[6];
};

void StagedActor_PlaceAtObjectTenCell();
void TakaraHashira_DropActorTen();


extern u8 TakaraHashira_PillarSlots[];
void SceneActor_ApplyPlacementQueryAndTag();
void BattleFx_StartFadeOverlay();
void TakaraHashira_ReleasePillarRider();
void OverlayObject_SetCallbackAndMode2();
void TakaraHashira_PrepLoweredActor();
void TakaraHashira_RestoreFlaggedPillar();

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void TakaraHashira_PushPillar(void)
{
    Engine_EventBegin();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    Object_GetById(0)->y.fixed = Map_GetTerrainHeight(
        Object_GetById(0)->terrain_id,
        Object_GetById(0)->x.fixed,
        Object_GetById(0)->z.fixed);
#endif
    if (FieldScene_RunScene3b3SequenceD() == 0) {
        Object_GetById(0)->motion_flags &= 254;
        Object_GetById(0)->priority_flags &= 254;
        StagedActor_AdvancePair();
        TakaraHashira_UpdatePillarActors();
        Object_GetById(ACTOR_PARTY_LEADER)->motion_flags |= ACTOR_FOLLOWS_TERRAIN;
        Object_GetById(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    }
    Engine_EventEnd();
}

void FieldScene_RunSingleStep(void)
{
    battle_owner_69();
}

void TakaraHashira_RunStagedCellScene(void)
{
    struct FieldActor *leader = Object_GetById(0);
    s32 pos[3];
    s32 *p = pos;

    p[0] = leader->x.fixed;
    p[1] = leader->y.fixed;
    p[2] = leader->z.fixed + 0x100000;
    if (StagedActor_FindAtTile(p, leader) != NULL) {
        TakaraHashira_PushPillar();
    } else if (FieldScene_RunScene3b3SequenceD() == 0) {
        FieldEffect_UpdateGridPlacement();
    }
}

void TakaraHashira_AttachPillarRider(void)
{
    struct FieldActor *obj = Object_GetById(ACTOR_PARTY_LEADER);
    PILLAR_WORK->rider = obj;
    obj->rise_counter = 1;
}

void TakaraHashira_ReleasePillarRider(void)
{
    struct FieldActor *obj = Object_GetById(ACTOR_PARTY_LEADER);

    PILLAR_WORK->rider = NULL;
    obj->rise_counter = 0;
}

s32 SceneActor_UpdateBit1ByPositionToSlotZero(struct FieldActor *actor)
{
    struct FieldActor *ref = Object_GetById(ACTOR_PARTY_LEADER);

    if (actor->z.fixed > ref->z.fixed) {
        actor->priority_flags &= ~ACTOR_PRIORITY_UNDERFOOT;
        if (actor->y.fixed < ref->y.fixed)
            actor->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    }

    return 0;
}

void TakaraHashira_RunActorAction(s32 id)
{
    struct FieldActor *actor;

    actor = Object_GetById(id);
    Engine_EventBegin();
    actor->update = (void (*)(union FieldObject *))SceneActor_UpdateBit1ByPositionToSlotZero;
    Map_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
    GameFlag_Set(id + 0x1f5);
    Engine_ActorEnableActionCallback(id, TakaraHashira_ActionTable);
    Engine_EventEnd();
}

void FieldScene_RunActor11Step(void)
{
    TakaraHashira_RunActorAction(11);
}

void FieldScene_RunActor12Step(void)
{
    TakaraHashira_RunActorAction(12);
}

void TakaraHashira_CheckLeaderOnPillar(void)
{
    struct FieldActor *leader;
    struct FieldActor *pillar;
    u8 *effects;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    pillar = Object_GetById(13);
    effects = gEffectWork;
    if (pillar->x.fixed >> 20 == leader->x.fixed >> 20 && pillar->z.fixed >> 20 == leader->z.fixed >> 20) {
        Engine_GameFlagSet(0x203);
        effects[53] = 1;
    } else {
        Engine_GameFlagClear(0x203);
    }
}

void TakaraHashira_RaiseActorFourteen(void)
{
    struct FieldActor *pillar;

    pillar = Object_GetById(14);
    Object_GetById(13)->scale_x = 0x10000;
    Object_GetById(13)->scale_y = 0x10000;
    ((struct FieldSpritePriority *)Object_GetById(13)->sprite)->priority = 2;
    ((struct FieldSpritePriority *)pillar->sprite)->priority = 2;
    pillar->acceleration = 0x6666;
    pillar->speed = 0xcccc;
    FieldObject_SetPosition(pillar, pillar->x.fixed, 0x200000, pillar->z.fixed);
    Engine_ActorWaitForMove(14);
    Map_CopyCellAttributes(20, 14, 1, 1, 22, 16);
}

void SceneEffect_SpawnRandomizedParticle(void)
{
    struct EffectOptions options;
    struct EffectOptions *d;
    s32 spread;
    s32 secondary;
    u32 draw;
    u32 mask;

    if ((gFrameCount & 2) != 0)
        return;
    if ((gFrameCount & 7) == 0)
        Engine_AudioPlayCue(136);

    d = &options;
    d->palette = 10;
    d->start_scale_x = 0x8000;
    d->start_scale_y = 0x8000;
    d->target_scale_x = 0x19999;
    d->target_scale_y = 0x19999;
    draw = Engine_RandomNext();
    mask = 0x0ffff000;
    mask &= draw;
    d->rotation = mask;
    d->update = FieldScene_RunScene3b3SequenceE;

    draw = (u32)Engine_RandomNext();
    spread = -((s32)((draw * 5) >> 16) * 0x10000 + 0x60000);
    spread /= 2;
    draw = (u32)Engine_RandomNext();
    secondary = -((s32)((draw * 5) >> 16) * 0x10000 + 0x50000);

    Effect_Spawn(PIXELS(324), PIXELS(48), PIXELS(228), spread, secondary, 0,
                 EFFECT_USE_UPDATE | EFFECT_USE_ROTATION | EFFECT_USE_START_SCALE
                     | EFFECT_SCALE_TO_TARGET | EFFECT_USE_PALETTE,
                 d);
}

s32 SceneEffect_SpawnRandomEffectEveryEightFrames(struct FieldActor *actor)
{
    struct EffectOptions options;
    struct EffectOptions *p;
    u32 phase = gFrameCount & 7;
    s32 x;
    s32 y;
    s32 z;
    s32 scale;

    if (phase != 0)
        return 0;

    p = &options;
    p->palette = 7;
    p->start_scale_x = 0xb333;
    p->start_scale_y = 0xb333;

    x = actor->x.fixed + (((s32)(((u32)Engine_RandomNext() * 17) >> 16) - 8) << 16);
    y = actor->y.fixed + ((s32)(((u32)Engine_RandomNext() * 17) >> 16) << 16);
    z = actor->z.fixed + (((s32)(((u32)Engine_RandomNext() * 17) >> 16) - 8) << 16);
    scale = Math_Divide((s32)(((u32)Engine_RandomNext() * 5) >> 16) * 0x10000 + 0x30000, 10);

    Effect_Spawn(x, y, z, 0, scale, phase, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1, p);
    return 0;
}

void TakaraHashira_RunMapShiftScene(void)
{
    s32 set;

    set = Engine_GameFlagIsSet(0x203);
    if (set == 0) {
        Engine_GameFlagSet(0x202);
        Engine_EventBegin();
        Engine_CameraSetSpeed(0x9999, 0x1333);
        Engine_CameraMoveTo(0x1380000, -1, 0xb80000, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(20);
        Map_CopyCellsTo(73, 10, 60, 10, 1, 2);
        Engine_EventWait(20);
        Engine_TaskAddCallback((s32)SceneEffect_SpawnRandomizedParticle, 0xc80);
        Engine_EventWait(40);
        if (GameFlag_IsSet(0x201) != 0) {
            Object_GetById(12)->update = (void (*)(union FieldObject *))SceneEffect_SpawnRandomEffectEveryEightFrames;
            Engine_ActorSetAnimation(12, 6);
            ObjectMotion_WaitForAnimationChange(12);
            Object_GetById(12)->update = (void (*)(union FieldObject *))set;
            Map_CopyCellAttributes(17, 13, 1, 1, 18, 13);
            Engine_GameFlagClear(0x201);
            Engine_ActorSetChildValue(12, 0);
            Engine_ActorEnableActionCallback(12, 1);
        } else {
            Engine_EventWait(60);
        }
        ((void (*)())Engine_TaskRemoveCallback)((s32)SceneEffect_SpawnRandomizedParticle);
        Engine_EventWait(20);
        Map_CopyCellsTo(72, 10, 60, 10, 1, 2);
        Engine_EventWait(20);
        Engine_EventEnd();
    }
}

void StagedActor_PlaceAtObjectTenCell(void)
{
    struct FieldActor *obj = Object_GetById(10);
    s32 x;
    s32 z;

    Engine_EventBegin();

    x = obj->x.fixed >> 20;
    z = obj->z.fixed >> 20;

    StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0);
    Engine_EventEnd();
}

void TakaraHashira_DropActorTen(void)
{
    struct FieldActor *rec7;
    s32 rec8;

    rec7 = Object_GetById(10);
    Engine_EventBegin();
    StagedActor_FillGridAttributeRectangle(2, (rec7->x.fixed >> 20), (rec7->z.fixed >> 20), 1, 1, 255);
    if ((rec7->x.fixed >> 20) == 16) {
        rec8 = GameFlag_IsSet(0x204);
        if (rec8 == 0) {
            Engine_EventWait(10);
            Engine_AudioPlayCue(159);
            rec7->motion_flags = rec8;
            rec7->terrain_height = -0x20000;
            rec7->y.fixed = -0x20000;
            Engine_GameFlagSet(0x204);
        }
    }
    Engine_EventEnd();
}

void TakaraHashira_RunPushScene(void)
{
    struct Probe probe;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        ((void (*)(struct Probe))SceneActor_MoveAndRedraw)(probe);
    } else {
        StagedActor_PlaceAtObjectTenCell();
        StagedActor_AdvancePair();
        TakaraHashira_DropActorTen();
    }
    Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraHashira1) {
        return gTakaraHashiraEvents1;
    }
    if (scene == (s32)&SceneId_TakaraHashira2) {
        return gTakaraHashiraEvents2;
    }
    if (scene == (s32)&SceneId_TakaraHashira3) {
        return gTakaraHashiraEvents3;
    }
    if (scene == (s32)&SceneId_TakaraHashira4) {
        return gTakaraHashiraEvents4;
    }
    if (scene == (s32)&SceneId_TakaraHashira5) {
        return gTakaraHashiraEvents5;
    }
    return gTakaraHashiraEventsOther;
}

void TakaraHashira_PrepLoweredActor(s32 id)
{
    struct FieldActor *actor = Object_GetById(id);

    actor->collision_flags &= 254;
    actor->priority_flags |= 2;
    actor->motion_flags = 0;
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->sprite->priority = 2;
}

void TakaraHashira_RestoreFlaggedPillar(s32 a0)
{
    struct FieldActor *actor;

    actor = Object_GetById(a0);
    if (GameFlag_IsSet(a0 + 0x1f5) != 0) {
        Object_SetMode(actor, 5);
        actor->update = (void (*)(union FieldObject *))SceneActor_UpdateBit1ByPositionToSlotZero;
        Map_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
        Engine_ActorEnableActionCallback(a0, TakaraHashira_ActionTable);
    }
}

void OverlayObject_SetCallbackAndMode2(s32 id)
{
    struct FieldActor *actor = Object_GetById(id);

    actor->terrain_id = 2;
    actor->motion_flags = 0;
    actor->update = (void (*)(union FieldObject *))TakaraHashira_UpdateActorPriority;
}

void SceneActor_CheckActors8To11NearSlotZero(void)
{
    struct FieldActor *hero = Object_GetById(ACTOR_PARTY_LEADER);
    u32 selector;
    struct FieldActor *actor;

    for (selector = 8; selector <= 11; selector++) {
        actor = Object_GetById(selector);
        if (hero->y.fixed / 0x10000 == actor->y.fixed / 0x10000
            && hero->z.fixed <= actor->z.fixed - 0x80000
            && hero->z.fixed > actor->z.fixed - 0x180000) {
            if (hero->x.fixed - 0x100000 <= actor->x.fixed
                && actor->x.fixed < hero->x.fixed + 0x100000) {
                Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, ((struct FieldSpritePriority *)actor->sprite)->priority);
                return;
            }
        } else {
            Object_GetById(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
        }
    }
}

s32 TakaraHashira_SetupArea(void)
{
    struct FieldActor *actor;
    u32 i;
    u32 id;
    s32 x;
    s32 z;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_TakaraHashira2) {
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        FieldScene_RedrawActorFootprint(11);
        FieldScene_RedrawActorFootprint(12);
    } else if (gGameState.scene == (s32)&SceneId_TakaraHashira3) {
        Map_CopyCells(32, 0, 64, 32, 0, 64);
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        FieldScene_RedrawActorFootprint(11);
        FieldScene_RedrawActorFootprint(12);
        FieldScene_RedrawActorFootprint(13);
        FieldScene_RedrawActorFootprint(14);
        FieldScene_RedrawActorFootprint(15);
        if (Engine_GameFlagIsSet(0x109) != 0 && Engine_GameFlagIsSet(0x200) != 0) {
            Map_CopyCellsTo(79, 34, 84, 24, 1, 2);
            Engine_MapCopyCellsTo(0, 32, 32, 0, 32, 32);
            Engine_MapCopyCellsTo(32, 32, 64, 0, 32, 32);
            SceneActor_ApplyPlacementQueryAndTag(9);
            SceneActor_ApplyPlacementQueryAndTag(10);
            SceneActor_ApplyPlacementQueryAndTag(11);
            SceneActor_ApplyPlacementQueryAndTag(12);
            SceneActor_ApplyPlacementQueryAndTag(13);
            SceneActor_ApplyPlacementQueryAndTag(14);
            SceneActor_ApplyPlacementQueryAndTag(15);
            Map_CopyCellAttributes(24, 3, 1, 1, 24, 8);
        }
    } else if (gGameState.scene == (s32)&SceneId_TakaraHashira4) {
        OverlayObject_CreateConfiguredObject(PIXELS(584), 0, PIXELS(200), 223);
        if (Engine_GameFlagIsSet(0x109) == 0) {
            Object_GetById(ACTOR_PARTY_LEADER)->rise_counter = 1;
        }
        BattleFx_StartFadeOverlay(0);
        if (Object_GetById(ACTOR_PARTY_LEADER)->rise_counter == 0) {
            TakaraHashira_ReleasePillarRider();
        }
        OverlayObject_SetCallbackAndMode2(8);
        OverlayObject_SetCallbackAndMode2(9);
        OverlayObject_SetCallbackAndMode2(10);
        OverlayObject_SetCallbackAndMode2(11);
        {
            s32 *entry = (s32 *)TakaraHashira_PillarSlots;

            for (i = 0; i < 4; i++) {
                entry[0] = 0;
                entry[1] = 0;
                entry[2] = 0;
                entry[4] = i + 0x200;
                entry += 5;
            }
        }
        TakaraHashira_UpdatePillarActors();
        Engine_EventWait(1);
        Engine_TaskAddCallback(SceneActor_CheckActors8To11NearSlotZero, TASK_PRIORITY_SCENE);
        if (Engine_GameFlagIsSet(0x109) != 0) {
            for (id = 8; id < 12; id++) {
                actor = Object_GetById(id);
                x = actor->x.fixed >> 20;
                if (x == 37) {
                    z = actor->z.fixed >> 20;
                    if (z == 9) {
                        Engine_MapCopyCellAttributes(27, 8, 1, 1, x, z);
                        break;
                    }
                }
            }
        }
    } else if (gGameState.scene == (s32)&SceneId_TakaraHashira5) {
        Engine_ActorSetAnimation(10, 2);
        Engine_ActorSetChildValue(10, 6);
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
#if EDITION_INTERNATIONAL
        Object_GetById(8)->motion_flags = 0;
        Object_GetById(9)->motion_flags = 0;
#endif
        TakaraHashira_DropActorTen();
        TakaraHashira_PrepLoweredActor(11);
        TakaraHashira_PrepLoweredActor(12);
        TakaraHashira_PrepLoweredActor(13);
        TakaraHashira_RestoreFlaggedPillar(11);
        TakaraHashira_RestoreFlaggedPillar(12);
        TakaraHashira_RestoreFlaggedPillar(13);
        actor = Object_GetById(13);
        actor->update = 0;
        TakaraHashira_PrepLoweredActor(14);
        {
            u8 *flags = &Object_GetById(14)->collision_flags;
            /* FAKEMATCH: needed to match: the merged flags in a word temporary keep the byte load in r2. */
            s32 value = *flags | 8;

            *flags = value;
        }
        if (Engine_GameFlagIsSet(0x202) == 0) {
            Object_GetById(13)->scale_x = 0x18000;
            Object_GetById(13)->scale_y = 0x18000;
            ((u8 *)Object_GetById(13)->sprite)[9] |= 12;
            ((u8 *)Object_GetById(14)->sprite)[9] |= 12;
            Map_CopyCellAttributes(26, 12, 1, 1, 22, 16);
        }
    }
    return 0;
}

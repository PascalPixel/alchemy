#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mura.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "call.h"
#include "text/msg_ids.h"

s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);



struct SpawnCounter {
    s16 frames;
};

struct SpawnCounter gCornerSpawnCounter;

s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        tgt = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }

            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BiribinoMura1) {
        return gBiribinoMuraEntrances1;
    }
    if (scene == (s32)&SceneId_BiribinoMura3) {
        return gBiribinoMuraEntrances3;
    }
    if (scene == (s32)&SceneId_BiribinoMura2) {
        return gBiribinoMuraEntrances2;
    }
    return gBiribinoMuraEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoMura2) {
        return gBiribinoMuraRegions2;
    }
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

void SceneState_SetValues9_3_0(void)
{
    BattleFx_RunPageEffectForSlot(9, 3, 0);
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BiribinoMura1) {
        return gBiribinoMuraPlacements1;
    }
    if (scene == (s32)&SceneId_BiribinoMura3) {
        return gBiribinoMuraPlacements3;
    }
    if (scene == (s32)&SceneId_BiribinoMura2) {
        return gBiribinoMuraPlacements2;
    }
    return gBiribinoMuraPlacementsOther;
}

void FieldScene_RunScriptedSteps947And29DD(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgFieldPeeredWell, 1);
    Engine_MessageShowCentered((s32)MsgBiribinoBottomNotVisibleLooksVery, 1);
    Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BiribinoMura1) {
        return gBiribinoMuraEvents1;
    }
    if (scene == (s32)&SceneId_BiribinoMura3) {
        return gBiribinoMuraEvents3;
    }
    if (scene == (s32)&SceneId_BiribinoMura2) {
        return gBiribinoMuraEvents2;
    }
    return gBiribinoMuraEventsOther;
}

void FieldScene_RunScriptedStep1472(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgBiribinoAreaOffLimitsThoseWithout, 1);
    Engine_EventEnd();
}

void FieldScene_RunScriptedStep146E(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgBiribinoMccoysHiddenWarehouseDoNot, 1);
    Engine_EventEnd();
}

void SceneDialogue_RunLine1470(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgBiribinoThereTreeLooksLikePerson, 1);
    Engine_EventEnd();
}

void BiribinoMura_TalkAboutHumanTree(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoItsTreeButAlmostLooks);
    if (GameFlag_IsSet(0x301) != 0) {
        gEventWork->message++;
    }
    Event_ShowMessage(9, 0);
    GameFlag_Set(0x301);
    Engine_EventEnd();
}

void SceneDialogue_RunActorTwelveDialogue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoDidSeeTreeAtEntrance);
    Engine_EventAskYesNo(12, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorFourteenDialogue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoHaveTriedHeadingSoutheastFrom);
    Engine_EventAskYesNo(14, 0);
    Engine_EventEnd();
}

void SceneDialogue_ShowLine16BF(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoWasTurnedIntoTreeFor);
    Engine_EventAskYesNo(21, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorSixteenDialogue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoCurseWasBrokenThanksEfforts);
    Engine_EventAskYesNo(16, 0);
    Engine_EventEnd();
}

void SceneDialogue_ShowLine16CC(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoHaveEverBeenVillageImil);
    Engine_EventAskYesNo(18, 0);
    Engine_EventEnd();
}

void FieldScene_RunEarlySequence(void)
{

    u32 i;
    struct FieldActor *record;
    s32 v5;
    u8 *tbl;
    u8 *tbl2;
    s32 off;
    s32 off2;
    s32 a1;
    s32 a2;
    u8 *p7;

    p7 = *(u8 **)&gEventWork;
    Engine_EventBegin();
    for (i = 8; i < 66; i++) {
        record = Object_GetById(i);
        if (record != 0) {
            record->motion_flags = 0;
        }
    }
    v5 = (s32)((s32)(*(u16 *)(p7 + 0x16c) - 3) << 16) >> 16;
    if (v5 == 6) {
        Event_PlayCue(188);
    } else {
        Engine_AudioPlayCue(158);
    }
    off = v5 << 2;
    tbl = (u8 *)Mura_DoorCellOrigins;
    a1 = *(s16 *)(tbl + off);
    off2 = off + 2;
    a2 = *(s16 *)(tbl + off2);
    tbl2 = (u8 *)Mura_DoorCellSteps;
    Engine_MapAnimateCells(*(s32 *)(tbl2 + off), a1, a2);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Object_GetById(0)->motion_flags = 0;
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (v5 == 6) {
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
    } else {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
    if (v5 == 4) {
        Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    } else {
        Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    }
    Engine_EventWait(16);
    Engine_EventRequestExit(v5 + 3);
    Engine_EventEnd();
}

void FieldScene_RunScene38bSequenceC(void)
{
    struct FieldActor *rec;
    struct FieldActor *rec7;

    rec = Object_GetById(ACTOR_PARTY_LEADER);
    rec7 = Object_GetById(11);
    if ((rec7->x.fixed >> 20) == 6) {
        Engine_EventBegin();
        Engine_ActorSetSpritePriority(11, 1);
        Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Engine_EventWait(20);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
        Actor_SetSpeed(11, 0x3333, 0x1999);
        Actor_Get(ACTOR_PARTY_LEADER)->steering_flags &= ~ACTOR_FACES_MOTION;
        rec7->motion_flags = 0;
        rec->scale_x = -0x10000;
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
        Actor_MoveToAndWait(11, 111, 196);
        rec->scale_x = 0x10000;
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
        Engine_EventWait(20);
        rec->scale_x = -0x10000;
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
        Actor_MoveToAndWait(11, 121, 190);
        rec->scale_x = 0x10000;
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
        Engine_EventWait(20);
        rec->scale_x = -0x10000;
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
        Actor_MoveToAndWait(11, 132, 186);
        rec->scale_x = 0x10000;
        Actor_Get(ACTOR_PARTY_LEADER)->steering_flags |= ACTOR_FACES_MOTION;
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
        Engine_ActorSetSpritePriority(11, 2);
        RunSceneTransitionEffect(0, 11);
        Engine_TaskWait(10);
        Engine_EventSetMessage((s32)MsgBiribinoThankSavedMeFromBeing);
        Event_ShowMessage(11, 0);
        Engine_PsynergyCancel();
        Engine_TaskWait(10);
        GameFlag_Set(0x848);
        Engine_EventEnd();
    }
}

void BiribinoMura_CheckLeaderAhead(void)
{
    Engine_LeaderCheckAhead();
}

void BiribinoMura_ReceiveJillsGift(void)
{
    u32 i;
    struct FieldActor *rec7;
    struct FieldActor *record;

    rec7 = Object_GetById(ACTOR_PARTY_LEADER);
    if (GameFlag_IsSet(0x845) == 0) {
    } else {
        if (GameFlag_IsSet(0x848) == 0) {
        } else {
            Engine_EventBegin();
            Camera_SetSpeed(0x26666, 0x4ccc);
            Camera_MoveTo(0x1070000, -1, 0xad0000, 1);
            Engine_CameraWaitForMove();
            record = Actor_Get(12);
            if (record->x.fixed > rec7->x.fixed) {
                Actor_FaceDirection(13, 0x5000, 20);
                Actor_ShowEmote(13, 0x100, 20);
                Engine_EventSetMessage((s32)MsgBiribinoYoureGuy);
                Event_ShowMessageAndWait(13, 0, 10);
                Actor_ShowEmote(12, 0x100, 0);
            } else {
                Actor_FaceDirection(12, 0x3000, 20);
                Actor_ShowEmote(12, 0x100, 20);
                Engine_EventSetMessage((s32)MsgBiribinoYoureGuy);
                Event_ShowMessageAndWait(12, 0, 10);
                Actor_ShowEmote(13, 0x100, 0);
            }
            Actor_ShowEmote(14, 0x100, 0);
            Actor_FaceDirection(14, 0x3000, 0);
            Actor_FaceDirection(12, 0x5000, 0);
            Actor_FaceDirection(13, 0x3000, 0);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x10c, 184);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
            Engine_ActorRunRepeatedMotion(13, 2);
            Event_ShowMessageAndWait(13, 0, 10);
            Actor_FaceDirection(13, 0, 0);
            Actor_FaceDirection(14, 0x3000, 20);
            Actor_FaceDirection(12, 0x8000, 20);
            Engine_ActorSetAnimationAndWait(12, 3);
            Actor_SetAttachedEffect(14, 0x102);
            Engine_EventWait(40);
            Actor_FaceDirection(14, 0x3000, 10);
            Actor_FaceDirection(12, 0x5000, 0);
            Actor_FaceDirection(13, 0x3000, 10);
            Engine_ActorRunRepeatedMotion(14, 1);
            Event_ShowMessageAndWait(14, 0, 10);
            Engine_ActorSetAnimation(12, 3);
            Engine_ActorSetAnimationAndWait(13, 3);
            Engine_EventWait(20);
            Event_ShowMessage(14, 0);
            Actor_SetSpeed(14, 0x9999, 0x4ccc);
            Object_GetById(14)->steering_flags &= ~ACTOR_FACES_MOTION;
            Actor_WalkToAndWait(14, 0x10a, 172);
            Engine_EventWait(1);
            Object_GetById(14)->steering_flags |= ACTOR_FACES_MOTION;
            Engine_EventWait(10);
            Engine_ActorSetAnimationAndWait(14, 3);
            Event_ShowMessageAndWait(14, 0, 10);
            Engine_MessageShowCentered((s32)MsgBiribinoJillGaveRobinSpecialGift, 1);
            gEventWork->message++;
            Engine_ItemShowFound(ITEM_HARD_NUT, 3);
            Engine_PartyGiveItem(ITEM_HARD_NUT, 0);
            Engine_ActorSetAnimationAndWait(14, 3);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
            Actor_SetSpeed(14, 0x10000, 0x8000);
            Object_GetById(14)->steering_flags &= ~ACTOR_FACES_MOTION;
            Actor_WalkToAndWait(14, 0x106, 156);
            Engine_EventWait(1);
            {
                u8 *record = Actor_Get(14);
                u8 value = record[90] | 1;

                record[90] = value;
            }
            Engine_EventWait(20);
            Engine_ActorRunRepeatedMotion(12, 2);
            Event_ShowMessageAndWait(12, 0, 10);
            Engine_ActorSetAnimation(12, 3);
            Engine_ActorSetAnimation(13, 3);
            Engine_ActorSetAnimationAndWait(14, 3);
            FieldObject_SetTargetAndCallback(12, 0x10000, Mura_VillagerActions);
            FieldObject_SetTargetAndCallback(13, 0x10000, Mura_VillagerActions);
            FieldObject_SetTargetAndCallback(14, 0x10000, Mura_VillagerActions);
            GameFlag_Set(0x849);
            Engine_EventEnd();
        }
    }
}

s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (gGameState.scene == (s32)&SceneId_BiribinoMura1) {
        BiribinoMura_RestoreVillageActors();
    } else {
        if (gGameState.scene == (s32)&SceneId_BiribinoMura3) {
            FieldScene_RunScene38bSequenceA();
            Engine_TaskAddCallback(BiribinoMura_UpdateCornerSpawn, 0xc80);
        } else {
            if (gGameState.scene == (s32)&SceneId_BiribinoMura2) {
                BiribinoMura_RestoreRaisedEntrance();
            }
        }
    }
    return 0;
}

void BiribinoMura_RestoreVillageActors(void)
{

    struct FieldActor *record;
    s16 sub_state;

    if (GameFlag_IsSet(0x845) != 0) {
        Engine_ActorSetPosition(9, 0, 0);
        Actor_FaceDirection(14, 0x3000, 0);
        Actor_FaceDirection(15, 0x5000, 0);
    } else {
        record = Actor_Get(9);
        Engine_ActorSetSpriteFlags(record, 0);
        Actor_SetPosition(21, 0, 0);
    }
    record = Actor_Get(8);
    record->scale_y = 0x18000;
    {
        s32 off = 450;
        sub_state = *(s16 *)((u8 *)&gGameState + off);
    }
    if (sub_state == 10) {
        Actor_SetPosition(8, 0, 0);
    } else {
        if (sub_state == 9) {
            GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
        }
    }
    if (GameFlag_IsSet(0x109) == 0) {
        {
            s32 off = 450;
            sub_state = *(s16 *)((u8 *)&gGameState + off);
        }
        if (sub_state == 11) {
            Actor_SetPosition(20, 0xf80000, 0xd80000);
        }
    }
    Scene_UpdatePuzzleActors();
    if (GameFlag_IsSet(0x84a) != 0) {
        if (GameFlag_IsSet(0x84b) == 0) {
            GameFlag_Set(0x304);
        }
    }
}

void Scene_UpdatePuzzleActors(void)
{
    s32 p10;
    s32 p9;
    struct FieldActor *rec7;
    struct FieldActor *record;
    s32 p6;
    s32 row;

    rec7 = Object_GetById(ACTOR_PARTY_LEADER);
    record = Object_GetById(20);
    row = record->z.fixed >> 20;
    p9 = (rec7->x.fixed >> 20);
    p10 = (rec7->z.fixed >> 20);
    p6 = record->x.fixed;
    Map_CopyCellAttributes(15, 11, 3, 1, 15, 12);
    Map_CopyCellAttributes(15, 11, 3, 1, 15, 13);
    Map_CopyCellAttributes(15, 11, 3, 1, 15, 14);
    Map_CopyCellAttributes(1, 0, 1, 1, (p6 >> 20), row);
    if (((s32)p6 >> 20) == 16) {
        if (row == 13) {
            goto L_02000a60;
        }
    }
    Map_CopyCellAttributes(0, 0, 1, 1, 16, 13);
    L_02000a60:;
    if (p9 == 16) {
        if (p10 == 13) {
            Engine_EventBegin();
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
            Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
            if (row == 13) {
                Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x106, 196);
                Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
            } else {
                Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x11e, 218);
                Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
            }
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunScene38bSequenceA(void)
{
    u32 i;
    struct FieldActor *rec;
    s32 rec7;
    struct FieldActor *rec8;
    s32 record;

    rec8 = Object_GetById(10);
    rec = Object_GetById(11);
    record = Actor_Get(8);
    Engine_ActorSetSpriteFlags(record, 0);
    rec7 = GameFlag_IsSet(0x845);
    if (rec7 != 0) {
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Map_CopyCellsTo(56, 15, 40, 15, 1, 2);
        Map_CopyCellAttributes(26, 15, 1, 3, 10, 15);
        if (GameFlag_IsSet(0x849) == 0) {
            if (GameFlag_IsSet(0x848) != 0) {
                goto L_02000c92;
            }
            Engine_ActorSetPosition(14, 0, 0);
        }
        Actor_FaceDirection(12, 0xd000, 0);
        Actor_FaceDirection(13, 0xb000, 0);
    } else {
        Actor_SetPosition(12, 0, 0);
        Actor_SetPosition(13, 0, 0);
        Actor_SetPosition(14, 0, 0);
        record = Actor_Get(9);
        Engine_ActorSetSpriteFlags(record, 0);
        record = Actor_Get(10);
        Engine_ActorSetSpriteFlags(record, 0);
        record = Actor_Get(11);
        Engine_ActorSetSpriteFlags(record, 0);
        rec8->motion_flags = rec7;
        record = GameFlag_IsSet(0x881);
        if (record != 0) {
            Object_GetById(9)->collision_flags |= 16;
            Object_GetById(16)->collision_flags |= 16;
            Object_GetById(11)->collision_flags |= 16;
            Actor_SetPosition(16, 0x8e0000, 0x9c0000);
            record = Actor_Get(16);
            Engine_ActorSetSpriteFlags(record, 0);
            Actor_SetPosition(10, 0x8e0000, 0x9c0000);
            rec8->sprite->rotation = 0x4000;
            rec8->y.fixed += -0x80000;
            if (GameFlag_IsSet(0x848) != 0) {
                Actor_SetPosition(11, 0x840000, 0xba0000);
                goto L_02000c92;
            }
            Actor_SetPosition(11, 0x580000, 0xc40000);
            Engine_ActorSetSpritePriority(11, 3);
            rec->collision_flags |= 4;
        } else {
            rec8->y.fixed = 0x200000;
            rec->motion_flags = record;
            rec->y.fixed = 0x300000;
        }
    }
    L_02000c92:;
    ActorPresentation_RepaintTenCellsAndActorEightCell();
}

void ActorPresentation_RepaintTenCellsAndActorEightCell(void)
{
    struct FieldActor *actor;
    s32 tx;
    s32 tz;
    u32 i;

    actor = Object_GetById(8);
    tx = actor->x.fixed >> 20;
    tz = actor->z.fixed >> 20;

    for (i = 0; i < 20; i += 2) {
        s32 x = (s32)Mura_RepaintCells[i];
        s32 z = (s32)Mura_RepaintCells[i + 1];
        Engine_MapCopyCellAttributes(1, 0, 1, 1, x, z);
    }

    Engine_MapCopyCellAttributes(0, 0, 1, 1, tx, tz);
}

void BiribinoMura_RestoreRaisedEntrance(void)
{
    struct FieldActor *leader;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    FieldScene_DrawTilesByActor8Row();
    if (gGameState.entrance == 3 || gGameState.entrance == 4) {
        if (Engine_GameFlagIsSet(0x109) == 0) {
            leader = Object_GetById(ACTOR_PARTY_LEADER);
            Engine_EventBegin();
            leader->y.fixed = 0x100000;
            Engine_CameraMoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
            Engine_MapRedraw();
            Engine_EventEnd();
            Engine_TaskWait(1);
        }
    }
}

struct FieldActor *SceneActor_FindActorAtCell(s32 x, s32 z)
{
    struct FieldActor **actors = gEventWork->placed_actors - 8;
    u32 i;

    for (i = 8; i <= 65; i++) {
        struct FieldActor *actor = actors[i];

        if (x == actor->x.fixed >> 20 && z == actor->z.fixed >> 20) {
            return actor;
        }
    }
    return 0;
}

void BiribinoMura_PushFacedBlock(void)
{
    struct FieldActor *leader;
    struct FieldActor *block;
    s32 step;
    u32 dir;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;

    leader = Object_GetById(0);
    dir = leader->facing >> 12;
    block = SceneActor_FindActorAtCell(
        (leader->x.part.pixel + (BiribinoMura_FacingCellSteps[dir] >> 16)) >> 4,
        (leader->z.part.pixel + (s16)BiribinoMura_FacingCellSteps[dir]) >> 4);
    if (block != NULL) {
        block->terrain_id = 2;
        /* FAKEMATCH: the probe is filled through a pointer to it; indexing pos directly drops a saved register and moves the step table address (measured). */
        p = pos;
        step = BiribinoMura_FacingCellSteps[dir];
        p[0].fixed = block->x.fixed + (step & -0x10000);
        p[1].fixed = block->y.fixed;
        p[2].fixed = block->z.fixed + (step << 16);
        if (Object_CheckMovementCollision(block, p) <= 0) {
            Object_SetMode(leader, 8);
            Engine_TaskWait(15);
            Engine_AudioPlayCue(185);
            block->speed = 0x3333;
            block->acceleration = 0x3333;
            Engine_ObjectSetPosition(block, p[0].fixed, p[1].fixed, p[2].fixed);
            leader->speed = 0x3333;
            leader->acceleration = 0x3333;
            Engine_ObjectSetPosition(leader, p[0].fixed, p[1].fixed, p[2].fixed);
            Engine_ObjectCommitPosition(block);
            block->x.fixed = p[0].fixed;
            block->z.fixed = p[2].fixed;
            block->velocity_x = 0;
            block->velocity_z = 0;
            Object_SetMode(leader, 1);
            if (gGameState.scene == (s32)&SceneId_BiribinoMura3)
                ActorPresentation_RepaintTenCellsAndActorEightCell();
            else if (gGameState.scene == (s32)&SceneId_BiribinoMura1)
                Scene_UpdatePuzzleActors();
            else if (gGameState.scene == (s32)&SceneId_BiribinoMura2)
                FieldScene_DrawTilesByActor8Row();
        }
    }
}

void BiribinoMura_UpdateCornerSpawn(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(gGameState.selected_actor);
    if (actor->x.fixed < 0x8e0000) {
        if (actor->y.fixed < 0x80000) {
            if (gCornerSpawnCounter.frames == 0)
                OverlayObject_SpawnKind24AtActor(actor);
            if (++gCornerSpawnCounter.frames == 30)
                gCornerSpawnCounter.frames = 0;
        } else {
            gCornerSpawnCounter.frames = 0;
        }
    }
}

void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor)
{
    struct FieldActor *effect;
    struct FieldSprite *sprite;

    effect = Engine_ObjectCreate(24, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    if (effect == NULL)
        return;

    sprite = effect->sprite;
    Engine_ObjectSetScript(effect, Mura_SpawnScript);
    effect->motion_flags = 0;
    effect->terrain_id = 1;
    effect->priority_flags = 2;
    if (sprite == NULL)
        return;

    AnimationObjects_SelectAnimation(sprite, 2);
    sprite->flags = 0;
    sprite->blend_mode = 1;
    sprite->priority = 3;
}

void FieldScene_DrawTilesByActor8Row(void)
{
    struct FieldActor *actor;

    actor = Actor_Get(8);
    if (actor == NULL)
        return;

    if (actor->z.fixed >> 20 == 6)
        Map_CopyCellAttributes(2, 0, 1, 1, 14, 6);
    else
        Map_CopyCellAttributes(0, 0, 1, 1, 14, 6);

    if (actor->z.fixed >> 20 == 9)
        Map_CopyCellAttributes(2, 0, 1, 1, 14, 9);
    else
        Map_CopyCellAttributes(1, 0, 1, 1, 14, 9);
}

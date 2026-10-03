#include "MURA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"

extern const struct SceneEntrance gBiribinoMuraEntrances1[];
extern const struct SceneEntrance gBiribinoMuraEntrances2[];
extern const struct SceneEntrance gBiribinoMuraEntrances3[];
extern const struct SceneEntrance gBiribinoMuraEntrancesOther[];
extern const struct SceneRegion gBiribinoMuraRegions2[];
extern const struct ScenePlacement gBiribinoMuraPlacements1[];
extern const struct ScenePlacement gBiribinoMuraPlacements2[];
extern const struct ScenePlacement gBiribinoMuraPlacements3[];
extern const struct ScenePlacement gBiribinoMuraPlacementsOther[];
extern u8 MsgBiribinoBottomNotVisibleLooksVery[];
extern u8 MsgFieldPeeredWell[];
extern const struct SceneEvent gBiribinoMuraEvents1[];
extern const struct SceneEvent gBiribinoMuraEvents2[];
extern const struct SceneEvent gBiribinoMuraEvents3[];
extern const struct SceneEvent gBiribinoMuraEventsOther[];
extern u8 MsgBiribinoAreaOffLimitsThoseWithout[];
extern u8 MsgBiribinoCurseWasBrokenThanksEfforts[];
extern u8 MsgBiribinoDidSeeTreeAtEntrance[];
extern u8 MsgBiribinoHaveEverBeenVillageImil[];
extern u8 MsgBiribinoHaveTriedHeadingSoutheastFrom[];
extern u8 MsgBiribinoItsTreeButAlmostLooks[];
extern u8 MsgBiribinoJillGaveRobinSpecialGift[];
extern u8 MsgBiribinoMccoysHiddenWarehouseDoNot[];
extern u8 MsgBiribinoThankSavedMeFromBeing[];
extern u8 MsgBiribinoThereTreeLooksLikePerson[];
extern u8 MsgBiribinoWasTurnedIntoTreeFor[];
extern u8 MsgBiribinoYoureGuy[];
void FieldScene_RunScene38b_020008f0(void);
void FieldScene_RunScene38bSequenceA(void);
void FieldScene_RunScene38b_02000d10(void);
void BiribinoMura_UpdateCornerSpawn(void);
s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void ActorPresentation_RepaintTenCellsAndActorEightCell(void);
void Scene_UpdatePuzzleActors(void);
void FieldScene_DrawTilesByActor8Row(void);

/* One cell step per facing sixteenth: x in the high half, z in the low. */
extern s32 BiribinoMura_FacingCellSteps[];

void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor);

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
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

/* Where the party appears in each of Bilibin's three scenes. */
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

/* Only the second scene has map regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoMura2) {
        return gBiribinoMuraRegions2;
    }
    return 0;
}

/* Returns the scene's message table. */
u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

void SceneState_SetValues9_3_0(void)
{
    BattleFx_RunPageEffectForSlot(9, 3, 0);
}

/* The actors placed in each of Bilibin's three scenes. */
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

/* What each of Bilibin's three scenes answers. */
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

void FieldScene_RunScene38b_02000240(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoItsTreeButAlmostLooks);
    if (GameFlag_IsSet(0x301) != 0) {
        bump_step_020001ec(1);
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
    u8 *record;
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
        record = (u8 *)Object_GetById(i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    v5 = (s32)((s32)(*(u16 *)(p7 + 0x16c) - 3) << 16) >> 16;
    if (v5 == 6) {
        Audio_PlayCue(188);
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
    *((u8 *)Object_GetById(0) + 85) = 0;
    *(s32 *)((*(u8 **)&gEventWork + 0x1c0)) = 0x100;
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

    rec = (struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER);
    rec7 = (struct FieldActor *)Object_GetById(11);
    if ((rec7->x.fixed >> 20) == 6) {
        Engine_EventBegin();
        Engine_ActorSetSpritePriority(11, 1);
        Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Engine_EventWait(20);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
        Actor_SetSpeed(11, 0x3333, 0x1999);
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= ~1;
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
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a |= 1;
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

void FieldScene_CallHelper170c(void)
{
    Engine_LeaderCheckAhead();
}

void FieldScene_RunScene38b_02000584(void)
{
    u32 i;
    struct FieldActor *rec7;
    struct FieldActor *record;
    s32 villager_actions;

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
            *((u8 *)Object_GetById(14) + 90) &= 254;
            Actor_WalkToAndWait(14, 0x10a, 172);
            Engine_EventWait(1);
            *((u8 *)Object_GetById(14) + 90) |= 1;
            Engine_EventWait(10);
            Engine_ActorSetAnimationAndWait(14, 3);
            Event_ShowMessageAndWait(14, 0, 10);
            Engine_MessageShowCentered((s32)MsgBiribinoJillGaveRobinSpecialGift, 1);
            bump_step(1);
            Engine_ItemShowFound(ITEM_HARD_NUT, 3);
            Engine_PartyGiveItem(ITEM_HARD_NUT, 0);
            Engine_ActorSetAnimationAndWait(14, 3);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
            Actor_SetSpeed(14, 0x10000, 0x8000);
            *((u8 *)Object_GetById(14) + 90) &= 254;
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
            villager_actions = (s32)Mura_VillagerActions;
            Call3(Object_SetTargetAndCallback, 12, 0x10000, villager_actions);
            Call3(Object_SetTargetAndCallback, 13, 0x10000, villager_actions);
            Call3(Object_SetTargetAndCallback, 14, 0x10000, villager_actions);
            GameFlag_Set(0x849);
            Engine_EventEnd();
        }
    }
}

/* Bilibin's scene start: fade in from the backdrop and run the scene's own
   opening; the third scene also schedules its task. */
s32 Scene_Initialize(void)
{
    /* FAKEMATCH: retained return/prototype call casts preserve the current call lowering. */

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (gGameState.scene == (s32)&SceneId_BiribinoMura1) {
        FieldScene_RunScene38b_020008f0();
    } else {
        if (gGameState.scene == (s32)&SceneId_BiribinoMura3) {
            FieldScene_RunScene38bSequenceA();
            ((void (*)())Engine_TaskAddCallback)((s32)BiribinoMura_UpdateCornerSpawn, 0xc80);
        } else {
            if (gGameState.scene == (s32)&SceneId_BiribinoMura2) {
                FieldScene_RunScene38b_02000d10();
            }
        }
    }
    return 0;
}

void FieldScene_RunScene38b_020008f0(void)
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
    s32 rec7;
    s32 record;
    s32 p6;
    s32 row;

    rec7 = Object_GetById(ACTOR_PARTY_LEADER);
    record = Object_GetById(20);
    row = *(s32 *)(record + 16) >> 20;
    p9 = (*(s32 *)(rec7 + 8) >> 20);
    p10 = (*(s32 *)(rec7 + 16) >> 20);
    p6 = *(s32 *)(record + 8);
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
            *((u8 *)Object_GetById(9) + 89) |= 16;
            *((u8 *)Object_GetById(16) + 89) |= 16;
            *((u8 *)Object_GetById(11) + 89) |= 16;
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

/*
 * Ten (x, z) tile pairs, held in the overlay's own writable image.  Overlay
 * data lives in EWRAM and is deliberately not const.
 */

/*
 * Slot accessor: Object_GetById(slot) returns the actor record, or NULL.
 * Typed as a byte pointer so the +0x08 and +0x10 field reads are explicit.
 */

/*
 * The six-argument renderer ABI: four register arguments plus two stack
 * words, here the tile x and tile z of the cell being repainted.  The two
 * names are separate per-site call words that reach the same renderer.
 */

/*
 * Repaint ten fixed collision cells and then actor 8's own cell.  The
 * 92-byte owner includes the alignment halfword and the single pool word
 * that follows the code; that word holds the address of Mura_RepaintCells, which
 * is in-image data rather than a RAM global.  The two renderer calls must
 * keep their separate call words -- naming one renderer for both changes the
 * displacement emitted at each site.
 */
void ActorPresentation_RepaintTenCellsAndActorEightCell(void)
{
    u8 *actor;
    s32 tx;
    s32 tz;
    u32 i;

    /*
     * Slot 8 is the scene's own actor; the accessor result is not
     * null-checked here.  The 20-bit shift is one signed arithmetic shift:
     * 16 takes the fixed-point coordinate to pixels, the further 4 take it
     * to the 16-pixel tile grid.
     */
    actor = Object_GetById(8);
    tx = *(s32 *)(actor + 0x08) >> 20;
    tz = *(s32 *)(actor + 0x10) >> 20;

    /*
     * Ten fixed cells from the table, then the actor's own cell.  The table
     * is walked by the byte index itself rather than by a 0..9 counter
     * scaled by two, so the loop steps the byte offset directly.
     */
    for (i = 0; i < 20; i += 2) {
        s32 x = (s32)Mura_RepaintCells[i];
        s32 z = (s32)Mura_RepaintCells[i + 1];
        Engine_MapCopyCellAttributes(1, 0, 1, 1, x, z);
    }

    /*
     * The same repaint with 0 rather than 1 in the first argument.  What
     * that selector chooses is not established.
     */
    Engine_MapCopyCellAttributes(0, 0, 1, 1, tx, tz);
}

void FieldScene_RunScene38b_02000d10(void)
{
    /* FAKEMATCH: the inline halfword cell view remains pending a native field review. */

    s32 arg0;
    s32 rec7;
    s32 record;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    FieldScene_DrawTilesByActor8Row();
    record = ReadU16Elem((u16 *)&gGameState, 225);
    if ((u32)((record - 3) << 16) <= 0x10000) {
        if (Engine_GameFlagIsSet(0x109) == 0) {
            rec7 = Object_GetById(ACTOR_PARTY_LEADER);
            Engine_EventBegin();
            arg0 = *(s32 *)(rec7 + 8);
            *(s32 *)(rec7 + 12) = 0x100000;
            Engine_CameraMoveTo(arg0, 0x100000, *(s32 *)(rec7 + 16), 0);
            Engine_MapRedraw();
            Engine_EventEnd();
            Engine_TaskWait(1);
        }
    }
}

s32 *SceneActor_FindAtTileXZ(s32 x, s32 z)
{

    s32 **tbl = (s32 **)((u8 *)gEventWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = tbl[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/* Pushes the block the leader faces one cell ahead when nothing is in the
 * way, walking the leader along with it. */
void BiribinoMura_PushFacedBlock(void)
{
    /* FAKEMATCH: retained return/prototype call casts preserve the current call lowering. */

    struct FieldActor *leader;
    struct FieldActor *block;
    s32 zero;
    s32 step;
    u32 dir;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;

    leader = Object_GetById(0);
    dir = leader->facing >> 12;
    block = (struct FieldActor *)SceneActor_FindAtTileXZ(
        (leader->x.part.pixel + (BiribinoMura_FacingCellSteps[dir] >> 16)) >> 4,
        (leader->z.part.pixel + (s16)BiribinoMura_FacingCellSteps[dir]) >> 4);
    if (block != NULL) {
        zero = 0;
        block->unknown_22 = 2;
        p = pos;
        step = BiribinoMura_FacingCellSteps[dir];
        p[0].fixed = block->x.fixed + (step & -0x10000);
        p[1].fixed = block->y.fixed;
        p[2].fixed = block->z.fixed + (step << 16);
        if (((s32 (*)())Object_CheckMovementCollision)((s32)block, (s32)p) <= 0) {
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
            block->velocity_x = zero;
            block->velocity_z = zero;
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

/* Spawn a kind-24 effect every thirty calls while the selected actor is
 * below both coordinate limits. Only the y limit resets the counter. */
void BiribinoMura_UpdateCornerSpawn(void)
{
    /* FAKEMATCH: halfword aggregates retain the short literal-pool reach. */

    struct FieldActor *actor;

    actor = Object_GetById(gGameState.selected_actor);
    if (actor->x.fixed < 0x8e0000) {
        if (actor->y.fixed < 0x80000) {
            if (gCornerSpawnCounter.frames == 0)
                OverlayObject_SpawnKind24AtActor(actor);
            if (++gCornerSpawnCounter.frames == 30) {
                struct SpawnCounter zero = { 0 };
                gCornerSpawnCounter = zero;
            }
        } else {
            struct SpawnCounter *cnt = &gCornerSpawnCounter;
            struct SpawnCounter zero;
            zero.frames = 0;
            *cnt = zero;
        }
    }
}

/* Spawns the kind-24 effect where the actor stands, drawn translucent
   behind the background layers. */
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
    effect->unknown_22 = 1;
    effect->priority_flags = 2;
    if (sprite == NULL)
        return;

    AnimationObjects_SelectAnimation(sprite, 2);
    sprite->flags = 0;
    sprite->blend_mode = 1;
    sprite->priority = 3;
}

/* Opens the two gate cells on row 14 the actor stands in (z 6 or 9). */
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

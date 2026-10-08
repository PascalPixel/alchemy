#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "imiru.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_service.h"
#include "call.h"
#include "iwram_call.h"
#include "text/msg_ids.h"

#define FieldScene_PrepareActors ((void (*)(u8*))FieldScene_PrepareActors)










s32 OverlayObject_UpdateWobbleByCounter(struct Object *obj)
{
    switch (obj->counter) {
    case 6:
        obj->x += 0xffffc000;
        obj->z += 0x2000;
        break;
    case 4:
        obj->x += 0x2000;
        obj->z -= 0x1000;
        break;
    case 2:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        break;
    case 0:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        if (obj->mode != 0) {
            obj->counter = (u32)Engine_RandomNext() % 40 + 40;
        } else {
            obj->counter = (u32)Engine_RandomNext() % 20 + 20;
        }
        break;
    }
    obj->counter--;
    return 1;
}

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags &= ~1;
        angle = (u16)ArcTan2(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            obj->facing = old + delta;
        }
    }
    return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        return gImiruMuraEntrances2;
    }
    return gImiruMuraEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *ImiruMura_GetExits(void)
{
    return (u8 *)ImiruMura_SceneTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    u8 *script;

    if (gGameState.scene == ((s32)&SceneId_ImiruMura2)) {
        script = gImiruMuraPlacements2;
        FieldScene_PrepareActors(script);
        if (Engine_GameFlagIsSet(0x881) != 0) {
            script[262] = 0;
            *(s32 *)(script + 0x50) = 182 << 16;
            *(s32 *)(script + 0x58) = 564 << 16;
            *(s32 *)(script + 0x4c) = 2;
        }
        return (const struct ScenePlacement *)script;
    }

    if (Engine_GameFlagIsSet(0x881) != 0) {
        return gImiruMuraPlacementsFlag881;
    }
    return gImiruMuraPlacements;
}

void ImiruMura_RunHeadingProbe(void)
{
    StagedActor_RunHeadingProbeStep();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        return gImiruMuraEvents2;
    }
    return gImiruMuraEventsOther;
}

void SceneDialogue_RunActorEightFlagGatedDialogue(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruEvenIfMaryHealsUs));
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruBrrrrrChooImFreezingMary));
    }
    {
        s32 val = 0;
        s32 mode = 8;
        Engine_EventShowMessage(mode, val);
    }
    Engine_EventEnd();
}

void SceneDialogue_ShowLine1571Or152F(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruCantUnderstandWhyAnyOne));
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruMaryShouldHereByNow));
    }
    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor9Line(void)
{
    Engine_EventBegin();
    Engine_ActorFaceActor(9, ACTOR_PARTY_LEADER, 10);
    Engine_EventSetMessage(MSGNO(MsgImiruHiNewInImil));
    Engine_EventAskYesNo(9, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorTenFlag881Dialogue(void)
{
    Engine_EventBegin();

    if (GameFlag_IsSet(0x881) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruCountingAfterDeparture));
        Event_ShowMessage(10, 0);
        Actor_SetAttachedEffect(10, 258);
        Engine_EventWait(40);
        Engine_ActorSetAnimation(10, 1);
        Engine_EventWait(20);
        Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
        Event_AskYesNo(10, 0);
        Actor_FaceDirection(10, 0x3000, 10);
        Engine_ActorSetAnimation(10, 9);
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruOneTwoThreeFour));
        Event_ShowMessage(10, 0);
        Actor_SetAttachedEffect(10, 258);
        Engine_EventWait(40);
        Engine_ActorSetAnimation(10, 1);
        Engine_EventWait(20);
        Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
        Event_ShowMessage(10, 0);
        Actor_FaceDirection(10, 0x3000, 10);
        Engine_ActorSetAnimation(10, 9);
    }

    Engine_EventEnd();
}

void FieldScene_RunSupplementalSequenceOne(void)
{

    void *actor9_record;
    struct FieldActor *actor8_record;

    if (GameFlag_IsSet(2177) != 0) {
        Engine_EventBegin();
        Actor_FaceActor(9, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgMakyuriReallySayDie);
        Event_AskYesNo(9, 0);
        Engine_EventEnd();
    } else {
        if (GameFlag_IsSet(2091) != 0) {
            Engine_EventBegin();
            Engine_ActorSetAnimation(9, 7);
            Engine_MapAnimateCells((s32)ImiruMura_CellStepsA, 10, 69);
            Engine_EventSetMessage((s32)MsgMakyuriFeelMuchBetter);
            Event_ShowMessage(9, 0);
            Engine_ActorSetAnimation(9, 8);
            Map_AnimateCells((s32)ImiruMura_CellStepsB, 10, 69);
            Engine_EventEnd();
        } else {
            Engine_EventBegin();
            actor9_record = Object_GetById(9);
            ((struct MuraSceneRecord *)actor9_record)->field_0x64 = 10;
            Engine_ActorEnableActionCallback(9, ImiruMura_ActorScriptA);
            Engine_EventSetMessage((s32)MsgMakyuriOoohHelpMe);
            Engine_EventShowMessage(9, 0);
            Engine_ActorStop(8);
            Actor_ShowEmote(8, 256, 40);
            Actor_FaceDirection(8, 53248, 10);
            Engine_ActorStartRepeatedMotion(8, 2);
            Event_ShowMessageAndWait(8, 0, 20);
            Engine_ActorEnableActionCallback(0, ImiruMura_ActorScriptC);
            Actor_SetSpeed(8, 104857, 52428);
            Object_SetActionCallbackAndRefreshById(8, (s32)ImiruMura_ActorScriptB);
            Engine_EventWait(40);
            Engine_ActorJump(8, 2, 0);
            Engine_ActorStartRepeatedMotion(8, 2);
            Engine_ActorSetAttachedEffect(8, 258);
            Engine_EventWait(60);
            Event_ShowMessageAndWait(8, 0, 10);
            Actor_FaceDirection(8, 12288, 20);
            Engine_ActorStartRepeatedMotion(8, 2);
            Engine_EventShowMessage(8, 0);
            actor8_record = Object_GetById(8);
            *(u8 *)((u8 *)(actor8_record) + ACTOR_FLAGS_OFFSET) ^= 0x2;
            GameFlag_Set(0x82c);
            Engine_EventEnd();
        }
    }
}

void SceneDialogue_RunActor12Line(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage(MSGNO(MsgImiruDoWantWeapons));
    Engine_EventAskYesNo(12, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor18Line(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage(MSGNO(MsgImiruDoWantSeeRestaurantMenu));
    Engine_EventAskYesNo(18, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor20BranchScene(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruEveryoneCountsOnMaryThats));
        Engine_EventShowMessage(20, 0);
    } else {
        Engine_EventSetMessage((s32)MsgImiruHaveVisitedOldCoupleWho);
        Engine_EventAskYesNo(20, 0);
        Engine_GameFlagSet(0x82a);
        Engine_GameFlagSet(0x82c);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor20FlaggedLine(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruWeHaveDoWhateverWe));
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruMaryWasSayingSheHas));
    }
    Engine_EventShowMessage(20, 0);
    Engine_EventEnd();
}

void ImiruMura_TalkAboutLighthouse(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage(MSGNO(MsgImiruHappenedInLighthouseNortheast));
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(8, 0x3000, 10);
    Engine_EventEnd();
}

void SceneDialogue_RunActorEightBranchedDialogue(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x82b) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruMaryClanOnceLivedHere));
    } else if (Engine_GameFlagIsSet(0x82c) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruHesAlwaysExaggeratingThingsBut));
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruMaryRunningAroundTownCaring));
    }
    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}

void FieldScene_RunSingleStep(void)
{
    FieldScene_RunSupplementalSequenceOne();
}

void SceneDialogue_ShowLine156E(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage(MSGNO(MsgImiruMuraMaryIsAGoodGirlI));
    Engine_EventShowMessage(10, 0);
    Engine_EventEnd();
}

void SceneDialogue_ShowLine1573Or155A(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_EventSetMessage(MSGNO(MsgImiruFeelLikeGrownUpWhen));
    } else {
        Engine_EventSetMessage(MSGNO(MsgImiruTheseFolkOkayTheyDont));
    }
    Engine_EventShowMessage(19, 0);
    Engine_EventEnd();
}

void ImiruMura_RunExitDoor(void)
{
    struct EventWork *event;
    struct FieldActor *actor;
    u32 i;
    s32 exit;

    event = gEventWork;
    Engine_EventBegin();
    for (i = 8; i <= 65; i++) {
        actor = Object_GetById(i);
        if (actor != NULL) {
            actor->motion_flags = 0;
        }
    }
    exit = (s16)(event->touched_trigger - 50);
    if (exit == 6) {
        Engine_AudioPlayCue(188);
    } else {
        Engine_AudioPlayCue(158);
    }
    {
        s32 x = ImiruMura_ExitCellPoints[exit - 1][0];
        s32 y = ImiruMura_ExitCellPoints[exit - 1][1];

        Engine_MapAnimateCells(ImiruMura_ExitCellSteps[exit - 1], x, y);
    }
    Actor_SetSpeed(0, 0x8000, 0x4000);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (exit == 6) {
        Actor_SetSpeed(0, 0x3333, 0x1999);
        Engine_ActorSetAnimation(0, 2);
        Engine_ActorSetSpritePriority(0, 3);
        Actor_SetDestinationOffset(0, 0, -8);
    } else {
        Object_GetById(0)->motion_flags = 0;
        Actor_CenterAndWalk(0, 3, -16);
    }
    Engine_EventWait(16);
    Engine_EventRequestExit(exit);
    Engine_EventEnd();
}

void ImiruMura_RunWeaponShop(void)
{
    s32 dir;

    dir = *(s16 *)(((s32 (*)())Object_GetById)(0) + 6);
    if (Engine_GameFlagIsSet(0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(10, 12);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(12, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgMakyuriCatchUpLostOpportunity);
        Engine_EventShowMessage(12, 0);
        Actor_FaceDirection(12, 0x4000, 10);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Engine_CameraSetSpeed(0x60000, 0xc000);
            Camera_MoveTo(0x1aa0000, -1, 0x1ec0000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(20);
            Object_SetActionCallbackAndRefreshById(12, (s32)ImiruMura_TurnScript);
            Engine_EventSetMessage((s32)MsgMakyuriStoreClosedUntilWell);
            Engine_EventShowMessage(12, 0);
            Engine_CameraMoveTo(0x1aa0000, -1, 0x2680000, 1);
            Engine_CameraWaitForMove();
            Engine_EventEnd();
        }
    }
}

void ImiruMura_RunArmorShop(void)
{
    s32 dir;

    dir = *(s16 *)(((s32 (*)())Object_GetById)(0) + 6);
    if (Engine_GameFlagIsSet(0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(11, 13);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(13, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgMakyuriShameStoreClosed);
        Engine_EventShowMessage(13, 0);
        Actor_FaceDirection(13, 0x4000, 10);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Engine_CameraSetSpeed(0x60000, 0xc000);
            Camera_MoveTo(0x1aa0000, -1, 0x1ec0000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(20);
            Object_SetActionCallbackAndRefreshById(13, (s32)ImiruMura_TurnScript);
            Engine_EventSetMessage((s32)MsgMakyuriTooSickBusiness);
            Engine_EventShowMessage(13, 0);
            Engine_CameraMoveTo(0x1aa0000, -1, 0x2680000, 1);
            Engine_CameraWaitForMove();
            Engine_EventEnd();
        }
    }
}

void ImiruMura_RunItemShop(void)
{
    s32 dir;

    dir = *(s16 *)(((s32 (*)())Object_GetById)(0) + 6);
    if (Engine_GameFlagIsSet(0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(12, 15);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(15, 0, 0);
        Engine_EventSetMessage((s32)MsgMakyuriRecoveredThanksFountain);
        Engine_EventShowMessage(15, 0);
        Actor_FaceDirection(15, 0x4000, 0);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Engine_EventSetMessage((s32)MsgMakyuriMomsSickGetWhatever);
            Engine_EventShowMessage(14, 0);
            ((s32 (*)())Engine_ShopOpen)(12, 14);
            Engine_EventEnd();
        } else {
            Engine_ActorFaceActor(14, 0, 10);
            Engine_EventSetMessage((s32)MsgMakyuriDadLumberjack);
            Engine_EventShowMessage(14, 0);
            Actor_FaceDirection(14, 0x5000, 10);
        }
    }
}

void ImiruMura_OpenInn(void)
{
    struct FieldActor *leader;

    leader = Object_GetById(0);
    if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
        Engine_InnOpen(4, 16);
    } else {
        Engine_EventBegin();
        Actor_FaceActor(16, ACTOR_PARTY_LEADER, 10);
        if (GameFlag_IsSet(0x881) != 0) {
            Engine_EventSetMessage(MSGNO(MsgImiruItsAlmostTimeForLeave));
            Event_AskYesNo(16, 0);
        } else {
            Engine_EventSetMessage(MSGNO(MsgImiruWhyHaveTwoGroupsTravelers));
            Event_ShowMessage(16, 0);
        }
        Actor_FaceDirection(16, 0x3000, 10);
        Engine_EventEnd();
    }
}

void ImiruMura_OpenSanctum(void)
{
    struct FieldActor *leader;

    leader = Object_GetById(0);
    if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
        Engine_EventBegin();
        if (GameFlag_IsSet(0x82d) == 0) {
            Engine_EventSetMessage(MSGNO(MsgImiruMayOnlyStudentButCan));
            Event_ShowMessage(19, 0);
            GameFlag_Set(0x82d);
        }
        Engine_EventEnd();
        Engine_SanctumOpen(19);
    } else {
        Engine_EventBegin();
        if (GameFlag_IsSet(0x881) != 0) {
            Engine_EventSetMessage(MSGNO(MsgImiruMaryGoingOnJourneyWith));
            Event_ShowMessage(19, 0);
        } else if (GameFlag_IsSet(3) != 0) {
            Engine_EventSetMessage(MSGNO(MsgImiruAmHealerWhileMaryOut));
            Event_ShowMessage(19, 0);
        } else {
            Engine_EventSetMessage(MSGNO(MsgImiruLookingForMary));
            (void)Event_AskYesNo(19, 0);
            Actor_FaceDirection(19, 0x3000, 10);
        }
        Engine_EventEnd();
    }
}

s32 ImiruMura_ApplyEntryState(void)
{
    struct FieldActor *leader;
    s32 entrance;

    if (gGameState.scene == (s32)&SceneId_ImiruMura1) {
        leader = Object_GetById(0);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        Engine_ActorSetAnimation(10, 9);
        if (GameFlag_IsSet(0x109)) {
            Engine_GameFlagClear(0x200);
            Engine_GameFlagClear(0x201);
        }
        leader->timer = 0;
        leader->work = 0;
        Task_AddCallback(FieldScene_RunScene399SequenceA, 0xc80);
        Engine_TaskAddCallback(SceneState_UpdateZoneFlagsFromActorZero, 0xc80);
        Engine_ActorSetSpritePriority(11, 1);
        if (Engine_GameFlagIsSet(0x203)) {
            SceneState_UpdateActor11WithFlag203();
        }
        if (!Engine_GameFlagIsSet(0x109) && gGameState.entrance == 9) {
            RunEventScript02();
        }
    } else if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        entrance = gGameState.entrance;
        if (entrance == 1) {
            Engine_ActorSetChildValue(21, 15);
            Object_GetById(21)->collision_flags |= 8;
            Engine_ActorSetSpritePriority(21, 1);
            if (Engine_GameFlagIsSet(0x881)) {
                Map_CopyCellAttributes(10, 7, 1, 1, 10, 8);
                Engine_MapCopyCellsTo(3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                Engine_ActorEnableActionCallback(8, (const u8 *)2);
                Engine_ActorSetPosition(10, 0, 0);
            } else if (Engine_GameFlagIsSet(0x82c) && Engine_GameFlagIsSet(0x82a)) {
                Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
                Engine_ActorSetPosition(9, 0xae0000, 0xa40000);
                Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
                Engine_ActorSetAnimation(9, 5);
                Actor_SetPosition(8, 0xa80000, 0x980000);
                Object_GetById(8)->facing = 0x3000;
                if (!Engine_GameFlagIsSet(0x82b)) {
                    FieldScene_RunPrimaryScriptChoreography();
                }
            } else {
                Map_CopyCellAttributes(10, 7, 1, 1, 10, 8);
                Engine_MapCopyCellsTo(3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                if (Engine_GameFlagIsSet(0x82c)) {
                    Engine_ActorSetPosition(8, 0x950000, 0x740000);
                    Object_GetById(8)->facing = 0;
                    Object_GetById(9)->work = 0;
                    Engine_ActorEnableActionCallback(9, ImiruMura_ActorScriptA);
                } else {
                    Engine_ActorEnableActionCallback(8, (const u8 *)2);
                }
            }
        } else if (entrance == 2) {
            if (!Engine_GameFlagIsSet(0x881)) {
                Object_GetById(11)->work = 1;
                Engine_ActorEnableActionCallback(11, ImiruMura_ActorScriptA);
            }
        } else if (entrance == 4) {
            if (Engine_GameFlagIsSet(0x881)) {
                Actor_SetPosition(12, 0x16c0000, 0x2420000);
                Engine_ActorSetSpritePriority(12, 2);
                Object_GetById(12)->collision_flags |= 4;
                Engine_MapCopyCellsTo(6, 125, 22, 88, 3, 3);
                Actor_SetPosition(13, 0x1ec0000, 0x2420000);
                Engine_ActorSetSpritePriority(13, 2);
                Object_GetById(13)->collision_flags |= 4;
                Engine_MapCopyCellsTo(9, 125, 28, 88, 3, 3);
            } else {
                Object_GetById(12)->scale_x = -0x10000;
                Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
                Engine_ActorSetAnimation(12, 5);
                Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
                Engine_ActorSetAnimation(13, 5);
            }
        } else if (entrance == 3) {
            if (Engine_GameFlagIsSet(0x881)) {
                Actor_SetPosition(15, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(15, 2);
                Object_GetById(15)->collision_flags |= 4;
                Engine_ActorSetPosition(14, 0x1980000, 0x1080000);
                Object_GetById(14)->facing = 0x1000;
                Engine_MapCopyCellsTo(12, 125, 26, 70, 3, 3);
            } else {
                Actor_SetPosition(14, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(14, 2);
                Object_GetById(14)->collision_flags |= 4;
                Object_GetById(15)->scale_x = -0x10000;
                Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
                Engine_ActorSetAnimation(15, 5);
            }
        } else if (entrance == 7) {
            if (Engine_GameFlagIsSet(0x881)) {
                Object_GetById(20)->facing = 0x3000;
                if (!Engine_GameFlagIsSet(0x82e)) {
                    Actor_SetPosition(20, 0x28a0000, 0xa10000);
                    FieldScene_RunThreeActorChoreography();
                } else {
                    Actor_SetPosition(20, 0x2840000, 0xa60000);
                }
            }
        }
    }
    return 0;
}

void FieldScene_RunPrimaryScriptChoreography(void)
{
    struct FieldActor *leader;
    s32 tbl;
    struct EventWork *work;

    Engine_EventBegin();
    Actor_SetPosition(MEARI, 0xb60000, 0x960000);
    Camera_MoveTo(0x8d0000, -1, 0xdd0000, 0);
    Engine_TaskWait(1);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(0x8c0000, -1, 0xa40000, 1);
    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 40;
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_SetSpeed(GERALD, 0x6666, 0x3333);
    Actor_SetSpeed(IWAN, 0x6666, 0x3333);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 142, 221);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    leader = Object_GetById(0);
    if (leader != NULL) {
        Actor_SetPosition(GERALD, leader->x.fixed, leader->z.fixed);
    }
    leader = Object_GetById(0);
    if (leader != NULL) {
        Actor_SetPosition(IWAN, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkTo(GERALD, 150, 234);
    Actor_WalkToAndWait(IWAN, 134, 234);
    Engine_ActorSetAnimation(GERALD, 1);
    tbl = (s32)ImiruMura_PrimaryScript;
    Call3(Engine_ObjectSetTargetAndCallback, 0, 0x10003, tbl);
    Call3(Engine_ObjectSetTargetAndCallback, 1, 0x10003, tbl);
    Call3(Engine_ObjectSetTargetAndCallback, 2, 0x10003, tbl);
    Engine_CameraWaitForMove();
    tbl = (s32)ImiruMura_TurnScript;
    Engine_ActorEnableActionCallback(9, tbl);
    Engine_EventWait(40);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Engine_EventSetMessage(MSGNO(MsgImiruHowFeeling));
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Engine_ActorEnableActionCallback(9, tbl);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(MEARI, 0x8000, 20);
    Actor_FaceDirection(8, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 40);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(10);
    Actor_FaceDirection(MEARI, 0x4000, 0);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Engine_ActorEnableActionCallback(9, tbl);

    ImiruMura_CastPsynergyAtActorNine();
    Actor_ShowEmote(MEARI, 0x101, 60);
    Event_ShowMessageAndWait(MEARI, 0, 40);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Engine_ActorSetAnimation(9, 7);
    Map_AnimateCells((s32)ImiruMura_CellStepsA, 10, 69);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Engine_ActorSetAnimationAndWait(MEARI, 4);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(9, 8);
    Map_AnimateCells((s32)ImiruMura_CellStepsB, 10, 69);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(8, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(MEARI, 0x101, 30);
    Actor_FaceDirection(MEARI, 0x8000, 10);
    Engine_ActorSetAnimation(MEARI, 4);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(40);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    ((struct Work_399 *)((s32)Object_GetById(3)))->f100 = 0;
    Engine_ActorEnableActionCallback(MEARI, ImiruMura_MiaScriptA);
    while (*(s16 *)(((s32)Object_GetById(3)) + ACTOR_DONE_OFFSET) == 0) {
        Engine_TaskWait(1);
    }
    Camera_MoveTo(0x8c0000, -1, 0xc60000, 1);
    Object_RefreshSelectorById(3);
    Actor_ShowEmote(MEARI, 0x101, 80);
    Event_ShowMessageAndWait(MEARI, 0, 40);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Engine_EventWait(10);
    Event_ShowMessage(MEARI, 0);
    Event_PlayCue(131);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x207e9f, 0);
    Engine_ColorBufferInterpolate(10);
    Engine_TaskWait(1);
    Event_PlayCue(220);
    Engine_TaskWait(40);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(20);
    Actor_FaceDirection(MEARI, 0, 10);
    Actor_SetSpeed(MEARI, 0x20000, 0x10000);
    Actor_WalkToAndWait(MEARI, 202, 198);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Event_ShowMessage(MEARI, 0);
    Engine_ActorSetAnimationAndWait(MEARI, 4);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(MEARI, 0, 40);
    Actor_ShowEmote(MEARI, 0x100, 40);
    Event_ShowMessage(MEARI, 0);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(GERALD);
    Engine_ActorStop(IWAN);
    Actor_SetSpeed(MEARI, 0x30000, 0x18000);
    ((struct Work_399 *)((s32)Object_GetById(3)))->f100 = 0;
    Engine_ActorEnableActionCallback(MEARI, ImiruMura_MiaScriptB);
    while (*(s16 *)(((s32)Object_GetById(3)) + ACTOR_DONE_OFFSET) == 0) {
        Engine_TaskWait(1);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(GERALD, 0x4000, 0);
    Actor_FaceDirection(IWAN, 0x4000, 10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
    Actor_SetSpeed(GERALD, 0x40000, 0x20000);
    Actor_SetSpeed(IWAN, 0x40000, 0x20000);
    Event_PlayCue(152);
    (Object_GetById(0))->steering_flags &= ~ACTOR_FACES_MOTION;
    (Object_GetById(1))->steering_flags &= ~ACTOR_FACES_MOTION;
    (Object_GetById(2))->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_SetDestination(ACTOR_PARTY_LEADER, 132, 206);
    Actor_SetDestination(GERALD, 136, 221);
    Actor_SetDestination(IWAN, 122, 238);
    Object_RefreshSelectorById(3);
    Engine_EventWait(80);
    (Object_GetById(0))->steering_flags |= ACTOR_FACES_MOTION;
    (Object_GetById(1))->steering_flags |= ACTOR_FACES_MOTION;
    (Object_GetById(2))->steering_flags |= ACTOR_FACES_MOTION;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
    tbl = (s32)ImiruMura_PrimaryScript2;
    Engine_ActorEnableActionCallback(GERALD, tbl);
    Object_SetActionCallbackAndRefreshById(2, tbl);
    Engine_EventWait(20);
    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    work->transition_frames = 24;
    Engine_GameFlagSet(0x82b);
    Engine_EventEnd();
}

void SceneActor_UpdateCountdownArcPosition(T_0200154c *o)
{
    s32 buf[3];
    s32 *b;
    s32 n;
    s32 t;

    if (o != 0) {
        n = o->unk64 - 1;
        o->unk64 = n;
        t = (s16)n;
        if (t != 0) {
            b = buf;
            b[0] = ImiruMura_ArcOrigin[0];
            b[1] = ImiruMura_ArcOrigin[1] + 0x80000;
            b[2] = ImiruMura_ArcOrigin[2];
            Vector_AddPolarOffset(t << 16, (t << 11) + o->unk66, b);
            o->unk8 = b[0];
            o->unkC = b[1];
            o->unk10 = b[2];
        } else {
            Engine_ObjectDispatchRelease(o);
        }
    }
}


void ImiruMura_SwayAndSpark(struct FieldActor *actor)
{
    s32 pos[3];
    struct FieldActor *spark;
    s32 radius;

    actor->x.fixed = ImiruMura_ArcOrigin[0] + Iwram_MulQ16(0x60000, Engine_MathSin((s16)actor->timer << 10));
    actor->timer++;
    actor->timer = ((s16)actor->timer + 64) % 64;
    if (gFrameCount % 3 == 0) {
        pos[0] = actor->x.fixed;
        pos[1] = actor->y.fixed + 0x20000;
        pos[2] = actor->z.fixed;
        radius = Engine_RandomNext();
        Vector_AddPolarOffset(radius * 6, Engine_RandomNext(), pos);
        spark = Engine_ObjectCreate(0x11d, pos[0], pos[1], pos[2]);
        if (spark != NULL) {
            spark->sprite->priority = 0;
            Engine_ActorSetSpriteFlags(spark, 0);
            Object_SetMode(spark, 1);
            spark->scale_x = 0x9999;
            spark->scale_y = 0x9999;
            spark->priority_flags = 2;
            spark->motion_flags = 0;
            ObjectGroup_SetChildValue(spark, 9);
            Engine_ObjectSetScript(spark, ImiruMura_SparkScript);
        }
    }
}

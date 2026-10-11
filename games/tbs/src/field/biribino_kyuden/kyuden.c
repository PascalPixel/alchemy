#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "kyuden.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"

#define FieldScene_PrepareActors ((void (*)(u8*))FieldScene_PrepareActors)


struct KyudenSceneRecord {
    u8 unk_000[166];
    u8 field_166;
    u8 unk_167[23];
    u8 field_190;
    u8 unk_191[23];
    u8 field_214;
    u8 unk_215[23];
    u8 field_238;
};








s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8)(0xFE & obj->facing_flags);
        angle = (u16)ArcTan2(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }

            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (u16)(old + delta);
        }
    }
    return 1;
}

u8 *SceneData_GetScriptTable(void)
{
    return Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    u8 *p;

    if (gGameState.scene == (s32)&SceneId_BiribinoKyuden) {
        p = gBiribinoKyudenPlacements;
        FieldScene_PrepareActors(p);

        if (Engine_GameFlagIsSet(FLAG_REWARD_TAKEN) != 0) {
            struct KyudenSceneRecord *rec = (struct KyudenSceneRecord *)p;

            rec->field_166 = 2;
            rec->field_190 = 0;
            rec->field_214 = 3;
            rec->field_238 = 1;
        }

        return (const struct ScenePlacement *)p;
    }
    return gBiribinoKyudenPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoKyuden) {
        return gBiribinoKyudenEvents;
    }
    return gBiribinoKyudenEventsOther;
}

void Kyuden_AskAboutKolima(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoYouWillingGoKolimaForest);
    Engine_EventAskYesNo(10, 0);
    Engine_EventEnd();
}

void FieldScene_RunStartledGuard(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_ShowEmote(14, 0x102, 0);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(40);
    Engine_EventSetMessage((s32)MsgBiribinoAaaah);
    Event_ShowMessageAndWait(14, 0, 20);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_FaceDirection(14, 0xb000, 10);
    Engine_EventEnd();
}

void FieldScene_RunScene38dSequenceA(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoFineWarrior);
    if (GameFlag_IsSet(0x302) != 0) {
        Engine_EventSetMessage((s32)MsgBiribinoItsVeryRecklessForSuch);
    }
    Event_ShowMessage(15, 0);
    GameFlag_Set(0x302);
    Engine_EventEnd();
}

void SceneDialogue_RunActor16Message1769(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoWasWatchingFromHereAfter);
    Engine_EventAskYesNo(16, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorSeventeenFlaggedDialogue(void)
{
    Engine_EventBegin();

    if (Engine_GameFlagIsSet(0x202) != 0) {
        Engine_EventSetMessage((s32)MsgBiribinoMayChooseOnlyOneItem);
    } else if (Engine_GameFlagIsSet(FLAG_REWARD_TAKEN) != 0) {
        Engine_EventSetMessage((s32)MsgBiribinoRewardReceivedWasIndeedGreatest);
    } else {
        Engine_EventSetMessage((s32)MsgBiribinoDoThinkTheseBilibinsGreat);
        if (Engine_GameFlagIsSet(0x84d) != 0) {
            gEventWork->message++;
        }
    }

    Engine_EventShowMessage(17, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor15Flag303Scene(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoSometimesWeNeedChildrenRemind);
    if (GameFlag_IsSet(0x303) != 0) {
        Engine_EventSetMessage((s32)MsgBiribinoLookedVeryCourageousWalkingToward);
    }
    Event_ShowMessage(15, 0);
    GameFlag_Set(0x303);
    Engine_EventEnd();
}

void FieldScene_RunActorSeventeenFlagDialogue(void)
{
    Engine_EventBegin();

    if (Engine_GameFlagIsSet(0x202) != 0) {
        Engine_EventSetMessage((s32)MsgBiribinoNonethelessIfYourLuckSour);
    } else if (Engine_GameFlagIsSet(0x845) == 0) {
        Engine_EventSetMessage((s32)MsgBiribinoGoodTreasureIfGetTurned);
    } else {
        Engine_EventSetMessage((s32)MsgBiribinoWeReallyGivingOurTreasure);
        if (Engine_GameFlagIsSet(FLAG_REWARD_TAKEN) != 0) {
            Engine_EventSetMessage((s32)MsgBiribinoGotPrettyNiceRewardBut);
        }
    }

    Engine_EventShowMessage(17, 0);
    Engine_EventEnd();
}

void BiribinoKyuden_RunDoorExitScene(void)
{
    struct EventWork *work;
    s32 lock;

    work = gEventWork;
    lock = 0;
    if (work->touched_trigger == 9) {
        if (Engine_GameFlagIsSet(0x200) == 0) {
            Engine_AudioPlayCue(188);
            lock = 1;
        }
    } else {
        Engine_AudioPlayCue(158);
        lock = 1;
    }
    if (lock != 0) {
        Map_ClearLayerEntryFlag(1);
        Map_ClearLayerEntryFlag(2);
    }
    Engine_EventBegin();
    Engine_EventWait(10);
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Engine_ActorSetAnimation(0, 2);
    if (work->touched_trigger == 9)
        Actor_SetDestinationOffset(0, 0, -16);
    else
        Actor_CenterAndWalk(0, 3, -16);
    Engine_EventWait(16);
    Engine_EventRequestExit(work->touched_trigger);
    Engine_EventEnd();
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
}

void FieldScene_ShowChestEmpty(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgBiribinoRobinCheckedChestButWas, 1);
    Engine_EventEnd();
}

void FieldScene_ShowChestLocked(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgBiribinoTreasureChestLocked, 1);
    Engine_EventEnd();
}

void FieldScene_RunChestStep(s32 flag)
{
    if (Engine_GameFlagIsSet(flag) != 0) {
        FieldScene_ShowChestEmpty();
    } else {
        FieldScene_ShowChestLocked();
    }
}

void FieldScene_RunStep210ByFlag84e(void)
{
    if (Engine_GameFlagIsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x210);
    } else {
        FieldScene_RunSlotSubjectBranch(21, 182, 0x210);
    }
}

void FieldScene_RunStep211ByFlag84e(void)
{
    if (Engine_GameFlagIsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x211);
    } else {
        FieldScene_RunSlotSubjectBranch(22, 183, 0x211);
    }
}

void FieldScene_RunStep212ByFlag84e(void)
{
    if (Engine_GameFlagIsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x212);
    } else {
        FieldScene_RunSlotSubjectBranch(23, 186, 0x212);
    }
}

void FieldScene_RunStep213ByFlag84e(void)
{
    if (Engine_GameFlagIsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x213);
    } else {
        FieldScene_RunSlotSubjectBranch(24, 189, 0x213);
    }
}

void FieldScene_RunSlotSubjectBranch(s32 actor, s32 item, s32 flag)
{
    s32 record;

    Engine_EventBegin();

    record = BattleFx_PlayCueAndStartEmitterOnTarget(0, actor, item);

    if (Engine_PartyGiveItem(item, 0) != -1) {
        Engine_ActorSetAnimation(actor, 2);
        Engine_GameFlagSet(FLAG_REWARD_TAKEN);
        Engine_GameFlagSet(flag);
        Engine_GameFlagClear(0x322);
        Engine_GameFlagClear(0x202);
    } else {
        Engine_AudioPlayCue(125);
        Engine_ActorSetAnimation(actor, 5);
    }

    Engine_ObjectDispatchRelease(record);
    Engine_EventEnd();
}

void FieldScene_RunRewardReminder(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) == 0) {
        if (Engine_GameFlagIsSet(0x322) != 0) {
            Engine_EventBegin();
            Actor_ShowEmote(19, 0x100, 0);
            Actor_FaceDirection(19, 0x7000, 10);
            Engine_ActorRunRepeatedMotion(19, 2);
            Engine_EventWait(20);
            Engine_EventSetMessage((s32)MsgBiribinoPleaseTakeYourRewardBefore);
            Event_ShowMessage(19, 0);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x268, 0x2fa);
            Actor_FaceDirection(19, 0xd000, 10);
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunPalaceFarewell(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Engine_EventBegin();
        Actor_FaceActor(ACTOR_PARTY_LEADER, 19, 0);
        Actor_SetSpeed(19, 0x9999, 0x4ccc);
        Actor_WalkToAndWait(19, 0x26e, 0x2fc);
        Actor_FaceDirection(19, 0xf000, 20);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_ActorSetAnimationAndWait(17, 3);
        Engine_EventWait(20);
        Actor_FaceActor(19, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventSetMessage((s32)MsgBiribinoAlwaysWelcomeInPalaceLord);
        Event_ShowMessageAndWait(19, 0, 10);
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Actor_WalkToAndWait(19, 0x23a, 0x2f6);
        Actor_SetPosition(19, 0, 0);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        GameFlag_Set(0x85e);
        GameFlag_Set(0x333);
        Engine_EventEnd();
    }
}

s32 BiribinoKyuden_ApplyEntryState(s32 a0, s32 a1)
{
    u32 i;
    s32 rec8;
    s32 record;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Engine_GameFlagSet(0x84b);
    if (Engine_GameFlagIsSet(0x109) != 0) {
        Engine_GameFlagClear(0x200);
    }
    if (Engine_GameFlagIsSet(0x84f) == 0) {
        record = Engine_GameFlagIsSet(0x845);
        if (record != 0) {
            goto L_02000758;
        }
        if (gGameState.entrance == 29) {
            Kyuden_RunKolimaRequest();
            goto L_02000888;
        }
        if (gGameState.entrance != 9) {
            goto L_02000888;
        }
        if (Engine_GameFlagIsSet(0x321) == 0) {
            goto L_02000888;
        }
        FieldScene_RunPalaceGreeting();
    } else {
        L_02000758:;
        rec8 = Engine_GameFlagIsSet(0x84e);
        if (rec8 != 0) {
        } else {
            if (gGameState.entrance == 29) {
                if (Engine_GameFlagIsSet(0x85e) != 0) {
                    goto L_02000888;
                }
                record = Engine_GameFlagIsSet(0x845);
                if (record == 0) {
                    goto L_02000888;
                }
                RunEventScript02();
            } else {
                if (gGameState.entrance == 28) {
                    if (Engine_GameFlagIsSet(0x322) != 0) {
                        if (Engine_GameFlagIsSet(0x109) != 0) {
                            Engine_MapCopyCellAttributes(38, 55, 4, 1, 38, 45);
                            Engine_MapCopyCellAttributes(42, 55, 4, 1, 38, 46);
                            Actor_SetPosition(21, 0x2680000, 0x2d80000);
                            Actor_SetPosition(22, 0x2780000, 0x2d80000);
                            Actor_SetPosition(23, 0x2880000, 0x2d80000);
                            Engine_ActorSetPosition(24, 0x2980000, 0x2d80000);
                            record = (s32)Object_GetById(21);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            record = (s32)Object_GetById(22);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            record = (s32)Object_GetById(23);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            record = (s32)Object_GetById(24);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            Object_GetById(21)->motion_flags = rec8;
                            Object_GetById(22)->motion_flags = rec8;
                            Object_GetById(23)->motion_flags = rec8;
                            Object_GetById(24)->motion_flags = rec8;
                            record = (s32)Object_GetById(21);
                            *(s32 *)(record + 12) = -0x40000;
                            record = ((s32 (*)())Object_GetById)(22);
                            *(s32 *)(record + 12) = -0x40000;
                            record = ((s32 (*)())Object_GetById)(23);
                            *(s32 *)(record + 12) = -0x40000;
                            record = ((s32 (*)())Object_GetById)(24);
                            *(s32 *)(record + 12) = -0x40000;
                        } else {
                            BiribinoKyuden_RunActorRowScene();
                        }
                    }
                }
            }
        }
    }
    L_02000888:;
    return 0;
}

void Kyuden_RunKolimaRequest(void)
{
    struct FieldActor *record;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 0);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    if (GameFlag_IsSet(0x85f) != 0) {
        Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 0);
        Actor_SetPosition(19, PIXELS(876), PIXELS(634));
        Actor_FaceDirection(19, FACING_SOUTHEAST + FACING_STEP, 0);
        Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(894), PIXELS(798));
    }
    Engine_MapRedraw();
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 40;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    if (Engine_GameFlagIsSet(0x85f) == 0) {
        Engine_EventWait(80);
        Actor_SetPosition(19, PIXELS(894), PIXELS(798));
        Camera_SetSpeed(0x9999, 0x1333);
        Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Actor_WalkTo(19, 894, 696);
        Engine_EventWait(80);
        Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
        Engine_ActorWaitForMove(19);
        Actor_WalkToAndWait(19, 842, 696);
        Actor_WalkToAndWait(19, 842, 636);
        Actor_FaceDirection(18, FACING_SOUTHWEST + FACING_STEP, 20);
        Actor_WalkToAndWait(19, 876, 634);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(18, 3);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgBiribinoMatter);
        Event_ShowMessageAndWait(0x2012, 0, 10);
        Engine_ActorRunRepeatedMotion(19, 2);
        Engine_EventShowMessageAndWait(19, 0, 20);
        Engine_ActorRunRepeatedMotion(18, 1);
        Event_ShowMessageAndWait(0x2012, 0, 10);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(40);
        Actor_ShowEmote(18, 0x105, 60);
        Event_ShowMessage(0x2012, 0);
        Engine_ActorRunRepeatedMotion(18, 1);
        Event_ShowMessageAndWait(0x2012, 0, 10);
        Engine_ActorSetAttachedEffect(19, 0x102);
        Engine_EventWait(60);
        Actor_FaceDirection(19, FACING_SOUTHEAST + FACING_STEP, 10);
        Actor_FaceDirection(18, FACING_SOUTH + FACING_STEP, 10);
        Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
        Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(894), PIXELS(798));
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 726);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(18, 1);
        Engine_EventShowMessage(0x2012, 0);
        Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
        Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Engine_ActorSetPosition(GERALD, record->x.fixed, record->z.fixed);
        }
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Engine_ActorSetPosition(IWAN, record->x.fixed, record->z.fixed);
        }
        if (Engine_GameFlagIsSet(3) != 0) {
            record = Object_GetById(ACTOR_PARTY_LEADER);
            if (record != 0) {
                Engine_ActorSetPosition(MEARI, record->x.fixed, record->z.fixed);
            }
        }
        Actor_SetSpeed(GERALD, 0x9999, 0x4ccc);
        Actor_SetSpeed(IWAN, 0x9999, 0x4ccc);
        Actor_SetSpeed(MEARI, 0x10000, 0x8000);
        Engine_ActorSetAnimation(GERALD, 2);
        Engine_ActorSetAnimation(IWAN, 2);
        Engine_ActorSetAnimation(MEARI, 2);
        Actor_SetDestinationOffset(GERALD, -16, 16);
        Engine_ActorSetDestinationOffset(IWAN, 16, 16);
        if (Engine_GameFlagIsSet(3) != 0) {
            Engine_ActorSetDestinationOffset(MEARI, 32, 16);
        }
        Engine_ActorWaitForMove(IWAN);
        Engine_ActorSetAnimation(GERALD, 1);
        Engine_ActorSetAnimation(IWAN, 1);
        Engine_ActorSetAnimation(MEARI, 1);
        Engine_EventWait(10);
        Actor_FaceDirection(MEARI, FACING_NORTH, 0);
        Actor_FaceDirection(GERALD, FACING_NORTH, 0);
        Actor_FaceDirection(IWAN, FACING_NORTH, 20);
        Engine_ActorJump(18, 2, 20);
        Actor_FaceDirection(18, FACING_SOUTHWEST + FACING_STEP, 10);
        Event_ShowMessageAndWait(0x2012, 0, 10);
        Actor_FaceDirection(19, FACING_EAST + FACING_STEP, 10);
        Engine_ActorSetAnimationAndWait(19, 3);
        Actor_FaceDirection(18, FACING_SOUTH + FACING_STEP, 40);
        Actor_FaceDirection(18, FACING_SOUTHWEST + FACING_STEP, 10);
        Engine_ActorSetAnimationAndWait(18, 4);
        Event_ShowMessageAndWait(0x2012, 0, 10);
        Engine_ActorSetAttachedEffect(19, 0x102);
        Engine_EventWait(40);
        Actor_FaceDirection(18, FACING_SOUTH + FACING_STEP, 20);
        Actor_ShowEmote(18, 0x105, 40);
        Engine_EventOpenMessage(0x2012, 0);
        Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
        Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            // 最初の申し出と心変わりは断りと承諾を一つずつ共有し、互いのブロックへ飛ぶ (構造化すると複製が要りバイトが変わる)
            goto accepted;
        }
    declined:
        Engine_EventSetMessage((s32)MsgBiribinoTooYoungForTheJob);
        Event_ShowMessageAndWait(0x2012, 0, 10);
        Actor_FaceDirection(19, FACING_SOUTHEAST + FACING_STEP, 0);
        Engine_ActorSetAnimationAndWait(18, 4);
        Engine_EventShowMessageAndWait(0x2012, 0, 10);
        Actor_FaceDirection(GERALD, FACING_NORTH, 0);
        Actor_FaceDirection(IWAN, FACING_NORTH, 0);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimation(GERALD, 2);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Engine_ActorSetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
        }
        Engine_ActorSetAnimation(IWAN, 2);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Engine_ActorSetDestination(IWAN, record->x.part.pixel, record->z.part.pixel);
        }
        if (Engine_GameFlagIsSet(3) != 0) {
            Engine_ActorSetAnimation(MEARI, 2);
            record = Object_GetById(ACTOR_PARTY_LEADER);
            if (record != 0) {
                Engine_ActorSetDestination(MEARI, record->x.part.pixel, record->z.part.pixel);
            }
        }
        Engine_ActorWaitForMove(IWAN);
        Engine_ActorSetPosition(GERALD, 0, 0);
        Engine_ActorSetPosition(IWAN, 0, 0);
        Actor_SetPosition(MEARI, 0, 0);
        Engine_GameFlagSet(0x85f);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
        gEventWork->transition_frames = 16;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        goto leave;
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 894, 684);
    Engine_EventWait(80);
    Engine_CameraSetSpeed(0x9999, 0x1333);
    Engine_CameraMoveTo(PIXELS(894), -1, PIXELS(664), 1);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Engine_ActorSetPosition(GERALD, record->x.fixed, record->z.fixed);
    }
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Engine_ActorSetPosition(IWAN, record->x.fixed, record->z.fixed);
    }
    if (Engine_GameFlagIsSet(3) != 0) {
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Engine_ActorSetPosition(MEARI, record->x.fixed, record->z.fixed);
        }
    }
    Actor_SetSpeed(GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(IWAN, 0x9999, 0x4ccc);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    Engine_ActorSetAnimation(GERALD, 2);
    Engine_ActorSetAnimation(IWAN, 2);
    Engine_ActorSetAnimation(MEARI, 2);
    Actor_SetDestinationOffset(GERALD, -16, 16);
    Engine_ActorSetDestinationOffset(IWAN, 16, 16);
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_ActorSetDestinationOffset(MEARI, 32, 16);
    }
    Engine_ActorWaitForMove(IWAN);
    Engine_ActorSetAnimation(GERALD, 1);
    Engine_ActorSetAnimation(IWAN, 1);
    Engine_ActorSetAnimation(MEARI, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(MEARI, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 20);
    Engine_ActorShowEmote(18, 0x101, 60);
    Engine_EventSetMessage((s32)MsgBiribinoYehveChangeHeart);
    Engine_EventOpenMessage(0x2012, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        // 最初の申し出の断りへ
        goto declined;
    }
accepted:
    Actor_FaceDirection(MEARI, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 20);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_ActorShowEmote(18, 0x105, 60);
    Engine_EventSetMessage((s32)MsgBiribinoHmmmWellGrant);
    Event_ShowMessage(0x2012, 0);
    record = Object_GetById(20);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(20);
    record->scale_x = 0x8000;
    record->scale_y = 0x8000;
    record = Object_GetById(18);
    if (record != 0) {
        Engine_ActorSetPosition(20, record->x.fixed, record->z.fixed);
    }
    Engine_TaskWait(1);
    Engine_ActorJump(20, 6, 0);
    Actor_SetSpeed(20, 0x20000, 0x10000);
    Actor_MoveToAndWait(20, 894, 668);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_ShowEmote(MEARI, 0x101, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(IWAN, 0x101, 60);
    Engine_ActorSetAnimationAndWait(18, 4);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_ShowEmote(GERALD, 0x103, 60);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 10);
    Engine_EventOpenMessage(0x4001, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        do {
            Engine_ActorStartRepeatedMotion(GERALD, 2);
            Engine_ActorRunRepeatedMotion(IWAN, 2);
            Engine_EventSetMessage((s32)MsgBiribinoSeriousDoesntBother);
            Engine_EventOpenMessage(0x4001, 0);
        } while (Engine_EventChooseYesNo(0, 0) != 1);
    }
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventSetMessage((s32)MsgBiribinoWellDontLet);
    Event_ShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(MEARI, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Actor_ShowEmote(18, 0x105, 60);
    Engine_EventShowMessageAndWait(0x2012, 0, 20);
    Actor_ShowEmote(IWAN, 0x102, 60);
    Actor_FaceDirection(18, FACING_SOUTHEAST + FACING_STEP, 10);
    Actor_ShowEmote(18, 0x101, 60);
    Actor_ShowEmote(GERALD, 0x101, 40);
    Engine_ActorFaceDirection(GERALD, FACING_EAST, 20);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Event_ShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(MEARI, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
    Actor_ShowEmote(IWAN, 0x102, 60);
    Actor_FaceDirection(IWAN, FACING_WEST, 10);
    Engine_EventShowMessageAndWait(0x4002, 0, 10);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Event_ShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 10);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_EventOpenMessage(0x4001, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    while (Engine_EventChooseYesNo(0, 0) != 0) {
        Engine_EventSetMessage((s32)MsgBiribinoComeSaidYoud);
        Engine_EventOpenMessage(0x4001, 0);
    }
    Engine_EventWait(10);
    Actor_FaceDirection(MEARI, FACING_WEST, 0);
    Engine_ActorFaceDirection(GERALD, FACING_EAST, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Actor_ShowEmote(IWAN, 0x105, 60);
    Actor_FaceDirection(IWAN, FACING_WEST, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Engine_EventSetMessage((s32)MsgBiribinoWellSGoing);
    Event_ShowMessageAndWait(0x4002, 0, 20);
    Engine_ActorRunRepeatedMotion(18, 1);
    Actor_FaceDirection(18, FACING_SOUTH + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(18, 4);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_FaceDirection(MEARI, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 20);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(40);
    Actor_ShowEmote(18, 0x105, 80);
    Actor_FaceDirection(19, FACING_EAST + FACING_STEP, 10);
    Actor_SetAttachedEffect(19, 0x102);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorRunRepeatedMotion(18, 1);
    Engine_EventWait(40);
    Actor_FaceDirection(18, FACING_SOUTHWEST + FACING_STEP, 20);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(18, 4);
    Engine_ActorSetAnimation(18, 4);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorJump(20, 6, 0);
    record = Object_GetById(18);
    if (record != 0) {
        Engine_ActorSetDestination(20, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(20);
    Engine_ActorSetPosition(20, 0, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Engine_ActorSetAttachedEffect(IWAN, 0x102);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(19, 2);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 20);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4002, 0, 10);
    Actor_FaceDirection(19, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(18, FACING_SOUTHEAST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(18, 4);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_ShowEmote(GERALD, 0x103, 60);
    Engine_EventShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(18, FACING_SOUTH + FACING_STEP, 10);
    Engine_ActorSetAnimation(18, 4);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorSetAnimation(IWAN, 4);
    Engine_EventShowMessageAndWait(0x4002, 0, 10);
    Actor_FaceDirection(18, FACING_SOUTHEAST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_ShowEmote(MEARI, 0x107, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x107, 0);
    Actor_ShowEmote(GERALD, 0x107, 0);
    Actor_ShowEmote(IWAN, 0x107, 60);
    Actor_FaceDirection(18, FACING_SOUTHWEST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorRunRepeatedMotion(19, 2);
    Actor_FaceDirection(19, FACING_EAST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventWait(20);
    FieldObject_SetTargetAndCallback(0, 0x10013, Kyuden_FacingActions);
    FieldObject_SetTargetAndCallback(1, 0x10013, Kyuden_FacingActions);
    FieldObject_SetTargetAndCallback(2, 0x10013, Kyuden_FacingActions);
    FieldObject_SetTargetAndCallback(3, 0x10013, Kyuden_FacingActions);
    Actor_SetSpeed(19, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(19, 852, 646);
    Actor_WalkToAndWait(19, 852, 666);
    Actor_WalkToAndWait(19, 864, 672);
    Actor_FaceDirection(19, FACING_EAST + FACING_STEP, 10);
    Event_ShowMessageAndWait(0x4013, 0, 20);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(GERALD);
    Engine_ActorStop(IWAN);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(GERALD, 0x105, 0);
    Actor_ShowEmote(IWAN, 0x105, 60);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    FieldObject_SetTargetAndCallback(19, 0x10000, Kyuden_FacingActions);
    Engine_ActorSetAnimation(GERALD, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Engine_ActorSetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorSetAnimation(IWAN, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Engine_ActorSetDestination(IWAN, record->x.part.pixel, record->z.part.pixel);
    }
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_ActorSetAnimation(MEARI, 2);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Engine_ActorSetDestination(MEARI, record->x.part.pixel, record->z.part.pixel);
        }
    }
    Engine_ActorWaitForMove(IWAN);
    Engine_ActorSetPosition(GERALD, 0, 0);
    Engine_ActorSetPosition(IWAN, 0, 0);
    Engine_ActorSetPosition(MEARI, 0, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
    gEventWork->transition_frames = 16;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(0x321);
leave:
    Engine_EventRequestExit(29);
    Engine_EventEnd();
}

void FieldScene_RunPalaceGreeting(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Actor_FaceDirection(19, 0x3000, 0);
    Actor_SetSpeed(0, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x294);
    Engine_EventWait(20);
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_GameFlagSet(0x200);
    Event_PlayCue(188);
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Engine_ActorSetPosition(19, 0x1000000, 0x2780000);
    Engine_TaskWait(1);
    Actor_SetSpeed(19, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(19, 0x100, 0x284);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(19, 2);
    Engine_EventSetMessage((s32)MsgBiribinoNameSorryRejected);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Actor_ShowEmote(0, 0x100, 40);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x108, 0x294);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_WalkToAndWait(19, 248, 0x294);
    Actor_FaceDirection(19, 0x1000, 40);
    Engine_ActorSetAnimationAndWait(19, 4);
    Engine_EventShowMessage(19, 0);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventAskYesNo(19, 0);
    Engine_ActorRunRepeatedMotion(19, 2);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Actor_ShowEmote(0, 0x101, 60);
    Actor_SetAttachedEffect(19, 0x102);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Event_ShowMessage(19, 0);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_WalkToAndWait(19, 248, 0x304);
    Engine_ActorSetPosition(19, 0, 0);
    Engine_GameFlagClear(0x12f);
    Engine_GameFlagSet(0x84f);
    Engine_EventEnd();
}

void ConfigurePrimarySceneChannels(void)
{
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0xa000, 0);
    Actor_FaceDirection(3, 0x8000, 0);
}

void ConfigureSecondarySceneChannels(void)
{
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xa000, 0);
}

void RunEventScript02(void)
{
    struct FieldActor *buf;
    struct FieldActor *actor;
    s32 flag;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    buf = Engine_EventGetViewCenter();
    flag = 0;
    buf->motion_flags = flag;
    Camera_MoveTo(0x037e0000, -1, 0x02980000, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    gEventWork->transition_frames = 16;

    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Actor_SetPosition(19, 0x03780000, 0x031e0000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x03880000, 0x031e0000);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x037e0000, -1, 0x02ba0000, 1);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkTo(19, 888, 720);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 904, 736);
    Engine_EventWait(60);
    Engine_ActorWaitForMove(19);
    Engine_ActorSetAnimation(19, 1);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(19, 2);
    Engine_EventSetMessage((s32)MsgBiribinoWeveBroughtWarriorsMilord);

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        gEventWork->message++;
        flag = 0;
    }
    Event_ShowMessage(19, 0);
    if (flag != 0) {
        gEventWork->message++;
    }

    Camera_MoveTo(0x037e0000, -1, 0x02980000, 1);
    Engine_ActorEnableActionCallback(19, Kyuden_PacingActions);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);

    actor = Object_GetById(0);
    if (actor != 0) {
        Actor_SetPosition(GERALD, actor->x.fixed, actor->z.fixed);
    }
    actor = Object_GetById(0);
    if (actor != 0) {
        Actor_SetPosition(IWAN, actor->x.fixed, actor->z.fixed);
    }
    actor = Object_GetById(0);
    if (actor != 0) {
        Actor_SetPosition(MEARI, actor->x.fixed, actor->z.fixed);
    }

    Actor_SetSpeed(GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(IWAN, 0x9999, 0x4ccc);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    Engine_ActorSetAnimation(GERALD, 2);
    Engine_ActorSetAnimation(IWAN, 2);
    Engine_ActorSetAnimation(MEARI, 2);
    Actor_SetDestinationOffset(GERALD, -16, 16);
    Actor_SetDestinationOffset(IWAN, 16, 16);
    Actor_SetDestinationOffset(MEARI, 32, 16);
    Engine_ActorWaitForMove(IWAN);
    Engine_ActorSetAnimation(GERALD, 1);
    Engine_ActorSetAnimation(IWAN, 1);
    Engine_ActorSetAnimation(MEARI, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(GERALD, 0xc000, 0);
    Actor_FaceDirection(IWAN, 0xc000, 0);
    Engine_ActorWaitForMove(MEARI);
    Actor_FaceDirection(MEARI, 0xa000, 0);
    Object_RefreshSelectorById(19);
    Engine_EventWait(20);

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        gEventWork->message++;
        flag = 0;
    }
    Engine_ActorRunRepeatedMotion(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 20);
    if (flag != 0) {
        gEventWork->message++;
    }

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        gEventWork->message++;
        flag = 0;
    }
    Engine_ActorRunRepeatedMotion(18, 1);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    if (flag != 0) {
        gEventWork->message++;
    }

    ConfigurePrimarySceneChannels();
    Engine_EventWait(20);
    if (GameFlag_IsSet(0x84f) != 0) {
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Actor_ShowEmote(GERALD, 261, 40);
    } else {
        Engine_EventWait(40);
    }

    Actor_FaceDirection(GERALD, 0xc000, 10);
    Event_ShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(IWAN, 0xc000, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Event_ShowMessage(0x4002, 0);
    Actor_FaceDirection(MEARI, 0xa000, 10);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Event_ShowMessageAndWait(0x4003, 0, 20);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x2012, 0, 20);
    Actor_ShowEmote(GERALD, 259, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);

    if (GameFlag_IsSet(0x84f) != 0) {
        Engine_ActorRunRepeatedMotion(18, 1);
        Engine_ActorSetAnimationAndWait(18, 4);
        Event_OpenMessage(0x2012, 0);
        ConfigurePrimarySceneChannels();
        flag = 1;
        if (Engine_EventChooseYesNo(0, 0) != 0) {
            gEventWork->message++;
            flag = 0;
        }
        Actor_FaceDirection(18, 0x5000, 0);
        ConfigureSecondarySceneChannels();
        Engine_EventWait(10);
        Event_ShowMessageAndWait(0x2012, 0, 10);
        if (flag != 0) {
            gEventWork->message++;
        }
        Actor_ShowEmote(18, 258, 60);
    } else {
        gEventWork->message += 4;
    }

    Event_OpenMessage(0x2012, 0);
    ConfigurePrimarySceneChannels();
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventSetMessage((s32)MsgBiribinoHumblyThank);
    } else {
        Engine_EventSetMessage((s32)MsgBiribinoNaeYehDinnaeNeedTae);
    }

    ConfigureSecondarySceneChannels();
    Event_ShowMessageAndWait(0x2012, 0, 20);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventSetMessage((s32)MsgBiribinoWasButWorriedYehMight);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(GERALD, 0xe000, 0);
    Actor_FaceDirection(IWAN, 0, 0);
    Actor_FaceDirection(MEARI, 0x8000, 40);
    Engine_ActorRunRepeatedMotion(18, 2);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    ConfigureSecondarySceneChannels();
    Engine_EventWait(10);
    Actor_ShowEmote(18, 261, 60);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_ShowEmote(18, 264, 60);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_FaceDirection(18, 0x3000, 10);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Event_OpenMessage(0x2012, 0);

    ConfigurePrimarySceneChannels();
    flag = 1;
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        gEventWork->message++;
        flag = 0;
    }
    ConfigureSecondarySceneChannels();
    Event_ShowMessageAndWait(0x2012, 0, 10);
    if (flag != 0) {
        gEventWork->message++;
    }

    Actor_FaceDirection(18, 0x7000, 10);
    Engine_ActorRunRepeatedMotion(19, 1);
    Actor_FaceDirection(19, 0x1000, 20);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(19, 3);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(19, 0x3000, 10);
    Actor_FaceDirection(18, 0x3000, 20);
    Engine_ActorRunRepeatedMotion(18, 1);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);

    Engine_ActorSetAnimation(GERALD, 2);
    actor = Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(GERALD, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorSetAnimation(IWAN, 2);
    actor = Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(IWAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorSetAnimation(MEARI, 2);
    actor = Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(MEARI, actor->x.part.pixel, actor->z.part.pixel);
    }

    Engine_ActorWaitForMove(GERALD);
    Actor_SetPosition(GERALD, 0, 0);
    Actor_SetPosition(IWAN, 0, 0);
    Engine_ActorWaitForMove(MEARI);
    Actor_SetPosition(MEARI, 0, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(18, 0x5000, 0);
    FieldObject_SetTargetAndCallback(0, 0x10013, Kyuden_FacingActions);
    Actor_WalkToAndWait(19, 852, 646);
    Actor_WalkToAndWait(19, 852, 666);
    Actor_WalkToAndWait(19, 864, 672);
    Actor_FaceDirection(19, 0x1000, 10);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_WalkToAndWait(19, 886, 708);
    Actor_WalkTo(19, 894, 764);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 764);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    GameFlag_Set(802);
    if (GameFlag_IsSet(0x84f) == 0) {
        GameFlag_Set(0x84f);
        GameFlag_Set(0x84a);
    }
    Engine_EventRequestExit(6);
    Engine_EventEnd();
}

void BiribinoKyuden_RunActorRowScene(void)
{
    u16 zero;
    struct FieldActor *actor;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Engine_CameraMoveTo(0x2740000, -1, 0x2ec0000, 0);
    Engine_MapCopyCellAttributes(38, 55, 4, 1, 38, 45);
    Engine_MapCopyCellAttributes(42, 55, 4, 1, 38, 46);
    Object_GetById(0)->facing = 0;
    Actor_SetPosition(0, 0x2410000, 0x2f80000);
    Object_GetById(19)->facing = 0;
    Engine_ActorSetPosition(19, 0x2500000, 0x2f80000);
    Object_GetById(17)->facing = 0x9000;
    Actor_SetPosition(17, 0x2960000, 0x2fc0000);
    Actor_SetPosition(21, 0x2680000, 0x2d80000);
    Actor_SetPosition(22, 0x2780000, 0x2d80000);
    Actor_SetPosition(23, 0x2880000, 0x2d80000);
    Engine_ActorSetPosition(24, 0x2980000, 0x2d80000);
    Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(22), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(23), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(24), 0);
    actor = Object_GetById(21);
    zero = 0;
    actor->motion_flags = zero;
    Object_GetById(22)->motion_flags = zero;
    Object_GetById(23)->motion_flags = zero;
    Object_GetById(24)->motion_flags = zero;
    Object_GetById(21)->y.fixed = -0x40000;
    Object_GetById(22)->y.fixed = -0x40000;
    Object_GetById(23)->y.fixed = -0x40000;
    Object_GetById(24)->y.fixed = -0x40000;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    gEventWork->transition_frames = 16;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Actor_SetSpeed(19, 0x9999, 0x4ccc);
    Actor_SetSpeed(0, 0x9999, 0x4ccc);
    Actor_WalkTo(19, 0x274, 0x2fc);
    Actor_WalkToAndWait(0, 0x264, 0x2fc);
    Engine_ActorSetAnimation(19, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventSetMessage((s32)MsgBiribinoLordMccoyOrdered);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Actor_WalkToAndWait(19, 0x26e, 0x30c);
    Actor_FaceDirection(19, 0xc000, 10);
    Engine_ActorRunRepeatedMotion(17, 2);
    Engine_EventShowMessageAndWait(17, 0, 10);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_GameFlagClear(0x12f);
    Engine_GameFlagSet(0x202);
    Engine_EventEnd();
}

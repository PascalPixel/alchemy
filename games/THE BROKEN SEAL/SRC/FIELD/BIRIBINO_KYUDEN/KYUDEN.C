#include "KYUDEN.H"
#include "TYPES.H"
#include "CALL.H"
#include "FIELD_EVENT.H"

extern u8 gBiribinoKyudenPlacements[];
extern const struct ScenePlacement gBiribinoKyudenPlacementsOther[];
extern const struct SceneEvent gBiribinoKyudenEvents[];
extern const struct SceneEvent gBiribinoKyudenEventsOther[];
void FieldScene_PrepareActors(u8 *placements);

struct SceneRecord {
    u8 unk_000[166];
    u8 field_166;
    u8 unk_167[23];
    u8 field_190;
    u8 unk_191[23];
    u8 field_214;
    u8 unk_215[23];
    u8 field_238;
};

extern u8 MsgBiribinoYouWillingGoKolimaForest[];

extern u8 MsgBiribinoAaaah[];
extern u8 MsgBiribinoDoThinkTheseBilibinsGreat[];
extern u8 MsgBiribinoFineWarrior[];
extern u8 MsgBiribinoGoodTreasureIfGetTurned[];
extern u8 MsgBiribinoGotPrettyNiceRewardBut[];
extern u8 MsgBiribinoItsVeryRecklessForSuch[];
extern u8 MsgBiribinoLookedVeryCourageousWalkingToward[];
extern u8 MsgBiribinoMayChooseOnlyOneItem[];
extern u8 MsgBiribinoNonethelessIfYourLuckSour[];
extern u8 MsgBiribinoRewardReceivedWasIndeedGreatest[];
extern u8 MsgBiribinoSometimesWeNeedChildrenRemind[];
extern u8 MsgBiribinoWasWatchingFromHereAfter[];
extern u8 MsgBiribinoWeReallyGivingOurTreasure[];

extern struct EventWork *gEventWork;
s32 Engine_GameFlagIsSet();
void Engine_AudioPlayCue();
void Map_ClearLayerEntryFlag();
void Engine_EventBegin();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetAnimation();
void Engine_ActorSetDestinationOffset();
void Engine_ActorCenterAndWalk();
void Engine_EventRequestExit();
void Engine_EventEnd();
void Map_SetLayerEntryFlag();

extern u8 MsgBiribinoAlwaysWelcomeInPalaceLord[];
extern u8 MsgBiribinoPleaseTakeYourRewardBefore[];
extern u8 MsgBiribinoRobinCheckedChestButWas[];
extern u8 MsgBiribinoTreasureChestLocked[];

extern u8 MsgBiribinoComeSaidYoud[];
extern u8 MsgBiribinoHmmmWellGrant[];
extern u8 MsgBiribinoMatter[];
extern u8 MsgBiribinoSeriousDoesntBother[];
extern u8 MsgBiribinoTooYoungForTheJob[];
extern u8 MsgBiribinoWellDontLet[];
extern u8 MsgBiribinoWellSGoing[];
extern u8 MsgBiribinoYehveChangeHeart[];

extern u8 MsgBiribinoHumblyThank[];
extern u8 MsgBiribinoNaeYehDinnaeNeedTae[];
extern u8 MsgBiribinoNameSorryRejected[];
extern u8 MsgBiribinoWasButWorriedYehMight[];
extern u8 MsgBiribinoWeveBroughtWarriorsMilord[];

extern u8 MsgBiribinoLordMccoyOrdered[];

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
            /* The loader relocates the stored pool word to -0x1000. */
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

/* The actors placed in the palace. Taking the reward changes four entries
   of the palace's table, which is prepared first. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    u8 *p;

    /* A signed halfword read. */
    if (gGameState.scene == (s32)&SceneId_BiribinoKyuden) {
        p = gBiribinoKyudenPlacements;
        FieldScene_PrepareActors(p);

        if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
            struct SceneRecord *rec = (struct SceneRecord *)p;

            rec->field_166 = 2;
            rec->field_190 = 0;
            rec->field_214 = 3;
            rec->field_238 = 1;
        }

        return (const struct ScenePlacement *)p;
    }
    return gBiribinoKyudenPlacementsOther;
}

/* What the palace answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoKyuden) {
        return gBiribinoKyudenEvents;
    }
    return gBiribinoKyudenEventsOther;
}

void Kyuden_AskAboutKolima(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoYouWillingGoKolimaForest);
    Event_AskYesNo(10, 0);
    Event_End();
}

void FieldScene_RunStartledGuard(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_ShowEmote(14, 0x102, 0);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(40);
    Event_SetMessage((s32)MsgBiribinoAaaah);
    Event_ShowMessageAndWait(14, 0, 20);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_FaceDirection(14, 0xb000, 10);
    Event_End();
}

void FieldScene_RunScene38dSequenceA(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoFineWarrior);
    if (GameFlag_IsSet(0x302) != 0) {
        Event_SetMessage((s32)MsgBiribinoItsVeryRecklessForSuch);
    }
    Event_ShowMessage(15, 0);
    GameFlag_Set(0x302);
    Event_End();
}

void SceneDialogue_RunActor16Message1769(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoWasWatchingFromHereAfter);
    Event_AskYesNo(16, 0);
    Event_End();
}

void FieldScene_RunActorSeventeenFlaggedDialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage((s32)MsgBiribinoMayChooseOnlyOneItem);
    } else if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Event_SetMessage((s32)MsgBiribinoRewardReceivedWasIndeedGreatest);
    } else {
        Event_SetMessage((s32)MsgBiribinoDoThinkTheseBilibinsGreat);
        if (GameFlag_IsSet(0x84d) != 0) {
            gEventWork->message++;
        }
    }

    Event_ShowMessage(17, 0);
    Event_End();
}

void SceneDialogue_RunActor15Flag303Scene(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoSometimesWeNeedChildrenRemind);
    if (GameFlag_IsSet(0x303) != 0) {
        Event_SetMessage((s32)MsgBiribinoLookedVeryCourageousWalkingToward);
    }
    Event_ShowMessage(15, 0);
    GameFlag_Set(0x303);
    Event_End();
}

void FieldScene_RunActorSeventeenFlagDialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage((s32)MsgBiribinoNonethelessIfYourLuckSour);
    } else if (GameFlag_IsSet(0x845) == 0) {
        Event_SetMessage((s32)MsgBiribinoGoodTreasureIfGetTurned);
    } else {
        Event_SetMessage((s32)MsgBiribinoWeReallyGivingOurTreasure);
        if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
            Event_SetMessage((s32)MsgBiribinoGotPrettyNiceRewardBut);
        }
    }

    Event_ShowMessage(17, 0);
    Event_End();
}

void BiribinoKyuden_RunDoorExitScene(void)
{
    u8 *work;
    s32 lock;

    work = *(u8 **)&gEventWork;
    lock = 0;
    if (*(s16 *)(work + 0x16c) == 9) {
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
    Call3(Engine_ActorSetSpeed, 0, 0x8000, 0x4000);
    Engine_ActorSetAnimation(0, 2);
    if (*(s16 *)(work + 0x16c) == 9)
        Call3(Engine_ActorSetDestinationOffset, 0, 0, -16);
    else
        Call3(Engine_ActorCenterAndWalk, 0, 3, -16);
    Engine_EventWait(16);
    Engine_EventRequestExit(*(s16 *)(work + 0x16c));
    Engine_EventEnd();
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
}

void FieldScene_ShowChestEmpty(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgBiribinoRobinCheckedChestButWas, 1);
    Event_End();
}

void FieldScene_ShowChestLocked(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgBiribinoTreasureChestLocked, 1);
    Event_End();
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
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x210);
    } else {
        FieldScene_RunSlotSubjectBranch(21, 182, 0x210);
    }
}

void FieldScene_RunStep211ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x211);
    } else {
        FieldScene_RunSlotSubjectBranch(22, 183, 0x211);
    }
}

void FieldScene_RunStep212ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x212);
    } else {
        FieldScene_RunSlotSubjectBranch(23, 186, 0x212);
    }
}

void FieldScene_RunStep213ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x213);
    } else {
        FieldScene_RunSlotSubjectBranch(24, 189, 0x213);
    }
}

void FieldScene_RunSlotSubjectBranch(s32 actor, s32 item, s32 flag)
{
    s32 record;

    Event_Begin();

    record = BattleFx_PlayCueAndStartEmitterOnTarget(0, actor, item);

    if (Party_GiveItem(item, 0) != -1) {
        Actor_SetAnimation(actor, 2);
        GameFlag_Set(FLAG_REWARD_TAKEN);
        GameFlag_Set(flag);
        GameFlag_Clear(0x322);
        GameFlag_Clear(0x202);
    } else {
        Audio_PlayCue(125);
        Actor_SetAnimation(actor, 5);
    }

    Engine_ObjectDispatchRelease(record);
    Event_End();
}

void FieldScene_RunRewardReminder(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) == 0) {
        if (Engine_GameFlagIsSet(0x322) != 0) {
            Event_Begin();
            Actor_ShowEmote(19, 0x100, 0);
            Actor_FaceDirection(19, 0x7000, 10);
            Actor_RunRepeatedMotion(19, 2);
            Event_Wait(20);
            Event_SetMessage((s32)MsgBiribinoPleaseTakeYourRewardBefore);
            Event_ShowMessage(19, 0);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x268, 0x2fa);
            Actor_FaceDirection(19, 0xd000, 10);
            Event_End();
        }
    }
}

void FieldScene_RunPalaceFarewell(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Event_Begin();
        Actor_FaceActor(ACTOR_PARTY_LEADER, 19, 0);
        Actor_SetSpeed(19, 0x9999, 0x4ccc);
        Actor_WalkToAndWait(19, 0x26e, 0x2fc);
        Actor_FaceDirection(19, 0xf000, 20);
        Actor_SetAnimationAndWait(19, 3);
        Actor_SetAnimationAndWait(17, 3);
        Event_Wait(20);
        Actor_FaceActor(19, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Actor_SetAnimationAndWait(19, 3);
        Event_SetMessage((s32)MsgBiribinoAlwaysWelcomeInPalaceLord);
        Event_ShowMessageAndWait(19, 0, 10);
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Actor_WalkToAndWait(19, 0x23a, 0x2f6);
        Actor_SetPosition(19, 0, 0);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        GameFlag_Set(0x85e);
        GameFlag_Set(0x333);
        Event_End();
    }
}

/* McCoy's Palace entry: record the arrival, clear two layer flags and, by
 * the story flags and the entrance, run the matching scene or restore the
 * room (the four guards at row 0x2d8, the opened doorway cells). */
s32 BiribinoKyuden_ApplyEntryState(s32 a0, s32 a1)
{
    u32 i;
    s32 rec8;
    s32 record;

    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x209;
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    ((void (*)())Engine_GameFlagSet)(0x84b);
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
                            Call3(Engine_ActorSetPosition, 21, 0x2680000, 0x2d80000);
                            Call3(Engine_ActorSetPosition, 22, 0x2780000, 0x2d80000);
                            Call3(Engine_ActorSetPosition, 23, 0x2880000, 0x2d80000);
                            Engine_ActorSetPosition(24, 0x2980000, 0x2d80000);
                            record = (s32)Object_GetById(21);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            record = (s32)Object_GetById(22);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            record = (s32)Object_GetById(23);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            record = (s32)Object_GetById(24);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            *((u8 *)Object_GetById(21) + 85) = rec8;
                            *((u8 *)Object_GetById(22) + 85) = rec8;
                            *((u8 *)Object_GetById(23) + 85) = rec8;
                            *((u8 *)Object_GetById(24) + 85) = rec8;
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
    s32 record;

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    ((u8 *)Engine_EventGetViewCenter())[85] = 0;
    Call3(Engine_CameraMoveTo, 0x37e0000, -1, 0x2980000);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(0, 0, 0);
    if (Value1(Engine_GameFlagIsSet, 0x85f) != 0) {
        Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2ba0000, 0);
        Call3(Engine_ActorSetPosition, 19, 0x36c0000, 0x27a0000);
        Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
        Call3(Engine_ActorSetPosition, 0, 0x37e0000, 0x31e0000);
    }
    Engine_MapRedraw();
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 40;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    if (Engine_GameFlagIsSet(0x85f) == 0) {
        Engine_EventWait(80);
        Call3(Engine_ActorSetPosition, 19, 0x37e0000, 0x31e0000);
        Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
        Camera_MoveTo(0x37e0000, -1, 0x2ba0000, 1);
        Call3(Engine_ActorSetSpeed, 19, 0xcccc, 0x6666);
        Actor_WalkTo(19, 0x37e, 0x2b8);
        Engine_EventWait(80);
        Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2980000, 1);
        Actor_WaitForMove(19);
        Call3(Engine_ActorWalkToAndWait, 19, 0x34a, 0x2b8);
        Actor_WalkToAndWait(19, 0x34a, 0x27c);
        Actor_FaceDirection(18, 0x7000, 20);
        Call3(Engine_ActorWalkToAndWait, 19, 0x36c, 0x27a);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(20);
        Actor_SetAnimationAndWait(18, 3);
        Event_Wait(10);
        Engine_EventSetMessage((s32)MsgBiribinoMatter);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Actor_RunRepeatedMotion(19, 2);
        Engine_EventShowMessageAndWait(19, 0, 20);
        Engine_ActorRunRepeatedMotion(18, 1);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(40);
        Call3(Engine_ActorShowEmote, 18, 0x105, 60);
        Call2(Engine_EventShowMessage, 0x2012, 0);
        Engine_ActorRunRepeatedMotion(18, 1);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Engine_ActorSetAttachedEffect(19, 0x102);
        Engine_EventWait(60);
        Call3(Engine_ActorFaceDirection, 19, 0x3000, 10);
        Call3(Engine_ActorFaceDirection, 18, 0x5000, 10);
        Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2ba0000, 1);
        Call3(Engine_ActorSetPosition, 0, 0x37e0000, 0x31e0000);
        Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
        Call3(Engine_ActorWalkToAndWait, 0, 0x37e, 0x2d6);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(18, 1);
        Engine_EventShowMessage(0x2012, 0);
        Camera_MoveTo(0x37e0000, -1, 0x2980000, 1);
        Engine_ActorWalkToAndWait(0, 0x37e, 0x2ac);
        record = ((s32 (*)())Object_GetById)(0);
        if (record != 0) {
            Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        record = ((s32 (*)())Object_GetById)(0);
        if (record != 0) {
            Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        if (Engine_GameFlagIsSet(3) != 0) {
            record = ((s32 (*)())Object_GetById)(0);
            if (record != 0) {
                Engine_ActorSetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
        }
        Call3(Engine_ActorSetSpeed, 1, 0x9999, 0x4ccc);
        Call3(Engine_ActorSetSpeed, 2, 0x9999, 0x4ccc);
        Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
        Engine_ActorSetAnimation(1, 2);
        Engine_ActorSetAnimation(2, 2);
        Engine_ActorSetAnimation(3, 2);
        Actor_SetDestinationOffset(ACTOR_GERALD, -16, 16);
        Engine_ActorSetDestinationOffset(2, 16, 16);
        if (Engine_GameFlagIsSet(3) != 0) {
            Engine_ActorSetDestinationOffset(3, 32, 16);
        }
        Engine_ActorWaitForMove(2);
        Engine_ActorSetAnimation(1, 1);
        Engine_ActorSetAnimation(2, 1);
        Actor_SetAnimation(ACTOR_MIA, 1);
        Engine_EventWait(10);
        Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
        Engine_ActorJump(18, 2, 20);
        Call3(Engine_ActorFaceDirection, 18, 0x7000, 10);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
        Actor_SetAnimationAndWait(19, 3);
        Call3(Engine_ActorFaceDirection, 18, 0x5000, 40);
        Call3(Engine_ActorFaceDirection, 18, 0x7000, 10);
        Engine_ActorSetAnimationAndWait(18, 4);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Engine_ActorSetAttachedEffect(19, 0x102);
        Event_Wait(40);
        Call3(Engine_ActorFaceDirection, 18, 0x5000, 20);
        Call3(Engine_ActorShowEmote, 18, 0x105, 40);
        Engine_EventOpenMessage(0x2012, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            /* FAKEMATCH: the first offer and the change of heart share one
             * refusal and one acceptance by jumping into each other's
             * blocks, which keeps the reference's single copy of each in
             * this order. */
            goto accepted;
        }
    declined:
        Engine_EventSetMessage((s32)MsgBiribinoTooYoungForTheJob);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
        Engine_ActorSetAnimationAndWait(18, 4);
        Engine_EventShowMessageAndWait(0x2012, 0, 10);
        Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(ACTOR_GERALD, 2);
        record = ((s32 (*)())Object_GetById)(0);
        if (record != 0) {
            Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorSetAnimation(2, 2);
        record = ((s32 (*)())Object_GetById)(0);
        if (record != 0) {
            Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        if (Engine_GameFlagIsSet(3) != 0) {
            Engine_ActorSetAnimation(3, 2);
            record = ((s32 (*)())Object_GetById)(0);
            if (record != 0) {
                Engine_ActorSetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
        }
        Engine_ActorWaitForMove(2);
        Engine_ActorSetPosition(1, 0, 0);
        Engine_ActorSetPosition(2, 0, 0);
        Actor_SetPosition(ACTOR_MIA, 0, 0);
        ((void (*)())Engine_GameFlagSet)(0x85f);
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x37e, 0x2f0);
        gEventWork->transition_frames = 16;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        goto leave;
    }
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Engine_ActorWalkTo(0, 0x37e, 0x2ac);
    Engine_EventWait(80);
    Engine_CameraSetSpeed(0x9999, 0x1333);
    Engine_CameraMoveTo(0x37e0000, -1, 0x2980000, 1);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetAnimation(0, 1);
    record = ((s32 (*)())Object_GetById)(0);
    if (record != 0) {
        Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = ((s32 (*)())Object_GetById)(0);
    if (record != 0) {
        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    if (Engine_GameFlagIsSet(3) != 0) {
        record = ((s32 (*)())Object_GetById)(0);
        if (record != 0) {
            Engine_ActorSetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
    }
    Call3(Engine_ActorSetSpeed, 1, 0x9999, 0x4ccc);
    Call3(Engine_ActorSetSpeed, 2, 0x9999, 0x4ccc);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Engine_ActorSetAnimation(1, 2);
    Engine_ActorSetAnimation(2, 2);
    Engine_ActorSetAnimation(3, 2);
    Call3(Engine_ActorSetDestinationOffset, 1, -16, 16);
    Engine_ActorSetDestinationOffset(2, 16, 16);
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_ActorSetDestinationOffset(3, 32, 16);
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetAnimation(1, 1);
    Engine_ActorSetAnimation(2, 1);
    Engine_ActorSetAnimation(3, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
    Engine_ActorShowEmote(18, 0x101, 60);
    Event_SetMessage((s32)MsgBiribinoYehveChangeHeart);
    Engine_EventOpenMessage(0x2012, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        /* FAKEMATCH: into the first offer's refusal, as above. */
        goto declined;
    }
accepted:
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
    Engine_ActorSetAnimation(3, 3);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_ActorShowEmote(18, 0x105, 60);
    Engine_EventSetMessage((s32)MsgBiribinoHmmmWellGrant);
    Call2(Engine_EventShowMessage, 0x2012, 0);
    record = (s32)Object_GetById(20);
    Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
    record = (s32)Object_GetById(20);
    *(s32 *)(record + 24) = 0x8000;
    *(s32 *)(record + 28) = 0x8000;
    record = ((s32 (*)())Object_GetById)(18);
    if (record != 0) {
        Engine_ActorSetPosition(20, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Engine_TaskWait(1);
    Engine_ActorJump(20, 6, 0);
    Call3(Engine_ActorSetSpeed, 20, 0x20000, 0x10000);
    Actor_MoveToAndWait(20, 0x37e, 0x29c);
    Event_Wait(40);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 3, 0x101, 0);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Call3(Engine_ActorShowEmote, 2, 0x101, 60);
    Engine_ActorSetAnimationAndWait(18, 4);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 1, 0x103, 60);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 10);
    Engine_EventOpenMessage(0x4001, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        do {
            Engine_ActorStartRepeatedMotion(1, 2);
            Engine_ActorRunRepeatedMotion(2, 2);
            Engine_EventSetMessage((s32)MsgBiribinoSeriousDoesntBother);
            Engine_EventOpenMessage(0x4001, 0);
        } while (Engine_EventChooseYesNo(0, 0) != 1);
    }
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_EventSetMessage((s32)MsgBiribinoWellDontLet);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 10);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Actor_ShowEmote(18, 0x105, 60);
    Engine_EventShowMessageAndWait(0x2012, 0, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 60);
    Call3(Engine_ActorFaceDirection, 18, 0x3000, 10);
    Call3(Engine_ActorShowEmote, 18, 0x101, 60);
    Call3(Engine_ActorShowEmote, 1, 0x101, 40);
    Engine_ActorFaceDirection(1, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x2000, 20);
    Call3(Engine_ActorShowEmote, 2, 0x102, 60);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 10);
    Engine_EventShowMessageAndWait(0x4002, 0, 10);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(1, 3);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 10);
    Engine_ActorStartRepeatedMotion(1, 1);
    Engine_EventOpenMessage(0x4001, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    while (Event_ChooseYesNo(0, 0) != 0) {
        Engine_EventSetMessage((s32)MsgBiribinoComeSaidYoud);
        ((void (*)())Engine_EventOpenMessage)(0x4001, 0);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
    Engine_ActorFaceDirection(1, 0, 10);
    Call3(Engine_ActorFaceDirection, 0, 0x2000, 10);
    Engine_ActorSetAnimationAndWait(1, 3);
    Call3(Engine_ActorShowEmote, 2, 0x105, 60);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 10);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventSetMessage((s32)MsgBiribinoWellSGoing);
    Call3(Engine_EventShowMessageAndWait, 0x4002, 0, 20);
    Engine_ActorRunRepeatedMotion(18, 1);
    Call3(Engine_ActorFaceDirection, 18, 0x5000, 10);
    Engine_ActorSetAnimationAndWait(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
    Engine_ActorSetAnimation(3, 3);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(40);
    Actor_ShowEmote(18, 0x105, 80);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
    Call2(Engine_ActorSetAttachedEffect, 19, 0x102);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorRunRepeatedMotion(18, 1);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 18, 0x7000, 20);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(18, 4);
    Actor_SetAnimation(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Engine_ActorJump(20, 6, 0);
    record = ((s32 (*)())Object_GetById)(18);
    if (record != 0) {
        Engine_ActorSetDestination(20, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(20);
    Engine_ActorSetPosition(20, 0, 0);
    Engine_EventWait(20);
    Call2(Engine_ActorSetAttachedEffect, 3, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Engine_ActorSetAttachedEffect(2, 0x102);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(19, 2);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 20);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Call3(Engine_EventShowMessageAndWait, 0x4002, 0, 10);
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 18, 0x3000, 10);
    Engine_ActorSetAnimationAndWait(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 1, 0x103, 60);
    Engine_EventShowMessageAndWait(0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 18, 0x5000, 10);
    Engine_ActorSetAnimation(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Engine_ActorSetAnimation(2, 4);
    Engine_EventShowMessageAndWait(0x4002, 0, 10);
    Call3(Engine_ActorFaceDirection, 18, 0x3000, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 3, 0x107, 0);
    Call3(Engine_ActorShowEmote, 0, 0x107, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x107, 0);
    Call3(Engine_ActorShowEmote, 2, 0x107, 60);
    Call3(Engine_ActorFaceDirection, 18, 0x7000, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorRunRepeatedMotion(19, 2);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventWait(20);
    Call3(Object_SetTargetAndCallback, 0, 0x10013, (s32)Kyuden_FacingActions);
    Call3(Object_SetTargetAndCallback, 1, 0x10013, (s32)Kyuden_FacingActions);
    Call3(Object_SetTargetAndCallback, 2, 0x10013, (s32)Kyuden_FacingActions);
    Call3(Object_SetTargetAndCallback, 3, 0x10013, (s32)Kyuden_FacingActions);
    Call3(Engine_ActorSetSpeed, 19, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(19, 0x354, 0x286);
    Call3(Engine_ActorWalkToAndWait, 19, 0x354, 0x29a);
    Call3(Engine_ActorWalkToAndWait, 19, 0x360, 0x2a0);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
    Event_ShowMessageAndWait(0x4013, 0, 20);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(1);
    Engine_ActorStop(2);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Call3(Engine_ActorShowEmote, 1, 0x105, 0);
    Call3(Engine_ActorShowEmote, 2, 0x105, 60);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Call3(Object_SetTargetAndCallback, 19, 0x10000, (s32)Kyuden_FacingActions);
    Engine_ActorSetAnimation(1, 2);
    record = ((s32 (*)())Object_GetById)(0);
    if (record != 0) {
        Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(2, 2);
    record = ((s32 (*)())Object_GetById)(0);
    if (record != 0) {
        Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    if (Engine_GameFlagIsSet(3) != 0) {
        Engine_ActorSetAnimation(3, 2);
        record = ((s32 (*)())Object_GetById)(0);
        if (record != 0) {
            Engine_ActorSetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(2, 0, 0);
    Engine_ActorSetPosition(3, 0, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Engine_ActorWalkToAndWait(0, 0x37e, 0x2f0);
    gEventWork->transition_frames = 16;
    Event_CloseScreen();
    Engine_EventWaitForScreen();
    ((void (*)())Engine_GameFlagSet)(0x321);
leave:
    Event_RequestExit(29);
    Engine_EventEnd();
}

void FieldScene_RunPalaceGreeting(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x294);
    Engine_EventWait(20);
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_GameFlagSet(0x200);
    Audio_PlayCue(188);
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Engine_ActorSetPosition(19, 0x1000000, 0x2780000);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 19, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkToAndWait, 19, 0x100, 0x284);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Event_Wait(20);
    Actor_RunRepeatedMotion(19, 2);
    Engine_EventSetMessage((s32)MsgBiribinoNameSorryRejected);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Call3(Engine_ActorShowEmote, 0, 0x100, 40);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x108, 0x294);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Call3(Engine_ActorWalkToAndWait, 19, 248, 0x294);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 40);
    Engine_ActorSetAnimationAndWait(19, 4);
    Engine_EventShowMessage(19, 0);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventAskYesNo(19, 0);
    Actor_RunRepeatedMotion(19, 2);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Call3(Engine_ActorShowEmote, 0, 0x101, 60);
    Actor_SetAttachedEffect(19, 0x102);
    Event_Wait(60);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Event_ShowMessage(19, 0);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 19, 248, 0x304);
    Engine_ActorSetPosition(19, 0, 0);
    Engine_GameFlagClear(0x12f);
    Engine_GameFlagSet(0x84f);
    Engine_EventEnd();
}

void ConfigurePrimarySceneChannels(void)
{
    FaceActor(1, 0xe000, 0);
    FaceActor(2, 0xa000, 0);
    FaceActor(3, 0x8000, 0);
}

void ConfigureSecondarySceneChannels(void)
{
    FaceActor(1, 0xc000, 0);
    FaceActor(2, 0xc000, 0);
    FaceActor(3, 0xa000, 0);
}

/* FAKEMATCH: The cleared motion byte keeps its own zero local so the
 * following call reloads its separate zero argument. */
void RunEventScript02(void)
{
    u8 *buf;
    struct FieldActor *actor;
    s32 flag;
    u8 clear = 0;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    buf = (u8 *)Engine_EventGetViewCenter();
    buf[85] = clear;
    Camera_MoveTo(0x037e0000, -1, 0x02980000, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Map_Redraw();
    Task_Wait(1);

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    gEventWork->transition_frames = 16;

    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_SetPosition(19, 0x03780000, 0x031e0000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x03880000, 0x031e0000);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x037e0000, -1, 0x02ba0000, 1);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkTo(19, 888, 720);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 904, 736);
    Event_Wait(60);
    Actor_WaitForMove(19);
    Actor_SetAnimation(19, 1);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(19, 2);
    Event_SetMessage((s32)MsgBiribinoWeveBroughtWarriorsMilord);

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        AdvanceMessage(1);
        flag = 0;
    }
    Event_ShowMessage(19, 0);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    Camera_MoveTo(0x037e0000, -1, 0x02980000, 1);
    Actor_EnableActionCallback(19, Kyuden_PacingActions);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);

    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_GERALD, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }

    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_IVAN, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    Actor_SetAnimation(ACTOR_MIA, 2);
    Actor_SetDestinationOffset(ACTOR_GERALD, -16, 16);
    Actor_SetDestinationOffset(ACTOR_IVAN, 16, 16);
    Actor_SetDestinationOffset(ACTOR_MIA, 32, 16);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_SetAnimation(ACTOR_MIA, 1);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    Object_RefreshSelectorById(19);
    Event_Wait(20);

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        AdvanceMessage(1);
        flag = 0;
    }
    Actor_RunRepeatedMotion(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 20);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        AdvanceMessage(1);
        flag = 0;
    }
    Actor_RunRepeatedMotion(18, 1);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    ConfigurePrimarySceneChannels();
    Event_Wait(20);
    if (GameFlag_IsSet(0x84f) != 0) {
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Actor_ShowEmote(ACTOR_GERALD, 261, 40);
    } else {
        Event_Wait(40);
    }

    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 10);
    Event_ShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_ShowMessage(0x4002, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_ShowMessageAndWait(0x4003, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x2012, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 259, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);

    if (GameFlag_IsSet(0x84f) != 0) {
        Actor_RunRepeatedMotion(18, 1);
        Actor_SetAnimationAndWait(18, 4);
        Event_OpenMessage(0x2012, 0);
        ConfigurePrimarySceneChannels();
        flag = 1;
        if (Event_ChooseYesNo(0, 0) != 0) {
            AdvanceMessage(1);
            flag = 0;
        }
        Actor_FaceDirection(18, 0x5000, 0);
        ConfigureSecondarySceneChannels();
        Event_Wait(10);
        Event_ShowMessageAndWait(0x2012, 0, 10);
        if (flag != 0) {
            AdvanceMessage(1);
        }
        Actor_ShowEmote(18, 258, 60);
    } else {
        AdvanceMessage(4);
    }

    Event_OpenMessage(0x2012, 0);
    ConfigurePrimarySceneChannels();
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage((s32)MsgBiribinoHumblyThank);
    } else {
        Event_SetMessage((s32)MsgBiribinoNaeYehDinnaeNeedTae);
    }

    ConfigureSecondarySceneChannels();
    Event_ShowMessageAndWait(0x2012, 0, 20);
    Actor_RunRepeatedMotion(19, 1);
    Event_SetMessage((s32)MsgBiribinoWasButWorriedYehMight);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 40);
    Actor_RunRepeatedMotion(18, 2);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    ConfigureSecondarySceneChannels();
    Event_Wait(10);
    Actor_ShowEmote(18, 261, 60);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_ShowEmote(18, 264, 60);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_FaceDirection(18, 0x3000, 10);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimationAndWait(18, 3);
    Event_OpenMessage(0x2012, 0);

    ConfigurePrimarySceneChannels();
    flag = 1;
    if (Event_ChooseYesNo(0, 0) == 1) {
        AdvanceMessage(1);
        flag = 0;
    }
    ConfigureSecondarySceneChannels();
    Event_ShowMessageAndWait(0x2012, 0, 10);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    Actor_FaceDirection(18, 0x7000, 10);
    Actor_RunRepeatedMotion(19, 1);
    Actor_FaceDirection(19, 0x1000, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(19, 3);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(19, 0x3000, 10);
    Actor_FaceDirection(18, 0x3000, 20);
    Actor_RunRepeatedMotion(18, 1);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimationAndWait(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);

    Actor_SetAnimation(ACTOR_GERALD, 2);
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_SetAnimation(ACTOR_IVAN, 2);
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_SetAnimation(ACTOR_MIA, 2);
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
    }

    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Event_Wait(20);
    Actor_FaceDirection(18, 0x5000, 0);
    Call3(Object_SetTargetAndCallback, 0, 0x10013, (s32)Kyuden_FacingActions);
    Actor_WalkToAndWait(19, 852, 646);
    Actor_WalkToAndWait(19, 852, 666);
    Actor_WalkToAndWait(19, 864, 672);
    Actor_FaceDirection(19, 0x1000, 10);
    Actor_RunRepeatedMotion(19, 1);
    Event_Wait(10);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_WalkToAndWait(19, 886, 708);
    Actor_WalkTo(19, 894, 764);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 764);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(802);
    if (GameFlag_IsSet(0x84f) == 0) {
        GameFlag_Set(0x84f);
        GameFlag_Set(0x84a);
    }
    Event_RequestExit(6);
    Event_End();
}

/* Pans the camera over the palace, lines up four actors in a row and runs their scripted beat. */
void BiribinoKyuden_RunActorRowScene(void)
{
    u16 zero;
    struct FieldActor *actor;

    Engine_EventBegin();
    Call4((void (*)())Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_EventGetViewCenter()->motion_flags = 0;
    ((void (*)())Engine_CameraMoveTo)(0x2740000, -1, 0x2ec0000, 0);
    Engine_MapCopyCellAttributes(38, 55, 4, 1, 38, 45);
    Engine_MapCopyCellAttributes(42, 55, 4, 1, 38, 46);
    Object_GetById(0)->facing = 0;
    Call3((void (*)())Engine_ActorSetPosition, 0, 0x2410000, 0x2f80000);
    Object_GetById(19)->facing = 0;
    ((void (*)())Engine_ActorSetPosition)(19, 0x2500000, 0x2f80000);
    Object_GetById(17)->facing = 0x9000;
    Call3((void (*)())Engine_ActorSetPosition, 17, 0x2960000, 0x2fc0000);
    Call3((void (*)())Engine_ActorSetPosition, 21, 0x2680000, 0x2d80000);
    Call3((void (*)())Engine_ActorSetPosition, 22, 0x2780000, 0x2d80000);
    Call3((void (*)())Engine_ActorSetPosition, 23, 0x2880000, 0x2d80000);
    ((void (*)())Engine_ActorSetPosition)(24, 0x2980000, 0x2d80000);
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
    gEventWork->start_transition = 0x201;
    gEventWork->transition_frames = 16;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Call3((void (*)())Engine_ActorSetSpeed, 19, 0x9999, 0x4ccc);
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Call3((void (*)())Engine_ActorWalkTo, 19, 0x274, 0x2fc);
    Call3((void (*)())Engine_ActorWalkToAndWait, 0, 0x264, 0x2fc);
    Engine_ActorSetAnimation(19, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventSetMessage((s32)MsgBiribinoLordMccoyOrdered);
    ((void (*)())Engine_EventShowMessageAndWait)(19, 0, 10);
    Call3((void (*)())Engine_ActorWalkToAndWait, 19, 0x26e, 0x30c);
    Call3((void (*)())Engine_ActorFaceDirection, 19, 0xc000, 10);
    Engine_ActorRunRepeatedMotion(17, 2);
    ((void (*)())Engine_EventShowMessageAndWait)(17, 0, 10);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_GameFlagClear(0x12f);
    Engine_GameFlagSet(0x202);
    Engine_EventEnd();
}

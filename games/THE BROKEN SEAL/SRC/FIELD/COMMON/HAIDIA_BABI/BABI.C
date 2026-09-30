#include "HAIDIA_BABI.H"

s32 ArcTan2(s32, s32);
s32 FieldScene_PrepareActors(s32);

extern u8 MsgHaidiaFolksSeemKnow[];

extern u8 MsgHaidiaHeyBoy[];
extern u8 MsgHaidiaTheMaskedManWasGarcia[];
void BattleFx_SetBlock30ValuesMaxZero(void);

void Map_ClearLayerEntryFlag();
void FieldScene_RunPaletteRampSequence();
void FieldScene_RunComplexActorSequence();
void BattleFx_StartTwelveFrameBlend();
void BattleFx_SetBlock30Values12Zero();
void BattleFx_SetBlock30Values128One();

/* The facing controller and the scene hooks the entry veneers export. */
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

s32 *HaidiaBabi_GetEntrances(void)
{
    return gHaidiaBabiEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 HaidiaBabi_SelectExits(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return (s32)gHaidiaBabiExits2;
    }
    return (s32)gHaidiaBabiExits;
}

s32 HaidiaBabi_SelectPlacements(void)
{
    u8 *b = (u8 *)&gGameState;
    s32 *tbl;

    if (*(s16 *)(b + 0x1c2) == 19)
        return (s32)gHaidiaBabiPlacements4;
    if (GameFlag_IsSet(0x87a) != 0)
        tbl = gHaidiaBabiPlacements3;
    else if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
        tbl = gHaidiaBabiPlacements2;
    else
        tbl = gHaidiaBabiPlacements;
    FieldScene_PrepareActors((s32)tbl);
    return (s32)tbl;
}

/* Asks whether the party knows Kraden, with a line for each answer. */
void HaidiaBabi_AskAboutKraden(s32 object)
{
    s32 msg = (s32)MsgHaidiaFolksSeemKnow;

    Event_SetMessage(msg);
    Event_OpenMessage(object, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(msg + 1);
    } else {
        Event_SetMessage(msg + 2);
    }
    Event_ShowMessage(object, 0);
}

/* The events hook, the villagers' scenes and the house's exits. */
s32 HaidiaBabi_SelectEvents(void)
{
    if (gGameState.entrance == 19) {
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)gHaidiaBabiEvents6;
        }
        return (s32)gHaidiaBabiEvents5;
    }

    if (GameFlag_IsSet(0x834) != 0) {
        return (s32)gHaidiaBabiEvents4;
    }
    if (GameFlag_IsSet(0x87A) != 0) {
        return (s32)gHaidiaBabiEvents3;
    }
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        return (s32)gHaidiaBabiEvents2;
    }
    return (s32)gHaidiaBabiEvents;
}

void HaidiaBabi_RunHeyBoyScene(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_StartRepeatedMotion(16, 2);
    Event_Wait(30);
    Event_SetMessage((s32)MsgHaidiaHeyBoy);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 16, 10);
    Event_ShowMessageAndWait(16, 0, 6);
    Actor_ShowEmote(16, 0x102, 0);
    Actor_StartRepeatedMotion(16, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Actor_StartRepeatedMotion(16, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 4);
    Event_End();
}

void SceneDialogue_RunActorFourteenDialogue11AA(void)
{
    void *work;

    Event_Begin();
    Actor_FaceActor(0xE, ACTOR_PARTY_LEADER, 0xA);
    Event_SetMessage((s32)MsgHaidiaTheMaskedManWasGarcia);
    Event_OpenMessage(0xE, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_ShowMessage(0xE, 0);
    } else {
        work = *(void **)&gEventWork;
        FIELD_AT_OFFSET(work, u16 *, 0x1D8) = (u16)(FIELD_AT_OFFSET(work, u16 *, 0x1D8) + 1);
        Event_AskYesNo(0xE, 0);
    }
    Event_End();
}

void SceneState_SetWork448To521AndRun(s32 object)
{
    if (GameFlag_IsSet(0x834) != 0) {
        BattleFx_SetBlock30ValuesMaxZero();
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(object);
}

void SceneState_SetValue123Mode1(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(1);
}

void FieldScene_RunStep7BThen2(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(2);
}

void SceneState_SetValue123Mode3(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(3);
}

void FieldScene_RunStep7BThen4(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(4);
}

void FieldScene_RunStep80Then5(void)
{
    Audio_PlayCue(0x80);
    SceneState_SetWork448To521AndRun(5);
}

void FieldScene_RunStep7BThen6(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(6);
}

void FieldScene_RunStep80Then7(void)
{
    Audio_PlayCue(0x80);
    SceneState_SetWork448To521AndRun(7);
}

void SceneState_SetValue129Mode8(void)
{
    Audio_PlayCue(0x81);
    SceneState_SetWork448To521AndRun(8);
}

void SceneState_SetValue129Mode9(void)
{
    Audio_PlayCue(0x81);
    SceneState_SetWork448To521AndRun(9);
}

void FieldScene_RunStep7BThen10(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(10);
}

void SceneState_ApplyValues123And11(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(11);
}

/* The entry hook: how the house is set up for the entrance and the story. */
s32 HaidiaBabi_RestoreEntryState(void)
{
    u32 i;
    s32 record;
    s32 base5_3001ebc;

    if (gGameState.entrance == 19) {
        Engine_GameFlagClear(0x12f);
        *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x209;
    } else {
        if (Engine_GameFlagIsSet(0x834) != 0) {
            Engine_ActorSetPosition(11, 0, 0);
            Engine_ActorSetPosition(12, 0, 0);
            Engine_ActorSetPosition(13, 0, 0);
            Engine_ActorSetPosition(14, 0, 0);
            Engine_ActorSetPosition(15, 0, 0);
            Engine_ActorSetPosition(16, 0, 0);
        } else {
            ActorPresentation_SetTwoSceneCells();
        }
        Engine_ActorSetSpritePriority(13, 1);
        if (Engine_GameFlagIsSet(0x87a) != 0) {
            record = (s32)Object_GetById(17);
            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
            if (gGameState.entrance != 6 && gGameState.entrance != 7) {
                goto L_02000550;
            }
            if (Engine_GameFlagIsSet(0x109) != 0) {
                record = Engine_GameFlagIsSet(0x203);
                if (record == 0) {
                    goto L_02000550;
                }
                Map_ClearLayerEntryFlag(12);
                goto L_02000550;
            }
            Map_ClearLayerEntryFlag(11);
            record = (s32)Object_GetById(8);
            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
            Engine_ActorSetAnimation(8, 10);
        } else {
            if (gGameState.entrance == 21) {
                FieldScene_RunPaletteRampSequence();
            } else {
                if (gGameState.entrance == 20) {
                    Engine_GameFlagSet(0x834);
                    FieldScene_RunComplexActorSequence();
                } else {
                    if (gGameState.entrance == 22) {
                        FieldScene_RunSupplementalSequenceOne();
                    } else {
                        base5_3001ebc = (u32)&gEventWork;
                        *(s32 *)((*(s32 *)base5_3001ebc + 0x1c0)) = 0x209;
                        if (Engine_GameFlagIsSet(0x834) != 0) {
                            BattleFx_StartTwelveFrameBlend();
                            {
                                u16 *target = (u16 *)((*(s32 *)(base5_3001ebc + 12) + 0x1f84));
                                s32 shown = 1;

                                *target = shown;
                            }
                            BattleFx_SetBlock30Values12Zero();
                            Engine_TaskWait(30);
                            Engine_EventOpenScreen();
                            Engine_EventWaitForScreen();
                            BattleFx_SetBlock30Values128One();
                        } else {
                            Engine_MapRedraw();
                            Engine_TaskWait(1);
                        }
                    }
                }
            }
        }
    }
    L_02000550:;
    return 0;
}

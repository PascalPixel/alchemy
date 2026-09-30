#include "SANCTUM.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TYPES.H"

/* The scene's tables, which the entry veneers publish to the map engine. */
extern const struct SceneEntrance gSoruNichigetsuEntrances[];
extern const u32 gSoruNichigetsuExits[];
extern const struct ScenePlacement gSoruNichigetsuPlacements[];
extern const struct SceneEvent gSoruNichigetsuEvents[];

extern u8 MsgSoruWayLeadsOutSanctum[];
extern u8 MsgSoruThank[];
extern u8 MsgSoruFound[];
extern u8 MsgSoruGoBackVillage[];
extern u8 MsgSoruLunaSolRooms[];
extern u8 MsgSoruMeanLookFarther[];
extern u8 MsgSoruPutWayDont[];
extern u8 MsgSoruRoomLunaOne[];
extern u8 MsgSoruWhRoom[];

extern u8 MsgSoruLookSymbolFloor[];
extern u8 MsgSoruThePictureOfLunaChanged[];
extern u8 MsgSoruWhatsHappening[];
extern u8 MsgSoruWhatsHappeningAtTheTrap[];
extern u8 MsgSoruYouFoundIt[];

extern u8 MsgSoruHa[];
void SetSolShindenActorStep();
s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_ActorWalkToAndWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetAnimation();
void Engine_ActorSetPosition();
void Engine_MapCopyCellsTo();
void Engine_EventWait();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_ActorFaceDirection();
void Map_ClearLayerEntryFlag();
void Engine_AudioPlayCue();
void Engine_ActorShowEmote();
void Object_LinkPair();
void Engine_TaskWait();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void ObjectGroup_ConfigureChildValue();
struct FieldActor *Object_GetById();
void Engine_ActorJump();
void Engine_ActorRunRepeatedMotion();
void Engine_EventEnd();
void Engine_ColorBufferApplyTarget();
void Engine_EventRequestExit();
void Engine_EventCloseScreen();
void Engine_ColorBufferInterpolate();
void Engine_EventWaitForScreen();

extern struct EventWork *gEventWork;
s32 Engine_GameFlagSet();
void Engine_ColorBufferApplySource();
void Scene_SpringStatueTrap();
void FieldScene_RunClosingSequence();
void Engine_GameFlagClear();
void FieldScene_RunFlaggedSequence();
s32 CheckAllStatueLights();
void Engine_MapCopyCellAttributes();
void SoruNichigetsu_RunLightScene();
void BattleFx_SetQueuedSoundAndPlay();
void Engine_WorkSetValuesIfNonNegative();
void InitializeSceneRecordBuffer();
extern u8 Data_02000240[];
extern s16 gCell[][1];

extern u8 MsgSoruFloatingEyeThing[];
void InitializeSceneRecordBuffer(void);
void BattleFx_SetQueuedSoundAndPlay(s32 value);
void SetSolShindenActorStep(s32 actor_step, s32 wait_frames);

/* The IWRAM event globals: the event work, then the field work at +0x14. */
struct EventGlobals {
    struct EventWork *event;
    u8 unknown_04[0x10];
    u8 *field;
};

extern struct EventGlobals Data_03001ebc;

extern u8 MsgSoruSukuretaJustWaitOverThere[];
extern u8 MsgSoruSukuretaLetMeKnowWhat[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSoruNichigetsuEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gSoruNichigetsuExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gSoruNichigetsuPlacements;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gSoruNichigetsuEvents;
}

void FieldScene_RunScene37aSequenceA(void)
{
    u32 i;
    s32 record;

    if (CheckAllStatueLights()!= 0) {
        record = GameFlag_IsSet(0x201);
        if (record != 0) {
            goto L_020000f0;
        }
        Event_Begin();
        ColorBuffer_ApplyTarget(0x2051cc, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x201);
        GameFlag_Clear(0x200);
        GameFlag_Clear(0x202);
        if (GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
            Scene_EnterInnerSanctum();
        }
        if (CheckAllStatueLights()!= 0) {
            if (GameFlag_IsSet(0x811) == 0) {
                FieldScene_RunActorPositionTransition();
            }
        }
        Event_End();
    } else {
        if (GameFlag_IsSet(0x200) == 0) {
            Event_Begin();
            ColorBuffer_ApplyTarget(0x10000, 1);
            ColorBuffer_Interpolate(20);
            GameFlag_Set(0x200);
            GameFlag_Clear(0x201);
            GameFlag_Clear(0x202);
            Event_End();
        }
    }
    L_020000f0:;
}

void FieldScene_RunScene37aSequenceB(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x200) == 0) {
        Event_Begin();
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x200);
        GameFlag_Clear(0x201);
        GameFlag_Clear(0x202);
        Event_End();
    }
}

void FieldScene_RunScene37aSequenceC(void)
{
    u32 i;
    s32 record;

    if (CheckAllStatueLights()!= 0) {
        record = GameFlag_IsSet(0x200);
        if (record != 0) {
            goto L_020001d6;
        }
        Event_Begin();
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x200);
        GameFlag_Clear(0x201);
        GameFlag_Clear(0x202);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x201) == 0) {
            Event_Begin();
            ColorBuffer_ApplyTarget(0x2051cc, 1);
            ColorBuffer_Interpolate(20);
            GameFlag_Set(0x201);
            GameFlag_Clear(0x200);
            GameFlag_Clear(0x202);
            if (GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
                Scene_EnterInnerSanctum();
            }
            Event_End();
        }
    }
    L_020001d6:;
}

void FieldScene_RunScene37aSequenceD(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x202) == 0) {
        ColorBuffer_ApplyTarget(0x202db1, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x202);
        GameFlag_Clear(0x200);
        GameFlag_Clear(0x201);
    }
}

void ClearSolShindenBackdrop(void)
{
    s32 black = 0;
    u16 *backdrop_color = (u16 *)0x5000000;
    *backdrop_color = black;
}

void SetStatueLightGroup1(void)
{
    if (Engine_GameFlagIsSet(FLAG_STATUE_LIGHT_1) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 28, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_2) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 28, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_3) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 29, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_4) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup2(void)
{
    if (GameFlag_IsSet(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 28, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 28, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 29, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup3(void)
{
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_1) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 30, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_2) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 30, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_3) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 31, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_4) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 31, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup4(void)
{
    if (GameFlag_IsSet(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 30, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 30, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 31, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 31, 0x24, 11, a, b);
    }
}

void FieldScene_RunScene37aSequenceF(void)
{
    u8 *rec;

    if (Engine_GameFlagIsSet(0x814) != 0) {
        FieldScene_RunScene37aSequenceA();
    }
    if (Engine_GameFlagIsSet(0x809) == 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgSoruFound);
        Engine_AudioPlayCue(17);
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Call3(Engine_ActorWalkToAndWait, 0, 0x120, 232);
        Engine_ActorSetAnimation(0, 0);
        Engine_EventWait(20);
        Engine_AudioPlayCue(21);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        rec = (u8 *)Object_GetById(0);
        if (rec != 0) {
            Engine_ActorSetPosition(16, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Call3(Engine_ActorSetSpeed, 16, 0x16666, 0xb333);
        Call3(Engine_ActorWalkToAndWait, 16, 0x120, 206);
        Engine_EventWait(40);
        Call3(Engine_ActorShowEmote, 16, 0x100, 0);
        Engine_ActorJump(16, 4, 60);
        SetSolShindenActorStep(16, 20);
        rec = (u8 *)Object_GetById(0);
        if (rec != 0) {
            Engine_ActorSetPosition(1, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        rec = (u8 *)Object_GetById(0);
        if (rec != 0) {
            Engine_ActorSetPosition(5, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Call3(Engine_ActorSetSpeed, 1, 0x8000, 0x4000);
        Call3(Engine_ActorSetSpeed, 5, 0x8000, 0x4000);
        Call3(Engine_ActorWalkTo, 1, 0x118, 248);
        Call3(Engine_ActorWalkToAndWait, 5, 0x128, 248);
        Engine_ActorSetAnimation(1, 1);
        Call3(Engine_ActorFaceDirection, 1, 0xd000, 0);
        Call3(Engine_ActorFaceDirection, 5, 0xb000, 30);
        Engine_CameraSetSpeed(0x9999, 0x1333);
        Camera_MoveTo(0x1200000, -1, 0xd50000, 1);
        Call3(Engine_ActorSetSpeed, 16, 0x6666, 0x3333);
        Engine_ActorWalkToAndWait(16, 0x120, 176);
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(16, 2);
        SetSolShindenActorStep(16, 6);
        Call3(Engine_ActorFaceDirection, 16, 0x4000, 60);
        SetSolShindenActorStep(16, 20);
        Engine_ActorFaceDirection(16, 0, 40);
        Engine_ActorSetAnimationAndWait(16, 3);
        Engine_EventWait(10);
        Call3(Engine_ActorFaceDirection, 16, 0x8000, 40);
        Engine_ActorSetAnimationAndWait(16, 3);
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(5, 2);
        Call3(Engine_ActorFaceDirection, 5, 0x9000, 10);
        SetSolShindenActorStep(5, 10);
        Engine_ActorRunRepeatedMotion(1, 2);
        Call3(Engine_ActorFaceDirection, 1, 0xf000, 10);
        SetSolShindenActorStep(1, 6);
        Call2(Engine_ActorSetAttachedEffect, 5, 0x102);
        Engine_EventWait(40);
        Call3(Engine_ActorFaceDirection, 5, 0xa000, 10);
        SetSolShindenActorStep(0x2005, 10);
        Engine_ActorRunRepeatedMotion(16, 2);
        Engine_EventWait(10);
        Call3(Engine_ActorFaceDirection, 16, 0xa000, 20);
        Call2(Engine_ActorSetAttachedEffect, 16, 0x102);
        Engine_EventWait(20);
        Call3(Engine_ActorFaceDirection, 0, 0x5000, 40);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
        Call3(Engine_ActorFaceDirection, 5, 0xa000, 40);
        Call3(Engine_ActorShowEmote, 1, 0x101, 20);
        Engine_EventShowMessage(1, 0);
        Engine_EventWait(60);
        Engine_ActorSetAnimationAndWait(16, 4);
        Engine_EventWait(40);
        SetSolShindenActorStep(16, 20);
        Call3(Engine_ActorShowEmote, 5, 0x101, 40);
        SetSolShindenActorStep(5, 60);
        Engine_ActorSetAnimationAndWait(16, 3);
        SetSolShindenActorStep(16, 10);
        Call3(Engine_ActorShowEmote, 0, 0x105, 0);
        Call3(Engine_ActorShowEmote, 1, 0x105, 0);
        Call3(Engine_ActorShowEmote, 5, 0x105, 60);
        Engine_ActorRunRepeatedMotion(1, 2);
        Engine_EventWait(20);
        SetSolShindenActorStep(1, 10);
        Engine_ActorSetAnimationAndWait(16, 3);
        Engine_EventWait(20);
        Call3(Engine_ActorShowEmote, 0, 0x102, 0);
        Call3(Engine_ActorShowEmote, 1, 0x102, 0);
        Call3(Engine_ActorShowEmote, 5, 0x102, 80);
        Call3(Engine_ActorShowEmote, 16, 0x105, 80);
        SetSolShindenActorStep(16, 6);
        Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xf000, 0);
        Call3(Engine_ActorFaceDirection, 5, 0x9000, 60);
        Call3(Engine_ActorFaceDirection, 16, 0x4000, 10);
        Engine_ActorRunRepeatedMotion(16, 3);
        Engine_EventWait(6);
        Engine_EventOpenMessage(16, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventSetMessage((s32)MsgSoruThank);
        } else {
            Engine_EventSetMessage((s32)MsgSoruGoBackVillage);
            Call3(Engine_ActorShowEmote, 16, 0x107, 20);
        }
        Engine_ActorJump(16, 4, 20);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
        Call3(Engine_ActorFaceDirection, 5, 0xa000, 0);
        SetSolShindenActorStep(16, 6);
        Engine_EventSetMessage((s32)MsgSoruPutWayDont);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(5, 4);
        SetSolShindenActorStep(0x2005, 6);
        Engine_ActorSetAnimationAndWait(1, 3);
        SetSolShindenActorStep(1, 20);
        Engine_ActorJump(16, 6, 20);
        Call3(Engine_ActorShowEmote, 16, 0x104, 20);
        SetSolShindenActorStep(16, 30);
        Engine_ActorSetAnimation(0, 3);
        Engine_ActorSetAnimation(1, 3);
        Engine_ActorSetAnimationAndWait(5, 3);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(16, 3);
        SetSolShindenActorStep(16, 6);
        Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
        Call3(Engine_ActorSetSpeed, 5, 0x10000, 0x8000);
        Call3(Engine_ActorSetSpeed, 16, 0x20000, 0x10000);
        Engine_ActorSetAnimation(16, 2);
        rec = (u8 *)Object_GetById(0);
        if (rec != 0) {
            Actor_SetDestination(ACTOR_SUKURETA, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Engine_ActorWaitForMove(16);
        Engine_ActorSetPosition(16, 0, 0);
        Engine_ActorSetAnimation(1, 2);
        rec = (u8 *)Object_GetById(0);
        if (rec != 0) {
            Actor_SetDestination(ACTOR_GERALD, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Engine_ActorWaitForMove(1);
        Engine_ActorSetPosition(1, 0, 0);
        Engine_ActorSetAnimation(5, 2);
        rec = (u8 *)Object_GetById(0);
        if (rec != 0) {
            Actor_SetDestination(ACTOR_JASMINE, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Engine_ActorWaitForMove(5);
        Engine_ActorSetPosition(5, 0, 0);
        Engine_GameFlagSet(0x144);
        Engine_GameFlagSet(0x809);
        Engine_EventEnd();
    }
}

void Scene_EnterInnerSanctum(void)
{
    u32 i;
    s32 record;
    s32 request;

    Event_SetMessage((s32)MsgSoruWhRoom);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1e8, 176);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_SUKURETA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 1);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d8, 168);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 60);
    Actor_Jump(ACTOR_SUKURETA, 4, 40);
    SetSolShindenActorStep(16, 6);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x23f0000, -1, 0xb50000, 1);
    Camera_WaitForMove();
    Event_Wait(120);
    SetSolShindenActorStep(0x1010, 80);
    Camera_MoveTo(0x1ec0000, -1, 0xa80000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 20);
    SetSolShindenActorStep(0x4010, 6);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 60);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Event_OpenMessage(0x4010, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage((s32)MsgSoruLunaSolRooms);
    } else {
        Event_SetMessage((s32)MsgSoruRoomLunaOne);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 10);
    SetSolShindenActorStep(0x4010, 10);
    request = (s32)MsgSoruMeanLookFarther;
    Event_SetMessage(request);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 40);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x105, 40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Actor_SetAnimation(ACTOR_SUKURETA, 4);
    Event_OpenMessage(0x4010, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage((request + 1));
        GameFlag_Set(FLAG_ROBIN_SEARCHING_FOR_SUKURETA);
    } else {
        Event_SetMessage((request + 2));
    }
    SetSolShindenActorStep(0x4010, 4);
    Camera_FollowActor(ACTOR_SUKURETA, 1);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1e6, 131);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 120);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 2);
    Camera_SetSpeed(0x40000, 0x8000);
    GameFlag_Set(FLAG_INNER_SANCTUM_ENTERED);
}

void UpdateStatueTrapActor(void)
{
    EntA *scene_actor;
    EntA *target_actor;
    EntB *target_position;

    scene_actor = Object_GetById(16);
    if (GameFlag_IsSet(0x809) == 0) {
        return;
    }
    if (GameFlag_IsSet(0x814) != 0) {
        FieldScene_RunScene37aSequenceB();
        return;
    }
    if (GameFlag_IsSet(0x819) != 0) {
        return;
    }
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    Event_SetMessage((s32)MsgSoruWayLeadsOutSanctum);
    if (GameFlag_IsSet(0x810) != 0 || GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
        target_actor = Object_GetById(0);
        if (target_actor != 0) {
            Actor_SetPosition(ACTOR_SUKURETA, target_actor->unk8, target_actor->unk10);
        }
        Event_Wait(4);
        Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
    } else if (GameFlag_IsSet(0x810) != 0 || scene_actor->unk8 > 0x1540000) {
        Actor_SetPosition(ACTOR_SUKURETA, 0x1880000, 0xa80000);
        Event_Wait(4);
        Actor_SetSpeed(ACTOR_SUKURETA, 0x20000, 0x10000);
    }
    if (GameFlag_IsSet(0x810) != 0 || scene_actor->unk8 > 0x1540000) {
        Actor_WalkToAndWait(ACTOR_SUKURETA, 0x120, 0xe8);
    } else {
        /* FAKEMATCH: the reference tests the entered flag here and drops
         * the result; no use of it is recovered. */
        GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED);
    }
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x120, 0xe8);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    if (GameFlag_IsSet(0x810) != 0 || GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
        Actor_SetAnimation(ACTOR_SUKURETA, 2);
        target_position = Object_GetById(0);
        if (target_position != 0) {
            Actor_SetDestination(ACTOR_SUKURETA, target_position->unkA, target_position->unk12);
        }
        Actor_WaitForMove(ACTOR_SUKURETA);
        Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x120, 0xe8);
    } else {
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x120, 0xf8);
    }
    Event_End();
}

void UpdateStatueLight1(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (SoruNichigetsu_Light1Timer) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x21, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x21, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x22, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x22, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x23, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x23, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x24, 1, 5);
            break;
        case 4:
            SoruNichigetsu_FlashState = 2;
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x24, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 30, 0x25, 1, 5);
            break;
        case 0x50:
            Map_CopyCellsTo(0x2e, 0x31, 30, 0x21, 1, 10);
            break;
        }
        SoruNichigetsu_Light1Timer++;
        if (SoruNichigetsu_Light1Timer > ((u32)(Random_Next() * 40) >> 16) + 90) {
            SoruNichigetsu_Light1Timer = 0;
        }
    }
    if (SoruNichigetsu_FlashState != 0) {
        if (SoruNichigetsu_FlashState == 2) {
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        } else if (SoruNichigetsu_FlashState == 1) {
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        }
        SoruNichigetsu_FlashState--;
    }
}

void UpdateStatueLight2(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (SoruNichigetsu_Light2Timer) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x21, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x21, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x22, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x22, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x23, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x23, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x24, 1, 5);
            break;
        case 4:
            SoruNichigetsu_FlashState = 2;
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x24, 1, 1);
            Map_CopyCellsTo(0x2f, 0x3b, 42, 0x25, 1, 5);
            break;
        case 0x5a:
            Map_CopyCellsTo(0x2f, 0x31, 42, 0x21, 1, 10);
            break;
        }
        SoruNichigetsu_Light2Timer++;
        if (SoruNichigetsu_Light2Timer > ((u32)(Random_Next() * 40) >> 16) + 100) {
            SoruNichigetsu_Light2Timer = 0;
        }
    }
}

void UpdateStatueLight3(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (SoruNichigetsu_Light3Timer) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x24, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x24, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x25, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x25, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x26, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x26, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x27, 1, 5);
            break;
        case 4:
            SoruNichigetsu_FlashState = 2;
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x27, 1, 1);
            Map_CopyCellsTo(0x30, 0x3b, 31, 0x28, 1, 5);
            break;
        case 0x5f:
            Map_CopyCellsTo(0x30, 0x31, 31, 0x24, 1, 10);
            break;
        }
        SoruNichigetsu_Light3Timer++;
        if (SoruNichigetsu_Light3Timer > ((u32)(Random_Next() * 40) >> 16) + 105) {
            SoruNichigetsu_Light3Timer = 0;
        }
    }
}

void UpdateStatueLight4(void)
{
    if ((Random_Next() & 3) != 0) {
        switch (SoruNichigetsu_Light4Timer) {
        case 0:
            Audio_PlayCue(0xbb);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x24, 1, 5);
            break;
        case 1:
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x24, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x25, 1, 5);
            break;
        case 2:
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x25, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x26, 1, 5);
            break;
        case 3:
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x26, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x27, 1, 5);
            break;
        case 4:
            SoruNichigetsu_FlashState = 2;
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x27, 1, 1);
            Map_CopyCellsTo(0x2e, 0x3b, 41, 0x28, 1, 5);
            break;
        case 0x55:
            Map_CopyCellsTo(0x2e, 0x31, 41, 0x24, 1, 10);
            break;
        }
        SoruNichigetsu_Light4Timer++;
        if (SoruNichigetsu_Light4Timer > ((u32)(Random_Next() * 40) >> 16) + 95) {
            SoruNichigetsu_Light4Timer = 0;
        }
    }
}

void FieldScene_PrepareStatueTransition(void)
{
    u32 i;
    s32 record;

    Camera_MoveTo(-1, -1, -1, 0);
    Map_CopyCellsTo(30, 43, 32, 40, 8, 3);
    Map_CopyCellsTo(30, 43, 33, 39, 8, 1);
    Map_CopyCellsTo(30, 43, 36, 38, 3, 3);
    Map_CopyCellsTo(14, 41, 32, 41, 8, 4);
    Camera_MoveTo(0x23e0000, -1, 0x9e0000, 0);
    Map_Redraw();
    Actor_SetPosition(ACTOR_SUKURETA, 0x23e0000, 0x780000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Task_Wait(1);
    ColorBuffer_ApplyTarget(0x2051cc, 1);
    ColorBuffer_Interpolate(20);
    GameFlag_Set(0x201);
    GameFlag_Clear(0x200);
    GameFlag_Clear(0x202);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    Engine_EventWaitForScreen();
    Event_Wait(40);
    Audio_PlayCue(171);
    ColorBuffer_ApplyTarget(0x10005, 1);
    ColorBuffer_Interpolate(8);
    Event_Wait(32);
    Engine_ColorBufferApplyTarget(0x2051cc, 1);
    ColorBuffer_Interpolate(24);
}

void Scene_SpringStatueTrap(void)
{
    s32 outer_pair;
    s32 second_pair;
    s32 middle_pair;
    s32 fourth_pair;
    s32 inner_pair;

    Event_Begin();
    FieldScene_PrepareStatueTransition();
    Event_SetMessage((s32)MsgSoruWhatsHappeningAtTheTrap);
    SetInitialScale(ACTOR_SUKURETA, 0x4000, 20);
    SetInitialDirection(ACTOR_SUKURETA, 256, 0);
    Actor_Jump(ACTOR_SUKURETA, 6, 30);
    Camera_MoveTo(37617664, -1, 11403264, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    SetSolShindenActorStep(32784, 20);
    for (outer_pair = 0; outer_pair != 4; outer_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(12);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(12);
    }
    for (second_pair = 0; second_pair != 6; second_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(8);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(8);
    }
    for (middle_pair = 0; middle_pair != 8; middle_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(6);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(6);
    }
    for (fourth_pair = 0; fourth_pair != 10; fourth_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(4);
        Engine_AudioPlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(4);
    }
    for (inner_pair = 0; inner_pair != 12; inner_pair++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(2);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(2);
    }
    SetStatueLightGroup1();
    Event_Wait(6);
    SetSolShindenActorStep(32784, 6);
    SetFinalScale(ACTOR_SUKURETA, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 576, 280);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(FLAG_STATUE_TRAP_SPRUNG);
    Event_RequestExit(3);
}

void FieldScene_RunClosingSequence(void)
{
    s32 i;
    Event_Begin();
    FieldScene_PrepareStatueTransition();
    SoruNichigetsu_Light1Timer = 0;
    SoruNichigetsu_Light2Timer = 0;
    SoruNichigetsu_Light3Timer = 0;
    SoruNichigetsu_Light4Timer = 0;
    Event_SetMessage((s32)MsgSoruWhatsHappening);
    Actor_FaceDirection(ACTOR_SUKURETA, 16384, 20);
    Actor_ShowEmote(ACTOR_SUKURETA, 256, 0);
    Actor_Jump(ACTOR_SUKURETA, 6, 30);
    Camera_MoveTo(37617664, -1, 11403264, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    SetSolShindenActorStep(32784, 20);
    for (i = 0; i != 4; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(12);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(12);
    }
    SetSolShindenActorStep(32784, 6);
    SoruNichigetsu_Light1Timer = (((u32)Engine_RandomNext() * 60) >> 16) + 20;
    SoruNichigetsu_Light2Timer = (((u32)Engine_RandomNext() * 60) >> 16) + 20;
    SoruNichigetsu_Light3Timer = (((u32)Engine_RandomNext() * 60) >> 16) + 20;
    SoruNichigetsu_Light4Timer = (((u32)Engine_RandomNext() * 60) >> 16) + 20;
    SoruNichigetsu_FlashState = 0;
    Value2(Engine_TaskAddCallback, (s32)UpdateStatueLight1, 3200);
    Value2(Engine_TaskAddCallback, (s32)UpdateStatueLight2, 3200);
    Value2(Engine_TaskAddCallback, (s32)UpdateStatueLight3, 3200);
    Engine_TaskAddCallback((s32)UpdateStatueLight4, 3200);
    for (i = 0; i != 6; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(5);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(5);
    }
    for (i = 0; i != 8; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(4);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(4);
    }
    for (i = 0; i != 10; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(3);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(3);
    }
    for (i = 0; i != 12; i++) {
        Audio_PlayCue(246);
        SetStatueLightGroup1();
        Event_Wait(2);
        Audio_PlayCue(246);
        SetStatueLightGroup3();
        Event_Wait(2);
    }
    Map_CopyCellsTo(45, 30, 34, 10, 4, 2);
    Actor_Jump(ACTOR_SUKURETA, 6, 40);
    SetSolShindenActorStep(32784, 6);
    Actor_SetSpeed(ACTOR_SUKURETA, 131072, 65536);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 576, 280);
    Engine_TaskRemoveCallback((s32)UpdateStatueLight1);
    Engine_TaskRemoveCallback((s32)UpdateStatueLight2);
    Engine_TaskRemoveCallback((s32)UpdateStatueLight3);
    Engine_TaskRemoveCallback((s32)UpdateStatueLight4);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(4);
}

void FieldScene_RunFlaggedSequence(void)
{
    s32 base;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 i9;
    s32 i10;

    Event_Begin();
    if (GameFlag_IsSet(2059) != 0 && GameFlag_IsSet(2086) != 0) {
        GameFlag_Clear(2086);
        Map_CopyCellsTo(45, 28, 34, 10, 2, 1);
    } else if (GameFlag_IsSet(2059) != 0 && GameFlag_IsSet(2086) == 0) {
        GameFlag_Set(2086);
    }
    if (GameFlag_IsSet(2060) != 0 && GameFlag_IsSet(2087) != 0) {
        GameFlag_Clear(2087);
        Map_CopyCellsTo(47, 28, 36, 10, 2, 1);
    } else if (GameFlag_IsSet(2060) != 0 && GameFlag_IsSet(2087) == 0) {
        GameFlag_Set(2087);
    }
    if (GameFlag_IsSet(2061) != 0 && GameFlag_IsSet(2088) != 0) {
        GameFlag_Clear(2088);
        Map_CopyCellsTo(45, 29, 34, 11, 2, 1);
    } else if (GameFlag_IsSet(2061) != 0 && GameFlag_IsSet(2088) == 0) {
        GameFlag_Set(2088);
    }
    if (GameFlag_IsSet(2062) != 0 && GameFlag_IsSet(2089) != 0) {
        GameFlag_Clear(2089);
        Map_CopyCellsTo(47, 29, 36, 11, 2, 1);
    } else if (GameFlag_IsSet(2062) != 0 && GameFlag_IsSet(2089) == 0) {
        GameFlag_Set(2089);
    }
    FieldScene_PrepareStatueTransition();
    Actor_FaceDirection(ACTOR_SUKURETA, 16384, 20);
    Actor_Jump(ACTOR_SUKURETA, 6, 30);
    Camera_MoveTo(37617664, -1, 11403264, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    for (i6 = 0; i6 != 4; i6++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(12);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(12);
    }
    for (i7 = 0; i7 != 6; i7++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(8);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(8);
    }
    for (i8 = 0; i8 != 8; i8++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(6);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(6);
    }
    for (i9 = 0; i9 != 10; i9++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(4);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(4);
    }
    for (i10 = 0; i10 != 12; i10++) {
        Audio_PlayCue(246);
        SetStatueLightGroup2();
        Event_Wait(2);
        Audio_PlayCue(246);
        SetStatueLightGroup4();
        Event_Wait(2);
    }
    Audio_PlayCue(246);
    SetStatueLightGroup2();
    Event_Wait(6);
    if (GameFlag_IsSet(2082) == 0) {
        base = 32784;
        Event_SetMessage((s32)MsgSoruYouFoundIt);
        SetSolShindenActorStep(base, 6);
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
        SetSolShindenActorStep(base, 6);
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_CloseScreen();
    Engine_EventWaitForScreen();
    Event_RequestExit(5);
}

void Scene_ChangeLunaPictureToSol(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(FLAG_LUNA_PICTURE_CHANGED_TO_SOL) != 0) {
    } else {
        if (CheckAllStatueLights() == 0) {
        } else {
            Event_Begin();
            Actor_SetPosition(ACTOR_SUKURETA, 0x2410000, 0x930000);
            Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 1);
            Camera_MoveTo(0x23e0000, -1, 0xb80000, 1);
            Event_SetMessage((s32)MsgSoruThePictureOfLunaChanged);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x240, 232);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
            Camera_WaitForMove();
            Event_Wait(10);
            Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 152);
            Event_Wait(6);
            Actor_Jump(ACTOR_SUKURETA, 6, 30);
            SetSolShindenActorStep(16, 6);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Event_Wait(2);
            Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
            SetSolShindenActorStep(16, 6);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
            Event_Wait(40);
            Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
            Event_Wait(30);
            SetSolShindenActorStep(16, 6);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 184);
            Event_Wait(6);
            Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
            Event_Wait(40);
            SetSolShindenActorStep(0x4010, 6);
            Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 208);
            Event_Wait(40);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Event_Wait(6);
            Actor_SetSpeed(ACTOR_SUKURETA, 0x8000, 0x4000);
            Actor_SetAnimation(ACTOR_SUKURETA, 2);
            record = Object_GetById(0);
            if (record != 0) {
                Actor_SetDestination(ACTOR_SUKURETA, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Actor_WaitForMove(ACTOR_SUKURETA);
            Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
            GameFlag_Set(FLAG_LUNA_PICTURE_CHANGED_TO_SOL);
            Event_End();
        }
    }
}

void FieldScene_RunActorPositionTransition(void)
{
    u32 i;
    s32 record;

    Engine_AudioPlayCue(21);
    Call3(Engine_ActorWalkToAndWait, 0, 0x178, 184);
    Engine_ActorSetAnimation(0, 0);
    Call3(Engine_ActorSetPosition, 16, 0x1780000, 0xb80000);
    Call3(Engine_ActorSetSpeed, 16, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 16, 0x188, 168);
    Call3(Engine_ActorFaceDirection, 16, 0x8000, 30);
    Engine_ActorSetAnimation(16, 1);
    Event_SetMessage((s32)MsgSoruLookSymbolFloor);
    Engine_ActorJump(16, 4, 30);
    SetSolShindenActorStep(16, 6);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(6);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    SetSolShindenActorStep(16, 6);
    Call3(Engine_ActorWalkToAndWait, 16, 0x178, 184);
    Call3(Engine_ActorSetPosition, 16, 0x6480000, 0x6480000);
    Engine_EventWait(4);
    Engine_GameFlagSet(0x811);
}

/*
 * Exact 2026-09-23 (1,120 bytes), with two tagged fake matches for the
 * zero stores of the two presentations. Both rise-and-shrink loops of each
 * actor share one counter.
 */
void FieldScene_RunSanctumRiseAndShrink(void)
{
    u32 i;
    struct FieldActor *actor;
    s32 record;
    s32 base5_8010;
    s32 base5_0;
    struct FieldSprite *sprite;

    if (Engine_GameFlagIsSet(0x811) == 0) {
    } else {
        Engine_EventBegin();
        Call2(Engine_CameraSetSpeed, 0x10000, 0x2000);
        Call4(Engine_CameraMoveTo, 0x11f0000, -1, 0x940000, 1);
        Call3(Engine_ActorWalkToAndWait, 0, 0x120, 120);
        Engine_ActorSetAnimation(0, 0);
        Actor_Jump(ACTOR_PARTY_LEADER, 4, 30);
        Call3(Engine_ActorSetPosition, 16, 0x1200000, 0x780000);
        Call3(Engine_ActorSetSpeed, 16, 0x10000, 0x8000);
        Actor_WalkToAndWait(16, 0x114, 136);
        Actor_WalkTo(16, 0x108, 136);
        Call3(Engine_ActorWalkToAndWait, 0, 0x138, 136);
        Engine_ActorSetAnimation(0, 1);
        Engine_ActorSetAnimation(16, 1);
        Call3(Engine_ActorFaceDirection, 0, 0xb000, 0);
        Engine_ActorFaceDirection(16, 0xd000, 20);
        if (Value1(Engine_GameFlagIsSet, 0x819) == 0) {
            Engine_AudioPlayCue(220);
        }
        Engine_EventWait(40);
        if (GameFlag_IsSet(0x819) != 0) {
        } else {
            Map_CopyCellsTo(36, 62, 17, 36, 2, 3);
            Engine_MapCopyCellsTo(44, 59, 17, 38, 2, 1);
            Engine_EventWait(10);
            Engine_MapCopyCellsTo(38, 62, 17, 36, 2, 3);
            Engine_MapCopyCellsTo(44, 59, 17, 39, 2, 1);
            Engine_EventWait(10);
            Engine_MapCopyCellsTo(40, 62, 17, 36, 2, 3);
            Engine_MapCopyCellsTo(0, 32, 17, 39, 2, 1);
            Engine_MapCopyCellsTo(44, 59, 17, 40, 2, 1);
            Engine_EventWait(10);
            Call3(Engine_ActorShowEmote, 0, 0x100, 0);
            Call3(Engine_ActorShowEmote, 16, 0x100, 0);
            Engine_MapCopyCellsTo(42, 62, 17, 36, 2, 3);
            Engine_MapCopyCellsTo(0, 32, 17, 40, 2, 1);
            Engine_MapCopyCellsTo(44, 59, 17, 41, 2, 1);
            Event_Wait(10);
            Engine_MapCopyCellsTo(0, 32, 17, 41, 2, 1);
            Engine_MapCopyCellsTo(44, 59, 17, 42, 2, 1);
            Engine_EventWait(10);
            Map_CopyCellsTo(0, 32, 17, 42, 2, 3);
            Engine_EventWait(80);
            Map_ClearLayerEntryFlag(9);
            Map_ClearLayerEntryFlag(10);
            Engine_GameFlagSet(0x819);
        }
        Object_LinkPair(16, 0, 30);
        Engine_ActorSetAnimation(16, 3);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(16, 1);
        base5_8010 = 0x8010;
        Engine_ActorSetAnimation(0, 0);
        Engine_EventSetMessage((s32)MsgSoruHa);
        SetSolShindenActorStep(base5_8010, 6);
        Actor_SetAnimationAndWait(16, 3);
        Actor_SetAnimation(16, 1);
        Engine_EventShowMessage(base5_8010, 0);
        Engine_ActorSetAnimation(0, 3);
        Engine_EventWait(60);
        actor = Actor_Get(ACTOR_PARTY_LEADER);
        Engine_CameraSetSpeed(0x9999, 0x1333);
        Engine_CameraMoveTo(0x11f0000, -1, 0x720000, 1);
        Engine_ActorWalkToAndWait(0, 0x120, 120);
        Engine_EventWait(20);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 20);
        Call3(Engine_ActorSetSpeed, 0, 0x4ccc, 0x2666);
        actor->unknown_5a &= 254;
        /* FAKEMATCH: the zero goes through the sprite variable (r7). */
        sprite = 0;
        actor->motion_flags = (u32)sprite;
        Engine_AudioPlayCue(201);
        Call2(ObjectGroup_ConfigureChildValue, 0, 0x100);
        sprite = actor->sprite;
        base5_0 = 0;
        sprite->flags = 0;
        do {
            actor->y.fixed += 0x3333;
            Task_Wait(1);
            base5_0++;
        } while (base5_0 != 120);
        Engine_AudioPlayCue(190);
        base5_0 = 0;
        do {
            actor->y.fixed += 0x1999;
            sprite->scale += -0x400;
            Engine_TaskWait(1);
            base5_0++;
        } while (base5_0 != 60);
        Engine_ActorSetPosition(0, 0, 0);
        Engine_ActorJump(16, 4, 20);
        SetSolShindenActorStep(16, 6);
        Call3(Engine_ActorWalkToAndWait, 16, 0x120, 120);
        Engine_ActorRunRepeatedMotion(16, 2);
        Engine_EventWait(20);
        Call3(Engine_ActorFaceDirection, 16, 0xc000, 20);
        Engine_ActorSetSpeed(16, 0x4ccc, 0x2666);
        actor = Object_GetById(16);
        /* FAKEMATCH: a mask temporary delays both byte stores past the zero. */
        {
            s32 m = 254;

            m &= actor->unknown_5a;
            base5_0 = 0;
            actor->unknown_5a = m;
            actor->motion_flags = base5_0;
        }
        Engine_AudioPlayCue(201);
        Call2(ObjectGroup_ConfigureChildValue, 16, 0x100);
        sprite = actor->sprite;
        sprite->flags = 0;
        do {
            actor->y.fixed += 0x3333;
            Engine_TaskWait(1);
            base5_0++;
        } while (base5_0 != 120);
        Engine_AudioPlayCue(190);
        base5_0 = 0;
        do {
            actor->y.fixed += 0x1999;
            sprite->scale += -0x400;
            Engine_TaskWait(1);
            base5_0++;
        } while (base5_0 != 60);
        Engine_ActorSetPosition(16, 0, 0);
        Engine_EventWait(80);
        gEventWork->start_transition = 0x203;
        gEventWork->transition_frames = 24;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        Engine_ColorBufferApplyTarget(0, 0);
        Engine_ColorBufferInterpolate(1);
        Engine_TaskWait(1);
        Engine_EventRequestExit(7);
        Engine_EventEnd();
    }
}

s32 SoruNichigetsu_RestoreEntryState(void)
{
    u32 i;
    s32 record;
    s32 done;
    s32 lit;

    Engine_TaskWait(1);
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x204;
    done = 0;
    if (Engine_GameFlagIsSet(0x809) != 0 && Engine_GameFlagIsSet(0x814) == 0
        && Engine_GameFlagIsSet(0x819) == 0) {
        Engine_GameFlagSet(0x144);
    }
    Engine_ColorBufferApplySource(0x10000, 0);
    if (Engine_GameFlagIsSet(0x109) != 0) {
        if (Engine_GameFlagIsSet(0x201) != 0) {
            Engine_ColorBufferApplyTarget(0x2051cc, 1);
            Engine_ColorBufferInterpolate(1);
            Engine_TaskWait(1);
        } else if (Engine_GameFlagIsSet(0x202) != 0) {
            Engine_ColorBufferApplyTarget(0x202db1, 1);
            Engine_ColorBufferInterpolate(1);
            Engine_TaskWait(1);
        }
    } else {
        Engine_GameFlagSet(0x200);
        if (Engine_GameFlagIsSet(0x80a) != 0) {
            Call3(Engine_ActorSetPosition, 16, 0x2400000, 0x780000);
        }
    }
    if (gCell[225][0] == 4) {
        if (Value1(Engine_GameFlagIsSet, 0x813) == 0) {
            Scene_SpringStatueTrap();
            Engine_GameFlagSet(0x813);
            done = 1;
        }
    } else if (gCell[225][0] == 5) {
        if (Value1(Engine_GameFlagIsSet, 0x812) == 0) {
            FieldScene_RunClosingSequence();
            Engine_GameFlagSet(0x812);
            Engine_GameFlagClear(0x80b);
            Engine_GameFlagClear(0x80c);
            Engine_GameFlagClear(0x80d);
            Engine_GameFlagClear(0x80e);
            done = 1;
        }
    } else if (gCell[225][0] == 6) {
        if (Engine_GameFlagIsSet(0x812) != 0) {
            FieldScene_RunFlaggedSequence();
            Engine_GameFlagSet(0x822);
        }
        done = 1;
    }
    if (Value1(Engine_GameFlagIsSet, 0x80b) != 0) {
        Engine_GameFlagSet(0x826);
    }
    if (Value1(Engine_GameFlagIsSet, 0x80c) != 0) {
        Engine_GameFlagSet(0x827);
    }
    if (Value1(Engine_GameFlagIsSet, 0x80d) != 0) {
        Engine_GameFlagSet(0x828);
    }
    if (Value1(Engine_GameFlagIsSet, 0x80e) != 0) {
        Engine_GameFlagSet(0x829);
    }
    Engine_TaskWait(4);
    if (done != 0) {
    } else {
        if (CheckAllStatueLights() == 0) {
        } else {
            Call6(Engine_MapCopyCellsTo, 30, 44, 30, 38, 12, 5);
            Engine_MapCopyCellsTo(30, 44, 34, 37, 4, 1);
            Engine_MapCopyCellsTo(14, 41, 32, 41, 8, 4);
            Engine_MapCopyCellsTo(45, 28, 34, 10, 4, 2);
            Engine_MapCopyCellsTo(45, 30, 16, 10, 4, 2);
            Engine_MapCopyCellsTo(14, 45, 14, 41, 8, 4);
            if (gCell[225][0] != 8) {
                if (Engine_GameFlagIsSet(0x814) == 0) {
                    if (Engine_GameFlagIsSet(0x819) != 0) {
                        Engine_MapCopyCellsTo(0, 32, 17, 39, 2, 1);
                        Engine_MapCopyCellsTo(42, 62, 17, 36, 2, 3);
                        Engine_MapCopyCellsTo(0, 32, 17, 40, 2, 1);
                        Engine_MapCopyCellsTo(0, 32, 17, 41, 2, 1);
                        Engine_MapCopyCellsTo(0, 32, 17, 42, 2, 3);
                    } else {
                        Engine_MapCopyCellsTo(44, 59, 17, 37, 2, 6);
                    }
                    Map_ClearLayerEntryFlag(9);
                    Map_ClearLayerEntryFlag(10);
                }
            }
            Engine_ActorSetPosition(16, 0, 0);
            goto L_02002510;
        }
        lit = 0;
        if (Engine_GameFlagIsSet(0x80b) != 0) {
            Call6(Engine_MapCopyCellsTo, 45, 28, 34, 10, 2, 1);
            Engine_MapCopyCellsTo(45, 30, 16, 10, 2, 1);
            lit = 1;
        }
        if (Engine_GameFlagIsSet(0x80c) != 0) {
            Call6(Engine_MapCopyCellsTo, 47, 28, 36, 10, 2, 1);
            Engine_MapCopyCellsTo(47, 30, 18, 10, 2, 1);
            lit = 1;
        }
        if (Engine_GameFlagIsSet(0x80d) != 0) {
            Call6(Engine_MapCopyCellsTo, 45, 29, 34, 11, 2, 1);
            Engine_MapCopyCellsTo(45, 31, 16, 11, 2, 1);
            lit = 1;
        }
        if (Engine_GameFlagIsSet(0x80e) != 0) {
            Call6(Engine_MapCopyCellsTo, 47, 29, 36, 11, 2, 1);
            Engine_MapCopyCellsTo(47, 31, 18, 11, 2, 1);
            lit = 1;
        }
        if (Engine_GameFlagIsSet(0x812) == 0) {
            if (lit == 0) {
                goto L_020024fc;
            }
        }
        Engine_MapCopyCellsTo(30, 43, 32, 40, 8, 3);
        Engine_MapCopyCellsTo(30, 43, 33, 39, 8, 1);
        Engine_MapCopyCellsTo(30, 43, 36, 38, 3, 3);
        Engine_MapCopyCellsTo(36, 58, 32, 41, 8, 4);
        L_020024fc:;
        Call6(Engine_MapCopyCellAttributes, 15, 6, 2, 1, 17, 6);
    }
    L_02002510:;
    if (Value1(Engine_GameFlagIsSet, 0x309) == 0) {
        if (gCell[225][0] != 8) {
            goto L_0200254a;
        }
        SoruNichigetsu_RunLightScene();
        Engine_GameFlagSet(0x309);
        Call6(Engine_MapCopyCellAttributes, 15, 6, 2, 1, 17, 6);
    } else {
        L_0200254a:;
        if (Engine_GameFlagIsSet(0x814) != 0) {
            BattleFx_SetQueuedSoundAndPlay(141);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
            InitializeSceneRecordBuffer();
            Call6(Engine_MapCopyCellAttributes, 15, 6, 2, 1, 17, 6);
        }
    }
    return 0;
}

s32 CheckAllStatueLights(void)
{
    s32 all_set = 1;

    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_1) == 0)
        all_set = 0;
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_2) == 0)
        all_set = 0;
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_3) == 0)
        all_set = 0;
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_4) == 0)
        all_set = 0;

    return all_set;
}

void SetSolShindenActorStep(s32 actor_step, s32 wait_frames)
{
    Event_ShowMessage(actor_step, 0);
    Event_Wait(wait_frames);
}

/* Sol Sanctum: the two actors walk up, the light rises, and the scene clears the light tables and backdrop before handing back to the map. */
void SoruNichigetsu_RunLightScene(void)
{
    struct FieldActor *leader;
    u8 *field;
    s32 frames;

    Engine_EventBegin();
    Call4((void (*)())Engine_CameraMoveTo, 0x11e0000, -1, 0x860000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call2((void (*)())Engine_ColorBufferApplySource, 0x7fff, 0);
    Engine_ColorBufferApplyTarget(0x7fff, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    Call3((void (*)())Engine_ActorSetPosition, 1, 0x1180000, 0x860000);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0xa000, 0);
    {
        u8 *ev = (u8 *)Data_03001ebc.event;

        frames = 0x1c8;
        *(s32 *)(ev + frames) = 1;
    }
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_ColorBufferApplyTarget(0x2051cc, 1);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(120);
    Engine_GameFlagSet(0x201);
    Engine_GameFlagClear(0x200);
    Engine_GameFlagClear(0x202);
    Call2((void (*)())Engine_ColorBufferApplyTarget, 0x10000, 2);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(100);
    Call2((void (*)())Engine_ActorSetAttachedEffect, 0, 0x102);
    Engine_ActorSetAttachedEffect(1, 0x102);
    Engine_EventWait(60);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0x2000, 20);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0xe000, 40);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0x9000, 40);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0x5000, 80);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0x8000, 10);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0x1000, 60);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Engine_EventWait(20);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0x6000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0xe000, 20);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventSetMessage((s32)MsgSoruFloatingEyeThing);
    Engine_EventOpenMessage(1, 0);
    Engine_EventChooseYesNo(0, 0);
    Engine_ActorSetAnimation(0, 1);
    Engine_EventWait(40);
    Engine_AudioPlayCue(107);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Engine_EventWait(40);
    InitializeSceneRecordBuffer();
    Object_GetById(0)->unknown_5a &= ~1;
    Object_GetById(1)->unknown_5a &= ~1;
    Engine_ActorJump(0, 4, 0);
    Engine_ActorJump(1, 4, 0);
    Call3((void (*)())Engine_ActorSetDestination, 0, 0x12c, 0x82);
    Engine_ActorSetDestination(1, 0x10a, 0x90);
    Engine_ActorWaitForMove(1);
    Engine_EventWait(40);
    Object_GetById(0)->unknown_5a |= 1;
    Object_GetById(1)->unknown_5a |= 1;
    BattleFx_SetQueuedSoundAndPlay(141);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(1, 2);
    SetSolShindenActorStep(1, 10);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_ActorSetAnimation(1, 2);
    leader = Object_GetById(0);
    if (leader != NULL) {
        Engine_ActorSetDestination(1, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    field = Data_03001ebc.field;
    *(u16 *)(field + 0xe00) = 0;
    *(u16 *)(field + 0xe02) = 0;
    *(u16 *)(field + 0xe04) = 0;
    field[0x2a00] = 0;
    field[0x2a01] = 1;
    field[0x2a02] = 1;
    field[0x2a03] = 1;
    /* FAKEMATCH: the do/while keeps the backdrop-colour clear ahead of the event-work load. */
    do { *(u16 *)0x05000000 = 0; } while (0);
    Data_03001ebc.event->start_transition = 0x204;
    *(s32 *)((u8 *)Data_03001ebc.event + frames) = 16;
    Engine_EventEnd();
}

void Sukureta_Talk(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(FLAG_ROBIN_SEARCHING_FOR_SUKURETA) != 0) {
        Event_SetMessage((s32)MsgSoruSukuretaLetMeKnowWhat);
    } else {
        Event_SetMessage((s32)MsgSoruSukuretaJustWaitOverThere);
    }
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 10);
    Event_End();
}

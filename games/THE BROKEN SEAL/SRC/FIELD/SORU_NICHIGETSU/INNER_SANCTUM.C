#include "SANCTUM.H"
extern u8 MsgSoruWayLeadsOutSanctum[];
extern u8 MsgSoruThank[];
extern u8 MsgSoruFound[];
extern u8 MsgSoruGoBackVillage[];
extern u8 MsgSoruLunaSolRooms[];
extern u8 MsgSoruMeanLookFarther[];
extern u8 MsgSoruPutWayDont[];
extern u8 MsgSoruRoomLunaOne[];
extern u8 MsgSoruWhRoom[];

void FieldScene_RunScene37aSequenceF(void)
{
    u8 *rec;

    if (Value1(Engine_GameFlagIsSet, 0x814) != 0) {
        FieldScene_RunScene37aSequenceA();
    }
    if (Value1(Engine_GameFlagIsSet, 0x809) == 0) {
        Engine_EventBegin();
        Call1(Engine_EventSetMessage, (s32)MsgSoruFound);
        Engine_AudioPlayCue(17);
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Call3(Engine_ActorWalkToAndWait, 0, 0x120, 232);
        Engine_ActorSetAnimation(0, 0);
        Engine_EventWait(20);
        Engine_AudioPlayCue(21);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        rec = (u8 *)Value1(Engine_ActorGet, 0);
        if (rec != 0) {
            Engine_ActorSetPosition(16, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Call3(Engine_ActorSetSpeed, 16, 0x16666, 0xb333);
        Call3(Engine_ActorWalkToAndWait, 16, 0x120, 206);
        Engine_EventWait(40);
        Call3(Engine_ActorShowEmote, 16, 0x100, 0);
        Engine_ActorJump(16, 4, 60);
        SetSolShindenActorStep(16, 20);
        rec = (u8 *)Value1(Engine_ActorGet, 0);
        if (rec != 0) {
            Engine_ActorSetPosition(1, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        rec = (u8 *)Value1(Engine_ActorGet, 0);
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
        Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
        Camera_MoveTo(0x1200000, -1, 0xd50000, 1);
        Call3(Engine_ActorSetSpeed, 16, 0x6666, 0x3333);
        Call3(Engine_ActorWalkToAndWait, 16, 0x120, 176);
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
        Call2(SetSolShindenActorStep, 0x2005, 10);
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
        Value2(Engine_EventOpenMessage, 16, 0);
        if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
            Engine_EventSetMessage((s32)MsgSoruThank);
        } else {
            Call1(Engine_EventSetMessage, (s32)MsgSoruGoBackVillage);
            Call3(Engine_ActorShowEmote, 16, 0x107, 20);
        }
        Engine_ActorJump(16, 4, 20);
        Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
        Call3(Engine_ActorFaceDirection, 5, 0xa000, 0);
        SetSolShindenActorStep(16, 6);
        Call1(Engine_EventSetMessage, (s32)MsgSoruPutWayDont);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(5, 4);
        Call2(SetSolShindenActorStep, 0x2005, 6);
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
        rec = (u8 *)Value1(Engine_ActorGet, 0);
        if (rec != 0) {
            Actor_SetDestination(ACTOR_SUKURETA, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Engine_ActorWaitForMove(16);
        Engine_ActorSetPosition(16, 0, 0);
        Engine_ActorSetAnimation(1, 2);
        rec = (u8 *)Value1(Engine_ActorGet, 0);
        if (rec != 0) {
            Actor_SetDestination(ACTOR_GERALD, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Engine_ActorWaitForMove(1);
        Engine_ActorSetPosition(1, 0, 0);
        Engine_ActorSetAnimation(5, 2);
        rec = (u8 *)Value1(Engine_ActorGet, 0);
        if (rec != 0) {
            Actor_SetDestination(ACTOR_JASMINE, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Engine_ActorWaitForMove(5);
        Engine_ActorSetPosition(5, 0, 0);
        Call1(Engine_GameFlagSet, 0x144);
        Call1(Engine_GameFlagSet, 0x809);
        Engine_EventEnd();
    }
}

void Scene_EnterInnerSanctum(void)
{
    u32 i;
    s32 record;
    s32 request;
    s32 base5_4010;
    s32 base5_4010_2;

    Event_SetMessage((s32)MsgSoruWhRoom);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1e8, 176);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    record = Value1(Engine_ActorGet, 0);
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
    Call2(SetSolShindenActorStep, 0x1010, 80);
    Camera_MoveTo(0x1ec0000, -1, 0xa80000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    base5_4010 = 0x4010;
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 20);
    SetSolShindenActorStep(base5_4010, 6);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 60);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Event_OpenMessage(base5_4010, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage((s32)MsgSoruLunaSolRooms);
    } else {
        Event_SetMessage((s32)MsgSoruRoomLunaOne);
    }
    base5_4010_2 = 0x4010;
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 10);
    SetSolShindenActorStep(base5_4010_2, 10);
    request = (s32)MsgSoruMeanLookFarther;
    Event_SetMessage(request);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 40);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x105, 40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Actor_SetAnimation(ACTOR_SUKURETA, 4);
    Event_OpenMessage(base5_4010_2, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage((request + 1));
        GameFlag_Set(FLAG_ROBIN_SEARCHING_FOR_SUKURETA);
    } else {
        Event_SetMessage((request + 2));
    }
    Call2(SetSolShindenActorStep, 0x4010, 4);
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
    s32 g1 = 0x810;
    s32 g2 = 0x810;
    s32 g3 = 0x810;
    s32 g4 = 0x810;
    s32 s1 = 0x10000;
    s32 s2 = 0x8000;
    s32 s3 = 0x20000;
    s32 s4 = 0x10000;
    s32 s5 = 0x4000;
    s32 d1 = 0x120;
    s32 d2 = 0x120;
    s32 d3 = 0x120;
    s32 d4 = 0x120;
    s32 d5 = 0xc000;

    scene_actor = Engine_ActorGet(16);
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
    if (GameFlag_IsSet(g1)!= 0 || GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
        target_actor = Engine_ActorGet(0);
        if (target_actor != 0) {
            Actor_SetPosition(ACTOR_SUKURETA, target_actor->unk8, target_actor->unk10);
        }
        Event_Wait(4);
        Actor_SetSpeed(ACTOR_SUKURETA, s1, s2);
    } else if (GameFlag_IsSet(g2) != 0 || scene_actor->unk8 > 0x1540000) {
        Actor_SetPosition(ACTOR_SUKURETA, 0x1880000, 0xa80000);
        Event_Wait(4);
        Actor_SetSpeed(ACTOR_SUKURETA, s3, s4);
    }
    if (GameFlag_IsSet(g3) != 0 || scene_actor->unk8 > 0x1540000) {
        Actor_WalkToAndWait(ACTOR_SUKURETA, d1, 0xe8);
    } else {
        GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED);
    }
    Actor_WalkToAndWait(ACTOR_SUKURETA, d2, 0xe8);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, d5, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, s5, 10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    if (GameFlag_IsSet(g4)!= 0 || GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
        Actor_SetAnimation(ACTOR_SUKURETA, 2);
        target_position = Engine_ActorGet(0);
        if (target_position != 0) {
            Actor_SetDestination(ACTOR_SUKURETA, target_position->unkA, target_position->unk12);
        }
        Actor_WaitForMove(ACTOR_SUKURETA);
        Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, d3, 0xe8);
    } else {
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, d4, 0xf8);
    }
    Event_End();
}

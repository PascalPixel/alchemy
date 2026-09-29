#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"
extern u8 MsgShianDidDoWarrior[];
extern u8 MsgShianImTravelingAroundWorldSpread[];
extern u8 MsgShianItIsLocked[];
extern u8 MsgShianRobinAmCountingOnBring[];
extern u8 MsgShianWarriorWillShowMeYour[];
extern u8 MsgShianWaterMonstersFloodedAltinDid[];
extern u8 MsgShianYaTreeFell[];

void FieldScene_DispatchApproachByFacing(void)
{
    Event_Begin();

    if (*(u16 *)(((u8 *)Engine_ActorGet(0)) + 6) > (128 << 7)
        && *(u16 *)(((u8 *)Engine_ActorGet(0)) + 6) < (192 << 8)) {
        FieldScene_RunForwardArcBurst();
    } else {
        ShianJiin_SpinAway();
    }

    if (GameFlag_IsSet(0x898) != 0) {
        ShianJiin_RunGatheringScene();
    } else {
        ShianJiin_RunMasterScene(0);
    }

    Event_End();
}

void FieldScene_DispatchByFacing(void)
{
    struct SceneActor_02001334 *record = Actor_Get(ACTOR_PARTY_LEADER);
    u16 angle;

    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 8);
    Event_Wait(20);

    angle = *(u16 *)((u8 *)record + 6);

    if ((u16)(angle - 0x2000) <= 0x3fffu) {
        Scene_RunPrimarySequence();
    } else if ((u16)(angle - 0x6000) <= 0x3fffu) {
        FieldScene_RunForwardArcBurst();
    } else if ((u16)(angle + (192 << 7)) <= 0x3fffu) {
        FieldScene_RunDescentBurst();
    } else {
        ShianJiin_SpinAway();
    }

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    ShianJiin_RunMasterScene(1);
    Event_End();
}

void FieldScene_DispatchByFacingAndFlags(void)
{
    u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
    u16 facing;

    Event_Begin();

    facing = *(u16 *)(record + 6);
    if ((u16)(facing - 0x2000) <= 0x3fff) {
        Scene_RunPrimarySequence();
    } else if ((u16)(facing - 0x6000) <= 0x3fff) {
        FieldScene_RunForwardArcBurst();
    } else if ((u16)(facing + 0x6000) <= 0x3fff) {
        FieldScene_RunDescentBurst();
    } else {
        ShianJiin_SpinAway();
    }

    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveToActor(20, 1);
    Camera_WaitForMove();

    if (*(s16 *)(record + 18) <= 209) {
        if (GameFlag_IsSet(0x89a) == 0) goto scene0;
        if (GameFlag_IsSet(0x89b) != 0) goto scene0;
        goto scene1;
scene0:
        ShianJiin_RunMasterScene(0);
        goto firstSceneComplete;
scene1:
        ShianJiin_RunGatheringScene();
firstSceneComplete:
        Event_End();
        return;
    }

    if (GameFlag_IsSet(0x89b) != 0) {
        ShianJiin_RunMasterScene(2);
    } else if (GameFlag_IsSet(0x89a) == 0) {
        FieldScene_RunSecondEnsembleBeat();
    } else {
        FieldScene_RunEnsembleStoryBeat();
    }
    Event_End();
}

void FieldScene_RunSecondEnsembleBeat(void)
{
    s32 id;
    u8 *rec;

    GameFlag_Set(0x89a);
    Event_Wait(30);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(15, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(16, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_ShowEmote(13, 128 << 1, 0);
    Actor_ShowEmote(15, 128 << 1, 0);
    Actor_ShowEmote(16, 128 << 1, 0);
    Event_Wait(60);
    Event_SetMessage((s32)MsgShianDidDoWarrior);
    Event_ShowMessageAndWait(13, 0, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_RunRepeatedMotion(15, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 15, 0);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 16, 0);
    Event_AskYesNo(16, 0);
    Event_Wait(50);
    Camera_FollowActor(16, 1);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkToAndWait(16, 176, 248);
    Actor_WalkToAndWait(16, 154 << 1, 248);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 128 << 6, 0);
    Actor_FaceDirection(16, 192 << 8, 20);
    Audio_PlayCue(158);
    Map_AnimateCells(0x0200c77a, 78, 13);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Actor_SetSpeed(16, 192 << 9, 192 << 8);
    rec = Record1(Engine_ActorGet, 16);
    rec[90] &= 0xfe;
    Actor_WalkToAndWait(16, 154 << 1, 136 << 1);
    Event_Wait(1);
    rec = Record1(Engine_ActorGet, 16);
    {
        /*
         * A result temporary, not the compound or-assign the matching
         * &= 0xfe case above uses. The reference writes the result into
         * the mask register rather than the loaded value, and the
         * two-address ORR only does that when the merged result is its
         * own object; the compound form keeps the loaded value as
         * destination. Same technique already adopted in the sibling
         * owner resource_3bd:020013f8.
         */
        u8 merged = (u8)(rec[90] | 1);

        rec[90] = merged;
    }
    Event_ShowMessageAndWait(16, 0, 50);
    Actor_SetPosition(17, 152 << 17, 216 << 16);
    Actor_WalkToAndWait(17, 152 << 1, 248);
    Actor_FaceActor(9, 17, 0);
    Actor_FaceActor(10, 17, 0);
    Actor_FaceActor(11, 17, 0);
    Actor_FaceActor(12, 17, 0);
    Actor_FaceActor(13, 17, 0);
    Actor_FaceActor(14, 17, 0);
    Actor_FaceActor(15, 17, 0);
    Actor_FaceActor(16, 17, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Actor_StartRepeatedMotion(13, 2);
    Actor_StartRepeatedMotion(14, 2);
    Actor_StartRepeatedMotion(15, 2);
    Actor_RunRepeatedMotion(16, 2);
    Actor_ShowEmote(17, 0x103, 60);
    Actor_SetPosition(18, 152 << 17, 216 << 16);
    Actor_WalkTo(18, 152 << 1, 248);
    Actor_WalkTo(17, 140 << 1, 132 << 1);
    Actor_WaitForMove(18);
    Actor_FaceDirection(18, 160 << 7, 0);
    Actor_WaitForMove(17);
    Audio_PlayCue(159);
    Map_AnimateCells(0x0200c790, 78, 13);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceActor(9, 17, 0);
    Actor_FaceActor(10, 17, 0);
    Actor_FaceActor(11, 17, 0);
    Actor_FaceActor(12, 17, 0);
    Actor_FaceActor(13, 17, 0);
    Actor_FaceActor(14, 17, 0);
    Actor_FaceActor(15, 17, 0);
    Actor_FaceActor(16, 17, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 17, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(17, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(18, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceDirection(17, 208 << 8, 20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_ShowEmote(18, 0x102, 60);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_RunRepeatedMotion(17, 2);
    Event_Wait(20);
    Actor_FaceDirection(17, 0, 20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_FaceDirection(16, 128 << 8, 20);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(17, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_FaceDirection(17, 128 << 8, 20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(17, 208 << 8, 20);
    Actor_RunRepeatedMotion(17, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_ShowEmote(18, 0x102, 60);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(17, 0x101, 60);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(17, 0x100, 60);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Actor_ShowEmote(17, 0x103, 60);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_ShowEmote(18, 0x100, 60);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(17, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_RunRepeatedMotion(17, 2);
    Event_Wait(10);
    Actor_WalkToAndWait(17, 128 << 1, 140 << 1);
    Actor_FaceDirection(17, 128 << 7, 20);
    Actor_SetPosition(17, 0, 0);
    Actor_Destroy(17);
    Event_Wait(30);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_RunRepeatedMotion(15, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceActor(16, 18, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_FaceActor(18, 16, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_WalkToAndWait(18, 128 << 1, 248);
    Actor_FaceDirection(18, 192 << 8, 20);
    Actor_StartRepeatedMotion(18, 1);
    Actor_ShowEmote(18, 0x100, 60);
    Actor_WalkToAndWait(18, 240, 184);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_SetPosition(19, 232 << 16, 168 << 16);
    Actor_SetPosition(20, 232 << 16, 168 << 16);
    rec = Record1(Engine_ActorGet, 19);
    *(s32 *)(rec + 12) = 0xc0000;
    rec = Record1(Engine_ActorGet, 19);
    *(s32 *)(rec + 60) = -0x80000000;
    rec = Record1(Engine_ActorGet, 19);
    *(s32 *)(rec + 24) = 0xcccc;
    rec = Record1(Engine_ActorGet, 19);
    {
        u8 *target = *(u8 **)(rec + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Audio_PlayCue(124);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 192 << 8, 20);
    Actor_WalkToAndWait(16, 128 << 1, 240);
    Actor_FaceDirection(16, 176 << 8, 20);
    Actor_RunRepeatedMotion(16, 1);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(15, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(16, ACTOR_PARTY_LEADER, 0);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_FaceDirection(18, 160 << 7, 20);
    Actor_WalkToAndWait(18, 248, 208);
    Actor_FaceDirection(18, 160 << 7, 20);
    Event_OpenMessage(18, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Value2((s32 (*)())Engine_ActorRunRepeatedMotion, 16, 1);
        Event_Wait(20);
        id = 16;
        goto joinBeat;
    }

    /* Skipped once: bump the workspace skip counter and offer the beat again. */
    Scene_BumpStep(1);
    Event_Wait(20);
    Actor_ShowEmote(18, 0x105, 60);
    Actor_FaceDirection(18, 128 << 7, 20);
    Actor_RunRepeatedMotion(16, 2);
    Event_Wait(20);
    Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        goto skipTwice;
    }
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_FaceDirection(18, 176 << 8, 20);
    id = 18;

joinBeat:
    Event_ShowMessageAndWait(id, 0, 20);
    GameFlag_Set(0x898);
    goto finish;

skipTwice:
    Scene_BumpStep(1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    GameFlag_Set(0x899);

finish:
    Actor_FaceDirection(10, 128 << 8, 0);
    Actor_FaceDirection(11, 128 << 8, 20);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Engine_ActorEnableActionCallback(12, 0x0200c638);
}

void FieldScene_RunSkippableStoryBeat(void)
{
    extern u8 *gWork;

    u8 *workspace;

    Event_Begin();
    Event_SetMessage((s32)MsgShianWarriorWillShowMeYour);
    Event_OpenMessage(18, 0);

    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_ShowMessageAndWait(18, 0, 20);
        GameFlag_Set(0x898);
        Event_End();
    } else {
        workspace = gWork;
        *(u16 *)(workspace + 472) += 1;
        Event_ShowMessageAndWait(18, 0, 20);
        Event_End();
    }
}

/* Looks up actors 18, 13, 14, 15 and 16 and clears their +108 field before
 * the scene runs. */
void FieldScene_RunEnsembleStoryBeat(void)
{
    u32 i;
    s32 actor;

    actor = Actor_Get(18);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_2(13);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_3(14);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_4(15);
    ACTOR_FIELD_108(actor) = 0;
    actor = Actor_Get(16);
    ACTOR_FIELD_108(actor) = 0;
    Object_SetModeById_1(11, 1);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0xe80000, -1, 0xc80000, 1);
    Camera_WaitForMove();
    Event_SetMessage((s32)MsgShianYaTreeFell);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_SetSpeed(12, 0xcccc, 0x6666);
    Actor_WalkTo(10, 152, 200);
    Actor_WalkToAndWait(12, 144, 248);
    Actor_WaitForMove(10);
    Actor_FaceActor(9, 19, 0);
    Actor_FaceActor(11, 19, 0);
    Actor_FaceActor(13, 19, 0);
    Actor_FaceActor(14, 19, 0);
    Actor_FaceActor(15, 19, 0);
    Actor_FaceActor(16, 19, 0);
    Actor_FaceActor(18, 19, 0);
    Actor_SetSpeed(10, 0x18000, 0xc000);
    Actor_SetSpeed(12, 0x20000, 0x10000);
    Actor_WalkTo(10, 152, 200);
    Actor_WalkToAndWait(12, 144, 248);
    Actor_FaceActor(12, 19, 0);
    Actor_WaitForMove(10);
    Actor_FaceActor(10, 19, 0);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 40);
    Actor_FaceActor(9, 18, 0);
    Actor_FaceActor(10, 18, 0);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceActor(12, 18, 0);
    Actor_FaceDirection(13, 0x3000, 0);
    Actor_FaceActor(14, 18, 0);
    Actor_FaceActor(15, 18, 0);
    Actor_FaceActor(16, 18, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_ShowEmote(18, 0x105, 60);
    Actor_ShowEmote(16, 0x101, 60);
    Event_ShowMessageAndWait(16, 0, 20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(16, 0x102, 60);
    Actor_ShowEmote(15, 0x101, 60);
    Actor_SetSpeed(15, 0xcccc, 0x6666);
    Actor_WalkToAndWait(15, 216, 176);
    Actor_FaceDirection(15, 0x3000, 20);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceDirection(18, 0xb000, 20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Actor_StartRepeatedMotion(13, 2);
    Actor_StartRepeatedMotion(14, 2);
    Actor_StartRepeatedMotion(15, 2);
    Actor_StartRepeatedMotion(16, 2);
    Event_Wait(40);
    Actor_RunRepeatedMotion(13, 2);
    Event_ShowMessageAndWait(13, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
    Actor_FaceDirection(18, 0x5000, 20);
    Event_AskYesNo(18, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(10, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(11, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(12, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(13, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(14, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(15, 0x101, 0);
    Event_Wait(5);
    Actor_ShowEmote(16, 0x101, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(16, 2);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_ShowEmote(15, 0x101, 60);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_FaceActor(18, 15, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 40);
    Actor_FaceEachOther(11, 10, 0);
    Actor_FaceEachOther(12, 14, 0);
    Actor_FaceEachOther(13, 15, 0);
    Event_Wait(60);
    Actor_FaceActor(10, 18, 0);
    Actor_FaceActor(11, 18, 0);
    Actor_FaceActor(12, 18, 0);
    Actor_FaceActor(13, 18, 0);
    Actor_FaceActor(14, 18, 0);
    Actor_FaceActor(15, 18, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(13, 3);
    Actor_SetAnimation(14, 3);
    Actor_SetAnimation(15, 3);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_FaceDirection(18, 0x5000, 20);
    Event_AskYesNo(18, 0);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(13, 3);
    Actor_SetAnimation(14, 3);
    Actor_SetAnimation(15, 3);
    ObjectMotion_CallThenWaitForAnimationChange_9(16, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(18, 0, 20);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(13, 3);
    Actor_SetAnimation(14, 3);
    Actor_SetAnimation(15, 3);
    Actor_SetAnimationAndWait(16, 3);
    Event_Wait(20);
    Actor_WalkTo(10, 120, 200);
    Actor_WalkTo(12, 120, 248);
    Actor_WaitForMove(10);
    Actor_FaceDirection(11, 0x8000, 20);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Actor_WaitForMove(12);
    ObjectMotion_EnableActionAndSetCallback_1(12, 0x200c638);
    Actor_SetSpeed(15, 0xcccc, 0x6666);
    Actor_WalkToAndWait(15, 216, 168);
    Actor_WalkToAndWait(15, 232, 168);
    Actor_FaceDirection(15, 0xc000, 20);
    Actor_RunRepeatedMotion(15, 3);
    Actor_SetPosition(19, 0xe80000, 0xa80000);
    actor = Actor_Get(19);
    *(s32 *)(actor + 12) = 0xc0000;
    actor = Actor_Get(19);
    *(s32 *)(actor + 60) = -0x80000000;
    actor = Actor_Get(19);
    {
        s32 target = *(s32 *)(actor + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Audio_PlayCue(124);
    Event_Wait(40);
    Actor_WalkToAndWait(15, 216, 152);
    Actor_FaceDirection(15, 0x4000, 30);
    GameFlag_Clear(0x898);
    GameFlag_Set(0x89b);
}

void FieldScene_ShowDialogue1A58(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgShianWaterMonstersFloodedAltinDid);
    Event_AskYesNo(11, 0);
    Event_End();
}

void StartSchoolDoorEvent(void)
{
    Event_Begin();
    if (GameFlag_IsSet(2202) == 0 && GameFlag_IsSet(2197) == 0) {
        Message_ShowCentered((s32)MsgShianItIsLocked, 1);
        Event_End();
    } else {
        Audio_PlayCue(158);
        Map_AnimateCells(0x0200c77a, 78, 13);
        SetScale(0, 0x8000, 0x4000);
        SetPosition(0, 306, 248);
        Actor_WalkTo(ACTOR_PARTY_LEADER, 304, 216);
        Event_Wait(20);
        Event_RequestExit(4);
        Event_End();
    }
}

void FieldScene_DispatchByRange(void)
{
    u8 *record;
    u32 biased;

    record = Actor_Get(ACTOR_PARTY_LEADER);
    biased = *(u16 *)(record + 6);
    Event_Begin();

    biased = biased + 0xffff5fff;
    if (biased <= 0x3ffe) {
        Sanctum_Open(13);
    } else {
        Event_SetMessage((s32)MsgShianImTravelingAroundWorldSpread);
        Event_ShowMessage(13, 0);
    }

    Event_End();
}

void FieldScene_ShowDialogue17DF(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(8, 2);
    Event_SetMessage((s32)MsgShianRobinAmCountingOnBring);
    Event_ShowMessage(8, 0);
    Event_End();
}

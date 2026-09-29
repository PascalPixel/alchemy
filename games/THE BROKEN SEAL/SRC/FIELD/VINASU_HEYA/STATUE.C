#include "ENTRY_SETUP.H"
extern u8 MsgFieldDoorTightlyLocked[];
extern u8 MsgFieldVenusLighthouseWasAttackedBy[];
extern u8 MsgVinasuHmmmWeCantPushBlock[];
extern u8 MsgVinasuIveWaitedLongSeeIts[];
extern u8 MsgVinasuStatueSpeaksRobinSoulYe[];
extern u8 MsgVinasuThereWordsCarvedIntoRelief[];

void SceneState_SetFlag953(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldDoorTightlyLocked, 1);
    Event_End();
}

void FieldScene_RunActorEightTenStepLoop(void)
{
    u32 n;
    u32 w;
    s32 a;
    s32 b;

    Event_Begin();
    Actor_RunRepeatedMotion(8, 3);
    Event_SetMessage((s32)MsgFieldVenusLighthouseWasAttackedBy);
    n = 10;
    w = 8;
    Event_ShowMessageAndWait(8, 0, 20);
    do {
        Actor_SetChildValue(8, 15);
        Task_Wait(2);
        Actor_SetChildValue(8, 0);
        Task_Wait(w);
        if (w > 3) {
            w--;
        }
        n--;
    } while (n != 0);
    GameFlag_Set(0x981);
    Actor_SetPosition(8, 0, 0);
    a = 7;
    b = 16;
    Map_CopyCellAttributes(7, 17, 2, 1, a, b);
    Event_End();
}

void SceneDialogue_RunActorElevenDialogue(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgVinasuIveWaitedLongSeeIts);
    Event_ShowMessageAndWait(11, 0, 20);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessage(11, 0);
    Event_End();
}

void FieldScene_SetFlag987AtActorTwelveTile(void)
{
    Struct_0ff0 *s;

    s = Actor_Get(12);
    Event_Begin();
    if (s->unk8 >> 20 == 54 || s->unk10 >> 20 == 6) {
        GameFlag_Set(0x987);
    }
    Event_End();
}

void SceneDialogue_RunLine2682(void)
{
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered((s32)MsgVinasuThereWordsCarvedIntoRelief, 1);
    Event_End();
}

void SceneState_ApplySixRectsAfter161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    GameFlag_Clear(0x161);
    x = 23;
    y = 8;
    Map_CopyCellAttributes(35, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Map_CopyCellsTo(35, 8, 23, 8, b, a);
    Map_CopyCellsTo(99, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Map_CopyCellAttributes(57, 55, 3, 3, x, y);
    Map_CopyCellsTo(57, 55, 46, 55, a, a);
    Map_CopyCellsTo(121, 55, 110, 55, a, a);
}

void SceneState_ApplySixRectsAfterFlag161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    GameFlag_Set(0x161);
    x = 23;
    y = 8;
    Map_CopyCellAttributes(36, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Map_CopyCellsTo(36, 8, 23, 8, b, a);
    Map_CopyCellsTo(100, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Map_CopyCellAttributes(53, 55, 3, 3, x, y);
    Map_CopyCellsTo(53, 55, 46, 55, a, a);
    Map_CopyCellsTo(117, 55, 110, 55, a, a);
}

void FieldScene_RunLeaderSurpriseApproach(void)
{
    struct FieldActor *actor;
    s32 z;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 0x200d21c);
    Engine_ActorStartAction(0);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 6);
    actor->velocity_y = 0x40000;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
    if (actor->z.fixed >> 20 <= 54) {
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= 254;
        z = 210;
    } else {
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= 254;
        z = 238;
    }
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, actor->x.part.pixel, z << 2);
    Event_Wait(1);
    SetFlagBits(&Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a, 1);
    Event_Wait(20);
    actor->update = (void (*)(union FieldObject *))0x20085e5;
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    actor->update = 0;
    Event_End();
}

void FieldScene_RunStatueDialogueSequence(void)
{
    extern struct EventWorkState *gWork;
    struct EventWorkState *work;

    work = gWork;
    work->field_cba = 0;
    work->field_cb6 = 1;
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered((s32)MsgVinasuStatueSpeaksRobinSoulYe, 1);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x10005, 0);
    ColorBuffer_Interpolate(120);
    Event_Wait(100);
    Audio_PlayCue(142);
    Event_Wait(30);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(60);
    Event_Wait(70);
    if (GameFlag_IsSet(0x982) == 0) {
        if (GameFlag_IsSet(0x983) == 0) {
            if ((gFrameCount & 1) != 0) {
                GameFlag_Set(0x982);
            } else {
                GameFlag_Set(0x983);
            }
        }
    }
    if (GameFlag_IsSet(0x982) == 0) {
        GameFlag_Set(0x982);
        GameFlag_Clear(0x983);
        Map_CopyCellsTo(103, 27, 89, 27, 7, 8);
        Map_CopyCellsTo(41, 90, 27, 92, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 93, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 94, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 91, 3, 2);
        Map_CopyCellsTo(41, 92, 25, 93, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 95, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 97, 3, 2);
        Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
    } else {
        GameFlag_Set(0x983);
        GameFlag_Clear(0x982);
        Map_CopyCellsTo(111, 27, 89, 27, 7, 8);
        Map_CopyCellsTo(41, 90, 25, 91, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 93, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 95, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 97, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
        Map_CopyCellsTo(41, 94, 27, 92, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 93, 3, 2);
        Map_CopyCellsTo(41, 94, 27, 94, 3, 2);
        Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
    }
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(20);
    Event_Wait(40);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0x1c80000, -1, 0x21e0000, 1);
    Camera_WaitForMove();
    Event_Wait(50);
    Camera_MoveTo(0x1c80000, -1, 0x1a70000, 1);
    Camera_WaitForMove();
    Event_End();
    work->field_cb6 = 0;
}

void FieldScene_RunFlag986ActorOneScene(void)
{
    Struct_A *o;
    Struct_B *u;
    s32 g;
    s32 m1;
    s32 m2;
    s32 h;
    s32 k;

    g = 0x986;
    m1 = 0xcccc;
    m2 = 0x6666;
    h = 0x100;
    k = 0x338;
    Event_Begin();
    o = Actor_Get(12);
    if (o->unk8 >> 20 == 53) {
        if (GameFlag_IsSet(g) == 0) {
            GameFlag_Set(g);
            o = Actor_Get(ACTOR_PARTY_LEADER);
            if (o != 0) {
                Actor_SetPosition(ACTOR_GERALD, o->unk8, o->unk10);
            }
            Actor_SetSpeed(ACTOR_GERALD, m1, m2);
            Actor_WalkToAndWait(ACTOR_GERALD, k, 88);
            Actor_WalkToAndWait(ACTOR_GERALD, k, 104);
            Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
            Event_Wait(20);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Event_Wait(20);
            Event_SetMessage((s32)MsgVinasuHmmmWeCantPushBlock);
            Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
            Actor_FaceDirection(ACTOR_GERALD, 0, 10);
            Actor_ShowEmote(ACTOR_GERALD, h, 60);
            Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
            Event_Wait(20);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
            Event_Wait(20);
            Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
            Event_Wait(30);
            Actor_WalkToAndWait(ACTOR_GERALD, k, 88);
            Actor_SetAnimation(ACTOR_GERALD, 2);
            u = Actor_Get(ACTOR_PARTY_LEADER);
            if (u != 0) {
                Actor_SetDestination(ACTOR_GERALD, u->unkA, u->unk12);
            }
            Actor_WaitForMove(ACTOR_GERALD);
            Actor_SetPosition(ACTOR_GERALD, 0, 0);
            Event_End();
        }
    }
}

void FieldScene_RunFiveCallSequence(void)
{

    Event_Begin();
    RunStagedActorTransition();
    Event_Wait(20);
    Event_End();
    FieldScene_RunFlag986ActorOneScene();
}

void SceneState_RunActor13AtColumn42Setup(void)
{
    Struct_1644 *obj;
    s32 val;
    s32 a;
    s32 b;

    obj = Actor_Get(13);
    Event_Begin();
    if (obj->unk8 >> 20 == 42) {
        Event_Wait(30);
        Audio_PlayCue(188);
        obj->unk55 = 0;
        val = 0xfffe0000;
        obj->unk14 = val;
        obj->unkC = val;
        GameFlag_Set(0x200);
        a = 3;
        b = 5;
        Map_CopyCellsTo(44, 117, 41, 117, a, b);
    }
    Event_End();
}

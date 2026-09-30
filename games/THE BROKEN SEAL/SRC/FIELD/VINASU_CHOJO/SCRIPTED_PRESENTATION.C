/* Bracketed scenes, the pair's defeat and the multi-actor presentation. */
#include "CHOJO.H"
extern u8 MsgVinasuPairDefeated[];

void FieldScene_RunThreeStepsInBracket(void)
{
    Event_Begin();
    FieldScene_RunPairDefeat();
    FieldScene_RunMultiActorPresentation();
    VinasuChojo_RunActorTransition();
    Event_End();
}

/*
 * Brackets a scripted scene: opens it, runs two of this overlay's own
 * steps, sets the story flag, writes the workspace phase and timer, then
 * closes. The 72-byte owner includes its alignment halfword and one pool
 * word. No incoming argument is read before being overwritten, so this is
 * void.
 */
void FieldScene_RunBracketedSceneWithFlag282(void)
{
    extern u8 *Data_03001ebc;

    u8 *workspace;

    Event_Begin();
    FieldScene_RestageParty();
    FieldScene_RunScene3c9_02004b28();
    /* The flag id is built as 141 << 1 rather than folded. */
    GameFlag_Set(141 << 1);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 512;
    *(s32 *)(workspace + 456) = 24;

    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(1);
    Event_End();
}

void FieldScene_RunScene3c9_02003924(void)
{
    u32 i;
    s32 rec4;
    s32 record;

    rec4 = Engine_ActorGet(ACTOR_PARTY_LEADER);
    Event_Begin();
    *((u8 *)Engine_EventGetViewCenter() + 85) = 0;
    Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
    Map_CopyCellsTo(39, 72, 11, 72, 16, 20);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
    Task_Wait(1);
    Camera_MoveTo(0xc00000, -0x400000, 0xee0000, 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    Actor_Destroy(20);
    Actor_Destroy(19);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 19);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, 0);
    *(s32 *)(rec4 + 8) = 0x15a0000;
    *(s32 *)(rec4 + 16) = 0xcd0000;
    *(s32 *)(rec4 + 12) = 0x200000;
    {
        s32 shown = 0x6000;

        *(u16 *)(rec4 + 6) = shown;
    }
    SceneActor_ParkRecord(rec4);
    Actor_SetAnimation(ACTOR_GERALD, 18);
    record = Actor_Get(ACTOR_GERALD);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(ACTOR_GERALD);
    *(s32 *)(record + 8) = 0x1640000;
    *(s32 *)(record + 16) = 0xc00000;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    SceneActor_ParkRecord((u8 *)record);
    Actor_SetAnimation(ACTOR_IVAN, 18);
    record = Actor_Get(ACTOR_IVAN);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(ACTOR_IVAN);
    *(s32 *)(record + 8) = 0x1680000;
    {
        s32 shown = 0x2000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    SceneActor_ParkRecord((u8 *)record);
    Actor_SetAnimation(ACTOR_MIA, 18);
    record = Actor_Get(ACTOR_MIA);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(ACTOR_MIA);
    *(s32 *)(record + 8) = 0x14e0000;
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    SceneActor_ParkRecord((u8 *)record);
    Actor_SetPosition(21, 0xc40000, 0xdc0000);
    Actor_SetAnimation(21, 5);
    Actor_SetPosition(6, 0xbc0000, 0x13c0000);
    ((void (*)())Engine_ActorSetAnimation)(6, 5);
    record = Actor_Get(6);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(8);
    *(s32 *)(record + 8) += -0x100000;
    SceneActor_ParkRecord((u8 *)record);
    record = Engine_ActorGet(9);
    *(s32 *)(record + 8) += -0x100000;
    SceneActor_ParkRecord((u8 *)record);
    record = Actor_Get(10);
    *(s32 *)(record + 8) += 0x100000;
    SceneActor_ParkRecord((u8 *)record);
    record = Engine_ActorGet(11);
    *(s32 *)(record + 8) += 0x100000;
    SceneActor_ParkRecord((u8 *)record);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    record = Actor_Get(23);
    Actor_SetSpriteFlags(record, 0);
    *(u8 *)((u8 *)Engine_ActorGet(23) + 85) = 4;
    Actor_SetChildValue(23, 4);
    record = Actor_Get(23);
    *(s32 *)(record + 12) = 0x280000;
    ((void (*)())Engine_TaskAddCallback)((s32)SceneEffect_SpawnParticlesBesideActor, 0xc80);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transition_frames = 24;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Scene_RunExtendedActorTransition();
    GameFlag_Set(0x9a7);
    Event_RequestExit(2);
}

void SceneEffect_SpawnParticlesAboveActor(void);

/*
 * The pair's defeat. Actors 0 to 3 are placed facing northwest, actors 21
 * and 6 between north and northeast, and the pair between south and
 * southeast. Actors 24 and 25 are prepared, the particle task starts and the
 * screen opens. The pair speak, then each falls away in three poses and is
 * removed.
 */
void FieldScene_RunPairDefeat(void)
{
    struct FieldActor *actor;

    Audio_PlayCue(141);
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTHWEST;
    Actor_Get(ACTOR_GERALD)->facing = FACING_NORTHWEST;
    Actor_SetPosition(ACTOR_GERALD, PIXELS(328), PIXELS(168));
    Actor_Get(ACTOR_IVAN)->facing = FACING_NORTHWEST;
    Actor_SetPosition(ACTOR_IVAN, PIXELS(340), PIXELS(196));
    Actor_Get(ACTOR_MIA)->facing = FACING_NORTHWEST;
    Actor_SetPosition(ACTOR_MIA, PIXELS(326), PIXELS(204));
    Actor_Get(21)->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(21, PIXELS(200), PIXELS(216));
    Actor_Get(6)->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(6, PIXELS(200), PIXELS(216));
    Actor_Get(ACTOR_FIRST_OF_PAIR)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_SetPosition(ACTOR_FIRST_OF_PAIR, PIXELS(310), PIXELS(158));
    Actor_Get(ACTOR_SECOND_OF_PAIR)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_SetPosition(ACTOR_SECOND_OF_PAIR, PIXELS(292), PIXELS(158));

    Actor_SetSpriteFlags(Actor_Get(24), 0);
    Actor_SetChildValue(24, 7);
    Actor_SetSpritePriority(24, 1);
    actor = Actor_Get(24);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x3333;
    actor->motion_flags = 0;
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(2);
    actor->z.fixed = PIXELS(96);

    Actor_SetSpriteFlags(Actor_Get(25), 0);
    Actor_SetChildValue(25, 7);
    Actor_SetSpritePriority(25, 1);
    actor = Actor_Get(25);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x3333;
    actor->motion_flags = 0;
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(34);
    actor->z.fixed = PIXELS(96);

    Task_AddCallback(SceneEffect_SpawnParticlesAboveActor, TASK_PRIORITY_SCENE);
    Event_GetViewCenter()->motion_flags = 0;
    Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
    Event_Wait(10);
    Event_SetMessage((s32)MsgVinasuPairDefeated);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_SECOND_OF_PAIR, 0, 40);
    Audio_PlayCue(17);

    actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
    actor->x.fixed = PIXELS(308);
    actor->y.fixed = PIXELS(28);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 10);
    Event_Wait(20);
    actor->x.fixed = PIXELS(306);
    actor->y.fixed = PIXELS(28);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 11);
    Event_Wait(12);
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(21);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 12);
    Event_Wait(8);
    Actor_Destroy(ACTOR_FIRST_OF_PAIR);

    actor = Actor_Get(ACTOR_SECOND_OF_PAIR);
    actor->x.fixed = PIXELS(294);
    actor->y.fixed = PIXELS(28);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 8);
    Event_Wait(20);
    actor->x.fixed = PIXELS(300);
    actor->y.fixed = PIXELS(27);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 9);
    Event_Wait(12);
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(17);
    actor->z.fixed = PIXELS(152);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 10);
    Event_Wait(8);
    Actor_Destroy(ACTOR_SECOND_OF_PAIR);
    Event_Wait(160);
}

void FieldScene_RunMultiActorPresentation(void)
{
    s32 count_flag;
    s32 request_a;
    s32 request_b;

    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    Event_OpenMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    VinasuChojo_FaceActor(0, 0x4000);
    count_flag = 0;
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        count_flag = 1;
    } else {
        Event_Wait(20);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
    }
    VinasuChojo_ShowMessage(2);
    if (count_flag != 0) {
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    request_a = 0x1001;
    Actor_WalkToAndWait(ACTOR_GERALD, 0x141, 174);
    VinasuChojo_FaceActor(1, 0x2000);
    VinasuChojo_ShowMessage(request_a);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    VinasuChojo_FaceActor(3, 0xc000);
    VinasuChojo_FaceActor(1, 0x4000);
    VinasuChojo_ShowMessage(request_a);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 40);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    VinasuChojo_ShowMessage(request_a);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 40);
    Event_ShowMessageAndWait(0x8001, 0, 40);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    VinasuChojo_ShowMessage(request_a);
    Audio_PlayCue(17);
    Actor_SetAnimation(ACTOR_MIA, 4);
    VinasuChojo_ShowMessage(3);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 80);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 80);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 60);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    VinasuChojo_FaceActor(2, 0xc000);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 40);
    Actor_Jump(ACTOR_IVAN, 4, 60);
    VinasuChojo_FaceActor(1, 0x2000);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    VinasuChojo_ShowMessage(request_a);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 40);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_Jump(ACTOR_GERALD, 4, 40);
    VinasuChojo_ShowMessage(1);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_SetDestination(21, 200, 188);
    Actor_SetDestination(6, 200, 204);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0xfc0000, 0, 0xbe0000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Audio_PlayCue(23);
    Actor_SetAttachedEffect(21, 0x102);
    VinasuChojo_ShowMessage(21);
    Actor_RunRepeatedMotion(21, 1);
    Event_Wait(20);
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_FaceDirection(21, 0x3000, 20);
    VinasuChojo_ShowMessage(21);
    Actor_SetAttachedEffect(6, 0x102);
    request_b = 0x2003;
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    VinasuChojo_ShowMessage(request_b);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    VinasuChojo_ShowMessage(0x2002);
    VinasuChojo_FaceActor(21, 0xe000);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    VinasuChojo_ShowMessage(1);
    Actor_RunRepeatedMotion(21, 1);
    Event_ShowMessageAndWait(21, 0, 20);
    VinasuChojo_ShowMessage(request_b);
    Actor_SetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    VinasuChojo_ShowMessage(0x2002);
    Actor_SetAnimation(21, 3);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 40);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    VinasuChojo_ShowMessage(1);
    Actor_SetAnimation(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_FaceDirection(21, 0x5000, 20);
    Actor_Jump(ACTOR_MIA, 4, 20);
    Event_ShowMessageAndWait(request_b, 0, 20);
    Actor_ShowEmote(21, 0x103, 40);
}

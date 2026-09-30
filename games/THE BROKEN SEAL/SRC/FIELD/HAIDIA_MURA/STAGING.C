#include "STAGED_MOTION.H"
#include "IWRAM_CALL.H"
#include "CALL.H"
extern u8 MsgHaidiaAsStubbornAsYourFather[];
extern u8 MsgHaidiaDevastatedWhenKyle[];
extern u8 MsgHaidiaGoodJob[];
extern u8 MsgHaidiaWorkingYourselvesBone[];
extern u8 gGeraldAction[];
extern u8 gJasmineAction[];

void Scene_RepairTheHouse(void)
{
    u8 *scene;
    u8 *rec;
    u32 i;
    s32 turn_back;
    s32 turn_side;
    u8 *turned;
    s32 none;
    s32 flag;
    s32 callback_a;
    s32 callback_b;
    s32 callback_c;
    s32 callback_d;
    s32 callback_e;
    s32 callback_f;

    scene = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
    Actor_SetPosition(ACTOR_DORA, 0x1880000, 0x3800000);
    turned = Actor_Get(ACTOR_DORA);
    /*
     * Overwritten at once, but the store must stay: its zero halfword
     * temporary is what the record byte stores below reuse out of a high
     * register.
     */
    *(u16 *)(turned + 6) = 0;
    turn_back = 0xc000;
    *(u16 *)(turned + 6) = turn_back;
    Actor_SetPosition(ACTOR_GERALD, 0x12a0000, 0x2e00000);
    turned = Actor_Get(ACTOR_GERALD);
    turn_side = 0x4000;
    *(u16 *)(turned + 6) = turn_side;
    Actor_SetPosition(ACTOR_JASMINE, 0x12a0000, 0x2f80000);
    turned = Actor_Get(ACTOR_JASMINE);
    *(u16 *)(turned + 6) = turn_side;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 11);
    Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
    rec = Actor_Get(23);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    Actor_SetSpriteFlags(rec, 0);
    rec = Actor_Get(24);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    Actor_SetSpriteFlags(rec, 0);
    rec = Actor_Get(25);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3500000;
    Actor_SetSpriteFlags(rec, 0);
    ((u8 *)Engine_EventGetViewCenter())[85] = 0;
    Task_Wait(1);
    Camera_MoveTo(0x17f0000, 0xa00000, 0x36d0000, 0);
    Map_Redraw();
    Task_Wait(1);
    *(s32 *)(*(u8 **)&gEventWork + 0x1c8) = 32;
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_JASMINE, 0x8000, turn_side);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, turn_side);
    Engine_ActorEnableActionCallback(5, (u32)gJasmineAction);
    Engine_ActorEnableActionCallback(1, (u32)gGeraldAction);
    Event_Wait(40);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Call4(Engine_ActorFaceDirection, 0, 0xb000, 40, 0x10000);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 400, 840);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, turn_back, 30);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 40);
    FieldScene_RunStep8C();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 17);
    Engine_TaskAddCallback((void (*)(void))Effect_PlayStepSound, 3200);
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(scene);
        Task_Wait(1);
    }
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    /* Callback symbols are pooled loads that stay after the preceding call. */
    callback_a = (s32)SceneState_SetValue0ThenCall;
    Value2(Engine_TaskAddCallback, callback_a, 3200);
    callback_b = (s32)FieldScene_RunStep17;
    Call2(Engine_TaskAddCallback, callback_b, 3200);
    Actor_SetSpeed(23, 0x3333, 0x1999);
    Actor_MoveToAndWait(23, 390, 832);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_MoveToAndWait(23, 400, 826);
    Event_Wait(20);
    {
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        u8 value = *(volatile u8 *)&record[35]; /* Keeps the byte in its own register. */

        record[35] = (u8)(value | 1);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Scheduler_RemoveCallback((s32)Effect_PlayStepSound);
    Scheduler_RemoveCallback(callback_a);
    Scheduler_RemoveCallback(callback_b);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(23, 0);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 11);
    Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
    Event_Wait(120);
    Map_CopyCellsTo(7, 102, 84, 41, 2, 1);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 17);
    Call2(Engine_TaskAddCallback, (s32)Effect_PlayStepSound, 3200);
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(scene);
        Task_Wait(1);
    }
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    callback_c = (s32)SceneState_SetValue0ThenCall;
    Value2(Engine_TaskAddCallback, callback_c, 3200);
    callback_d = (s32)SceneState_SetValue24ThenCall;
    Call2(Engine_TaskAddCallback, callback_d, 3200);
    Actor_SetSpeed(24, 0x3333, 0x1999);
    Actor_MoveToAndWait(24, 390, 832);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_MoveToAndWait(24, 377, 828);
    Event_Wait(20);
    {
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Scheduler_RemoveCallback((s32)Effect_PlayStepSound);
    Scheduler_RemoveCallback(callback_c);
    Scheduler_RemoveCallback(callback_d);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(24, 0);
    Actor_SetPosition(24, 0, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 11);
    Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
    Event_Wait(120);
    Map_CopyCellsTo(6, 102, 83, 41, 1, 1);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
    Actor_FaceDirection(ACTOR_DORA, 0xb000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 17);
    Call2(Engine_TaskAddCallback, (s32)Effect_PlayStepSound, 3200);
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(scene);
        Task_Wait(1);
    }
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    callback_e = (s32)SceneState_SetValue0ThenCall;
    Value2(Engine_TaskAddCallback, callback_e, 3200);
    callback_f = (s32)SceneState_SetValue25ThenCall;
    Engine_TaskAddCallback(callback_f, 3200);
    Actor_SetSpeed(25, 0x3333, 0x1999);
    Actor_MoveToAndWait(25, 390, 832);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_MoveToAndWait(25, 360, 837);
    Event_Wait(20);
    {
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Scheduler_RemoveCallback((s32)Effect_PlayStepSound);
    Scheduler_RemoveCallback(callback_e);
    Scheduler_RemoveCallback(callback_f);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetChildValue(25, 0);
    Actor_SetPosition(25, 0, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 11);
    Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
    Event_Wait(120);
    FieldScene_RunSingleStep();
    Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_Jump(ACTOR_DORA, 2, 20);
    Event_SetMessage((s32)MsgHaidiaGoodJob);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x1000, 10);
    HaidiaMura_RunWalkScene032B0(21, 5, 6, 0);
    Actor_SetSpeed(ACTOR_DORA, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 60);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 60);
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 40);
    Actor_FaceDirection(ACTOR_DORA, 0x8000, 40);
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 20);
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_DORA, 4);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(ACTOR_DORA, 3);
        bump_step(1);
    } else {
        Actor_SetAnimationAndWait(ACTOR_DORA, 4);
    }
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Event_SetMessage((s32)MsgHaidiaWorkingYourselvesBone);
    Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_DORA, 0xd000, 60);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 30);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, 0xd000, 60);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_SetMessage((s32)MsgHaidiaDevastatedWhenKyle);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_DORA, 4);
    Event_Wait(60);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x1790000, 0xa00000, 0x35c0000, 1);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_GERALD, 369, 904);
    Actor_WalkToAndWait(ACTOR_JASMINE, 392, 904);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    HaidiaMura_RunWalkScene032B0(5, 10, 11, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_DORA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x1000, 20);
    Actor_Jump(ACTOR_JASMINE, 4, 0);
    Actor_WalkToAndWait(ACTOR_JASMINE, 392, 843);
    Actor_FaceDirection(ACTOR_JASMINE, 0x9000, 0);
    Actor_FaceDirection(ACTOR_DORA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 20);
    ((void (*)())Engine_ActorSetAnimationAndWait)(21, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Actor_SetAnimation(ACTOR_DORA, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    HaidiaMura_RunWalkScene032B0(1, 10, 11, 0);
    Actor_SetSpeed(ACTOR_JASMINE, 0x4ccc, 0x2666);
    Actor_SetSpeed(ACTOR_GERALD, 0x4ccc, 0x2666);
    Actor_WalkTo(ACTOR_GERALD, 392, 843);
    ((u8 *)Object_GetById(5))[90] &= 0xfe;
    Actor_WalkToAndWait(ACTOR_JASMINE, 408, 843);
    Event_Wait(1);
    {
        u8 *record = Actor_Get(ACTOR_JASMINE);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 30);
    Actor_Jump(ACTOR_DORA, 4, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 30);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 258);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_DORA, 0x3000, 40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(ACTOR_DORA, 257, 80);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
    Actor_FaceDirection(ACTOR_DORA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(80);
    Actor_FaceEachOther(ACTOR_JASMINE, ACTOR_GERALD, 30);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_DORA, 261, 60);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 10);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_DORA, 2);
    Event_Wait(40);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_DORA, 4);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x1750000, 0xa00000, 0x3450000, 1);
    Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_FaceDirection(ACTOR_DORA, 0x3000, 10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 40);
    Actor_FaceEachOther(ACTOR_JASMINE, ACTOR_GERALD, 30);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 5, 0x8000, 20);
    Actor_FaceDirection(ACTOR_DORA, 0x5000, 20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 261, 60);
    Actor_SetAnimationAndWait(ACTOR_DORA, 4);
    Event_OpenMessage(ACTOR_DORA, 0);
    none = 0; /* One zero shared by the placement call and the byte store. */
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Event_SetMessage((s32)MsgHaidiaAsStubbornAsYourFather);
    Actor_ShowEmote(ACTOR_DORA, 259, 0);
    Actor_RunRepeatedMotion(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_Jump(ACTOR_DORA, 4, 0);
    Actor_RunRepeatedMotion(ACTOR_DORA, 3);
    Actor_SetAnimation(ACTOR_DORA, 7);
    Event_Wait(5);
    Call11(Engine_EventShowTwoMessagesAndWait, 21, 14, 2, 24, 2, 1, 10, 14, 4, 14, none);
    Audio_PlayCue(161);
    rec = Actor_Get(ACTOR_DORA);
    {
        u8 value = *(volatile u8 *)&rec[90];

        *(u8 *)(*(s32 *)(rec + 80) + 38) = none;
        rec[90] = (u8)(value & 0xfe);
    }
    Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
    Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
    Event_Wait(4);
    for (i = 0; i != 4; i++) {
        *(s32 *)(rec + 16) += 0x18000;
        *(s32 *)(rec + 28) += -0x1999;
        Event_Wait(1);
    }
    Actor_SetPosition(ACTOR_DORA, 0, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0x30000, 0x18000);
    Actor_Jump(ACTOR_GERALD, 6, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 374, 827);
    Event_ShowMessage(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 256, 0);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_ShowEmote(ACTOR_GERALD, 256, 10);
    Actor_SetAnimation(ACTOR_GERALD, 13);
    Actor_Jump(ACTOR_GERALD, 2, 5);
    Audio_PlayCue(143);
    Work_SetValuesIfNonNegative(0, 0x40000, 0x10000);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 10);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 3);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_ShowEmote(ACTOR_GERALD, 258, 80);
    Actor_SetAnimation(ACTOR_DORA, 8);
    *(s32 *)(rec + 28) = 0x8000;
    Actor_SetPosition(ACTOR_DORA, 0x16c0000, 0x32b0000);
    for (i = 0; i != 5; i++) {
        *(s32 *)(rec + 28) += 0x1999;
        Event_Wait(1);
    }
    none = 0; /* Refreshed after the loops for the closing scene store. */
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 30);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(0x1740000, 0xa00000, 0x35b0000, 1);
    Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
    Actor_Jump(ACTOR_DORA, 6, 0);
    Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 20);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    rec[35] &= 0xfe;
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 80);
    Actor_ShowEmote(ACTOR_DORA, 257, 80);
    Actor_FaceDirection(ACTOR_DORA, 0, 60);
    Actor_RunRepeatedMotion(ACTOR_DORA, 3);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAttachedEffect(ACTOR_DORA, 258);
    Event_Wait(80);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 40);
    Actor_ShowEmote(ACTOR_GERALD, 258, 80);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_GERALD), 1);
    Actor_Jump(ACTOR_GERALD, 6, 0);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x40000, 0x20000);
    rec = Actor_Get(ACTOR_GERALD);
    rec[90] &= 0xfe;
    Actor_SetDestination(ACTOR_GERALD, 403, 827);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 258);
    Actor_FaceDirection(ACTOR_JASMINE, 0xc000, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 1);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 256, 0);
    Actor_SetAnimation(ACTOR_GERALD, 13);
    Actor_Jump(ACTOR_GERALD, 2, 5);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Audio_PlayCue(143);
    Work_SetValuesIfNonNegative(0, 0x40000, 0x10000);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_ShowEmote(ACTOR_GERALD, 258, 30);
    Actor_SetSpeed(ACTOR_JASMINE, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(ACTOR_JASMINE, 408, 855);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_ShowEmote(ACTOR_DORA, 261, 60);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 3);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
    Event_Wait(80);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 30);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 4);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_DORA, 0, 60);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 60);
    Actor_SetAnimationAndWait(ACTOR_DORA, 4);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_DORA, 0, 80);
    Actor_ShowEmote(ACTOR_DORA, 261, 80);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 257, 0);
    Actor_ShowEmote(ACTOR_GERALD, 257, 60);
    Actor_SetAnimationAndWait(ACTOR_DORA, 4);
    Event_Wait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 60);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_FaceDirection(ACTOR_DORA, 0, 30);
    Actor_SetAnimationAndWait(ACTOR_DORA, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_ShowEmote(ACTOR_DORA, 256, 0);
    Actor_RunRepeatedMotion(ACTOR_DORA, 3);
    Event_Wait(30);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Actor_Jump(ACTOR_GERALD, 4, 0);
    ((void (*)())Engine_ActorWalkToAndWait)(1, 398, 828);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 60);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Event_Wait(60);
    rec = Actor_Get(ACTOR_GERALD);
    flag = 1; /* One shared mark bit for the three record flags. */
    rec[90] |= flag;
    rec = Actor_Get(ACTOR_JASMINE);
    rec[90] |= flag;
    rec = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_WalkTo(ACTOR_JASMINE, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18));
    Actor_WalkToAndWait(ACTOR_GERALD, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18) - 16);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 30);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_JASMINE, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(40);
    Actor_WalkToAndWait(ACTOR_JASMINE, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Party_AddMembers(1, 5);
    Camera_MoveTo(0x1790000, 0xa00000, 0x3770000, 1);
    HaidiaMura_RunWalkScene03380(0, 13, 10, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    {
        u8 *record = Actor_Get(ACTOR_DORA);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | flag);
    }
    HaidiaMura_RunWalkScene03380(21, 6, 5, 0);
    Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
    Actor_FaceDirection(ACTOR_DORA, 0x4000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
    Actor_SetAnimation(ACTOR_DORA, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Camera_WaitForMove();
    Event_Wait(100);
    Map_CopyCellAttributes(49, 46, 8, 4, 20, 50);
    GameFlag_Set(514);
    GameFlag_Clear(303);
    scene[85] = 3;
    *(s32 *)(scene + 12) = 0xa00000;
    *(s32 *)(scene + 60) = 0x80000000;
    *(s32 *)(scene + 40) = none;
    Event_End();
}

void FieldScene_RunStep8C(void)
{
    Psynergy_Begin(0x8c, 0);
}

void FieldScene_RunSingleStep(void)
{
    BattleEffect_CleanupSceneObjects();
}

void SceneState_SetValue1ThenCall(void)
{
    Actor_Get(ACTOR_GERALD);
    SceneEffect_UpdateObjectOnOddFrames();
}

void SceneState_SetValue0ThenCall(void)
{
    Actor_Get(ACTOR_PARTY_LEADER);
    SceneEffect_UpdateObjectOnOddFrames();
}

void FieldScene_RunStep9(void)
{
    Actor_Get(9);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

void FieldScene_RunStep17(void)
{
    Actor_Get(0x17);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

void SceneState_SetValue24ThenCall(void)
{
    Actor_Get(0x18);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

void SceneState_SetValue25ThenCall(void)
{
    Actor_Get(0x19);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

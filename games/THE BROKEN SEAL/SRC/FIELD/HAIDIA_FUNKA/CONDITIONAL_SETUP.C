#include "TYPES.H"
#include "FIELD_EVENT.H"

extern s16 Data_02000240[];
extern u8 Data_02008cb4[];
extern u8 Data_03001ebc[];

s32 Func_020000dc(void);
void Func_02000cbc();
void Func_02000cc0();
void Func_02000d2c();
void Func_02000d3c();
void Func_02000d5a();
void Func_02000e7e();
void Func_0200110a();
void Func_02001126();
void Func_02001182();
void Func_02001528();
void Func_02001612();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

u8 *ConditionalSceneSetup_GetScriptData(void)
{
    return (u8 *)0x02008cf0;
}

s32 ConditionalSceneSetup_GetInitialState(void)
{
    return 0;
}

u8 *ConditionalSceneSetup_GetMessageData(void)
{
    return (u8 *)0x02008d38;
}

u8 *ConditionalSceneSetup_GetActorData(void)
{
    return (u8 *)0x02008d44;
}

u8 *ConditionalSceneSetup_GetEffectData(void)
{
    return (u8 *)0x02008e94;
}

s32 ConditionalSceneSetup_InitForScene15(void)
{
    if (Data_02000240[225] == 15) {
        Func_020000dc();
    }
    return 0;
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base5_2008cb4;

    Event_Begin();
    Actor_SetAnimation(14, 0);
    Actor_SetAnimation(15, 0);
    Actor_SetAnimation(16, 0);
    Actor_SetAnimation(17, 0);
    Actor_SetAnimation(18, 0);
    Actor_SetAnimation(19, 0);
    Actor_WalkToAndWait(11, 0x109, 0x1e7);
    Actor_FaceDirection(11, 0xa000, 0);
    Actor_WalkToAndWait(12, 0x100, 0x1f4);
    Actor_FaceDirection(12, 0xa000, 0);
    Call2(Func_02000cc0, 0x10003, 0x10006);
    Func_02000cbc();
    Task_Wait(60);
    Camera_MoveTo(0x1000000, -1, 0x2640000, 0);
    Camera_WaitForMove();
    Map_Redraw();
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0;
    *(s32 *)(work + 0x1c8) = 32;
    Event_OpenScreen();
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x1000000, -1, 0x1f40000, 1);
    Event_Wait(20);
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Func_02000d2c();
    Audio_PlayCue(145);
    Event_Wait(30);
    Func_02000d3c();
    Audio_PlayCue(145);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x20000, 0x30000, 0x10000);
    Func_02000d5a();
    Audio_PlayCue(145);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Event_Wait(60);
    Event_SetMessage(0x1122);
    Actor_ShowEmote(8, 0x102, 0);
    Event_Wait(60);
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(11, 4);
    Event_ShowMessage(11, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_FaceActor(12, 11, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(12, 4);
    Event_ShowMessage(12, 0);
    Actor_RunRepeatedMotion(13, 1);
    Event_ShowMessage(13, 0);
    Actor_FaceActor(10, 13, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(10, 1);
    Event_ShowMessage(10, 0);
    Actor_FaceActor(9, 10, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(9, 1);
    Event_ShowMessage(9, 0);
    Actor_FaceActor(10, 9, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(10, 4);
    Event_ShowMessage(10, 0);
    Event_Wait(60);
    Work_SetValuesIfNonNegative(0x20000, 0x30000, 0x10000);
    Func_02000e7e();
    Audio_PlayCue(145);
    Event_Wait(60);
    Actor_FaceEachOther(8, 9, 0);
    Actor_FaceEachOther(10, 11, 0);
    Actor_FaceEachOther(12, 13, 0);
    Actor_StartRepeatedMotion(8, 2);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Actor_StartRepeatedMotion(13, 2);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_SetSpeed(1, 0x8000, 0x4000);
    Actor_SetPosition(0, 0x780000, 0x1020000);
    Camera_SetSpeed(0x18000, 0x3000);
    Camera_MoveTo(0x700000, -1, 0x1400000, 1);
    Event_Wait(40);
    Actor_SetAnimation(0, 2);
    Actor_SetAnimation(1, 2);
    Actor_SetDestination(0, 120, 0x140);
    Actor_MoveToAndWait(1, 104, 0x140);
    Actor_SetAnimation(0, 1);
    Actor_SetAnimation(1, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Actor_FaceDirection(1, 0x3000, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Event_Wait(50);
    Actor_SetSpeed(1, 0x18000, 0xc000);
    Actor_SetAnimation(1, 2);
    Actor_MoveToAndWait(1, 105, 0x156);
    Actor_SetAnimation(1, 1);
    Actor_RunRepeatedMotion(1, 2);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(0, 1);
    Actor_FaceEachOther(0, 1, 0);
    Event_Wait(20);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(60);
        Actor_SetAnimation(0, 3);
        Actor_SetAnimation(1, 3);
        Event_Wait(50);
        Actor_FaceDirection(0, 0xc000, 0);
        Actor_SetAnimation(1, 2);
        Actor_SetSpeed(1, 0x10000, 0x8000);
        Actor_MoveToAndWait(1, 103, 0x140);
        Actor_SetAnimation(1, 1);
    } else {
        Event_Wait(60);
        Actor_SetAnimation(0, 3);
        Actor_SetAnimation(1, 3);
        Event_Wait(50);
        Actor_FaceDirection(1, 0x3000, 0);
        Actor_SetAnimation(0, 2);
        Actor_MoveToAndWait(0, 120, 0x154);
        Actor_SetAnimation(0, 1);
    }
    Event_ShowMessage(12, 0);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(40);
    Actor_FaceDirection(9, 0xa000, 0);
    Actor_FaceDirection(11, 0xa000, 0);
    Actor_FaceDirection(10, 0xa000, 0);
    Actor_FaceDirection(12, 0xa000, 0);
    Actor_FaceDirection(13, 0xa000, 0);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveToActor(10, 1);
    Camera_WaitForMove();
    Event_Wait(50);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessage(10, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessage(8, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(9, 1);
    Event_ShowMessage(9, 0);
    Event_Wait(40);
    Actor_FaceDirection(0, 0x3000, 0);
    Actor_FaceDirection(1, 0x3000, 0);
    Camera_MoveTo(0x700000, -1, 0x1400000, 1);
    Camera_WaitForMove();
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Func_0200110a(1);
    Event_Wait(50);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Func_02001126(1);
    Event_Wait(60);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0xd60000, -1, 0x1d80000, 1);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_EnableActionCallback(0, 0x2008c00);
    Event_Wait(30);
    Actor_EnableActionCallback(1, 0x2008c64);
    Func_02001182(1);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(1, 0, 0);
    Camera_WaitForMove();
    Actor_FaceDirection(9, 0x8000, 0);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_SetAnimation(8, 2);
    Actor_MoveToAndWait(8, 0x109, 0x1c7);
    Actor_MoveToAndWait(8, 246, 0x1c7);
    Actor_SetAnimation(8, 1);
    Event_Wait(30);
    Actor_RunRepeatedMotion(9, 1);
    Event_ShowMessage(9, 0);
    Actor_ShowEmote(0, 0x102, 0);
    Event_Wait(50);
    Actor_ShowEmote(1, 0x102, 0);
    Event_Wait(50);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessage(8, 0);
    Event_Wait(40);
    Actor_FaceEachOther(0, 1, 0);
    Event_Wait(50);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(1, 0, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 4);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(40);
    Actor_ShowEmote(10, 0x102, 0);
    Event_Wait(50);
    Actor_FaceDirection(1, 0x3000, 0);
    Event_Wait(10);
    Event_OpenMessage(10, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(40);
        Actor_FaceEachOther(8, 9, 0);
        Event_Wait(50);
        Actor_FaceDirection(8, 0x8000, 0);
        Actor_FaceDirection(9, 0x8000, 0);
        Event_Wait(40);
        Actor_RunRepeatedMotion(9, 1);
        Event_ShowMessage(9, 0);
        bump_step(1);
    } else {
        Event_Wait(40);
        Actor_FaceEachOther(8, 9, 0);
        Event_Wait(50);
        Actor_FaceDirection(8, 0x8000, 0);
        Actor_FaceDirection(9, 0x8000, 0);
        Event_Wait(40);
        Actor_RunRepeatedMotion(9, 1);
        bump_step(1);
        Event_ShowMessage(9, 0);
    }
    Event_Wait(30);
    Actor_FaceDirection(1, 0, 0);
    Event_Wait(30);
    Actor_StartRepeatedMotion(0, 2);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(40);
    Actor_SetAnimation(0, 4);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(60);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessage(8, 0);
    Actor_FaceActor(8, 9, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0xb000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(50);
    Actor_FaceDirection(8, 0x8000, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessage(8, 0);
    Event_Wait(40);
    Actor_FaceEachOther(8, 9, 0);
    Event_Wait(40);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(30);
    Actor_WalkToAndWait(8, 255, 0x1bd);
    Event_Wait(40);
    Map_AnimateCells(0x2008ea0, 45, 11);
    Audio_PlayCue(188);
    Event_Wait(30);
    Actor_WalkTo(8, 255, 0x186);
    Event_Wait(20);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkTo(9, 255, 0x186);
    Actor_WalkToAndWait(10, 255, 0x1cc);
    Actor_FaceDirection(10, 0x8000, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(30);
    Actor_StartRepeatedMotion(0, 1);
    Actor_RunRepeatedMotion(1, 1);
    Event_Wait(40);
    Actor_WalkTo(10, 255, 0x186);
    base5_2008cb4 = (s32)Data_02008cb4;
    Actor_EnableActionCallback(0, base5_2008cb4);
    Event_Wait(40);
    Actor_EnableActionCallback(1, base5_2008cb4);
    Func_02001528(1);
    Actor_ShowEmote(11, 0x102, 0);
    Actor_ShowEmote(12, 0x102, 0);
    Event_Wait(40);
    Work_SetValuesIfNonNegative(0x20000, 0x30000, 0x10000);
    Func_02001612();
    Audio_PlayCue(145);
    Event_Wait(30);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0;
    *(s32 *)(work + 0x1c8) = 64;
    Event_CloseScreen();
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    GameFlag_Clear(0x12f);
    GameFlag_Set(0x879);
    Event_RequestExit(1);
    Event_End();
}

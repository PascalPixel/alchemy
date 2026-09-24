#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum PartyEventsMessage {
    MSG_WHEN_HEARD_WERE_BACK_IVAN = 0x1b21,
    MSG_LORD_HAMMET_WILL_RELEASED_SOON = 0x1b83,
    MSG_HAS_LEGACY_LORD_HAMMETS_SILK = 0x1b88,
    MSG_ROBIN_SNEAKED_INTO_LUNPA_THATS = 0x1b91,
    MSG_ITS_IVAN_HIS_COMPANIONS_PERFECT = 0x2588
};


extern s16 Data_02000240[];
extern u8 Value_00000067;
extern u8 Data_02009c04[];
extern u8 Data_02009bd4[];
extern u8 Data_02009df4[];
extern u8 Data_02009ddc[];
extern u8 Data_02009f38[];
extern u8 Data_02009f2c[];
extern u8 Data_02009b94[];

void Func_02001b1a();
void Func_02001c06();
void Func_02001c6c();
void Func_02001c72();
void Func_02001cc0();
void Func_02001cc6();
u8 *Func_02001d46();
u8 *Func_02001d82();
void Func_02001d58_handoff(s32);
void Func_0200191e();
void Func_02001bbe();
void Func_02001f4e();
void Func_02001ffc();
void Func_02002258();
void Func_020022c2();
void Func_02002410();
void Func_02002646();
void Func_020026ae();
s32 Func_02002e84();
void Func_02002a2e();
void Func_02002d34();
s32 Func_02002ff0();
s32 Func_02003004();
s32 Func_02003018();
s32 Func_020034a2_a();

/* Signed halfword table in RAM; index 225 selects the scene. */

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; imports are named by the main-image address in the
 * trailing word of the overlay veneer. Old-style declarations are required
 * here, because the arity varies from site to site.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight
 * into the argument registers, while a direct call precomputes a costly
 * constant into a local that later uses in the block share. A call that
 * returns a value sets r0 last of its arguments.
 */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Func_02001c00();

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];
    void Func_02001c00();

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureFourth(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformSecond(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformThird(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformFourth(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    extern u8 Data_03001ebc[];

    *(u16 *)(*(u8 **)Data_03001ebc + 0x1d8) += amount;
}

s32 SceneData_SelectTable9bd4ByState(void)
{
    if (gGameState.scene == (s32)&Value_00000067) {
        return (s32)Data_02009c04;
    }
    return (s32)Data_02009bd4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * Returns the in-image table address 0x02009d9c, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr.
 */
u8 *SceneData_GetTable9d9c(void)
{
    return (u8 *)0x02009d9c;
}

s32 SceneData_SelectTable9ddcByStateWithInit(void)
{
    if (gGameState.scene == (s32)&Value_00000067) {
        Func_02001b1a(Data_02009df4);
        return (s32)Data_02009df4;
    }
    return (s32)Data_02009ddc;
}

s32 SceneData_SelectTable9f2cByState(void)
{
    if (gGameState.scene == (s32)&Value_00000067) {
        return (s32)Data_02009f38;
    }
    return (s32)Data_02009f2c;
}

void SceneDialogue_RunActor13Message1b83(void)
{
    Event_Begin();
    Event_SetMessage(MSG_LORD_HAMMET_WILL_RELEASED_SOON);
    Event_AskYesNo(13, 0);
    Event_End();
}

void SceneDialogue_RunActor16Message1b88(void)
{
    Event_Begin();
    Event_SetMessage(MSG_HAS_LEGACY_LORD_HAMMETS_SILK);
    Event_AskYesNo(16, 0);
    Event_End();
}

void FieldScene_RunActorEightTurnDialogue(void)
{
    void Event_End(void);

    Event_Begin();
    Actor_ShowEmote(8, 0x100, 0x3C);
    Event_SetMessage(MSG_ROBIN_SNEAKED_INTO_LUNPA_THATS);
    Event_ShowMessageAndWait(8, 0, 0xA);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 0xA);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 0xA);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 0xA);
    GameFlag_Set(0x913);
    Event_End();
}

void FieldScene_RunScene3aa_02000184(void)
{
    void Func_02001c00();

    u32 i;
    s32 record;
    struct EventWork *p5;

    p5 = gEventWork;
    Event_Begin();
    Event_Wait(10);
    if (p5->touched_trigger == 4) {
        Audio_PlayCue(188);
    } else {
        Audio_PlayCue(158);
    }
    Func_02001c00(1);
    Func_02001c06(2);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    if (p5->touched_trigger == 4) {
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
    Event_Wait(16);
    Event_RequestExit(p5->touched_trigger);
    Func_02001c6c(1);
    Func_02001c72(2);
    Event_End();
}

s32 SceneState_SetWord448To209AndRun(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.scene == (s32)(u32)&Value_00000067) {
        FieldScene_DispatchSceneByIndex();
    }
    return 0;
}

/*
 * The overlay's scene dispatcher, switching on the scene selector
 * Data_02000240[225]: 10 and 11 share a body and any other value does
 * nothing. The epilogue pops the return address into r0, so this is void
 * and takes no arguments; the 252-byte owner includes its seven pool words.
 * The locals holding the coordinates, the record's +6 halfword (named by
 * position only) and the fifth and sixth arguments force those to be built.
 */
void FieldScene_DispatchSceneByIndex(void)
{
    u8 *rec;
    s32 h;
    s32 x1 = 0x038a0000;
    s32 z1 = 0x01a60000;

    Func_02001cc0(1);
    Func_02001cc6(2);

    switch (gGameState.entrance) {
    case 9:
        if (GameFlag_IsSet(0x941) != 0) {
            rec = Func_02001d46(8);
            h = 0x1000;
            *(u16 *)(rec + 6) = h;

            if (GameFlag_IsSet(0x914) == 0) {
                Scene_RunPartySequence();
            }
        } else {
            Actor_SetPosition(9, 0, 0);
            if (GameFlag_IsSet(0x321) != 0) {
                Actor_SetPosition(8, x1, z1);
                rec = Func_02001d82(8);
                h = 0xd000;
                *(u16 *)(rec + 6) = h;
            }
        }
        break;

    case 10:
    case 11:
        if (GameFlag_IsSet(0x915) != 0) {
            s32 a5 = 4;
            s32 a6 = 3;
            Map_CopyCellsTo(58, 70, 54, 70, a5, a6);
            {
                s32 b5 = 55;
                s32 b6 = 8;
                Map_CopyCellAttributes(55, 9, 2, 1, b5, b6);
            }
            Map_Redraw();
            Task_Wait(1);
        }
        break;

    case 20:
        Actor_SetPosition(9, 0, 0);
        if (GameFlag_IsSet(0x109) == 0) {
            RunEventScript01();
        }
        break;

    default:
        break;
    }
}

void RunEventScript01(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    u8 *work;
    s32 v5;
    s32 tbl;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    ColorBuffer_ApplySource(0x10002, 0);
    ColorBuffer_ApplyTarget(0x10002, 0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c8) = 24;
    *(s32 *)(work + 0x1c0) = 0x201;
    Actor_SetPosition(8, 0x3580000, 0x1b80000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3580000, 0x1e60000);
    Actor_SetPosition(ACTOR_GERALD, 0x3500000, 0x1f60000);
    Actor_SetPosition(ACTOR_IVAN, 0x3680000, 0x1e60000);
    Actor_SetPosition(ACTOR_MIA, 0x3700000, 0x1f60000);
    Actor_SetPosition(10, 0x3480000, 0x2060000);
    Actor_SetPosition(11, 0x3780000, 0x2060000);
    Camera_MoveTo(0x3600000, -1, 0x1d80000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Func_0200191e(20);
    Actor_SetAttachedEffect(8, 0x102);
    Actor_StartRepeatedMotion(8, 2);
    Event_Wait(60);
    Actor_FaceDirection(8, 0x5000, 10);
    Actor_SetAnimationAndWait(8, 3);
    Actor_ShowEmote(10, 0x100, 0);
    Actor_StartRepeatedMotion(10, 2);
    Event_Wait(60);
    Actor_FaceDirection(10, 0xf000, 20);
    Actor_RunRepeatedMotion(11, 2);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0xb000, 20);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_RunRepeatedMotion(11, 2);
    Actor_SetAnimationAndWait(11, 4);
    Actor_SetAnimationAndWait(11, 4);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(40);
    Task_Wait(60);
    Actor_ShowEmote(8, 0x105, 60);
    Event_SetMessage(MSG_WHEN_HEARD_WERE_BACK_IVAN);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_SetAnimationAndWait(11, 4);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 10);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_SetAnimation(10, 4);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 10);
    Actor_SetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 10);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Actor_SetAnimationAndWait(10, 3);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 10);
    Event_AskYesNo(0x6002, 0);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Actor_FaceDirection(8, 0x1000, 40);
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x37c, 0x1b8);
    Event_Wait(40);
    Actor_FaceDirection(8, 0xd000, 20);
    Actor_ShowEmote(8, 0x105, 60);
    ConfigureFourSceneChannelsAndHandoff(60);
    Func_02001bbe(40);
    Actor_WalkToAndWait(8, 0x358, 0x1b8);
    Event_Wait(40);
    Actor_FaceDirection(8, 0x9000, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_FaceDirection(10, 0xf000, 0);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0xb000, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_RunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0x5000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 0);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_FaceDirection(10, 0xf000, 20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Actor_FaceDirection(11, 0xb000, 20);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimationAndWait(11, 3);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0xb000, 10);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 20);
    Func_02001d58_handoff(20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(40);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Actor_SetAnimation(8, 4);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    v5 = 1;
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
        v5 = 0;
    }
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    if (v5 != 0) {
        bump_step(1);
    }
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 10);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    Actor_ShowEmote(8, 0x101, 0);
    Actor_FaceDirection(8, 0x3000, 40);
    Actor_FaceDirection(8, 0x5000, 20);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 10);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(8, 0x107, 60);
    ((void (*)())Engine_EventShowMessageAndWait)(8, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_OpenMessage(8, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(8, 3);
        Event_ShowMessageAndWait(8, 0, 10);
        bump_step(1);
    } else {
        Event_Wait(20);
        Actor_SetAnimationAndWait(8, 4);
        bump_step(1);
        Event_ShowMessageAndWait(8, 0, 10);
    }
    Func_02001f4e(20);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 40);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 10);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x364, 0x1d8);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    Func_02001ffc(20);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x5000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_OpenMessage(8, 0);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAttachedEffect(8, 0x102);
        Event_Wait(40);
        Event_ShowMessageAndWait(8, 0, 10);
        bump_step(2);
    } else {
        Event_Wait(20);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        bump_step(1);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Actor_SetAttachedEffect(8, 0x102);
        Event_Wait(40);
        Event_ShowMessageAndWait(8, 0, 10);
    }
    Actor_ShowEmote(ACTOR_MIA, 0x101, 40);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 10);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(40);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 10);
    Func_02002258(10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 4);
    Actor_SetAnimation(ACTOR_GERALD, 4);
    Actor_SetAnimation(ACTOR_IVAN, 4);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Func_020022c2(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_ShowEmote(8, 0x108, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_FaceDirection(8, 0x5000, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(10, 2);
    Actor_SetAnimationAndWait(10, 4);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_ShowEmote(8, 0x105, 40);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_SetAnimation(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x5000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(8, 0x100, 40);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    Func_02002410(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimation(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(60);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimation(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 60);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 10);
    Actor_SetAnimation(ACTOR_IVAN, 4);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 10);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_StartRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessageAndWait(11, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Actor_SetAnimation(ACTOR_IVAN, 4);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    ((void (*)())Engine_EventShowMessageAndWait)(1, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_OpenMessage(8, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Func_02002646(10);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Actor_SetAnimationAndWait(ACTOR_MIA, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        bump_step(2);
    } else {
        Event_Wait(10);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        bump_step(1);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Func_020026ae(10);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Actor_SetAnimationAndWait(ACTOR_MIA, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    }
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_FaceDirection(8, 0x5000, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 1);
    Event_Wait(20);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(10);
    Actor_RunRepeatedMotion(11, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimationAndWait(11, 3);
    Actor_SetSpeed(10, 0x10000, 0x8000);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_WalkTo(10, 0x350, 0x21c);
    Actor_WalkToAndWait(11, 0x370, 0x21c);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetAnimationAndWait(8, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    tbl = (s32)Data_02009b94;
    Actor_EnableActionCallback(ACTOR_GERALD, tbl);
    Value2(Engine_ActorEnableActionCallback, 2, tbl);
    Value2(Func_02002e84, 3, tbl);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c8) = 16;
    *(s32 *)(work + 0x1c0) = 0x209;
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Set(0x912);
    Event_End();
}

void ConfigureFourSceneChannelsAndHandoff(s32 handoff)
{
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    ConfigureSecond(1, 0xe000, 0);
    ConfigureThird(2, 0x2000, 0);
    ConfigureFourth(3, 0xa000, 0);
    if (handoff != 0) {
        Event_Wait(handoff);
    }
}

void Func_02001450(s32 handoff)
{
    ConfigureFirst(0, 0xc000, 0);
    ConfigureUniformSecond(1, 0xc000, 0);
    ConfigureUniformThird(2, 0xc000, 0);
    ConfigureUniformFourth(3, 0xc000, 0);
    if (handoff != 0) {
        Event_Wait(handoff);
    }
}

void Scene_RunPartySequence(void)
{
    extern u8 Data_03001ebc[];

    s32 record;
    u8 *work;
    s32 v5;
    s32 tbl;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Camera_MoveTo(0x3600000, -1, 0x2180000, 0);
    Map_Redraw();
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3600000, 0x2760000);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = v5 = 0x100;
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 40;
    Event_OpenScreen();
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3600000, -1, 0x1d80000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x360, 0x1f2);
    record = Value1(Func_02002ff0, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003004, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003018, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x358, 0x1e6);
    Actor_WalkTo(ACTOR_GERALD, 0x350, 0x1f6);
    Actor_WalkTo(ACTOR_IVAN, 0x368, 0x1e6);
    Actor_WalkToAndWait(ACTOR_MIA, 0x370, 0x1f6);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Event_Wait(10);
    Func_02002a2e(10);
    Actor_ShowEmote(9, v5, 20);
    Actor_FaceDirection(9, 0x5000, 20);
    Event_SetMessage(MSG_ITS_IVAN_HIS_COMPANIONS_PERFECT);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_ShowEmote(8, v5, 20);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(8, 0x107, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(60);
    Actor_ShowEmote(9, 0x102, 60);
    Actor_FaceDirection(9, 0x7000, 10);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_FaceDirection(8, 0x1000, 10);
    Actor_ShowEmote(8, 0x108, 20);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_OpenMessage(8, 0);
    v5 = 1;
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_SetAnimation(8, 3);
    } else {
        Event_Wait(10);
        Scene_AdvanceStep(1);
        Actor_SetAnimation(8, 4);
        v5 = 0;
    }
    Event_ShowMessageAndWait(8, 0, 10);
    if (v5 != 0) {
        Scene_AdvanceStep(1);
    }
    Actor_StartRepeatedMotion(9, 2);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(80);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_FaceDirection(8, 0x1000, 10);
    Actor_ShowEmote(8, 0x107, 40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 3);
    Event_ShowMessageAndWait(0x2002, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_FaceDirection(8, 0x3000, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 10);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessageAndWait(0x2002, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 40);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Scene_AdvanceStep(1);
    } else {
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    }
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Func_02002d34(20);
    Actor_RunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_FaceDirection(9, 0x3000, 10);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_RunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0x5000, 10);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(9, 4);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    tbl = (s32)Data_02009b94;
    Actor_EnableActionCallback(ACTOR_GERALD, tbl);
    Value2(Engine_ActorEnableActionCallback, 2, tbl);
    Value2(Func_020034a2_a, 3, tbl);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(((s32)work + 0x1c8)) = 16;
    *(s32 *)(((s32)work + 0x1c0)) = 0x209;
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Set(0x914);
    Event_End();
}

#include "TYPES.H"
#include "FIELD_EVENT.H"

enum EventSetupSceneMessage {
    MSG_YAHH_SILK_ROAD_BOULDERS_BLOCK = 0x18b5,
    MSG_DO_DO_WARRIOR_SHOULD_RETURN = 0x18b9,
    MSG_HE_WHO_HAS_POWER_SEE = 0x1956,
    MSG_YOUNG_WARRIORS_DO_COME_FROM = 0x1958,
    MSG_DO_KNOW_MEDITATION = 0x195d,
    MSG_DID_KNOW_MASTER_HAMA_GREATEST = 0x1961,
    MSG_HSU_OKAY = 0x19cf,
    MSG_ROBIN_DID_LIFT_BOULDER = 0x19da,
    MSG_ADEPTS_LET_ME_THANK_AGAIN = 0x19e9,
    MSG_NORTH_ALTIN_MINE_WEST_LAMA = 0x1a12,
    MSG_AM_TRAVELING_AROUND_WORLD_SPREAD = 0x1a1e
};


#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct EventActor {
    u8 reserved_00[0x23];
    u8 flags;
    u8 reserved_24[0x2c];
    u8 *render_state;
};

struct Slot020008e0 {
    u8 head[6];
    u16 heading;
};

struct Actor02001060 {
    u8 head[12];
    s32 rank;
    u8 body[19];
    u8 flags;
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern u8 Value_0000004a;
extern u8 Data_02009844[];
extern u8 Data_020097b4[];
extern u8 Data_02009a38[];
extern u8 Data_02009918[];
extern u8 Data_02009c9c[];
extern u8 Data_02009b10[];

s16 Func_0200150e(s32, s32);
void *Func_02001564(s32);
s32 Func_0200169e();
void Func_02001b98();
u8 *Func_02001e6e();
s32 Func_02001e78();
u8 *Func_02001eb4();
void Func_02001f3c();
u8 *Func_02001f50();
void Func_02001f5c();
s32 Func_02002214();
s32 Func_02002226_b();
u8 *Func_02002408();
struct Actor02001060 *Func_02002570(s32);
struct Actor *Func_0200259e(s32);
struct Actor *Func_020025a6(s32);
u8 *Func_020025cc();
u8 *Func_020025de();
u8 *Func_020025ec();
u8 *Func_020025f8();
u8 *Func_0200266c();

/* Scene event steps and table getters for resource_3a2. */

/*
 * Table getter at 0x020000b0. The eight-byte owner includes its one pool word
 * at 0x020000b4, which holds 0x020098ec; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/* Value-returning: the reference sets r1 before r0 at this site. */

/* Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call instead precomputes a costly constant
 * into a pseudo shared with later uses in the block. A value-returning call
 * sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/*
 * Prepare actor 14 at 0x020010b8: clear bit 1 of the bytes at +35 and +89,
 * clear the byte at +85, and install the callback at 0x02009061 in the record
 * at +0x6c -- that pool word is odd, so it is a Thumb entry and not data. The
 * zero stored at +85 is held in a local because a register carries it. The
 * callback drives the same bit-1 flag the clears here touch.
 */

/* Two sites reach this one symbol with different arities; old-style so both
 * calls are legal. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    void Event_ShowMessageAndWait();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Event_ShowMessageAndWait();

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    void Event_ShowMessageAndWait();

    return f(a0, a1);
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Actor_SetPosition(s32, s32, s32);

    Actor_SetPosition(actor, x, y);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    gEventWork->message += amount;
}

s32 EventScript_PrepareActorRenderFlags(struct EventActor *actor)
{
    actor->flags &= ~1;
    actor->render_state[9] |= 0xc;
    actor->render_state[21] |= 0xc;
    return 0;
}

s32 OverlayObject_SetFacingTowardObject10(void *self)
{
    void *obj;

    obj = Func_02001564(0xA);
    FIELD_AT_OFFSET(self, s16 *, 6) = Func_0200150e(FIELD_AT_OFFSET(obj, s32 *, 0x10) - FIELD_AT_OFFSET(self, s32 *, 0x10), FIELD_AT_OFFSET(obj, s32 *, 8) - FIELD_AT_OFFSET(self, s32 *, 8));
    return 0;
}

s32 SceneData_SelectTable97b4ByState(void)
{
    if (gGameState.scene == (s32)&Value_0000004a) {
        return (s32)Data_02009844;
    }
    return (s32)Data_020097b4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable98ec(void)
{
    return (u8 *)0x020098ec;
}

s32 SceneData_SelectTable9918ByState(void)
{
    if (gGameState.scene == (s32)&Value_0000004a) {
        return (s32)Data_02009a38;
    }
    return (s32)Data_02009918;
}

void SceneDialogue_RunMessage1958Step(void)
{
    extern u8 *Data_03001ebc;

    u8 *work;

    Event_Begin();
    Event_SetMessage(MSG_YOUNG_WARRIORS_DO_COME_FROM);
    Event_OpenMessage(10, 0);

    if (Event_ChooseYesNo(0, 0) == 1) {
        Event_Wait(20);
        Event_ShowMessage(10, 0);
    } else {
        work = Data_03001ebc;
        *(u16 *)(work + 472) += 1;
        Event_AskYesNo(10, 0);
    }

    Event_End();
}

void SceneDialogue_RunActor11Message195d(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DO_KNOW_MEDITATION);
    Event_AskYesNo(11, 0);
    Event_End();
}

void SceneDialogue_RunActor13Message1961(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DID_KNOW_MASTER_HAMA_GREATEST);
    Event_AskYesNo(13, 0);
    Event_End();
}

void FieldScene_RunPrimaryScript(void)
{
    Audio_PlayCue(188);
    Map_AnimateCells(0x2009788, 67, 6);
    *(u8 *)(Func_0200169e(0) + 85) = 0;
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_SetAnimation(0, 2);
    Actor_SetDestinationOffset(0, 0, -16);
    Event_Wait(16);
    Event_RequestExit(2);
}

void FieldScene_RunScene3a2SequenceA(void)
{
    void Event_ShowMessageAndWait();

    Event_Begin();
    Actor_SetPosition(8, 0x880000, 0xa80000);
    Actor_FaceDirection(8, 0x5000, 0);
    Actor_SetPosition(0, 0x900000, 0xc80000);
    Actor_SetPosition(1, 0xa00000, 0xc00000);
    Actor_SetPosition(2, 0x800000, 0xc80000);
    Actor_SetPosition(3, 0x700000, 0xc00000);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xa000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xe000, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(60);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_SetMessage(MSG_ADEPTS_LET_ME_THANK_AGAIN);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(3, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(2, 0x102, 60);
    Event_Wait(120);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Actor_FaceDirection(1, 0x6000, 20);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_FaceDirection(3, 0, 20);
        Actor_ShowEmote(3, 0x101, 60);
        Event_ShowMessageAndWait(3, 0, 20);
        Actor_FaceDirection(0, 0xe000, 0);
        Actor_FaceDirection(2, 0xe000, 0);
        Actor_StartRepeatedMotion(1, 2);
        Actor_SetAttachedEffect(1, 0x102);
        Event_Wait(60);
        Actor_FaceDirection(1, 0x8000, 20);
        Event_ShowMessageAndWait(1, 0, 20);
        bump_step(2);
    } else {
        bump_step(2);
        Event_Wait(20);
        Actor_FaceDirection(3, 0, 20);
        Actor_SetAnimationAndWait(3, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(3, 0, 20);
        Actor_ShowEmote(1, 0x102, 60);
        Actor_FaceDirection(0, 0xe000, 0);
        Actor_FaceDirection(2, 0xe000, 0);
        Event_ShowMessageAndWait(1, 0, 20);
    }
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xa000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xe000, 0);
    Event_Wait(20);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_FaceDirection(8, 0xc000, 30);
    Audio_PlayCue(188);
    Map_AnimateCells(0x2009788, 67, 6);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 136, 136);
    Actor_SetPosition(8, 0, 0);
    Audio_PlayCue(188);
    Map_AnimateCells(0x200979e, 67, 6);
    Event_Wait(60);
    Func_02001b98();
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Actor_FaceDirection(1, 0x6000, 20);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_FaceDirection(0, 0x8000, 20);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Actor_FaceDirection(3, 0x2000, 20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_RunRepeatedMotion(1, 1);
    Actor_FaceDirection(0, 0xe000, 20);
    Event_AskYesNo(1, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Actor_FaceDirection(0, 0x8000, 20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_RunRepeatedMotion(3, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_FaceDirection(2, 0xb000, 20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_ShowEmote(1, 0x102, 60);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_FaceDirection(2, 0xe000, 20);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(3, 0x102, 60);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_FaceDirection(2, 0xa000, 20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_RunRepeatedMotion(3, 1);
    Event_Wait(20);
    Actor_SetSpeed(2, 0x8000, 0x4000);
    Actor_WalkToAndWait(2, 128, 184);
    Actor_FaceDirection(2, 0x4000, 20);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xa000, 0);
    Actor_FaceDirection(3, 0xe000, 20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(2, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_SetSpeed(1, 0x8000, 0x4000);
    Actor_SetSpeed(3, 0x8000, 0x4000);
    Actor_WalkTo(1, 144, 200);
    Actor_WalkTo(2, 144, 200);
    Actor_WalkTo(3, 144, 200);
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    Actor_WaitForMove(3);
    Actor_SetPosition(3, 0, 0);
    Event_End();
}

void SceneDialogue_RunLine1956(void)
{
    void Event_Begin(void);

    Event_Begin();
    Actor_SetAnimation(0, 1);
    Message_ShowCentered(MSG_HE_WHO_HAS_POWER_SEE, 1);
    Event_End();
}

void ConfigureAndPlaceActorFourteen(void)
{
    void Actor_SetPosition(s32, s32, s32);

    s32 a = 21, b = 9;
    Map_CopyCellAttributes(85, 9, 1, 1, a, b);
    MapObject_SetPosition(100, 0, 0);
    PlaceActor(14, 0x01580000, 0x00980000);
}

void FieldScene_RunScene3a2_020008a8(void)
{
    u32 i;
    s32 record;

    Map_CopyCellAttributes(21, 73, 1, 1, 21, 9);
    MapObject_SetPosition(100, -1, -1);
    Actor_SetPosition(14, 0, 0);
}

void SceneDialogue_RunActorFifteenByLeaderHeading(void)
{
    struct Slot020008e0 *Func_02001dee_a(s32);

    u32 heading = Func_02001dee_a(0)->heading;

    Event_Begin();
    if (heading - 0xA001 <= 0x3FFE) {
        Sanctum_Open(15);
    } else {
        Event_SetMessage(MSG_AM_TRAVELING_AROUND_WORLD_SPREAD);
        Event_ShowMessage(15, 0);
    }
    Event_End();
}

void Scene_RunEventTransition(void)
{
    u8 *record;
    s32 none;

    if (GameFlag_IsSet(0x89a) == 0) {
    } else {
        Event_Begin();
        Actor_SetPosition(10, 0x2180000, 0xd80000);
        Event_SetMessage(MSG_YAHH_SILK_ROAD_BOULDERS_BLOCK);
        Event_ShowMessageAndWait(10, 0, 20);
        Actor_RunRepeatedMotion(0, 2);
        Event_Wait(20);
        record = Func_02001e6e(0);
        *(s32 *)((s32)record + 108) = 0x2008055;
        record = Value1(Func_02001e78, 0);
        if ((*(s32 *)((s32)record + 16) >> 20) == 13) {
            Actor_WalkToAndWait(0, 0x1b8, 200);
        }
        Actor_SetSpeed(10, 0x20000, 0x10000);
        Actor_SetSpritePriority(10, 2);
        Actor_WalkToAndWait(10, 0x198, 216);
        {
            u8 *record = Func_02001eb4(10);
            u32 flag = 1;

            flag = flag | record[35];
            record[35] = (u8)flag;
        }
        Event_Wait(10);
        Actor_FaceDirection(10, 0x8000, 20);
        Event_ShowMessageAndWait(10, 0, 20);
        Actor_StartRepeatedMotion(10, 2);
        Actor_SetAttachedEffect(10, 0x102);
        Event_Wait(60);
        Event_ShowMessageAndWait(10, 0, 20);
        Value2(Engine_ActorEnableActionCallback, 10, 0x200962c);
        Camera_MoveTo(0x1280000, -1, 0x1580000, 1);
        GameFlag_Set(0x8b0);
        Func_02001f3c(10);
        Camera_WaitForMove();
        Actor_SetSpeed(0, 0x10000, 0x8000);
        Actor_EnableActionCallback(0, 0x20096b8);
        Func_02001f5c(0);
        Event_Wait(10);
        none = 0;
        record = Func_02001f50(0);
        *(s32 *)((s32)record + 108) = none;
        Event_Wait(30);
        Actor_RunRepeatedMotion(10, 2);
        Event_Wait(20);
        Actor_FaceDirection(10, 0x5000, 120);
        Actor_ShowEmote(10, 0x105, 60);
        Actor_ShowEmote(0, 0x101, 60);
        Actor_SetAnimationAndWait(10, 4);
        Event_Wait(20);
        Event_ShowMessageAndWait(10, 0, 20);
        Event_End();
    }
}

void Scene_RunActorCue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DO_DO_WARRIOR_SHOULD_RETURN);
    Actor_ShowEmote(10, 0x105, 60);
    Event_OpenMessage(10, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        Scene_AdvanceStep(1);
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(10, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Event_End();
}

void Scene_RunActorExchange(void)
{
    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(30);
    Actor_FaceDirection(0, 0x6000, 0);
    Camera_MoveToActor(9, 1);
    Camera_WaitForMove();
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_SetMessage(MSG_HSU_OKAY);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(10, 0xd000, 20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(60);
    Actor_ShowEmote(8, 0x102, 60);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_StartRepeatedMotion(10, 2);
    Actor_SetAttachedEffect(10, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(10, 0xb000, 20);
    Actor_SetAnimation(9, 5);
    Event_End();
    GameFlag_Set(0x8b1);
}

void Scene_RunActorSequence(void)
{
    void Event_Begin();

    s32 mask;

    Event_Begin();
    Actor_SetAttachedEffect(8, 0x102);
    Actor_StartRepeatedMotion(8, 2);
    Event_Wait(60);
    Event_SetMessage(MSG_ROBIN_DID_LIFT_BOULDER);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAttachedEffect(10, 0x102);
    Actor_Jump(10, 4, 0);
    Event_Wait(60);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(10, 0xd000, 20);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(20);
    Actor_WalkTo(8, 178, 0x114);
    Actor_WalkToAndWait(10, 172, 0x11c);
    Actor_WaitForMove(8);
    Actor_FaceDirection(8, 0x5000, 0);
    Actor_FaceDirection(10, 0xb000, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    mask = 254;
    Event_ShowMessageAndWait(8, 0, 20);
    *(u8 *)(Func_02002214(8) + 90) &= mask;
    *(u8 *)(Func_02002226_b(10) + 90) &= mask;
    Actor_SetSpeed(8, 0x3333, 0x1999);
    Actor_SetSpeed(10, 0x3333, 0x1999);
    Actor_SetAnimation(8, 5);
    Actor_SetAnimation(10, 6);
    Event_Wait(20);
    Audio_PlayCue(125);
    Actor_SetDestinationOffset(8, 2, 0);
    Actor_SetDestinationOffset(9, 2, 0);
    Actor_SetDestinationOffset(10, 2, 0);
    Actor_WaitForMove(10);
    Event_Wait(30);
    Actor_SetAnimation(8, 5);
    Actor_SetAnimation(10, 6);
    Event_Wait(20);
    Audio_PlayCue(125);
    Actor_SetDestinationOffset(8, 4, 0);
    Actor_SetDestinationOffset(9, 4, 0);
    Actor_SetDestinationOffset(10, 4, 0);
    Actor_WaitForMove(10);
    Actor_Stop(9);
    Actor_SetAnimation(8, 1);
    Actor_SetAnimation(10, 1);
    Event_Wait(50);
    Actor_Jump(10, 2, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_AskYesNo(8, 0);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(8, 5);
    Actor_SetAnimation(10, 6);
    Event_Wait(20);
    Audio_PlayCue(125);
    Actor_SetDestinationOffset(8, 2, 0);
    Actor_SetDestinationOffset(9, 2, 0);
    Actor_SetDestinationOffset(10, 2, 0);
    Actor_WaitForMove(10);
    Event_Wait(30);
    Actor_SetAnimation(8, 5);
    Actor_SetAnimation(10, 6);
    Event_Wait(20);
    Audio_PlayCue(125);
    Actor_SetDestinationOffset(8, 4, 0);
    Actor_SetDestinationOffset(9, 4, 0);
    Actor_SetDestinationOffset(10, 4, 0);
    Actor_WaitForMove(10);
    Event_Wait(40);
    Actor_SetAnimation(8, 1);
    Actor_SetAnimation(10, 1);
    Actor_Jump(10, 2, 0);
    Event_Wait(20);
    Actor_FaceDirection(10, 0xd000, 20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 30);
    Event_ShowMessageAndWait(8, 0, 20);
    {
        u8 *record = Func_02002408(10);
        u32 flag = 1;

        flag = flag | record[90];
        record[90] = (u8)flag;
    }
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkToAndWait(10, 168, 0x128);
    Actor_FaceDirection(10, 0xd000, 20);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Event_End();
    GameFlag_Set(0x8b2);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_RequestExit(6);
}

void FieldScene_RunScriptedSteps0And1A12(void)
{
    Event_Begin();
    Actor_SetAnimation(0, 1);
    Message_ShowCentered(MSG_NORTH_ALTIN_MINE_WEST_LAMA, 1);
    Event_End();
}

void FieldScene_RunPairedLayoutStepsThenSetOne(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Map_CopyCellsTo(5, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Map_CopyCellAttributes(5, 28, 1, 2, fifth, sixth);
    }
    Event_Wait(1);
}

void SceneState_RunRect6x28Step(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Map_CopyCellsTo(6, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Map_CopyCellAttributes(6, 28, 1, 2, fifth, sixth);
    }
    Event_Wait(1);
}

s32 SceneActor_SetFlagBitByRankAgainstActorZero(struct Actor02001060 *actor)
{
    if (Func_02002570(0)->rank > actor->rank) {
        actor->flags |= 2;
    } else {
        actor->flags &= 0xFD;
    }
}

void SceneActor_UpdateActorFourteenByDepth(void)
{
    struct Actor *current = Func_0200259e(0);
    struct Actor *other = Func_020025a6(14);

    if (current->f10 <= other->f10) {
        Actor_SetSpritePriority(14, 1);
    }
}

void ActorPresentation_PrepareActorFourteenWithCallback(void)
{
    u8 zero;

    zero = 0;
    Event_Begin();

    Func_020025cc(14)[35] &= 0xfd;
    Func_020025de(14)[89] &= 0xfd;
    Func_020025ec(14)[85] = zero;
    *(void **)(Func_020025f8(14) + 108) = (void *)0x02009061;

    Map_CopyCellAttributes(55, 16, 1, 1, 56, 18);
    Map_CopyCellAttributes(55, 16, 1, 1, 20, 18);

    Task_Wait(1);
    GameFlag_Set(512);
    Actor_SetSpritePriority(14, 2);
    Event_End();
}

void FieldScene_SetSlot15Byte89AndRunStep(void)
{
    u8 *slot;

    Event_Begin();
    {
        s32 fifth = 21;
        s32 sixth = 11;

        Map_CopyCellAttributes(14, 6, 1, 2, fifth, sixth);
    }
    slot = Func_0200266c(15) + 89;
    *slot = 254;
    GameFlag_Set(0x201);
    Event_End();
}

s32 SceneData_SelectTableByWord224(void)
{
    if (gGameState.scene == (s32)&Value_0000004a) {
        return (s32)Data_02009c9c;
    }
    return (s32)Data_02009b10;
}

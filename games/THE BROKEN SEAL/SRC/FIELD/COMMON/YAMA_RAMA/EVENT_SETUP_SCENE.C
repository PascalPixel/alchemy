#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgYamaAdeptsLetMeThankAgain[];
extern u8 MsgYamaAmTravelingAroundWorldSpread[];
extern u8 MsgYamaDidKnowMasterHamaGreatest[];
extern u8 MsgYamaDoDoWarriorShouldReturn[];
extern u8 MsgYamaDoKnowMeditation[];
extern u8 MsgYamaHeWhoHasPowerSee[];
extern u8 MsgYamaHsuOkay[];
extern u8 MsgYamaNorthAltinMineWestLama[];
extern u8 MsgYamaRobinDidLiftBoulder[];
extern u8 MsgYamaYahhSilkRoadBouldersBlock[];
extern u8 MsgYamaYoungWarriorsDoComeFrom[];



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

/* Tables laid out after the code. */
extern const u16 YamaRama_BoulderCells[];
extern const u16 YamaRama_BoulderCellsBack[];
extern const u8 YamaRama_HsuAction[];
extern const u8 YamaRama_LeaderAction[];

void *Object_GetById(u32 id);
void Object_RefreshSelectorById();
void BattleFx_PlayQueuedSound(void);
s32 SceneActor_SetFlagBitByRankAgainstActorZero(struct Actor02001060 *actor);
s32 OverlayObject_SetFacingTowardObject10(void *self);

/* Scene event steps for resource_3a2. */

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

void SceneDialogue_RunMessage1958Step(void)
{

    u8 *work;

    Event_Begin();
    Event_SetMessage((s32)MsgYamaYoungWarriorsDoComeFrom);
    Event_OpenMessage(10, 0);

    if (Event_ChooseYesNo(0, 0) == 1) {
        Event_Wait(20);
        Event_ShowMessage(10, 0);
    } else {
        work = (u8 *)gEventWork;
        *(u16 *)(work + 472) += 1;
        Event_AskYesNo(10, 0);
    }

    Event_End();
}

void SceneDialogue_RunActor11Message195d(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgYamaDoKnowMeditation);
    Event_AskYesNo(11, 0);
    Event_End();
}

void SceneDialogue_RunActor13Message1961(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgYamaDidKnowMasterHamaGreatest);
    Event_AskYesNo(13, 0);
    Event_End();
}

void FieldScene_RunPrimaryScript(void)
{
    Audio_PlayCue(188);
    Map_AnimateCells(YamaRama_BoulderCells, 67, 6);
    *(u8 *)(Object_GetById(0) + 85) = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    Event_Wait(16);
    Event_RequestExit(2);
}

void FieldScene_RunScene3a2SequenceA(void)
{
    void Event_ShowMessageAndWait();

    Event_Begin();
    Actor_SetPosition(8, 0x880000, 0xa80000);
    Actor_FaceDirection(8, 0x5000, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x900000, 0xc80000);
    Actor_SetPosition(ACTOR_GERALD, 0xa00000, 0xc00000);
    Actor_SetPosition(ACTOR_IVAN, 0x800000, 0xc80000);
    Actor_SetPosition(ACTOR_MIA, 0x700000, 0xc00000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(60);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_SetMessage((s32)MsgYamaAdeptsLetMeThankAgain);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 60);
    Event_Wait(120);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_FaceDirection(ACTOR_MIA, 0, 20);
        Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
        Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
        Actor_FaceDirection(ACTOR_IVAN, 0xe000, 0);
        Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
        Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
        Event_Wait(60);
        Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        bump_step(2);
    } else {
        bump_step(2);
        Event_Wait(20);
        Actor_FaceDirection(ACTOR_MIA, 0, 20);
        Actor_SetAnimationAndWait(ACTOR_MIA, 3);
        Event_Wait(20);
        Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
        Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
        Actor_FaceDirection(ACTOR_IVAN, 0xe000, 0);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    }
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    Event_Wait(20);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_FaceDirection(8, 0xc000, 30);
    Audio_PlayCue(188);
    Map_AnimateCells(YamaRama_BoulderCells, 67, 6);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 136, 136);
    Actor_SetPosition(8, 0, 0);
    Audio_PlayCue(188);
    Map_AnimateCells(YamaRama_BoulderCellsBack, 67, 6);
    Event_Wait(60);
    BattleFx_PlayQueuedSound();
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
    Event_AskYesNo(ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0xb000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0xe000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IVAN, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_IVAN, 128, 184);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_MIA, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_GERALD, 144, 200);
    Actor_WalkTo(ACTOR_IVAN, 144, 200);
    Actor_WalkTo(ACTOR_MIA, 144, 200);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Event_End();
}

void SceneDialogue_RunLine1956(void)
{
    void Event_Begin(void);

    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered((s32)MsgYamaHeWhoHasPowerSee, 1);
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

    u32 heading = ((struct Slot020008e0 *)Object_GetById(0))->heading;

    Event_Begin();
    if (heading - 0xA001 <= 0x3FFE) {
        Sanctum_Open(15);
    } else {
        Event_SetMessage((s32)MsgYamaAmTravelingAroundWorldSpread);
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
        Event_SetMessage((s32)MsgYamaYahhSilkRoadBouldersBlock);
        Event_ShowMessageAndWait(10, 0, 20);
        Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Event_Wait(20);
        record = Object_GetById(0);
        *(s32 *)((s32)record + 108) = (s32)OverlayObject_SetFacingTowardObject10;
        record = Value1(Object_GetById, 0);
        if ((*(s32 *)((s32)record + 16) >> 20) == 13) {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b8, 200);
        }
        Actor_SetSpeed(10, 0x20000, 0x10000);
        Actor_SetSpritePriority(10, 2);
        Actor_WalkToAndWait(10, 0x198, 216);
        {
            u8 *record = Object_GetById(10);
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
        Value2(Engine_ActorEnableActionCallback, 10, (s32)YamaRama_HsuAction);
        Camera_MoveTo(0x1280000, -1, 0x1580000, 1);
        GameFlag_Set(0x8b0);
        Object_RefreshSelectorById(10);
        Camera_WaitForMove();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        Actor_EnableActionCallback(ACTOR_PARTY_LEADER, YamaRama_LeaderAction);
        Object_RefreshSelectorById(0);
        Event_Wait(10);
        none = 0;
        record = Object_GetById(0);
        *(s32 *)((s32)record + 108) = none;
        Event_Wait(30);
        Actor_RunRepeatedMotion(10, 2);
        Event_Wait(20);
        Actor_FaceDirection(10, 0x5000, 120);
        Actor_ShowEmote(10, 0x105, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
        Actor_SetAnimationAndWait(10, 4);
        Event_Wait(20);
        Event_ShowMessageAndWait(10, 0, 20);
        Event_End();
    }
}

void Scene_RunActorCue(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgYamaDoDoWarriorShouldReturn);
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
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Camera_MoveToActor(9, 1);
    Camera_WaitForMove();
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_SetMessage((s32)MsgYamaHsuOkay);
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
    Event_SetMessage((s32)MsgYamaRobinDidLiftBoulder);
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
    *(u8 *)(Object_GetById(8) + 90) &= mask;
    *(u8 *)(Object_GetById(10) + 90) &= mask;
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
        u8 *record = Object_GetById(10);
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
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Event_End();
    GameFlag_Set(0x8b2);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_RequestExit(6);
}

void FieldScene_RunScriptedSteps0And1A12(void)
{
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered((s32)MsgYamaNorthAltinMineWestLama, 1);
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
    if (((struct Actor02001060 *)Object_GetById(0))->rank > actor->rank) {
        actor->flags |= 2;
    } else {
        actor->flags &= 0xFD;
    }
}

void SceneActor_UpdateActorFourteenByDepth(void)
{
    struct Actor *current = Object_GetById(0);
    struct Actor *other = Object_GetById(14);

    if (current->f10 <= other->f10) {
        Actor_SetSpritePriority(14, 1);
    }
}

void ActorPresentation_PrepareActorFourteenWithCallback(void)
{
    u8 zero;

    zero = 0;
    Event_Begin();

    ((u8 *)Object_GetById(14))[35] &= 0xfd;
    ((u8 *)Object_GetById(14))[89] &= 0xfd;
    ((u8 *)Object_GetById(14))[85] = zero;
    *(void **)(Object_GetById(14) + 108) = (void *)SceneActor_SetFlagBitByRankAgainstActorZero;

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
    slot = Object_GetById(15) + 89;
    *slot = 254;
    GameFlag_Set(0x201);
    Event_End();
}


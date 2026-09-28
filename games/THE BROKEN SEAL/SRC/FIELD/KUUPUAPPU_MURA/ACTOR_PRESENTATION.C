#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"


struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct SceneActor {
    u8 reserved_00[6];
    s16 temporary_state;
    u8 reserved_08[92];
    u16 presentation_flags;
};

struct SceneActor_02000cfc {
    u8 reserved_00[100];
    u16 presentation_flags;
};

struct SceneActor_02000d78 { u8 reserved_00[100]; u16 presentation_flags; };

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor_02000fb4 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor_02001010 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor_0200113c {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct OverlayEffectMotion {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[28];
    s32 horizontal_rate;
    s32 vertical_rate;
    s32 shadow_x;
    s32 shadow_y;
    s32 shadow_z;
    u8 pad44[32];
    s16 mode;
};

extern u8 *Data_03001e8c[];
extern u8 *gWork;

/* The scene's tables, laid out after the code. */
extern u8 KuupuappuMura_Scripts[];
extern u8 KuupuappuMura_Messages[];
extern u8 KuupuappuMura_Actors[];
extern u8 KuupuappuMura_ActorsFlag855[];
extern u8 KuupuappuMura_Extras[];
extern u8 KuupuappuMura_ExtrasFlag855[];

/* Map cell steps played as the leader arrives in each scene. */
extern const u16 KuupuappuMura_Scene5Cells[];
extern const u16 KuupuappuMura_Scene6Cells[];
extern const u16 KuupuappuMura_Scene7Cells[];
extern const u16 KuupuappuMura_Scene8Cells[];
extern const u16 KuupuappuMura_Scene9Cells[];
extern const u16 KuupuappuMura_Scene10Cells[];

u8 *Owner_GetState();
void BattlePlacement_UpdateTimedEntriesTwentyTimes(void);
void Djinn_Transfer(int, int, int, int);
s32 Owner_RecalculateStats(int);
void Party_RemoveOwnerRestored();
s32 PartyInventory_FindOwner(s32);

s32 SceneActor_CheckFacingAndRange();
void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay);
void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay);

/*
 * The wrapper helpers below pass their constants straight into the argument
 * registers. A direct call precomputes an expensive constant into a value the
 * compiler then shares with later uses in the same block.
 */
void SceneActor_RunActorCommandWithFlag91(s32 x);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

/*
 * A value-returning call sets r0 last of its arguments, so the callee must be
 * spelled as returning a value even where the result is unused.
 */
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Actor_SetSpeed(actor, horizontal, vertical);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Actor_SetPosition();

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Actor_SetPosition();

    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Actor_SetPosition();

    f(a0, a1, a2, a3);
}

/*
 * Per-frame callback for one actor record, installed into field +0x6c of
 * actors 14 and 15 by the overlay initialiser. Always returns 0. The work
 * pointer table holds the scene record at entry 0 and the scene work at
 * entry 12, the same pointer the rest of this overlay reads as gWork.
 */
s32 SceneActor_UpdateProximityToLeader(u8 *self)
{
    u8 **globals = Data_03001e8c;
    u8 *scene = globals[0];
    u8 *workspace = globals[12];        /* the scene work, gWork */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    /*
     * Bit 0 of the actor's own flag halfword selects which partner to test.
     * This must stay two calls rather than one call on a conditional
     * expression: the conditional form folds to arithmetic on the flag.
     */
    if ((*flags & 1) != 0) {
        partner = Actor_Get(15);
    } else {
        partner = Actor_Get(14);
    }
    if (SceneActor_CheckFacingAndRange(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Actor_Get(ACTOR_PARTY_LEADER);

    /*
     * Widen the range when the scene counter at workspace + 376 is already
     * running, or when the scene byte at scene + 0x0ea4 is set.
     */
    if (*(s16 *)(workspace + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    SceneActor_CheckFacingAndRange(self, player, range, force);
    return 0;
}

s32 ActorPresentation_UpdateEntityFromLeader(u8 *entity)
{
    u8 *base = Data_03001e8c[0];
    u8 *workspace = Data_03001e8c[12];
    s32 flag = 0;
    s32 selector = 18;
    u8 *leader;

    if (*(s32 *)(entity + 56) == (s32)0x80000000)
        return 0;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (*(s16 *)(workspace + 376) != 0 || base[0x0ea4] != 0) {
        selector = 26;
        flag = 1;
    }
    SceneActor_CheckFacingAndRange(entity, leader, selector, flag);
    return 0;
}

void *SceneData_GetScriptTable(void)
{
    return KuupuappuMura_Scripts;
}

int SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetMessageTable(void)
{
    return KuupuappuMura_Messages;
}

void *SceneData_SelectActorTableByFlag855(void)
{
    if (GameFlag_IsSet(0x855) != 0)
        return KuupuappuMura_ActorsFlag855;
    return KuupuappuMura_Actors;
}

int OverlayObject_GetObject2Byte280(void)
{
    return Owner_GetState(2)[280];
}

/*
 * The flagged path passes the result of its final call back to the caller, so
 * this is spelled as a tail call and the return type is s32. The
 * fall-through path returns nothing.
 */
s32 OverlayObject_RunObject2WhenFlagged(void)
{
    BattlePlacement_UpdateTimedEntriesTwentyTimes();
    if ((*(u32 *)(Owner_GetState(2) + 248) & 1) != 0) {
        Djinn_Transfer(2, 0, 0, 0);
        Audio_PlayCue(126);
        Owner_RecalculateStats(0);
        return Owner_RecalculateStats(2);
    }
}

#include "TYPES.H"

enum ActorPresentationMessage {
    MSG_ROBIN_PEERED_INTO = 0x947,
    MSG_WASNT_ERUPTION_MT_ALEPH_INCREDIBLE = 0x1223,
    MSG_THOSE_TRAVELERS_LEFT_IN_BIG = 0x1229,
    MSG_ACCUSING_US_STEALING_HAMMETS_TREASURED = 0x122f,
    MSG_OFF_ON_ADVENTURE = 0x1232,
    MSG_RUFF_RRRUFF = 0x1235,
    MSG_MAN_SHOULD_STEAL_FROM_ANOTHER = 0x1239,
    MSG_GROUP_TRAVELERS_WAS_STRANGE_BUNCH = 0x123b,
    MSG_BET_WAS_THOSE_THREE_CREEPS = 0x123c,
    MSG_SUPPOSE_DOESNT_MATTER_HOW_RICH = 0x123d,
    MSG_EVERYONE_KNOWS_THOSE_THREE_AT = 0x123e,
    MSG_STEALING_IN_MIDST_VOLCANIC_ERUPTION = 0x1241,
    MSG_LEAVING_IM_STILL_WORRIED_ABOUT = 0x1327,
    MSG_WAIT_DONT_WANT_TAKE_YOUR = 0x132a,
    MSG_YOURE_HITTING_ROAD_AGAIN = 0x1330,
    MSG_TALKING_ABOUT_HAMMETS_SERVANT_IVAN = 0x1336,
    MSG_THOSE_MEN_CAPTURED_THEYRE_IN = 0x133c,
    MSG_ONES_WHO_CAPTURED_THIEVES = 0x133f,
    MSG_RUFF_RRRUFF_2 = 0x1342,
    MSG_WITH_ROAD_OUT_ONLY_WAY = 0x1348,
    MSG_WHERE_DID_IVAN_GO_BY = 0x1349,
    MSG_MUST_STRONGER_THAN_LOOK_HAVE = 0x134b,
    MSG_IF_BRING_ME_BONE_ILL = 0x134e,
    MSG_THANK_FOR_OTHER_DAY_LEAVING = 0x137f,
    MSG_CAREFUL_SEARCH_WILL_REVEAL_PASSAGE = 0x13ab,
    MSG_HE_SHOULD_BE_FIXING_ROOF_NOW = 0x12c0,
    MSG_CAN_HEAR_WATER_RUMBLING_DOWN = 0x29dc
};


/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

void FieldScene_RunScene382_020004a0(void)
{
    s32 record;
    struct EventWork *p5;

    p5 = gEventWork;
    if (GameFlag_IsSet(0x855) != 0 || GameFlag_IsSet(0x856) == 0) {
        Event_RequestExit(p5->touched_trigger - 19);
        return;
    }
    Event_Begin();
    record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    if (p5->touched_trigger == 20) {
        Actor_WalkToAndWait(ACTOR_IVAN, 0x190, 0x1c0);
    } else {
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(0xe00000, -1, 0xa20000, 1);
        Actor_WalkToAndWait(ACTOR_IVAN, 224, 162);
        Camera_WaitForMove();
    }
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Event_Wait(20);
    Event_SetMessage(MSG_LEAVING_IM_STILL_WORRIED_ABOUT);
    Event_ShowMessageAndWait(0x9002, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    if (Value0(OverlayObject_GetObject2Byte280)!= 0) {
        Event_SetMessage(MSG_WAIT_DONT_WANT_TAKE_YOUR);
        Event_ShowMessage(ACTOR_IVAN, 0);
        OverlayObject_RunObject2WhenFlagged();
        Task_Wait(20);
    }
    Party_RemoveOwnerRestored(2);
    Event_RequestExit(p5->touched_trigger - 19);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void SceneState_SetFlags947And29dc(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_PEERED_INTO, 1);
    Message_ShowCentered(MSG_CAN_HEAR_WATER_RUMBLING_DOWN, 1);
    Event_End();
}

void *SceneData_SelectTableA414ByFlag855(void)
{
    if (GameFlag_IsSet(0x855) != 0)
        return KuupuappuMura_ExtrasFlag855;
    return KuupuappuMura_Extras;
}

void SceneDialogue_RunActor9LineAndAdvance(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_WASNT_ERUPTION_MT_ALEPH_INCREDIBLE);
    SceneActor_ApplyActorCueThenWait(9, 0, 2);
    Event_OpenMessage(9, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Event_End();
}

void ActorPresentation_RunActorThirteenSceneSetup(void)
{
    u8 *workspace;

    Event_Begin();
    Event_SetMessage(MSG_THOSE_TRAVELERS_LEFT_IN_BIG);
    Actor_SetAnimation(13, 1);
    SceneActor_ApplyActorCueThenWait(13, 0, 2);
    Event_OpenMessage(13, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        workspace = gWork;
        ++*(u16 *)(workspace + 472);
    }
    Event_ShowMessage(13, 0);
    Event_End();
}

void ActorPresentation_RunActorSeventeenSceneSetup(void)
{
    int Event_ChooseYesNo(int, int);

    u8 *workspace;

    Event_Begin();
    Event_SetMessage(MSG_ACCUSING_US_STEALING_HAMMETS_TREASURED);
    SceneActor_ApplyActorCueThenWait(17, 0, 2);
    Event_OpenMessage(17, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        workspace = gWork;
        ++*(u16 *)(workspace + 472);
    }
    Event_ShowMessage(17, 0);
    Event_End();
}

void ActorPresentation_RunActorEighteenSceneSetup(void)
{
    u8 *workspace;

    Event_Begin();
    Event_SetMessage(MSG_OFF_ON_ADVENTURE);
    SceneActor_ApplyActorCueThenWait(18, 0, 2);
    Event_OpenMessage(18, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        workspace = gWork;
        ++*(u16 *)(workspace + 472);
    }
    Event_ShowMessage(18, 0);
    Event_End();
}

void SceneDialogue_RunActor11Line(void) { Engine_EventBegin(); Engine_EventSetMessage(0x1227); SceneActor_RunActorStep(11); Engine_EventEnd(); }

void SceneDialogue_RunActor16Line(void) { Engine_EventBegin(); Engine_EventSetMessage(0x122e); SceneActor_RunActorStep(16); Engine_EventEnd(); }

void SceneDialogue_RunActor19Line(void)
{
    void Event_ShowMessage(int, int);

    Event_Begin(); Event_SetMessage(MSG_RUFF_RRRUFF); Actor_SetAnimation(19, 0);
    SceneActor_ApplyActorCueThenWait(19, 0, 2); Event_ShowMessage(19, 0); Event_End();
}

void ActorPresentation_RunActorFourteenDialogue(void)
{
    void Actor_SetAnimation(s32, s32);

    struct SceneActor *actor = Actor_Get(14);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Event_Begin();
    Engine_EventSetMessage(0x122c);
    Actor_SetAnimation(14, 0);
    SceneActor_ApplyActorCueThenWait(14, 0, 2);
    SceneActor_ApplyActorZeroThenWait(14, 10);
    actor->temporary_state = saved;
    Task_Wait(1);
    Event_End();
    actor->presentation_flags &= 1;
}

void ActorPresentation_RunActorFifteenDialogue(void)
{
    struct SceneActor *actor = Actor_Get(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Event_Begin();
    Engine_EventSetMessage(0x122d);
    Actor_SetAnimation(15, 0);
    SceneActor_ApplyActorCueThenWait(15, 0, 2);
    SceneActor_ApplyActorZeroThenWait(15, 10);
    actor->temporary_state = saved;
    Task_Wait(1);
    Event_End();
    actor->presentation_flags &= 1;
}

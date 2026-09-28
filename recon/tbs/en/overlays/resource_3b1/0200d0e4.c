/* Draft: FieldScene_RunPositionTransferPresentation, resource_3b1 at 0x0200d0e4 (listing 0x020050e4).
 * Not linked: its message id loads from the literal pool as a link-time constant (Data_00001e46) that the main image does not define; spelled as a plain constant GCC builds it differently.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"

enum ExtendedChoreographyMessage {
    MSG_WONDER_COULD_HAVE_HAPPENED = 0x1d26,
    MSG_ITS_TOO_LATE_HIRE_MERCENARIES = 0x1d30,
    MSG_BUT_WE_CANT_SEND_SHIP = 0x1d31,
    MSG_LONGER_WE_SIT_HERE_MORE = 0x1d4e,
    MSG_IF_WE_ARENT_GOING_SET = 0x1d56,
    MSG_NOW_WANT_SEE_CAPTAIN_TOO = 0x1d91,
    MSG_YOURE_TRYING_LAUNCH_SHIP = 0x1d93,
    MSG_BAD_LUCK_LOSING_MY_LUCKY = 0x1dcd,
    MSG_IF_SHIP_FROM_TOLBI_HAD = 0x1dd4,
    MSG_ITS_MY_LUCKY_ANCHOR = 0x1ddb,
    MSG_WE_DONT_KNOW_MIGHT_HAPPEN = 0x1e06,
    MSG_THESE_PROUD_WARRIORS_NOT_GOING = 0x1e13,
    MSG_OUR_REPLACEMENT_NEVER_ARRIVED_BUT = 0x1e27,
    MSG_CAST_OFF = 0x1e3b,
    MSG_ROW_THOSE_OARS = 0x1e3c,
    MSG_WERE_OFF = 0x1e3d,
    MSG_IM_TURNING = 0x1e43,
    MSG_HEY_ARE_YOU_OK = 0x1e6e,
    MSG_OHHHH_NOOOO_GOING_MAKE_ME = 0x1e81,
    MSG_HA_HA_HA_ROWING_FEEL = 0x1e84,
    MSG_GIVES_ME_CHILLS_THINK_COULD = 0x1ea1,
    MSG_ROBIN_YOUVE_GOT_GOOD_EYE = 0x1ea2,
    MSG_HO_HO_PERSON_GOING_GET = 0x1ea6,
    MSG_OARSMAN_WAS_INJURED = 0x1eb2,
    MSG_WONDER_WHATS_WRONG_SHIP_SHOULDNT = 0x1ec1,
    MSG_THING_HAS_KAJA_HIS_MEN = 0x1ece,
    MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING = 0x1ecf,
    MSG_SHIP_STARTING_LIST_IF_WE = 0x1ed0,
    MSG_HOW_MANY_MONSTERS_OUT_THERE = 0x1ed1,
    MSG_ANOTHER_MONSTER_ISNT_FIRST_CLASS = 0x1ed2,
    MSG_DONT_CARE_TAKES_JUST_HURRY = 0x1edb,
    MSG_IF_THOSE_MONSTERS_COME_BACK = 0x1edc,
    MSG_BOATS_ROCKING_MUCH_IM_CERTAIN = 0x1edd,
    MSG_HAD_IDEA_THERE_WERE_MANY = 0x1ede,
    MSG_WERE_SURROUNDED_BY_MONSTERS_STILL = 0x1edf,
    MSG_HATE_ARGUING = 0x1f48,
    MSG_SORRY_EVERYONE_BUT_WE_NEED = 0x1f78,
    MSG_IM_SPREADING_GOODWILL_WHEREVER_TRAVEL = 0x1f7b,
    MSG_SHIPS_CREW_READY_FOR_ANYTHING = 0x1f7d,
    MSG_SHIPS_CREW_READY_FOR_ANYTHING_2 = 0x1f7f,
    MSG_GOOD_SHIP_HAS_ARRIVED_SAFELY = 0x1f81
};

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};
extern u8 Data_00001e46[];
extern u8 Data_0200e7c8[];
s32 Func_0200b642();
s32 Func_0200b656();
s32 Func_0200b66a();
void Func_0200b6b2();
void Func_0200b7b8();
void Func_0200b7ee();

/* Scene-step dispatcher for overlay resource 0x3b1.
 *
 * The owner takes a step selector plus two step parameters and jumps through a
 * 26-entry table into one bounded block of scene setup calls per step. Step 18
 * finishes by re-entering the dispatcher with step 14, which the compiler turns
 * into a jump back to the range check.
 *
 * Uncertain: the roles of the two parameters differ per step (actor slot,
 * count, flag, upper loop bound), so they keep neutral names here. The record
 * fields written at +6 (halfword) and +24 (word) are the same scene-object
 * fields the neighbouring scene sources touch; their meaning is not recovered.
 */

/* Loader-relocated overlay calls: each Func_ symbol names the pre-relocation
 * call word the image holds.
 *
 * Three of those pre-relocation words repeat in this owner while reaching
 * different runtime helpers (0x0200af5a, 0x0200b0e8 and 0x0200b20c each cover
 * two distinct destinations), so one Func_ spelling cannot name both sites.
 * Those six sites are declared by their runtime address instead, which the
 * overlay symbol resolver binds directly. Registering this owner as a
 * translation unit with explicit absolute_symbols would let them go back to
 * suffixed Func_ spellings without changing a byte. */

/* The scene work record pointer; +0x1c0 holds the scene request word. */

/*
 * Actor slot search for resource_3b1.  The 48-byte owner at 0x02005038 has no
 * pool; the halfword at 0x02005066 is alignment before the next owner.
 */

/*
 * Field scene beat for overlay resource_3b1.  Each callee is named for its own
 * call site: every call reaches its target through its own local veneer, even
 * where the same logical callee is used from more than one site.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void FieldScene_CallPairWith10(s32 a, u16 b);

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void FieldScene_RunStepThen10(s32 a);
void FieldScene_CallPairWith10(s32 a, u16 b);
void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags);
void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt);

/* Sets up three actor slots (2, 1, 3) with position/pose data pulled from a
 * per-slot lookup record (fields at +8 and +16), then drives a chain of
 * actor animation, camera, and text/dialog calls for the scene. */
void FieldScene_RunPositionTransferPresentation(void)
{
    u32 i;
    /* Per-slot lookup record; fields at +8 and +16 feed the setup call. */
    s32 record;
    s32 base5_1e46;
    s32 base5_200e7c8;

    ConfigureSceneMotionFlags(0x1b80000, -1, 0xb00000, 0x1000001);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1b80000, 0x860000);
    Event_OpenScreen(); /* main:0808a360 */
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 5);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 152);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x1b0, 166);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    record = Value1(Func_0200b642, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_0200b656, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_0200b66a, 1);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Task_Wait(1); /* main:080000c0 */
    Actor_SetSpeed(ACTOR_IVAN, 0x19999, 0xcccc);
    Actor_WalkTo(ACTOR_IVAN, 0x1a8, 152);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkTo(ACTOR_GERALD, 0x1c0, 168);
    Actor_SetSpeed(ACTOR_MIA, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_MIA, 0x1ca, 152);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 40);
    Func_0200b7ee(10); /* main:0808a1d8 */
    /* Text/dialog resource pointer, passed by base address and by base+1. */
    base5_1e46 = (s32)Data_00001e46;
    Func_0200b6b2(base5_1e46, 1, 10); /* main:08015210 */
    Event_Wait(10);
    FieldScene_RunSceneStep(0, 0, 40);
    Value3(FieldScene_RunSceneStep, 1, 0x4000, 20);
    Camera_SetSpeed(0x39999, 0x7333); /* main:0808a208 */
    Call4(ConfigureSceneMotionFlags, 0x1b80000, -1, 0x1400000, 0x10000014);
    Actor_RunRepeatedMotion(8, 2); /* main:0808a138 */
    Call2(FieldScene_CallPairWith10, 8, 0xd000);
    Event_SetMessage((base5_1e46 + 1)); /* main:0808a170 */
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, 0, 20);
    Call4(ConfigureSceneMotionFlags, 0x1b80000, -1, 0x860000, 0x10000000);
    /* Second text/dialog resource pointer, shared across three calls. */
    base5_200e7c8 = (s32)Data_0200e7c8;
    Actor_EnableActionCallback(ACTOR_GERALD, base5_200e7c8);
    Value2(Engine_ActorEnableActionCallback, 2, base5_200e7c8);
    Func_0200b7b8(3, base5_200e7c8); /* main:0808a0b0 */
    Event_Wait(40);
    GameFlag_Set(0x301);
    FieldScene_RunSceneStep(23, 0, 0);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
}

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 *Data_03001ebc;
s32 ShindenHeya_MatchLeaderPriority();

/* Calls use this overlay's loader veneers. The early long branch shares
 * the dialogue tail and epilogue; the two timing loops each run six times. */

#define SCENE_REQUEST (*(u32 *)(Data_03001ebc + 0x1c0))
#define SCENE_SETUP_WORD (*(u32 *)(Data_03001ebc + 0x1c8))
#define SKIP_BEATS (*(u16 *)(Data_03001ebc + 0x1d8))

void ShindenHeya_CopyActorPose();
void ShindenHeya_SpawnOwnerEffect();
void FieldScene_RunPairedActorChoreography();
u8 *Object_GetById();
void ObjectMotion_WaitForAnimationChange();
void AudioCommand_WaitForCompletion();

/* The sibling actor-update script passes repeated large constants through
 * these inline call forms, keeping each call's argument evaluation local. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3_scene_primary_script(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

static __inline__ void bump_step(void)
{
    u8 *work = Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + 1);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#include "TYPES.H"

#include "FACING_OBJECT.H"

s16 CalculateFacingAngle(s32, s32);
struct FacingObject *ResolveFacingObject(s16);

#include "TYPES.H"

/*
 * Resource 378 scene reset at 0x020006e8(100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */

#include "TYPES.H"

/* Resource 378 object reset at 0x02002660(28 bytes including alignment). */

/* Publish the scene's upper prompt and lower dialogue panel. */

#include "TYPES.H"

/*
 * Ellipse orbit step for resource_378.  The object is offset from its anchor
 * along two axes and its angle advanced once per call.
 */

/*
 * The anchor at +104 supplies the centre; the result is published to the
 * object's +8/+16 and to its +56/+64 shadow pair, with +56 taken from a fresh
 * read of +8 rather than from x.  The two imports take the same angle and form
 * a cosine/sine pair; which is which is not settled.  The radii 14 and 10 and
 * the +100/+102 displacements are built from immediates.  The angle at +100 and
 * its step at +102 are separate halfwords, not one 32-bit field.
 */

#include "TYPES.H"

typedef struct {
    u8 pad_to_angle[6];
    u16 angle;
} ActorState;

ActorState *GetActorState(s32 actor_id);

#include "TYPES.H"

/* Close through scene 8 when facing inward; otherwise select the story line. */

#include "TYPES.H"

/* Close scene 8 when facing inward; otherwise choose its story line. */
/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
/* Close scene 8 when facing inward; otherwise emit its fixed story line. */

#include "TYPES.H"

void ShindenHeya_SpawnActorSpark();

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; a single word can serve two sites with different
 * targets. Where a macro names an engine function, that is the function the
 * site reaches through the overlay veneer and the main-image veneer island,
 * keeping the site's own calling form. Names without a binding in the
 * repository are provisional.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight
 * into the argument registers, while a direct call precomputes a costly
 * constant into a local that later uses in the block share. A call that
 * returns a value sets r0 last of its arguments; the Value wrappers spell
 * those sites, and the result is sometimes unused.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */

static __inline__ void Call2_scene_primary_script(void (*f)(), s32 a0, s32 a1)
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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    return f(a0, a1, a2, a3);
}

/*
 * Runs actor nine's flag-branched dialogue. The 112-byte owner includes its
 * five pool words. The scene selector is the signed halfword at
 * Data_02000240 + 450, reached as index 225. The last two calls must stay
 * after the selector test: the epilogue pops the return address into r0, so
 * the second call's result is discarded there.
 */

/*
 * The sibling path to the dialogue above, reading the same selector
 * halfword. The 116-byte owner includes its five pool words.
 */

/*
 * Steps actor ten and, when the check passes, increments the same workspace
 * +472 halfword the preceding owner writes. The 88-byte owner includes its
 * two pool words.
 */

/*
 * Steps a fixed sequence of actor position, pose and timing calls over
 * slots 0, 1, 8, 9, 10, 11, 12 and 13, including one loop that nudges a
 * pair of per-actor record fields down 32 times.
 */

/*
 * Dispatches on the scene selector Data_02000240[225] over the range 10 to
 * 50, through a 41-entry jump table. The epilogue pops the return address
 * into r0, so no result survives it and the owner is void; the 296-byte
 * owner covers dispatcher, table, case bodies and literal pool. The default
 * arm doubles as the shared tail, so the arms that fall into it break while
 * the 20/21/50 arm returns instead.
 */

#include "TYPES.H"

enum FacingGatedMessage {
    MSG_WIELDERS_PSYNERGY_CALLED_ADEPTS_ADEPTS = 0x1035,
    MSG_WE_HAD_IDEA_TRUE_SANCTUM = 0x1138,
    MSG_ROBIN_WILL_ACCEPT_RESPONSIBILITY_FOR = 0x1162,
    MSG_ARE_YOU_SURE = 0x1164,
    MSG_ONCE_STEP_OUTSIDE_VILLAGE_CANNOT = 0x116c,
    MSG_ACCEPT_ROBIN_CANT_MEAN = 0x1171,
    MSG_MY_CONTROL_OVER_PSYNERGY_HAS = 0x119d,
    MSG_DO_FEEL_ANY_CHANGE_IN = 0x119f,
    MSG_WE_WILL_HELP_ANYTIME_AS = 0x1288,
    MSG_HEALER_MUST_WORRIED_ABOUT_NEVER = 0x1289,
    MSG_WONDER_IF_EVER_SEE_OUR = 0x128b,
    MSG_WAS_HAND_FATE_RETURNED_GOLD = 0x1376,
    MSG_WHEN_STRAY_FROM_YOUR_WORLDLY = 0x1377,
    MSG_CHILD_HAS_AWAKENED_OUR_TEACHINGS = 0x1379,
    MSG_AM_STARTING_FEEL_ONLY_BEGINNING = 0x1408,
    MSG_CURSE_MAY_OVER_BUT_WE = 0x171c,
    MSG_CAME_XIAN_FROM_VERY_DISTANT = 0x1823,
    MSG_WAS_AFTER_EERIE_NIGHT_WHEN = 0x190a,
    MSG_SAVED_ALTIN_FROM_MONSTERS_CLEARLY = 0x1951,
    MSG_PATH_SOL_SANCTUM_STILL_CLOSED = 0x1bfc,
    MSG_ROBIN_YOUR_NEW_FRIENDS_ADEPTS = 0x1bfd,
    MSG_MAY_WRONG_BUT_LATELY_THERE = 0x1ce8,
    MSG_POLISHED_GOLD_STATUE_RETURNED_US = 0x1ce9,
    MSG_DIRTY_GOLDEN_STATUE_CLEANED_UP = 0x1ceb
};

void FieldScene_RunScriptedSceneSequence(void)
{
    u32 i;

    Event_Begin();
    if (GameFlag_IsSet(0x201) != 0) {
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, 8, 0);
        Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(0xb, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(0xa, ACTOR_PARTY_LEADER, 0);
        Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
        Camera_WaitForMove();
        SCENE_REQUEST = 0x100;
        SCENE_SETUP_WORD = 0x40;
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(0x78);
        goto dialogue;
    }

    ColorBuffer_ApplyTarget(0x10002, 0);
    ColorBuffer_Interpolate(1);
    Event_Wait(1);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Camera_WaitForMove();
    SCENE_REQUEST = 0x209;
    Event_OpenScreen();
    Event_WaitForScreen();
    FieldScene_RunPairedActorChoreography();
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(0x3c);
    Event_Wait(0x64);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(0x1e);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0xc, 0);
    Actor_FaceActor(ACTOR_GERALD, 0xc, 0);
    Event_Wait(0x14);
    Actor_EnableActionCallback(8, 1);
    Actor_EnableActionCallback(0xc, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(0xb, 0xcccc, 0x6666);
    Actor_SetSpeed(0xc, 0xcccc, 0x6666);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(0xa, 0xcccc, 0x6666);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Event_Wait(0x1e);
    Actor_RunRepeatedMotion(0xc, 2);
    Event_SetMessage(MSG_WE_HAD_IDEA_TRUE_SANCTUM);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0xa);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(0x1e);
    Actor_SetAnimationAndWait(0xc, 3);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(0xb, 2);
    Event_Wait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0xb, 0);
    Actor_FaceActor(ACTOR_GERALD, 0xb, 0);
    Event_Wait(0x14);
    Event_OpenMessage(0xb, 0);
    /* Each arm advances once, on its own side of the object-state call. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(0x14);
        Actor_SetAnimationAndWait(0xb, 3);
        Event_Wait(0x14);
        Event_ShowMessage(0xb, 0);
        SKIP_BEATS++;
    } else {
        Event_Wait(0x14);
        Actor_SetAnimationAndWait(0xb, 4);
        Event_Wait(0x14);
        SKIP_BEATS++;
        Event_ShowMessage(0xb, 0);
    }
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    Actor_FaceActor(0xb, 9, 0);
    Event_Wait(0x14);
    Event_OpenMessage(9, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(0x14);
        Actor_SetAnimationAndWait(9, 3);
        Event_Wait(0x14);
        Event_ShowMessage(9, 0);
        SKIP_BEATS++;
    } else {
        Event_Wait(0x14);
        Actor_SetAnimationAndWait(9, 4);
        Event_Wait(0x14);
        SKIP_BEATS++;
        Event_ShowMessage(9, 0);
    }
    Event_Wait(0x14);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(0xb, 3);
    Actor_SetAnimation(0xc, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(0xa, 3);
    Event_Wait(0x1e);
    Actor_ShowEmote(0xc, 0x101, 0);
    Event_Wait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Event_Wait(0x14);
    Actor_WalkToAndWait(0xc, 0xe0, 0x78);
    Event_Wait(0xa);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_Wait(0xa);
    Actor_RunRepeatedMotion(0xb, 1);
    Event_Wait(0xa);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x1e);
    Actor_RunRepeatedMotion(8, 3);
    Actor_ShowEmote(8, 0x100, 0);
    Event_Wait(0x3c);
    Actor_FaceActor(8, 0xc, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_FaceDirection(0xc, 0x5000, 0);
    Event_Wait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(0xc, 3);
    Event_Wait(0xa);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0xa);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x14);
    Actor_FaceDirection(8, 0x3000, 0);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_FaceDirection(0xc, 0x5000, 0);
    Event_Wait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Event_Wait(0x32);
    Actor_ShowEmote(0xc, 0x101, 0);
    Event_Wait(0x28);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Event_Wait(0x3c);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0xa);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(0xb, 0x100, 0);
    Actor_ShowEmote(0xc, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_ShowEmote(0xa, 0x100, 0);
    Event_Wait(0x3c);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(0xa);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0xa);
    Actor_ShowEmote(0xc, 0x102, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(0xb, 1);
    Actor_StartRepeatedMotion(9, 1);
    Actor_RunRepeatedMotion(0xa, 1);
    Event_Wait(0xa);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0x1e);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(0xb, ACTOR_PARTY_LEADER, 0);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(0x3c);
    Actor_RunRepeatedMotion(0xc, 2);
    Event_Wait(0x14);
    Actor_FaceActor(0xc, 8, 0);
    Event_Wait(0x14);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x1e);
    Actor_RunRepeatedMotion(0xb, 2);
    Event_Wait(0x14);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Event_Wait(0x14);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x14);
    Event_Wait(0x28);
    Actor_RunRepeatedMotion(8, 2);
    Actor_SetAnimation(8, 0);
    Actor_SetChildValue(8, 0x100);
    ShindenHeya_CopyActorPose();
    Audio_PlayCue(0xc4);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Event_Wait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Event_Wait(0x10);
    for (i = 0; i < 6; i++) {
        ShindenHeya_SpawnOwnerEffect(8, 0x1200);
        Event_Wait(8);
    }
    Event_Wait(8);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Event_Wait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Event_Wait(0x60);
    Event_Wait(0x20);
    Actor_SetChildValue(8, 0);
    Event_Wait(0x1e);
    SceneState_ResetObject14Word108();
    Actor_SetAnimation(8, 1);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_SetAttachedEffect(0xc, 0x102);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(0xb, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(0xa, 0x102);
    Event_Wait(0x3c);
    Actor_RunRepeatedMotion(0xa, 1);
    Event_Wait(0x14);
    Event_ShowMessage(0xa, 0);
    Event_Wait(0x14);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Event_Wait(0x3c);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x1e);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Event_Wait(0x14);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_FaceEachOther(9, 0xa, 0);
    Event_Wait(0x14);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(0xa, 3);
    ObjectMotion_WaitForAnimationChange(0xa);
    Event_Wait(0x1e);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 0xb, 0);
    Actor_FaceEachOther(ACTOR_GERALD, 0xc, 0);
    Event_Wait(0x14);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(0xb, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(0xc, 3);
    ObjectMotion_WaitForAnimationChange(0xc);
    Event_Wait(0x3c);
    Actor_RunRepeatedMotion(8, 2);
    Actor_SetAnimation(8, 0);
    Actor_SetChildValue(8, 0x100);
    ShindenHeya_CopyActorPose();
    Audio_PlayCue(0xc4);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Event_Wait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_Wait(0x10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(0xb, 1);
    Actor_StartRepeatedMotion(0xc, 1);
    Actor_StartRepeatedMotion(9, 1);
    Actor_StartRepeatedMotion(0xa, 1);
    for (i = 0; i < 6; i++) {
        ShindenHeya_SpawnOwnerEffect(8, 0x1200);
        Event_Wait(8);
    }
    Event_Wait(8);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Event_Wait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Event_Wait(0x80);
    Actor_SetChildValue(8, 0);
    Event_Wait(0x1e);
    SceneState_ResetObject14Word108();
    Actor_SetAnimation(8, 1);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(0x1e);
    Actor_ShowEmote(0xc, 0x105, 0);
    Event_Wait(0x14);
    Actor_FaceEachOther(9, 0xa, 0);
    Event_Wait(0x28);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(0xb, 2);
    Event_Wait(0x14);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x14);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Actor_StartRepeatedMotion(9, 1);
    Actor_StartRepeatedMotion(0xa, 1);
    Actor_FaceEachOther(9, 0xa, 0);
    Actor_SetAttachedEffect(0xc, 0x102);
    Event_Wait(0x3c);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_Wait(0x14);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(0xc, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Event_Wait(0x3c);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(0xb, 3);
    Actor_SetAnimation(0xc, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(0xa, 3);
    ObjectMotion_WaitForAnimationChange(0xa);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(0xc, 1);
    Event_Wait(0x14);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x14);
    Actor_FaceActor(8, 0xc, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x32);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x1e);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(0xb, 0x102, 0);
    Actor_ShowEmote(0xc, 0x102, 0);
    Actor_ShowEmote(9, 0x102, 0);
    Actor_ShowEmote(0xa, 0x102, 0);
    Event_Wait(0x50);
    Actor_RunRepeatedMotion(0xb, 1);
    Event_Wait(0x14);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0x14);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(0xc, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Event_Wait(0x50);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(0xb, 0x102);
    Actor_SetAttachedEffect(0xc, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(0xa, 0x102);
    Event_Wait(0x3c);
    Actor_SetAnimationAndWait(0xc, 4);
    Event_Wait(0x14);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x28);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Actor_FaceEachOther(9, 0xa, 0);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Event_Wait(0x3c);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(0xb, 1);
    Event_Wait(0x14);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x3c);
    Actor_WalkToAndWait(8, 0xc8, 0x88);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Actor_FaceEachOther(8, ACTOR_GERALD, 0);
    Event_Wait(0x28);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x32);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Event_Wait(0x28);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x32);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x1e);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Event_Wait(0x3c);
    Actor_SetAttachedEffect(0xb, 0x102);
    Actor_RunRepeatedMotion(0xb, 1);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x1e);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x1e);
    Actor_ShowEmote(0xc, 0x102, 0);
    Actor_StartRepeatedMotion(0xc, 1);
    Event_Wait(0x14);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x1e);
    Actor_WalkToAndWait(8, 0xa8, 0x78);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceDirection(8, 0xd000, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x1e);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Event_Wait(0x3c);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0xa);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(0x14);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(0x1e);
dialogue:
    Event_SetMessage(MSG_ROBIN_WILL_ACCEPT_RESPONSIBILITY_FOR);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(0x14);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        FieldScene_RunActorUpdateSequence();
        ColorBuffer_ApplyTarget(0, 0);
        ColorBuffer_Interpolate(0x78);
        Event_Wait(0x78);
        Audio_PlayCue(0x56);
        AudioCommand_WaitForCompletion();
        GameFlag_Set(0x9f0);
        Event_RequestExit(0x1e);
    } else {
        u8 *actor;
        s32 flags;

        Event_Wait(0x1e);
        Actor_RunRepeatedMotion(0xc, 1);
        Actor_SetAnimationAndWait(0xc, 4);
        Event_Wait(0x14);
        Event_ShowMessage(0xc, 0);
        Event_Wait(0x14);
        actor = Object_GetById(0xa);
        actor += 0x23;
        flags = 254;
        flags &= *actor;
        *actor = flags;
        actor = Object_GetById(0xa);
        *(u32 *)(actor + 0x6c) = (u32)ShindenHeya_MatchLeaderPriority;
    }
    Event_End();
}

void FieldScene_RunActorUpdateSequence(void)
{

    u32 i;
    u8 *record;

    Call2_scene_primary_script(Engine_ActorSetAttachedEffect, 1, 0x102);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3_scene_primary_script(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Call3_scene_primary_script(Engine_ActorSetSpeed, 11, 0xcccc, 0x6666);
    Call3_scene_primary_script(Engine_ActorSetSpeed, 12, 0xcccc, 0x6666);
    Call3_scene_primary_script(Engine_ActorSetSpeed, 9, 0xcccc, 0x6666);
    Call3_scene_primary_script(Engine_ActorSetSpeed, 10, 0xcccc, 0x6666);
    Call3_scene_primary_script(Engine_ActorSetSpeed, 8, 0xcccc, 0x6666);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Camera_WaitForMove();
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(60);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_Wait(20);
    Event_SetMessage(MSG_ACCEPT_ROBIN_CANT_MEAN);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_FaceActor(12, ACTOR_GERALD, 0);
    Actor_StartRepeatedMotion(12, 2);
    Call3_scene_primary_script(Engine_ActorShowEmote, 12, 0x103, 0);
    Event_Wait(60);
    Event_ShowMessage(12, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(40);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorFaceDirection, 8, 0xd000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Event_Wait(80);
    Audio_PlayCue(17);
    Call2_scene_primary_script(Engine_ColorBufferApplyTarget, 0x10005, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(40);
    Call2_scene_primary_script(Engine_CameraSetSpeed, 0x6666, 0xccc);
    Camera_MoveTo(0xc00000, -1, 0x680000, 1);
    Event_Wait(120);
    Audio_PlayCue(21);
    Audio_PlayCue(0x134);
    Call3_scene_primary_script(Engine_ActorSetPosition, 13, 0xc80000, 0x80000);
    Call3_scene_primary_script(Engine_ActorSetSpeed, 13, 0x6666, 0x3333);
    Actor_MoveToAndWait(13, 200, 72);
    Audio_PlayCue(0x120);
    Event_Wait(30);
    Actor_RunRepeatedMotion(8, 2);
    Actor_SetAnimation(8, 0);
    ShindenHeya_CopyActorPose();
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_FaceActor(ACTOR_GERALD, 13, 0);
    Actor_FaceActor(11, 13, 0);
    Actor_FaceActor(12, 13, 0);
    Actor_FaceActor(9, 13, 0);
    Actor_FaceActor(10, 13, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(10, 2);
    Event_Wait(40);
    Event_ShowMessage(13, 0);
    Event_Wait(20);
    Event_Wait(40);
    Event_ShowMessage(13, 0);
    Event_Wait(60);
    Call2_scene_primary_script(Engine_ActorSetChildValue, 13, 0x100);
    Audio_PlayCue(17);
    Audio_PlayCue(0x134);
    /*
     * 32 repeats: step actor 13's animation, then subtract a fixed amount
     * from the record fields at +24 and +28.
     */
    for (i = 0; i < 32; i++) {
        ShindenHeya_SpawnActorSpark(13);
        Event_Wait(4);
        record = Object_GetById(13);
        *(s32 *)(record + 24) += -0x28f;
        record = Object_GetById(13);
        *(s32 *)(record + 28) += -0x28f;
    }
    Audio_PlayCue(0x120);
    Actor_SetChildValue(13, 0);
    Actor_SetPosition(13, 0, 0);
    Event_Wait(40);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Camera_WaitForMove();
    Call2_scene_primary_script(Engine_ColorBufferApplyTarget, 0x10000, 0);
    ColorBuffer_Interpolate(60);
    Event_Wait(120);
    SceneState_ResetObject14Word108();
    Actor_SetAnimation(8, 1);
    Audio_PlayCue(2);
    Event_Wait(60);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(11, 1);
    Actor_StartRepeatedMotion(12, 1);
    Actor_StartRepeatedMotion(9, 1);
    Actor_RunRepeatedMotion(10, 1);
    Event_Wait(30);
    Event_ShowMessage(11, 0);
    Event_Wait(30);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Event_Wait(30);
    Actor_FaceActor(12, 8, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    Event_Wait(20);
    Actor_FaceActor(8, 12, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 1, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 11, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 12, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 9, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 10, 0x102, 0);
    Event_Wait(30);
    Call3_scene_primary_script(Engine_ActorShowEmote, 8, 0x102, 0);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(180);
    Call3_scene_primary_script(Engine_ActorShowEmote, 12, 0x105, 0);
    Event_Wait(60);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Actor_FaceEachOther(9, 10, 0);
    Event_Wait(40);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(30);
    Call3_scene_primary_script(Engine_ActorShowEmote, 8, 0x101, 0);
    Event_Wait(60);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(11, 1);
    Actor_StartRepeatedMotion(12, 1);
    Actor_StartRepeatedMotion(9, 1);
    Actor_RunRepeatedMotion(10, 1);
    Event_Wait(30);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorShowEmote, 8, 0x101, 0);
    Event_Wait(80);
    Call3_scene_primary_script(Engine_ActorShowEmote, 12, 0x102, 0);
    Event_Wait(60);
    Event_ShowMessage(12, 0);
    Event_Wait(30);
    Call3_scene_primary_script(Engine_ActorShowEmote, 8, 0x105, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(8, 1);
    Call3_scene_primary_script(Engine_ActorShowEmote, 8, 0x106, 0);
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(11, 1);
    Actor_StartRepeatedMotion(12, 1);
    Actor_StartRepeatedMotion(9, 1);
    Actor_RunRepeatedMotion(10, 1);
    Event_Wait(40);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(120);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorShowEmote, 0, 0x105, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 1, 0x105, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 11, 0x105, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 12, 0x100, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 9, 0x105, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 10, 0x105, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(12, 1);
    Event_ShowMessage(12, 0);
    Event_Wait(20);
    Actor_FaceActor(8, 12, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 11, 0x100, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 12, 0x100, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 9, 0x100, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 10, 0x100, 0);
    Event_Wait(60);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessage(8, 0);
    Event_Wait(40);
    Actor_WalkToAndWait(8, 168, 176);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Actor_WalkToAndWait(8, 200, 200);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Actor_FaceDirection(11, 0, 0);
    Call3_scene_primary_script(Engine_ActorFaceDirection, 9, 0x8000, 0);
    Call3_scene_primary_script(Engine_ActorFaceDirection, 10, 0x8000, 0);
    Call3_scene_primary_script(Engine_ActorWalkTo, 8, 200, 0x110);
    Event_Wait(40);
    Call3_scene_primary_script(Engine_ActorFaceDirection, 11, 0x3000, 0);
    Call3_scene_primary_script(Engine_ActorFaceDirection, 9, 0x5000, 0);
    Call3_scene_primary_script(Engine_ActorFaceDirection, 10, 0x5000, 0);
    Actor_WaitForMove(8);
    Actor_SetPosition(8, 0, 0);
    Event_Wait(60);
    Call3_scene_primary_script(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 1, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 11, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 12, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 9, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 10, 0x102, 0);
    Event_Wait(60);
    Event_Wait(120);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 10, 0);
    Actor_FaceActor(11, 9, 0);
    Actor_FaceActor(12, 9, 0);
    Event_Wait(120);
    Call3_scene_primary_script(Engine_ActorShowEmote, 9, 0x105, 0);
    Event_Wait(60);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(10);
    Call3_scene_primary_script(Engine_ActorFaceDirection, 9, 0x5000, 0);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorShowEmote, 9, 0x102, 0);
    Event_Wait(80);
    Actor_FaceActor(9, 10, 0);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorFaceDirection, 10, 0x5000, 0);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorShowEmote, 10, 0x101, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(30);
    Call3_scene_primary_script(Engine_ActorFaceDirection, 10, 0x5000, 0);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(20);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorWalkTo, 9, 200, 0x110);
    Call3_scene_primary_script(Engine_ActorWalkToAndWait, 10, 200, 0x110);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 12, 0x105, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(12, 200, 136);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Actor_FaceActor(11, 12, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(12, 4);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    Actor_WalkToAndWait(11, 168, 168);
    Actor_FaceActor(11, 12, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(ACTOR_GERALD, 11, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(11, 4);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 1, 0x102, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(12, 4);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    Call3_scene_primary_script(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 1, 0x102, 0);
    Event_Wait(60);
    Call3_scene_primary_script(Engine_ActorShowEmote, 12, 0x102, 0);
    Event_Wait(60);
    Event_ShowMessage(12, 0);
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Call3_scene_primary_script(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3_scene_primary_script(Engine_ActorShowEmote, 1, 0x100, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Event_Wait(30);
    Actor_SetAnimation(11, 3);
    Event_Wait(30);
    Actor_SetAnimation(12, 3);
}
void SceneState_ResetObject14Word108(void);

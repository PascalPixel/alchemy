#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 *Data_03001ebc;
/* The one placement the second scene sequence spawns. */
extern const s32 ShindenHeya_PlacementSequenceB[];

/* Calls use this overlay's loader veneers. The early long branch shares
 * the dialogue tail and epilogue; the two timing loops each run six times. */

void ShindenHeya_SpawnOwnerEffect();

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

extern s16 Data_02000240[];

struct FacingObject *Object_GetById();
void Event_CallWithLastActiveObjectId();
void AudioCommand_WaitForCompletion();

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

extern u8 ShindenHeya_TableA;
extern u8 ShindenHeya_TableB;
extern u8 ShindenHeya_PlacementA[];   /* Empty table: place nothing. */
extern u8 ShindenHeya_PlacementB[];
extern u8 ShindenHeya_PlacementC[];
extern u8 ShindenHeya_PlacementD[];
extern u8 ShindenHeya_PlacementE[];
extern u8 ShindenHeya_SceneTableA[];
extern u8 ShindenHeya_SceneTableB[];
extern u8 ShindenHeya_SceneTableC[];
extern u8 ShindenHeya_SceneTableD[];
extern u8 ShindenHeya_SceneTableE[];
extern u8 ShindenHeya_SceneTableF[];
extern u8 ShindenHeya_SceneTableG[];
extern u8 ShindenHeya_SceneTableH[];
extern u8 ShindenHeya_SceneTableI[];

/* One symbol per call site, named at the site's decoded address. */

/*
 * Select this scene's placement script from its stored sub-state.  The
 * 220-byte owner at 0x02000070 includes the 35-entry jump table at
 * 0x0200008c-0x02000117 and the literal pool at 0x02000130-0x0200014b.  The
 * selector is the signed halfword at offset 450 of the scene-record block, and
 * its address is built rather than folded: the `s32 off = 450;` local in its
 * own block is what forces that.  The out-of-range arm is also the arm for
 * most of the in-range entries, so it is a genuine default.
 */

/*
 * In-image script tables: runs of 24-byte records terminated by 0x0000ffff, in
 * the shape {0xffff0000 | selector, count, x, 0, z, value} with coordinates in
 * 16.16 fixed point.  The layout is read off the data, so the fields at +12
 * and +20 are named by position only, and the selector's return type stays an
 * opaque pointer.  The tables live in EWRAM, which is writable and used as
 * save state, so they are deliberately not const.
 */

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

/*
 * Select a table from the scene id and two flags.  The 304-byte owner at
 * 0x0200014c decomposes as dispatcher, the 41-entry jump table at
 * 0x02000168-0x0200020b, the case bodies, an alignment halfword at 0x02000246
 * and the literal pool.  Case-arm order follows the table -- entries grouped
 * by value, distinct values ascending -- so the 20/21/50 arm comes third and
 * the 29 arm after the 32 arm, not in selector order.
 */

s32 UpdateFacingFromResolvedObject(struct FacingObject *object)
{
    struct FacingObject *target;

    target = ResolveFacingObject(object->unknown_64);
    object->facing = CalculateFacingAngle(
        target->position_z - object->position_z,
        target->position_x - object->position_x
    );
    return 0;
}

void *SceneData_GetTableBaa8(void)
{
    return &ShindenHeya_TableA;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTableBbc8(void)
{
    return &ShindenHeya_TableB;
}

void *SceneData_SelectPlacementTableBySubstate(void)
{

    s16 sub;

    {
        s32 off = 450;

        sub = *(s16 *)((u8 *)Data_02000240 + off);
    }
    switch ((s32)sub) {
    case 1:
    case 2:
        return ShindenHeya_PlacementB;

    case 10:
    case 11:
    case 12:
    case 35:
        return ShindenHeya_PlacementC;

    case 20:
    case 21:
        return ShindenHeya_PlacementD;

    case 29:
    case 32:
        return ShindenHeya_PlacementE;

    default:
        return ShindenHeya_PlacementA;
    }
}

u8 *SceneData_SelectTableBySceneIdAndFlags(void)
{
    extern s16 Data_02000240[];

    s16 *tbl = Data_02000240;
    s32 no = tbl[225];

    switch (no) {
    case 10:
    case 12:
        return ShindenHeya_SceneTableB;
    case 11:
        return ShindenHeya_SceneTableC;
    case 20:
    case 21:
    case 50:
        return ShindenHeya_SceneTableD;
    case 32:
        return ShindenHeya_SceneTableI;
    case 29:
        return ShindenHeya_SceneTableG;
    case 35:
        return ShindenHeya_SceneTableH;
    default:
        break;
    }

    if (GameFlag_IsSet(0x87a) != 0) {
        return ShindenHeya_SceneTableF;
    }
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        return ShindenHeya_SceneTableE;
    }
    return ShindenHeya_SceneTableA;
}

void FieldScene_RunActorNineFlagDialogueA(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x855) != 0) {
        Event_SetMessage(MSG_WHEN_STRAY_FROM_YOUR_WORLDLY);
    } else {
        Event_SetMessage(MSG_HEALER_MUST_WORRIED_ABOUT_NEVER);
    }

    if (gGameState.entrance == 11) {
        Event_SetMessage(MSG_POLISHED_GOLD_STATUE_RETURNED_US);
    }

    Actor_SetAnimation(9, 1);
    Actor_FaceEachOther(9, ACTOR_PARTY_LEADER, 0);
    Event_Wait(2);
    Event_ShowMessage(9, 0);
    Event_End();
}

void FieldScene_RunActorNineFlagDialogueB(void)
{
    void Actor_Stop(s32 id);

    Event_Begin();

    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(MSG_WONDER_IF_EVER_SEE_OUR);
    } else {
        Event_SetMessage(MSG_CHILD_HAS_AWAKENED_OUR_TEACHINGS);
    }

    if (gGameState.entrance == 11) {
        Event_SetMessage(MSG_DIRTY_GOLDEN_STATUE_CLEANED_UP);
    }

    Actor_Stop(9);
    Actor_SetAnimation(9, 1);
    Event_Wait(2);
    Event_ShowMessage(9, 0);
    Actor_EnableActionCallback(9, 2);
    Event_End();
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    Event_Begin();
    Event_SetMessage(MSG_ARE_YOU_SURE);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(8, 3);
        Event_Wait(20);
    } else {
        Event_Wait(20);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(20);
            Event_OpenMessage(8, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                Event_Wait(20);
                if (Object_GetById(8)->facing >= 0xa000 && Object_GetById(8)->facing <= 0xe000) {
                    Call3_scene_primary_script(Engine_ActorSetSpeed, 8, 0x8000, 0x4000);
                    Actor_FaceDirection(8, 0, 0);
                    Event_Wait(10);
                    Object_GetById(8)->facing_flags &= ~1;
                    Actor_WalkToAndWait(8, 152, 120);
                    Event_Wait(1);
                    Object_GetById(8)->facing_flags |= 1;
                    Event_Wait(20);
                    Actor_SetAnimationAndWait(8, 3);
                    Event_Wait(20);
                    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 120);
                    Actor_WalkTo(ACTOR_PARTY_LEADER, 192, 168);
                    Event_Wait(20);
                    Actor_WalkToAndWait(8, 168, 120);
                    Call3_scene_primary_script(Engine_ActorFaceDirection, 8, 0x3000, 0);
                    Actor_WaitForMove(ACTOR_PARTY_LEADER);
                } else {
                    Actor_WalkTo(ACTOR_PARTY_LEADER, 192, 168);
                    Event_Wait(20);
                    Call3_scene_primary_script(Engine_ActorFaceDirection, 8, 0x3000, 0);
                    Actor_WaitForMove(ACTOR_PARTY_LEADER);
                }
                FieldScene_RunActorUpdateSequence();
                ColorBuffer_ApplyTarget(0, 0);
                ColorBuffer_Interpolate(120);
                Event_Wait(120);
                Audio_PlayCue(86);
                AudioCommand_WaitForCompletion();
                GameFlag_Set(0x9f0);
                Event_RequestExit(30);
            }
        }
    }
    Event_End();
}

void FieldScene_RunScene378SequenceB(void)
{
    void Actor_Stop();

    u32 i;
    s32 record;

    Event_Begin();
    Call1(Event_CallWithLastActiveObjectId, (s32)ShindenHeya_PlacementSequenceB);
    Call1((void (*)())Engine_TaskWait, 1);
    Event_SetMessage(MSG_ROBIN_YOUR_NEW_FRIENDS_ADEPTS);
    Event_OpenMessage(9, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_ShowMessage(9, 0);
    } else {
        bump_step();
        Call11(Engine_EventShowTwoMessagesAndWait, 2, 16, 1, 24, 1, 3, 7, 16, 1, 14, 0);
        Event_ShowMessage(9, 0);
    }
    Event_End();
}

void FieldScene_RunActorTenCountStep(void)
{

    Event_Begin();
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessage(MSG_DO_FEEL_ANY_CHANGE_IN);
    Event_OpenMessage(10, 0);

    if (Event_ChooseYesNo(0, 0) == 1) {
        (gEventWork->message)++;
    }

    Event_ShowMessage(10, 0);
    Event_End();
}
void FieldScene_RunActorUpdateSequence(void);

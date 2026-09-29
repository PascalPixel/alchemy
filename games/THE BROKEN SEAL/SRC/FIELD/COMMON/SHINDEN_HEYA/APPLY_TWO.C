#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 *Data_03001ebc;

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
extern u8 MsgShindenAmStartingFeelOnlyBeginning[];
extern u8 MsgShindenCameXianFromVeryDistant[];
extern u8 MsgShindenCurseMayOverButWe[];
extern u8 MsgShindenMayWrongButLatelyThere[];
extern u8 MsgShindenMyControlOverPsynergyHas[];
extern u8 MsgShindenPathSolSanctumStillClosed[];
extern u8 MsgShindenSavedAltinFromMonstersClearly[];
extern u8 MsgShindenWasAfterEerieNightWhen[];
extern u8 MsgShindenWasHandFateReturnedGold[];
extern u8 MsgShindenWeWillHelpAnytimeAs[];
extern u8 MsgShindenWieldersPsynergyCalledAdeptsAdepts[];


void FieldScene_RunSupplementalSequenceOne(void);

void SceneState_ApplyTwoRects(void)
{
    {
        s32 a5 = 3, a6 = 2;
        Map_CopyCellsTo(0, 64, 11, 68, a5, a6);
    }
    {
        s32 a5 = 11, a6 = 8;
        Map_CopyCellAttributes(11, 10, 3, 2, a5, a6);
    }
    Task_Wait(1);
}

s32 IsActorFacingInward(void)
{
    ActorState *actor = GetActorState(0);

    if ((u32)((actor->angle + 0x5fff) << 16) <= 0x3ffe0000) {
        return 1;
    }
    return 0;
}

void FieldScene_RunActorEightFacingDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Sanctum_Open(8);
        return;
    }

    Event_Begin();
    if (GameFlag_IsSet(0x87a) != 0)
        Event_SetMessage((s32)MsgShindenPathSolSanctumStillClosed);
    else if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
        Event_SetMessage((s32)MsgShindenMyControlOverPsynergyHas);
    else
        Event_SetMessage((s32)MsgShindenWieldersPsynergyCalledAdeptsAdepts);
    Event_ShowMessage(8, 0);
    Event_End();
}

void FieldScene_DispatchBySceneId(void)
{
    s16 *tbl;
    s32 no;

    if (IsActorFacingInward() != 0) {
        Sanctum_Open(8);
        return;
    }

    Event_Begin();

    tbl = Data_02000240;
    no = tbl[225];

    switch (no) {
    case 10:
    case 12:
        if (GameFlag_IsSet(0x855) != 0) {
            Event_SetMessage((s32)MsgShindenWasHandFateReturnedGold);
        } else {
            Event_SetMessage((s32)MsgShindenWeWillHelpAnytimeAs);
        }
        break;
    case 11:
        Event_SetMessage((s32)MsgShindenMayWrongButLatelyThere);
        break;
    case 20:
    case 21:
    case 50:
        Event_End();
        FieldScene_RunSupplementalSequenceOne();
        return;
    default:
        break;
    }

    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActorEightFlaggedDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Sanctum_Open(8);
        return;
    }

    Event_Begin();
    if (GameFlag_IsSet(0x845) != 0)
        Event_SetMessage((s32)MsgShindenCurseMayOverButWe);
    else
        Event_SetMessage((s32)MsgShindenAmStartingFeelOnlyBeginning);
    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActorEightFollowupDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Sanctum_Open(8);
        return;
    }

    Event_Begin();
    Event_SetMessage((s32)MsgShindenWasAfterEerieNightWhen);
    if (GameFlag_IsSet(0x909) != 0)
        Event_SetMessage((s32)MsgShindenSavedAltinFromMonstersClearly);
    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActorEightDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Sanctum_Open(8);
        return;
    }

    Event_Begin();
    Event_SetMessage((s32)MsgShindenCameXianFromVeryDistant);
    Event_ShowMessage(8, 0);
    Event_End();
}

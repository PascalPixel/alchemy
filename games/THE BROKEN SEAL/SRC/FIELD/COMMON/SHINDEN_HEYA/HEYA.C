#include "EDITION.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FACING_OBJECT.H"
#include "CALL.H"

extern u8 *Data_03001ebc;

/* The one placement the second scene sequence spawns. */
extern const s32 ShindenHeya_PlacementSequenceB[];
void ShindenHeya_SpawnOwnerEffect();

static __inline__ void bump_step(void)
{
    u8 *work = Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + 1);
}

s16 CalculateFacingAngle(s32, s32);
struct FacingObject *ResolveFacingObject(s16);

typedef struct {
    u8 pad_to_angle[6];
    u16 angle;
} ActorState;

ActorState *GetActorState(s32 actor_id);
extern s16 Data_02000240[];
void Event_CallWithLastActiveObjectId();
void AudioCommand_WaitForCompletion();
extern u8 MsgShindenAreYouSure[];
extern u8 MsgShindenChildHasAwakenedOurTeachings[];
extern u8 MsgShindenDirtyGoldenStatueCleanedUp[];
extern u8 MsgShindenDoFeelAnyChangeIn[];
extern u8 MsgShindenHealerMustWorriedAboutNever[];
extern u8 MsgShindenPolishedGoldStatueReturnedUs[];
extern u8 MsgShindenRobinYourNewFriendsAdepts[];
extern u8 MsgShindenWhenStrayFromYourWorldly[];
extern u8 MsgShindenWonderIfEverSeeOur[];
extern u8 ShindenHeya_TableA;
extern u8 ShindenHeya_TableB;
extern u8 ShindenHeya_PlacementA[];

/* Empty table: place nothing. */
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
void FieldScene_RunActorUpdateSequence(void);

void SceneState_ApplyTwoRects(void);
void FieldScene_RunScriptedSceneSequence(void);

extern u8 MsgShindenOnceStepOutsideVillageCannot[];

extern u8 MsgShindenWorldBeganDrifting[];
extern struct BattleEffectBuffers *Data_03001ed0;

struct ShrineWork {
    u8 unknown_0000[0xe5a];
    u16 colors[3];
    u8 unknown_0e60[0x2a00 - 0xe60];
    u8 lamps[4];
};

void UiText_ShowCenteredMessage(s32 message, s32 a1, s32 a2);
s32 ShindenHeya_ChooseRestartOption(void);

s32 ShindenHeya_MatchLeaderPriority();
#define SCENE_REQUEST (*(u32 *)(Data_03001ebc + 0x1c0))
#define SCENE_SETUP_WORD (*(u32 *)(Data_03001ebc + 0x1c8))
#define SKIP_BEATS (*(u16 *)(Data_03001ebc + 0x1d8))
void ShindenHeya_CopyActorPose();
void FieldScene_RunPairedActorChoreography();
void ObjectMotion_WaitForAnimationChange();
void ShindenHeya_SpawnActorSpark();
extern u8 MsgShindenAcceptRobinCantMean[];
extern u8 MsgShindenRobinWillAcceptResponsibilityFor[];
extern u8 MsgShindenWeHadIdeaTrueSanctum[];
void SceneState_ResetObject14Word108(void);

void Engine_ActorSetPosition();
void Engine_ActorSetAnimation();
void ShindenHeya_FollowLeaderOffset();

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

struct Flags5 {
    u8 pad[5];
    u8 flags;
};

struct Flags37 {
    u8 pad[37];
    u8 flags;
};

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

struct Flags39 {
    u8 pad[39];
    u8 count;
};

/* Resource 378 object reset at 0x02002660(28 bytes including alignment). */
extern 

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

extern u32 gFrameCount;

void Engine_ObjectSetScript();
void SceneEffect_StepEllipseOrbit();

/* The motion script the owner effect runs before deleting itself. */
extern const s32 ShindenHeya_OwnerEffectScript[];

/* Calls use this overlay's loader veneers. The early long branch shares
 * the dialogue tail and epilogue; the two timing loops each run six times. */

/*
 * Resource 378 scene reset at 0x020006e8(100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */

/* Resource 378 object reset at 0x02002660(28 bytes including alignment). */

/* Publish the scene's upper prompt and lower dialogue panel. */

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

/* Close through scene 8 when facing inward; otherwise select the story line. */

/* Close scene 8 when facing inward; otherwise choose its story line. */
/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
/* Close scene 8 when facing inward; otherwise emit its fixed story line. */

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; a single word can serve two sites with different
 * targets. Where a macro names an engine function, that is the function the
 * site reaches through the overlay veneer and the main-image veneer island,
 * keeping the site's own calling form. Names without a binding in the
 * repository are provisional.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */

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
    Engine_EventBegin();

    if (GameFlag_IsSet(0x855) != 0) {
        Engine_EventSetMessage((s32)MsgShindenWhenStrayFromYourWorldly);
    } else {
        Engine_EventSetMessage((s32)MsgShindenHealerMustWorriedAboutNever);
    }

    if (gGameState.entrance == 11) {
        Engine_EventSetMessage((s32)MsgShindenPolishedGoldStatueReturnedUs);
    }

    Engine_ActorSetAnimation(9, 1);
    Engine_ActorFaceEachOther(9, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(2);
    Event_ShowMessage(9, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorNineFlagDialogueB(void)
{
    void Engine_ActorStop(s32 id);

    Engine_EventBegin();

    if (GameFlag_IsSet(0x855) == 0) {
        Engine_EventSetMessage((s32)MsgShindenWonderIfEverSeeOur);
    } else {
        Engine_EventSetMessage((s32)MsgShindenChildHasAwakenedOurTeachings);
    }

    if (gGameState.entrance == 11) {
        Engine_EventSetMessage((s32)MsgShindenDirtyGoldenStatueCleanedUp);
    }

    Engine_ActorStop(9);
    Engine_ActorSetAnimation(9, 1);
    Engine_EventWait(2);
    Event_ShowMessage(9, 0);
    Engine_ActorEnableActionCallback(9, 2);
    Engine_EventEnd();
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShindenAreYouSure);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(8, 3);
        Engine_EventWait(20);
    } else {
        Engine_EventWait(20);
        Event_OpenMessage(8, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(20);
            Event_OpenMessage(8, 0);
            if (Engine_EventChooseYesNo(0, 0) == 0) {
                Engine_EventWait(20);
                if (((struct FacingObject *)Object_GetById(8))->facing >= 0xa000 && ((struct FacingObject *)Object_GetById(8))->facing <= 0xe000) {
                    Call3(Engine_ActorSetSpeed, 8, 0x8000, 0x4000);
                    Actor_FaceDirection(8, 0, 0);
                    Engine_EventWait(10);
                    ((struct FacingObject *)Object_GetById(8))->facing_flags &= ~1;
                    Actor_WalkToAndWait(8, 152, 120);
                    Engine_EventWait(1);
                    ((struct FacingObject *)Object_GetById(8))->facing_flags |= 1;
                    Engine_EventWait(20);
                    Engine_ActorSetAnimationAndWait(8, 3);
                    Engine_EventWait(20);
                    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 120);
                    Actor_WalkTo(ACTOR_PARTY_LEADER, 192, 168);
                    Engine_EventWait(20);
                    Actor_WalkToAndWait(8, 168, 120);
                    Engine_ActorFaceDirection(8, 0x3000, 0);
                    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
                } else {
                    Actor_WalkTo(ACTOR_PARTY_LEADER, 192, 168);
                    Engine_EventWait(20);
                    Engine_ActorFaceDirection(8, 0x3000, 0);
                    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
                }
                FieldScene_RunActorUpdateSequence();
                ColorBuffer_ApplyTarget(0, 0);
                Engine_ColorBufferInterpolate(120);
                Engine_EventWait(120);
                Audio_PlayCue(86);
                AudioCommand_WaitForCompletion();
                GameFlag_Set(0x9f0);
                Engine_EventRequestExit(30);
            }
        }
    }
    Engine_EventEnd();
}

void FieldScene_RunScene378SequenceB(void)
{
    void Engine_ActorStop();

    u32 i;
    s32 record;

    Engine_EventBegin();
#if EDITION_INTERNATIONAL
    Event_CallWithLastActiveObjectId((s32)ShindenHeya_PlacementSequenceB);
    Engine_TaskWait(1);
#endif
    Engine_EventSetMessage((s32)MsgShindenRobinYourNewFriendsAdepts);
    Event_OpenMessage(9, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Event_ShowMessage(9, 0);
    } else {
        bump_step();
#if EDITION_INTERNATIONAL
        Call11(Engine_EventShowTwoMessagesAndWait, 2, 16, 1, 24, 1, 3, 7, 16, 1, 14, 0);
#else
        Call11(Engine_EventShowTwoMessagesAndWait, 1, 16, 1, 24, 1, 3, 7, 16, 1, 14, 0);
#endif
        Event_ShowMessage(9, 0);
    }
    Engine_EventEnd();
}

void FieldScene_RunActorTenCountStep(void)
{

    Engine_EventBegin();
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgShindenDoFeelAnyChangeIn);
    Event_OpenMessage(10, 0);

    if (Engine_EventChooseYesNo(0, 0) == 1) {
        (gEventWork->message)++;
    }

    Event_ShowMessage(10, 0);
    Engine_EventEnd();
}

/* Sanctum entry: record the arrival, fade in, then restore the room or run the entrance's scene. */
s32 ShindenHeya_ApplyEntryState(void)
{
    gEventWork->start_transition = 0x209;
    Engine_ColorBufferApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_EventWait(1);
    switch (gGameState.entrance) {
    case 10:
    case 11:
    case 12:
        if (Engine_GameFlagIsSet(0x855) != 0) {
            Call3(Engine_ActorSetPosition, 10, 0xc80000, 0x500000);
        }
        Engine_GameFlagClear(0x12f);
        break;
    case 20:
        SceneState_ApplyTwoRects();
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunScriptedSceneSequence();
        }
    case 29:
    case 32:
    case 35:
        Engine_GameFlagClear(0x12f);
        break;
    case 21:
        SceneState_ApplyTwoRects();
        Engine_GameFlagSet(0x201);
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunScriptedSceneSequence();
        }
        Engine_GameFlagClear(0x12f);
        break;
    }
    return 0;
}

/* Calls use this overlay's loader veneers. The early long branch shares
 * the dialogue tail and epilogue; the two timing loops each run six times. */

/*
 * Resource 378 scene reset at 0x020006e8(100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */

/* Resource 378 object reset at 0x02002660(28 bytes including alignment). */

/* Publish the scene's upper prompt and lower dialogue panel. */

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

/* Close through scene 8 when facing inward; otherwise select the story line. */

/* Close scene 8 when facing inward; otherwise choose its story line. */
/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
/* Close scene 8 when facing inward; otherwise emit its fixed story line. */

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; a single word can serve two sites with different
 * targets. Where a macro names an engine function, that is the function the
 * site reaches through the overlay veneer and the main-image veneer island,
 * keeping the site's own calling form. Names without a binding in the
 * repository are provisional.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */

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
void FieldScene_RunActorEightResetSequence(void)
{
    Engine_EventBegin();
    Camera_SetSpeed(0x10000, 0x2000);
    Engine_CameraMoveToActor(1, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgShindenOnceStepOutsideVillageCannot);
    Event_ShowMessage(8, 0);
    GameFlag_Set(0x200);
    Engine_EventEnd();
}

/* Shrine room: the party faces the altar, the room fades to blue, three of the four lamps light, and the scene exits by the altar's answer. */
void ShindenHeya_RunAltarScene(void)
{
    struct ShrineWork *work;
    u8 zero;

    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 0);
    Engine_ActorSetAnimation(1, 0);
    Engine_ActorSetAnimation(11, 0);
    Engine_ActorSetAnimation(12, 0);
    Engine_ActorSetAnimation(8, 0);
    Engine_ActorSetAnimation(9, 0);
    Engine_ActorSetAnimation(10, 0);
    Engine_ColorBufferApplyTarget(0x10002, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(180);
    work = *(struct ShrineWork **)&Data_03001ed0;
    work->colors[0] = 0x7c00;
    work->colors[1] = 0x7c00;
    work->colors[2] = 0x7c00;
    /* FAKEMATCH: the first lamp is cleared through a u8 local zero, which
     * the compiler loads from the pool. */
    zero = 0;
    work->lamps[0] = zero;
    work->lamps[1] = 1;
    work->lamps[2] = 1;
    work->lamps[3] = 1;
    Engine_EventWait(1);
    UiText_ShowCenteredMessage((s32)MsgShindenWorldBeganDrifting, 1, 0);
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(120);
    Engine_EventWait(60);
    if (ShindenHeya_ChooseRestartOption() == 0) {
        Engine_EventEnd();
        Engine_EventRequestExit(20);
    } else {
        Engine_EventEnd();
        Engine_EventRequestExit(50);
    }
}

/* Draw the actor, both parts, at the leader's sprite priority. */
s32 ShindenHeya_MatchLeaderPriority(struct FieldActor *actor)
{
    actor->sprite->priority = Object_GetById(0)->sprite->priority;
    actor->sprite->second_priority = Object_GetById(0)->sprite->priority;
    return 0;
}

/* Calls use this overlay's loader veneers. The early long branch shares
 * the dialogue tail and epilogue; the two timing loops each run six times. */

/*
 * Resource 378 scene reset at 0x020006e8(100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */

/* Resource 378 object reset at 0x02002660(28 bytes including alignment). */

/* Publish the scene's upper prompt and lower dialogue panel. */

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

/* Close through scene 8 when facing inward; otherwise select the story line. */

/* Close scene 8 when facing inward; otherwise choose its story line. */
/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
/* Close scene 8 when facing inward; otherwise emit its fixed story line. */

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; a single word can serve two sites with different
 * targets. Where a macro names an engine function, that is the function the
 * site reaches through the overlay veneer and the main-image veneer island,
 * keeping the site's own calling form. Names without a binding in the
 * repository are provisional.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */

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
void FieldScene_RunScriptedSceneSequence(void)
{
    u32 i;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x201) != 0) {
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 8, 0);
        Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(0xb, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(0xa, ACTOR_PARTY_LEADER, 0);
        Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
        Engine_CameraWaitForMove();
        SCENE_REQUEST = 0x100;
        SCENE_SETUP_WORD = 0x40;
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_EventWait(0x78);
        goto dialogue;
    }

    ColorBuffer_ApplyTarget(0x10002, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_EventWait(1);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Engine_CameraWaitForMove();
    SCENE_REQUEST = 0x209;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    FieldScene_RunPairedActorChoreography();
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(0x3c);
    Engine_EventWait(0x64);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_EventWait(0x1e);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0xc, 0);
    Actor_FaceActor(ACTOR_GERALD, 0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorEnableActionCallback(8, 1);
    Engine_ActorEnableActionCallback(0xc, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(0xb, 0xcccc, 0x6666);
    Actor_SetSpeed(0xc, 0xcccc, 0x6666);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(0xa, 0xcccc, 0x6666);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Engine_EventWait(0x1e);
    Engine_ActorRunRepeatedMotion(0xc, 2);
    Engine_EventSetMessage((s32)MsgShindenWeHadIdeaTrueSanctum);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0xa);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(0x1e);
    Engine_ActorSetAnimationAndWait(0xc, 3);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(0xb, 2);
    Engine_EventWait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0xb, 0);
    Actor_FaceActor(ACTOR_GERALD, 0xb, 0);
    Engine_EventWait(0x14);
    Event_OpenMessage(0xb, 0);
    /* Each arm advances once, on its own side of the object-state call. */
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(0x14);
        Engine_ActorSetAnimationAndWait(0xb, 3);
        Engine_EventWait(0x14);
        Event_ShowMessage(0xb, 0);
        SKIP_BEATS++;
    } else {
        Engine_EventWait(0x14);
        Engine_ActorSetAnimationAndWait(0xb, 4);
        Engine_EventWait(0x14);
        SKIP_BEATS++;
        Event_ShowMessage(0xb, 0);
    }
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    Actor_FaceActor(0xb, 9, 0);
    Engine_EventWait(0x14);
    Event_OpenMessage(9, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(0x14);
        Engine_ActorSetAnimationAndWait(9, 3);
        Engine_EventWait(0x14);
        Event_ShowMessage(9, 0);
        SKIP_BEATS++;
    } else {
        Engine_EventWait(0x14);
        Engine_ActorSetAnimationAndWait(9, 4);
        Engine_EventWait(0x14);
        SKIP_BEATS++;
        Event_ShowMessage(9, 0);
    }
    Engine_EventWait(0x14);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(0xb, 3);
    Engine_ActorSetAnimation(0xc, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(0xa, 3);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(0xc, 0x101, 0);
    Engine_EventWait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Engine_EventWait(0x14);
    Actor_WalkToAndWait(0xc, 0xe0, 0x78);
    Engine_EventWait(0xa);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Engine_EventWait(0xa);
    Engine_ActorRunRepeatedMotion(0xb, 1);
    Engine_EventWait(0xa);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x1e);
    Engine_ActorRunRepeatedMotion(8, 3);
    Actor_ShowEmote(8, 0x100, 0);
    Engine_EventWait(0x3c);
    Actor_FaceActor(8, 0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_FaceDirection(0xc, 0x5000, 0);
    Engine_EventWait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(0xc, 3);
    Engine_EventWait(0xa);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0xa);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x14);
    Actor_FaceDirection(8, 0x3000, 0);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_FaceDirection(0xc, 0x5000, 0);
    Engine_EventWait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Engine_EventWait(0x32);
    Actor_ShowEmote(0xc, 0x101, 0);
    Engine_EventWait(0x28);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Engine_EventWait(0x3c);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0xa);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(0xb, 0x100, 0);
    Actor_ShowEmote(0xc, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_ShowEmote(0xa, 0x100, 0);
    Engine_EventWait(0x3c);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(0xa);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0xa);
    Actor_ShowEmote(0xc, 0x102, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorStartRepeatedMotion(0xb, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorRunRepeatedMotion(0xa, 1);
    Engine_EventWait(0xa);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x1e);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(0xb, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(0x3c);
    Engine_ActorRunRepeatedMotion(0xc, 2);
    Engine_EventWait(0x14);
    Actor_FaceActor(0xc, 8, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x1e);
    Engine_ActorRunRepeatedMotion(0xb, 2);
    Engine_EventWait(0x14);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x14);
    Engine_EventWait(0x28);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorSetAnimation(8, 0);
    Actor_SetChildValue(8, 0x100);
    ShindenHeya_CopyActorPose();
    Audio_PlayCue(0xc4);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x10);
    for (i = 0; i < 6; i++) {
        ShindenHeya_SpawnOwnerEffect(8, 0x1200);
        Engine_EventWait(8);
    }
    Engine_EventWait(8);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x60);
    Engine_EventWait(0x20);
    Actor_SetChildValue(8, 0);
    Engine_EventWait(0x1e);
    SceneState_ResetObject14Word108();
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_SetAttachedEffect(0xc, 0x102);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(0xb, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(0xa, 0x102);
    Engine_EventWait(0x3c);
    Engine_ActorRunRepeatedMotion(0xa, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xa, 0);
    Engine_EventWait(0x14);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_EventWait(0x3c);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x1e);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(0xa, 3);
    ObjectMotion_WaitForAnimationChange(0xa);
    Engine_EventWait(0x1e);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 0xb, 0);
    Engine_ActorFaceEachOther(ACTOR_GERALD, 0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(0xb, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(0xc, 3);
    ObjectMotion_WaitForAnimationChange(0xc);
    Engine_EventWait(0x3c);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorSetAnimation(8, 0);
    Actor_SetChildValue(8, 0x100);
    ShindenHeya_CopyActorPose();
    Audio_PlayCue(0xc4);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Engine_EventWait(0x10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorStartRepeatedMotion(0xb, 1);
    Engine_ActorStartRepeatedMotion(0xc, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorStartRepeatedMotion(0xa, 1);
    for (i = 0; i < 6; i++) {
        ShindenHeya_SpawnOwnerEffect(8, 0x1200);
        Engine_EventWait(8);
    }
    Engine_EventWait(8);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x80);
    Actor_SetChildValue(8, 0);
    Engine_EventWait(0x1e);
    SceneState_ResetObject14Word108();
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(0xc, 0x105, 0);
    Engine_EventWait(0x14);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Engine_EventWait(0x28);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(0xb, 2);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x14);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorStartRepeatedMotion(0xa, 1);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Actor_SetAttachedEffect(0xc, 0x102);
    Engine_EventWait(0x3c);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(0xc, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Engine_EventWait(0x3c);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(0xb, 3);
    Engine_ActorSetAnimation(0xc, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(0xa, 3);
    ObjectMotion_WaitForAnimationChange(0xa);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(0xc, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x14);
    Actor_FaceActor(8, 0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x32);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(0xb, 0x102, 0);
    Actor_ShowEmote(0xc, 0x102, 0);
    Actor_ShowEmote(9, 0x102, 0);
    Actor_ShowEmote(0xa, 0x102, 0);
    Engine_EventWait(0x50);
    Engine_ActorRunRepeatedMotion(0xb, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0x14);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(0xc, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Engine_EventWait(0x50);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(0xb, 0x102);
    Actor_SetAttachedEffect(0xc, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(0xa, 0x102);
    Engine_EventWait(0x3c);
    Engine_ActorSetAnimationAndWait(0xc, 4);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x28);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x3c);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(0xb, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x3c);
    Actor_WalkToAndWait(8, 0xc8, 0x88);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(8, ACTOR_GERALD, 0);
    Engine_EventWait(0x28);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x32);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x28);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x32);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Engine_EventWait(0x3c);
    Actor_SetAttachedEffect(0xb, 0x102);
    Engine_ActorRunRepeatedMotion(0xb, 1);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x1e);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(0xc, 0x102, 0);
    Engine_ActorStartRepeatedMotion(0xc, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x1e);
    Actor_WalkToAndWait(8, 0xa8, 0x78);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceDirection(8, 0xd000, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Engine_EventWait(0x3c);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0xa);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(0x1e);
dialogue:
    Engine_EventSetMessage((s32)MsgShindenRobinWillAcceptResponsibilityFor);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(0x14);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        FieldScene_RunActorUpdateSequence();
        ColorBuffer_ApplyTarget(0, 0);
        Engine_ColorBufferInterpolate(0x78);
        Engine_EventWait(0x78);
        Audio_PlayCue(0x56);
        AudioCommand_WaitForCompletion();
        GameFlag_Set(0x9f0);
        Engine_EventRequestExit(0x1e);
    } else {
        u8 *actor;
        s32 flags;

        Engine_EventWait(0x1e);
        Engine_ActorRunRepeatedMotion(0xc, 1);
        Engine_ActorSetAnimationAndWait(0xc, 4);
        Engine_EventWait(0x14);
        Event_ShowMessage(0xc, 0);
        Engine_EventWait(0x14);
        actor = (u8 *)Object_GetById(0xa);
        actor += 0x23;
        flags = 254;
        flags &= *actor;
        *actor = flags;
        actor = (u8 *)Object_GetById(0xa);
        *(u32 *)(actor + 0x6c) = (u32)ShindenHeya_MatchLeaderPriority;
    }
    Engine_EventEnd();
}

void FieldScene_RunActorUpdateSequence(void)
{

    u32 i;
    u8 *record;

    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 11, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 12, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 9, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 10, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 8, 0xcccc, 0x6666);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Engine_CameraWaitForMove();
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgShindenAcceptRobinCantMean);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(20);
    Actor_FaceActor(12, ACTOR_GERALD, 0);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorShowEmote(12, 0x103, 0);
    Engine_EventWait(60);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(40);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(8, 0xd000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_EventWait(80);
    Audio_PlayCue(17);
    Engine_ColorBufferApplyTarget(0x10005, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(40);
    Call2(Engine_CameraSetSpeed, 0x6666, 0xccc);
    Camera_MoveTo(0xc00000, -1, 0x680000, 1);
    Engine_EventWait(120);
    Audio_PlayCue(21);
    Audio_PlayCue(0x134);
    Call3(Engine_ActorSetPosition, 13, 0xc80000, 0x80000);
    Call3(Engine_ActorSetSpeed, 13, 0x6666, 0x3333);
    Actor_MoveToAndWait(13, 200, 72);
    Audio_PlayCue(0x120);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorSetAnimation(8, 0);
    ShindenHeya_CopyActorPose();
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_FaceActor(ACTOR_GERALD, 13, 0);
    Actor_FaceActor(11, 13, 0);
    Actor_FaceActor(12, 13, 0);
    Actor_FaceActor(9, 13, 0);
    Actor_FaceActor(10, 13, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorStartRepeatedMotion(11, 2);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorStartRepeatedMotion(9, 2);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Event_ShowMessage(13, 0);
    Engine_EventWait(20);
    Engine_EventWait(40);
    Event_ShowMessage(13, 0);
    Engine_EventWait(60);
    Engine_ActorSetChildValue(13, 0x100);
    Audio_PlayCue(17);
    Audio_PlayCue(0x134);
    /*
     * 32 repeats: step actor 13's animation, then subtract a fixed amount
     * from the record fields at +24 and +28.
     */
    for (i = 0; i < 32; i++) {
        ShindenHeya_SpawnActorSpark(13);
        Engine_EventWait(4);
        record = (u8 *)Object_GetById(13);
        *(s32 *)(record + 24) += -0x28f;
        record = (u8 *)Object_GetById(13);
        *(s32 *)(record + 28) += -0x28f;
    }
    Audio_PlayCue(0x120);
    Actor_SetChildValue(13, 0);
    Actor_SetPosition(13, 0, 0);
    Engine_EventWait(40);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Engine_CameraWaitForMove();
    Engine_ColorBufferApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(120);
    SceneState_ResetObject14Word108();
    Engine_ActorSetAnimation(8, 1);
    Audio_PlayCue(2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorStartRepeatedMotion(12, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(30);
    Event_ShowMessage(11, 0);
    Engine_EventWait(30);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(30);
    Actor_FaceActor(12, 8, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Actor_FaceActor(8, 12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Call3(Engine_ActorShowEmote, 11, 0x102, 0);
    Call3(Engine_ActorShowEmote, 12, 0x102, 0);
    Call3(Engine_ActorShowEmote, 9, 0x102, 0);
    Call3(Engine_ActorShowEmote, 10, 0x102, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorShowEmote, 8, 0x102, 0);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(180);
    Call3(Engine_ActorShowEmote, 12, 0x105, 0);
    Engine_EventWait(60);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_ActorFaceEachOther(9, 10, 0);
    Engine_EventWait(40);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorShowEmote, 8, 0x101, 0);
    Engine_EventWait(60);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorStartRepeatedMotion(12, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(30);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 8, 0x101, 0);
    Engine_EventWait(80);
    Call3(Engine_ActorShowEmote, 12, 0x102, 0);
    Engine_EventWait(60);
    Event_ShowMessage(12, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorShowEmote, 8, 0x105, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorShowEmote(8, 0x106, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorStartRepeatedMotion(12, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(40);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(120);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 0, 0x105, 0);
    Call3(Engine_ActorShowEmote, 1, 0x105, 0);
    Call3(Engine_ActorShowEmote, 11, 0x105, 0);
    Call3(Engine_ActorShowEmote, 12, 0x100, 0);
    Call3(Engine_ActorShowEmote, 9, 0x105, 0);
    Call3(Engine_ActorShowEmote, 10, 0x105, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(12, 1);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Actor_FaceActor(8, 12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3(Engine_ActorShowEmote, 11, 0x100, 0);
    Call3(Engine_ActorShowEmote, 12, 0x100, 0);
    Call3(Engine_ActorShowEmote, 9, 0x100, 0);
    Call3(Engine_ActorShowEmote, 10, 0x100, 0);
    Engine_EventWait(60);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessage(8, 0);
    Engine_EventWait(40);
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
    Call3(Engine_ActorFaceDirection, 9, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 10, 0x8000, 0);
    Call3(Engine_ActorWalkTo, 8, 200, 0x110);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 11, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 9, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 0);
    Engine_ActorWaitForMove(8);
    Actor_SetPosition(8, 0, 0);
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Call3(Engine_ActorShowEmote, 11, 0x102, 0);
    Call3(Engine_ActorShowEmote, 12, 0x102, 0);
    Call3(Engine_ActorShowEmote, 9, 0x102, 0);
    Call3(Engine_ActorShowEmote, 10, 0x102, 0);
    Engine_EventWait(60);
    Engine_EventWait(120);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 10, 0);
    Actor_FaceActor(11, 9, 0);
    Actor_FaceActor(12, 9, 0);
    Engine_EventWait(120);
    Call3(Engine_ActorShowEmote, 9, 0x105, 0);
    Engine_EventWait(60);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 9, 0x5000, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 9, 0x102, 0);
    Engine_EventWait(80);
    Actor_FaceActor(9, 10, 0);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 0);
    Engine_EventWait(20);
    Engine_ActorShowEmote(10, 0x101, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(10, 0x5000, 0);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Call3(Engine_ActorWalkTo, 9, 200, 0x110);
    Call3(Engine_ActorWalkToAndWait, 10, 200, 0x110);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Engine_ActorShowEmote(12, 0x105, 0);
    Engine_EventWait(60);
    Actor_WalkToAndWait(12, 200, 136);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Actor_FaceActor(11, 12, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(12, 4);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Actor_WalkToAndWait(11, 168, 168);
    Actor_FaceActor(11, 12, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(ACTOR_GERALD, 11, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(12, 4);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorShowEmote(12, 0x102, 0);
    Engine_EventWait(60);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Engine_ActorShowEmote(1, 0x100, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(11, 3);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(12, 3);
}

void ShindenHeya_CopyActorPose(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v0;
    u8 *v2;
    u8 *p5;

    record = (s32)Object_GetById(8);
    if (record != 0) {
        Engine_ActorSetPosition(14, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Engine_ActorSetAnimation(14, 0);
    rec7 = (s32)Object_GetById(14);
    record = (s32)Object_GetById(8);
    *(u16 *)(rec7 + 6) = *(u16 *)(record + 6);
    record = (s32)Object_GetById(14);
    *(s32 *)(record + 108) = (s32)ShindenHeya_FollowLeaderOffset;
    record = (s32)Object_GetById(14);
    p5 = *(s32 *)(record + 80);
    {
        for (i = 0; i < ((struct Flags39 *)p5)->count; i++) {
            u8 *e = ((u8 **)(p5 + 40))[i];

            if (e != 0 && *(s32 *)(e + 16) != 0) {
                ((struct Flags5 *)e)->flags = 10;
            }
        }
    }
    ((struct Flags37 *)p5)->flags = 1;
    ((struct Flags35 *)((s32)Object_GetById(14)))->flags &= 254;
    ((struct Flags9 *)p5)->mode = 2;
}

/* Calls use this overlay's loader veneers. The early long branch shares
 * the dialogue tail and epilogue; the two timing loops each run six times. */

/*
 * Resource 378 scene reset at 0x020006e8(100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */

/* Publish the scene's upper prompt and lower dialogue panel. */

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

/* Close through scene 8 when facing inward; otherwise select the story line. */

/* Close scene 8 when facing inward; otherwise choose its story line. */
/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
/* Close scene 8 when facing inward; otherwise emit its fixed story line. */

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; a single word can serve two sites with different
 * targets. Where a macro names an engine function, that is the function the
 * site reaches through the overlay veneer and the main-image veneer island,
 * keeping the site's own calling form. Names without a binding in the
 * repository are provisional.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */

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
void SceneState_ResetObject14Word108(void)
{
    u8 *state = (u8 *)Object_GetById(14);
    *(s32 *)(state + 108) = 0;
    Actor_SetPosition(14, 0, 0);
}

void ShindenHeya_FollowLeaderOffset(u8 *obj)
{
    u8 *leader;

    leader = (u8 *)Object_GetById(8);
    *(s32 *)(obj + 56) = *(s32 *)(obj + 8) = *(s32 *)(leader + 8);
    *(s32 *)(obj + 60) = *(s32 *)(obj + 12) = *(s32 *)(leader + 12);
    *(s32 *)(obj + 64) = *(s32 *)(obj + 16) = *(s32 *)(leader + 16) + -0x20000;
    switch (*(u32 *)&gFrameCount & 3) {
    case 0:
        *(s32 *)(obj + 56) = *(s32 *)(obj + 8) = *(s32 *)(leader + 8) + -0x38000;
        break;
    case 1:
        *(s32 *)(obj + 56) = *(s32 *)(obj + 8) = *(s32 *)(leader + 8) + 0x30000;
        break;
    case 2:
        *(s32 *)(obj + 60) = *(s32 *)(obj + 12) = *(s32 *)(leader + 12) + 0x20000;
        break;
    case 3:
        *(s32 *)(obj + 64) = *(s32 *)(obj + 16) = *(s32 *)(leader + 16);
        break;
    }
}

/* Calls use this overlay's loader veneers. The early long branch shares
 * the dialogue tail and epilogue; the two timing loops each run six times. */

/*
 * Resource 378 scene reset at 0x020006e8(100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */

/* Resource 378 object reset at 0x02002660(28 bytes including alignment). */

/* Publish the scene's upper prompt and lower dialogue panel. */

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

/* Close through scene 8 when facing inward; otherwise select the story line. */

/* Close scene 8 when facing inward; otherwise choose its story line. */
/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
/* Close scene 8 when facing inward; otherwise emit its fixed story line. */

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; a single word can serve two sites with different
 * targets. Where a macro names an engine function, that is the function the
 * site reaches through the overlay veneer and the main-image veneer island,
 * keeping the site's own calling form. Names without a binding in the
 * repository are provisional.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */

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
void SceneEffect_StepEllipseOrbit(u8 *obj)
{
    u8 *anchor = *(u8 **)(obj + 104);
    u16 *angle = (u16 *)(obj + 100);
    u16 theta = *angle;
    s32 x;
    s32 z;
    s32 tmp;

    x = *(s32 *)(anchor + 8) + Engine_MathCos(theta)* 14;
    *(s32 *)(obj + 8) = x;

    z = *(s32 *)(anchor + 16) + Engine_MathSin(theta)* 10;
    tmp = *(s32 *)(obj + 8);
    *(s32 *)(obj + 16) = z;
    *(s32 *)(obj + 64) = z;
    *(s32 *)(obj + 56) = tmp;

    *angle = (u16)(*angle + *(u16 *)(obj + 102));
}

/* Spawns the effect object above actor a0 with its script, zeroes its state and copies the owner's sprite mode. */
void ShindenHeya_SpawnOwnerEffect(s32 a0, s32 a1)
{
    u32 i;
    s32 p8;
    s32 p8b;
    s32 rec;
    u8 *rec8;
    s32 record;
    u8 *p5;

    p8 = a1;
    rec = ((s32 (*)())Object_GetById)();
    if (rec != 0) {
        rec8 = Value4(Engine_ObjectCreate, 0x11d, *(s32 *)(rec + 8), (*(s32 *)(rec + 12) + 0x2d0000), *(s32 *)(rec + 16));
        if ((s32)rec8 != 0) {
            p5 = *(s32 *)((s32)rec8 + 80);
            Engine_ObjectSetScript((s32)rec8, (s32)ShindenHeya_OwnerEffectScript);
            {
                s32 zero = 0;

                rec8[85] = zero;
                *(u16 *)(rec8 + 100) = zero;
            }
            *(u16 *)(rec8 + 102) = p8;
            *(s32 *)((s32)rec8 + 108) = (s32)SceneEffect_StepEllipseOrbit;
            { u16 v = 0; p5[38] = v; }
            { u8 m = ((struct Flags9 *)(*(s32 *)(rec + 80)))->mode; *(s32 *)((s32)rec8 + 104) = rec; ((struct Flags9 *)p5)->mode = m; } /* FAKEMATCH: the mode is read into a temporary so the owner store schedules first */
        }
    }
}

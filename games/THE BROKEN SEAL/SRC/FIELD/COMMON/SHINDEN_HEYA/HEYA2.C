#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FACING_OBJECT.H"
#include "CALL.H"

void ShindenHeya_RaiseItemIcon(s32 item);

extern u8 *Data_03001ebc;

/* The action-callback paths that walk the leader and Gerald in circles. */
extern const s32 ShindenHeya_LeaderCircleScript[];
extern const s32 ShindenHeya_GeraldCircleScript[];
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
void Object_RefreshSelectorById();


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

s32 UpdateFacingFromResolvedObject(struct FacingObject *object);

void ObjectDispatch_Initialize();
s32 Runtime_AllocateHeapBlock();
void WaitFrames();
void Runtime_ReleaseHeapBlock();
s32 VramBlock_LoadCached();
void Ui_PrepareTransferForItem();

/* The motion scripts that pop the icon in and delete it a second later. */
extern const s32 ShindenHeya_ItemIconGrowScript[];
extern const s32 ShindenHeya_ItemIconEndScript[];

/* NONMATCHING: 172 of 172 bytes, 12 halfword edits (2026-09-24). Same shape
 * as 380:02004260. Remaining: the -33 mask is folded to 0xdf (a word mask
 * variable grows the function), the sprite[28] load is scheduled late, and
 * the loop zero is not shared with the counter in r5. */
struct Spr5 { u8 pad[5]; u8 lo:5; u8 bit5:1; u8 hi:2; };

/* The motion script that deletes a spark once it has risen. */
extern const s32 ShindenHeya_SparkEndScript[];

s32 Engine_RandomNext();
s32 Math_RemainderUnsigned();
void ShindenHeya_UpdateRisingSpark();

struct Sprite378 {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

/* A zero kept in a one-halfword struct, so it is a HImode value the compiler
 * holds in a high register and reloads from the pool. */
struct Half {
    u16 v;
};

extern s16 Data_02000240[];
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
void FieldScene_RunPairedActorChoreography(void)
{

    Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
    Call3(Engine_ActorSetSpeed, 1, 0x18000, 0xc000);
    Engine_ActorRunRepeatedMotion(12, 2); /* main:0808a138 */
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(12, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3); /* main:0808a110 */
    Engine_EventWait(15);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_ActorStartRepeatedMotion(0, 1); /* object 0, variant 1 */
    ((struct FacingObject *(*)())Object_GetById)(0)->facing_flags &= ~1;
    Actor_WalkTo(ACTOR_PARTY_LEADER, 184, 168);
    ((struct FacingObject *(*)())Object_GetById)(1)->facing_flags &= ~1;
    Engine_ActorWalkToAndWait(1, 200, 168);
    Engine_EventWait(1);
    ((struct FacingObject *(*)())Object_GetById)(1)->facing_flags |= 1;
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    ((struct FacingObject *(*)())Object_GetById)(0)->facing_flags |= 1;
    ((struct FacingObject *(*)())Object_GetById)(1)->facing_flags |= 1;
    Engine_ActorJump(ACTOR_GERALD, 2, 0);
    Engine_EventWait(15);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Engine_EventWait(5); /* main:0808a080 */
    Engine_ActorJump(ACTOR_GERALD, 2, 0);
    Engine_EventWait(25);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Engine_EventWait(5);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3); /* main:0808a110 */
    Engine_EventWait(5);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Engine_EventWait(15);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3); /* main:0808a110 */
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Engine_EventWait(10); /* main:0808a138 */
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(ACTOR_GERALD, 11, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Engine_EventWait(15);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2); /* main:0808a138 */
    Engine_EventWait(10);
    ((s32 (*)())ShindenHeya_RaiseItemIcon)(222, 0xb80000, 0x1b0000, 0xa80000);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventWait(10);
    Engine_ActorJump(ACTOR_GERALD, 4, 0); /* main:0808a138 */
    Call3(Engine_ActorFaceDirection, 1, 0xd000, 0);
    Engine_EventWait(15);
    Call3(Engine_ActorFaceDirection, 1, 0xb000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 0xd000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 0xb000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 0xd000, 0);
    Engine_EventWait(30);
    Engine_ActorJump(ACTOR_GERALD, 4, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x3000, 0);
    Engine_EventWait(15);
    Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 0x3000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 0x3000, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Object_GetById(8)->unknown_64 = 1;
    *(s32 *)((u8 *)Object_GetById(8) + 108) = (s32)UpdateFacingFromResolvedObject;
    Object_GetById(12)->unknown_64 = 1;
    *(s32 *)((u8 *)Object_GetById(12) + 108) = (s32)UpdateFacingFromResolvedObject;
    Engine_ActorWalkToAndWait(1, 196, 180);
    Actor_WalkToAndWait(ACTOR_GERALD, 184, 184);
    Actor_WalkToAndWait(ACTOR_GERALD, 180, 180);
    Actor_WalkToAndWait(ACTOR_GERALD, 168, 168);
    Actor_WalkToAndWait(ACTOR_GERALD, 180, 156);
    Actor_WalkTo(ACTOR_GERALD, 200, 104);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 168);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1); /* main:0808a138 */
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
    Engine_EventWait(15);
    *(s32 *)((u8 *)Object_GetById(12) + 108) = 0;
    *(s32 *)((u8 *)Object_GetById(8) + 108) = 0;
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_ActorShowEmote(8, 0x100, 0);
    Engine_EventWait(60); /* main:0808a080 */
    Engine_ActorSetAnimation(8, 0);
    Engine_ActorShowEmote(0, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(10);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0); /* main:0808a138 */
    Engine_EventWait(20);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
    Engine_EventWait(20);
    Engine_EventWait(15);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(12, 3); /* main:0808a110 */
    Engine_EventWait(60);
    Actor_WalkToAndWait(ACTOR_GERALD, 208, 168);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4); /* main:0808a110 */
    Engine_EventWait(10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1); /* main:0808a138 */
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(12, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Engine_EventWait(10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(0, (s32)ShindenHeya_LeaderCircleScript);
    Engine_ActorEnableActionCallback(1, (s32)ShindenHeya_GeraldCircleScript);
    Object_RefreshSelectorById(0); /* main:0808a0a0 */
    Object_RefreshSelectorById(1); /* main:0808a0a0 */
    Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
    Call3(Engine_ActorSetSpeed, 1, 0x18000, 0xc000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Engine_EventWait(1);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(ACTOR_GERALD, 11, 0);
    Engine_EventWait(1);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    Engine_EventWait(1); /* main:0808a138 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 192, 168);
    Actor_WalkToAndWait(ACTOR_GERALD, 208, 168);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xd000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xb000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0x3000, 0);
    Engine_ActorFaceDirection(1, 0xd000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0x5000, 0);
    Engine_ActorFaceDirection(1, 0xb000, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimationAndWait(12, 3); /* main:0808a110 */
    Engine_EventWait(30);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    Engine_EventWait(60);
}

/* Shrine room: raises an item icon object, loads the item's icon into its sprite and clears its motion flags while it rises for sixty frames. */
void ShindenHeya_RaiseItemIcon(s32 item)
{
    u8 *obj;
    u8 *spr;
    s32 buf;
    s32 zero;
    s32 z;
    u8 *flag;
    u32 i;

    obj = ((s32 (*)())Engine_ObjectCreate)(22);
    zero = 0;
    if (obj != 0) {
        ObjectDispatch_Initialize((s32)obj, (s32)ShindenHeya_ItemIconGrowScript);
        spr = *(u8 **)(obj + 80);
        spr[38] = zero;
        spr[39] = zero;
        ((struct Spr5 *)spr)->bit5 = 0;
        spr[9] &= 15;
        *(s32 *)(obj + 40) = 0x20000;
        *(s32 *)(obj + 72) = 0x4000;
        buf = Runtime_AllocateHeapBlock(17, 0x608);
        Ui_PrepareTransferForItem(item);
        VramBlock_LoadCached(spr[28], 128, buf + 0x400);
        Runtime_ReleaseHeapBlock(17);
        /* FAKEMATCH: the stored zero is a variable set after the flag
         * address, so it copies the counter's zero after that address. */
        for (i = 0, flag = obj + 85, z = 0; i <= 59; i++) {
            if ((u32)(*(s32 *)(obj + 40) + 255) <= 0x1fe)
                *flag = z;
            WaitFrames(1);
        }
        ObjectDispatch_Initialize((s32)obj, (s32)ShindenHeya_ItemIconEndScript);
    }
}

/*
 * Shrine room: raises a spark by its speed each frame and twinkles it
 * through full, four-fifths and three-fifths size every four frames, until
 * its timer runs out and its script deletes it. The spark's speed and
 * timer are the halfwords its spawner sets at 0x64 and 0x66.
 */
void ShindenHeya_UpdateRisingSpark(struct FieldActor *spark)
{
    s32 scale;
    s32 timer;

    spark->y.fixed += (s16)spark->unknown_64 << 12;
    spark->target_y = spark->y.fixed;
    scale = 0;
    switch ((u16)((s16)spark->unknown_66 >> 2) & 3) {
    case 0:
        scale = 0x10000;
        break;
    case 1:
    case 3:
        scale = 0xcccc;
        break;
    case 2:
        scale = 0x9999;
        break;
    }
    spark->scale_x = scale;
    spark->scale_y = scale;
    timer = spark->unknown_66 - 1;
    spark->unknown_66 = timer;
    if ((s16)timer <= 0)
        Engine_ObjectSetScript(spark, ShindenHeya_SparkEndScript);
}

/* Spawn a spark near the actor with a random delay and lifetime. */

/* Shrine room: spawns a spark effect at a random offset above an actor, with random delay and lifetime, on the actor's sprite layer. */
void ShindenHeya_SpawnActorSpark(s32 id)
{
    u8 *actor;
    u8 *obj;
    struct Sprite378 *spr;
    s32 x;
    s32 r;
    struct Half zero;

    actor = (u8 *)Object_GetById(id);
    if (actor == 0)
        return;
    r = Math_RemainderUnsigned(Engine_RandomNext(), 20);
    x = *(s32 *)(actor + 8);
    x += r << 16;
    x += -0xa0000;
    obj = (u8 *)Engine_ObjectCreate(0x11e, x, *(s32 *)(actor + 12) + ((Engine_RandomNext() & 15) << 16) + -0x80000, *(s32 *)(actor + 16));
    if (obj == 0)
        return;
    spr = *(struct Sprite378 **)(obj + 80);
    obj[85] = 0;
    *(u16 *)(obj + 100) = Math_RemainderUnsigned(Engine_RandomNext(), 10) + 5;
    /* FAKEMATCH: the spark frame zero held in a halfword struct. */
    zero.v = 0;
    *(u16 *)(obj + 102) = Math_RemainderUnsigned(Engine_RandomNext(), 60) + 30;
    *(s32 *)(obj + 108) = (s32)ShindenHeya_UpdateRisingSpark;
    ((u8 *)spr)[38] = zero.v;
    spr->layer = (*(struct Sprite378 **)(actor + 80))->layer;
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
    Engine_TaskWait(1);
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
        Engine_SanctumOpen(8);
        return;
    }

    Engine_EventBegin();
    if (GameFlag_IsSet(0x87a) != 0)
        Engine_EventSetMessage((s32)MsgShindenPathSolSanctumStillClosed);
    else if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
        Engine_EventSetMessage((s32)MsgShindenMyControlOverPsynergyHas);
    else
        Engine_EventSetMessage((s32)MsgShindenWieldersPsynergyCalledAdeptsAdepts);
    Event_ShowMessage(8, 0);
    Engine_EventEnd();
}

void FieldScene_DispatchBySceneId(void)
{
    s16 *tbl;
    s32 no;

    if (IsActorFacingInward() != 0) {
        Engine_SanctumOpen(8);
        return;
    }

    Engine_EventBegin();

    tbl = Data_02000240;
    no = tbl[225];

    switch (no) {
    case 10:
    case 12:
        if (GameFlag_IsSet(0x855) != 0) {
            Engine_EventSetMessage((s32)MsgShindenWasHandFateReturnedGold);
        } else {
            Engine_EventSetMessage((s32)MsgShindenWeWillHelpAnytimeAs);
        }
        break;
    case 11:
        Engine_EventSetMessage((s32)MsgShindenMayWrongButLatelyThere);
        break;
    case 20:
    case 21:
    case 50:
        Engine_EventEnd();
        FieldScene_RunSupplementalSequenceOne();
        return;
    default:
        break;
    }

    Event_ShowMessage(8, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorEightFlaggedDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Engine_SanctumOpen(8);
        return;
    }

    Engine_EventBegin();
    if (GameFlag_IsSet(0x845) != 0)
        Engine_EventSetMessage((s32)MsgShindenCurseMayOverButWe);
    else
        Engine_EventSetMessage((s32)MsgShindenAmStartingFeelOnlyBeginning);
    Event_ShowMessage(8, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorEightFollowupDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Engine_SanctumOpen(8);
        return;
    }

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShindenWasAfterEerieNightWhen);
    if (GameFlag_IsSet(0x909) != 0)
        Engine_EventSetMessage((s32)MsgShindenSavedAltinFromMonstersClearly);
    Event_ShowMessage(8, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorEightDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Engine_SanctumOpen(8);
        return;
    }

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShindenCameXianFromVeryDistant);
    Event_ShowMessage(8, 0);
    Engine_EventEnd();
}

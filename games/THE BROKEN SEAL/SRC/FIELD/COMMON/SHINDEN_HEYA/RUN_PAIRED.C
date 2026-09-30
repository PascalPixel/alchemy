#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 *Data_03001ebc;
/* The action-callback paths that walk the leader and Gerald in circles. */
extern const s32 ShindenHeya_LeaderCircleScript[];
extern const s32 ShindenHeya_GeraldCircleScript[];

/* Calls use this overlay's loader veneers. The early long branch shares
 * the dialogue tail and epilogue; the two timing loops each run six times. */

void ShindenHeya_SpawnOwnerEffect();

static __inline__ void bump_step(void)
{
    u8 *work = Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + 1);
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

s32 ShindenHeya_RaiseItemIcon();
void Object_RefreshSelectorById();

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

#include "TYPES.H"
#include "CALL.H"
/* FAKEMATCH: calls that cast Object_GetById to another return type keep their original register order. */
u8 *Object_GetById();

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

void FieldScene_RunPairedActorChoreography(void)
{

    Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
    Call3(Engine_ActorSetSpeed, 1, 0x18000, 0xc000);
    ((void (*)())Engine_ActorRunRepeatedMotion)(12, 2); /* main:0808a138 */
    Event_Wait(10);
    Actor_SetAnimationAndWait(12, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3); /* main:0808a110 */
    Event_Wait(15);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    ((void (*)())Engine_ActorStartRepeatedMotion)(0, 1); /* object 0, variant 1 */
    ((struct FacingObject *(*)())Object_GetById)(0)->facing_flags &= ~1;
    Actor_WalkTo(ACTOR_PARTY_LEADER, 184, 168);
    ((struct FacingObject *(*)())Object_GetById)(1)->facing_flags &= ~1;
    ((void (*)())Engine_ActorWalkToAndWait)(1, 200, 168);
    Event_Wait(1);
    ((struct FacingObject *(*)())Object_GetById)(1)->facing_flags |= 1;
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    ((struct FacingObject *(*)())Object_GetById)(0)->facing_flags |= 1;
    ((struct FacingObject *(*)())Object_GetById)(1)->facing_flags |= 1;
    Actor_Jump(ACTOR_GERALD, 2, 0);
    Event_Wait(15);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    ((void (*)())Engine_EventWait)(5); /* main:0808a080 */
    Actor_Jump(ACTOR_GERALD, 2, 0);
    Event_Wait(25);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3); /* main:0808a110 */
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_Wait(15);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3); /* main:0808a110 */
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Event_Wait(10); /* main:0808a138 */
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(ACTOR_GERALD, 11, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Event_Wait(15);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2); /* main:0808a138 */
    Event_Wait(10);
    ShindenHeya_RaiseItemIcon(222, 0xb80000, 0x1b0000, 0xa80000);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(10);
    Actor_Jump(ACTOR_GERALD, 4, 0); /* main:0808a138 */
    Call3(Engine_ActorFaceDirection, 1, 0xd000, 0);
    Event_Wait(15);
    Call3(Engine_ActorFaceDirection, 1, 0xb000, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 1, 0xd000, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 1, 0xb000, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 1, 0xd000, 0);
    Event_Wait(30);
    Actor_Jump(ACTOR_GERALD, 4, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x3000, 0);
    Event_Wait(15);
    Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 1, 0x3000, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 1, 0x3000, 0);
    Event_Wait(30);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Event_Wait(10);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_Wait(10);
    ((struct FacingObject *(*)())Object_GetById)(8)->unknown_64 = 1;
    *(s32 *)(Object_GetById(8) + 108) = (s32)UpdateFacingFromResolvedObject;
    ((struct FacingObject *(*)())Object_GetById)(12)->unknown_64 = 1;
    *(s32 *)(Object_GetById(12) + 108) = (s32)UpdateFacingFromResolvedObject;
    Engine_ActorWalkToAndWait(1, 196, 180);
    Actor_WalkToAndWait(ACTOR_GERALD, 184, 184);
    Actor_WalkToAndWait(ACTOR_GERALD, 180, 180);
    Actor_WalkToAndWait(ACTOR_GERALD, 168, 168);
    Actor_WalkToAndWait(ACTOR_GERALD, 180, 156);
    Actor_WalkTo(ACTOR_GERALD, 200, 104);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 168);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Actor_WaitForMove(ACTOR_GERALD);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1); /* main:0808a138 */
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
    Event_Wait(15);
    *(s32 *)(Object_GetById(12) + 108) = 0;
    *(s32 *)(Object_GetById(8) + 108) = 0;
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_ActorShowEmote(8, 0x100, 0);
    ((void (*)())Engine_EventWait)(60); /* main:0808a080 */
    Actor_SetAnimation(8, 0);
    Engine_ActorShowEmote(0, 0x102, 0);
    Event_Wait(60);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_SetAnimationAndWait(11, 3); /* main:0808a110 */
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(10);
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 0); /* main:0808a138 */
    Event_Wait(20);
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
    ((void (*)())Engine_EventWait)(20);
    Event_Wait(15);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_SetAnimationAndWait(12, 3); /* main:0808a110 */
    Event_Wait(60);
    Actor_WalkToAndWait(ACTOR_GERALD, 208, 168);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 4);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4); /* main:0808a110 */
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1); /* main:0808a138 */
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(12, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(10);
    Engine_ActorEnableActionCallback(0, (s32)ShindenHeya_LeaderCircleScript);
    Engine_ActorEnableActionCallback(1, (s32)ShindenHeya_GeraldCircleScript);
    Object_RefreshSelectorById(0); /* main:0808a0a0 */
    Object_RefreshSelectorById(1); /* main:0808a0a0 */
    Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
    Call3(Engine_ActorSetSpeed, 1, 0x18000, 0xc000);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_Jump(ACTOR_GERALD, 6, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Event_Wait(1);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(ACTOR_GERALD, 11, 0);
    Event_Wait(1);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    Event_Wait(1); /* main:0808a138 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 192, 168);
    Actor_WalkToAndWait(ACTOR_GERALD, 208, 168);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 0, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xd000, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 0, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xb000, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 0, 0x3000, 0);
    Engine_ActorFaceDirection(1, 0xd000, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 0, 0x5000, 0);
    Engine_ActorFaceDirection(1, 0xb000, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimationAndWait(12, 3); /* main:0808a110 */
    Event_Wait(30);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Event_Wait(60);
}

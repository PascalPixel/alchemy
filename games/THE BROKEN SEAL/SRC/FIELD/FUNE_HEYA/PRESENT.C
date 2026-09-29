#include "TYPES.H"
#include "FIELD_EVENT.H"

extern const s32 FuneHeya_Script01[];
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
extern u8 MsgFuneCastOff[];
extern u8 MsgFuneOurReplacementNeverArrivedBut[];
extern u8 MsgFuneRowThoseOars[];
extern u8 MsgFuneWereOff[];

/* Message ids. */


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
s32 FuneHeya_PlaceAnchorCharm();
void FieldScene_RunSceneStep();
void ConfigureSceneMotionFlags();
void SceneState_ApplyActor8FourFlags();
void OverlayObject_SetPositionAndHeading();
void FieldScene_CallPairWith10();
u8 *Object_GetByIdFar();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/*
 * Flag-gated scene setup for overlay resource_3b1. Each callee name refers
 * to that call site's own call word rather than to a shared runtime
 * address.
 */

static __inline__ u8 *Pointer1_020038ac(u8 *(*f)(), s32 a)
{
    return f(a);
}

void FieldScene_RunFourActorCoordinatePresentation(void)
{
    u8 *record;
    s32 mode;

    Event_Begin();
    FieldScene_RunSceneStep(25, 0, 0);
    FieldScene_RunSceneStep(24, 1, 0);
    ConfigureSceneMotionFlags(0x1b80000, -1, 0xa80000, 0x1000001);
    OverlayObject_SetPositionAndHeading(27, 0x1b8, 164, 0x5000);
    OverlayObject_SetPositionAndHeading(8, 0x1ac, 190, 0xd000);
    OverlayObject_SetPositionAndHeading(9, 0x1c4, 190, 0xb000);
    Actor_SetAnimation(9, 1);
    mode = 128;
    OverlayObject_SetPositionAndHeading(0, 0x1b8, 134, 0x8000);
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = (mode << 1);
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x198, 148);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a8, 148);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
    Actor_RunRepeatedMotion(27, 1);
    Event_SetMessage((s32)MsgFuneOurReplacementNeverArrivedBut);
    FieldScene_RunStepThen10(27);
    Actor_RunRepeatedMotion(8, 1);
    FieldScene_RunStepThen10(8);
    Actor_SetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    FieldScene_CallPairWith10(27, 0xd000);
    record = Pointer1_020038ac(Object_GetByIdFar, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x1b8, 148);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    record = Pointer1_020038ac(Object_GetByIdFar, 1);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x1c8, 148);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
    record = Pointer1_020038ac(Object_GetByIdFar, 2);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_MIA, 0x1d8, 148);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 20);
    FieldScene_RunSceneStep(0, 0, 60);
    FieldScene_RunSceneStep(1, 0x4000, 20);
    FieldScene_RunSceneStep(2, 1, 20);
    Actor_FaceDirection(27, 0x5000, 20);
    FieldScene_RunStepThen10(27);
    Actor_StartRepeatedMotion(9, 1);
    Actor_ShowEmote(9, (mode << 1), 40);
    FieldScene_RunStepThen10(9);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 3);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 60);
    Actor_SetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    Actor_RunRepeatedMotion(10, 1);
    Actor_SetAnimation(10, 3);
    FieldScene_RunStepThen10(10);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    FieldScene_RunSceneStep(0, 0, 40);
    FieldScene_RunSceneStep(2, 1, 0);
    FieldScene_RunSceneStep(1, 0x4000, 20);
    Actor_SetAnimationAndWait(27, 4);
    FieldScene_RunStepThen10(27);
    Actor_ShowEmote(8, 0x102, 60);
    Actor_StartRepeatedMotion(8, 1);
    FieldScene_RunStepThen10(8);
    Actor_SetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    Actor_FaceDirection(8, 0, 0);
    Actor_FaceDirection(9, 0x8000, 40);
    Actor_ShowEmote(8, 0x102, 0);
    Actor_ShowEmote(8, 0x102, 40);
    Actor_RunRepeatedMotion(27, 1);
    Actor_SetAnimation(27, 3);
    Event_ShowMessageAndWait(27, 0, 20);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(40);
    Actor_ShowEmote(9, (mode << 1), 20);
    FieldScene_CallPairWith10(9, 0xb000);
    FieldScene_RunStepThen10(9);
    FieldScene_CallPairWith10(27, 0x3000);
    Actor_ShowEmote(27, 0x101, 60);
    Event_ShowMessageAndWait(27, 0, 60);
    Actor_ShowEmote(27, 0x106, 20);
    FieldScene_CallPairWith10(27, 0xb000);
    Actor_SetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(3, 2, 80);
    FieldScene_CallPairWith10(8, 0xd000);
    Actor_StartRepeatedMotion(8, 2);
    FieldScene_RunStepThen10(8);
    Actor_SetAnimationAndWait(9, 3);
    Actor_StartRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(9);
    FieldScene_CallPairWith10(27, 0x5000);
    Actor_SetAnimationAndWait(27, 3);
    Actor_RunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    Actor_SetSpeed(27, 0xcccc, 0x6666);
    Actor_WalkToAndWait(27, 0x198, 158);
    Actor_WalkToAndWait(27, 0x198, 148);
    Actor_FaceDirection(27, 0, 20);
    Actor_RunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(1, 0x8000, 20);
    FieldScene_RunSceneStep(2, 1, 0);
    Actor_WalkToAndWait(27, 0x198, 134);
    Actor_WalkTo(27, 0x1b8, 134);
    Event_Wait(40);
    FieldScene_RunSceneStep(9, 10, 0);
    GameFlag_Set(0x926);
}

void FieldScene_RunScene3b1_02003d10(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    FieldScene_RunSceneStep(15, 0, 1);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 0x1d4, 0x266);
    Actor_WalkToAndWait(8, 0x1d8, 0x254);
    Actor_FaceDirection(8, 0x8000, 20);
    Actor_Jump(8, 4, 20);
    rec7 = Value0(FuneHeya_PlaceAnchorCharm);
    Event_Wait(20);
    Audio_PlayCue(214);
    Engine_ObjectSetScript(rec7, FuneHeya_Script01);
    Event_Wait(40);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_WalkToAndWait(8, 0x1d2, 0x270);
    Value2(FieldScene_CallPairWith10, 8, 0x5000);
    Actor_StartRepeatedMotion(8, 2);
    Event_SetMessage((s32)MsgFuneCastOff);
    Event_ShowMessageAndWait(8, 0, 20);
    FieldScene_RunSceneStep(9, 11, 0);
}

void FieldScene_RunScene3b1_02003dec(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    FieldScene_RunSceneStep(15, 1, 1);
    Actor_FaceDirection(8, 0x5000, 40);
    Actor_StartRepeatedMotion(8, 2);
    Event_SetMessage((s32)MsgFuneWereOff);
    Event_ShowMessageAndWait(8, 0, 20);
    FieldScene_RunSceneStep(9, 11, 0);
}

void FieldScene_RunScene3b1_02003e34(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    FieldScene_RunSceneStep(24, 0, 0);
    FieldScene_RunSceneStep(18, 0, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(16, 0x960000, 0x24a0000);
    Call4(ConfigureSceneMotionFlags, 0x9c0000, -1, 0x2180000, 0x1000001);
    FieldScene_RunSceneStep(8, 0, 0);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkToAndWait(16, 168, 0x242);
    Actor_WalkToAndWait(16, 168, 0x22a);
    Actor_FaceDirection(16, 0x8000, 20);
    Actor_StartRepeatedMotion(16, 2);
    Event_SetMessage((s32)MsgFuneRowThoseOars);
    Event_ShowMessageAndWait(16, 0, 20);
    FieldScene_RunSceneStep(9, 12, 0);
}

void FieldScene_RunScene3b1_02003eec(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    SceneState_ApplyActor8FourFlags();
    Actor_SetPosition(18, 0x960000, 0x24a0000);
    Call4(ConfigureSceneMotionFlags, 0x9c0000, -1, 0x2180000, 0x1000001);
    FieldScene_RunSceneStep(8, 0, 0);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_WalkToAndWait(18, 168, 0x242);
    Actor_WalkToAndWait(18, 168, 0x22a);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_StartRepeatedMotion(18, 2);
    Event_SetMessage((s32)MsgFuneRowThoseOars);
    Event_ShowMessageAndWait(18, 0, 20);
    FieldScene_RunSceneStep(9, 12, 0);
}

/*
 * A flat setter cascade, one workspace-slot store, then a four-way gated
 * chain ending in an unconditional default arm. The store spells both its
 * offset and its stored value as 224 << 1 rather than folded constants.
 * The 340-byte owner at 0x02003f94 includes its trailing pool words.
 */
void FieldScene_RunFlagBranchedSetupCascade(void)
{
    extern u8 *Data_03001ebc;

    Event_Begin();
    Actor_SetAnimation(9, 5);
    FieldScene_RunSceneStep(24, 1, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    FieldScene_RunSceneStep(17, 0, 0);
    FieldScene_InstallFlaggedActors10To17(0);
    FieldScene_RunSceneStep(8, 1, 20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x1b80000, -1, 0xb00000, 1);
    Event_Wait(20);
    Actor_SetAnimation(9, 7);
    Event_Wait(30);
    Audio_PlayCue(0xbc);
    Event_Wait(30);
    FieldScene_InstallFlaggedActors10To17(16);
    Event_Wait(0x50);
    FieldScene_InstallFlaggedActors10To17(0);
    Event_Wait(0x3c);
    Actor_SetAnimation(9, 7);
    Event_Wait(30);
    Audio_PlayCue(0xbc);
    Event_Wait(30);
    FieldScene_InstallFlaggedActors10To17(16);
    Event_Wait(0x50);
    FieldScene_InstallFlaggedActors10To17(0);
    Event_Wait(0x5a);
    Audio_PlayCue(0xbc);
    Event_Wait(30);

    *(u32 *)(Data_03001ebc + (224 << 1)) = (224 << 1) + 67;

    FieldScene_RunSceneStep(9, 0, 0);

    if (GameFlag_IsSet(0x92b) != 0) {
        Event_RequestExit(20);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        Event_RequestExit(18);
    } else if (GameFlag_IsSet(0x929) != 0) {
        Event_RequestExit(17);
    } else if (GameFlag_IsSet(0x928) != 0) {
        Event_RequestExit(16);
    } else {
        Event_RequestExit(13);
    }
}

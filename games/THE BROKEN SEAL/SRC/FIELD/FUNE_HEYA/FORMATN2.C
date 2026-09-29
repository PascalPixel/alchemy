#include "TYPES.H"
#include "FIELD_EVENT.H"

extern s32 FuneHeya_CueTimer;
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
extern u8 MsgFuneHeyAreYouOk[];
extern u8 MsgFuneWonderWhatsWrongShipShouldnt[];


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
extern u8 FuneHeya_EntryActionScript[];
extern u8 FuneHeya_ActionScriptC[];
extern u8 FuneHeya_ActionScriptD[];
s32 FuneHeya_FindFirstSetFlag();
void FuneHeya_PlaceFoundActors();
void Object_SetActionCallbackAndRefreshById();
u8 *Object_GetByIdFar();
s32 Scheduler_AddOrUpdateCallback();

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

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void SceneActor_SetFlagBit3ForActors28To35(void);
void FieldScene_InstallFlaggedActors10To17(u8 *src);
void FieldScene_RunStepThen10(s32 a);
void OverlayObject_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d);
void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags);
void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt);

void FieldScene_RunPositionTransferPresentation(void);

void FieldScene_RunFormationAndEffectPresentation(void)
{
    s32 action;

    OverlayObject_SetPositionAndHeading(0, 0x1bc, 0x12c, 0);
    OverlayObject_SetPositionAndHeading(1, 0x1ca, 0x136, 0);
    OverlayObject_SetPositionAndHeading(2, 0x1bc, 0x14a, 0);
    OverlayObject_SetPositionAndHeading(3, 0x1b0, 0x136, 0);
    OverlayObject_SetPositionAndHeading(27, 0x1b8, 134, 0x8000);
    OverlayObject_SetPositionAndHeading(10, 0x1c6, 248, 0x3000);
    Actor_SetAnimation(10, 6);
    ConfigureSceneMotionFlags(0x1b80000, -1, 0x1340000, 0x1000001);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 40);
    FieldScene_RunSceneStep(2, 1, 20);
    Event_SetMessage((s32)MsgFuneHeyAreYouOk);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(1, 0xc000, 0);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x1b80000, -1, 0xb00000, 1);
    Actor_SetSpeed(27, 0x19999, 0xcccc);
    Actor_WalkToAndWait(27, 0x198, 134);
    Actor_WalkToAndWait(27, 0x198, 152);
    Actor_WalkToAndWait(27, 0x1a8, 164);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1b80000, -1, 0x12c0000, 1);
    Actor_WalkToAndWait(27, 0x1a8, 222);
    Actor_WalkToAndWait(27, 0x1a8, 0x106);
    Actor_FaceDirection(27, 0x3000, 20);
    Actor_RunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(2, 1, 20);
    Actor_SetAnimationAndWait(27, 3);
    Actor_RunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(3, 2, 60);
    FieldScene_RunSceneStep(1, 0xe000, 60);
    Actor_FaceDirection(27, 0, 40);
    Actor_RunRepeatedMotion(27, 1);
    Actor_SetAnimation(27, 2);
    Actor_MoveToAndWait(27, 0x1b0, 0x10c);
    Actor_MoveToAndWait(27, 0x1c4, 0x10c);
    Actor_SetAnimation(27, 1);
    FieldScene_CallPairWith10(27, 0xd000);
    Actor_StartRepeatedMotion(27, 2);
    Event_ShowMessageAndWait(27, 0, 20);
    FieldScene_RunSceneStep(1, 0xc000, 20);
    Actor_SetAnimationAndWait(27, 4);
    Event_Wait(40);
    Event_ShowMessageAndWait(27, 0, 80);
    Actor_RunRepeatedMotion(27, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(27, 3);
    Event_Wait(10);
    FieldScene_CallPairWith10(27, 0x5000);
    Event_OpenMessage(27, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(27, 3);
        FieldScene_RunStepThen10(27);
    } else {
        Actor_SetAnimationAndWait(27, 4);
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
        FieldScene_RunStepThen10(27);
        FieldScene_RunSceneStep(3, 2, 40);
        Actor_RunRepeatedMotion(27, 1);
        Actor_SetAnimation(27, 3);
        FieldScene_RunStepThen10(27);
    }
    FieldScene_RunSceneStep(2, 1, 20);
    action = (s32)FuneHeya_ActionScriptC;
    Actor_EnableActionCallback(ACTOR_GERALD, action);
    Actor_EnableActionCallback(ACTOR_IVAN, action);
    Object_SetActionCallbackAndRefreshById(3, action);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x1b80000, -1, 0xb00000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a8, 0x110);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1a8, 164);
    Event_Wait(60);
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x209;
    FieldScene_RunSceneStep(9, 0, 0);
    GameFlag_Clear(0x301);
    GameFlag_Clear(0x927);
    Event_RequestExit(4);
}

void FieldScene_RunActors24And25SetupWithValue929(void)
{
    s32 handle = FuneHeya_FindFirstSetFlag(0, 0);

    Event_Begin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 0, 0);
    FuneHeya_PlaceFoundActors(0);
    FieldScene_RunSceneStep(19, handle, 12);
    Actor_SetPosition(11, 0, 0);
    FieldScene_RunFormationAndEffectPresentation();
    GameFlag_Set(0x929);
    Event_End();
}

void FieldScene_RunScene3b1_020056dc(void)
{
    extern u8 FuneHeya_ActionScriptE[];
    u32 i;
    s32 rec2;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = Value2(FuneHeya_FindFirstSetFlag, 0, 0);
    rec2 = Value2(FuneHeya_FindFirstSetFlag, 1, 0);
    Event_Begin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 3, 0);
    SceneActor_SetFlagBit3ForActors28To35();
    Value3(FieldScene_RunSceneStep, 19, rec8, rec2);
    Actor_SetAnimation(10, 6);
    base5_200e840 = (s32)FuneHeya_ActionScriptE;
    Actor_EnableActionCallback(rec8, base5_200e840);
    Actor_Destroy(11);
    Actor_EnableActionCallback(rec2, base5_200e840);
    Actor_Destroy(12);
    base5_200e8e4 = (s32)FuneHeya_EntryActionScript;
    Actor_EnableActionCallback(36, base5_200e8e4);
    Value2(Engine_ActorEnableActionCallback, 37, base5_200e8e4);
    Actor_SetChildValue(36, 3);
    Actor_SetChildValue(37, 3);
    FieldScene_RunPositionTransferPresentation();
    Event_End();
}

/*
 * A flat setter sequence, no branches. The 108-byte owner at 0x02005780
 * includes its one pool word, the address taken as Value_0000092a.
 */
void FieldScene_RunActors24And25SetupWithValue92a(void)
{
    s32 handle = FuneHeya_FindFirstSetFlag(0, 0);
    s32 other = FuneHeya_FindFirstSetFlag(1, 0);

    Event_Begin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 0, 0);
    FuneHeya_PlaceFoundActors(0);
    FieldScene_RunSceneStep(19, handle, other);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    FieldScene_RunFormationAndEffectPresentation();
    GameFlag_Set(0x92a);
    Event_End();
}

void FieldScene_RunExtendedFormationPresentation(void)
{
    s32 slot_b;
    s32 slot_c;
    s32 slot_a;
    u8 *record;
    s32 action;

    slot_a = Value2(FuneHeya_FindFirstSetFlag, 0, 0);
    slot_b = FuneHeya_FindFirstSetFlag(1, 0);
    slot_c = Value2(FuneHeya_FindFirstSetFlag, 2, 0);
    Event_Begin();
    FieldScene_RunSceneStep(10, 0, 0);
    FieldScene_RunSceneStep(17, 0, 0);
    Actor_SetPosition(8, 0x1d80000, 0x980000);
    Actor_SetAnimation(9, 5);
    Actor_SetPosition(27, 0x1b80000, 0x860000);
    Actor_SetChildValue(27, 15);
    record = Object_GetByIdFar(27);
    Actor_SetSpriteFlags(record, 0);
    FieldScene_InstallFlaggedActors10To17(16);
    ConfigureSceneMotionFlags(0x1b60000, -1, 0xae0000, 0x1000001);
    FieldScene_RunSceneStep(8, 1, 20);
    Audio_PlayCue(19);
    Audio_PlayCue(181);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(80);
    Audio_PlayCue(181);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Audio_PlayCue(63);
    GameFlag_Set(0x11a);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(40);
    FieldScene_CallPairWith10(3, 0x6000);
    Event_SetMessage((s32)MsgFuneWonderWhatsWrongShipShouldnt);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    FieldScene_RunStepThen10(27);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 40);
    Actor_FaceDirection(ACTOR_IVAN, 0xe000, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 60);
    FieldScene_CallPairWith10(2, 0x2000);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    FieldScene_RunStepThen10(2);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xe000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 20);
    Actor_SetChildValue(27, 0);
    record = Object_GetByIdFar(27);
    Actor_SetSpriteFlags(record, 1);
    Actor_SetSpeed(27, 0x10000, 0x8000);
    Actor_WalkToAndWait(27, 0x1ae, 134);
    FieldScene_CallPairWith10(27, 0x3000);
    Actor_StartRepeatedMotion(27, 2);
    FieldScene_RunStepThen10(27);
    Actor_StartRepeatedMotion(slot_a, 1);
    Actor_StartRepeatedMotion(slot_b, 1);
    Actor_StartRepeatedMotion(slot_c, 1);
    Actor_RunRepeatedMotion(13, 1);
    Actor_SetAttachedEffect(slot_a, 0x102);
    Actor_SetAttachedEffect(slot_b, 0x102);
    Actor_SetAttachedEffect(slot_c, 0x102);
    Actor_SetAttachedEffect(13, 0x102);
    Event_Wait(40);
    FieldScene_RunSceneStep(12, slot_a, 0);
    FieldScene_RunSceneStep(12, slot_b, 1);
    FieldScene_RunSceneStep(12, slot_c, 0);
    FieldScene_RunSceneStep(11, 1, 0);
    Actor_FaceDirection(slot_a, 0xd000, 0);
    Actor_FaceDirection(slot_b, 0xb000, 0);
    Actor_FaceDirection(slot_c, 0xd000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 40);
    Actor_StartRepeatedMotion(27, 2);
    Event_ShowMessage(27, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 20);
    Actor_WalkToAndWait(27, 0x1b8, 134);
    Actor_SetPosition(27, 0, 0);
    FieldScene_CallPairWith10(1, 0x8000);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    FieldScene_RunStepThen10(1);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    FieldScene_CallPairWith10(3, 0x8000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    action = (s32)FuneHeya_ActionScriptD;
    Actor_EnableActionCallback(ACTOR_GERALD, action);
    Actor_EnableActionCallback(ACTOR_IVAN, action);
    Object_SetActionCallbackAndRefreshById(3, action);
    GameFlag_Set(0x302);
    FuneHeya_CueTimer = 0;
    Value2(Scheduler_AddOrUpdateCallback, (s32)Scene_UpdateCueTimer, 0xc80);
    FieldScene_RunSceneStep(23, 0, 0);
    Actor_Destroy(27);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Clear(0x927);
    Event_End();
}

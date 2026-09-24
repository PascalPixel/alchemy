#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* Sukureta answers Gerald's "What's wrong, Kraden?" as placed actor 8. */
enum {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED
};

enum SanctumEntranceFlag {
    /* Set after the party is asked whether this is its first visit. */
    FLAG_SOL_SANCTUM_ENTERED = 0x802,
    /* Set once the party decides to help Sukureta look for the hidden passage. */
    FLAG_SEARCHING_FOR_HIDDEN_PASSAGE = 0x804
};

enum SanctumEntranceMessage {
    MSG_THE_DOOR_IS_LOCKED = 0x953,
    MSG_SUKURETA_FIRST_TIME_AT_SOL_SANCTUM = 0xfd3,
    MSG_GERALD_WHATS_WRONG_SUKURETA = 0xfd6,
    MSG_MINOTAUR_RELIEF_ONE_EYE = 0x1031,
    MSG_MINOTAUR_RELIEF_BOTH_EYES = 0x1034,
    MSG_MORE_STATUES_OUT_OF_REACH = 0x103a
};

#define GATE_ID 2081
#define Audio_PlayCue_1(a0) Value1(Engine_AudioPlayCue, a0)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Engine_EventWait, a0)
#define UiText_DrawMessage_1(a0, a1) Value2(Engine_MessageShowCentered, a0, a1)
#define Audio_PlayCue_2(a0) Value1(Engine_AudioPlayCue, a0)
#define BattleRuntime_WaitIfModeZero_3(a0) Value1(Engine_EventWait, a0)
#define BattleRuntime_WaitIfModeZero_4(a0) Value1(Engine_EventWait, a0)
#define BattleRuntime_WaitIfModeZero_5(a0) Value1(Engine_EventWait, a0)
#define UiText_DrawMessage_2(a0, a1) Value2(Engine_MessageShowCentered, a0, a1)
#define Scene_GetRecord_1(a0) Value1(Func_02002b6a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Value3(Engine_ActorSetPosition, a0, a1, a2)
#define Scene_GetRecord_2(a0) Value1(Func_02002b7e, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Value3(Engine_ActorSetPosition, a0, a1, a2)
#define Scene_GetRecord_3(a0) Value1(Func_02002b92, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Value3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_3(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(a0) Value1(Engine_ActorWaitForMove, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_2_late(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_OffsetPositionAndResetMotion_4(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_SetModeById_9(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define BattleRuntime_WaitIfModeZero_3_late(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_4_late(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_OffsetPositionAndResetMotion_5(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(a0) Value1(Engine_ActorWaitForMove, a0)
#define BattleRuntime_WaitIfModeZero_5_late(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_PlaceWithinCameraBounds_4(a0, a1, a2, a3) Value4(Engine_CameraMoveTo, a0, a1, a2, a3)
#define BattleRuntime_WaitIfModeZero_6(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_1(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(a0) Value1(Engine_EventWait, a0)
#define BattleRuntime_WaitIfModeZero_10(a0, a1) Value2(Engine_ActorSetAttachedEffect, a0, a1)
#define BattleRuntime_WaitIfModeZero_11(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_12(a0) Value1(Engine_EventWait, a0)
#define Object_LinkPair_1(a0, a1, a2) Value3(Engine_ActorFaceEachOther, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_13(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_14(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_17(a0) Value1(Engine_EventWait, a0)
#define SceneWork_SetStepValue_2(a0) Value1(Engine_EventSetMessage, a0)
#define BattleRuntime_RunThenWaitIfModeZero_9(a0, a1, a2) Value3(Engine_EventShowMessageAndWait, a0, a1, a2)
#define SceneWork_SetStepValue_3(a0) Value1(Engine_EventSetMessage, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_18(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_4(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_19(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_10(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_20(a0) Value1(Engine_EventWait, a0)
#define BattleRuntime_RunThenWaitIfModeZero_11(a0, a1, a2) Value3(Engine_EventShowMessageAndWait, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_6(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_SetModeById_14(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define BattleRuntime_WaitIfModeZero_21(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_ArmCallback_37(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_SetModeById_15(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define BattleRuntime_WaitIfModeZero_22(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_5(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_23(a0) Value1(Engine_EventWait, a0)
#define Object_SetModeById_18(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define Scene_GetRecord_4(a0) Value1(Func_0200312a, a0)
#define ObjectMotion_ResetAndSetPosition_1(a0, a1, a2) Value3(Engine_ActorSetDestination, a0, a1, a2)
#define Object_SetModeById_19(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define Scene_GetRecord_5(a0) Value1(Func_0200314a, a0)
#define ObjectMotion_ResetAndSetPosition_2(a0, a1, a2) Value3(Engine_ActorSetDestination, a0, a1, a2)
#define Object_SetModeById_20(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define Scene_GetRecord_6(a0) Value1(Func_0200316a, a0)
#define ObjectMotion_ResetAndSetPosition_3(a0, a1, a2) Value3(Engine_ActorSetDestination, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_6(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_SetModeById_23(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define GameFlag_Clear_1(a0) Value1(Engine_GameFlagClear, a0)
#define RECORD_A32(rec) (*(s32 *)((rec) + 8))
#define RECORD_B32(rec) (*(s32 *)((rec) + 16))
#define RECORD_A16(rec) (*(s16 *)((rec) + 10))
#define RECORD_B16(rec) (*(s16 *)((rec) + 18))

extern u8 Value_00000013;
extern u8 Value_00000010;
extern u8 Data_02009d04[];
extern u8 Data_02009d64[];
extern u8 Data_02009cd4[];
extern u8 Data_00000010[];
extern u8 Data_00000013[];
extern u8 Data_02009fc0[];
extern u8 Data_02009fd8[];
extern u8 Data_0200a050[];
extern u8 Data_0200a1b8[];
extern u8 Data_0200a2a8[];
extern s16 Data_02000240_t[][1];
extern u8 Value_00001032;
extern u8 Data_0200a69c[];
extern u8 Value_00000fe0;
extern u8 Data_0200a2e4[];
extern u8 Data_0200a524[];
extern u8 Data_0200a41c[];
extern u8 Data_0200a32c[];
extern u8 Data_0200a2d8[];

void Func_02001c70();
void Func_02001f3a();
void Func_020020fe();
s32 *Func_0200213e(s32);
s32 *Func_0200217e(s32);
s32 *Func_020021be(s32);
s32 *Func_02002206(s32);
s32 *Func_0200224e(s32);
s32 *Func_02002296(s32);
s32 *Func_020022de(s32);
s32 *Func_02002326(s32);
void Func_0200248a();
s32 Func_0200293e_a();
s32 Func_02002952();
s32 Func_02002966();
s32 Func_02002a7e_a();
s32 Func_02002a9e();
s32 Func_02002abe();
s32 Func_02002b6a();
s32 Func_02002b7e();
s32 Func_02002b92();
s32 Func_0200312a();
s32 Func_0200314a();
s32 Func_0200316a();
s32 Func_0200320e();
s32 Func_0200332e();

/*
 * Return the table at 0x02009f14.  The eight-byte owner includes its one
 * pool word.
 */

/* Runs a linear setup sequence once two prior gate checks both pass;
 * either gate failing skips the whole sequence. Every step below is an
 * opaque call into other overlay code, so only the gate values and the
 * shared byte pair are named from what this function itself shows. */

/* Gate id checked before the sequence, and passed again to the call that
 * closes it out. */

/* Contiguous unnamed state-owner run for resource_37f. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/*
 * resource_37f owner at 0x02000f8c, 1,680 bytes: full scene sequence
 * for entities 0, 1, 5 and 8.
 *
 * Complete owner: saved-link prologue at 0x02000f8c through the sole
 * interworking return at 0x02001600, alignment and six-word trailing pool
 * through 0x0200161b.  A branch at 0x020013d0 skips an eleven-word interior
 * constant pool and resumes at 0x02001400.  All 174 BL sites resolve to 30
 * import veneers and are represented below in machine order.
 */

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

/* Two-field record read used when copying a found record's s32 pair into a
 * setter call alongside an entity id. */

/* Two-field record read used when copying a found record's s16 pair into a
 * setter call alongside an entity id. */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Resource 37f owner at 0x020003bc: 76 bytes of code plus a five-word literal
 * pool at 0x0200040c-0x0200041f.  The interworking return pops the saved
 * return address into r0, so the owner returns void.  All seven calls go
 * through import veneers, and GameFlag_IsSet is used only as a predicate.  r5
 * holds the workspace pointer, loaded before the message call and read after
 * it; that is why the owner saves r5.
 */

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */
void Scene_RunActorFormation(s32 a0);

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Additional resolved engine calls: named directly from each target's own
 * reconstructed C source (see the trailing "provisional" comment on each). */
static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Scene_Call(void (*func)(s32), s32 arg)
{
    func(arg);
}

/* Runs the full scene sequence for entities 0, 1, 5 and 8: per-entity setup,
 * a shared movement/pose pass, a branch on one entity's saved state, and a
 * closing pass over the same four entities. */
s32 SceneData_SelectOverlayDataBySelector(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000013) {
        return (s32)Data_02009d04;
    }
    if (v == (s32)&Value_00000010) {
        return (s32)Data_02009d64;
    }
    return (s32)Data_02009cd4;
}

s32 Func_02000070(void)
{
    return 0;
}

u8 *SceneData_GetTable9F14(void)
{
    return (u8 *)0x02009f14;
}

s32 Func_0200007c(void)
{
    s32 base5_2009fd8;
    s32 lo = 11;

    if (Data_02000240_t[224][0] == (s32)Data_00000010) {
        if (Data_02000240_t[225][0] >= lo) {
            if (Data_02000240_t[225][0] > 13) {
                if (Data_02000240_t[225][0] > 16) {
                    goto L_020000ae;
                }
                return (s32)Data_0200a1b8;
            }
            return (s32)Data_0200a050;
        }
        L_020000ae:;
        base5_2009fd8 = (s32)Data_02009fd8;
        Func_02001c70(base5_2009fd8);
        return base5_2009fd8;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000013) {
            return (s32)Data_0200a2a8;
        }
    }
    return (s32)Data_02009fc0;
}

s32 SceneData_SelectFormationTable(void)
{
    s32 lo = 11;

    if (Data_02000240_t[224][0] == (s32)Data_00000013) {
        return (s32)Data_0200a2e4;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000010) {
            if (Data_02000240_t[225][0] >= lo) {
                if (Data_02000240_t[225][0] > 13) {
                    if (Data_02000240_t[225][0] > 16) {
                        goto L_02000128;
                    }
                    return (s32)Data_0200a524;
                }
                return (s32)Data_0200a41c;
            }
            L_02000128:;
            return (s32)Data_0200a32c;
        } else {
        }
    }
    L_0200012e:;
    return (s32)Data_0200a2d8;
}

void FieldScene_RunScene37fSequenceA(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Event_Begin();
    v5 = 3;
    v6 = 2;
    Audio_PlayCue(181);
    Map_CopyCellsTo(16, 28, 21, 3, v5, v6);
    Task_Wait(10);
    Map_CopyCellsTo(16, 30, 21, 3, v5, v6);
    Task_Wait(10);
    Map_CopyCellsTo(16, 32, 21, 3, v5, v6);
    Task_Wait(10);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
    Event_Wait(10);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(2);
    Event_End();
}

void SceneDialogue_RunFlag81aMessageBranch(void)
{
    extern u8 *Data_03001ebc;

    Event_Begin();

    if (GameFlag_IsSet(0x81a) != 0) {
        Message_ShowCentered(MSG_MINOTAUR_RELIEF_BOTH_EYES, 1);
    } else {
        Message_ShowCentered(MSG_MINOTAUR_RELIEF_ONE_EYE, 1);
        if (GameFlag_IsSet(0xf01) != 0) {
            u16 *p = (u16 *)(Data_03001ebc + 370);
            u16 val = 1;
            *p = val;
        }
    }

    Event_End();
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 id;
    s32 v5;
    s32 v6;

    if (GameFlag_IsSet(0xf01) == 0) {
    } else {
        if (GameFlag_IsSet(0x81a) != 0) {
        } else {
            Event_Begin();
            Func_02001f3a();
            v5 = 1;
            Audio_PlayCue(182);
            Map_CopyCellsTo(0, 70, 30, 42, v5, v5);
            Map_Redraw();
            Event_Wait(40);
            id = 0x1032;
            Message_ShowCentered(id, 1);
            Event_Wait(20);
            v6 = 3;
            Audio_PlayCue(183);
            Map_CopyCellsTo(0, 29, 3, 1, v6, 2);
            Map_CopyCellAttributes(0, 29, 3, 2, v6, v5);
            Map_CopyCellsTo(1, 109, 4, 81, v5, v5);
            Map_Redraw();
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Event_Wait(20);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
            Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
            Event_Wait(20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 40);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
            Actor_Jump(ACTOR_PARTY_LEADER, 4, 20);
            Actor_Jump(ACTOR_PARTY_LEADER, 6, 40);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Event_Wait(40);
            Message_ShowCentered(id + 1, 1);
            GameFlag_Set(0x143);
            GameFlag_Set(0x81a);
            Event_End();
        }
    }
}

void FieldScene_RunFlag821Dialogue(void)
{
    extern u8 *Data_03001ebc;

    u8 *work;

    Event_Begin();

    if (GameFlag_IsSet(0x821) != 0) {
        Message_ShowCentered(MSG_MINOTAUR_RELIEF_BOTH_EYES, 1);
    } else if (GameFlag_IsSet(0xf02) != 0) {
        work = Data_03001ebc;
        Message_ShowCentered(MSG_MINOTAUR_RELIEF_ONE_EYE, 1);
        {
            /*
             * The halfword store goes through a pointer local and then an
             * s32 value local, in that order.  Storing the literal straight
             * into the halfword builds the constant in HImode and fetches it
             * from the literal pool, which costs a pool word; splitting the
             * address out first also fixes which register holds it.
             */
            u16 *frame = (u16 *)(work + 370);
            s32 one = 1;
            *frame = (u16)one;
        }
    } else {
        Message_ShowCentered(MSG_MINOTAUR_RELIEF_ONE_EYE, 1);
    }

    Event_End();
}

void FieldScene_RunSupplementalSequenceTwo(void)
{

    s32 byte_pair_addr;

    if (GameFlag_IsSet(3842) == 0) {
    } else {
        if (GameFlag_IsSet(GATE_ID)!= 0) {
        } else {
            Event_Begin();
            Value0(Func_020020fe);
            Audio_PlayCue_1(182);
            Value6(Engine_MapCopyCellsTo, 0, 71, 100, 71, 1, 1);
            Value0(Engine_MapRedraw);
            BattleRuntime_WaitIfModeZero_1(40);
            /* Address of the byte pair starting at Value_00001032. */
            byte_pair_addr = (s32)&Value_00001032;
            UiText_DrawMessage_1(byte_pair_addr, 1);
            Event_Wait(20);
            Audio_PlayCue_2(183);
            Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
            Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
            Call0(Engine_MapRedraw);
            Value3(Engine_WorkSetValuesIfNonNegative, 65536, 65536, 65536);
            BattleRuntime_WaitIfModeZero_3(20);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
            Value3(Engine_WorkSetValuesIfNonNegative, 131072, 131072, 65536);
            BattleRuntime_WaitIfModeZero_4(20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 40);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 32768, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 10);
            Actor_Jump(ACTOR_PARTY_LEADER, 4, 20);
            Actor_Jump(ACTOR_PARTY_LEADER, 6, 40);
            Value3(Engine_WorkSetValuesIfNonNegative, -1, -1, 58982);
            BattleRuntime_WaitIfModeZero_5(40);
            UiText_DrawMessage_2(byte_pair_addr + 1, 1);
            GameFlag_Set(0x143);
            GameFlag_Set(GATE_ID);
            Event_End();
        }
    }
}

void Scene_UpdateOuterActor9Flags(void)
{
    s32 *work = Func_0200213e(9);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Engine_GameFlagClear, 0x302);
    Scene_Call(Engine_GameFlagClear, 0x303);
    if (pos == 93) {
        Scene_Call(Engine_GameFlagSet, 0x303);
    } else if (pos == 95) {
        Scene_Call(Engine_GameFlagSet, 0x302);
    }
}

void Scene_UpdateOuterActor10Flags(void)
{
    s32 *work = Func_0200217e(10);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Engine_GameFlagClear, 0x300);
    Scene_Call(Engine_GameFlagClear, 0x301);
    if (pos == 115) {
        Scene_Call(Engine_GameFlagSet, 0x300);
    } else if (pos == 113) {
        Scene_Call(Engine_GameFlagSet, 0x301);
    }
}

void Scene_UpdateFormationActor9Flags(void)
{
    s32 *work = Func_020021be(9);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Engine_GameFlagClear, 0x310);
    Scene_Call(Engine_GameFlagClear, 0x311);
    if (pos == 99) {
        Scene_Call(Engine_GameFlagSet, 0x311);
    } else if (pos == 101) {
        Scene_Call(Engine_GameFlagSet, 0x310);
    }
    Scene_Call(Scene_RunActorFormation, 0);
}

void Scene_UpdateFormationActor10Flags(void)
{
    s32 *work = Func_02002206(10);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Engine_GameFlagClear, 0x312);
    Scene_Call(Engine_GameFlagClear, 0x313);
    if (pos == 103) {
        Scene_Call(Engine_GameFlagSet, 0x313);
    } else if (pos == 105) {
        Scene_Call(Engine_GameFlagSet, 0x312);
    }
    Scene_Call(Scene_RunActorFormation, 0);
}

void Scene_UpdateFormationActor11Flags(void)
{
    s32 *work = Func_0200224e(11);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Engine_GameFlagClear, 0x314);
    Scene_Call(Engine_GameFlagClear, 0x315);
    if (pos == 107) {
        Scene_Call(Engine_GameFlagSet, 0x315);
    } else if (pos == 109) {
        Scene_Call(Engine_GameFlagSet, 0x314);
    }
    Scene_Call(Scene_RunActorFormation, 0);
}

void Scene_UpdateFormationActor12Flags(void)
{
    s32 *work = Func_02002296(12);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Engine_GameFlagClear, 0x316);
    Scene_Call(Engine_GameFlagClear, 0x317);
    if (pos == 111) {
        Scene_Call(Engine_GameFlagSet, 0x317);
    } else if (pos == 113) {
        Scene_Call(Engine_GameFlagSet, 0x316);
    }
    Scene_Call(Scene_RunActorFormation, 0);
}

void Scene_UpdateFormationActor13Flags(void)
{
    s32 *work = Func_020022de(13);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Engine_GameFlagClear, 0x318);
    Scene_Call(Engine_GameFlagClear, 0x319);
    if (pos == 115) {
        Scene_Call(Engine_GameFlagSet, 0x319);
    } else if (pos == 117) {
        Scene_Call(Engine_GameFlagSet, 0x318);
    }
    Scene_Call(Scene_RunActorFormation, 0);
}

void Scene_UpdateFormationActor14Flags(void)
{
    s32 *work = Func_02002326(14);
    s32 pos;

    if (work == 0) return;
    pos = work[2] >> 20;
    Scene_Call(Engine_GameFlagClear, 0x31a);
    Scene_Call(Engine_GameFlagClear, 0x31b);
    if (pos == 119) {
        Scene_Call(Engine_GameFlagSet, 0x31b);
    } else if (pos == 121) {
        Scene_Call(Engine_GameFlagSet, 0x31a);
    }
    Scene_Call(Scene_RunActorFormation, 0);
}

s32 *SceneActor_FindSlotByTilePosition(s32 x, s32 z)
{
    extern u8 *Data_03001ebc;

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

s32 FieldScene_DispatchByScenarioId(void)
{
    s32 scenario = gGameState.scene;

    if (scenario == (s32)&Value_00000013) {
        FieldScene_RunScene37f_0200092c();
    } else if (scenario == (s32)&Value_00000010) {
        FieldScene_RunSceneEntryHook();
    }
    return 0;
}

void FieldScene_RunScene37f_0200092c(void)
{
    extern u8 Data_03001ebc[];

    GameFlag_Set(0x144);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    if (GameFlag_IsSet(0x814) != 0) {
        s32 zero = 0;
        *(volatile s32 *)Data_0200a69c = zero;
        Call2(Func_0200248a, 0x2009ac9, 0xc80);
    }
    if (GameFlag_IsSet(0x879) != 0) {
        Map_CopyCellAttributes(5, 6, 1, 1, 6, 6);
        Map_CopyCellAttributes(5, 6, 1, 1, 7, 6);
        Map_CopyCellAttributes(5, 6, 1, 1, 8, 6);
        Map_CopyCellAttributes(0, 1, 3, 1, 6, 5);
    }
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Actor_SetPosition(8, 0x780000, 0xe80000);
        Map_CopyCellAttributes(2, 10, 1, 1, 6, 14);
        Map_CopyCellAttributes(2, 10, 1, 1, 7, 14);
        Map_CopyCellAttributes(2, 10, 1, 1, 8, 14);
    }
}

void Scene_EnterSolSanctum(void)
{
    s32 record;

    Event_Begin();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_OpenScreen();
    ((void (*)())Engine_ActorSetAnimation)(0, 0);
    Event_Wait(4);
    Camera_MoveTo(-1, -1, -1, 0);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x4c80000, -1, 0x880000, 1);
    record = Value1(Func_0200293e_a, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_SUKURETA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02002952, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_JASMINE, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02002966, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Actor_SetDestinationOffset(ACTOR_GERALD, -16, 0);
    Actor_SetDestinationOffset(ACTOR_JASMINE, 16, 0);
    Actor_SetDestinationOffset(ACTOR_SUKURETA, 0, -32);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 0);
    Actor_SetAnimation(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xc000, 0);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 20);
    Actor_Jump(ACTOR_SUKURETA, 4, 20);
    Event_SetMessage(MSG_SUKURETA_FIRST_TIME_AT_SOL_SANCTUM);
    Event_AskYesNo(0x4008, 0);
    Event_Wait(20);
    Camera_MoveTo(0x4c80000, -1, 0x940000, 1);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Value1(Func_02002a7e_a, 0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    record = Value1(Func_02002a9e, 0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    record = Value1(Func_02002abe, 0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_SUKURETA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    Event_End();
}

void Scene_SukuretaSuspectsHiddenPassage(void)
{
    u8 *record;

    Event_Begin();
    Event_OpenScreen(); /* main:0808a360 */
    Event_WaitForScreen(); /* main:0808a370 */

    record = Scene_GetRecord_1(0);
    if (record != 0)
        ObjectMotion_SetHorizontalPositionWithTerrain_1(8, RECORD_A32(record), RECORD_B32(record));
    record = Scene_GetRecord_2(0);
    if (record != 0)
        ObjectMotion_SetHorizontalPositionWithTerrain_2(5, RECORD_A32(record), RECORD_B32(record));
    record = Scene_GetRecord_3(0);
    if (record != 0)
        ObjectMotion_SetHorizontalPositionWithTerrain_3(1, RECORD_A32(record), RECORD_B32(record));

    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Actor_SetDestinationOffset(ACTOR_GERALD, -16, 0);
    Actor_SetDestinationOffset(ACTOR_JASMINE, 16, 0);
    ObjectMotion_OffsetPositionAndResetMotion_3(8, 0, -16);
    ObjectMotion_CommitCurrentPositionAndActivate_1(8);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_GERALD, 0);
    Actor_SetAnimation(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 2);
    BattleRuntime_WaitIfModeZero_2_late(10);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    ObjectMotion_OffsetPositionAndResetMotion_4(8, 0, -16);
    ObjectMotion_CommitCurrentPositionAndActivate_2(8);
    Object_SetModeById_9(8, 1);
    BattleRuntime_WaitIfModeZero_3_late(6);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(8, 2);
    BattleRuntime_WaitIfModeZero_4_late(20);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    ObjectMotion_OffsetPositionAndResetMotion_5(8, 0, -32);
    ObjectMotion_CommitCurrentPositionAndActivate_3(8);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);

    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x06310000, -1, 0x00960000, 1);
    Camera_WaitForMove();
    BattleRuntime_WaitIfModeZero_5_late(10);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0x06550000, -1, 0x00640000, 1);
    Camera_WaitForMove();
    Camera_MoveTo(0x06b60000, -1, 0x00640000, 1);
    Camera_WaitForMove();
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    ObjectMotion_PlaceWithinCameraBounds_4(0x06d80000, -1, 0x00960000, 1);
    Camera_WaitForMove();
    BattleRuntime_WaitIfModeZero_6(40);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x06840000, -1, 0x01000000, 1);
    Camera_WaitForMove();
    ObjectMotion_CallThenWaitForAnimationChange_1(8, 3);
    BattleRuntime_WaitIfModeZero_7(10);

    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 20);
    Event_SetMessage(MSG_GERALD_WHATS_WRONG_SUKURETA);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 60);
    Actor_StartRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_10(5, 0x102); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_11(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(8, 2);
    BattleRuntime_WaitIfModeZero_12(20);
    Event_ShowMessage(ACTOR_SUKURETA, 0);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Object_LinkPair_1(0, 5, 0);
    BattleRuntime_WaitIfModeZero_13(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(5, 1);
    BattleRuntime_WaitIfModeZero_14(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 20);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(8, 1);
    BattleRuntime_WaitIfModeZero_15(10);
    Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102); /* main:0808a1f0 */
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(8, 2);
    BattleRuntime_WaitIfModeZero_17(10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 20);
    Actor_Jump(ACTOR_SUKURETA, 2, 20);

    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 40);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    Event_OpenMessage(ACTOR_GERALD, 0); /* main:0808a178 */
    if (Event_ChooseYesNo(0, 0) == 0) {
        /* Passes the address of Value_00000fe0 in place of a record pointer. */
        SceneWork_SetStepValue_2((s32)&Value_00000fe0);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        BattleRuntime_RunThenWaitIfModeZero_9(1, 0, 10);
    } else {
        SceneWork_SetStepValue_3(0xfe1);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 20);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 40);
        ObjectMotion_SetVariantCallbackAndInvokeObject_9(1, 1);
        BattleRuntime_WaitIfModeZero_18(10);
        ObjectMotion_CallThenWaitForAnimationChange_4(1, 3);
        BattleRuntime_WaitIfModeZero_19(10);
        Actor_FaceDirection(ACTOR_GERALD, 0, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 30);
        ObjectMotion_SetVariantCallbackAndInvokeObject_10(1, 1);
        BattleRuntime_WaitIfModeZero_20(10);
        BattleRuntime_RunThenWaitIfModeZero_11(1, 0, 10);
    }

    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    ObjectMotion_OffsetPositionAndResetMotion_6(8, 0, 48);
    ObjectMotion_CommitCurrentPositionAndActivate_4(8);
    Object_SetModeById_14(8, 1);
    BattleRuntime_WaitIfModeZero_21(6);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    ObjectMotion_ArmCallback_37(5, 0xa000, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_5(8);
    Object_SetModeById_15(8, 1);
    BattleRuntime_WaitIfModeZero_22(20);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_JASMINE, 3);
    ObjectMotion_CallThenWaitForAnimationChange_5(0, 3);
    BattleRuntime_WaitIfModeZero_23(6);
    Object_SetModeById_18(1, 2);

    record = Scene_GetRecord_4(0);
    if (record != 0)
        ObjectMotion_ResetAndSetPosition_1(1, RECORD_A16(record), RECORD_B16(record));
    Object_SetModeById_19(5, 2);
    record = Scene_GetRecord_5(0);
    if (record != 0)
        ObjectMotion_ResetAndSetPosition_2(5, RECORD_A16(record), RECORD_B16(record));
    Object_SetModeById_20(8, 2);
    record = Scene_GetRecord_6(0);
    if (record != 0)
        ObjectMotion_ResetAndSetPosition_3(8, RECORD_A16(record), RECORD_B16(record));

    ObjectMotion_CommitCurrentPositionAndActivate_6(8);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Object_SetModeById_23(5, 1);
    GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
    GameFlag_Clear_1(FLAG_ARRIVAL_EVENT_PENDING);
    Event_End();
}

void Scene_RunTransitionCue(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x204;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    record = Value1(Func_0200320e, 0);
    if (record != 0) {
        Actor_SetPosition(8, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_SetAnimation(8, 2);
    Actor_SetDestinationOffset(8, 24, -10);
    Actor_WaitForMove(8);
    Actor_SetAnimation(8, 1);
    Event_Wait(6);
    Actor_FaceDirection(8, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x6880000, -1, 0x20c0000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x7580000, -1, 0x20c0000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0x6e90000, -1, 0x2240000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0, 30);
    Event_SetMessage(MSG_MORE_STATUES_OUT_OF_REACH);
    Event_ShowMessageAndWait(0x4008, 0, 10);
    Actor_ShowEmote(8, 0x100, 40);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(0x4008, 0, 10);
    Actor_SetAnimation(8, 2);
    record = Value1(Func_0200332e, 0);
    if (record != 0) {
        Actor_SetDestination(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(8);
    Actor_SetPosition(8, 0, 0);
    GameFlag_Set(0x825);
    Event_End();
}

void Scene_RunActorFormation(s32 a0)
{
    u32 i;
    s32 record;

    Value6(Engine_MapCopyCellAttributes, 122, 20, 1, 1, 100, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 104, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 108, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 112, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 116, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 120, 32);
    if (GameFlag_IsSet(0x311) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
        if (a0 == 0) {
            goto L_02001890;
        }
        Actor_SetPosition(9, 0x6380000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x310) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
            if (a0 != 0) {
                Actor_SetPosition(9, 0x6580000, 0x2080000);
            }
        }
    }
    L_02001890:;
    if (GameFlag_IsSet(0x313) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
        if (a0 == 0) {
            goto L_020018f2;
        }
        Actor_SetPosition(10, 0x6780000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x312) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
            if (a0 != 0) {
                Actor_SetPosition(10, 0x6980000, 0x2080000);
            }
        }
    }
    L_020018f2:;
    if (GameFlag_IsSet(0x315) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
        if (a0 == 0) {
            goto L_02001956;
        }
        Actor_SetPosition(11, 0x6b80000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x314) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
            if (a0 != 0) {
                Actor_SetPosition(11, 0x6d80000, 0x2080000);
            }
        }
    }
    L_02001956:;
    if (GameFlag_IsSet(0x317) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
        if (a0 == 0) {
            goto L_020019b8;
        }
        Actor_SetPosition(12, 0x6f80000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x316) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
            if (a0 != 0) {
                Actor_SetPosition(12, 0x7180000, 0x2080000);
            }
        }
    }
    L_020019b8:;
    if (GameFlag_IsSet(0x319) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
        if (a0 == 0) {
            goto L_02001a1c;
        }
        Actor_SetPosition(13, 0x7380000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x318) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
            if (a0 != 0) {
                Actor_SetPosition(13, 0x7580000, 0x2080000);
            }
        }
    }
    L_02001a1c:;
    if (GameFlag_IsSet(0x31b) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
        if (a0 == 0) {
            goto L_02001a7e;
        }
        Actor_SetPosition(14, 0x7780000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x31a) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
            if (a0 != 0) {
                Actor_SetPosition(14, 0x7980000, 0x2080000);
            }
        }
    }
    L_02001a7e:;
}

void FieldScene_RunScriptedStep953(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_THE_DOOR_IS_LOCKED, 1);
    Event_End();
}

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 record;
    s32 value;
    s32 base5_200a69c;
    s32 v3;

    base5_200a69c = 0x200a69c;
    if (*(s32 *)base5_200a69c != 0) {
        v3 = (*(s32 *)base5_200a69c - 1);
        *(s32 *)base5_200a69c = (*(s32 *)base5_200a69c - 1);
        if (v3 != 40) {
            goto L_02001b14;
        }
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    } else {
        value = Value0(Engine_RandomNext);
        if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
            Audio_PlayCue(138);
            Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
            *(s32 *)base5_200a69c = 80;
        }
    }
    L_02001b14:;
}

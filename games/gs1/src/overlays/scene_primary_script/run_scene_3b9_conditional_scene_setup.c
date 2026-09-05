#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02002e32();
s32 Func_02003d8a();
void Func_02003da8();
void Func_02003dbc();
s32 Func_02003e1a();
void Func_02003e62();
void Func_02003e70();
void Func_02003e74();
void Func_02003e78();
void Func_02003e9a();
void Func_02003eb2();
void Func_02003f00();
s32 Func_02003f06();
void Func_02003f1a();
void Func_02003f26();
void Func_02003f64();
void Func_02003f6c();
void Func_02003f78();
void Func_02003f84();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02003dbc(args)
#define GameFlag_IsSet_1(a0) Value1(Func_02003d8a, a0)
#define Object_LinkPair_1(args...) Func_02003e62(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02003e78, a0)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02003eb2, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02003e74(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_02003e9a, a0)
#define BattleEvent_RunActionAndWait_1(args...) Func_02003eb2(args)
#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_02003f06(args)
#define RuntimeBlock_GetOffset1e0Pointer_2(args...) Func_02003f06(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02003f00, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02003f1a, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02003f26(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_02003f84(args)
#define GameFlag_IsSet_2(a0) Value1(Func_02003e1a, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02003e70(args)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))

/* A second word at 0x1c8 of the shared scene work record, set only on the
 * flag-clear path below. */
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)0x03001ebc + 0x1c8))

/* Record id passed to every Func_02003exx call below. */
#define RECORD_17 17

/* Reads flag record 0x8a4; when set, runs one short setup on record 17.
 * When clear, runs a longer setup on record 17 plus scene phase/field
 * updates, then checks flag record 0x8a3 to pick a final call. Either path
 * ends with Func_02003e70(). */
void FieldScene_RunConditionalSceneSetup(void)
{
    u32 i;
    s32 flag_8a4;
    s32 record;

    BattleRuntime_Reset_1();
    flag_8a4 = GameFlag_IsSet_1(0x8a4);
    if (flag_8a4 != 0) {
        Object_LinkPair_1(RECORD_17, 0, 40);
        SceneWork_SetStepValue_1(0x206f);
        Func_02002e32(RECORD_17);
        ObjectMotion_ArmCallback_1(RECORD_17, 0x3000, 20);
    } else {
        ObjectMotion_SetVariantCallback_1(RECORD_17, 2);
        SceneWork_SetStepValue_2(0x206d);
        BattleEvent_RunActionAndWait_1(RECORD_17, 0);
        /* Byte at +85 of the record returned by RuntimeBlock_GetOffset1e0Pointer_1(); written
         * with the (already known zero) flag value here. */
        *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_2() + 85) = flag_8a4;
        Func_02003da8(1);
        ObjectMotion_SetSpeedLimitAndAcceleration_1(0x66666, 0xcccc);
        ObjectMotion_PlaceWithinCameraBounds_1(0x21c0000, -1, 0xd00000, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        SCENE_PHASE = 0x200;
        SCENE_FIELD_1C8 = 32;
        Func_02003f78();
        ObjectMotion_SetSpeedParameters_1();
        if (GameFlag_IsSet_2(0x8a3) != 0) {
            Func_02003f64(70);
        } else {
            Func_02003f6c(7);
        }
    }
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

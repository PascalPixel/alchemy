#include "types.h"

#define FieldScene_RunFlagGatedActorDialogue Func_02000ff0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002409[];
s32 Func_020029b8();
void Func_020029ce();
void Func_020029ea();
s32 Func_02002a10();
void Func_02002a14();
void Func_02002a68();
void Func_02002a6e();
s32 Func_02002a7c();
void Func_02002a7e();
void Func_02002a86();
void Func_02002a8a();
void Func_02002a9e();
void Func_02002aa0();
void Func_02002ac0();
void Func_02002ad2();
void Func_02002ad8();
void Func_02002ae8();
void Func_02002b04();
void Func_02002b06();
void Func_02002b0c();
void Func_02002b14();
void Func_02002b1e();
void Func_02002b22();
void Func_02002b28();
void Func_02002b36();
void Func_02002b3c();
void Func_02002b46();
void Func_02002b48();
void Func_02002b4c();
void Func_02002b52();
void Func_02002b54();
void Func_02002b56();
void Func_02002b60();
void Func_02002b6c();
void Func_02002b78();
void Func_02002b84();
void Func_02002b8e();
void Func_02002b96();
void Func_02002b9c();
void Func_02002ba2();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(a0) Value1(Func_020029b8, a0)
#define GameFlag_Set_1(a0) Call1(Func_020029ce, a0)
#define BattleRuntime_Reset_1(args...) Func_020029ea(args)
#define Scene_GetRecord_1(args...) Func_02002a10(args)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Value3(Func_02002a7c, a0, a1, a2)
#define ObjectMotion_SetAngleToward_2(args...) Func_02002a86(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02002a7e(args)
#define ObjectMotion_SetVariantCallback_2(args...) Func_02002a86(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02002a14(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02002ae8, a0, a1, a2)
#define SceneWork_SetStepValue_1(args...) Func_02002ac0(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02002ad8(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02002a6e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02002a7c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02002a8a, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02002ad2(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02002a68(args)
#define SceneWork_SetStepValue_2(args...) Func_02002b06(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_02002b1e(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02002b48, a0, a1, a2)
#define SceneWork_SetStepValue_3(args...) Func_02002b1e(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_02002b36(args)
#define Object_SetModeById_2(args...) Func_02002b06(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02002a9e(args)
#define SceneWork_SetStepValue_4(args...) Func_02002b3c(args)
#define BattleEvent_RunActionAndWait_4(args...) Func_02002b54(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1(args...) Func_02002b04(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02002b22(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2(args...) Func_02002b14(args)
#define ObjectMotion_ResetAndSetPositionInMode2_3(args...) Func_02002b1e(args)
#define ObjectMotion_ResetAndSetPositionInMode2_4(args...) Func_02002b28(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_02002b46(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(args...) Func_02002b4c(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(args...) Func_02002b52(args)
#define ObjectMotion_SetAngleToward_3(args...) Func_02002b84(args)
#define ObjectMotion_SetAngleToward_4(args...) Func_02002b8e(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02002b0c(args)
#define ObjectMotion_ResetAndSetPositionInMode2_5(a0, a1, a2) Call3(Func_02002b60, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_6(a0, a1, a2) Call3(Func_02002b6c, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_7(a0, a1, a2) Call3(Func_02002b78, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5(args...) Func_02002b96(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_6(args...) Func_02002b9c(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_7(args...) Func_02002ba2(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02002b56(args)

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* One-shot flag gate: the scene flag this function is named for. */
#define DIALOGUE_GATE_FLAG 0x244

/* Actor ids referenced throughout this scene. */
#define ACTOR_8 8
#define ACTOR_9 9

/* Runs once per save: if DIALOGUE_GATE_FLAG is already set the function does
 * nothing, otherwise it sets the flag and then positions actor 0's record,
 * actor 8, and actor 9, feeds four sequential bytes of the Data_00002409
 * buffer to their setup calls, and steps actors 8/9 and camera-target 0
 * through a sequence of move/pose calls. */
void FieldScene_RunFlagGatedActorDialogue(void)
{
    u32 unused_counter;
    s32 actor0_record;
    s32 unused_record;
    s32 script_data;

    if (GameFlag_IsSet_1(DIALOGUE_GATE_FLAG) != 0) {
    } else {
        GameFlag_Set_1(DIALOGUE_GATE_FLAG);
        BattleRuntime_Reset_1();
        actor0_record = Scene_GetRecord_1(0);
        ObjectMotion_SetAngleToward_1(ACTOR_8, 0, 0);
        ObjectMotion_SetAngleToward_2(ACTOR_9, 0, 0);
        ObjectMotion_SetVariantCallback_1(ACTOR_8, 1);
        ObjectMotion_SetVariantCallback_2(ACTOR_9, 1);
        BattleRuntime_WaitIfModeZero_1(20);
        BattleEffect_SpawnLinkedResourceObject_1(ACTOR_8, 0x102, 60);
        script_data = (s32)Data_00002409;
        SceneWork_SetStepValue_1(script_data);
        BattleEvent_RunActionAndWait_1(ACTOR_8, 0);
        ObjectMotion_SetSpeedParameters_1(0, 0x20000, 0x10000);
        ObjectMotion_SetSpeedParameters_2(ACTOR_8, 0x20000, 0x10000);
        ObjectMotion_SetSpeedParameters_3(ACTOR_9, 0x20000, 0x10000);
        Object_SetModeById_1(ACTOR_9, 4);
        BattleRuntime_WaitIfModeZero_2(35);
        SceneWork_SetStepValue_2((script_data + 1));
        BattleEvent_RunActionAndWait_2(ACTOR_9, 0);
        BattleEffect_SpawnLinkedResourceObject_2(ACTOR_8, 0x103, 30);
        SceneWork_SetStepValue_3((script_data + 2));
        BattleEvent_RunActionAndWait_3(ACTOR_8, 0);
        Object_SetModeById_2(ACTOR_9, 3); /* object_id ACTOR_9, action 3 */
        BattleRuntime_WaitIfModeZero_3(25);
        SceneWork_SetStepValue_4((script_data + 3));
        BattleEvent_RunActionAndWait_4(ACTOR_9, 0);
        /* +10 and +18 of actor 0's record read as halfwords: the first,
         * minus one, and the second are fed to actor 8's placement call. */
        ObjectMotion_ResetAndSetPositionInMode2_1(ACTOR_8, (*(s16 *)(actor0_record + 10) - 1),
                       *(s16 *)(actor0_record + 18));
        ObjectMotion_CommitCurrentPositionAndActivate_1(ACTOR_8);
        ObjectMotion_ResetAndSetPositionInMode2_2(0, 160, 216);
        ObjectMotion_ResetAndSetPositionInMode2_3(ACTOR_8, 152, 200); /* object_id ACTOR_8, x 152, z 200 */
        ObjectMotion_ResetAndSetPositionInMode2_4(ACTOR_9, 168, 200);
        ObjectMotion_CommitCurrentPositionAndActivate_2(ACTOR_8);
        ObjectMotion_CommitCurrentPositionAndActivate_3(ACTOR_9);
        ObjectMotion_CommitCurrentPositionAndActivate_4(0);
        ObjectMotion_SetAngleToward_3(ACTOR_8, 0, 0);
        ObjectMotion_SetAngleToward_4(ACTOR_9, 0, 0);
        BattleRuntime_WaitIfModeZero_4(12);
        ObjectMotion_ResetAndSetPositionInMode2_5(0, 160, 0x110);
        ObjectMotion_ResetAndSetPositionInMode2_6(ACTOR_8, 152, 0x100);
        ObjectMotion_ResetAndSetPositionInMode2_7(ACTOR_9, 168, 0x100);
        ObjectMotion_CommitCurrentPositionAndActivate_5(ACTOR_8);
        ObjectMotion_CommitCurrentPositionAndActivate_6(ACTOR_9);
        ObjectMotion_CommitCurrentPositionAndActivate_7(0);
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
        Call2(Func_02002aa0, 0x2009241, 0xc80); /* main:080000d0 */
    }
}

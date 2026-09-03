#include "types.h"

#define FieldScene_RunCompanionActorSequence Func_020031b4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200e65c[];
void Func_02009086();
void Func_020090a6();
void Func_020090c2();
void Func_02009110();
void Func_0200911c();
void Func_02009128();
void Func_0200915e();
void Func_02009174();
void Func_02009184();
s32 Func_020091a2();
s32 Func_020091ae();
s32 Func_020091ba();
void Func_02009208();
s32 Func_02009216();
void Func_02009228();
void Func_02009230();
void Func_02009236();
void Func_02009238();
s32 Func_02009242();
void Func_0200925a();
void Func_020092b6();
void Func_020092e4();
void Func_020092e8();
void Func_0200930a();
void Func_0200930e();
void Func_02009314();
void Func_02009338();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02009174(args)
#define Scene_GetRecord_1(args...) Func_020091a2(args)
#define Scene_GetRecord_2(args...) Func_020091ae(args)
#define Scene_GetRecord_3(args...) Func_020091ba(args)
#define Object_SetModeById_1(args...) Func_02009228(args)
#define Object_SetModeById_2(args...) Func_02009230(args)
#define Object_SetModeById_3(args...) Func_02009238(args)
#define ObjectMotion_EnableActionAndSetCallback_1(args...) Func_02009208(args)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_02009216, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(a0, a1) Value2(Func_02009242, a0, a1)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_0200930e, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_020092b6(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_0200930a, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020092e8, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_0200930a(args)
#define ObjectMotion_SetAngleToward_1(args...) Func_020092e4(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02009314(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02009338, a0, a1, a2)
#define GameFlag_Set_1(a0) Call1(Func_02009236, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_0200925a(args)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Actor IDs the scene sets up: three actors share a common data pointer,
 * a fourth actor (11) is driven through its own sequence of position/speed
 * calls. */
#define ACTOR_A 12
#define ACTOR_B 13
#define ACTOR_C 14
#define ACTOR_D 11

/* Shared data used for actors A, B, C. */
#define SHARED_DATA ((s32)Data_0200e65c)

/* Sets up actors 12, 13, 14 and 20 with shared data and movement/speed
 * parameters, then drives actor 11 through a further sequence of moves. */
void FieldScene_RunCompanionActorSequence(void)
{
    u32 i;
    s32 actor_data;
    s32 shared_data;

    BattleRuntime_Reset_1();
    actor_data = Scene_GetRecord_1(ACTOR_A);
    Func_02009110(actor_data, 0);
    actor_data = Scene_GetRecord_2(ACTOR_B);
    Func_0200911c(actor_data, 0);
    actor_data = Scene_GetRecord_3(ACTOR_C);
    Func_02009128(actor_data, 0);
    Object_SetModeById_1(ACTOR_A, 0);
    Object_SetModeById_2(ACTOR_B, 0);
    Object_SetModeById_3(ACTOR_C, 0);
    Func_02009086(20);
    Call3(Func_0200915e, 0x20000, 0x20000, 0x10000);
    shared_data = SHARED_DATA;
    ObjectMotion_EnableActionAndSetCallback_1(ACTOR_A, shared_data);
    Func_020090a6(10);
    ObjectMotion_EnableActionAndSetCallback_2(ACTOR_B, shared_data);
    Call3(Func_02009184, -1, -1, 0xe666);
    Func_020090c2(20);
    ObjectMotion_MarkActiveAndSetActionCallback_1(ACTOR_C, shared_data);
    BattleEffect_SpawnLinkedResourceObject_1(ACTOR_D, 0x100, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(ACTOR_D, 2);
    ObjectMotion_ArmCallback_1(ACTOR_D, 0xd000, 10);
    SceneWork_SetStepValue_1(0x1c90);
    BattleRuntime_RunThenWaitIfModeZero_1(ACTOR_D, 0, 40);
    ObjectMotion_SetAngleToward_1(ACTOR_D, 0, 20);
    BattleEvent_RunActionAndWait_1(ACTOR_D, 0);
    ObjectMotion_ArmCallback_2(ACTOR_D, 0x8000, 10);
    GameFlag_Set_1(0x305);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

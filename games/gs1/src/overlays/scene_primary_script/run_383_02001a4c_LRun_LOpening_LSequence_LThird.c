#include "types.h"

#define FieldScene_RunOpeningSequenceThird Func_02001a4c

struct SceneRecord {
    u8 pad[100];
    u16 value100;
};
void Func_020060d2();
void Func_020060d6();
void Func_020060ee();
void Func_020060fe();
void Func_020067a6();
void *Scene_GetRecord_1();
void *Scene_GetRecord_2();
void Func_020067da();
void Func_020067dc();
void Func_020067f0();
void Func_020067f8();
void Func_020067fa();
void Func_02006804();
void Func_02006810();
void Func_0200682e();
void Func_02006834();
void Func_02006840();
void Func_0200684c();
void Func_02006872();
void Func_0200687a();
void Func_0200689c();
void Func_020068aa();
void Func_020068ba();
void Func_020068c2();
void Func_020068c4();
void Func_020068ca();
void Func_020068d0();
void Func_020068e6();
void Func_020068ee();
void Func_020068f2();
void Func_020068fe();
void Func_02006930();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Scene_GetRecord_1(args...) Func_020067d0(args)
#define Scene_GetRecord_2(args...) Func_020067d8(args)
#define BattleRuntime_Reset_1() Call0(Func_020067a6)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020067f0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_020067fa, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02006804, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02006840, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_0200684c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1(a0) Call1(Func_020067da, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_020068e6, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020068f2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_2(a0) Call1(Func_020067f8, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(a0, a1) Call2(Func_020068ca, a0, a1)
#define BattleRuntime_WaitIfModeZero_3(a0) Call1(Func_02006810, a0)
#define SceneWork_SetStepValue_1(a0) Value1(Func_020068ee, a0)
#define BattleRuntime_WaitIfModeZero_4(a0, a1) Call2(Func_02006930, a0, a1)
#define BattleRuntime_WaitIfModeZero_5(a0) Call1(Func_0200682e, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(a0, a1) Call2(Func_020068fe, a0, a1)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_0200689c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_020068aa, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_020068ba, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_020068c2, a0, a1)
#define Object_LookupAndStep_1(a0) Call1(Func_020068d0, a0)
#define GameFlag_Set_1(a0) Value1(Func_02006872, a0)
#define GameFlag_Set_2(a0) Call1(Func_0200687a, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Value0(Func_020068c4)
void *Func_020067d0();
void *Func_020067d8();
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Pair of ratio-like arguments shared by three setup calls below (each
 * applied to a different index: 0, 1, 2). */
#define RATIO_HI 52428
#define RATIO_LO 26214

/* Third scene step: sets up actors 24 and 25 (fetching each one's record),
 * runs a shared series of configuration calls touching actors 0-2, 10, 14,
 * 20, 24 and 25, then marks the two fetched records with a byte flag. */
void Func_02001a4c(void)
{
    void *actor24;
    void *actor25;

    actor24 = Scene_GetRecord_1(24);
    actor25 = Scene_GetRecord_2(25);
    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(0, RATIO_HI, RATIO_LO);
    ObjectMotion_SetSpeedParameters_2(1, RATIO_HI, RATIO_LO);
    ObjectMotion_SetSpeedParameters_3(2, RATIO_HI, RATIO_LO);
    ObjectMotion_SetPositionAndReset_1(0, 232, 696);
    ObjectMotion_SetPositionAndReset_2(0, 200, 696);
    BattleRuntime_WaitIfModeZero_1(10);
    BattleEffect_SpawnLinkedResourceObject_1(25, 256, 0);
    BattleEffect_SpawnLinkedResourceObject_2(24, 256, 0);
    BattleRuntime_WaitIfModeZero_2(60);
    Call3(Func_020060d2, 25, 0, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(24, 2);
    BattleRuntime_WaitIfModeZero_3(20);
    SceneWork_SetStepValue_1(4758);
    Call2(Func_020060d6, 24, 20);
    BattleRuntime_WaitIfModeZero_4(25, 258); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_5(60);
    Call2(Func_020060ee, 25, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(24, 1);
    Call2(Func_020060fe, 24, 30);
    ObjectMotion_SetSpeedParameters_4(24, 262144, 131072);
    ObjectMotion_SetSpeedParameters_5(25, 229376, 114688);
    ObjectMotion_EnableActionAndSetCallback_1(25, 33609776);
    ObjectMotion_EnableActionAndSetCallback_2(24, 33609056);
    Object_LookupAndStep_1(24);
    Call6(Func_02006834, 14, 45, 3, 1, 14, 44); /* main:080091c0 */
    GameFlag_Set_1(2130);
    GameFlag_Set_2(768);
    Call2(Func_020067dc, 33598369, 3200); /* main:080000d0 */
    /* Byte flag at +100 of each record: 1 for actor 24's, 3 for actor 25's. */
    ((struct SceneRecord *)actor24)->value100 = 1;
    ((struct SceneRecord *)actor25)->value100 = 3;
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_0200259a();
extern u8 Value_0200b1d6;
/* Layout of the record returned for an actor: only the word at offset
 * 0x64 (100) is touched here. */
struct SceneRecord {
    u8 pad[100];
    u16 field_0x64;
};
void Func_020025b6();
void Func_020025c2();
void Func_020025ca();
s32 Func_020025cc();
void Func_020025d6();
void Func_020025ec();
void Func_020025f4();
void Func_02002630();
void Func_02002632();
void *Scene_GetRecord_1();
void Func_0200266c();
void *ObjectMotion_SetAngleToward_1();
void Func_02002684();
void Func_02002690();
void Func_0200269a();
void Func_020026a4();
void Func_020026a8();
void Func_020026b2();
void Func_020026b4();
void Func_020026bc();
void Func_020026c8();
void Func_020026dc();
void Func_020026e8();
void Func_020026ec();
void *Scene_GetRecord_2();
void Func_0200270c();
void Func_02002718();
void Func_0200271a();
void Func_0200272a();
void Func_02002744();
void Func_02002754();
void Func_02002760();
void Func_02002762();
void Func_02002764();
void Func_02002782();
void Func_020027ae();
void Func_020027c2();
void Func_020027c6();
void Func_020027ca();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(a0) Value1(Func_0200259a, a0)
#define BattleRuntime_Reset_1() Value0(Func_020025c2)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Value3(Func_0200267c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Func_020025ca, a0)
#define SceneWork_SetStepValue_1(a0) Value1(Func_020026a8, a0)
#define BattleEventRuntime_ProcessAction_1(a0, a1) Value2(Func_020026c8, a0, a1)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Value0(Func_020025ec)
#define GameFlag_IsSet_2(a0) Value1(Func_020025cc, a0)
#define BattleRuntime_Reset_2() Value0(Func_020025f4)
#define Object_SetModeById_1(a0, a1) Call2(Func_02002684, a0, a1)
#define SceneWork_SetStepValue_2(a0) Value1(Func_020026dc, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_020026ec, a0, a1)
#define Object_SetModeById_2(a0, a1) Call2(Func_020026a4, a0, a1)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_2() Call0(Func_02002632)
#define BattleRuntime_Reset_3() Value0(Func_02002630)
#define Scene_GetRecord_1(a0) Value1(Func_0200264e, a0)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_0200266c, a0, a1)
#define SceneWork_SetStepValue_3(a0) Value1(Func_0200271a, a0)
#define BattleEvent_RunActionAndWait_2(a0, a1) Value2(Func_0200272a, a0, a1)
#define ObjectMotion_EnableActionAndResetMotion_1(a0) Value1(Func_02002690, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02002764, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02002760, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(a0, a1) Call2(Func_02002718, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_1(a0, a1, a2) Call3(Func_02002762, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_020026b2, a0, a1)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020026b4, a0, a1, a2)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(a0, a1) Value2(Func_020026dc, a0, a1)
#define BattleRuntime_WaitIfModeZero_2(a0) Value1(Func_0200269a, a0)
#define ObjectMotion_Launch_1(a0, a1, a2) Call3(Func_02002744, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_2(a0, a1) Call2(Func_02002754, a0, a1)
#define BattleRuntime_WaitIfModeZero_3(a0, a1) Value2(Func_020027c6, a0, a1)
#define BattleRuntime_WaitIfModeZero_4(a0) Value1(Func_020026bc, a0)
#define BattleRuntime_RunThenWaitIfModeZero_2(a0, a1, a2) Call3(Func_020027ae, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020027ca, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_3(a0, a1) Call2(Func_02002782, a0, a1)
#define BattleEvent_RunActionAndWait_3(a0, a1) Value2(Func_020027c2, a0, a1)
#define Scene_GetRecord_2(a0) Value1(Func_02002708, a0)
#define GameFlag_Set_1(a0) Value1(Func_020026e8, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_3() Value0(Func_0200270c)
void *Func_0200267c();
void *Func_0200264e();
void *Func_02002708();

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

/* Byte offset of a flags field on the actor 8 record touched below; bit 1
 * of that byte is toggled at the end of the third branch. */
#define ACTOR_FLAGS_OFFSET 89

/* Picks one of three scripted call sequences depending on two condition
 * checks (codes 2177 and 2091), each acting on actor 9 and/or actor 8. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    void *actor9_record;
    void *unused_actor9_record;
    void *actor8_record;

    if (GameFlag_IsSet_1(2177) != 0) {
        BattleRuntime_Reset_1();
        unused_actor9_record = ObjectMotion_SetAngleToward_1(9, 0, 0);
        BattleRuntime_WaitIfModeZero_1(10);
        SceneWork_SetStepValue_1(5700);
        BattleEventRuntime_ProcessAction_1(9, 0);
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    } else {
        if (GameFlag_IsSet_2(2091) != 0) {
            BattleRuntime_Reset_2();
            Object_SetModeById_1(9, 7);
            Value3(Func_020025b6, 33599936, 10, 69);
            SceneWork_SetStepValue_2(5484);
            BattleEvent_RunActionAndWait_1(9, 0);
            Object_SetModeById_2(9, 8);
            Call3(Func_020025d6, (s32)&Value_0200b1d6, 10, 69);
            BattleRuntime_ScheduleShoulderButtonModeUpdate_2();
        } else {
            BattleRuntime_Reset_3();
            actor9_record = Scene_GetRecord_1(9);
            ((struct SceneRecord *)actor9_record)->field_0x64 = 10;
            ObjectMotion_EnableActionAndSetCallback_1(9, 33596660);
            SceneWork_SetStepValue_3(5428);
            BattleEvent_RunActionAndWait_2(9, 0);
            ObjectMotion_EnableActionAndResetMotion_1(8);
            BattleEffect_SpawnLinkedResourceObject_1(8, 256, 40);
            ObjectMotion_ArmCallback_1(8, 53248, 10);
            ObjectMotion_SetVariantCallback_1(8, 2);
            BattleRuntime_RunThenWaitIfModeZero_1(8, 0, 20);
            ObjectMotion_EnableActionAndSetCallback_2(0, 33596772);
            ObjectMotion_SetSpeedParameters_1(8, 104857, 52428);
            ObjectMotion_MarkActiveAndSetActionCallback_1(8, 33596680);
            BattleRuntime_WaitIfModeZero_2(40);
            ObjectMotion_Launch_1(8, 2, 0);
            ObjectMotion_SetVariantCallback_2(8, 2);
            BattleRuntime_WaitIfModeZero_3(8, 258);
            BattleRuntime_WaitIfModeZero_4(60);
            BattleRuntime_RunThenWaitIfModeZero_2(8, 0, 10);
            ObjectMotion_ArmCallback_2(8, 12288, 20);
            ObjectMotion_SetVariantCallback_3(8, 2);
            BattleEvent_RunActionAndWait_3(8, 0);
            actor8_record = Scene_GetRecord_2(8);
            *(u8 *)((u8 *)(actor8_record) + ACTOR_FLAGS_OFFSET) ^= 0x2;
            GameFlag_Set_1(0x82c);
            BattleRuntime_ScheduleShoulderButtonModeUpdate_3();
        }
    }
}

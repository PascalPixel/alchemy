#include "types.h"

#define FieldScene_RunLateAuxiliarySequence Func_02002f10

extern u8 Data_02000240[];
extern u8 Value_00000056;
void Func_020069d4();
void Func_02006a84();
void Func_02006a9c();
void Func_02006ad8();
void Func_02006b06();
void Func_02006b0e();
void Func_02006b18();
void Func_02006b1c();
void Func_02006b4c();
void Func_02006b58();
void Func_02006b70();
void Func_02006b94();
void Func_02006b98();
void Func_02006bae();
void Func_02006bba();
void Func_02006c0a();
void Func_02006c6a();
void Func_02006c72();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1() Value0(Func_02006a84)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02006b94, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02006bae, a0, a1, a2, a3)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02006ad8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Value3(Func_02006b1c, a0, a1, a2)
#define Audio_PlayCue_1(a0) Value1(Func_02006c6a, a0)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02006b0e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02006b18, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_02006b70, a0, a1)
#define ObjectMotion_ResetAndSetPosition_1(a0, a1, a2) Call3(Func_02006b4c, a0, a1, a2)
#define ObjectMotion_ResetAndSetPosition_2(a0, a1, a2) Value3(Func_02006b58, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Func_02006b06, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02006c0a, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(a0, a1) Value2(Func_02006bba, a0, a1)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(a0) Value1(Func_02006b98, a0)
#define SharedWorkData_SetFirstAndSecondFields_1(a0, a1) Call2(Func_02006c72, a0, a1)
#define BattleEffect_ComputeWeightedResultAndDispatch_1(a0, a1) Call2(Func_02006c6a, a0, a1)

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

static __inline__ void Call0(void (*f)())
{
    f();
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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* Byte at offset 0x22B of the shared work buffer. */
#define WORK_BYTE_22B (Data_02000240[0x22B])

/* Runs a fixed chain of 19 calls with literal arguments, sets byte 0x22B of
 * the shared work buffer to 3, then issues two more calls. */
void FieldScene_RunLateAuxiliarySequence(void)
{
    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedLimitAndAcceleration_1(39321, 4915);
    ObjectMotion_PlaceWithinCameraBounds_1(21495808, -1, 5701632, 1);
    ObjectMotion_SetSpeedParameters_1(0, 39321, 19660);
    ObjectMotion_SetPositionAndReset_1(0, 328, 116);
    Audio_PlayCue_1(148);
    Value2(Func_020069d4, 33599213, 3200); /* main:080000d0 */
    Call3(Func_02006a9c, 65536, 65536, 65536); /* main:080091f0 */
    ObjectMotion_SetSpeedParameters_2(8, 6553, 3276);
    ObjectMotion_SetSpeedParameters_3(9, 6553, 3276);
    Object_SetModeById_1(8, 2);
    ObjectMotion_ResetAndSetPosition_1(8, 328, 104);
    ObjectMotion_ResetAndSetPosition_2(9, 328, 108);
    BattleRuntime_WaitIfModeZero_1(60);
    BattleEffect_SpawnLinkedResourceObject_1(0, 256, 0);
    ObjectMotion_SetVariantCallback_1(0, 2);
    ObjectMotion_CommitCurrentPositionAndActivate_1(8);
    do {
        WORK_BYTE_22B = 3;
    } while (0);
    SharedWorkData_SetFirstAndSecondFields_1((s32)&Value_00000056, 99);
    BattleEffect_ComputeWeightedResultAndDispatch_1(53, 3);
}

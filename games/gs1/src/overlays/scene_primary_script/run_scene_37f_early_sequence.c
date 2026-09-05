#include "types.h"

/* Runs a linear setup sequence once two prior gate checks both pass;
 * either gate failing skips the whole sequence. Every step below is an
 * opaque call into other overlay code, so only the gate values and the
 * shared byte pair are named from what this function itself shows. */

/* Gate id checked before the sequence, and passed again to the call that
 * closes it out. */
#define GATE_ID 2081

extern u8 Value_00001032;
void Func_02001fa2();
s32 Func_02001fb4();
void Func_02001fb6();
s32 Func_02001fc0();
void Func_02001fe8();
void Func_02001fea();
void Func_02001ff2();
void Func_02002004();
void Func_02002008();
void Func_02002018();
void Func_02002030();
void Func_02002052();
void Func_0200205e();
void Func_02002080();
void Func_020020a8();
void Func_020020c0();
void Func_020020d6();
void Func_020020d8();
void Func_020020dc();
void Func_020020fe();
void Func_02002100();
void Func_02002114();
void Func_02002128();
void Func_02002132();
void Func_02002144();
void Func_02002146();
void Func_02002150();
void Func_0200215a();
void Func_02002166();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(a0) Value1(Func_02001fb4, a0)
#define GameFlag_IsSet_2(a0) Value1(Func_02001fc0, a0)
#define BattleRuntime_Reset_1() Call0(Func_02001fea)
#define Audio_PlayCue_1(a0) Value1(Func_02002114, a0)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Func_02002008, a0)
#define UiText_DrawMessage_1(a0, a1) Value2(Func_02001ff2, a0, a1)
#define BattleRuntime_WaitIfModeZero_2(a0) Call1(Func_02002018, a0)
#define Audio_PlayCue_2(a0) Value1(Func_02002146, a0)
#define BattleRuntime_WaitIfModeZero_3(a0) Value1(Func_0200205e, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02002132, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(a0) Value1(Func_02002080, a0)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02002144, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02002150, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_0200215a, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02002166, a0, a1, a2)
#define ObjectMotion_Launch_1(a0, a1, a2) Call3(Func_02002128, a0, a1, a2)
#define ObjectMotion_Launch_2(a0, a1, a2) Call3(Func_02002132, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5(a0) Value1(Func_020020d8, a0)
#define UiText_DrawMessage_2(a0, a1) Value2(Func_020020c0, a0, a1)
#define GameFlag_Set_1(a0) Value1(Func_020020d6, a0)
#define GameFlag_Set_2(a0) Call1(Func_020020dc, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Call0(Func_02002100)

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

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

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
    s32 byte_pair_addr;

    if (GameFlag_IsSet_1(3842) == 0) {
    } else {
        if (GameFlag_IsSet_2(GATE_ID) != 0) {
        } else {
            BattleRuntime_Reset_1();
            Value0(Func_020020fe);
            Audio_PlayCue_1(182);
            Value6(Func_02001fb6, 0, 71, 100, 71, 1, 1);
            Value0(Func_02001fa2);
            BattleRuntime_WaitIfModeZero_1(40);
            /* Address of the byte pair starting at Value_00001032. */
            byte_pair_addr = (s32)&Value_00001032;
            UiText_DrawMessage_1(byte_pair_addr, 1);
            BattleRuntime_WaitIfModeZero_2(20);
            Audio_PlayCue_2(183);
            Func_02001fe8(122, 20, 120, 30, 1, 2);
            Call6(Func_02002004, 122, 20, 1, 2, 120, 30);
            Call0(Func_02001fe8);
            Value3(Func_02002030, 65536, 65536, 65536);
            BattleRuntime_WaitIfModeZero_3(20);
            BattleEffect_SpawnLinkedResourceObject_1(0, 256, 0);
            Value3(Func_02002052, 131072, 131072, 65536);
            BattleRuntime_WaitIfModeZero_4(20);
            ObjectMotion_ArmCallback_1(0, 16384, 40);
            ObjectMotion_ArmCallback_2(0, 32768, 20);
            ObjectMotion_ArmCallback_3(0, 0, 20);
            ObjectMotion_ArmCallback_4(0, 16384, 10);
            ObjectMotion_Launch_1(0, 4, 20);
            ObjectMotion_Launch_2(0, 6, 40);
            Value3(Func_020020a8, -1, -1, 58982);
            BattleRuntime_WaitIfModeZero_5(40);
            UiText_DrawMessage_2(byte_pair_addr + 1, 1);
            GameFlag_Set_1(0x143);
            GameFlag_Set_2(GATE_ID);
            BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
        }
    }
}

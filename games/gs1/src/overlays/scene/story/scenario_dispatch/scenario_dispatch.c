#include "types.h"

/* overlays/scene/story/scenario_dispatch/run_scene_early_sequence.c */
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
void Func_02001fe8_a();
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
void Func_02002132_a();
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
#define Battle_Reset_1() Call0(Func_02001fea)
#define Audio_PlayCue_1(a0) Value1(Func_02002114, a0)
#define Battle_WaitMode0_1(a0) Value1(Func_02002008, a0)
#define UiText_DrawMessage_1(a0, a1) Value2(Func_02001ff2, a0, a1)
#define Battle_WaitMode0_2(a0) Call1(Func_02002018, a0)
#define Audio_PlayCue_2(a0) Value1(Func_02002146, a0)
#define Battle_WaitMode0_3(a0) Value1(Func_0200205e, a0)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_02002132, a0, a1, a2)
#define Battle_WaitMode0_4(a0) Value1(Func_02002080, a0)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02002144, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02002150, a0, a1, a2)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_0200215a, a0, a1, a2)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_02002166, a0, a1, a2)
#define Motion_Launch_1(a0, a1, a2) Call3(Func_02002128, a0, a1, a2)
#define Motion_Launch_2(a0, a1, a2) Call3(Func_02002132_a, a0, a1, a2)
#define Battle_WaitMode0_5(a0) Value1(Func_020020d8, a0)
#define UiText_DrawMessage_2(a0, a1) Value2(Func_020020c0, a0, a1)
#define GameFlag_Set_1(a0) Value1(Func_020020d6, a0)
#define GameFlag_Set_2(a0) Call1(Func_020020dc, a0)
#define Battle_SchedShoulder_1() Call0(Func_02002100)

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

void Scene_RunSupplementalSequenceTwo(void)
{
    s32 byte_pair_addr;

    if (GameFlag_IsSet_1(3842) == 0) {
    } else {
        if (GameFlag_IsSet_2(GATE_ID)!= 0) {
        } else {
            Battle_Reset_1();
            Value0(Func_020020fe);
            Audio_PlayCue_1(182);
            Value6(Func_02001fb6, 0, 71, 100, 71, 1, 1);
            Value0(Func_02001fa2);
            Battle_WaitMode0_1(40);
            /* Address of the byte pair starting at Value_00001032. */
            byte_pair_addr = (s32)&Value_00001032;
            UiText_DrawMessage_1(byte_pair_addr, 1);
            Battle_WaitMode0_2(20);
            Audio_PlayCue_2(183);
            Func_02001fe8(122, 20, 120, 30, 1, 2);
            Call6(Func_02002004, 122, 20, 1, 2, 120, 30);
            Call0(Func_02001fe8_a);
            Value3(Func_02002030, 65536, 65536, 65536);
            Battle_WaitMode0_3(20);
            BattleFx_SpawnLinked_1(0, 256, 0);
            Value3(Func_02002052, 131072, 131072, 65536);
            Battle_WaitMode0_4(20);
            Motion_ArmCb_1(0, 16384, 40);
            Motion_ArmCb_2(0, 32768, 20);
            Motion_ArmCb_3(0, 0, 20);
            Motion_ArmCb_4(0, 16384, 10);
            Motion_Launch_1(0, 4, 20);
            Motion_Launch_2(0, 6, 40);
            Value3(Func_020020a8, -1, -1, 58982);
            Battle_WaitMode0_5(40);
            UiText_DrawMessage_2(byte_pair_addr + 1, 1);
            GameFlag_Set_1(0x143);
            GameFlag_Set_2(GATE_ID);
            Battle_SchedShoulder_1();
        }
    }
}
#undef GATE_ID
#undef GameFlag_IsSet_1
#undef GameFlag_IsSet_2
#undef Battle_Reset_1
#undef Audio_PlayCue_1
#undef Battle_WaitMode0_1
#undef UiText_DrawMessage_1
#undef Battle_WaitMode0_2
#undef Audio_PlayCue_2
#undef Battle_WaitMode0_3
#undef BattleFx_SpawnLinked_1
#undef Battle_WaitMode0_4
#undef Motion_ArmCb_1
#undef Motion_ArmCb_2
#undef Motion_ArmCb_3
#undef Motion_ArmCb_4
#undef Motion_Launch_1
#undef Motion_Launch_2
#undef Battle_WaitMode0_5
#undef UiText_DrawMessage_2
#undef GameFlag_Set_1
#undef GameFlag_Set_2
#undef Battle_SchedShoulder_1

/* overlays/scene/story/scenario_dispatch/run_scene_late_sequence.c */
extern u8 Value_00000fe0;
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
int Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0e0();
void Func_0808a0e8();
s32 Func_0808a0f0();
s32 Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
s32 Func_0808a138();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
s32 Func_0808a1b8();
s32 Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a360();
void Func_0808a370();

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
s32 Func_02002b3c();
s32 Func_02002b6a();
s32 Func_02002b7e();
s32 Func_02002b92();
s32 Func_02002ba8();
s32 Func_02002bb2();
s32 Func_02002bbc();
s32 Func_02002bbc_a();
s32 Func_02002bc6();
s32 Func_02002bd0();
s32 Func_02002bfe();
s32 Func_02002c02();
s32 Func_02002c06();
s32 Func_02002c0c();
s32 Func_02002c0e();
s32 Func_02002c18();
s32 Func_02002c26();
s32 Func_02002c38();
s32 Func_02002c3e();
s32 Func_02002c46();
s32 Func_02002c4c();
s32 Func_02002c4e();
s32 Func_02002c56();
s32 Func_02002cb4();
s32 Func_02002cba();
s32 Func_02002cc6();
s32 Func_02002cd2();
s32 Func_02002cdc();
s32 Func_02002cde();
s32 Func_02002ce8();
s32 Func_02002cf4();
s32 Func_02002d00();
s32 Func_02002d0c();
s32 Func_02002d0c_a();
s32 Func_02002d10();
s32 Func_02002d18();
s32 Func_02002d1c();
s32 Func_02002d1e();
s32 Func_02002d22();
s32 Func_02002d2e();
s32 Func_02002d2e_a();
s32 Func_02002d36();
s32 Func_02002d3a();
s32 Func_02002d46();
s32 Func_02002d52();
s32 Func_02002d54();
s32 Func_02002d5e();
s32 Func_02002d68();
s32 Func_02002d74();
s32 Func_02002d76();
s32 Func_02002d86();
s32 Func_02002d8e();
s32 Func_02002da0();
s32 Func_02002da8();
s32 Func_02002daa();
s32 Func_02002db6();
s32 Func_02002dd2();
s32 Func_02002dec();
s32 Func_02002e12();
s32 Func_02002e2a();
s32 Func_02002e34();
s32 Func_02002e34_a();
s32 Func_02002e36();
s32 Func_02002e4c();
s32 Func_02002e58();
s32 Func_02002e5e();
s32 Func_02002e60();
s32 Func_02002e6c();
s32 Func_02002e6c_a();
s32 Func_02002e7e();
s32 Func_02002e88();
s32 Func_02002e8a();
s32 Func_02002e8a_a();
s32 Func_02002e90();
s32 Func_02002e96();
s32 Func_02002e96_a();
s32 Func_02002ea0();
s32 Func_02002ea2();
s32 Func_02002ea2_a();
s32 Func_02002eaa();
s32 Func_02002eac();
s32 Func_02002eac_a();
s32 Func_02002eac_b();
s32 Func_02002eb2();
s32 Func_02002ebc();
s32 Func_02002eca();
s32 Func_02002ed8();
s32 Func_02002ee4();
s32 Func_02002ee6();
s32 Func_02002f14();
s32 Func_02002f14_a();
s32 Func_02002f16();
s32 Func_02002f18();
s32 Func_02002f18_a();
s32 Func_02002f1e();
s32 Func_02002f26();
s32 Func_02002f28();
s32 Func_02002f2e();
s32 Func_02002f46();
s32 Func_02002f4c();
s32 Func_02002f52();
s32 Func_02002f70();
s32 Func_02002f7c();
s32 Func_02002f7e();
s32 Func_02002f90();
s32 Func_02002f92();
s32 Func_02002fa6();
s32 Func_02002fac();
s32 Func_02002fc2();
s32 Func_02002fcc();
s32 Func_02002fe2();
s32 Func_02002fe6();
s32 Func_02002fe6_a();
s32 Func_02002ff0();
s32 Func_02002ff2();
s32 Func_02002ffc();
s32 Func_02003004();
s32 Func_02003020();
s32 Func_0200303e();
s32 Func_0200304c();
s32 Func_02003054();
s32 Func_02003056();
s32 Func_02003058();
s32 Func_02003060();
s32 Func_0200306e();
s32 Func_02003070();
s32 Func_0200307a();
s32 Func_0200308a();
s32 Func_020030a8();
s32 Func_020030aa();
s32 Func_020030ae();
s32 Func_020030ae_a();
s32 Func_020030b4();
s32 Func_020030b8();
s32 Func_020030c4();
s32 Func_020030d0();
s32 Func_020030d6();
s32 Func_020030dc();
s32 Func_020030de();
s32 Func_020030ea();
s32 Func_020030ec();
s32 Func_020030ec_a();
s32 Func_020030f4();
s32 Func_020030f8();
s32 Func_02003104();
s32 Func_0200310e();
s32 Func_02003118();
s32 Func_0200311a();
s32 Func_02003122();
s32 Func_0200312a();
s32 Func_02003130();
s32 Func_0200313e();
s32 Func_02003146();
s32 Func_0200314a();
s32 Func_0200314c();
s32 Func_02003156();
s32 Func_0200315c();
s32 Func_0200316a();
s32 Func_0200316c();
s32 Func_0200316e();
s32 Func_0200317a();
s32 Func_0200317c();
s32 Func_02003186();
s32 Func_0200318c();
s32 Func_02003194();
s32 Func_0200319c();
s32 Func_020031aa();
s32 Func_020031b0();
s32 Func_020031bc();
s32 Func_020031c6();
s32 Func_020031d0();
s32 Func_020031e0();
s32 Func_020031e8();
s32 Func_020031f0();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Battle_Reset_1() Value0(Func_02002b3c)
#define Battle_WaitMode0_1() Value0(Func_02002c38)
#define Motion_SetSpeed_1() Value0(Func_02002c4c)
#define Scene_GetRecord_1(a0) Value1(Func_02002b6a, a0)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Value3(Func_02002ba8, a0, a1, a2)
#define Scene_GetRecord_2(a0) Value1(Func_02002b7e, a0)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Value3(Func_02002bbc, a0, a1, a2)
#define Scene_GetRecord_3(a0) Value1(Func_02002b92, a0)
#define Motion_SetHPosTerrain_3(a0, a1, a2) Value3(Func_02002bd0, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02002bb2, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02002bbc_a, a0, a1, a2)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_02002bc6, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_02002bfe, a0, a1)
#define Object_SetModeById_2(a0, a1) Call2(Func_02002c06, a0, a1)
#define Object_SetModeById_3(a0, a1) Call2(Func_02002c0e, a0, a1)
#define Motion_OffsetPositionAndResetMotion_1(a0, a1, a2) Call3(Func_02002c02, a0, a1, a2)
#define Motion_OffsetPositionAndResetMotion_2(a0, a1, a2) Call3(Func_02002c0c, a0, a1, a2)
#define Motion_OffsetPositionAndResetMotion_3(a0, a1, a2) Value3(Func_02002c18, a0, a1, a2)
#define Motion_CommitPos_1(a0) Value1(Func_02002c26, a0)
#define Object_SetModeById_4(a0, a1) Call2(Func_02002c3e, a0, a1)
#define Object_SetModeById_5(a0, a1) Call2(Func_02002c46, a0, a1)
#define Object_SetModeById_6(a0, a1) Call2(Func_02002c4e, a0, a1)
#define Object_SetModeById_7(a0, a1) Call2(Func_02002c56, a0, a1)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02002cba, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02002cc6, a0, a1, a2)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_02002cd2, a0, a1, a2)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_02002cde, a0, a1, a2)
#define Motion_ArmCb_5(a0, a1, a2) Call3(Func_02002ce8, a0, a1, a2)
#define Motion_ArmCb_6(a0, a1, a2) Call3(Func_02002cf4, a0, a1, a2)
#define Motion_ArmCb_7(a0, a1, a2) Call3(Func_02002d00, a0, a1, a2)
#define Motion_ArmCb_8(a0, a1, a2) Call3(Func_02002d0c, a0, a1, a2)
#define Motion_ArmCb_9(a0, a1, a2) Call3(Func_02002d18, a0, a1, a2)
#define Motion_ArmCb_10(a0, a1, a2) Call3(Func_02002d22, a0, a1, a2)
#define Motion_ArmCb_11(a0, a1, a2) Call3(Func_02002d2e, a0, a1, a2)
#define Motion_ArmCb_12(a0, a1, a2) Call3(Func_02002d3a, a0, a1, a2)
#define Motion_ArmCb_13(a0, a1, a2) Call3(Func_02002d46, a0, a1, a2)
#define Motion_ArmCb_14(a0, a1, a2) Call3(Func_02002d52, a0, a1, a2)
#define Motion_ArmCb_15(a0, a1, a2) Call3(Func_02002d5e, a0, a1, a2)
#define Motion_SetVarCbObj_1(a0, a1) Value2(Func_02002d2e_a, a0, a1)
#define Battle_WaitMode0_2(a0) Value1(Func_02002cb4, a0)
#define Object_SetModeById_8(a0, a1) Call2(Func_02002d1c, a0, a1)
#define Motion_OffsetPositionAndResetMotion_4(a0, a1, a2) Value3(Func_02002d10, a0, a1, a2)
#define Motion_CommitPos_2(a0) Value1(Func_02002d1e, a0)
#define Object_SetModeById_9(a0, a1) Value2(Func_02002d36, a0, a1)
#define Battle_WaitMode0_3(a0) Value1(Func_02002cdc, a0)
#define Motion_ArmCb_16(a0, a1, a2) Call3(Func_02002da0, a0, a1, a2)
#define Motion_ArmCb_17(a0, a1, a2) Call3(Func_02002daa, a0, a1, a2)
#define Motion_ArmCb_18(a0, a1, a2) Call3(Func_02002db6, a0, a1, a2)
#define Motion_SetVarCbObj_2(a0, a1) Value2(Func_02002d86, a0, a1)
#define Battle_WaitMode0_4(a0) Value1(Func_02002d0c_a, a0)
#define Object_SetModeById_10(a0, a1) Call2(Func_02002d74, a0, a1)
#define Motion_OffsetPositionAndResetMotion_5(a0, a1, a2) Value3(Func_02002d68, a0, a1, a2)
#define Motion_CommitPos_3(a0) Value1(Func_02002d76, a0)
#define Object_SetModeById_11(a0, a1) Call2(Func_02002d8e, a0, a1)
#define Motion_SetSpeedLim_1(a0, a1) Call2(Func_02002e12, a0, a1)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_02002e2a, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1() Value0(Func_02002e36)
#define Battle_WaitMode0_5(a0) Value1(Func_02002d54, a0)
#define Motion_SetSpeedLim_2(a0, a1) Call2(Func_02002e34, a0, a1)
#define Motion_CamBounds_2(a0, a1, a2, a3) Call4(Func_02002e4c, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_2() Call0(Func_02002e58)
#define Motion_CamBounds_3(a0, a1, a2, a3) Call4(Func_02002e60, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_3() Call0(Func_02002e6c)
#define Object_SetModeById_12(a0, a1) Call2(Func_02002dec, a0, a1)
#define Motion_CamBounds_4(a0, a1, a2, a3) Value4(Func_02002e7e, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_4() Value0(Func_02002e8a)
#define Battle_WaitMode0_6(a0) Value1(Func_02002da8, a0)
#define Motion_SetSpeedLim_3(a0, a1) Call2(Func_02002e88, a0, a1)
#define Motion_CamBounds_5(a0, a1, a2, a3) Call4(Func_02002ea0, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_5() Call0(Func_02002eac)
#define Motion_CallWaitAnim_1(a0, a1) Value2(Func_02002e34_a, a0, a1)
#define Battle_WaitMode0_7(a0) Value1(Func_02002dd2, a0)
#define Motion_ArmCb_19(a0, a1, a2) Call3(Func_02002e96, a0, a1, a2)
#define Motion_ArmCb_20(a0, a1, a2) Call3(Func_02002ea2, a0, a1, a2)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_02002ebc, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02002e8a_a, a0)
#define Battle_RunThenWaitIfModeZero_1(a0, a1, a2) Call3(Func_02002eac_a, a0, a1, a2)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_02002ed8, a0, a1, a2)
#define Motion_SetVarCb_1(a0, a1) Call2(Func_02002e90, a0, a1)
#define Battle_RunThenWaitIfModeZero_2(a0, a1, a2) Call3(Func_02002eca, a0, a1, a2)
#define Motion_SetVarCb_2(a0, a1) Call2(Func_02002ea2_a, a0, a1)
#define Motion_SetVarCb_3(a0, a1) Call2(Func_02002eaa, a0, a1)
#define Motion_SetVarCb_4(a0, a1) Call2(Func_02002eb2, a0, a1)
#define Battle_WaitMode0_8(a0, a1) Call2(Func_02002f14, a0, a1)
#define Battle_WaitMode0_9(a0, a1) Call2(Func_02002f1e, a0, a1)
#define Battle_WaitMode0_10(a0, a1) Value2(Func_02002f28, a0, a1)
#define Battle_WaitMode0_11(a0) Value1(Func_02002e5e, a0)
#define Motion_SetVarCbObj_3(a0, a1) Value2(Func_02002ee6, a0, a1)
#define Battle_WaitMode0_12(a0) Value1(Func_02002e6c_a, a0)
#define BattleEv_RunWait_1(a0, a1) Call2(Func_02002f14_a, a0, a1)
#define Motion_CallWaitAnim_2(a0, a1) Call2(Func_02002ee4, a0, a1)
#define Battle_RunThenWaitIfModeZero_3(a0, a1, a2) Call3(Func_02002f2e, a0, a1, a2)
#define Object_LinkPair_1(a0, a1, a2) Value3(Func_02002f18, a0, a1, a2)
#define Battle_WaitMode0_13(a0) Value1(Func_02002e96_a, a0)
#define Motion_SetVarCb_5(a0, a1) Call2(Func_02002f16, a0, a1)
#define Motion_SetVarCbObj_4(a0, a1) Value2(Func_02002f26, a0, a1)
#define Battle_WaitMode0_14(a0) Value1(Func_02002eac_b, a0)
#define Motion_ArmCb_21(a0, a1, a2) Call3(Func_02002f70, a0, a1, a2)
#define Motion_ArmCb_22(a0, a1, a2) Call3(Func_02002f7c, a0, a1, a2)
#define Motion_SetVarCbObj_5(a0, a1) Call2(Func_02002f4c, a0, a1)
#define Battle_RunThenWaitIfModeZero_4(a0, a1, a2) Call3(Func_02002f7e, a0, a1, a2)
#define Motion_CallWaitAnim_3(a0, a1) Call2(Func_02002f46, a0, a1)
#define Battle_RunThenWaitIfModeZero_5(a0, a1, a2) Call3(Func_02002f90, a0, a1, a2)
#define Motion_ArmCb_23(a0, a1, a2) Call3(Func_02002fac, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_6(a0, a1, a2) Call3(Func_02002fa6, a0, a1, a2)
#define Motion_ArmCb_24(a0, a1, a2) Call3(Func_02002fc2, a0, a1, a2)
#define Motion_SetVarCbObj_6(a0, a1) Value2(Func_02002f92, a0, a1)
#define Battle_WaitMode0_15(a0) Value1(Func_02002f18_a, a0)
#define Battle_WaitMode0_16(a0, a1) Call2(Func_02002ff2, a0, a1)
#define Motion_ArmCb_25(a0, a1, a2) Call3(Func_02002fe6, a0, a1, a2)
#define Motion_ArmCb_26(a0, a1, a2) Call3(Func_02002ff0, a0, a1, a2)
#define Motion_ArmCb_27(a0, a1, a2) Call3(Func_02002ffc, a0, a1, a2)
#define Motion_SetVarCbObj_7(a0, a1) Value2(Func_02002fcc, a0, a1)
#define Battle_WaitMode0_17(a0) Value1(Func_02002f52, a0)
#define Battle_RunThenWaitIfModeZero_7(a0, a1, a2) Call3(Func_02003004, a0, a1, a2)
#define Motion_ArmCb_28(a0, a1, a2) Call3(Func_02003020, a0, a1, a2)
#define Motion_Launch_1(a0, a1, a2) Call3(Func_02002fe2, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_8(a0, a1, a2) Call3(Func_02003054, a0, a1, a2)
#define Motion_ArmCb_29(a0, a1, a2) Call3(Func_0200306e, a0, a1, a2)
#define Motion_SetSpeed_5(a0, a1) Value2(Func_02003056, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02002fe6_a, a0, a1)
#define SceneWork_SetStepValue_2(a0) Value1(Func_02003060, a0)
#define Motion_SetVarCbObj_8(a0, a1) Call2(Func_02003058, a0, a1)
#define Battle_RunThenWaitIfModeZero_9(a0, a1, a2) Value3(Func_0200308a, a0, a1, a2)
#define SceneWork_SetStepValue_3(a0) Value1(Func_0200307a, a0)
#define Motion_ArmCb_30(a0, a1, a2) Call3(Func_020030ae, a0, a1, a2)
#define Battle_RunThenWaitIfModeZero_10(a0, a1, a2) Call3(Func_020030a8, a0, a1, a2)
#define Motion_ArmCb_31(a0, a1, a2) Call3(Func_020030c4, a0, a1, a2)
#define Motion_ArmCb_32(a0, a1, a2) Call3(Func_020030d0, a0, a1, a2)
#define Motion_ArmCb_33(a0, a1, a2) Call3(Func_020030dc, a0, a1, a2)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Call3(Func_020030f8, a0, a1, a2)
#define Motion_SetVarCbObj_9(a0, a1) Value2(Func_020030b8, a0, a1)
#define Battle_WaitMode0_18(a0) Value1(Func_0200303e, a0)
#define Motion_CallWaitAnim_4(a0, a1) Value2(Func_020030ae_a, a0, a1)
#define Battle_WaitMode0_19(a0) Value1(Func_0200304c, a0)
#define Motion_ArmCb_34(a0, a1, a2) Call3(Func_0200310e, a0, a1, a2)
#define Motion_ArmCb_35(a0, a1, a2) Call3(Func_0200311a, a0, a1, a2)
#define Motion_SetVarCbObj_10(a0, a1) Value2(Func_020030ea, a0, a1)
#define Battle_WaitMode0_20(a0) Value1(Func_02003070, a0)
#define Battle_RunThenWaitIfModeZero_11(a0, a1, a2) Value3(Func_02003122, a0, a1, a2)
#define Motion_SetSpeed_6(a0, a1, a2) Call3(Func_020030b4, a0, a1, a2)
#define Object_SetModeById_13(a0, a1) Call2(Func_020030ec, a0, a1)
#define Motion_OffsetPositionAndResetMotion_6(a0, a1, a2) Value3(Func_020030de, a0, a1, a2)
#define Motion_CommitPos_4(a0) Value1(Func_020030ec_a, a0)
#define Object_SetModeById_14(a0, a1) Value2(Func_02003104, a0, a1)
#define Battle_WaitMode0_21(a0) Value1(Func_020030aa, a0)
#define Motion_ArmCb_36(a0, a1, a2) Call3(Func_0200316e, a0, a1, a2)
#define Motion_ArmCb_37(a0, a1, a2) Value3(Func_0200317a, a0, a1, a2)
#define Motion_CommitPos_5(a0) Value1(Func_02003118, a0)
#define Object_SetModeById_15(a0, a1) Value2(Func_02003130, a0, a1)
#define Battle_WaitMode0_22(a0) Value1(Func_020030d6, a0)
#define Object_SetModeById_16(a0, a1) Call2(Func_0200313e, a0, a1)
#define Object_SetModeById_17(a0, a1) Call2(Func_02003146, a0, a1)
#define Motion_CallWaitAnim_5(a0, a1) Value2(Func_02003156, a0, a1)
#define Battle_WaitMode0_23(a0) Value1(Func_020030f4, a0)
#define Object_SetModeById_18(a0, a1) Value2(Func_0200315c, a0, a1)
#define Scene_GetRecord_4(a0) Value1(Func_0200312a, a0)
#define Motion_ResetAndSetPosition_1(a0, a1, a2) Value3(Func_0200314c, a0, a1, a2)
#define Object_SetModeById_19(a0, a1) Value2(Func_0200317c, a0, a1)
#define Scene_GetRecord_5(a0) Value1(Func_0200314a, a0)
#define Motion_ResetAndSetPosition_2(a0, a1, a2) Value3(Func_0200316c, a0, a1, a2)
#define Object_SetModeById_20(a0, a1) Value2(Func_0200319c, a0, a1)
#define Scene_GetRecord_6(a0) Value1(Func_0200316a, a0)
#define Motion_ResetAndSetPosition_3(a0, a1, a2) Value3(Func_0200318c, a0, a1, a2)
#define Motion_CommitPos_6(a0) Value1(Func_020031aa, a0)
#define Motion_SetHPosTerrain_4(a0, a1, a2) Call3(Func_020031bc, a0, a1, a2)
#define Motion_SetHPosTerrain_5(a0, a1, a2) Call3(Func_020031c6, a0, a1, a2)
#define Motion_SetHPosTerrain_6(a0, a1, a2) Call3(Func_020031d0, a0, a1, a2)
#define Object_SetModeById_21(a0, a1) Call2(Func_020031e0, a0, a1)
#define Object_SetModeById_22(a0, a1) Call2(Func_020031e8, a0, a1)
#define Object_SetModeById_23(a0, a1) Value2(Func_020031f0, a0, a1)
#define GameFlag_Set_1(a0) Value1(Func_02003186, a0)
#define GameFlag_Clear_1(a0) Value1(Func_02003194, a0)
#define Battle_SchedShoulder_1() Value0(Func_020031b0)

/* Additional resolved engine calls: named directly from each target's own
 * reconstructed C source (see the trailing "provisional" comment on each). */

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

/* Two-field record read used when copying a found record's s32 pair into a
 * setter call alongside an entity id. */
#define RECORD_A32(rec) (*(s32 *)((rec) + 8))
#define RECORD_B32(rec) (*(s32 *)((rec) + 16))

/* Two-field record read used when copying a found record's s16 pair into a
 * setter call alongside an entity id. */
#define RECORD_A16(rec) (*(s16 *)((rec) + 10))
#define RECORD_B16(rec) (*(s16 *)((rec) + 18))

/* Runs the full scene sequence for entities 0, 1, 5 and 8: per-entity setup,
 * a shared movement/pose pass, a branch on one entity's saved state, and a
 * closing pass over the same four entities. */
void Scene_RunFourEntitySequence(void)
{
    u8 *record;

    Battle_Reset_1();
    Battle_WaitMode0_1(); /* main:0808a360 */
    Motion_SetSpeed_1(); /* main:0808a370 */

    record = Scene_GetRecord_1(0);
    if (record != 0)
        Motion_SetHPosTerrain_1(8, RECORD_A32(record), RECORD_B32(record));
    record = Scene_GetRecord_2(0);
    if (record != 0)
        Motion_SetHPosTerrain_2(5, RECORD_A32(record), RECORD_B32(record));
    record = Scene_GetRecord_3(0);
    if (record != 0)
        Motion_SetHPosTerrain_3(1, RECORD_A32(record), RECORD_B32(record));

    Motion_SetSpeed_2(8, 0x9999, 0x4ccc);
    Motion_SetSpeed_3(5, 0x9999, 0x4ccc);
    Motion_SetSpeed_4(1, 0x9999, 0x4ccc);
    Object_SetModeById_1(1, 2);
    Object_SetModeById_2(5, 2);
    Object_SetModeById_3(8, 2);
    Motion_OffsetPositionAndResetMotion_1(1, -16, 0);
    Motion_OffsetPositionAndResetMotion_2(5, 16, 0);
    Motion_OffsetPositionAndResetMotion_3(8, 0, -16);
    Motion_CommitPos_1(8);
    Object_SetModeById_4(8, 1);
    Object_SetModeById_5(0, 0);
    Object_SetModeById_6(1, 0);
    Object_SetModeById_7(5, 0);
    Motion_ArmCb_1(1, 0xe000, 0);
    Motion_ArmCb_2(5, 0xa000, 0);
    Motion_ArmCb_3(8, 0xc000, 30);
    Motion_ArmCb_4(1, 0x8000, 0);
    Motion_ArmCb_5(5, 0, 0);
    Motion_ArmCb_6(0, 0x4000, 0);
    Motion_ArmCb_7(8, 0x8000, 30);
    Motion_ArmCb_8(1, 0x4000, 0);
    Motion_ArmCb_9(5, 0x8000, 0);
    Motion_ArmCb_10(0, 0, 0);
    Motion_ArmCb_11(8, 0x4000, 30);
    Motion_ArmCb_12(1, 0xe000, 0);
    Motion_ArmCb_13(5, 0xa000, 0);
    Motion_ArmCb_14(0, 0xc000, 0);
    Motion_ArmCb_15(8, 0xc000, 40);
    Motion_SetVarCbObj_1(8, 2);
    Battle_WaitMode0_2(10);
    Object_SetModeById_8(8, 2);
    Motion_OffsetPositionAndResetMotion_4(8, 0, -16);
    Motion_CommitPos_2(8);
    Object_SetModeById_9(8, 1);
    Battle_WaitMode0_3(6);
    Motion_ArmCb_16(8, 0x8000, 20);
    Motion_ArmCb_17(8, 0, 20);
    Motion_ArmCb_18(8, 0xc000, 40);
    Motion_SetVarCbObj_2(8, 2);
    Battle_WaitMode0_4(20);
    Object_SetModeById_10(8, 2);
    Motion_OffsetPositionAndResetMotion_5(8, 0, -32);
    Motion_CommitPos_3(8);
    Object_SetModeById_11(8, 1);

    Motion_SetSpeedLim_1(0x20000, 0x4000);
    Motion_CamBounds_1(0x06310000, -1, 0x00960000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    Battle_WaitMode0_5(10);
    Motion_SetSpeedLim_2(0x13333, 0x2666);
    Motion_CamBounds_2(0x06550000, -1, 0x00640000, 1);
    Object_CommitPositionThenWaitIfModeZero_2();
    Motion_CamBounds_3(0x06b60000, -1, 0x00640000, 1);
    Object_CommitPositionThenWaitIfModeZero_3();
    Object_SetModeById_12(8, 1);
    Motion_CamBounds_4(0x06d80000, -1, 0x00960000, 1);
    Object_CommitPositionThenWaitIfModeZero_4();
    Battle_WaitMode0_6(40);
    Motion_SetSpeedLim_3(0x26666, 0x4ccc);
    Motion_CamBounds_5(0x06840000, -1, 0x01000000, 1);
    Object_CommitPositionThenWaitIfModeZero_5();
    Motion_CallWaitAnim_1(8, 3);
    Battle_WaitMode0_7(10);

    Motion_ArmCb_19(1, 0xe000, 0);
    Motion_ArmCb_20(5, 0xa000, 10);
    BattleFx_SpawnLinked_1(1, 0x101, 20);
    SceneWork_SetStepValue_1(0xfd6);
    Battle_RunThenWaitIfModeZero_1(1, 0, 10);
    BattleFx_SpawnLinked_2(8, 0x102, 60);
    Motion_SetVarCb_1(8, 2);
    Battle_RunThenWaitIfModeZero_2(8, 0, 10);
    Motion_SetVarCb_2(0, 2);
    Motion_SetVarCb_3(1, 2);
    Motion_SetVarCb_4(5, 2);
    Battle_WaitMode0_8(0, 0x102);
    Battle_WaitMode0_9(1, 0x102); /* main:0808a1f0 */
    Battle_WaitMode0_10(5, 0x102); /* main:0808a1f0 */
    Battle_WaitMode0_11(40);
    Motion_SetVarCbObj_3(8, 2);
    Battle_WaitMode0_12(20);
    BattleEv_RunWait_1(8, 0);
    Motion_CallWaitAnim_2(8, 4);
    Battle_RunThenWaitIfModeZero_3(8, 0, 10);
    Object_LinkPair_1(0, 5, 0);
    Battle_WaitMode0_13(40);
    Motion_SetVarCb_5(0, 1);
    Motion_SetVarCbObj_4(5, 1);
    Battle_WaitMode0_14(10);
    Motion_ArmCb_21(0, 0xc000, 0);
    Motion_ArmCb_22(5, 0xa000, 20);
    Motion_SetVarCbObj_5(5, 2);
    Battle_RunThenWaitIfModeZero_4(5, 0, 10);
    Motion_CallWaitAnim_3(8, 4);
    Battle_RunThenWaitIfModeZero_5(8, 0, 10);
    Motion_ArmCb_23(8, 0x3000, 10);
    Battle_RunThenWaitIfModeZero_6(8, 0, 40);
    Motion_ArmCb_24(8, 0xc000, 20);
    Motion_SetVarCbObj_6(8, 1);
    Battle_WaitMode0_15(10);
    Battle_WaitMode0_16(8, 0x102); /* main:0808a1f0 */
    Motion_ArmCb_25(8, 0x8000, 20);
    Motion_ArmCb_26(8, 0, 20);
    Motion_ArmCb_27(8, 0xc000, 60);
    Motion_SetVarCbObj_7(8, 2);
    Battle_WaitMode0_17(10);
    Battle_RunThenWaitIfModeZero_7(8, 0, 10);
    Motion_ArmCb_28(8, 0x4000, 20);
    Motion_Launch_1(8, 2, 20);

    Battle_RunThenWaitIfModeZero_8(8, 0, 40);
    Motion_ArmCb_29(1, 0, 20);
    Motion_SetSpeed_5(1, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        /* Passes the address of Value_00000fe0 in place of a record pointer. */
        SceneWork_SetStepValue_2((s32)&Value_00000fe0);
        Motion_SetVarCbObj_8(1, 1);
        Battle_RunThenWaitIfModeZero_9(1, 0, 10);
    } else {
        SceneWork_SetStepValue_3(0xfe1);
        Motion_ArmCb_30(5, 0x8000, 20);
        Battle_RunThenWaitIfModeZero_10(5, 0, 10);
        Motion_ArmCb_31(0, 0xc000, 0);
        Motion_ArmCb_32(1, 0xe000, 0);
        Motion_ArmCb_33(5, 0xa000, 60);
        BattleFx_SpawnLinked_3(0, 0x102, 40);
        Motion_SetVarCbObj_9(1, 1);
        Battle_WaitMode0_18(10);
        Motion_CallWaitAnim_4(1, 3);
        Battle_WaitMode0_19(10);
        Motion_ArmCb_34(1, 0, 0);
        Motion_ArmCb_35(5, 0x8000, 30);
        Motion_SetVarCbObj_10(1, 1);
        Battle_WaitMode0_20(10);
        Battle_RunThenWaitIfModeZero_11(1, 0, 10);
    }

    Motion_SetSpeed_6(8, 0x9999, 0x4ccc);
    Object_SetModeById_13(8, 2);
    Motion_OffsetPositionAndResetMotion_6(8, 0, 48);
    Motion_CommitPos_4(8);
    Object_SetModeById_14(8, 1);
    Battle_WaitMode0_21(6);
    Motion_ArmCb_36(1, 0xe000, 0);
    Motion_ArmCb_37(5, 0xa000, 0);
    Motion_CommitPos_5(8);
    Object_SetModeById_15(8, 1);
    Battle_WaitMode0_22(20);
    Object_SetModeById_16(1, 3);
    Object_SetModeById_17(5, 3);
    Motion_CallWaitAnim_5(0, 3);
    Battle_WaitMode0_23(6);
    Object_SetModeById_18(1, 2);

    record = Scene_GetRecord_4(0);
    if (record != 0)
        Motion_ResetAndSetPosition_1(1, RECORD_A16(record), RECORD_B16(record));
    Object_SetModeById_19(5, 2);
    record = Scene_GetRecord_5(0);
    if (record != 0)
        Motion_ResetAndSetPosition_2(5, RECORD_A16(record), RECORD_B16(record));
    Object_SetModeById_20(8, 2);
    record = Scene_GetRecord_6(0);
    if (record != 0)
        Motion_ResetAndSetPosition_3(8, RECORD_A16(record), RECORD_B16(record));

    Motion_CommitPos_6(8);
    Motion_SetHPosTerrain_4(1, 0, 0);
    Motion_SetHPosTerrain_5(5, 0, 0);
    Motion_SetHPosTerrain_6(8, 0, 0);
    Object_SetModeById_21(8, 1);
    Object_SetModeById_22(1, 1);
    Object_SetModeById_23(5, 1);
    GameFlag_Set_1(0x804);
    GameFlag_Clear_1(0x12f);
    Battle_SchedShoulder_1();
}
#undef Battle_Reset_1
#undef Battle_WaitMode0_1
#undef Motion_SetSpeed_1
#undef Scene_GetRecord_1
#undef Motion_SetHPosTerrain_1
#undef Scene_GetRecord_2
#undef Motion_SetHPosTerrain_2
#undef Scene_GetRecord_3
#undef Motion_SetHPosTerrain_3
#undef Motion_SetSpeed_2
#undef Motion_SetSpeed_3
#undef Motion_SetSpeed_4
#undef Object_SetModeById_1
#undef Object_SetModeById_2
#undef Object_SetModeById_3
#undef Motion_OffsetPositionAndResetMotion_1
#undef Motion_OffsetPositionAndResetMotion_2
#undef Motion_OffsetPositionAndResetMotion_3
#undef Motion_CommitPos_1
#undef Object_SetModeById_4
#undef Object_SetModeById_5
#undef Object_SetModeById_6
#undef Object_SetModeById_7
#undef Motion_ArmCb_1
#undef Motion_ArmCb_2
#undef Motion_ArmCb_3
#undef Motion_ArmCb_4
#undef Motion_ArmCb_5
#undef Motion_ArmCb_6
#undef Motion_ArmCb_7
#undef Motion_ArmCb_8
#undef Motion_ArmCb_9
#undef Motion_ArmCb_10
#undef Motion_ArmCb_11
#undef Motion_ArmCb_12
#undef Motion_ArmCb_13
#undef Motion_ArmCb_14
#undef Motion_ArmCb_15
#undef Motion_SetVarCbObj_1
#undef Battle_WaitMode0_2
#undef Object_SetModeById_8
#undef Motion_OffsetPositionAndResetMotion_4
#undef Motion_CommitPos_2
#undef Object_SetModeById_9
#undef Battle_WaitMode0_3
#undef Motion_ArmCb_16
#undef Motion_ArmCb_17
#undef Motion_ArmCb_18
#undef Motion_SetVarCbObj_2
#undef Battle_WaitMode0_4
#undef Object_SetModeById_10
#undef Motion_OffsetPositionAndResetMotion_5
#undef Motion_CommitPos_3
#undef Object_SetModeById_11
#undef Motion_SetSpeedLim_1
#undef Motion_CamBounds_1
#undef Object_CommitPositionThenWaitIfModeZero_1
#undef Battle_WaitMode0_5
#undef Motion_SetSpeedLim_2
#undef Motion_CamBounds_2
#undef Object_CommitPositionThenWaitIfModeZero_2
#undef Motion_CamBounds_3
#undef Object_CommitPositionThenWaitIfModeZero_3
#undef Object_SetModeById_12
#undef Motion_CamBounds_4
#undef Object_CommitPositionThenWaitIfModeZero_4
#undef Battle_WaitMode0_6
#undef Motion_SetSpeedLim_3
#undef Motion_CamBounds_5
#undef Object_CommitPositionThenWaitIfModeZero_5
#undef Motion_CallWaitAnim_1
#undef Battle_WaitMode0_7
#undef Motion_ArmCb_19
#undef Motion_ArmCb_20
#undef BattleFx_SpawnLinked_1
#undef SceneWork_SetStepValue_1
#undef Battle_RunThenWaitIfModeZero_1
#undef BattleFx_SpawnLinked_2
#undef Motion_SetVarCb_1
#undef Battle_RunThenWaitIfModeZero_2
#undef Motion_SetVarCb_2
#undef Motion_SetVarCb_3
#undef Motion_SetVarCb_4
#undef Battle_WaitMode0_8
#undef Battle_WaitMode0_9
#undef Battle_WaitMode0_10
#undef Battle_WaitMode0_11
#undef Motion_SetVarCbObj_3
#undef Battle_WaitMode0_12
#undef BattleEv_RunWait_1
#undef Motion_CallWaitAnim_2
#undef Battle_RunThenWaitIfModeZero_3
#undef Object_LinkPair_1
#undef Battle_WaitMode0_13
#undef Motion_SetVarCb_5
#undef Motion_SetVarCbObj_4
#undef Battle_WaitMode0_14
#undef Motion_ArmCb_21
#undef Motion_ArmCb_22
#undef Motion_SetVarCbObj_5
#undef Battle_RunThenWaitIfModeZero_4
#undef Motion_CallWaitAnim_3
#undef Battle_RunThenWaitIfModeZero_5
#undef Motion_ArmCb_23
#undef Battle_RunThenWaitIfModeZero_6
#undef Motion_ArmCb_24
#undef Motion_SetVarCbObj_6
#undef Battle_WaitMode0_15
#undef Battle_WaitMode0_16
#undef Motion_ArmCb_25
#undef Motion_ArmCb_26
#undef Motion_ArmCb_27
#undef Motion_SetVarCbObj_7
#undef Battle_WaitMode0_17
#undef Battle_RunThenWaitIfModeZero_7
#undef Motion_ArmCb_28
#undef Motion_Launch_1
#undef Battle_RunThenWaitIfModeZero_8
#undef Motion_ArmCb_29
#undef Motion_SetSpeed_5
#undef UiWork_WaitThenFinalizeCapacity_1
#undef SceneWork_SetStepValue_2
#undef Motion_SetVarCbObj_8
#undef Battle_RunThenWaitIfModeZero_9
#undef SceneWork_SetStepValue_3
#undef Motion_ArmCb_30
#undef Battle_RunThenWaitIfModeZero_10
#undef Motion_ArmCb_31
#undef Motion_ArmCb_32
#undef Motion_ArmCb_33
#undef BattleFx_SpawnLinked_3
#undef Motion_SetVarCbObj_9
#undef Battle_WaitMode0_18
#undef Motion_CallWaitAnim_4
#undef Battle_WaitMode0_19
#undef Motion_ArmCb_34
#undef Motion_ArmCb_35
#undef Motion_SetVarCbObj_10
#undef Battle_WaitMode0_20
#undef Battle_RunThenWaitIfModeZero_11
#undef Motion_SetSpeed_6
#undef Object_SetModeById_13
#undef Motion_OffsetPositionAndResetMotion_6
#undef Motion_CommitPos_4
#undef Object_SetModeById_14
#undef Battle_WaitMode0_21
#undef Motion_ArmCb_36
#undef Motion_ArmCb_37
#undef Motion_CommitPos_5
#undef Object_SetModeById_15
#undef Battle_WaitMode0_22
#undef Object_SetModeById_16
#undef Object_SetModeById_17
#undef Motion_CallWaitAnim_5
#undef Battle_WaitMode0_23
#undef Object_SetModeById_18
#undef Scene_GetRecord_4
#undef Motion_ResetAndSetPosition_1
#undef Object_SetModeById_19
#undef Scene_GetRecord_5
#undef Motion_ResetAndSetPosition_2
#undef Object_SetModeById_20
#undef Scene_GetRecord_6
#undef Motion_ResetAndSetPosition_3
#undef Motion_CommitPos_6
#undef Motion_SetHPosTerrain_4
#undef Motion_SetHPosTerrain_5
#undef Motion_SetHPosTerrain_6
#undef Object_SetModeById_21
#undef Object_SetModeById_22
#undef Object_SetModeById_23
#undef GameFlag_Set_1
#undef GameFlag_Clear_1
#undef Battle_SchedShoulder_1
#undef RECORD_A32
#undef RECORD_B32
#undef RECORD_A16
#undef RECORD_B16

/* overlays/scene/story/scenario_dispatch/scenario_dispatch_scene_head.c */
#define SceneData_SelectOverlayDataBySelector Func_02000030
#define SceneData_GetTable9F14 Func_02000074
#define Scene_RunScene37fSequenceA Func_02000154
#define Dialogue_RunFlag81aMessageBranch Func_02000200
#define Scene_RunOpeningAuxiliarySequence Func_02000258
#define Scene_RunFlag821Dialogue Func_020003bc

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
extern void Func_02001db0(void);  /* Func_0808a018 veneer. */
extern s32 Func_02001d96();       /* GameFlag_IsSet veneer, first site. */
extern void Func_02001d9a();      /* UiText_DrawMessage veneer, first site. */
extern void Func_02001da4();      /* UiText_DrawMessage veneer, second site. */
extern s32 Func_02001db2();       /* GameFlag_IsSet veneer, second site. */
extern void Func_02001df0(void);  /* Func_0808a020 veneer. */

void Func_02001c70();
void Func_02001c9e();
void Func_02001cb4();
void Func_02001cca();
void Func_02001cd0();
void Func_02001ce6();
void Func_02001cfc();
void Func_02001d06();
void Func_02001d80();
void Func_02001d8c();
void Func_02001da2();
void Func_02001da6();
void Func_02001dc2();
void Func_02001dce();
void Func_02001e12();
void Func_02001e2c();
void Func_02001e7e();
void Func_02001e8c();
void Func_02001e98();
void Func_02001dde();
s32 Func_02001df0_a();
void Func_02001df2();
s32 Func_02001dfc();
void Func_02001e26();
void Func_02001e28();
void Func_02001e30();
void Func_02001e34();
void Func_02001e40();
void Func_02001e44();
void Func_02001e48();
void Func_02001e56();
void Func_02001e7c();
void Func_02001e9e();
void Func_02001eaa();
void Func_02001ecc();
void Func_02001ef4();
void Func_02001f0e();
void Func_02001f22();
void Func_02001f24();
void Func_02001f2a();
void Func_02001f3a();
void Func_02001f4e();
void Func_02001f50();
void Func_02001f74();
void Func_02001f7e();
void Func_02001f7e_a();
void Func_02001f84();
void Func_02001f90();
void Func_02001f9c();
void Func_02001fa6();
void Func_02001fb2();
void Func_02001f6c();
s32 Func_02001f52();
void Func_02001f56();
s32 Func_02001f66();
void Func_02001f6e();
void Func_02001f82();
void Func_02001fb6();

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
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call3_02000258(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

s32 SceneData_SelectOverlayDataBySelector(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000013) {
        return (s32)Data_02009d04;
    }
    if (v == (s32)&Value_00000010) {
        return (s32)Data_02009d64;
    }
    return (s32)Data_02009cd4;
}

/*
 * Return the table at 0x02009f14.  The eight-byte owner includes its one
 * pool word.
 */
u8 *SceneData_GetTable9F14(void)
{
    return (u8 *)0x02009f14;
}

s32 Func_0200007c(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

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

void Scene_RunScene37fSequenceA(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Func_02001d06();
    v5 = 3;
    v6 = 2;
    Func_02001e2c(181);
    Call6(Func_02001cd0, 16, 28, 21, 3, v5, v6);
    Func_02001c9e(10);
    Call6(Func_02001ce6, 16, 30, 21, 3, v5, v6);
    Func_02001cb4(10);
    Call6(Func_02001cfc, 16, 32, 21, 3, v5, v6);
    Func_02001cca(10);
    Func_02001e12(0, 2);
    Call3(Func_02001d8c, 0, 0x9999, 0x4ccc);
    Func_02001da6(0, 120, 98);
    Func_02001dce(0, 2);
    Call3(Func_02001dc2, 0, 0, -8);
    Func_02001d80(10);
    Func_02001e8c();
    Func_02001e98();
    Func_02001e7e(2);
    Func_02001da2();
}

void Dialogue_RunFlag81aMessageBranch(void)
{
    extern u8 *Data_03001ebc;

    Func_02001db0();

    if (Func_02001d96(0x81a) != 0) {
        Func_02001d9a(0x1034, 1);
    } else {
        Func_02001da4(0x1031, 1);
        if (Func_02001db2(0xf01) != 0) {
            u16 *p = (u16 *)(Data_03001ebc + 370);
            u16 val = 1;
            *p = val;
        }
    }

    Func_02001df0();
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    extern u8 Data_03001ebc[];

    s32 id;
    s32 v5;
    s32 v6;

    if (Value1(Func_02001df0_a, 0xf01) == 0) {
    } else {
        if (Value1(Func_02001dfc, 0x81a) != 0) {
        } else {
            Func_02001e26();
            Func_02001f3a();
            v5 = 1;
            Func_02001f50(182);
            Call6(Func_02001df2, 0, 70, 30, 42, v5, v5);
            Func_02001dde();
            Func_02001e44(40);
            id = 0x1032;
            Call2(Func_02001e30, id, 1);
            Func_02001e56(20);
            v6 = 3;
            Func_02001f84(183);
            Call6(Func_02001e28, 0, 29, 3, 1, v6, 2);
            Call6(Func_02001e40, 0, 29, 3, 2, v6, v5);
            Call6(Func_02001e48, 1, 109, 4, 81, v5, v5);
            Func_02001e34();
            Call3_02000258(Func_02001e7c, 0x10000, 0x10000, 0x10000);
            Func_02001eaa(20);
            Call3_02000258(Func_02001f7e, 0, 0x100, 0);
            Call3_02000258(Func_02001e9e, 0x20000, 0x20000, 0x10000);
            Func_02001ecc(20);
            Call3_02000258(Func_02001f90, 0, 0x4000, 40);
            Call3_02000258(Func_02001f9c, 0, 0x8000, 20);
            Func_02001fa6(0, 0, 20);
            Call3_02000258(Func_02001fb2, 0, 0x4000, 10);
            Func_02001f74(0, 4, 20);
            Func_02001f7e_a(0, 6, 40);
            Call3_02000258(Func_02001ef4, -1, -1, 0xe666);
            Func_02001f22(40);
            Call2(Func_02001f0e, id + 1, 1);
            Call1(Func_02001f24, 0x143);
            Call1(Func_02001f2a, 0x81a);
            Func_02001f4e();
        }
    }
}

void Scene_RunFlag821Dialogue(void)
{
    extern u8 *Data_03001ebc;

    u8 *work;

    Func_02001f6c();

    if (Func_02001f52(0x821) != 0) {
        Func_02001f56(0x1034, 1);
    } else if (Func_02001f66(0xf02) != 0) {
        work = Data_03001ebc;
        Func_02001f6e(0x1031, 1);
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
        Func_02001f82(0x1031, 1);
    }

    Func_02001fb6();
}
#undef SceneData_SelectOverlayDataBySelector
#undef SceneData_GetTable9F14
#undef Scene_RunScene37fSequenceA
#undef Dialogue_RunFlag81aMessageBranch
#undef Scene_RunOpeningAuxiliarySequence
#undef Scene_RunFlag821Dialogue

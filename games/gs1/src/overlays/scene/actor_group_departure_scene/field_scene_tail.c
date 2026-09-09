#include "types.h"

#define BattleRuntime_Reset_1(args...) Func_020086b0(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020087ea, a0, a1, a2, a3)
#define ObjectMotion_EnableActionAndResetMotion_1(args...) Func_02008720(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_0200875c, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02008776(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020087f2, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02008728(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_020087a0, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020087f4, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_0200880c, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_020087c0, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02008862, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_02008878, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02008884(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02008752(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02008802(args)
#define BattleRuntime_RunThenWaitIfModeZero_1(a0, a1, a2) Call3(Func_0200885c, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02008878_a, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02008838(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(a0, a1, a2) Call3(Func_0200887a, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02008896, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_0200883e(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020087ec, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02008836, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02008842, a0, a1, a2)
#define GameFlag_Set_1(a0) Call1(Func_020087b0, a0)
#define ACTOR_ID 22
#define FieldScene_RunScene372SequenceE Func_02001828
#define FieldScene_RunScene372SequenceD Func_0200198c
#define FieldScene_RunReturnScript Func_02001b18
#define FieldScene_RunFlagGatedActorSequence Func_02002180
#define FieldScene_RunScene372_02003c48 Func_02003c48
#define FieldScene_RunScene372_02003e48 Func_02003e48
#define FieldScene_ConfigureActorTwentyTwoScene Func_02003fb0
#define FieldScene_BuildPlacementGrid Func_020040f0
#define FieldScene_RunFourPairedSteps Func_0200456c
#define FieldScene_RunScriptedStep1120 Func_02004618

extern u8 Data_00000e74[];
extern u8 Data_03001ebc[];
extern u8 Data_0200d7f8[];
extern u8 Data_0200d7fc[];
extern u8 Value_0000083a;
extern u8 Value_00000e8c;
extern u8 Value_0000201a;
extern u8 Data_0200cec8[];
extern u8 Value_00001120;

s32 Func_02005f0a();
void Func_02005f34();
void Func_02005f9e();
void Func_02005fc8();
void Func_02005fce();
void Func_02005fd6();
s32 Func_02005fec();
s32 Func_02005ff6();
s32 Func_02005ff6_a();
void Func_0200601e();
void Func_02006036();
void Func_0200604a();
void Func_02006056();
s32 Func_0200605a();
void Func_0200605c();
void Func_02006066();
void Func_02006068();
void Func_0200606a();
void Func_0200606c();
void Func_0200606e();
void Func_02006082();
void Func_0200608e();
void Func_0200609e();
void Func_020060a0();
void Func_020060a0_a();
void Func_020060a4();
void Func_020060a8();
void Func_020060a8_a();
void Func_020060bc();
void Func_020060c6();
void Func_020060ca();
void Func_020060d0();
void Func_020060dc();
void Func_020060e8();
void Func_020060f4();
void Func_020060f8();
void Func_0200608c();
void Func_020060c0();
s32 Func_020060c2();
void Func_020060de();
void Func_020060ec();
void Func_020060ee();
void Func_02006130();
void Func_02006138();
s32 Func_02006146();
void Func_02006150();
void Func_0200618a();
void Func_02006190();
void Func_020061a0();
void Func_020061a6();
void Func_020061a6_a();
void Func_020061a8();
void Func_020061b6();
void Func_020061be();
void Func_020061c4();
void Func_020061c8();
void Func_020061f0();
void Func_02006200();
void Func_02005f86();
void Func_02005f90();
void Func_02005f9a();
void Func_02005fa4();
void Func_0200608e_a();
void Func_02006180();
s32 Func_02006206();
void Func_02006216();
void Func_0200622c();
void Func_02006230();
void Func_02006246();
void Func_02006254();
void Func_02006270();
void Func_020062a6();
s32 Func_020062ae();
void Func_020062c2();
void Func_020062da();
void Func_020062e8();
void Func_020062ea();
s32 Func_020062f8();
void Func_02006306();
void Func_0200630e();
void Func_02006318();
void Func_0200631c();
void Func_02006320();
void Func_0200632c();
s32 Func_02006350();
void Func_0200635e();
s32 Func_0200637e();
void Func_020063c8();
void Func_020063d0();
void Func_020063d8();
void Func_020063da();
void Func_020063e0();
void Func_020063e6();
s32 Func_020063f2();
s32 Func_020063fc();
void Func_020063fe();
s32 Func_02006406();
s32 Func_02006410();
void Func_02006414();
void Func_02006420();
void Func_0200642a();
void Func_02006430();
void Func_0200643a();
void Func_0200643a_a();
void Func_02006450();
void Func_02006462();
void Func_0200646a();
s32 Func_0200646c();
void Func_02006470();
void Func_02006472();
void Func_02006472_a();
void Func_0200647a();
void Func_0200648e();
void Func_02006492();
void Func_0200649a();
void Func_020064a4();
void Func_020064a8();
void Func_020064c0();
void Func_020064c6();
void Func_020064c6_a();
void Func_020064cc();
void Func_020064d2();
void Func_020064de();
void Func_020064de_a();
void Func_020064ea();
void Func_02006502();
void Func_02006506();
void Func_02006508();
void Func_0200650a();
void Func_02006514();
void Func_02006538();
void Func_0200653a();
void Func_02006540();
void Func_0200655c();
void Func_02006568();
void Func_02006570();
void Func_02006572();
void Func_02006576();
void Func_020065a6();
void Func_020065ac();
void Func_020065dc();
void Func_02006610();
void Func_0200662e();
void Func_02006632();
void Func_02006638();
void Func_02006638_a();
void Func_0200664a();
void Func_02006656();
void Func_0200665c();
void Func_02006666();
void Func_02006666_a();
void Func_0200666e();
void Func_02006670();
void Func_02006686();
void Func_0200668c();
void Func_0200669a();
void Func_0200669e();
void Func_020066a2();
void Func_020066a6();
void Func_020066be();
void Func_020066c4();
void Func_020066d4();
void Func_020066dc();
void Func_020066e8();
void Func_020066ec();
void Func_020066f2();
void Func_020066f8();
void Func_020066fe();
void Func_0200670a();
void Func_0200670a_a();
void Func_02006712();
void Func_02006712_a();
void Func_02006712_b();
void Func_02006718();
s32 Func_02006758();
void Func_0200675c();
void Func_02006768();
void Func_02006772();
void Func_02006780();
void Func_02006786();
void Func_02006788();
void Func_0200678e();
void Func_02006794();
void Func_0200679a();
void Func_020067a0();
void Func_020067a2();
void Func_020067a4();
void Func_020067a6();
void Func_020067b4();
void Func_020067ba();
void Func_020067ba_a();
void Func_020067c2();
void Func_020067c8();
void Func_020067d8();
void Func_020067da();
void Func_020067dc();
void Func_020067e8();
void Func_020067ee();
void Func_02006840();
void Func_0200686c();
void Func_0200c56d();
s32 Func_02006862(s32);
void Func_0200688c(void);
void Func_02006928(s32, s32, s32);
void Func_020069a4(s32, s32, s32);
void Func_02006944(s32, s32);
u8 *Func_020068e2(s32);
s32 Func_02006810(void);
s32 Func_020067f6(s32, s32);
void Func_02006916(s32, u8 *);
void Func_0200696a(s32, s32, s32);
void Func_020069e6(s32, s32, s32);
void Func_02006982(s32, s32, s32);
void Func_020069fe(s32, s32, s32);
void Func_0200699e(s32, s32);
u8 *Func_0200693c(s32);
s32 Func_0200686a(void);
s32 Func_02006850(s32, s32);
void Func_0200696e(s32, u8 *);
void Func_020069c2(s32, s32, s32);
void Func_02006a3e(s32, s32, s32);
void Func_020069de(s32, s32);
u8 *Func_0200697c(s32);
s32 Func_020068aa(void);
s32 Func_02006890(s32, s32);
void Func_020069ae(s32, u8 *);
void Func_02006a02(s32, s32, s32);
u8 *Func_02006a02_b(s32);
void Func_02006a7e(s32, s32, s32);
void Func_02006a1a(s32, s32, s32);
void Func_02006a96(s32, s32, s32);
s32 Func_020069cc(s32);
void Func_0200695a(s32, s32);
void Func_020068e8(s32);
void Func_02006a86(s32);
void Func_02006a9e(s32, s32);
void Func_02006ad2(s32, s32, s32);
void Func_02006a4c(s32, s32, s32);
void Func_02006a70(s32, s32, s32);
void Func_02006a6a(s32, s32, s32);
void Func_02006abc(s32, s32, s32);
void Func_020069f2(s32);
void Func_02006b06(s32, s32, s32);
void Func_02006b06_b(s32, s32);
void Func_02006b12(s32, s32, s32);
u8 *Func_02006b12_b(s32);
void Func_02006b3e(s32, s32);
void Func_02006b58(s32, s32, s32, s32);
void Func_02006b64(void);
void Func_02006a32(s32);
void Func_02006afa(s32, s32);
void Func_02006b3c(s32, s32, s32);
void Func_02006b0c(s32, s32);
void Func_02006b5e(s32, s32, s32);
void Func_02006b60(s32, s32, s32);
void Func_02006b7c(s32, s32, s32);
void Func_02006ba8(s32, s32);
void Func_02006bc0(s32, s32, s32, s32);
void Func_02006bcc(void);
void Func_02006a9a(s32);
void Func_02006c60(s32);
void Func_02006b5a(s32, s32, s32);
void Func_02006b5a_a(s32, s32, s32);
void Func_02006b52(s32, s32);
void Func_02006ab8(s32);
void Func_02006ac8(s32);
void Func_02006c84(void);
void Func_02006c84_a(void);
void Func_02006b74(s32, s32);
void Func_02006b8a(s32, s32);
u8 *Func_02006b28(s32);
void Func_02006b9c(s32, s32);
u8 *Func_02006b3a(s32);
void Func_02006bc6(s32, s32);
void Func_02006bce(s32, s32);
void Func_02006bd6(s32, s32);
void Func_02006bde(s32, s32);
void Func_02006bee(s32, s32);
void Func_02006c5e(s32, s32);
void Func_02006c78(s32, s32, s32, s32);
void Func_02006c76(s32, s32);
void Func_02006c80(s32, s32);
void Func_02006b66(s32);
void Func_02006c2e(s32, s32);
void Func_02006c2e_a(s32, s32);
void Func_02006c2e_b(s32);
void Func_02006c6e(s32, s32);
void Func_02006c38(s32, s32, s32);
void Func_02006c02(s32, s32, s32);
void Func_02006c02_b(s32);
void Func_02006c4c(s32, s32, s32);
void Func_02006c16(s32, s32, s32);
u8 *Func_02006c16_b(s32);
void Func_02006bac(s32);
void Func_02006cec(s32, s32, s32, s32);
void Func_02006cf8(void);
void Func_02006c64(s32, s32, s32);
void Func_02006b1a(s32);
void Func_02006d98(s32);
void Func_02006bea(s32);
void Func_02006c92(s32, s32);
void Func_02006bf8(s32);
void Func_02006db4(void);
void Func_02006d32(s32, s32);
void Func_02006d4c(s32, s32, s32, s32);
void Func_02006d4c_b(s32, s32, s32);
void Func_02006d58(void);
void Func_02006ce8(s32, s32);
void Func_02006d50(s32, s32, s32);
void Func_02006d12(s32, s32, s32);
void Func_02006c48(s32);
void Func_02006d10(s32, s32);
void Func_02006d10_b(s32, s32, s32);
void Func_02006d48(s32, s32);
u8 *Func_02006c96(s32);
u8 *Func_02006cee(s32);
void Func_02006d1a(s32, s32, s32);
void Func_02006d5e(s32, s32, s32);
u8 *Func_02006d24(s32);
u8 *Func_02006d34(s32);
void Func_02006e18(s32, s32, s32);
void Func_02006e24(s32, s32, s32);
void Func_02006dc4(s32, s32);
void Func_02006e26(s32, s32, s32);
void Func_02006e42(s32, s32, s32);
void Func_02006e4c(s32, s32, s32);
void Func_02006dec(s32, s32);
void Func_02006e4e(s32, s32, s32);
void Func_02006e78(s32, s32, s32);
void Func_02006e82(s32, s32, s32);
void Func_02006e8e(s32, s32, s32);
void Func_02006e8a(s32, s32, s32);
void Func_02006e96(s32, s32, s32);
void Func_02006ea0(s32, s32, s32);
void Func_02006eac(s32, s32, s32);
void Func_02006ea6(s32, s32, s32);
void Func_02006ea6_b(s32, s32);
void Func_02006ec2(s32, s32, s32);
void Func_02006ece(s32, s32, s32);
void Func_02006eda(s32, s32, s32);
void Func_02006ee6(s32, s32, s32);
void Func_02006ee8(s32, s32, s32);
void Func_02006e98(s32, s32);
void Func_02006efa(s32, s32, s32);
void Func_02006f24(s32, s32, s32);
void Func_02006f2e(s32, s32, s32);
void Func_02006f38(s32, s32, s32);
void Func_02006f42(s32, s32, s32);
void Func_02006f3c(s32, s32, s32);
void Func_02006f0e(s32, s32, s32);
void Func_02006e44(s32);
void Func_02006f56(s32, s32, s32);
void Func_02006f60(s32, s32, s32);
void Func_02006f6c(s32, s32, s32);
void Func_02006f78(s32, s32, s32);
void Func_02006f18(s32, s32);
void Func_02006f28(s32, s32);
void Func_02006f5a(s32, s32, s32);
void Func_02006e90(s32);
void Func_02006f58(s32, s32);
void Func_02006f9a(s32, s32, s32);
void Func_02006f52(s32, s32);
void Func_02006fbe(s32, s32, s32);
void Func_02006fbe_b(s32, s32);
void Func_02006f76(s32, s32);
void Func_02006fc0(s32, s32, s32);
void Func_02006fda(s32, s32, s32);
void Func_02006fe4(s32, s32, s32);
void Func_02006fe4_b(s32, s32);
void Func_02006fa4(s32, s32);
void Func_02006ff8(s32, s32, s32);
void Func_02007004(s32, s32, s32);
void Func_02006fac(s32, s32);
void Func_02007006(s32, s32, s32);
void Func_02006f1c(s32);
void Func_02006fd4(s32, s32);
void Func_0200702e(s32, s32, s32);
void Func_0200704a(s32, s32, s32);
void Func_02006ff2(s32, s32);
void Func_02006f50(s32);
void Func_02007018(s32, s32);
void Func_0200705a(s32, s32, s32);
void Func_02005116(void);
void Func_02006f5c(s32);
void Func_02006f80(void);
void Func_0200828a();
void Func_020082d4();
void Func_020082e8();
void Func_02008300();
void Func_02008306();
void Func_0200830c();
void Func_02008312();
void Func_02008318();
void Func_0200831e();
void Func_0200833a();
void Func_02008356();
void Func_0200835c();
void Func_02008362();
void Func_02008368();
void Func_0200836e();
void Func_02008374();
void Func_020083a0();
void Func_020084f6();
void Func_02008500();
void Func_0200850e();
void Func_02008514();
void Func_02008522();
s32 Func_0200853e();
void Func_02008558();
void Func_02008566();
void Func_02008574();
s32 Func_0200857a();
s32 Func_02008582();
void Func_0200859c();
void Func_020085c0();
void Func_020085f6();
s32 Func_02008602();
void Func_0200861e();
void Func_02008622();
void Func_02008624();
void Func_0200862c();
void Func_0200862e();
void Func_0200862e_a();
void Func_0200864c();
void Func_02008656();
void Func_0200865a();
void Func_0200865c();
void Func_0200866a();
void Func_02008678();
void Func_02008682();
void Func_02008686();
void Func_02008690();
void Func_0200869a();
void Func_0200869c();
void Func_020086ae();
void Func_020086c8();
s32 Func_020086ca();
void Func_020086dc();
void Func_020086dc_a();
void Func_020086de();
void Func_020086e6();
void Func_020086e6_a();
void Func_020086fe();
void Func_02008706();
void Func_02008722();
void Func_02008724();
void Func_02008762();
void Func_0200861e_a();
void Func_02008656_a();
void Func_02008676();
void Func_020086b0();
void Func_02008720();
u8 *Func_02008728();
void Func_02008752();
void Func_0200875c();
void Func_02008776();
void Func_020087a0();
void Func_020087b0();
void Func_020087c0();
void Func_020087ea();
void Func_020087ec();
void Func_020087f2();
void Func_020087f4();
void Func_02008802();
void Func_0200880c();
void Func_02008836();
void Func_02008838();
void Func_0200883e();
void Func_02008842();
void Func_0200885c();
void Func_02008862();
void Func_02008878();
void Func_02008878_a();
void Func_0200887a();
void Func_02008884();
void Func_02008896();
void Func_020087a8();
void Func_020087be();
void Func_020087d0();
void Func_020087e4();
void Func_020087f6();
void Func_0200880a();
void Func_0200881c();
void Func_0200882c();
void Func_02008858();
void Func_0200886c();
void Func_02008880();
void Func_02008892();
void Func_020088a4();
void Func_020088a6();
void Func_020088b6();
void Func_020088ca();
void Func_02008c9e(s32);
void Func_0200887a_a(void);
void Func_02008ca8(s32);
void Func_02008884_a(void);
void Func_02008cb2(s32);
void Func_0200888e(void);
void Func_02008cc4(s32);
void Func_020088a0(void);
void Func_02008d18(void);
void Func_02008cf8(s32, s32);
void Func_02008d2c(void);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Configures actor 22 (position, pose, and movement/sprite flags) for the
 * scene. */

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene372SequenceE(void)
{
    u32 i;
    s32 record;
    s32 base5_e74;
    s32 v6;

    if (Value1(Func_02005f0a, 0x837) != 0) {
    } else {
        Func_02005f34();
        Call2(Func_02006056, 22, 0x100);
        base5_e74 = (s32)Data_00000e74;
        Func_0200601e(base5_e74);
        Func_02006036(22, 0);
        Call3(Func_0200606a, 0, 0x100, 20);
        Call3(Func_02006066, 0, 0x4000, 0);
        Call2(Func_0200608e, 0x6666, 0xccc);
        Call4(Func_020060a8, 0x1000000, -1, 0x24c0000, 1);
        Call3(Func_02005fce, 22, 0x20000, 0x10000);
        Value2(Func_02005ff6, 22, 0x200c934);
        Func_02006068(0, 22, 0);
        Func_02005f9e(30);
        Value2(Func_02005ff6_a, 22, 0x200c984);
        Func_0200609e(22, 0);
        v6 = 128;
        record = Func_02005fec(22);
        *(volatile s32 *)(record + 28) = (v6 << 9);
        Func_02006082(22, 1);
        Func_02005fc8(20);
        Func_020060d0(22, 0);
        Func_02005fd6(40);
        Func_020060a0(22, 1);
        Func_020060c6((base5_e74 + 5));
        Func_020060e8(22, 0, 20);
        Func_020060a0_a(0, 3);
        Func_020060a8_a(22, 3);
        Func_020060f8(22, 0);
        Call3(Func_0200605c, 22, (v6 << 9), 0x8000);
        Func_020060bc(22, 2);
        record = Value1(Func_0200605a, 0);
        if (record != 0) {
            Func_020060a4(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020060ca(22);
        Func_020060dc(22, 0, 0);
        Func_0200606c(1, 1);
        Func_020060f4(21, 3);
        Call1(Func_0200604a, 0x837);
        Func_0200606e();
    }
}

void FieldScene_RunScene372SequenceD(void)
{
    u32 i;
    s32 record;

    Func_0200608c();
    record = Value1(Func_020060c2, 0);
    if (record != 0) {
        Func_02006130(22, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_020060ee, 22, 0x10000, 0x8000);
    Call3(Func_02006138, 22, 0x119, 0x1fb);
    Func_0200618a(22, 0, 0);
    Func_020060c0(30);
    Call1(Func_020061a6, 0xe7b);
    Func_020061be(22, 0);
    Func_020061a0(0, 22, 0);
    Func_020060de(10);
    Func_020061a6_a(0, 1);
    Func_020060ec(20);
    Call3(Func_02006200, 22, 0x4000, 0);
    Func_020061f0(22, 0);
    Func_020061a8(22, 2);
    record = Value1(Func_02006146, 0);
    if (record != 0) {
        Func_02006190(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020061b6(22);
    Func_020061c8(22, 0, 0);
    Call3(Func_020061c4, 0, 0x100, 0x205);
    Func_02006150();
}

void FieldScene_RunReturnScript(void)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p9;
    s32 record;
    s32 r9;
    s32 r10;
    s32 r8;
    s32 base5_200d7fc;
    s32 base7_200c56d;
    s32 base_200c5a9;
    s32 shifted;
    s32 v5;
    s32 v6;
    s32 v7;

    if (Value1(Func_02006206, 0x838) != 0) {
        v5 = r9;
        v6 = r10;
        v7 = r8;
    } else {
        Func_02006230();
        Call1(Func_02006246, 0x200d4b0);
        Func_0200608e_a();
        Func_02006180(1);
        Func_020063fe(141);
        Call3(Func_02006216, 0x20000, 0x20000, 0x10000);
        Func_02006254(30);
        Call3(Func_0200622c, 0x30000, 0x30000, 0x10000);
        Func_0200642a(145);
        Func_02006270(30);
        record = Value1(Func_020062ae, 0);
        if (record != 0) {
            Func_0200631c(22, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        Call3(Func_020062da, 0, 0x20000, 0x10000);
        Call3(Func_020062e8, 22, 0x20000, 0x10000);
        Value2(Func_020062f8, 0, 0x200ca00);
        Call2(Func_02006318, 22, 0x200ca3c);
        Func_0200630e(0);
        Call3(Func_020063da, 0, 0x100, 0);
        Call3(Func_020063e6, 22, 0x100, 30);
        Call3(Func_020062a6, 0x40000, 0x40000, 0x10000);
        Func_020064a4(145);
        Func_020062ea(40);
        Call3(Func_020062c2, 0x50000, 0x50000, 0x10000);
        Func_020064c0(145);
        Func_02006306(20);
        Call2(Func_02006430, 0, 0x102);
        Call2(Func_0200643a, 22, 0x102);
        Func_02006320(40);
        Func_020063c8(32, 5);
        Func_020063d0(33, 5);
        Func_020063d8(30, 8);
        Func_020063e0(29, 8);
        record = Func_0200637e(30);
        *(volatile s32 *)(record + 24) = -0x10000;
        Func_02006462(32, 2);
        Func_0200646a(33, 2);
        Func_02006472(30, 3);
        Func_0200647a(29, 3);
        Call1(Func_02006450, 0xe7f);
        Func_02006472_a(28, 0, 20);
        Call3(Func_0200648e, 0, 0xc000, 0);
        Call3(Func_0200649a, 22, 0xc000, 20);
        Call2(Func_020064c6, 0x40000, 0x8000);
        Call4(Func_020064de, 0x700000, -1, 0x14b0000, 1);
        Func_020064ea();
        for (i = 0; i < 40; i++) {
            Func_02005f86(Func_020063f2(32));
            Func_02005f90(Func_020063fc(33));
            Func_02005f9a(Func_02006406(30));
            Func_02005fa4(Func_02006410(29));
            Func_0200632c(1);
        }
        base5_200d7fc = (s32)Data_0200d7fc;
        base7_200c56d = (s32)Func_0200c56d;
        v5 = base5_200d7fc;
        v6 = 0;
        v7 = base7_200c56d;
        *(volatile s32 *)Data_0200d7f8 = 0;
        *(volatile s32 *)base5_200d7fc = 0;
        Value2(Func_02006350, base7_200c56d, 0xc80);
        base_200c5a9 = 0x200c5a9;
        Call2(Func_0200635e, base_200c5a9, 0xc80);
        Func_02006414(40);
        *(volatile s32 *)base5_200d7fc = 1;
        Func_02006420(30);
        Call3(Func_020064c6_a, 19, 0x720000, 0x1220000);
        record = Func_0200646c(19);
        shifted = *(s32 *)(record + 12) + 0x400000;
        *(s32 *)(record + 12) = shifted;
        *(s32 *)(record + 60) = shifted;
        Call3(Func_02006492, 19, 0xcccc, 0x6666);
        Func_02006610(145);
        Call3(Func_020064d2, 19, 114, 0x14d);
        Func_02006502(19, 2);
        Call3(Func_0200643a_a, 0x20000, 0x20000, 0x10000);
        Func_02006638(145);
        *(volatile s32 *)base5_200d7fc = 0;
        Call3(Func_020064cc, 19, 0x6666, 0x3333);
        Call3(Func_02006508, 19, 114, 0x12c);
        Func_02006538(19, 2);
        Call3(Func_02006470, 0x50000, 0x50000, 0x10000);
        Func_0200666e(145);
        *(volatile s32 *)base5_200d7fc = 2;
        Call3(Func_02006506, 19, 0xcccc, 0x6666);
        Call3(Func_02006540, 19, 114, 0x14d);
        Func_02006570(19, 2);
        Call3(Func_020064a8, 0x20000, 0x20000, 0x10000);
        Func_020066a6(145);
        *(volatile s32 *)base5_200d7fc = 0;
        Call3(Func_0200653a, 19, 0x6666, 0x3333);
        Call3(Func_02006576, 19, 114, 0x12c);
        Func_020065a6(19, 2);
        Call3(Func_020064de_a, 0x40000, 0x40000, 0x10000);
        Func_020066dc(145);
        *(volatile s32 *)base5_200d7fc = 2;
        Call3(Func_02006572, 19, 0xcccc, 0x6666);
        Call3(Func_020065ac, 19, 114, 0x14d);
        Func_020065dc(19, 2);
        Call3(Func_02006514, 0x20000, 0x20000, 0x10000);
        Func_02006712(145);
        *(volatile s32 *)base5_200d7fc = 1;
        Func_0200655c(20);
        Call2(Func_02006686, 32, 0x102);
        Func_0200662e(32, 2);
        Func_02006666(31, 0);
        Call3(Func_0200669a, 33, 0x100, 0);
        Func_0200664a(33, 2);
        Func_0200668c(28, 0, 40);
        Call2(Func_020066be, 30, 0x102);
        Func_02006666_a(30, 2);
        Func_0200669e(30, 0);
        *(volatile s32 *)Data_0200d7f8 = 1;
        Func_0200665c(29, 1);
        Func_0200650a(1);
        Func_020066a2(29, 0);
        Call3(Func_020066ec, 29, 0x105, 20);
        Call3(Func_020066e8, 29, 0x8000, 40);
        Func_020066f2(29, 0, 20);
        Call3(Func_020066fe, 29, 0x8000, 20);
        Call3(Func_0200670a, 29, 0x4000, 40);
        Call3(Func_02006768, 29, 0x100, 0);
        Func_02006718(29, 2);
        Func_02006712_a(29, 4, 40);
        Func_0200670a_a(29, 9);
        Func_02006670(10);
        Func_02006772(29, 0, 20);
        Call1(Func_02006840, 0x121);
        Call3(Func_02006656, -1, -1, 0xe666);
        Call2(Func_020067c2, 0x60000, 0xc000);
        Call4(Func_020067dc, 0x540000, -1, 0x2340000, 1);
        Func_020067e8();
        Func_0200686c();
        Func_02006786(22, 0, 0);
        Func_020066c4(20);
        Call2(Func_020067ee, 22, 0x102);
        Func_020066d4(30);
        Func_02006632(base7_200c56d);
        Func_02006638_a(base_200c5a9);
        Func_020067d8(22, 0);
        Func_020067ba(0, 22, 0);
        Func_020066f8(20);
        Func_02006568();
        Func_020067a4(0, 3);
        Func_020067b4(22, 3);
        Func_02006712_b(20);
        Func_020067ba_a(22, 2);
        record = Value1(Func_02006758, 0);
        if (record != 0) {
            Func_020067a2(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020067c8(22);
        Func_020067da(22, 0, 0);
        Func_02006788(31);
        Func_0200678e(28);
        Func_02006794(30);
        Func_0200679a(29);
        Func_020067a0(32);
        Func_020067a6(33);
        Call1(Func_0200675c, 0x838);
        Func_02006780();
    }
    p9 = v5;
    p10 = v6;
    p11 = v7;
}

void FieldScene_RunFlagGatedActorSequence(void)
{
    s32 kc0_1 = 0xC00000;
    s32 k4be_1 = 0x4BE0000;
    s32 k2000_1 = 0x2000;
    s32 kc0_2 = 0xC00000;
    s32 k4a5_1 = 0x4A50000;
    s32 k2000_2 = 0x2000;
    s32 ke3_1 = 0xE30000;
    s32 k4be_2 = 0x4BE0000;
    s32 k4000_1 = 0x4000;
    s32 kfa_1 = 0xFA0000;
    s32 k4be_3 = 0x4BE0000;
    s32 k4000_2 = 0x4000;
    s32 ke3_2 = 0xE30000;
    s32 k4a5_2 = 0x4A50000;
    s32 k2000_3 = 0x3000;
    s32 kf3_1 = 0xF30000;
    s32 k4fd_1 = 0x4FD0000;
    s32 kc000_1 = 0xC000;
    s32 k100_1 = 0x100;
    s32 k446_1 = 0x446;
    s32 k446_2 = 0x446;
    s32 k4000_3 = 0x4000;
    s32 k4000_4 = 0x4000;
    s32 k40000_1 = 0x40000;
    s32 k8000_1 = 0x8000;
    s32 kd8_1 = 0xD80000;
    s32 ng1 = -1;
    s32 k4d0_1 = 0x4D00000;
    s32 k3000_1 = 0x3000;
    s32 k30000_1 = 0x30000;
    s32 k6000_1 = 0x6000;
    s32 ke8_1 = 0xE80000;
    s32 ng2 = -1;
    s32 k4e5_1 = 0x4E50000;
    s32 k9999_1 = 0x9999;
    s32 k1333_1 = 0x1333;
    s32 kd8_2 = 0xD80000;
    s32 ng3 = -1;
    s32 k4d0_2 = 0x4D00000;
    s32 k102_1 = 0x102;
    s32 k102_2 = 0x102;
    s32 k4b5_1 = 0x4B5;
    s32 k4b1_1 = 0x4B1;
    s32 ke8_2 = 0xE80000;
    s32 ng4 = -1;
    s32 k4e5_2 = 0x4E50000;
    s32 kf3_2 = 0xF30000;
    s32 k4fd_2 = 0x4FD0000;
    s32 k20000_1 = 0x20000;
    s32 k19999_1 = 0x19999;
    s32 k3333_1 = 0x3333;
    s32 kd8_3 = 0xD80000;
    s32 ng5 = -1;
    s32 k4d0_3 = 0x4D00000;
    s32 k105_1 = 0x105;
    s32 k800a_1 = 0x800A;
    s32 k9999_2 = 0x9999;
    s32 k4ccc_1 = 0x4CCC;
    s32 k9999_3 = 0x9999;
    s32 k4ccc_2 = 0x4CCC;
    s32 k4ba_1 = 0x4BA;
    s32 k4a5_3 = 0x4A5;
    s32 k6000_2 = 0x6000;
    s32 k8000_2 = 0x8000;
    s32 k8018_1 = 0x8018;
    s32 kc000_2 = 0xC000;
    s32 k800a_2 = 0x800A;
    s32 k105_2 = 0x105;
    s32 k105_3 = 0x105;
    s32 k106_1 = 0x106;
    s32 k8000_3 = 0x8000;
    s32 kc000_3 = 0xC000;
    s32 k4000_5 = 0x4000;
    s32 kc000_4 = 0xC000;
    s32 k9000_1 = 0x9000;
    s32 ka000_1 = 0xA000;
    s32 k8000_4 = 0x8000;
    s32 k800a_3 = 0x800A;
    s32 k105_4 = 0x105;
    s32 k105_5 = 0x105;
    s32 k105_6 = 0x105;
    s32 k105_7 = 0x105;
    s32 k8000_5 = 0x8000;
    s32 k8000_6 = 0x8000;
    s32 k800a_4 = 0x800A;
    s32 kd000_1 = 0xD000;
    s32 k2000_4 = 0x2000;
    s32 ka000_2 = 0xA000;
    s32 k8000_7 = 0x8000;
    s32 v83a_2 = 0x83A;
    u8 *tbl;
    s32 w16;
    s32 m;
    s32 one;

    if (Func_02006862((s32)&Value_0000083a) != 0) {
        return;
    }
    Func_0200688c();
    Func_02006928(10, kc0_1, k4be_1);
    Func_020069a4(10, k2000_1, 0);
    Func_02006944(10, 5);
    {
        u8 *o;
        s32 v;
        o = Func_020068e2(10);
        v = Func_020067f6(Func_02006810(), 0x5A) + 60;
        tbl = Data_0200cec8;
        *(u16 *)(o + 0x64) = v;
        Func_02006916(10, tbl);
    }
    Func_0200696a(9, kc0_2, k4a5_1);
    Func_020069e6(9, k2000_2, 0);
    Func_02006982(24, ke3_1, k4be_2);
    Func_020069fe(24, k4000_1, 0);
    Func_0200699e(24, 6);
    {
        u8 *o;
        s32 v;
        o = Func_0200693c(24);
        v = Func_02006850(Func_0200686a(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Func_0200696e(24, tbl);
    }
    Func_020069c2(25, kfa_1, k4be_3);
    Func_02006a3e(25, k4000_2, 0);
    Func_020069de(25, 6);
    {
        u8 *o;
        s32 v;
        o = Func_0200697c(25);
        v = Func_02006890(Func_020068aa(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Func_020069ae(25, tbl);
    }
    Func_02006a02(26, ke3_2, k4a5_2);
    Func_02006a7e(26, k2000_3, 0);
    Func_02006a1a(23, kf3_1, k4fd_1);
    Func_02006a96(23, kc000_1, 0);
    Func_0200695a(Func_020069cc(23), 0);
    Func_020068e8(3);
    Func_02006a86((s32)&Value_00000e8c);
    Func_02006a9e((s32)&Value_0000201a, 0);
    Func_02006ad2(0, k100_1, 20);
    Func_02006a4c(0, 150, k446_1);
    {
        u8 *p;
        p = Func_02006a02_b(0);
        if (p != 0) {
            Func_02006a70(22, *(s32 *)(p + 8), *(s32 *)(p + 16));
        }
    }
    Func_02006a6a(22, 132, k446_2);
    Func_02006abc(0, 22, 0);
    Func_020069f2(40);
    Func_02006b06(0, k4000_3, 0);
    Func_02006b12(22, k4000_4, 20);
    Func_02006b3e(k40000_1, k8000_1);
    Func_02006b58(kd8_1, ng1, k4d0_1, 1);
    Func_02006b64();
    Func_02006a32(40);
    Func_02006afa(10, 2);
    Func_02006b3c(10, 0, 10);
    Func_02006b0c(23, 3);
    Func_02006b5e(9, 0, 10);
    Func_02006b06_b(9, 3);
    Func_02006b60(9, 0, 10);
    Func_02006b7c(9, k3000_1, 10);
    Func_02006ba8(k30000_1, k6000_1);
    Func_02006bc0(ke8_1, ng2, k4e5_1, 1);
    Func_02006bcc();
    Func_02006a9a(20);
    Func_02006c60(134);
    Func_02006b5a(23, 4, 0);
    Func_02006b52(23, 6);
    Func_02006ab8(10);
    Func_02006b5a_a(23, 0, 0);
    Func_02006ac8(60);
    Func_02006c84();
    Func_02006b74(10, 1);
    {
        u8 *o;
        o = Func_02006b12_b(10);
        w16 = 0x10000;
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Func_02006b8a(24, 1);
    {
        u8 *o;
        o = Func_02006b28(24);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Func_02006b9c(25, 1);
    {
        u8 *o;
        o = Func_02006b3a(25);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Func_02006bc6(10, 2);
    Func_02006bce(9, 2);
    Func_02006bd6(24, 2);
    Func_02006bde(25, 2);
    Func_02006bee(26, 2);
    Func_02006c5e(k9999_1, k1333_1);
    Func_02006c78(kd8_2, ng3, k4d0_2, 1);
    Func_02006c84_a();
    Func_02006c76(26, k102_1);
    Func_02006c80(9, k102_2);
    Func_02006b66(60);
    Func_02006c2e(26, 2);
    Func_02006c2e_a(26, 3);
    Func_02006c6e(26, 0);
    Func_02006c38(25, 2, 0);
    Func_02006c02(25, 234, k4b5_1);
    Func_02006c4c(26, 2, 0);
    Func_02006c16(26, 227, k4b1_1);
    Func_02006bac(90);
    Func_02006cec(ke8_2, ng4, k4e5_2, 1);
    Func_02006cf8();
    Func_02006c64(23, kf3_2, k4fd_2);
    Func_02006b1a(1);
    Func_02006d98(106);
    {
        u8 *o;
        o = Func_02006c16_b(23);
        *(s32 *)(o + 0x28) = k20000_1;
    }
    Func_02006bea(6);
    Func_02006c92(23, 7);
    Func_02006bf8(20);
    Func_02006db4();
    Func_02006c02_b(20);
    Func_02006d32(k19999_1, k3333_1);
    Func_02006d4c(kd8_3, ng5, k4d0_3, 1);
    Func_02006d58();
    Func_02006ce8(24, 2);
    Func_02006c2e_b(20);
    Func_02006d50(24, k105_1, 40);
    Func_02006d12(24, 10, 0);
    Func_02006c48(10);
    Func_02006d10(10, 2);
    Func_02006d48(k800a_1, 0);
    {
        u8 *o;
        o = Func_02006c96(25);
        o += 0x5A;
        m = 0xFE;
        {
            u8 lv = m & *o;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Func_02006cee(26);
        o += 0x5A;
        *o = *o & m;
    }
    Func_02006d10_b(25, k9999_2, k4ccc_1);
    Func_02006d1a(26, k9999_3, k4ccc_2);
    Func_02006d4c_b(25, 247, k4ba_1);
    Func_02006d5e(26, 227, k4a5_3);
    {
        u8 *o;
        o = Func_02006d24(25);
        o += 0x5A;
        one = 1;
        {
            u8 lv = *o | one;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Func_02006d34(26);
        o += 0x5A;
        {
            u8 lv = one | *o;
            *o = lv;
        }
    }
    Func_02006e18(26, k6000_2, 0);
    Func_02006e24(25, k8000_2, 10);
    Func_02006dc4(24, 4);
    Func_02006e26(k8018_1, 0, 10);
    Func_02006e42(10, kc000_2, 20);
    Func_02006e4c(10, 0, 10);
    Func_02006dec(10, 4);
    Func_02006e4e(k800a_2, 0, 10);
    Func_02006e78(24, k105_2, 0);
    Func_02006e82(10, k105_3, 60);
    Func_02006e8e(9, k106_1, 20);
    Func_02006e8a(9, k8000_3, 40);
    Func_02006e96(9, kc000_3, 20);
    Func_02006ea0(9, 0, 30);
    Func_02006eac(9, k4000_5, 10);
    Func_02006ea6(9, 0, 10);
    Func_02006ec2(10, kc000_4, 0);
    Func_02006ece(25, k9000_1, 0);
    Func_02006eda(24, ka000_1, 0);
    Func_02006ee6(26, k8000_4, 10);
    Func_02006ea6_b(10, 1);
    Func_02006ee8(k800a_3, 0, 10);
    Func_02006e98(9, 4);
    Func_02006efa(9, 0, 10);
    Func_02006f24(10, k105_4, 0);
    Func_02006f2e(24, k105_5, 0);
    Func_02006f38(25, k105_6, 0);
    Func_02006f42(26, k105_7, 40);
    Func_02006f3c(9, 0, 10);
    Func_02006f0e(24, 25, 0);
    Func_02006e44(20);
    Func_02006f56(9, 0, 0);
    Func_02006f60(10, 0, 10);
    Func_02006f6c(24, k8000_5, 0);
    Func_02006f78(25, k8000_6, 10);
    Func_02006f18(24, 3);
    Func_02006f28(25, 3);
    Func_02006f5a(10, 9, 0);
    Func_02006e90(20);
    Func_02006f58(10, 1);
    Func_02006f9a(k800a_4, 0, 10);
    Func_02006f52(9, 3);
    Func_02006fbe(24, kd000_1, 10);
    Func_02006f76(24, 1);
    Func_02006fc0(24, 0, 10);
    Func_02006fda(10, 0, 0);
    Func_02006fe4(9, 0, 0);
    Func_02006fa4(26, 1);
    Func_02006ff8(26, k2000_4, 20);
    Func_02007004(25, ka000_2, 20);
    Func_02006fac(25, 3);
    Func_02007006(25, 0, 10);
    Func_02006fbe_b(26, 3);
    Func_02006f1c(20);
    Func_02006fe4_b(9, 2);
    Func_02006fd4(9, 3);
    Func_0200702e(9, 0, 10);
    Func_0200704a(26, k8000_7, 10);
    Func_02006ff2(26, 3);
    Func_02006f50(20);
    Func_02007018(9, 1);
    Func_0200705a(9, 0, 10);
    Func_02005116();
    Func_02006f5c(v83a_2);
    Func_02006f80();
}

void FieldScene_RunScene372_02003c48(void)
{
    u32 i;
    s32 record;

    Func_0200828a(20);
    Call1(Func_0200833a, 0x166);
    Func_02008300(0);
    Func_02008306(1);
    Func_0200830c(2);
    Func_02008312(3);
    Func_02008318(4);
    Func_0200831e(5);
    Call2(Func_020084f6, 0x10003, 1);
    Call2(Func_02008500, 0x10000, 2);
    Func_0200850e(1);
    Func_020082d4(120);
    Func_02008514(0, 0);
    Func_02008522(60);
    Func_020082e8(60);
    Call1(Func_020083a0, 0x166);
    Func_02008356(0);
    Func_0200835c(1);
    Func_02008362(2);
    Func_02008368(3);
    Func_0200836e(4);
    Func_02008374(5);
}

void FieldScene_RunScene372_02003e48(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Value1(Func_0200857a, 0);
    rec7 = Value1(Func_02008582, 8);
    Func_02008558();
    if (Value1(Func_0200853e, 0x305) != 0) {
        Func_020085c0(8);
        Func_02008566(10);
        Func_0200862e(8, 2);
        Func_02008574(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Func_02008624(8, 7);
        } else {
            Func_0200862e_a(8, 8);
        }
        Func_02008656(8, 2);
        Func_0200859c(20);
        Call1(Func_02008682, 0xed2);
        Func_0200869a(8, 0);
        Value2(Func_02008602, 8, 0x200cec8);
        Func_0200865a(8, 6);
    } else {
        Func_02008622(8);
        *(s32 *)(rec7 + 24) = 0x10000;
        *(s32 *)(rec7 + 28) = 0x10000;
        Call3(Func_020086de, 8, 0x1000, 0);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Func_02008686(8, 7);
        } else {
            Func_02008690(8, 8);
        }
        Func_020085f6(20);
        Call1(Func_020086dc, 0xed1);
        Func_020086fe(8, 0, 20);
        Func_020086ae(8, 1);
        Func_020086c8(8, 4, 0);
        Func_0200861e(80);
        Func_020086e6(8, 2);
        Func_0200862c(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Func_020086dc_a(8, 7);
        } else {
            Func_020086e6_a(8, 8);
        }
        Func_0200864c(2);
        Func_02008706(8, 2, 0);
        Func_0200865c(60);
        Func_02008724(8, 2);
        Func_0200866a(20);
        Func_02008762(8, 0);
        Value2(Func_020086ca, 8, 0x200cec8);
        Func_02008722(8, 6);
        Call1(Func_02008678, 0x305);
    }
    Func_0200869c();
}

void FieldScene_ConfigureActorTwentyTwoScene(void)
{
    u32 i;
    u8 *record;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
    ObjectMotion_EnableActionAndResetMotion_1(ACTOR_ID);
    Call1(Func_0200861e_a, 0x200c5b9);
    ObjectMotion_SetPositionAndReset_1(0, 0x1e0, 0x570);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(0, 0, 0);
    ObjectMotion_ArmCallback_1(ACTOR_ID, 0x3000, 20);
    {
        /* Set bit 0 of the flag byte at +35. */
        u8 *record = Scene_GetRecord_1(ACTOR_ID);
        u8 flags = *(volatile u8 *)&record[35];

        record[35] = (u8)(flags | 1);
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_2(ACTOR_ID, 0xf90000, 0x4d80000);
    Func_02008656_a(1);
    SceneWork_SetStepValue_1(0xed3);
    BattleEvent_RunActionAndWait_1(0x1016, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(ACTOR_ID, 0xac0000, 0x4fe0000);
    Func_02008676(1);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x40000, 0x8000);
    ObjectMotion_PlaceWithinCameraBounds_2(0xa20000, 0, 0x5050000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleRuntime_WaitIfModeZero_1(40);
    ObjectMotion_CallThenWaitForAnimationChange_1(ACTOR_ID, 4);
    BattleRuntime_RunThenWaitIfModeZero_1(0x1016, 0, 10);
    ObjectMotion_ArmCallback_2(ACTOR_ID, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(ACTOR_ID, 2);
    BattleRuntime_RunThenWaitIfModeZero_2(0x1016, 0, 10);
    ObjectMotion_ArmCallback_3(ACTOR_ID, 0x1000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_2(ACTOR_ID, 3);
    ObjectMotion_SetSpeedParameters_1(ACTOR_ID, 0x20000, 0x10000);
    ObjectMotion_SetPositionAndReset_2(ACTOR_ID, 165, 0x514);
    ObjectMotion_SetPositionAndReset_3(ACTOR_ID, 195, 0x598);
    GameFlag_Set_1(0x842);
}

/* Each of the 15 placement calls below takes the same 6-argument shape:
 * two coordinate-like values, two more coordinate-like values, and a
 * trailing pair of small counts. The final call takes no arguments. */
void FieldScene_BuildPlacementGrid(void)
{
    u32 i;
    u8 *record;

    Call6(Func_020087a8, 16, 96, 11, 73, 6, 3); /* main:08009180 */
    Call6(Func_020087be, 16, 96, 34, 68, 14, 10); /* main:08009180 */
    Call6(Func_020087d0, 16, 96, 64, 68, 7, 7); /* main:08009180 */
    Call6(Func_020087e4, 9, 95, 11, 73, 6, 3); /* main:08009180 */
    Call6(Func_020087f6, 40, 94, 34, 68, 14, 10); /* main:08009180 */
    Call6(Func_0200880a, 54, 94, 64, 68, 8, 7); /* main:08009180 */
    Call6(Func_0200881c, 72, 75, 72, 76, 1, 1); /* main:08009180 */
    Call6(Func_0200882c, 72, 75, 74, 76, 1, 1); /* main:08009180 */
    Call6(Func_02008858, 7, 75, 1, 1, 6, 75); /* main:080091c0 */
    Call6(Func_0200886c, 8, 70, 3, 1, 8, 71); /* main:080091c0 */
    Call6(Func_02008880, 8, 70, 2, 1, 9, 72); /* main:080091c0 */
    Call6(Func_02008892, 8, 70, 2, 1, 9, 73); /* main:080091c0 */
    Call6(Func_020088a4, 11, 66, 1, 1, 8, 73); /* main:080091c0 */
    Call6(Func_020088b6, 12, 66, 1, 4, 11, 73); /* main:080091c0 */
    Call6(Func_020088ca, 25, 0, 1, 1, 6, 74); /* main:080091c0 */
    /* No-argument call that closes out the sequence started above. */
    Func_020088a6(); /* main:08009128 */
}

void FieldScene_RunFourPairedSteps(void)
{
    Func_02008c9e(32);
    Func_0200887a_a();
    Func_02008ca8(33);
    Func_02008884_a();
    Func_02008cb2(30);
    Func_0200888e();
    if (*(s32 *)0x0200d7f8 == 0) {
        Func_02008cc4(29);
        Func_020088a0();
    }
}

void FieldScene_RunScriptedStep1120(void)
{
    Func_02008d18();
    Func_02008cf8((s32)&Value_00001120, 1);
    Func_02008d2c();
}

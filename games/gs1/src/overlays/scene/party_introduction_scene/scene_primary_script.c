#include "types.h"

#define Audio_PlayCue_1(args...) Func_02005876(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02005734, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02005770_a, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02005804, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_020057a4(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_0200572a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_020057ca(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02005738_a(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_0200583a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02005780, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_020057ba, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_0200584e, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02005794(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_020057ea(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020057b8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_020057f2, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02005886, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02005892, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02005842(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02005852(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_020057c0(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020058c2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_020058cc, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02005854(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02005864(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_0200581e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02005828, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_0200585a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_0200586e, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_0200588e(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_0200590a, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02005916, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02005932, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_0200593e, a0, a1, a2)
#define ObjectMotion_Launch_1(args...) Func_020058d8(args)
#define ObjectMotion_Launch_2(args...) Func_020058e2_a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_020058d6, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_020058e2_b, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(a0, a1, a2) Call3(Func_020058ee, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(a0, a1, a2) Call3(Func_020058fa, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_020059a2, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020059b8_a, a0, a1, a2, a3)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02005994, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_020059a0, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_020059ac, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_020059b8_b, a0, a1, a2)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_020059f4(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_020058da(args)
#define FieldScene_RunSevenSceneChain Func_02000a64
#define FieldScene_StagePairedActors Func_02000d5c
#define FieldScene_RunActorFourteenGuestScene Func_02001d04
#define FieldScene_RunSixPassEffectSequence Func_02002400

extern u8 Data_00001076[];
extern u8 Data_03001ebc[];
extern u8 Data_000010b0[];
extern u8 Data_000010b2[];
extern u8 Data_000010b6[];
extern u8 Data_0000200a[];

s32 Func_02004bc6();
void Func_02004c2c();
void Func_02005056();
void Func_0200506c();
void Func_0200507c();
void Func_0200509c();
void Func_020050a0();
void Func_020050aa();
void Func_020050b4();
void Func_020050c4();
void Func_020050ca();
void Func_020050d2();
void Func_020050fa();
void Func_02005102();
void Func_02005112();
void Func_02005124();
void Func_02005134();
void Func_02005138();
void Func_0200513e();
void Func_02005162();
void Func_0200516e();
void Func_02005180();
void Func_02005186();
void Func_02005198();
void Func_020051a8();
void Func_020051b2_a();
void Func_020051b2_b();
void Func_020051cc();
void Func_020051e0();
void Func_020051ec();
void Func_020051f6_a();
void Func_020051f6_b();
void Func_0200520c();
void Func_0200521a();
void Func_0200521c();
void Func_0200521e();
void Func_02005220();
void Func_02005226();
void Func_02005236();
void Func_02005238();
void Func_0200523c();
void Func_02005242();
void Func_02005248();
void Func_02005260();
void Func_0200526a();
void Func_0200528a();
void Func_0200529a();
s32 Func_0200529c();
void Func_020052b0_a();
void Func_020052b0_b();
void Func_020052bc();
void Func_020052be();
void Func_020052ce();
void Func_020052d4();
void Func_020052e8();
void Func_02005302();
void Func_0200530e();
void Func_02005324();
void Func_02005330();
void Func_02005338();
void Func_0200535e();
void Func_02005380();
void Func_020053a0();
void Func_020053ac();
void Func_020053bc();
void Func_020053ca();
void Func_020053d6();
void Func_020053f2();
void Func_020053f6();
void Func_02005408();
void Func_0200540a();
void Func_02005414();
void Func_02005430();
void Func_02005436();
void Func_0200545e();
void Func_02005488();
void Func_02005494();
void Func_020054a4();
void Func_020054a8();
void Func_020054b8();
void Func_020054d4();
void Func_020054e0();
void Func_020054ee();
void Func_02005502();
void Func_02005404(void);
void Func_02001504(void);
void Func_020017cc(void);
void Func_02001a00(void);
void Func_02001ec8(void);
void Func_02002208(void);
void Func_02002784(void);
void Func_020053f2_a(s32);
void Func_0200542e(void);
void Func_02002d0a(void);
s32 Func_02004f5c();
void Func_020053f2_b();
void Func_02005408_a();
void Func_0200542c();
void Func_0200543a();
void Func_02005450();
void Func_02005454();
void Func_02005460();
void Func_02005466();
void Func_02005490();
void Func_02005496();
void Func_020054aa();
void Func_020054bc();
void Func_020054c8();
void Func_020054ce();
void Func_020054d6();
void Func_020054fa();
void Func_02005506();
void Func_0200550e();
void Func_02005520();
void Func_02005532();
void Func_02005544();
void Func_0200554a();
void Func_0200554c();
void Func_02005564();
void Func_0200557c();
void Func_02005584();
void Func_02005592_a();
void Func_02005592_b();
void Func_020055a8();
void Func_020055ae();
void Func_020055b2();
void Func_020055ba();
void Func_020055c0();
void Func_020055c6();
void Func_020055d0();
void Func_020055d2();
void Func_020055d4();
void Func_020055e0();
void Func_020055fa();
void Func_02005630_a();
void Func_02005630_b();
void Func_02005630_c();
void Func_02005648_a();
void Func_02005648_b();
void Func_0200564e();
void Func_02005654();
void Func_02005666();
void Func_0200567e();
void Func_02005698();
void Func_020056a6();
void Func_020056be();
void Func_020056ca();
void Func_020056d0();
void Func_02005738();
void Func_02005746();
void Func_02005764();
void Func_02005770();
void Func_0200578e();
void Func_020057a2();
void Func_020057ce();
void Func_0200572a();
void Func_02005734();
void Func_02005738_a();
void Func_02005770_a();
void Func_02005780();
u8 *Func_02005794();
void Func_020057a4();
void Func_020057b8();
void Func_020057ba();
void Func_020057c0();
void Func_020057ca();
void Func_020057ea();
void Func_020057f2();
void Func_02005804();
void Func_0200581e();
void Func_02005828();
void Func_0200583a();
void Func_02005842();
void Func_0200584e();
void Func_02005852();
void Func_02005854();
void Func_0200585a();
void Func_02005864();
void Func_0200586e();
void Func_02005876();
void Func_02005886();
void Func_0200588e();
void Func_02005892();
void Func_020058c2();
void Func_020058cc();
void Func_020058d6();
void Func_020058d8();
void Func_020058da();
void Func_020058e2_a();
void Func_020058e2_b();
void Func_020058ee();
void Func_020058fa();
void Func_0200590a();
void Func_02005916();
void Func_02005932();
void Func_0200593e();
void Func_02005994();
void Func_020059a0();
void Func_020059a2();
void Func_020059ac();
void Func_020059b8_a();
void Func_020059b8_b();
void Func_020059f4();
void Func_02005a3e();
void Func_02005a70();
void Func_02005a9a();
void Func_02005abe();
void Func_02005adc();
void Func_02005af4();
void Func_02005b04();
void Func_02005b1a();
void Func_02005b46();
void Func_02005b56();
void Func_02005b6c();
void Func_02005b84();
void Func_02005b94();
void Func_02005ba4();
void Func_02005bde();
void Func_02005d08();
void Func_02005d3c();
void Func_02005d62();
void Func_02005e0e();
void Func_02006134();
void Func_02006158();
s32 Func_02006162();
void Func_0200617a();
void Func_02006198();
void Func_020061b8();
void Func_020061c2();
void Func_020061c6();
void Func_020061ca();
void Func_020061f4();
void Func_02006202();
void Func_02006204();
void Func_0200620c();
void Func_02006212();
void Func_0200621a();
void Func_0200621c();
void Func_02006220();
void Func_02006230();
void Func_02006244();
void Func_0200624c_a();
void Func_0200624c_b();
void Func_0200625e();
void Func_02006276();
void Func_0200627c_a();
void Func_0200627c_b();
void Func_02006286_a();
void Func_02006286_b();
void Func_0200628a();
void Func_02006294();
void Func_0200629a();
void Func_020062a2();
void Func_020062a6();
void Func_020062b0_a();
void Func_020062b0_b();
void Func_020062be();
void Func_020062c8();
void Func_020062e0();
void Func_020062f0();
void Func_020062fe();
void Func_02006304();
void Func_0200630e();
void Func_0200631a();
void Func_02006340();
void Func_02006342();
void Func_02006358();
void Func_0200635a();
void Func_02006370();
void Func_02006380();
s32 Func_020063a2();
void Func_020063a6();
void Func_020063b2();
void Func_020063c0();
void Func_020063ce();
void Func_020063e4();
void Func_020063f4();
void Func_020063fe();
void Func_02006402_a();
void Func_02006402_b();
void Func_0200640a();
void Func_02006410();
void Func_02006418();
void Func_0200641a();
void Func_02006426();
void Func_0200643e();
void Func_02006448();
s32 Func_0200644e_a();
s32 Func_0200644e_b();
void Func_0200645a_a();
void Func_0200645a_b();
void Func_0200646e_a();
void Func_0200646e_b();
void Func_0200647e();
void Func_02006486();
void Func_02006492();
void Func_020064a0();
void Func_020064b4();
void Func_020064b8();
void Func_020064ba();
void Func_020064c0();
void Func_020064c2();
void Func_020064c6();
s32 Func_020064ee();
s32 Func_020064fa_a();
s32 Func_020064fa_b();
void Func_020064fc();
void Func_02006500();
void Func_02006526();
void Func_0200652a();
void Func_02006536_a();
void Func_02006536_b();
void Func_02006540();
void Func_0200654c();
void Func_02006556_a();
void Func_02006556_b();
void Func_02006556_c();
void Func_0200657e();
void Func_0200658c();
void Func_020065c0();
void Func_020065d2();
s32 Func_020065d6();
void Func_020065d8();
void Func_020065e2_a();
void Func_020065e2_b();
void Func_02006602();
void Func_0200661a();
void Func_0200661c();
void Func_0200662a();
void Func_02006632();
void Func_02006640();
void Func_02006648();
void Func_02006656();
void Func_0200665e();
void Func_02006678();
void Func_0200667a();
void Func_0200668a_a();
void Func_0200668a_b();
void Func_0200668a_c();
void Func_0200668e();
void Func_020066a4();
void Func_020066a6();
void Func_020066b6();
void Func_020066ba();
void Func_020066d0();
void Func_020066e6();
void Func_02006776();
void Func_020067a2();
void Func_020067ce();
void Func_02005ff6();
void Func_02005ffe();
void Func_02006054();
void Func_020060be();
void Func_020060d8();
void Func_02006104();
void Func_02006156();
void Func_02006174();
void Func_02006184();
void Func_02006196();
void Func_020061f8();
void Func_020062de();
void Func_020062f4();
void Func_0200662a_a();
void Func_020066aa();
s32 Func_020066e4();
void Func_02006716();
void Func_0200673a();
void Func_0200673c();
void Func_02006742();
void Func_02006748();
void Func_02006764();
void Func_02006772();
void Func_0200677a();
void Func_02006780();
void Func_0200679e();
void Func_020067a4();
void Func_020067a8();
void Func_020067b0();
void Func_020067c6();
void Func_020067c8();
void Func_020067d0();
void Func_020067de();
void Func_020067e4();
s32 Func_020067e6();
void Func_020067e8();
void Func_020067ec_a();
void Func_020067ec_b();
void Func_020067f0();
void Func_020067f4();
void Func_020067f6_a();
void Func_020067f6_b();
void Func_020067f6_c();
void Func_020067fc();
void Func_0200683e_a();
void Func_0200683e_b();
void Func_02006860();
void Func_02006874();
void Func_02006880();
s32 Func_02006882();
void Func_0200688a();
s32 Func_0200688e();
void Func_0200689c();
void Func_020068b8();
void Func_020068c2();
void Func_02006904();
void Func_0200690c();
void Func_02006928();
void Func_0200692a();
void Func_02006932();
void Func_02006934();
void Func_02006960();
void Func_0200699c();
void Func_020069a0();
void Func_020069a4();
void Func_020069a6();
s32 Func_020069b2();
void Func_020069bc();
void Func_020069d2();
void Func_020069e0_a();
void Func_020069e0_b();
void Func_020069e0_c();
void Func_020069e4();
void Func_020069ec();
void Func_020069ee();
void Func_020069f6();
void Func_02006a0e();
s32 Func_02006a14();
void Func_02006a1c();
void Func_02006a22();
s32 Func_02006a24_a();
s32 Func_02006a24_b();
void Func_02006a34();
void Func_02006a38();
void Func_02006a5e();
void Func_02006a60();
void Func_02006a64();
s32 Func_02006a68();
void Func_02006a6a();
void Func_02006a6c();
void Func_02006a7a();
void Func_02006a7e_a();
void Func_02006a7e_b();
void Func_02006a7e_c();
void Func_02006a8a();
void Func_02006a8e();
void Func_02006a90();
void Func_02006a96();
s32 Func_02006a9a();
void Func_02006aa8();
void Func_02006ab2();
void Func_02006ab4();
s32 Func_02006ac4();
void Func_02006acc();
s32 Func_02006ad6();
void Func_02006ae2();
void Func_02006af8();
void Func_02006b1c();
void Func_02006b24();
void Func_02006b3a_a();
void Func_02006b3a_b();
void Func_02006b50();
void Func_02006b54();
void Func_02006b66();
void Func_02006b6a();
void Func_02006b7e();
void Func_02006b80();
void Func_02006b8e();
void Func_02006b92_a();
void Func_02006b92_b();
void Func_02006b96();
void Func_02006b9e();
void Func_02006ba4();
void Func_02006baa();
void Func_02006bac();
void Func_02006bc2();
void Func_02006bd2();
void Func_02006bd8();
s32 Func_02006bda();
void Func_02006bde();
void Func_02006c06();
void Func_02006c0c();
void Func_02006c0e();
void Func_02006c1e();
void Func_02006c20();
void Func_02006c24();
void Func_02006c2e();
void Func_02006c32();
void Func_02006c44();
void Func_02006c52();
void Func_02006c6c_a();
void Func_02006c6c_b();
void Func_02006c7e();
void Func_02006caa();
s32 Func_020066f4();
void Func_02006d58();
void Func_02006d68();
void Func_02006d76();
void Func_02006da2();
void Func_02006db6();
void Func_02006dc8_a();
void Func_02006dc8_b();
void Func_02006dcc();
void Func_02006e16();
void Func_02006e30();
void Func_02006e52();
void Func_02006e68();
void Func_02006e8c();
void Func_02006e9c();
void Func_02006eac();
s32 Func_02006eba_a();
s32 Func_02006eba_b();
void Func_02006ec2();
void Func_02006ec4_a();
void Func_02006ec4_b();
void Func_02006ece();
void Func_02006ed8();
void Func_02006eda();
void Func_02006ee0();
void Func_02006ee2_a();
void Func_02006ee2_b();
void Func_02006ee6();
void Func_02006ef6();
void Func_02006ef8_a();
void Func_02006ef8_b();
void Func_02006f06();
void Func_02006f0e();
void Func_02006f10();
void Func_02006f14();
void Func_02006f20();
void Func_02006f66();
void Func_02006f70();
void Func_02006f94();
void Func_02006fea();
void Func_02006ff6();
void Func_02007018();
void Func_02007022();
void Func_02007026();
void Func_02007030();
void Func_0200703a();
void Func_0200703c();
void Func_02007044();
void Func_02007048();
void Func_0200704a();
void Func_02007054();
void Func_02007060();
void Func_02007094();
void Func_020070fe();
void Func_02007100();
s32 Func_02007126();
void Func_02007132();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Inline calls keep argument constants local; direct calls permit sharing. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call2_02000d5c(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001d04(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1_02002400(void (*f)(), s32 a0)
{
    f(a0);
}

void Func_020006f4(void)
{
    u32 i;
    s32 obj;
    s32 pos;
    s32 mes;

    Func_020050a0();
    Func_0200521e(141);
    for (i = 0; i != 6; i++) {
        Call2(Func_020051e0, 0x404a4e, 1);
        Func_020051f6_a(8);
        Func_020050b4(8);
        Call2(Func_020051f6_b, 0x10000, 1);
        Func_0200520c(8);
        Func_020050ca(8);
        if (i == 1) {
            Call3(Func_02005056, 0x10000, 0x10000, 0x10000);
        }
    }
    Call3(Func_0200506c, 0x10000, 0x20000, 0x10000);
    Func_020050fa(30);
    Func_0200507c(0, 0, 0);
    Call2(Func_0200521c, 0x59999, 0xb333);
    Call4(Func_02005236, 0x1d80000, -1, 0x620000, 1);
    Func_02005242();
    Call3(Func_020050aa, 0x10000, 0x20000, 0x10000);
    Func_02005138(20);
    Func_020052be(144);
    Func_0200509c(0x200d088, 84, 4);
    Call6(Func_020050c4, 0, 0, 1, 1, 29, 4);
    Call6(Func_020050d2, 87, 42, 29, 6, 1, 2);
    Func_02005180(40);
    Func_02005102(0, 0, 0);
    Call4(Func_020052b0_a, 0x1570000, -1, 0x1710000, 1);
    Func_020052bc();
    Call3(Func_02005124, 0x10000, 0x20000, 0x10000);
    Func_020051b2_a(20);
    Func_02005338(144);
    pos = 21;
    Func_02005112(0x200d088, 76, 21);
    Call6(Func_02005134, 0, 0, 1, 1, pos, pos);
    Call6(Func_0200513e, 87, 42, 21, 23, 1, 2);
    Func_020051ec(40);
    Func_0200516e(0, 0, 0);
    Call2(Func_0200530e, 0x33333, 0x6666);
    Call4(Func_02005324, 0x1570000, -1, 0x1f10000, 1);
    Func_02005330();
    Call3(Func_02005198, 0x10000, 0x20000, 0x10000);
    Func_02005226(20);
    Func_020053ac(144);
    Func_02005186(0x200d088, 76, 29);
    Call6(Func_020051a8, 0, 0, 1, 1, pos, 29);
    Call6(Func_020051b2_b, 87, 42, 21, 31, 1, 2);
    Func_02005260(40);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x202;
    Func_020053ca();
    Func_020053d6();
    Call4(Func_020053a0, 0x2c80000, -1, 0x980000, 0);
    Func_020051cc();
    Func_02005162(1);
    Call3(Func_0200521a, 0x20000, 0x10000, 0x10000);
    Func_020053f6();
    Func_0200540a();
    Func_020052b0_b(40);
    Call1(Func_02005436, 0x121);
    Call3(Func_0200523c, -1, -1, 0xe666);
    Func_02005248();
    Func_020052ce(20);
    Call6(Func_02005238, 0, 40, 43, 46, 3, 3);
    Func_020052e8(20);
    obj = Func_02004bc6(221, 0x2c80000, 0x100000, 0x900000);
    Func_02005302(40);
    Func_0200529a(obj, 1);
    mes = (s32)Data_00001076;
    Value2(Func_0200529c, mes, 1);
    Call3(Func_02005408, 9, 0xe000, 0);
    Call3(Func_02005414, 5, 0xe000, 20);
    Func_02005488();
    Func_02005494();
    Call4(Func_0200545e, 0x1ce0000, -1, 0x15e0000, 0);
    Func_0200528a();
    Func_02005220(1);
    Func_020054a4();
    Func_020054b8();
    Func_0200535e(40);
    Func_020053f2(9, 4, 30);
    Func_02005430(mes - 2);
    Func_02004c2c(9, 20);
    Func_020054d4();
    Func_020054e0();
    Func_020054a8(0x2c80000, -1, 0x980000, 0);
    Func_020052d4();
    Func_0200526a(1);
    Func_020054ee();
    Func_02005502();
    Call1(Func_02005380, 0x83d);
    Func_020053bc();
}

void FieldScene_RunSevenSceneChain(void)
{
    Func_02005404();
    Func_02001504();
    Func_020017cc();
    Func_02001a00();
    Func_02001ec8();
    Func_02002208();
    Func_02002784();
    Func_020053f2_a(0x83E);
    Func_0200542e();
    Func_02002d0a();
}

void Func_02000a98(void)
{
    u32 i;
    s32 obj;

    Func_020055ba(141);
    for (i = 0; i != 6; i++) {
        Call2(Func_0200557c, 0x4049d2, 1);
        Func_02005592_a(8);
        Func_02005450(8);
        Call2(Func_02005592_b, 0x10000, 1);
        Func_020055a8(8);
        Func_02005466(8);
        if (i == 1) {
            Call3(Func_020053f2_b, 0x10000, 0x10000, 0x10000);
        }
    }
    Call3(Func_02005408_a, 0x20000, 0x10000, 0x10000);
    Func_02005496(30);
    Call2(Func_020055ae, 0x26666, 0x4ccc);
    Call4(Func_020055c6, 0xa70000, -1, 0x2110000, 1);
    Func_020055d2();
    Call3(Func_0200543a, 0x10000, 0x20000, 0x10000);
    Func_020054c8(20);
    Func_0200564e(144);
    Func_0200542c(0x200d088, 65, 31);
    Call6(Func_02005454, 0, 0, 1, 1, 10, 31);
    Call6(Func_02005460, 87, 42, 10, 33, 1, 2);
    Func_0200550e(40);
    Func_02005490(0, 0, 0);
    Call2(Func_02005630_a, 0x66666, 0xcccc);
    Call4(Func_02005648_a, 0x1870000, -1, 0xb10000, 1);
    Func_02005654();
    Call3(Func_020054bc, 0x10000, 0x20000, 0x10000);
    Func_0200554a(20);
    Func_020056d0(144);
    Func_020054aa(0x200d088, 79, 9);
    Call6(Func_020054ce, 0, 0, 1, 1, 24, 9);
    Call6(Func_020054d6, 87, 42, 24, 11, 1, 2);
    Func_02005584(40);
    Func_02005506(0, 0, 0);
    Call2(Func_020056a6, 0x26666, 0x4ccc);
    Call4(Func_020056be, 0x2470000, -1, 0xc10000, 1);
    Func_020056ca();
    Call3(Func_02005532, 0x10000, 0x20000, 0x10000);
    Func_020055c0(20);
    Func_02005746(144);
    Func_02005520(0x200d088, 91, 10);
    Call6(Func_02005544, 0, 0, 1, 1, 36, 10);
    Call6(Func_0200554c, 87, 42, 36, 12, 1, 2);
    Func_020055fa(40);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x202;
    Func_02005764();
    Func_02005770();
    Call4(Func_02005738, 0xe80000, -1, 0x1dd0000, 0);
    Func_02005564();
    Func_020054fa(1);
    Call3(Func_020055b2, 0x20000, 0x10000, 0x10000);
    Func_0200578e();
    Func_020057a2();
    Func_02005648_b(40);
    Call1(Func_020057ce, 0x121);
    Call3(Func_020055d4, -1, -1, 0xe666);
    Func_020055e0();
    Func_02005666(20);
    Call6(Func_020055d0, 0, 40, 13, 66, 3, 3);
    Func_0200567e(20);
    obj = Value4(Func_02004f5c, 223, 0xe80000, 0x100000, 0x1d00000);
    Func_02005698(40);
    Func_02005630_b(obj, 1);
    Call2(Func_02005630_c, 0x1077, 1);
}

/* Runs a sequence of position/scale/timing calls for actor pair 0 and 1,
 * copying a stored pair of 32-bit fields (offsets +8, +16) from actor 0's
 * record onto actor 1 partway through, then runs an analogous sequence for
 * actors 5, 9, 10 and 11. */
void FieldScene_StagePairedActors(void)
{
    u32 i;
    u8 *record;

    Audio_PlayCue_1(17);
    ObjectMotion_SetSpeedParameters_1(0, 0x8000, 0x4000);
    ObjectMotion_SetPositionAndReset_1(0, 231, 0x1ea);
    ObjectMotion_ArmCallback_1(0, 0xc000, 30);
    ObjectMotion_CallThenWaitForAnimationChange_1(0, 3);
    BattleRuntime_WaitIfModeZero_1(180);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(0, 2);
    BattleRuntime_WaitIfModeZero_2(80);
    BattleEffect_SpawnLinkedResourceObject_1(0, 0x101, 60);
    ObjectMotion_SetSpeedParameters_2(0, 0x8000, 0x4000);
    ObjectMotion_SetPositionAndReset_2(0, 246, 0x1df);
    ObjectMotion_ArmCallback_2(0, 0xe000, 10);
    /* Copy actor 0's stored fields at +8 and +16 onto actor 1, if a record
     * for actor 0 exists. */
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    ObjectMotion_SetSpeedParameters_3(1, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset_3(1, 0x101, 0x1eb);
    ObjectMotion_ArmCallback_3(0, 0x2000, 0);
    ObjectMotion_ArmCallback_4(1, 0xa000, 40);
    ObjectMotion_SetVariantCallback_1(0, 2);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 2);
    BattleRuntime_WaitIfModeZero_3(20);
    BattleEffect_SpawnLinkedResourceObject_2(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x101, 80);
    Object_SetModeById_1(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_2(1, 4);
    ObjectMotion_SetSpeedParameters_4(0, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_5(1, 0x13333, 0x9999);
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 0x109, 0x1c5);
    ObjectMotion_SetPositionAndReset_4(1, 0x11a, 0x1d5);
    Object_SetModeById_2(0, 1);
    ObjectMotion_ArmCallback_5(0, 0xe000, 0);
    ObjectMotion_ArmCallback_6(1, 0xe000, 40);
    BattleEffect_SpawnLinkedResourceObject_4(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_5(1, 0x100, 0);
    ObjectMotion_Launch_1(0, 6, 0);
    ObjectMotion_Launch_2(1, 6, 60);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(5, 0x1db0000, 0x14c0000);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(9, 0x1eb0000, 0x14c0000);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(11, 0x1cb0000, 0x15c0000);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(10, 0x1fb0000, 0x15c0000);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x73333, 0xe666);
    ObjectMotion_PlaceWithinCameraBounds_1(0x1e50000, -1, 0x1590000, 1);
    ObjectMotion_ArmCallback_7(5, 0x6000, 0);
    ObjectMotion_ArmCallback_8(9, 0x5000, 0);
    ObjectMotion_ArmCallback_9(11, 0x5000, 0);
    ObjectMotion_ArmCallback_10(10, 0x5000, 0);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleRuntime_WaitIfModeZero_4(40);
}

void Func_0200178c(void)
{
    u8 *obj;
    s32 other;
    s32 tbl;
    s32 left;
    s32 cnt;
    s32 mes_a;
    s32 mes_b;

    Func_020062a6(161);
    Func_020061c6(12, 3);
    Func_02006134(40);
    other = Value1(Func_02006162, 12);
    if (other != 0) {
        Func_020061b8(13, *(volatile s32 *)(other + 8), *(volatile s32 *)(other + 16));
    }
    Func_020061c2(12, 0, 0);
    Func_02006158(20);
    Call3(Func_0200624c_a, 13, 0x3000, 40);
    Func_02006204(5, 3);
    Func_0200620c(5, 3);
    Func_0200617a(40);
    Func_0200621a(5, 2);
    Func_02005a3e(5, 20);
    Func_02006212(13, 3);
    Func_02006198(10);
    Func_02006230(5, 3);
    Call3(Func_02006294, 9, 0x8000, 10);
    Func_0200624c_b(9, 2);
    Func_02005a70(9, 40);
    Func_02006244(5, 3);
    Func_020061ca(40);
    Call3(Func_020062be, 9, 0xb000, 40);
    Func_02006276(13, 2);
    Func_02005a9a(13, 20);
    Func_02006286_a(5, 1);
    Func_020061f4(40);
    Func_0200627c_a(13, 3);
    Func_02006202(10);
    Func_02005abe(13, 40);
    Func_020062a2(10, 1);
    Func_0200629a(10, 3);
    Func_02006220(10);
    Func_02005adc(10, 10);
    Func_020062c8(11, 1);
    Func_020062b0_a(11, 3);
    Func_02005af4(11, 80);
    Func_020062e0(13, 2);
    Func_02005b04(13, 40);
    Func_020062f0(5, 2);
    Func_0200625e(10);
    Func_02005b1a(5, 10);
    Func_020062fe(13, 2);
    Func_0200630e(9, 2);
    Func_0200627c_b(80);
    Func_02006304(5, 4);
    Func_0200628a(20);
    Func_02005b46(5, 80);
    Func_0200631a(13, 4);
    Func_02005b56(13, 80);
    Func_02006342(5, 2);
    Func_020062b0_b(4);
    Func_02005b6c(5, 20);
    Func_02006358(10, 1);
    Func_02006340(10, 3);
    Func_02005b84(10, 10);
    Func_02006370(11, 1);
    Func_02005b94(11, 10);
    Func_02006380(10, 1);
    Func_02005ba4(10, 10);
    Call3(Func_020063e4, 9, 0x3000, 80);
    Call3(Func_020063fe, 9, 0x105, 80);
    Func_020063a6(11, 1);
    Call3(Func_02006402_a, 11, 0x5000, 40);
    Func_020063b2(11, 2);
    Func_02005bde(11, 20);
    Func_02006486();
    Func_02006492();
    Call4(Func_0200645a_a, 0x1050000, -1, 0x1d20000, 0);
    Func_02006286_b();
    Func_0200621c(1);
    Func_020064a0();
    Func_020064b4();
    Func_0200635a(20);
    Call3(Func_0200644e_a, 0, 0x3000, 0);
    Call3(Func_0200645a_b, 1, 0xb000, 20);
    Func_0200640a(0, 2);
    Func_0200641a(1, 2);
    Call3(Func_020063c0, 0, 0x8000, 0x4000);
    Call3(Func_020063ce, 1, 0x8000, 0x4000);
    Call3(Func_02006402_b, 0, 244, 0x1de);
    Call3(Func_02006418, 1, 0x104, 0x1ea);
    Func_02006426(0);
    Func_0200643e(0, 1);
    Call3(Func_020064ba, 0, 0x3000, 0);
    Call3(Func_020064c6, 1, 0xb000, 20);
    Func_0200647e(0, 2);
    Func_0200646e_a(1, 4);
    Func_020063f4(20);
    tbl = Func_020063a2(1);
    cnt = 0;
    tbl += 216;
    left = 14;
    do {
        u32 id = *(volatile u16 *)(tbl)& 0x1ff;
        tbl = (tbl + 2);
        if (id - 220 > 1) {
            if (id != 223) {
                goto L_02001a86;
            }
        }
        cnt = (cnt + 1);
        L_02001a86:;
        left = (left - 1);
    } while (left >= 0);
    Value2(Func_020064ee, 1, 0);
    if (Value2(Func_0200644e_b, 0, 0) == 0) {
        mes_a = (s32)Data_000010b0;
        ((void (*)())Func_020064fa_a)(mes_a);
        Func_020064c2(1, 3);
        Func_02006448(10);
        if (cnt <= 2) {
            Func_02005d08(1, 30);
            Call3(Func_020064c0, 1, 252, 0x1e6);
            Func_02006500(1, 2);
            Func_0200646e_b(10);
            Func_02006410((mes_a + 1), 1, 0);
            goto L_02001bdc;
        }
        Call1(Func_02006540, 0x10b4);
        Func_02005d3c(1, 30);
    } else {
        if (cnt <= 2) {
            mes_b = (s32)Data_000010b2;
            Func_02006556_a(mes_b);
            Func_02006536_a(1, 3);
            Func_02006526(1, 4);
            Func_02005d62(1, 10);
            Func_02006536_b(1, 4);
            Func_02006556_b(1, 1);
            Call3(Func_020064fc, 1, 0x20000, 0x10000);
            obj = Value1(Func_020064fa_b, 0);
            obj[90] &= 254;
            Call3(Func_0200654c, 1, 244, 0x1de);
            Call3(Func_0200652a, 0, 0x18000, 0xc000);
            Func_0200658c(0, 6, 0);
            Call3(Func_02006556_c, 0, 218, 0x1d7);
            Func_0200657e(0);
            Func_020064b8((mes_b + 1), 1, 0);
            Func_020065c0(0, 2);
            Func_0200661a(0, 0, 30);
            {
                u8 value = *(volatile u8 *)&obj[90];

                obj[90] = (u8)(value | 1);
            }
        } else {
            Call1(Func_02006602, 0x10b5);
            Func_020065e2_a(1, 3);
            Func_020065d2(1, 4);
            Func_02005e0e(1, 10);
            Func_020065e2_b(1, 4);
            Call3(Func_02006656, 0, 0xe000, 30);
        }
    }
    L_02001bdc:;
    Call2(Func_0200668a_a, 0x8000, 0x1000);
    Func_0200668a_b(1, 1);
    Func_020066a6();
    Call3(Func_0200667a, 1, 0x8000, 30);
    Func_02006632(1, 2);
    Call3(Func_020065d8, 1, 0x8000, 0x4000);
    obj = Value1(Func_020065d6, 1);
    obj[90] &= 254;
    Call3(Func_0200662a, 1, 0x108, 0x1e2);
    {
        u8 value = *(volatile u8 *)&obj[90];

        obj[90] = (u8)(value | 1);
    }
    Call3(Func_02006640, 1, 0x116, 0x1e0);
    *(volatile s32 *)((s32)obj + 48) = 0x30000;
    *(volatile s32 *)((s32)obj + 52) = 0x20000;
    Func_02006776(153);
    *(volatile s32 *)((s32)obj + 40) = 0x60000;
    Func_02006678(1, 7);
    Call3(Func_0200665e, 1, 0x138, 0x1d6);
    Func_0200668e(1, 1);
    Func_0200661c(30);
    Func_020067a2(153);
    *(volatile s32 *)((s32)obj + 40) = 0x60000;
    Func_020066a4(1, 7);
    Call3(Func_0200668a_c, 1, 0x156, 0x1d6);
    Func_020066ba(1, 1);
    Func_02006648(30);
    Func_020067ce(153);
    *(volatile s32 *)((s32)obj + 40) = 0x60000;
    Func_020066d0(1, 7);
    Call3(Func_020066b6, 1, 0x178, 0x1d6);
    Func_020066e6(1, 1);
}

void FieldScene_RunActorFourteenGuestScene(void)
{
    u32 i;
    u8 *rec;
    s32 record;
    s32 base6_10b6;
    s32 base5_200a;
    s32 none;
    s32 v5;

    Func_0200673c(1, 3);
    Func_020066aa(10);
    Call3(Func_0200679e, 1, 0x3000, 0);
    record = Func_020066e4(14);
    Func_0200662a_a(record, 0);
    Func_0200677a(14, 15);
    Call3(Func_02006748, 14, 0x1880000, 0x1c60000);
    Func_02006104();
    Call3(Func_020067d0, 1, 0xd000, 10);
    Func_02006780(1, 2);
    Call3(Func_020067f4, 1, 0x100, 40);
    Call3(Func_020067f0, 14, 0x5000, 10);
    Func_020067a8(14, 2);
    Func_02006716(20);
    base6_10b6 = (s32)Data_000010b6;
    Func_020067de(base6_10b6);
    Func_020067f6_a(14, 0);
    Call3(Func_020067a4, 10, 0x1d50000, 0x15c0000);
    base5_200a = 0x200a;
    Func_0200673a(20);
    Func_02005ff6(base5_200a, 10);
    Func_02005ffe(base5_200a, 40);
    Call3(Func_020067c6, 10, 0x1fb0000, 0x15c0000);
    Func_020067f6_b(1, 2);
    Func_02006764(40);
    Func_020067ec_a(1, 3);
    Func_02006772(40);
    Call3(Func_020067b0, 1, 0x8000, 0x4000);
    Call3(Func_020067ec_b, 1, 0x185, 0x1d4);
    Call3(Func_02006880, 1, 0xd000, 60);
    Func_02006054(1, 20);
    Func_02006742((base6_10b6 + 4), 1, 10);
    Call3(Func_020067e8, 1, 0x8000, 0x4000);
    rec = Value1(Func_020067e6, 1);
    rec[90] &= 254;
    none = 0;
    Call3(Func_0200683e_a, 1, 0x178, 0x1d6);
    Func_020067e4(30);
    {
        u8 value = *(volatile u8 *)&rec[90];

        rec[90] = (u8)(value | 1);
    }
    Func_02006874(14, 4);
    Func_020067fc(10);
    Func_020068c2((base6_10b6 + 5));
    Func_020060be(14, 20);
    Call3(Func_0200690c, 1, 0x101, 60);
    Func_0200689c(14, 3);
    Func_020060d8(14, 20);
    Call3(Func_02006928, 1, 0x102, 60);
    Func_020068b8(14, 3);
    Func_0200683e_b(20);
    Call3(Func_02006932, 14, 0xc000, 20);
    Call2(Func_02006904, 14, 0x100);
    record = Func_02006882(14);
    Func_020067c8(record, 0);
    rec = Value1(Func_0200688e, 14);
    rec[85] = none;
    v5 = 0;
    Func_020069f6(220);
    for (i = 0; i != 30; i++) {
        *(volatile s32 *)((s32)rec + 12) += 0x10000;
        Func_0200688a(1);
        v5 = i;
    }
    rec[85] = 5;
    Func_0200692a(1, 2);
    Func_02006156(1, 10);
    Call3(Func_020069a4, 14, 0x101, 60);
    Call3(Func_020069a0, 14, 0x5000, 10);
    Func_02006174(1, 20);
    Func_02006960(14, 1);
    Func_02006184(14, 20);
    Call3(Func_020069d2, 1, 0x103, 20);
    Func_02006196(1, 30);
    Call3(Func_020069e4, 14, 0x105, 80);
    Call3(Func_020069e0_a, 14, 0xd000, 40);
    Call3(Func_020069ec, 10, 0x5000, 10);
    Func_02006a60();
    Func_02006a6c();
    Call4(Func_02006a34, 0x1dd0000, -1, 0x14e0000, 0);
    Func_02006860();
    Func_020067f6_c(1);
    Func_02006a7a();
    Func_02006a8e();
    Func_02006934(20);
    Func_020069bc(10, 4);
    Func_020061f8(10, 10);
    Value2(Func_02006a14, 11, 0);
    Call2(Func_02006a64, 0x66666, 0xcccc);
    Call4(Func_02006a7e_a, 0x1760000, -1, 0x1d60000, 1);
    Func_02006a8a();
    Call3(Func_02006a5e, 14, 0x5000, 0);
    Call3(Func_02006a6a, 1, 0xe000, 10);
    Func_02006a22(1, 2);
    if (Value2(Func_020069b2, 1, 0) != 0) {
        s32 code;

        Func_0200699c(10);
        Value2(Func_02006a24_a, 14, 4);
        do {
            code = 0x10c3;
        } while (0);
        for (;;) {
            Call1_02001d04(Func_02006ab4, code);
            Value2(Func_02006ac4, 14, 0);
            if (Value2(Func_02006a24_b, 1, 0) != 0)
                break;
            Func_020069e0_b(20);
            Value2(Func_02006a68, 14, 4);
            Func_020069ee(10);
            code = 0x10c6;
        }
    }
    Func_02006a0e(30);
    Func_02006a96(14, 3);
    Func_02006a1c(20);
    Call1_02001d04(Func_02006ae2, 0x10c4);
    Func_020062de(14, 30);
    Func_02006ab2(14, 3);
    Func_02006a38(10);
    Func_020062f4(14, 30);
    rec[85] = 0;
    Call3(Func_02006a7e_b, 14, 0x26666, 0x13333);
    Call4(Func_020069a6, (s32)rec, 0x1cc0000, 0, 0x1680000);
    Func_02006acc(14);
    Func_02006b1c(14, 0);
    record = Func_02006a9a(14);
    Func_020069e0_c(record, 1);
    Func_02006a7e_c(30);
    Func_02006b8e(1, 1);
    Func_02006baa();
    Func_02006a90(40);
    Call3(Func_02006b92_a, 1, 0x103, 40);
    Func_02006b3a_a(1, 3);
    Func_02006aa8(20);
    rec = Value1(Func_02006ad6, 1);
    {
        u8 value = *(volatile u8 *)&rec[90];

        rec[90] = (u8)(value | 1);
    }
    *(volatile s32 *)((s32)rec + 48) = 0x30000;
    *(volatile s32 *)((s32)rec + 52) = 0x20000;
    Func_02006c52(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Func_02006b54(1, 7);
    Call3(Func_02006b3a_b, 1, 0x156, 0x1d6);
    Func_02006b6a(1, 1);
    Func_02006af8(30);
    Func_02006c7e(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Func_02006b80(1, 7);
    Call3(Func_02006b66, 1, 0x138, 0x1d6);
    Func_02006b96(1, 1);
    Func_02006b24(30);
    Func_02006caa(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Func_02006bac(1, 7);
    Call3(Func_02006b92_b, 1, 0x116, 0x1e0);
    Func_02006bc2(1, 1);
    Func_02006b50(30);
    Call2(Func_02006c6c_a, 0x8000, 0x1000);
    Func_02006c6c_b(0, 1);
    Call3(Func_02006b9e, 1, 0x19999, 0xcccc);
    Func_02006c20(0, 1, 0);
    Func_02006b7e(30);
    Func_02006c06(1, 3);
    Func_02006c0e(0, 4);
    Func_02006c2e(1, 2);
    Func_02006c1e(0, 3);
    Func_02006ba4(20);
    Func_02006c24(1, 2);
    {
        s32 slot = Value1(Func_02006bda, 0);

        if (slot != 0) {
            Func_02006c0c(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
        }
    }
    Func_02006c32(1);
    Func_02006c44(1, 0, 0);
    Func_02006bd2(220);
    Func_02006bd8(221);
    Func_02006bde(223);
}

void FieldScene_RunSixPassEffectSequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v5;

    Func_02006da2();
    Func_02006f20(141);
    v5 = 0;
    do {
        Call2(Func_02006ee2_a, 0x4039d2, 1);
        Func_02006ef8_a(8);
        Func_02006db6(8);
        Call2(Func_02006ef8_b, 0x10000, 1);
        Func_02006f0e(8);
        Func_02006dcc(8);
        if (v5 == 1) {
            Call3(Func_02006d58, 0x10000, 0x10000, 0x10000);
        }
        v5 = ((u32)((v5 + 1) << 24) >> 24);
    } while (v5 != 6);
    Call1_02002400(Func_02006f70, 0x121);
    Call3(Func_02006d76, -1, -1, 0xe666);
    Call6(Func_02006d68, 0, 40, 13, 46, 3, 3);
    Func_02006e16(20);
    rec7 = Value4(Func_020066f4, 222, 0xe80000, 0x100000, 0x900000);
    Func_02006e30(40);
    Func_02006dc8_a(rec7, 1);
    Call2(Func_02006dc8_b, 0x1078, 1);
    Call3(Func_02006eba_a, 5, 0x1330000, 0x1150000);
    Call3(Func_02006ec4_a, 9, 0x1330000, 0x1150000);
    Call3(Func_02006ece, 11, 0x1330000, 0x1150000);
    Call3(Func_02006ed8, 10, 0x1330000, 0x1150000);
    Call3(Func_02006ee2_b, 14, 0x1330000, 0x1150000);
    Call3(Func_02006eac, 0, 0x13333, 0x9999);
    Func_02006ee6(0, 232, 156);
    Func_02006e8c(10);
    record = Value1(Func_02006eba_b, 0);
    if (record != 0) {
        Func_02006f10(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_02006eda, 1, 0x13333, 0x9999);
    Func_02006f14(1, 218, 172);
    Func_02006f66(1, 0, 0);
    Func_02006ec4_b(20);
    Func_0200704a(145);
    Call3(Func_02006e52, 0x40000, 0x40000, 0x10000);
    Func_02006ee0(20);
    Call3(Func_02006e68, 0x10000, 0x10000, 0x10000);
    Func_02006ef6(40);
    Call3(Func_02006fea, 0, 0xd000, 0);
    Call3(Func_02006ff6, 1, 0x5000, 50);
    Func_02007094(144);
    Call3(Func_02006e9c, 0x30000, 0x30000, 0x10000);
    Call3(Func_02007018, 0, 0x8000, 0);
    Func_02007022(1, 0, 50);
    Call3(Func_02006ec2, 0x10000, 0x10000, 0x10000);
    Func_0200703c(0, 0, 0);
    Call3(Func_02007048, 1, 0x8000, 50);
    Call3(Func_02007054, 0, 0xb000, 0);
    Call3(Func_02007060, 1, 0xd000, 0);
    Func_020070fe(144);
    Call3(Func_02006f06, 0x30000, 0x30000, 0x10000);
    Func_02006f94(30);
    Func_02007026(0, 2, 0);
    Func_02007030(1, 2, 20);
    Func_0200703a(0, 6, 0);
    Func_02007044(1, 6, 40);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Value0(Func_02007126);
    Func_02007132();
    Func_02007100(2);
}

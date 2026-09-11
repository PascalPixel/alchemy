#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareObject      Func_02000048
#define AcquireOverlayObject      Func_0200491e
#define RunOverlayObjectCommand0  Func_0200497c
#define RunOverlayObjectCommand1  Func_02004994
#define RunOverlayObjectCommand14 Func_02004ad4
#define CreateOverlayObject Func_02004976
#define SetOverlayObjectMode Func_020049d8
#define SetOverlayObjectSlot Func_02004b30
#define CalculateAngleFromCoordinateDelta Func_02004bb2
#define OverlayObject_SetHandleFlagBits2And3 Func_02000030
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define UpdateOverlayObjectAngle Func_02000314
#define SceneEffect_UpdateMotion Func_02000104
#define ConfigurableEffectDescriptors Data_0200cbc4
#define GetPrimaryEffectRecord Func_02004b1a
#define SpawnConfiguredEffectObject Func_02004a48
#define SetConfiguredEffectVariant Func_02004a52
#define ApplyConfiguredEffectDescriptor Func_02004a6c
#define SetConfiguredEffectLink Func_02004c66
#define ComputeConfiguredEffectRateFromDelta Func_02004ad0
#define ComputeConfiguredEffectRateFromOffset Func_02004ae8
#define ComputeConfiguredEffectSecondRate Func_02004af6
#define SetConfiguredEffectCallbackMode Func_02004b6c
#define ApplyConfiguredEffectCallbackArgument Func_02004b7c
#define Overlay380_SpawnConfiguredEffect Func_0200013c
#define SceneData_GetTableCd88 Func_0200036c
#define SceneData_ReturnZero Func_02000374
#define SceneData_GetTableCDB8 Func_02000378
#define SceneData_GetTableCdc4 Func_02000380
#define SceneData_GetTableCFA4 Func_02000388
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
#define RunEventScript01 Func_02001450
#define FieldScene_RunActorNineDialogueByFlags Func_02002674
#define SceneDialogue_RunActorFiveTalk Func_020026e0
#define SceneDialogue_RunActorTenDialogue Func_0200274c
#define SceneDialogue_RunActor11TalkLine Func_0200276c
#define SceneDialogue_ShowLine10C8 Func_0200278c
#define SceneDialogue_RunActor14TalkLine Func_020027ac
#define SceneDialogue_RunLine1072WithPair9And10 Func_020027cc
#define Lifted_020027f8 Func_020027f8
#define SceneState_PumpUntilSlotsFree Func_02004328
#define FieldScene_Forward72b4 Func_020027ec
#define FieldScene_RunSplitPairSteps Func_02004248

#include "create_configured_overlay_object.h"
#include "overlay_object.h"

typedef struct {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} S;

union MotionWork {
    struct {
        u32 unk_00[2];
        s32 x, y, z;
        u32 unk_14;
        s32 accum_x, accum_y;
        u32 unk_20[4];
        s32 rate_x, rate_y;
        u32 unk_38[3];
        s32 velocity_x, velocity_y, velocity_z;
        u16 *record;
        u8 unk_54[16];
        u16 angle_step;
    } fields;
    u8 bytes[102];
};

/*
 * Configurable spawn/copy owner at 0x0200013c.  Its code runs through the
 * unwind at 0x02000306; the following three pool words belong to this owner:
 * 0x0200cbc4, 0x02008105, and 0xffff0000.  The next owner begins at 0x02000314.
 */
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Options {
    u8 mode_bits;
    u8 pad01[3];
    s32 link;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct Descriptor {
    s32 pad00[3];
    s32 duration;
};

extern struct Descriptor *Data_0200cbc4[];
extern s32 Data_0200cd88[];
extern s32 Data_0200cdb8[];
extern s32 Data_0200cdc4[];
extern s32 Data_0200cfa4[];
extern u8 Data_00001076[];
extern u8 Data_03001ebc[];
extern u8 Data_000010b0[];
extern u8 Data_000010b2[];
extern u8 Data_000010b6[];
extern u8 Data_0000200a[];
extern u8 Data_00004009[];
extern u8 Data_00008009[];
extern void Func_02008e52(s32 arg0);                        /* 0x0200433c -> 080f9010 */
extern void Func_02008e4a(s32 arg0, s32 arg1);              /* 0x02004344 -> 0808a398 */
extern void Func_02008c6a(s32 arg0, s32 arg1);              /* 0x0200434c -> 08015040 */
extern s32 Func_02008cd0(s32 arg0);                         /* 0x02004352 -> 08077248 */
extern s32 Func_02008cda(s32 arg0);                         /* 0x0200435c -> 08077248 */
extern void Func_02008c88(s32 arg0, s32 arg1);              /* 0x0200436a -> 08015040 */
extern s32 Func_02008e80(s32 *out_first, s32 *out_second);  /* 0x02004372 -> 080b0058 */
extern void Func_02008d08(s32 arg0, s32 arg1);              /* 0x02004382 -> 080772b0 */
extern void Func_02008cd8(s32 arg0);                        /* 0x0200438a -> 08077030 */
extern void Func_02008cde(s32 arg0);                        /* 0x02004390 -> 08077030 */
extern void Func_02008ce4(s32 arg0);                        /* 0x02004396 -> 08077030 */
extern void Func_02008cea(s32 arg0);                        /* 0x0200439c -> 08077030 */

void *Func_0200491e(s32, s32, s32, s32);
void Func_0200497c(void *, s32);
void Func_02004994(void *, s32);
void Func_02004ad4(void *, s32);
u16 Func_02004bb2(s32, s32);
struct Effect *Func_02004b1a(s32 index);
struct Effect *Func_02004a48(s32 kind, s32 x, s32 y, s32 z);
void Func_02004a52(struct Effect *effect, s32 mode);
void Func_02004a6c(struct Effect *effect, struct Descriptor *descriptor);
void Func_02004c66(struct Effect *effect, s32 link);
s32 Func_02004ad0(s32 delta, s32 duration);
s32 Func_02004ae8(s32 delta, s32 duration);
s32 Func_02004af6(s32 delta, s32 duration);
void Func_02004b6c(struct Effect *effect, s32 mode);
void Func_02004b7c(struct Effect *effect, struct Descriptor *descriptor);
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
void Func_020056e4();
void Func_02005704();
void Func_0200574c();
void Func_020057c4();
void Func_0200593e_a();
void Func_0200595e();
void Func_02005974();
void Func_020059a4();
void Func_02005e18();
void Func_02005e48();
void Func_02005e68();
void Func_02005e74();
void Func_02005eb0();
void Func_02005ec0();
void Func_02005ed0();
void Func_02005ed2();
void Func_02005ed8();
void Func_02005edc();
void Func_02005ee4();
void Func_02005ef0();
void Func_02005efa();
void Func_02005efc();
void Func_02005f04();
void Func_02005f08_a();
void Func_02005f08_b();
void Func_02005f0e();
void Func_02005f14();
void Func_02005f2a_a();
void Func_02005f2a_b();
void Func_02005f34();
void Func_02005f42();
void Func_02005f5e();
void Func_02005f6e();
void Func_02005f70();
void Func_02005f88();
void Func_02005f8a();
void Func_02005f94();
void Func_02005fa2();
void Func_02005fa6();
void Func_02005fae();
void Func_02005fb2();
void Func_02005fc6();
void Func_02005fd0();
void Func_02005fd4();
void Func_02005fdc();
void Func_02005fe2();
void Func_02005fe8();
void Func_02005fee();
void Func_02005ff0();
void Func_02006014();
void Func_02006020_a();
void Func_02006020_b();
void Func_02006026();
void Func_02006044();
void Func_02006050();
void Func_0200605c();
void Func_0200605e();
void Func_02006068();
void Func_02006076();
void Func_0200609a();
void Func_020060a2_a();
void Func_020060a2_b();
void Func_020060a6();
void Func_020060ae();
void Func_020060b2();
void Func_020060b6_a();
void Func_020060b6_b();
void Func_020060b8();
void Func_020060c4();
void Func_020060ca();
void Func_020060ce();
void Func_020060d0();
void Func_020060ec();
void Func_020060f2();
void Func_02006100();
void Func_02006108();
void Func_02006114();
void Func_0200611c();
void Func_02006120();
void Func_0200614a();
void Func_02006160();
void Func_02006168_a();
void Func_02006168_b();
void Func_0200616a();
void Func_0200617c();
void Func_02006188();
void Func_02006194();
void Func_02006198_a();
void Func_020061a0();
void Func_020061ae();
void Func_020061bc();
void Func_020061c2_a();
void Func_020061c8();
void Func_020061f4_a();
void Func_02007014(void);
s32 Func_02006fe2(s32 flag);
void Func_020070dc(s32 msg);
void Func_020070f4(s32 id, s32 arg1);
s32 Func_02006ffc(s32 flag);
void Func_020070f6(s32 msg);
void Func_020070fe_a(s32 msg);
void Func_020070f0(s32 id, s32 arg1, s32 arg2);
void Func_0200704e(s32 frames);
void Func_02007126_a(s32 id, s32 arg1);
void Func_0200706a(void);
void Func_02007080(void);
s32 Func_0200704e_a(s32 flag);
void Func_02007148(s32 msg);
void Func_02007160(s32 id, s32 arg1);
s32 Func_02007068(s32 flag);
void Func_02007162(s32 msg);
void Func_0200716a(s32 msg);
void Func_0200715c(s32 id, s32 arg1, s32 arg2);
void Func_020070ba(s32 frames);
void Func_02007192(s32 id, s32 arg1);
void Func_020070d6(void);
void Func_02007106(void);
void Func_020071aa(s32 line);
void Func_020071c2(s32 id, s32 arg1);
void Func_02007126_b(void);
void Func_020071ca(s32 no);
void Func_020071e2(s32 no, s32 val);
void Func_02007146(void);
void Func_020071ea(s32 dialogueId);
void Func_02007202(s32 id, s32 arg1);
void Func_02007166(void);
void Func_0200720a(s32 no);
void Func_02007222(s32 no, s32 val);
void Func_02006a26(s32 a, s32 b);
void Func_02007186(void);
void Func_0200722a(s32 no);
void Func_020072b4(void);
void Func_020070ae();
void Func_02007108();
void Func_02007112();
void Func_02007172();
void Func_020071a4();
void Func_020071a6();
void Func_020071b8();
void Func_020071f4();
struct ObjectRuntime *Func_020071f6(u32);
struct ObjectRuntime *Func_02007204(u32);
void Func_02007234();
void Func_02007272();
void Func_0200727a();
void Func_02007292();
void Func_020072a6();
void Func_020072b0();
void Func_020072ba();
void Func_020072d2();
void Func_020072d8();
void Func_020072e4();
u8 *Func_02007308();
void Func_0200730c();
void Func_02007334();
void Func_0200733e();
void Func_02007342();
void Func_02007352();
s32 Func_02007372_a();
struct ObjectRuntime *Func_02007372_b(u32);
void Func_0200737a();
struct ObjectRuntime *Func_02007386(u32);
void Func_0200738e_a();
void Func_0200738e_b();
void Func_020073a2();
void Func_020073a6();
void Func_020073aa_a();
void Func_020073aa_b();
void Func_020073b8();
void Func_020073bc();
void Func_020073c2();
void Func_020073c6();
void Func_020073c8();
void Func_020073ca();
void Func_020073d4();
void Func_020073dc();
void Func_020073ec();
void Func_020073ee();
void Func_02007400();
void Func_0200741c();
void Func_02007420_a();
void Func_02007420_b();
void Func_02007426();
void Func_02007444();
void Func_02007468();
void Func_02007476();
void Func_0200747e();
void Func_02007480();
void Func_02007488();
void Func_0200748a();
void Func_02007492();
void Func_02007496();
void Func_0200749a();
void Func_0200749c();
void Func_020074a2();
void Func_020074a6();
void Func_020074a8();
void Func_020074b2();
void Func_020074b6();
void Func_020074c4();
void Func_020074c6();
void Func_020074ce();
void Func_020074d0();
void Func_020074d2();
void Func_020074dc();
void Func_020074de();
void Func_020074ea();
void Func_020074f4_a();
void Func_020074f4_b();
void Func_020074fe();
void Func_02007502();
void Func_0200750c();
void Func_02007510();
void Func_0200751a();
void Func_02007528();
void Func_0200752a();
void Func_02007534();
void Func_0200753e();
void Func_0200754a();
void Func_02007558();
void Func_0200757a();
void Func_020075bc();
void Func_020075c2();
void Func_020075ce();
void Func_020075d2();
void Func_020075da();
void Func_020075dc();
void Func_020075e6();
void Func_020075ec();
void Func_020075f0();
void Func_020075fc();
void Func_02007606();
void Func_0200760c();
void Func_02007612();
void Func_0200761c();
struct ObjectRuntime *Func_02007622(u32);
void Func_0200763a();
void Func_0200764c();
void Func_02007678();
void Func_0200767c();
void Func_02007684();
void Func_02007696();
void Func_020076ca();
void Func_020076da();
void Func_020076dc();
void Func_020076e4();
void Func_020076ee_a();
void Func_020076ee_b();
void Func_02007700();
void Func_02007706();
void Func_02007716();
void Func_02007718();
void Func_02007724();
void Func_02007730();
void Func_0200775a();
u8 *Func_02007766_a();
u8 *Func_02007766_b();
u8 *Func_02007766_c();
void Func_02007772();
void Func_02007782();
void Func_020077a0();
void Func_020077a6_a();
void Func_020077a6_b();
void Func_020077b2();
void Func_020077bc();
void Func_020077c0();
void Func_020077c8();
void Func_020077d0();
void Func_020077d8();
void Func_020077dc();
void Func_020077f2();
void Func_02007800();
void Func_02007820();
void Func_02007828();
void Func_02007834_a();
void Func_02007834_b();
void Func_02007838();
void Func_0200783a();
void Func_0200784e();
void Func_02007852();
void Func_0200785a();
void Func_0200785c();
void Func_0200786a_a();
void Func_0200786a_b();
void Func_02007870();
void Func_02007884();
void Func_02007886();
void Func_0200789a();
void Func_020078a6();
void Func_020078b2();
void Func_020078b6();
void Func_020078c8();
void Func_020078cc();
void Func_020078ce();
void Func_020078d6();
void Func_020078da();
void Func_020078e0();
void Func_02007902();
void Func_02007904();
void Func_02007910();
void Func_0200791c();
void Func_02007928();
void Func_02007930();
void Func_02007938_a();
void Func_02007938_b();
void Func_0200795c();
void Func_0200795e();
void Func_02007966();
void Func_02007968();
void Func_0200796c();
void Func_02007970();
void Func_02007974();
void Func_02007976();
void Func_02007980();
void Func_020079b8();
void Func_020079c0();
void Func_020079c4_a();
void Func_020079c4_b();
void Func_020079c8();
void Func_020079d8();
void Func_020079da();
void Func_020079e0();
void Func_020079e4();
void Func_020079e6();
void Func_020079f0();
void Func_020079f8();
void Func_020079fe();
void Func_02007a00();
void Func_02007a04();
void Func_02007a1e();
void Func_02007a28();
void Func_02007a3a();
void Func_02007a62();
void Func_02007a74();
void Func_02007a80();
void Func_02007a82();
void Func_02007a88();
void Func_02007a94();
void Func_02007a96();
void Func_02007a98();
void Func_02007a9e_a();
void Func_02007a9e_b();
void Func_02007aa8();
void Func_02007ab2();
void Func_02007ab4_a();
void Func_02007ab4_b();
void Func_02007abe();
void Func_02007ac2();
void Func_02007ac4();
void Func_02007aca();
void Func_02007aea();
void Func_02007af2();
void Func_02007b00();
void Func_02007b1e();
void Func_02007b26();
void Func_02007b42();
void Func_02007b44();
void Func_02007b4c();
void Func_02007b4e();
void Func_02007b56();
void Func_02007b5a();
void Func_02007b62();
void Func_02007b6e();
void Func_02007b74();
void Func_02007b7c();
void Func_02007b80();
void Func_02007b8c();
void Func_02007b92();
s32 Func_02007bac();
void Func_02007bb0();
void Func_02007bb8();
void Func_02007bd4();
void Func_02007c12();
void Func_02007c1a();
void Func_02007c24();
s32 Func_02007c36();
void Func_02007c44();
void Func_02007c4e();
void Func_02007c60();
void Func_02007c6a();
void Func_02007c6c();
void Func_02007c72();
void Func_02007c74();
void Func_02007c7a();
void Func_02007c92();
void Func_02007c94();
void Func_02007ca8();
void Func_02007cae_a();
void Func_02007cae_b();
void Func_02007cb0();
void Func_02007cb4();
void Func_02007cba_a();
void Func_02007cba_b();
void Func_02007cbe();
void Func_02007cc0();
void Func_02007cc6();
void Func_02007cca();
void Func_02007cd6();
void Func_02007ce4();
void Func_02007cf2();
void Func_02007d0c();
void Func_02007d0e();
void Func_02007d1a();
void Func_02007d34();
void Func_02007d38();
void Func_02007d44();
void Func_02007d4a();
void Func_02007d50();
void Func_02007d5a();
void Func_02007d64();
void Func_02007d76();
void Func_02007d7e();
void Func_02007d84();
void Func_02007d88();
void Func_02007d8c();
void Func_02007d9c();
void Func_02007da0();
void Func_02007db4_a();
void Func_02007db4_b();
void Func_02007dc0();
void Func_02007dce();
void Func_02007de6();
void Func_02007df2_a();
void Func_02007df2_b();
void Func_02007dfe_a();
void Func_02007dfe_b();
void Func_02007e08();
void Func_02007e0a();
void Func_02007e1e_a();
void Func_02007e1e_b();
void Func_02007e22_a();
void Func_02007e22_b();
void Func_02007e24();
void Func_02007e3a();
void Func_02007e42();
void Func_02007e4a();
void Func_02007e52();
void Func_02007e56();
void Func_02007e72();
void Func_02007e76();
void Func_02007e84();
void Func_02007e8a();
void Func_02007e8c_a();
void Func_02007e8c_b();
void Func_02007e98();
void Func_02007eae();
void Func_02007eba();
void Func_02007ec6();
void Func_02007ee0();
void Func_02007f00();
void Func_02007f08();
void Func_02007f12();
void Func_02007f14();
void Func_02007f16_a();
void Func_02007f16_b();
void Func_02007f1a();
void Func_02007f32();
void Func_02007f34();
void Func_02007f3a();
void Func_02007f4a_a();
void Func_02007f4a_b();
void Func_02007f56();
void Func_02007f6a();
void Func_02007f78();
void Func_02007f82();
void Func_02007f84();
void Func_02007fa6();
void Func_02007fb2();
void Func_02007fb4();
void Func_02007fbe();
void Func_02007fd8();
void Func_02007fe0();
void Func_02007ffc();
void Func_02008000();
void Func_0200800c();
void Func_02008010();
void Func_02008012_a();
void Func_02008012_b();
s32 Func_02008018();
void Func_02008034();
void Func_02008036_a();
void Func_02008036_b();
void Func_02008038();
void Func_0200803e();
void Func_02008040();
void Func_02008042();
void Func_0200804c();
void Func_0200804e();
void Func_02008060();
void Func_02008064();
void Func_02008068();
void Func_0200807c();
void Func_0200807e();
s32 Func_020080b8_a();
s32 Func_020080b8_b();
void Func_020080c2();
void Func_020080c8();
void Func_020080ca();
void Func_020080cc();
void Func_020080ec();
s32 Func_020080ee();
void Func_020080f0();
void Func_02008106();
void Func_02008120();
void Func_02008150();
void Func_0200815a();
void Func_02008164();
void Func_0200816a();
void Func_02008176_a();
void Func_02008176_b();
void Func_02008182();
void Func_02008188();
s32 Func_0200818e();
void Func_02008194();
void Func_02008196();
void Func_0200819a();
void Func_020081a0();
void Func_020081a2();
void Func_020081a6();
void Func_020081cc();
void Func_020081ce();
void Func_020081d2();
void Func_020081d6();
void Func_020081da();
void Func_020081e4_a();
void Func_020081e4_b();
void Func_020081ec();
void Func_020081f0();
void Func_020081fc();
void Func_0200820e();
void Func_02008216();
void Func_0200821a();
void Func_0200823a();
void Func_02008240();
void Func_0200825e();
void Func_02008260();
void Func_02008282();
void Func_02008288();
void Func_02008290_a();
void Func_02008290_b();
void Func_02008292();
void Func_0200829c();
void Func_020082a0();
void Func_020082a6();
void Func_020082bc();
void Func_020082c2();
struct ObjectRuntime *Func_020082c6(u32);
void Func_020082cc();
void Func_020082d6();
void Func_020082e0();
void Func_02008300();
void Func_02008304();
void Func_0200830a();
void Func_0200830c_a();
void Func_0200830c_b();
void Func_0200831c();
void Func_02008326();
void Func_0200832e();
void Func_0200833a();
void Func_02008346();
void Func_02008368();
void Func_02008372();
void Func_0200837a();
void Func_0200837c();
void Func_02008392();
void Func_020083a0();
void Func_020083aa_a();
void Func_020083aa_b();
void Func_020083b8();
void Func_020083be();
void Func_020083d0();
void Func_020083d8();
void Func_020083e4();
void Func_020083ea();
void Func_020083f0();
void Func_02008404();
void Func_02008410();
void Func_02008416();
void Func_0200841c();
void Func_0200842a();
void Func_0200842e();
void Func_02008440();
void Func_02008442();
void Func_02008448_a();
void Func_02008448_b();
void Func_0200845c();
void Func_02008462();
void Func_02008468();
void Func_0200847c();
void Func_020084a0();
void Func_020084e4();
void Func_02008516();
void Func_0200852c();
void Func_0200852e();
void Func_0200853a();
void Func_0200854e();
void Func_02008560();
void Func_02008568();
void Func_0200856c();
void Func_02008578();
void Func_0200857e();
void Func_02008584();
void Func_02008588();
void Func_02008596();
void Func_0200859a();
void Func_0200859e();
void Func_020085ac();
void Func_020085b0();
void Func_020085b4();
void Func_020085c0();
void Func_020085c2();
void Func_020085cc();
void Func_020085d0_a();
void Func_020085d0_b();
void Func_020085e0();
void Func_020085f4();
void Func_020085fa();
void Func_02008600();
void Func_02008620_a();
void Func_02008620_b();
void Func_02008626();
void Func_02008630();
void Func_0200863c();
void Func_02008642();
void Func_0200864a();
s32 Func_0200864c();
void Func_02008664();
void Func_02008670();
void Func_02008684();
s32 Func_02008686();
void Func_0200869e();
void Func_020086a4();
void Func_020086ac();
void Func_020086b2();
void Func_020086bc();
void Func_020086c4();
void Func_020086c8();
void Func_020086ca();
void Func_020086e4();
void Func_020086ec();
void Func_020086f2();
void Func_020086f6();
void Func_020086fe();
void Func_02008706();
void Func_0200870a();
void Func_0200871a();
void Func_0200873e_a();
void Func_0200873e_b();
void Func_0200874e();
void Func_02008752();
void Func_0200875e();
void Func_02008778();
void Func_02008784();
void Func_02008794();
void Func_020087a8();
void Func_020087b8();
void Func_020087ba();
void Func_020087c0();
void Func_020087c6();
void Func_020087ea();
void Func_020087f4();
void Func_020087f8();
void Func_02008812();
void Func_02008814();
void Func_0200881c();
void Func_02008820();
void Func_02008822();
void Func_02008830();
void Func_02008836();
void Func_0200883a();
s32 Func_0200883c();
void Func_02008840();
void Func_02008846();
void Func_0200884e();
void Func_0200885a();
void Func_02008860();
void Func_0200886a();
void Func_0200886e();
void Func_02008872();
void Func_02008876();
void Func_0200887c();
void Func_02008882_a();
void Func_02008882_b();
void Func_02008886();
void Func_02008894();
void Func_020088a6();
void Func_020088b8();
void Func_020088c4();
void Func_02008bea(s32 arg0);
void Func_02008cb4(s32 arg0, s32 arg1);

/* Overlay-local import veneers, retained per call site. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/*
 * resource_380: the talk handler for actor 9, one of a bank of per-actor
 * published callbacks. The handler is published through a script record
 * rather than called, so nothing here reaches it by name.
 */

/*
 * resource_380: the talk handler for actor 5, published through a script
 * record rather than called.
 */

/*
 * Overlay resource_380: the per-actor talk handlers of the party
 * introduction scene.
 */

/*
 * Callback for resource_380 that forwards to Func_020072b4 and nothing else.
 * A script record holds its Thumb pointer; nothing calls it directly. The
 * twelve-byte owner includes the zero alignment halfword after the return and
 * has no literal pool. Func_020072b4 takes no arguments -- nothing is loaded
 * into r0 before the call.
 */

/*
 * resource_380 owner at 0x02004248, 24 bytes including the trailing
 * alignment halfword: a two-call convenience wrapper.  Both Func_ names are
 * the overlay's import-veneer entries for main-ROM code, not runtime
 * addresses.  The callees' roles beyond call shape remain open.
 */

/* One symbol per call site, named at that site's call word rather than at a
 * runtime address. A callee reached from several sites therefore appears
 * several times here under different names. */

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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001450(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020027f8(void (*f)(), s32 a0)
{
    f(a0);
}

struct ObjectRuntime;

void OverlayObject_SetHandleFlagBits2And3(s32 obj, u32 v)
{
    S *h = *(S **)(obj + 0x50);
    h->f1 = v;
}

void *OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void SceneEffect_UpdateMotion(union MotionWork *work)
{
    u16 *record;
    work->fields.x += work->fields.velocity_x;
    work->fields.y += work->fields.velocity_y;
    work->fields.z += work->fields.velocity_z;
    work->fields.accum_x += work->fields.rate_x;
    work->fields.accum_y += work->fields.rate_y;
    record = work->fields.record;
    record[15] += work->fields.angle_step;
}

void Overlay380_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                   u32 flags, const struct Options *options)
{
    u32 table_offset;
    struct Effect *party;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *mode_block;
    u32 option_bits;
    u16 *tag;
    s32 duration;
    s32 first_delta;
    s32 accumulated;

    party = GetPrimaryEffectRecord(0);
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = SpawnConfiguredEffectObject(options->kind, x, y, z);
    } else {
        effect = SpawnConfiguredEffectObject(222, x, y, z);
    }
    if (effect == 0)
        return;

    block = effect->sprite;
    mode_block = block;

    SetConfiguredEffectVariant(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    ApplyConfiguredEffectDescriptor(effect, ConfigurableEffectDescriptors[table_offset >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = party->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&block->flags9;
    flag_mask = ~12;
    block->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    if ((flags & 0xffff0000) == 0 || options == 0)
        return;

    if ((flags & 0x10000) != 0)
        SetConfiguredEffectLink(effect, options->link);

    if ((flags & 0x20000) != 0) {
        effect->flags23 &= 0xfe;
        option_bits = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)mode_block + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct Descriptor *descriptor =
            ConfigurableEffectDescriptors[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = ComputeConfiguredEffectRateFromDelta(first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = ComputeConfiguredEffectRateFromOffset(first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate34 = ComputeConfiguredEffectSecondRate(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        SetConfiguredEffectCallbackMode(effect, 1);
        ApplyConfiguredEffectCallbackArgument(effect, (struct Descriptor *)options->callback_arg);
    }

    if ((flags & 0x400000) != 0)
        block->angle = options->angle;
    if ((flags & 0x800000) != 0)
        *tag = options->step;
    if ((flags & 0x1000000) != 0)
        effect->callback = options->callback;
}

s32 UpdateOverlayObjectAngle(struct OverlayObject *object)
{
    struct OverlayObject *linked_object = object->linked_object;
    if (linked_object != NULL) {
        s32 angle_delta;
        u16 angle;
        object->unknown_5a = object->unknown_5a & 0xFE;
        angle_delta = CalculateAngleFromCoordinateDelta(
            linked_object->coordinate_10 - object->coordinate_10,
            linked_object->coordinate_08 - object->coordinate_08);
        angle = object->angle;
        angle_delta -= angle;
        angle_delta <<= 16;
        angle_delta >>= 16;
        if (angle_delta != 0) {
            if (angle_delta > 0x1000) {
                angle_delta = 0x1000;
            }
            if (angle_delta < -0x1000) {
                angle_delta = -0x1000;
            }
            object->angle = angle + angle_delta;
        }
    }
    return 1;
}

s32 *SceneData_GetTableCd88(void)
{
    return Data_0200cd88;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 *SceneData_GetTableCDB8(void)
{
    return Data_0200cdb8;
}

s32 *SceneData_GetTableCdc4(void)
{
    return Data_0200cdc4;
}

s32 *SceneData_GetTableCFA4(void)
{
    return Data_0200cfa4;
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

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    Call3(Func_02005ed8, 5, 0x6000, 0);
    Call3(Func_02005ee4, 9, 0x5000, 0);
    Call3(Func_02005ef0, 10, 0x8000, 0);
    Call3(Func_02005efc, 12, 0x5000, 40);
    Call3(Func_02005f08_a, 11, 0xd000, 40);
    Func_02005ec0(11, 2);
    Func_020056e4(11, 20);
    Func_02005ed0(12, 2);
    Call2(Func_02005f42, 12, 0x102);
    Func_02005e48(60);
    Func_02005704(12, 10);
    Func_02005efa(10, 12, 0);
    Func_02005f04(5, 12, 0);
    Func_02005f0e(9, 12, 0);
    Func_02005e74(40);
    Func_02005f14(10, 1);
    Call3(Func_02005f70, 10, 0x8000, 10);
    Func_02005f08_b(10, 3);
    Func_0200574c(10, 10);
    Func_02005f8a(11, 0, 10);
    Func_02005f2a_a(11, 3);
    Func_02005eb0(10);
    Func_02005fa2(5, 0, 0);
    Call3(Func_02005fae, 9, 0x8000, 20);
    Func_02005f5e(5, 2);
    Func_02005f6e(9, 2);
    Func_02005edc(10);
    Call3(Func_02005fd0, 5, 0xc000, 0);
    Call3(Func_02005fdc, 9, 0xb000, 10);
    Call3(Func_02005fe8, 10, 0xb000, 40);
    Func_02005f88(10, 3);
    Func_020057c4(10, 10);
    Call3(Func_02006014, 5, 0x100, 20);
    Call2(Func_02006026, 12, 0x102);
    Func_02005fc6(12, 3);
    Func_02005f34(40);
    Call3(Func_02005fb2, 1, 0x15a0000, 0x1b80000);
    Func_02005e18(1);
    Func_02006020_a(1, 0);
    Call3(Func_02006044, 0, 0x2000, 0);
    Call3(Func_02006050, 1, 0xa000, 20);
    Func_020060c4();
    Func_020060d0();
    Call3(Func_02005fee, 1, 0x1180000, 0x1d60000);
    Call4(Func_020060a6, 0x1050000, -1, 0x1d20000, 0);
    Func_02005ed2();
    Func_02005e68(1);
    Func_020060ec();
    Func_02006100();
    Func_02005fa6(20);
    Call3(Func_0200609a, 0, 0x3000, 10);
    Call3(Func_020060b6_a, 1, 0x102, 0);
    Func_0200605e(1, 3);
    Func_020060ae(1, 0);
    Func_02005fd4(10);
    Func_0200605c(1, 4);
    Func_02005fe2(10);
    Func_020060ca(1, 0);
    Func_02005ff0(10);
    Call1_02001450(Func_020060b6_b, 0x109b);
    Func_020060ce(11, 0);
    Call3(Func_020060f2, 11, 0xd000, 10);
    Func_020060a2_a(0, 2);
    Func_020060b2(0, 2);
    Func_02006020_b(10);
    Call3(Func_02006114, 0, 0xe000, 0);
    Call3(Func_02006120, 1, 0xe000, 20);
    Func_02006194();
    Func_020061a0();
    Call4(Func_02006168_a, 0x1dd0000, -1, 0x14e0000, 0);
    Func_02005f94();
    Func_02005f2a_b(1);
    Func_020061ae();
    Func_020061c2_a();
    Func_02006068(20);
    Func_02006108(9, 2);
    Func_02006076(10);
    Call3(Func_0200616a, 9, 0x8000, 30);
    Func_0200593e_a(9, 20);
    Func_0200617c(5, 0, 40);
    Func_0200611c(5, 4);
    Func_020060a2_b(20);
    Func_0200595e(5, 10);
    Func_0200614a(12, 2);
    Func_020060b8(80);
    Func_02005974(12, 20);
    Func_02006160(5, 2);
    Call3(Func_020061bc, 5, 0xc000, 0);
    Call3(Func_020061c8, 9, 0xb000, 20);
    Func_02006168_b(12, 3);
    Func_020059a4(12, 20);
    Func_02006188(5, 2);
    Func_02006198_a(9, 2);
    Call3(Func_020061f4_a, 12, 0xb000, 40);
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

/*
 * A scripted-scene bracket around a two-level story-flag test. When flag
 * 0x83e is set the actor speaks one line and the handler ends; otherwise flag
 * 0x83c chooses between two alternative lines and the longer path also plays
 * a motion, waits ten frames and closes. The two tests read with opposite
 * polarity. The trailing zero arguments have no established meaning. The
 * 108-byte owner includes its five-word literal pool.
 */
void FieldScene_RunActorNineDialogueByFlags(void)
{
    Func_02007014();
    if (Func_02006fe2(0x83e)) {
        Func_020070dc(0x10cb);
        Func_020070f4(9, 0);
    } else {
        if (Func_02006ffc(0x83c) == 0)
            Func_020070f6(0x1079);
        else
            Func_020070fe_a(0x107b);
        Func_020070f0(9, 0, 0);
        Func_0200704e(10);
        Func_02007126_a(9, 0);
    }
    Func_0200706a();
}

/*
 * A scripted-scene bracket around a two-level story-flag test. Flag 0x83e
 * selects a single line; otherwise flag 0x83c chooses between two lines and
 * the longer path plays a motion, waits ten frames and closes. The two tests
 * read with opposite polarity. The trailing zero arguments have no
 * established meaning. The 108-byte owner includes its five-word pool.
 */
void SceneDialogue_RunActorFiveTalk(void)
{
    Func_02007080();
    if (Func_0200704e_a(0x83e)) {
        Func_02007148(0x10c9);
        Func_02007160(5, 0);
    } else {
        if (Func_02007068(0x83c) == 0)
            Func_02007162(0x107a);
        else
            Func_0200716a(0x107c);
        Func_0200715c(5, 0, 0);
        Func_020070ba(10);
        Func_02007192(5, 0);
    }
    Func_020070d6();
}

/*
 * Talk handler for actor 10: the scripted-scene bracket around one line of
 * dialogue, with no branch and no state. It is published into a script record
 * rather than called from the image.
 *
 * The thirty-two-byte owner includes its one pool word, which carries the
 * line id, and the zero alignment halfword after it. The second argument of
 * the tail call is zero at every site in this bank and its role is not
 * established.
 */
void SceneDialogue_RunActorTenDialogue(void)
{
extern void Func_020070ec(void);

    Func_020070ec();
    Func_020071aa(0x10ca);
    Func_020071c2(10, 0);
    Func_02007106();
}

/*
 * Talk handler for actor 11, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void SceneDialogue_RunActor11TalkLine(void)
{
extern void Func_0200710c(void);

    Func_0200710c();
    Func_020071ca(0x10c7);
    Func_020071e2(11, 0);
    Func_02007126_b();
}

/*
 * Talk handler for actor 13, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void SceneDialogue_ShowLine10C8(void)
{
extern void Func_0200712c(void);

    Func_0200712c();
    Func_020071ea(0x10c8);
    Func_02007202(13, 0);
    Func_02007146();
}

/*
 * Talk handler for actor 14, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void SceneDialogue_RunActor14TalkLine(void)
{
extern void Func_0200714c(void);

    Func_0200714c();
    Func_0200720a(0x10cc);
    Func_02007222(14, 0);
    Func_02007166();
}

/*
 * Same bracket and line call as the talk handlers, but the tail pairs actors
 * 9 and 10 through the overlay's own call.
 */
void SceneDialogue_RunLine1072WithPair9And10(void)
{
    Func_0200716c();
    Func_0200722a(0x1072);
    Func_02006a26(9, 10);
    Func_02007186();
}

void FieldScene_Forward72b4(void)
{
    Func_020072b4();
}

void Lifted_020027f8(void)
{
    u16 i;
    u8 *p10;
    u8 *p10b;
    u8 *p9;
    u8 *p9b;
    u8 *p9c;
    u8 *rec;
    u8 *rec2;
    u8 *record;
    u8 *work;
    s32 base5_4009;
    s32 v6;
    s32 base5_8009;
    s32 zero;

    Func_020071a6();
    Call6(Func_02007112, 27, 16, 5, 1, 27, 17);
    Call4(Func_020072d8, 0x1d70000, -1, 0x1050000, 0);
    Func_020072e4();
    Func_02007108();
    rec2 = (u8 *)Func_020071f6(8);
    *(s32 *)(rec2 + 24) = 0x1999;
    *(s32 *)(rec2 + 28) = 0x1999;
    rec = (u8 *)Func_02007204(0);
    p9 = *(u8 **)(rec + 80) + 38;
    *p9 = 0;
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    Call2(Func_020072a6, 0, 0x100);
    Call3(Func_02007272, 0, 0x1d70000, 0x1220000);
    p10 = rec + 85;
    *p10 = 0;
    *(s32 *)(rec + 12) = 0x280000;
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(((s32)work + 0x1c0)) = 0x203;
    *(s32 *)(((s32)work + 0x1c8)) = 32;
    Func_0200737a();
    Func_0200738e_a();
    Func_02007234(20);
    Call3(Func_020072b0, 8, 0x1d70000, 0x1220000);
    Func_020073c6(190);
    Func_0200733e(0, 2);
    for (i = 0; i != 90; i++) {
        *(s32 *)(rec + 12) += -0x1999;
        *(s32 *)(rec + 24) += 0x28f;
        *(s32 *)(rec + 28) += 0x28f;
        *(s32 *)(rec2 + 24) += 0x28f;
        *(s32 *)(rec2 + 28) += 0x28f;
        Func_0200727a(1);
    }
    *p10 = 5;
    Func_02007292(80);
    Call2(Func_020073aa_a, 0x4ccc, 0x999);
    Call4(Func_020073c2, 0x1d70000, -1, 0x1220000, 1);
    for (i = 0; i != 60; i++) {
        *(s32 *)(rec + 12) += -0x8000;
        Func_020072ba(1);
    }
    *p10 = 3;
    Func_020072d2(20);
    Func_020073ca(0, 1);
    {
        u8 *record = Func_02007308(0);
        s32 flags = 1 | record[35];
        record[35] = flags;
    }
    Func_020073a2(0, 0);
    {
        s32 shown = 1;

        *p9 = shown;
    }
    ((void (*)())Func_02007372_a)(8, 0, 0);
    Func_02007426();
    Func_0200730c(20);
    Func_0200741c(0, 1);
    Call3(Func_02007352, 0, 0x8000, 0x4000);
    Call3(Func_0200738e_b, 0, 0x1d7, 0x136);
    Func_02007334(20);
    Func_020073d4(0, 2);
    Func_02007342(20);
    zero = 0;
    record = (u8 *)Func_02007372_b(0);
    if ((s32)record != 0) {
        Func_020073c8(5, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    record = (u8 *)Func_02007386(0);
    if ((s32)record != 0) {
        Func_020073dc(1, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    Call3(Func_020073aa_b, 1, 0x8000, 0x4000);
    Call3(Func_020073b8, 5, 0x8000, 0x4000);
    Call3(Func_020073ec, 5, 0x1c5, 0x12e);
    Call3(Func_02007400, 1, 0x1e9, 0x12e);
    Func_02007420_a(5, 1);
    Call3(Func_0200749c, 1, 0x3000, 0);
    Call3(Func_020074a8, 5, 0x6000, 10);
    Call3(Func_020074c4, 0, 0x100, 0);
    Call3(Func_020074d0, 5, 0x100, 0);
    Call3(Func_020074dc, 1, 0x100, 0);
    Func_02007476(0, 2, 0);
    Func_02007480(5, 2, 0);
    Func_0200748a(1, 2, 40);
    Func_0200749a(0, 3);
    Func_020074a2(5, 3);
    Func_020074b2(1, 3);
    Func_02007420_b(40);
    Call2(Func_0200752a, 0, 0x102);
    Call2(Func_02007534, 5, 0x102);
    Call2(Func_0200753e, 1, 0x102);
    Func_02007444(60);
    Call3(Func_0200747e, 0, 0x13333, 0x9999);
    Call3(Func_02007488, 5, 0x13333, 0x9999);
    Call3(Func_02007492, 1, 0x13333, 0x9999);
    Call3(Func_020074c6, 0, 0x1d7, 0x15a);
    Call3(Func_020074d2, 5, 0x1af, 0x152);
    Call3(Func_020074de, 1, 0x1ff, 0x152);
    Func_020074f4_a(0);
    Func_0200750c(0, 1);
    Func_02007502(5);
    Func_0200751a(5, 1);
    Func_02007510(1);
    Func_02007528(1, 1);
    Call3(Func_020074ea, 0, 0x4ccc, 0x2666);
    Call3(Func_020074f4_b, 5, 0x4ccc, 0x2666);
    Call3(Func_020074fe, 1, 0x4ccc, 0x2666);
    Call3(Func_020075c2, 5, 0xc000, 0);
    Call3(Func_020075ce, 1, 0xc000, 0);
    Call3(Func_020075da, 0, 0xc000, 60);
    Call3(Func_020075e6, 5, 0x8000, 0);
    Func_020075f0(1, 0, 0);
    Call3(Func_020075fc, 0, 0x4000, 60);
    Func_02007606(5, 0, 0);
    Call3(Func_02007612, 1, 0x8000, 40);
    Func_0200761c(0, 0, 40);
    Func_020075bc(0, 3);
    Func_020075dc(1, 2);
    Func_0200754a(20);
    Func_020075d2(1, 4);
    Func_02007558(20);
    Call3(Func_0200764c, 0, 0x8000, 40);
    Func_020075ec(0, 3);
    Func_0200760c(5, 2);
    Func_0200757a(20);
    Call3(Func_0200767c, 5, 0x101, 60);
    Call3(Func_02007678, 0, 0x4000, 0);
    Call3(Func_02007684, 1, 0x3000, 0);
    Call3(Func_020076dc, 5, 0x6000, 60);
    rec = (u8 *)Func_02007622(9);
    p9b = *(u8 **)(rec + 80) + 38;
    *p9b = zero;
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    *(s32 *)(rec2 + 24) = 0x1999;
    *(s32 *)(rec2 + 28) = 0x1999;
    Call2(Func_020076ca, 9, 0x100);
    Call3(Func_02007696, 9, 0x1d70000, 0x1220000);
    p10b = rec + 85;
    *p10b = zero;
    *(s32 *)(rec + 12) = 0x280000;
    Func_0200763a(1);
    Call1_020027f8(Func_02007700, 0x103c);
    Func_02007718(9, 0);
    Func_020076da(0, 4, 0);
    Func_020076e4(5, 4, 0);
    Func_020076ee_a(1, 4, 40);
    Call3(Func_0200775a, 0, 0xc000, 0);
    Call3((void (*)())Func_02007766_a, 5, 0xe000, 0);
    Call3(Func_02007772, 1, 0xb000, 0);
    Call2(Func_020077a6_a, 0x20000, 0x4000);
    Call4(Func_020077bc, 0x1d70000, -1, 0x1350000, 1);
    Func_020077c8();
    Call3(Func_02007724, 8, 0x1d70000, 0x1220000);
    Func_0200783a(190);
    Func_020077b2(9, 2);
    for (i = 0; i != 90; i++) {
        *(s32 *)(rec + 12) += -0x1999;
        *(s32 *)(rec + 24) += 0x28f;
        *(s32 *)(rec + 28) += 0x28f;
        *(s32 *)(rec2 + 24) += 0x28f;
        *(s32 *)(rec2 + 28) += 0x28f;
        Func_020076ee_b(1);
    }
    *p10b = 5;
    Func_02007706(80);
    for (i = 0; i != 60; i++) {
        *(s32 *)(rec + 12) += -0x8000;
        Func_02007716(1);
    }
    *p10b = 3;
    Func_02007730(30);
    Func_02007828(9, 1);
    {
        u8 *record = Func_02007766_b(9);
        s32 flags = 1 | record[35];
        record[35] = flags;
    }
    Func_02007800(9, 0);
    {
        s32 shown = 1;

        *p9b = shown;
    }
    Func_020077d0(8, 0, 0);
    Call1_020027f8((void (*)())Func_02007766_c, 30);
    Call3(Func_020077a0, 9, 0x13333, 0x9999);
    Call3(Func_020077dc, 9, 0x1d7, 0x132);
    Func_02007782(20);
    Call3(Func_02007886, 9, 0x100, 0);
    Func_02007820(9, 2, 80);
    Func_02007838(9, 3);
    Func_020077a6_b(30);
    Call3(Func_0200789a, 9, 0x8000, 30);
    Func_02007852(9, 2);
    Func_020077c0(20);
    Func_020078b2(9, 0, 30);
    Func_0200786a_a(9, 2);
    Func_020077d8(20);
    Call3(Func_020078cc, 9, 0x4000, 30);
    Func_02007884(9, 3);
    Func_020077f2(30);
    Func_020070ae(9, 20);
    Call3(Func_02007834_a, 9, 0x26666, 0x13333);
    Call3(Func_02007870, 9, 0x1a7, 0x132);
    Call3(Func_02007904, 9, 0xb000, 0);
    Call3(Func_02007910, 0, 0xb000, 0);
    Call3(Func_0200791c, 5, 0xa000, 0);
    Call3(Func_02007928, 1, 0xb000, 30);
    Func_020078e0(9, 2);
    Func_0200784e(30);
    Func_020078d6(9, 3);
    Func_0200785c(40);
    Call3(Func_020078c8, 9, 0x207, 0x132);
    Call3(Func_0200795c, 9, 0xd000, 0);
    Call3(Func_02007968, 0, 0xd000, 0);
    Call3(Func_02007974, 5, 0xe000, 0);
    Call3(Func_02007980, 1, 0xd000, 20);
    Func_02007938_a(9, 2);
    Func_020078a6(30);
    base5_4009 = (s32)Data_00004009;
    Func_02007930(9, 4);
    Func_020078b6(40);
    Func_02007172(base5_4009, 30);
    Func_02007970(0, 5, 0);
    Func_020078ce(40);
    Func_02007966(0, 2);
    Func_02007976(5, 2);
    Call3(Func_020079e0, 1, 0x101, 40);
    Func_020071a4(1, 40);
    Call3(Func_020079e4, 9, 0x5000, 20);
    Func_020071b8(base5_4009, 20);
    Call3(Func_020079f8, 0, 0xd000, 0);
    Call3(Func_02007a04, 5, 0xe000, 20);
    Call3(Func_02007a1e, 0, 0x101, 0);
    Call3(Func_02007a28, 5, 0x101, 40);
    Func_020079b8(9, 4);
    Func_020071f4(base5_4009, 10);
    Func_020079c0(1, 3);
    Func_020079c8(5, 3);
    Func_020079d8(0, 3);
    Func_0200795e(20);
    Func_020079e6(9, 3);
    Func_0200796c(40);
    Call2(Func_02007a88, 0x8000, 0x1000);
    Call4(Func_02007a9e_a, 0x2150000, -1, 0x1530000, 1);
    Call3(Func_020079c4_a, 9, 0x8000, 0x4000);
    Call3(Func_020079fe, 9, 0x215, 0x153);
    Func_02007ac2();
    Func_02007a94(0, 0, 0);
    Func_02007a9e_b(5, 0, 0);
    Func_02007aa8(1, 0, 20);
    Call3(Func_02007ab4_a, 9, 0xd000, 40);
    Func_02007abe(9, 0, 40);
    Call3(Func_02007aca, 9, 0x3000, 60);
    Func_02007a82(9, 3);
    Func_020079f0(30);
    Func_02007b4e(9, 1);
    Call3(Func_02007a80, 9, 0x19999, 0xcccc);
    Call3(Func_02007ab4_b, 9, 0x1c7, 0x168);
    Func_02007a62(20);
    Call3(Func_02007b56, 0, 0x4000, 0);
    Call3(Func_02007b62, 5, 0x2000, 0);
    Call3(Func_02007b6e, 1, 0x5000, 0);
    Call3(Func_02007af2, 9, 0x1c7, 0x168);
    Func_02007a98(20);
    Call3(Func_02007b8c, 9, 0x5000, 30);
    Func_02007b44(9, 3);
    Func_02007ab2(30);
    Call3(Func_02007b1e, 9, 0x1d7, 0x168);
    Func_02007ac4(20);
    Call3(Func_02007bb8, 9, 0x3000, 30);
    Call3(Func_02007bd4, 9, 0x100, 0);
    Func_02007b7c(9, 2);
    Func_02007aea(30);
    Func_020073a6(9, 30);
    Func_02007b92(9, 3);
    Func_02007b00(20);
    Func_020073bc(9, 30);
    Call3(Func_02007b42, 5, 0xcccc, 0x6666);
    Call3(Func_02007b80, 5, 0x1b8, 0x15a);
    Func_02007b26(10);
    Call3(Func_02007c1a, 5, 0x2000, 10);
    Func_020073ee(5, 20);
    Call3(Func_02007b74, 1, 0xcccc, 0x6666);
    Call3(Func_02007bb0, 1, 0x1ef, 0x15a);
    Func_02007c74();
    Func_02007b5a(10);
    Call3(Func_02007c4e, 1, 0x6000, 10);
    Value2(Func_02007c36, 1, 0);
    Func_02007c60(5, 0, 0);
    Call3(Func_02007c6c, 1, 0x8000, 0);
    if (Value2(Func_02007bac, 0, 0) == 1) {
        bump_step(1);
    }
    Call3(Func_02007c94, 9, 0xc000, 30);
    Func_02007468(9, 30);
    Call1_020027f8(Func_02007c7a, 0x1048);
    Call3(Func_02007cae_a, 5, 0x2000, 0);
    Call3(Func_02007cba_a, 1, 0x6000, 30);
    Func_02007c72(9, 2);
    Func_02007496(9, 10);
    Func_02007c6a(9, 3);
    Func_020074a6(9, 20);
    Func_02007c92(9, 2);
    Func_020074b6(9, 10);
    Call2(Func_02007d0c, 9, 0x102);
    Func_02007c12(40);
    Func_020074ce(9, 10);
    Call3(Func_02007d0e, 9, 0x2000, 20);
    Func_02007cbe(0, 2);
    Func_02007cc6(5, 2);
    Func_02007cd6(1, 2);
    Func_02007c44(10);
    Call3(Func_02007d38, 0, 0x2000, 0);
    Call3(Func_02007d44, 5, 0x2000, 0);
    Call3(Func_02007d50, 1, 0x2000, 20);
    Func_02007cae_b(9);
    Func_02007cb4(0);
    Func_02007cba_b(5);
    Func_02007cc0(1);
    Call2(Func_02007d9c, 0x80000, 0x10000);
    Call4(Func_02007db4_a, 0x2c70000, -1, 0x1d00000, 1);
    Func_02007dc0();
    Call3(Func_02007d1a, 9, 0x24d0000, 0x1610000);
    Func_02007cb0(40);
    Call2(Func_02007d88, 0x1009, 0);
    Call3(Func_02007d34, 9, 0x1d70000, 0x1680000);
    Func_02007cca(80);
    Call4(Func_02007df2_a, 0x1d70000, -1, 0x1720000, 1);
    Func_02007dfe_a();
    Func_02007ce4(10);
    Func_02007d84(9, 3);
    Func_02007cf2(10);
    Call3(Func_02007de6, 9, 0xd000, 20);
    Call3(Func_02007df2_b, 0, 0xe000, 0);
    Call3(Func_02007dfe_b, 5, 0xe000, 0);
    Call3(Func_02007e0a, 1, 0xe000, 20);
    Call4(Func_02007e4a, 0x2c70000, -1, 0x930000, 1);
    Func_02007e56();
    Call3(Func_02007db4_b, 9, 0x2540000, 0xee0000);
    Func_02007d4a(40);
    Call2(Func_02007e22_a, 0x1009, 0);
    Call3(Func_02007dce, 9, 0x1d70000, 0x1680000);
    Func_02007d64(80);
    Call4(Func_02007e8c_a, 0x1d70000, -1, 0x1720000, 1);
    Func_02007e98();
    Func_02007d7e(10);
    Func_02007e1e_a(9, 2);
    Func_02007d8c(10);
    Func_02007e1e_b(9, 4, 0);
    Call3(Func_02007e8a, 9, 0xb000, 20);
    Func_02007e3a(0, 1);
    Func_02007e42(5, 1);
    Func_02007e52(1, 1);
    Call3(Func_02007eae, 0, 0xa000, 0);
    Call3(Func_02007eba, 5, 0xa000, 0);
    Call3(Func_02007ec6, 1, 0xa000, 20);
    Call4(Func_02007f08, 0xe70000, -1, 0x930000, 1);
    Func_02007f14();
    Call3(Func_02007e72, 9, 0x1340000, 0xfa0000);
    Func_02007e08(40);
    Call2(Func_02007ee0, 0x2009, 0);
    Call3(Func_02007e8c_b, 9, 0x1d70000, 0x1680000);
    Func_02007e22_b(80);
    Call4(Func_02007f4a_a, 0x1d70000, -1, 0x1720000, 1);
    Func_02007f56();
    Func_02007e76(10);
    Func_02007f16_a(9, 3);
    Func_02007e84(10);
    Func_02007f16_b(9, 6, 0);
    Call3(Func_02007f82, 9, 0x5000, 20);
    Func_02007f32(0, 2);
    Func_02007f3a(5, 2);
    Func_02007f4a_b(1, 2);
    Call3(Func_02007fa6, 0, 0x6000, 0);
    Call3(Func_02007fb2, 5, 0x6000, 0);
    Call3(Func_02007fbe, 1, 0x6000, 20);
    Call4(Func_02008000, 0xe70000, -1, 0x1d00000, 1);
    Func_0200800c();
    Call3(Func_02007f6a, 9, 0x1320000, 0x16a0000);
    Func_02007f00(40);
    Call2(Func_02007fd8, 0x2009, 0);
    Call3(Func_02007f84, 9, 0x1d70000, 0x1680000);
    Func_02007f1a(80);
    Call4(Func_02008042, 0x1d70000, -1, 0x1720000, 1);
    Func_0200804e();
    Func_02007f34(30);
    Call3(Func_02008038, 9, 0x104, 60);
    Call3(Func_02008034, 0, 0x4000, 0);
    Call3(Func_02008040, 5, 0x2000, 0);
    Call3(Func_0200804c, 1, 0x6000, 40);
    Call3(Func_02008068, 5, 0x102, 40);
    Func_02008010(5, 1);
    Func_02007834_b(5, 20);
    Func_02008012_a(9, 4, 0);
    Call3(Func_0200807e, 9, 0xb000, 20);
    Func_02008036_a(9, 2);
    Call2(Func_0200785a, 0xa009, 10);
    Func_0200803e(9, 3);
    Call2(Func_0200786a_b, 0xa009, 20);
    Call3(Func_020080b8_a, 0, 0x101, 0);
    Call3(Func_020080c2, 5, 0x101, 0);
    Call3(Func_020080cc, 1, 0x101, 60);
    Call3(Func_020080c8, 9, 0xc000, 20);
    Func_02008060(9, 4);
    Value2(Func_020080b8_b, 0x8009, 0);
    if (Value2(Func_02008018, 0, 0) == 1) {
        bump_step(1);
    }
    Func_02008012_b(20);
    Call3(Func_02008106, 1, 0x8000, 10);
    Func_020078da(1, 10);
    Call1_020027f8(Func_020080ec, 0x1056);
    Call3(Func_02008120, 1, 0x6000, 20);
    Func_020080ca(9, 4, 40);
    Func_02007902(0x8009, 10);
    Call3(Func_02008150, 0, 0x101, 0);
    Call3(Func_0200815a, 5, 0x101, 0);
    Call3(Func_02008164, 1, 0x101, 80);
    Call2(Func_02008176_a, 9, 0x102);
    Func_0200807c(40);
    Func_02007938_b(0x8009, 40);
    Call3(Func_02008188, 0, 0x106, 0);
    Call3(Func_02008194, 5, 0x106, 0);
    Call3(Func_020081a0, 1, 0x106, 60);
    Func_0200819a(5, 0, 0);
    Call3(Func_020081a6, 1, 0x8000, 20);
    Value2(Func_0200818e, 0x8009, 0);
    if (Value2(Func_020080ee, 0, 0) == 1) {
        bump_step(1);
    }
    Func_02008182(9, 1);
    Func_020080f0(20);
    Call3(Func_020081e4_a, 5, 0x2000, 0);
    Call3(Func_020081f0, 1, 0x5000, 20);
    Func_020079c4_b(0x8009, 40);
    Call1_020027f8(Func_020081d6, 0x105b);
    Func_02008196(5, 4);
    Func_020079da(5, 10);
    Call3(Func_0200821a, 9, 0xb000, 10);
    Func_020081d2(9, 1);
    Func_020081cc(9, 4, 40);
    Call2(Func_02007a00, 0xa009, 10);
    Func_020081e4_b(0, 1);
    Func_020081ec(5, 1);
    Func_020081fc(1, 1);
    Func_0200816a(40);
    Call3(Func_0200825e, 9, 0xc000, 10);
    Func_02008216(9, 1);
    Func_02007a3a(0x8009, 40);
    Call3(Func_02008288, 0, 0x105, 0);
    Call3(Func_02008292, 5, 0x105, 0);
    Call3(Func_0200829c, 1, 0x105, 120);
    Call3(Func_020082a6, 1, 0x107, 0);
    Func_02008240(1, 4, 40);
    Func_02007a74(1, 10);
    Func_02008260(9, 1);
    Func_020081ce(40);
    Call3(Func_020082c2, 9, 0x4000, 80);
    Func_02007a96(0x8009, 10);
    Call3(Func_020082d6, 1, 0x8000, 0);
    Func_020082e0(0, 0, 30);
    Func_02008290_a(0, 2);
    Func_020082a0(1, 2);
    Func_0200820e(20);
    Func_02008300(5, 0, 0);
    Call3(Func_0200830c_a, 0, 0x8000, 30);
    Func_020082bc(0, 2);
    Func_020082cc(5, 2);
    Func_0200823a(20);
    Call3(Func_0200832e, 0, 0x4000, 0);
    Call3(Func_0200833a, 5, 0x2000, 0);
    Call3(Func_02008346, 1, 0x5000, 40);
    Call2(Func_02008368, 0, 0x102);
    Call2(Func_02008372, 5, 0x102);
    Call2(Func_0200837c, 1, 0x102);
    Func_02008282(80);
    Func_0200830a(9, 3);
    Func_02008290_b(20);
    Func_02007b4c(0x8009, 40);
    rec = (u8 *)Func_020082c6(9);
    v6 = 192;
    Func_02008176_b(6);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Func_02008440(153);
    *(s32 *)(rec + 40) = (v6 << 11);
    Call3(Func_0200831c, 9, 0x1d7, 0x18b);
    Func_020081a2(6);
    Call3(Func_0200830c_b, 9, 0x4ccc, 0x2666);
    p9c = rec + 90;
    *p9c &= 254;
    Call3(Func_0200837a, 9, 0x1d9, 0x18b);
    Func_020083a0(9);
    Func_020083d8(9, 2);
    Call3(Func_02008392, 9, 0x1d5, 0x18b);
    Func_020083b8(9);
    Func_020083f0(9, 2);
    Call3(Func_020083aa_a, 9, 0x1d7, 0x18b);
    Func_020083d0(9);
    Func_02007c24(9, 10);
    Call3(Func_020083aa_b, 9, 0xcccc, 0x6666);
    Call3(Func_020083e4, 9, 0x1d7, 0x19b);
    Call3(Func_020083be, 9, 0x4ccc, 0x2666);
    *p9c &= 254;
    Call3(Func_020083ea, 9, 0x1da, 0x19b);
    Func_02008410(9);
    Func_02008448_a(9, 3);
    Call3(Func_02008404, 9, 0x1d4, 0x19b);
    Func_0200842a(9);
    Func_02008462(9, 3);
    Call3(Func_0200841c, 9, 0x1d7, 0x19b);
    Func_02008442(9);
    Call2(Func_020084e4, 9, 0x102);
    Func_0200847c(9, 3);
    Func_02007ca8(9, 10);
    Call3(Func_0200842e, 9, 0x3333, 0x1999);
    Call3(Func_02008468, 9, 0x1d7, 0x18b);
    Func_02008448_b(9, 1);
    Func_02008416(30);
    {
        s32 flags = 1 | *p9c;
        *p9c = flags;
    }
    Call3(Func_02008516, 9, 0xc000, 60);
    Func_02008304(6);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Func_020085c2(153);
    *(s32 *)(rec + 40) = (v6 << 11);
    Call3(Func_020084a0, 9, 0x1d7, 0x168);
    Func_02008326(6);
    Func_0200845c(40);
    Call3(Func_02008560, 9, 0x102, 80);
    Call3(Func_0200856c, 0, 0x102, 0);
    Call3(Func_02008578, 5, 0x102, 0);
    Call3(Func_02008584, 1, 0x102, 80);
    Func_0200852c(5, 1);
    Call3(Func_02008596, 5, 0x107, 40);
    Func_02007d5a(5, 20);
    Call3(Func_0200859a, 9, 0xb000, 40);
    Func_0200853a(9, 4);
    Call2(Func_02007d76, 0xa009, 30);
    Func_020085b4(5, 0, 30);
    Call3(Func_020085d0_a, 5, 0x106, 60);
    Call3(Func_020085cc, 5, 0x2000, 30);
    Func_02007da0(5, 20);
    Func_0200857e(0, 4, 0);
    Func_02008588(1, 4, 0);
    Call3(Func_020085f4, 0, 0x8000, 0);
    Call3(Func_02008600, 1, 0x8000, 40);
    Func_020085b0(0, 1);
    Func_020085c0(1, 1);
    Func_0200852e(40);
    Func_02008620_a(0, 0, 30);
    Func_020085d0_b(0, 2);
    Func_020085e0(1, 2);
    Func_0200854e(30);
    Call3(Func_02008642, 9, 0xd000, 30);
    Func_020085fa(9, 1);
    Func_02008568(20);
    Func_02007e24(0x8009, 20);
    Call3(Func_02008664, 0, 0x4000, 0);
    Call3(Func_02008670, 1, 0x5000, 30);
    Func_02008620_b(0, 2);
    Func_02008630(1, 2);
    Func_0200859e(20);
    Func_02008626(5, 3);
    Func_020085ac(20);
    Func_0200869e(5, 0, 30);
    Value2(Func_02008686, 5, 0);
    Call3(Func_020086b2, 0, 0x8000, 0);
    while (Value2(Func_0200864c, 0, 0) != 0) {
        Call1_020027f8(Func_020086c8, 0x1068);
        Call3(Func_0200870a, 5, 0x107, 0);
        Func_020086a4(5, 4, 60);
        Func_020086ec(5, 0);
    }
    Call1_020027f8(Func_020086f6, 0x1069);
    Func_0200863c(20);
    Func_020086c4(5, 3);
    Func_0200864a(20);
    Call3(Func_0200873e_a, 5, 0x2000, 30);
    Func_02007f12(5, 10);
    Call3(Func_02008752, 0, 0x4000, 10);
    Call3(Func_0200875e, 9, 0xb000, 30);
    Func_020086fe(9, 3);
    Func_02008684(20);
    Call3(Func_02008778, 9, 0xc000, 30);
    Call3(Func_020086ca, 9, 0x3333, 0x1999);
    Call3(Func_02008706, 9, 0x1d7, 0x160);
    Func_020086ac(20);
    base5_8009 = (s32)Data_00008009;
    Func_0200874e(9, 2);
    Func_020086bc(20);
    Func_02007f78(base5_8009, 60);
    *p9c &= 254;
    Call3(Func_0200873e_b, 9, 0x1c8, 0x168);
    Func_020086e4(20);
    Func_02008784(5, 2);
    Func_020086f2(20);
    Call1_020027f8(Func_020087b8, 0x106d);
    Func_02007fb4(5, 30);
    Call3(Func_020087f4, 9, 0xb000, 30);
    Func_02008794(9, 3);
    Func_0200871a(20);
    Call3(Func_0200881c, 1, 0x101, 60);
    Func_02007fe0(1, 10);
    Call3(Func_02008820, 9, 0xd000, 20);
    Func_020087c0(9, 3);
    Func_02007ffc(base5_8009, 20);
    Func_0200883a(0, 0, 0);
    Call3(Func_02008846, 1, 0x8000, 40);
    Call3(Func_02008860, 0, 0x101, 0);
    Call3(Func_0200886a, 1, 0x101, 40);
    Func_02008812(9, 2);
    Func_02008036_b(base5_8009, 10);
    Call3(Func_02008876, 0, 0x4000, 0);
    Call3(Func_02008882_a, 1, 0x5000, 30);
    Func_02008822(9, 3);
    Func_020087a8(10);
    Func_02008064(base5_8009, 30);
    Func_02008830(1, 3);
    Func_02008840(0, 3);
    Func_020087c6(20);
    Func_020088b8(0, 0, 0);
    Call3(Func_020088c4, 1, 0x8000, 30);
    Func_0200887c(1, 2);
    Func_020087ea(30);
    Func_02008872(0, 3);
    Func_020087f8(10);
    Call3(Func_02008836, 1, 0x10000, 0x8000);
    Func_02008886(1, 2);
    record = Value1(Func_0200883c, 0);
    if ((s32)record != 0) {
        Func_0200886e(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Func_02008894(1);
    Func_020088a6(1, 0, 0);
    Call1_020027f8(Func_02008814, 0x83b);
    Func_0200885a(5);
    Func_020081da();
    Call6(Func_020087ba, 8, 0, 5, 1, 27, 17);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c8)) = 16;
    Call1_020027f8(Func_0200884e, 0x12f);
    Func_02008882_b();
}

void FieldScene_RunSplitPairSteps(s32 a, s32 b)
{
    Func_02008cb4(a, 0);
    Func_02008bea(b);
}

/*
 * Drain until room: save the s16 counter at scene workspace + 472, prime two
 * channels, then loop while fewer than four of thirty slots are free,
 * requesting more and passing on any event pair that is not -1. On exit it
 * flushes four times with id 224 and restores the saved counter. The 148-byte
 * owner includes its three-word literal pool. Callee roles are not established.
 */
void SceneState_PumpUntilSlotsFree(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s16 saved = *(s16 *)(work + 472);
    s32 first;
    s32 second;
    s32 cnt;

    Func_02008e52(0x53);
    Func_02008e4a(224, 3);
    Func_02008c6a(0x111b, 1);
    do {
        cnt = 30 - Func_02008cd0(0);
        cnt -= Func_02008cda(1);

        if (cnt <= 3) {
            Func_02008c88(0x111c, 1);
            if (Func_02008e80(&second, &first) != -1)
                Func_02008d08(second, first);
        }
    } while (cnt <= 3);
    Func_02008cd8(224);
    Func_02008cde(224);
    Func_02008ce4(224);
    Func_02008cea(224);
    *(s16 *)(work + 472) = saved;
}

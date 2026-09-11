#include "types.h"

#define InitializeStagedActorState Func_02006488
#define GetDescendingMotionStep Func_0200649a
#define GetAscendingMotionStep Func_020064b4
#define ResetStagedActorState Func_02000030
#define UpdateStagedActorVerticalPosition Func_02000058
#define StagedActor_SetAngleByKind Func_020000b0
#define StagedActor_AdvanceCounter98 Func_020000d8
#define StagedActor_CountdownUntilPositionUnset Func_020000fc
#define StagedActor_SetReadyState Func_020002f4
#define UpdateActorNineEffectMode Func_02000284
#define SceneData_SelectTableByWord224 Func_02000340
#define SceneData_ReturnZero Func_02000370
#define SceneData_GetTableEB94 Func_02000374
#define SceneData_SelectTableBySceneIndexAndFlags Func_0200054c
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_0200b64e, a0, a1, a2)
#define Battle_WaitMode0_1(args...) Func_0200b73a(args)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_0200b60c, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_0200b66c(args)
#define Motion_SetPositionAndCommit_1(a0, a1, a2) Call3(Func_0200b648, a0, a1, a2)
#define Motion_SetPositionAndCommit_2(a0, a1, a2) Call3(Func_0200b654, a0, a1, a2)
#define Motion_SetPositionAndCommit_3(a0, a1, a2) Call3(Func_0200b660, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_0200b698(args)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_0200b724, a0, a1, a2)
#define Scene_GetRecord_1(a0) Value1(Func_0200b642, a0)
#define Motion_SetHPosTerrain_2(args...) Func_0200b6b0(args)
#define Scene_GetRecord_2(a0) Value1(Func_0200b656, a0)
#define Motion_SetHPosTerrain_3(args...) Func_0200b6c4(args)
#define Scene_GetRecord_3(a0) Value1(Func_0200b66a, a0)
#define Motion_SetHPosTerrain_4(args...) Func_0200b6d8(args)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_0200b698_a, a0, a1, a2)
#define Motion_ResetPosMode2_1(a0, a1, a2) Call3(Func_0200b6d4, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_0200b6ae, a0, a1, a2)
#define Motion_ResetPosMode2_2(a0, a1, a2) Call3(Func_0200b6ea, a0, a1, a2)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_0200b6c8, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_0200b70c, a0, a1, a2)
#define Object_SetModeById_3(args...) Func_0200b734(args)
#define Object_SetModeById_4(args...) Func_0200b73c(args)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_0200b7c8, a0, a1, a2)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_0200b7d4, a0, a1, a2)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_0200b7e0, a0, a1, a2)
#define Battle_WaitMode0_2(args...) Func_0200b6e8(args)
#define Motion_SetSpeedLim_1(a0, a1) Call2(Func_0200b83e, a0, a1)
#define Motion_SetVarCbObj_1(args...) Func_0200b7dc(args)
#define SceneWork_SetStepValue_1(args...) Func_0200b816(args)
#define Motion_ArmCb_5(args...) Func_0200b84e(args)
#define Motion_EnableActCb_1(args...) Func_0200b798(args)
#define Motion_EnableActCb_2(a0, a1) Value2(Func_0200b7a0, a0, a1)
#define Motion_MarkActiveAndSetActionCallback_1(args...) Func_0200b7b8(args)
#define Battle_WaitMode0_3(args...) Func_0200b76e(args)
#define GameFlag_Set_1(a0) Call1(Func_0200b75c, a0)
#define GameFlag_Clear_1(a0) Call1(Func_0200b774, a0)
#define FieldScene_RunScene3b1_02000670 Func_02000670
#define FieldScene_RunScene3b1_02000728 Func_02000728
#define FieldScene_RunScene3b1_020007f8 Func_020007f8
#define Scene_RunActor11FlagDialogue Func_02000a80
#define Scene_RunScene3b1SequenceA Func_02000b84
#define Scene_RunScene3b1SequenceB Func_02000cc8
#define Scene_RunFlag300BranchDialogue Func_02000ee0
#define Scene_RunActor15FlagDialogue Func_02000fac
#define Scene_RunActor16FlagDialogue Func_02001078
#define Scene_RunPrimarySequence Func_02001804
#define Scene_RunScene3b1SequenceC Func_02001894
#define Scene_RunScene3b1SequenceD Func_02001a08
#define FieldScene_RunScene3b1_02003d10 Func_02003d10
#define FieldScene_RunScene3b1_02003dec Func_02003dec
#define FieldScene_RunScene3b1_02003e34 Func_02003e34
#define FieldScene_RunScene3b1_02003eec Func_02003eec
#define FieldScene_RunScene3b1_020040e8 Func_020040e8
#define FieldScene_RunScene3b1_0200413c Func_0200413c
#define FieldScene_RunScene3b1_02004198 Func_02004198
#define FieldScene_RunScene3b1_02005068 Func_02005068
#define Scene_RunPositionTransferPresentation Func_020050e4
#define FieldScene_RunScene3b1_020056dc Func_020056dc
#define FieldScene_RunScene3b1_02006110 Func_02006110
#define Dialogue_RunActor12Line Func_02000708
#define Dialogue_ShowLine1E19Or1D50 Func_020008a8
#define Dialogue_ShowLine1ECETo1ED0 Func_02001324
#define Dialogue_RunActor19TwoFlagLineA Func_02001378
#define Dialogue_RunActor20TwoFlagLine Func_020013cc
#define Dialogue_ShowLine1ED1Or1ED2 Func_02001420
#define Dialogue_RunActor22TwoFlagLine Func_0200145c
#define Dialogue_RunActor23BranchedDialogue Func_020014b0
#define Dialogue_RunActor24BranchedDialogue Func_02001504
#define Dialogue_RunActor25FlaggedLine Func_02001558
#define Dialogue_RunActor18TwoFlagLine Func_02001594
#define Dialogue_RunActor19TwoFlagLineB Func_020015e8
#define Dialogue_ShowLine1EDBTo1EDDActor20 Func_0200163c
#define Dialogue_RunActor21FlaggedLine Func_02001690
#define Dialogue_RunActor22BranchedDialogue Func_020016cc
#define Dialogue_ShowLine1EDBTo1EDDActor23 Func_02001720
#define Dialogue_ShowLine1EDBTo1EDDActor24 Func_02001774
#define Dialogue_ShowLine1EDEOr1EDF Func_020017c8
#define State_RunFlagBranchedActor8Setup Func_020008ec
#define Scene_RunFlagGatedThreeActorSetup Func_02001a60
#define Scene_RunFlagBranchedSetupCascade Func_02003f94
#define Scene_RunActors24And25Setup Func_02004218
#define Scene_InstallFlaggedActors10To17 Func_02004670
#define Scene_RunActors24And25SetupWithValue929 Func_02005684
#define Scene_RunActors24And25SetupWithValue92a Func_02005780
#define Dialogue_RunActorTenFlaggedDialogue Func_020009b4
#define Actor_SetFlagBit3ForActors28To35 Func_020037b4
#define RunActorsEightAndNineMapEvent Func_02005ca4
#define Dialogue_RunActorThirteenFlag300Branch Func_02000e14
#define Actor_CheckBucketOffsetPoint Func_02001280
#define State_ApplyLevelFromFlags Func_020012dc
#define State_ApplyActor8FourFlags Func_02005004
#define State_ScanTwoArraysAndCrossNotify Func_020063ac
#define Scene_RunThreeActorPresentation Func_02001b34
#define Scene_RunExtendedActorChoreography Func_02001f3c
#define Scene_RunBranchingActorPresentation Func_020027d8
#define State_RunFlagGatedSetupCascade Func_0200351c
#define RunSceneSelectionChain Func_0200366c
#define Scene_RunFourActorCoordinatePresentation Func_020038ac
#define Scene_RunFormationAndEffectPresentation Func_020052f4
#define Scene_RunExtendedFormationPresentation Func_020057ec
#define Scene_RunStepThen10 Func_0200486c
#define Scene_CallPairWith10 Func_02004880
#define OvObj_SetPositionAndHeading Func_02004890
#define ConfigureSceneMotionFlags Func_020048ac
#define Scene_RunSceneStep Func_020048e8
#define FindActorNearPosition Func_02001144
#define Actor_RunFirstMatchingSlot Func_02005038

#include "staged_actor.h"

/*
 * resource_3b1 owner at 0x020002f4, 76 bytes.
 * Puts an actor into its ready state: stage byte at +89, two mode fields on the
 * linked record, and the flag byte at +35 rewritten. Returns 1.
 *
 * The two mode writes are bitfields (the -13 mask stays 32-bit and is shared
 * between them); the +35 write is ordinary byte arithmetic, and its 0xfe mask
 * is narrowed to a byte, which is why the two are spelled differently.
 */
struct Rec_3b1 {
    u8 pad00[9];
    u8 lo9 : 2;
    u8 mode9 : 2;               /* +9,  bits 2..3 */
    u8 hi9 : 4;
    u8 pad0a[11];
    u8 lo15 : 2;
    u8 mode15 : 2;              /* +21, bits 2..3 */
    u8 hi15 : 4;
};

struct Work_3b1 {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Rec_3b1 *f80;        /* +80 */
    u8 pad51[5];
    u8 f89;                     /* +89 */
};

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};

struct SceneActor_02001144 {
    u8 pad00[10];
    s16 x;
    u8 pad0c[6];
    s16 y;
};

struct SceneWork {
    u8 pad00[52];
    struct SceneActor_02001144 *actors[58];
};

extern s16 Data_02000240[];
extern u8 Value_0000006f;
extern u8 Data_0200e984[];
extern u8 Data_0200e96c[];
extern u8 Data_0200eb94;
extern u8 Data_0200f420[];
extern u8 Data_0200f444[];
extern u8 Data_0200f570[];
extern u8 Data_0200f6fc[];
extern u8 Data_0200f81c[];
extern u8 Data_0200f930[];
extern u8 Data_0200f984[];
extern u8 Data_0200f9c0[];
extern u8 Data_0200fb58[];
extern u8 Data_0200fd44[];
extern u8 Data_0200fedc[];
extern u8 Value_00001f47;
extern u8 Value_00001ea0;
extern u8 Value_0000092b;
extern u8 Value_00001e7e;
extern u8 Value_00000993;
extern u8 Value_0000092a;
extern u8 Value_0000091a;
extern u8 Value_00000929;
extern u8 Value_00000938;
extern u8 Value_0000092f;
extern u8 Value_00001ea4;
extern u8 Value_00000996;
extern u8 Value_00001e8b;
extern u8 Value_0000091d;
extern u8 Value_0000093b;
extern u8 Value_00000932;
extern u8 Value_00001ea5;
extern u8 Value_00000997;
extern u8 Value_00001e8e;
extern u8 Value_0000091e;
extern u8 Value_0000093c;
extern u8 Value_00000933;
extern u8 Data_00001e40[];
extern u8 Data_0200e8e4[];
extern u8 Data_00001e46[];
extern u8 Data_0200e7c8[];
extern u8 Value_00001dd1;
extern u8 Value_00000925;
extern u8 Value_00001e19;
extern u8 Value_00001d50;
extern u8 Value_00001e9e;
extern u8 Value_00001e78;
extern u8 Value_00000917;
extern u8 Value_00000935;
extern u8 Value_0000092c;
extern u8 Value_00000928;
extern u8 Value_00001e9f;
extern u8 Value_00000992;
extern u8 Value_00001e7b;
extern u8 Value_00000919;
extern u8 Value_00000937;
extern u8 Value_0000092e;
extern u8 Value_00001ea3;
extern u8 Value_00000995;
extern u8 Value_00001e88;
extern u8 Value_0000091c;
extern u8 Value_0000093a;
extern u8 Value_00000931;
extern u32 Data_0200e668[];
extern u8 Data_0200e6a8[];
extern u8 Data_0200e6e4[];
extern u8 Value_0000093e;
extern u8 Value_00000911;
extern u8 Value_00000922;
extern u8 Data_0200e7f0[];
extern u8 Data_0200e818[];
extern const s32 Data_0200f2a0[];
extern const s32 Data_0200f300[];
extern const s32 Data_0200f360[];
extern const s32 Data_0200f3c0[];

void Func_02006488(struct StagedActor *actor, s32 state);
s32 Func_0200649a(void);
s32 Func_020064b4(void);
s32 Func_020064e6(u8 *o);
void Func_02006786();
void Func_020068d6();
void Func_0200671c(struct EffectRecord *record, s32 mode);
struct SceneActor *Func_02006778(s32 actor_id);
s32 Func_02006a6a();
s32 Func_02006a78();
s32 Func_02006a82();
s32 Func_02006a94();
s32 Func_02006aa8();
s32 Func_02006ab6();
void Func_020025f6();
s32 Func_02006b1e();
s32 Func_02006b38();
void Func_02006b3c();
s32 Func_02006b92();
void Func_02006bbc();
void Func_02006c30();
void Func_02006c3c();
void Func_02006c48();
void Func_02006c4a();
s32 Func_02006c5a();
void Func_02006c84();
void Func_02006c88();
void Func_02006ca0();
void Func_02006ca8();
void Func_02006cc0();
void Func_02004fae();
void Func_02004fc8();
s32 Func_02006bd2();
void Func_02006bf4();
s32 Func_02006c0c();
s32 Func_02006c26();
s32 Func_02006c3e();
s32 Func_02006c48_a();
void Func_02006c9e();
void Func_02006cb6();
void Func_02006cc4();
void Func_02006ce4();
void Func_02006d1e();
void Func_02006d1e_a();
void Func_02006d36();
void Func_02006d38();
void Func_02006d50();
void Func_02006d6a();
void Func_02006d82();
void Func_02005086();
void Func_020050b0();
s32 Func_020050ce();
s32 Func_02006ca2();
void Func_02006cc4_a();
void Func_02006cfe();
s32 Func_02006d14();
void Func_02006d60();
void Func_02006d86();
void Func_02006dac();
void Func_02006dbc();
s32 Func_02006ddc();
void Func_02006e0e();
void Func_02006e28();
void Func_02006e2c();
void Func_02006e44();
void Func_02006e4c();
s32 Func_02006f28(s32 n);
void Func_02006f58(void);
void Func_0200708a(s32 a, s32 b);
void Func_02006f60(s32 n);
void Func_0200704e(s32 n);
void Func_02007066(s32 a, s32 b);
void Func_02006f82(void);
s32 Func_02006f5c(s32 n);
u8 *Func_02001da0(void);
void Func_02006f92(void);
void Func_02001c60(u8 *o);
void Func_0200707e(s32 n);
void Func_02005348(s32 n);
void Func_02007034(u8 *o, s32 n);
u8 *Func_02006fd2(s32 n);
void Func_02007014(u8 *o, s32 a, s32 b);
void Func_02007042(u8 *o);
void Func_02007054(u8 *o, s32 a, s32 b);
void Func_02006fe0(void);
s32 Func_02006fb8(s32 n);
s32 Func_02006fc8(s32 n);
s32 Func_02006fd8(s32 n);
void Func_0200234a(s32 n, s32 a, s32 b);
void Func_02002356(s32 n, s32 a, s32 b);
void Func_02001d2e();
s32 Func_02001e74();
void Func_02005416();
void Func_02005476();
void Func_02005504();
s32 Func_02007030();
void Func_02007050();
s32 Func_020070a0();
void Func_020070aa();
s32 Func_020070e0();
void Func_020070e2();
s32 Func_020070e8();
void Func_020070ea();
s32 Func_020070fa();
s32 Func_020070fa_a();
s32 Func_02007100();
void Func_02007102();
s32 Func_0200710c();
s32 Func_0200710c_a();
void Func_02007110();
void Func_0200711e();
void Func_02007122();
void Func_02007128();
void Func_02007142();
void Func_0200714c();
void Func_02007162();
void Func_02007164();
void Func_0200716c();
void Func_02007170();
void Func_02007182();
void Func_02007198();
s32 Func_020071a8();
void Func_02001e72();
s32 Func_02001fb8();
void Func_0200555a();
void Func_020055be();
void Func_0200564a();
s32 Func_02007174();
void Func_02007194();
s32 Func_020071e4();
void Func_02007226();
s32 Func_02007228();
s32 Func_02007230();
void Func_02007232();
s32 Func_02007242();
s32 Func_02007242_a();
void Func_02007246();
s32 Func_02007248();
s32 Func_02007254();
s32 Func_02007254_a();
s32 Func_02007254_b();
void Func_02007266();
void Func_02007266_a();
void Func_0200726e();
void Func_0200728a();
void Func_02007290();
void Func_020072aa();
void Func_020072b2();
void Func_020072b8();
void Func_020072ca();
void Func_020072ce();
s32 Func_020072f0();
s32 Func_020072f0_a();
s32 Func_02007388(s32 n);
u8 *Func_020021cc(void);
void Func_020073be(void);
void Func_0200208c(u8 *o);
void Func_020074aa(s32 n);
void Func_02005774(s32 n);
void Func_02007460(u8 *o, s32 n);
u8 *Func_020073fe(s32 n);
void Func_02007440(u8 *o, s32 a, s32 b);
void Func_0200746e(u8 *o);
void Func_02007480(u8 *o, s32 a, s32 b);
void Func_0200740c(void);
s32 Func_020073e4(s32 n);
s32 Func_020073f4(s32 n);
s32 Func_02007404(s32 n);
void Func_02002776(s32 n, s32 a, s32 b);
void Func_02002782(s32 n, s32 a, s32 b);
s32 Func_02007454(s32 n);
u8 *Func_02002298(void);
void Func_0200748a(void);
void Func_02002158(u8 *o);
void Func_02007576(s32 n);
void Func_02005840(s32 n);
void Func_0200752c(u8 *o, s32 n);
u8 *Func_020074ca(s32 n);
void Func_0200750c(u8 *o, s32 a, s32 b);
void Func_0200753a(u8 *o);
void Func_0200754c(u8 *o, s32 a, s32 b);
void Func_020074d8(void);
s32 Func_020074b0(s32 n);
s32 Func_020074c0(s32 n);
s32 Func_020074d0(s32 n);
void Func_02002842(s32 n, s32 a, s32 b);
void Func_0200284e(s32 n, s32 a, s32 b);
void Func_0200609a();
void Func_020060f0();
void Func_02007cd6();
s32 Func_02007d04();
void Func_02007d0e();
void Func_02007d14();
s32 Func_02007d24();
void Func_02007d58();
void Func_02007d66();
void Func_02007d86();
void Func_02007d94();
void Func_02007da6();
void Func_02007dbc();
s32 Func_02007dcc();
s32 Func_02007d50();
s32 Func_02007d5a();
s32 Func_02007d78();
s32 Func_02007d82();
s32 Func_02007d8c();
void Func_02007d9a();
s32 Func_02007d9e();
s32 Func_02007db0();
s32 Func_02007dc8();
s32 Func_02007dd2();
void Func_02007e1c();
void Func_02007e8a();
void Func_02007e9e();
void Func_02007eb0();
void Func_02007ec2();
void Func_02007eca();
void Func_02007ef6();
void Func_02007f00();
void Func_02007f1c();
void Func_02007f24();
s32 Func_020062c6();
s32 Func_02007eae();
void Func_02007eb6();
void Func_02007ede();
void Func_02007f1a();
void Func_02007f28();
void Func_02007f6c();
void Func_02007ff4();
s32 Func_0200833a();
void Func_02008606();
s32 Func_0200862c();
void Func_020086b6();
void Func_0200a1d8();
void Func_0200a1dc();
void Func_0200a1ec();
void Func_0200a22e();
void Func_0200a232();
void Func_0200a246();
void Func_0200a254();
void Func_0200a272();
void Func_0200a280();
void Func_0200a2a6();
void Func_0200a2d2();
void Func_0200a2d6();
void Func_0200a2ee();
void Func_0200a32c();
void Func_0200a362();
void Func_0200a384();
void Func_0200a3d8();
void Func_020086e2();
void Func_02008710();
void Func_0200a2b8();
void Func_0200a386();
void Func_0200a3bc();
void Func_0200a3d6();
void Func_0200a3de();
void Func_02008720();
void Func_0200872a();
void Func_02008734();
void Func_02008766();
void Func_020087b2();
void Func_0200a300();
void Func_0200a380();
void Func_0200a39e();
void Func_0200a3aa();
void Func_0200a3c2();
void Func_0200a3cc();
void Func_0200a428();
void Func_0200a45e();
void Func_0200a478();
void Func_0200a480();
void Func_020087c8();
void Func_0200880e();
void Func_0200885a();
void Func_02008f02();
void Func_0200a3b8();
void Func_0200a428_a();
void Func_0200a442();
void Func_0200a452();
void Func_0200a46a();
void Func_0200a474();
void Func_0200a4d0();
void Func_0200a506();
void Func_0200a520();
void Func_0200a528();
void Func_020089de();
void Func_02008a1a();
void Func_0200a5b4();
void Func_0200a5c4();
void Func_0200a67e();
void Func_0200a690();
void Func_0200a6c6();
void Func_0200a6e0();
void Func_0200a6e8();
void Func_020089e2();
void Func_02008a4a();
void Func_02008a74();
void Func_0200a578();
void Func_0200a608();
void Func_0200a6ea();
void Func_0200a718();
void Func_0200a752();
void Func_0200a752_a();
void Func_02007968();
void Func_02008a8e();
void Func_02008a98();
void Func_02008aa6();
void Func_020092e8();
void Func_0200a664();
s32 Func_0200a6ce();
void Func_0200a6d8();
void Func_0200a6d8_a();
s32 Func_0200a6e0_a();
s32 Func_0200a6e8_a();
void Func_0200a716();
void Func_0200a788();
void Func_0200a790();
void Func_0200a798(s32, s32);
void Func_02008842();
void Func_02009968();
void Func_02009972();
s32 Func_02009980();
s32 Func_0200a018();
void Func_0200a1b2();
void Func_0200b53e();
void Func_0200b5a0();
void Func_0200b5a2();
void Func_0200b5aa();
s32 Func_0200b5b8();
void Func_0200b5c2();
void Func_0200b5ca();
void Func_0200b5f0();
void Func_020099a6();
void Func_02009ae0();
void Func_02009ae8();
void Func_02009b00();
void Func_02009b1a();
s32 Func_02009b26();
void Func_02009b3a();
void Func_02009ba6();
void Func_0200b5b6();
void Func_0200b60c();
s32 Func_0200b642();
void Func_0200b648();
void Func_0200b64e();
void Func_0200b654();
s32 Func_0200b656();
void Func_0200b660();
s32 Func_0200b66a();
void Func_0200b66c();
void Func_0200b698();
void Func_0200b698_a();
void Func_0200b6ae();
void Func_0200b6b0();
void Func_0200b6b2();
void Func_0200b6c4();
void Func_0200b6c8();
void Func_0200b6d4();
void Func_0200b6d8();
void Func_0200b6e8();
void Func_0200b6ea();
void Func_0200b70c();
void Func_0200b724();
void Func_0200b734();
void Func_0200b73a();
void Func_0200b73c();
void Func_0200b75c();
void Func_0200b76e();
void Func_0200b774();
void Func_0200b798();
s32 Func_0200b7a0();
void Func_0200b7b8();
void Func_0200b7c8();
void Func_0200b7d4();
void Func_0200b7dc();
void Func_0200b7e0();
void Func_0200b7ee();
void Func_0200b816();
void Func_0200b83e();
void Func_0200b84e();
void Func_02008ec4();
void Func_02009fea();
void Func_02009ff4();
s32 Func_0200a002();
s32 Func_0200a690_a();
s32 Func_0200a69a();
void Func_0200a84a();
void Func_0200bbc0();
void Func_0200bc22();
void Func_0200bc2c();
void Func_0200bc30();
void Func_0200bc3a();
void Func_0200bc3a_a();
void Func_0200bc4a();
s32 Func_0200bc52();
void Func_0200bc72();
void Func_0200bcf2();
void Func_0200bcfa(s32, s32);
void Func_0200a9f2();
void Func_0200aa06();
void Func_0200aa12();
void Func_0200aa3a();
void Func_0200ab22();
void Func_0200ab48();
s32 Func_0200ab92();
s32 Func_0200b0cc();
s32 Func_0200b0d6();
s32 Func_0200b0e0();
s32 Func_0200b0ea();
void Func_0200c610();
void Func_0200c652();
void Func_0200c68c();
void Func_0200c6fe();
void Func_0200c70c();
void Func_0200c712();
void Func_0200c71a();
void Func_0200c71a_a();
void Func_0200c71c();
void Func_0200c726();
void Func_0200c728();
void Func_0200c72e();
void Func_0200c736();
void Func_0200c746();
void Func_0200c748();
void Func_0200c764();
void Func_0200c770();
void Func_0200c77c();
void Func_0200c790();
void Func_0200c7a6();
void Func_0200c7b2();
void Func_0200c7b8();
void Func_0200c7bc();
void Func_0200c7c0();
void Func_0200c7c8();
void Func_0200c7cc();
void Func_0200c80e();
void Func_0200c816();
void Func_0200c81e();
void Func_0200c826();
void Func_0200c828();
void Func_0200c830();
void Func_0200c834();
void Func_0200c836();
void Func_0200c83c();
void Func_0200c844();
void Func_0200c854();
void Func_0200c85e();
void Func_0200c860();
void Func_0200c868();
void Func_0200c86c();
void Func_0200c874();
void Func_0200c878();
void Func_0200c87a();
void Func_0200c882();
void Func_0200c896();
void Func_0200c8da();
void Func_0200c992();
void Func_0200c996();
void Func_0200c99e();
void Func_02006bd4(void);
void Func_02006cba(s32 n);
s32 Func_02006ce2(s32 a, s32 b);
void Func_02006bee(void);
void Func_02006d74(void);
s32 Func_02006d52(s32 n);
void Func_02006e64(s32 n);
void Func_02006e74(s32 n);
void Func_02006e8c(s32 a, s32 b);
void Func_02006e8c_a(s32 a, s32 b);
void Func_02006da8(void);
void Func_020077f0();
void Func_0200782e();
int Func_020077ce();
int Func_020077e0();
void Func_020078e0();
void Func_020078f2();
void Func_020078fa();
void Func_02007912();
void Func_02007844();
void Func_02007882();
int Func_02007822();
int Func_02007834();
void Func_02007934();
void Func_02007946();
void Func_0200794e();
void Func_02007966();
void Func_02007898();
void Func_020078d6();
int Func_02007876();
int Func_02007888();
void Func_02007988();
void Func_0200799a();
void Func_020079a2();
void Func_020079ba();
void Func_020078ec();
void Func_02007918();
int Func_020078ca();
void Func_020079dc();
void Func_020079e4();
void Func_020079fc();
void Func_02007928();
void Func_02007968_a();
int Func_02007908();
int Func_0200791a();
void Func_02007a1a();
void Func_02007a2c();
void Func_02007a34();
void Func_02007a4c();
void Func_0200797c();
void Func_020079ba_a();
int Func_0200795a();
int Func_0200796c();
void Func_02007a6c();
void Func_02007a7e();
void Func_02007a86();
void Func_02007a9e();
void Func_020079d0();
void Func_02007a0e();
int Func_020079ae();
int Func_020079c0();
void Func_02007ac0();
void Func_02007ad2();
void Func_02007ada();
void Func_02007af2();
void Func_02007a24();
void Func_02007a50();
int Func_02007a02();
void Func_02007b14();
void Func_02007b1c();
void Func_02007b34();
void Func_02007a60();
void Func_02007a9e_a();
int Func_02007a3e();
int Func_02007a50_a();
void Func_02007b50();
void Func_02007b62();
void Func_02007b6a();
void Func_02007b82();
void Func_02007ab4();
void Func_02007af2_a();
int Func_02007a92();
int Func_02007aa4();
void Func_02007ba4();
void Func_02007bb6();
void Func_02007bbe();
void Func_02007bd6();
void Func_02007b08();
void Func_02007b46();
int Func_02007ae6();
int Func_02007af8();
void Func_02007bf8();
void Func_02007c0a();
void Func_02007c12();
void Func_02007c2a();
void Func_02007b5c();
void Func_02007b88();
int Func_02007b3a();
void Func_02007c4c();
void Func_02007c54();
void Func_02007c6c();
void Func_02007b98();
void Func_02007bd8();
int Func_02007b78();
int Func_02007b8a();
void Func_02007c8a();
void Func_02007c9c();
void Func_02007ca4();
void Func_02007cbc();
void Func_02007bec();
void Func_02007c2a_a();
int Func_02007bca();
int Func_02007bdc();
void Func_02007cdc();
void Func_02007cee();
void Func_02007cf6();
void Func_02007d0e_a();
void Func_02007c40();
void Func_02007c7e();
int Func_02007c1e();
int Func_02007c30();
void Func_02007d30();
void Func_02007d42();
void Func_02007d4a();
void Func_02007d62();
void Func_02007c94();
void Func_02007cc0();
int Func_02007c72();
void Func_02007d84();
void Func_02007d8c_a();
void Func_02007da4();
s32 Func_02006d94(s32 n);
u8 *Func_02001bd8(void);
void Func_02006dca(void);
void Func_02001a98(u8 *obj);
void Func_02006eb6(s32 n);
void Func_02005180(s32 n);
void Func_02006e6c(u8 *obj, s32 n);
u8 *Func_02006e0a(s32 n);
void Func_02006e4c_a(u8 *obj, s32 a, s32 b);
void Func_02006e7a(u8 *obj);
void Func_02006e8c_b(u8 *obj, s32 a, s32 b);
void Func_02006e18(void);
s32 Func_02006df0(s32 n);
s32 Func_02006e02(s32 n);
s32 Func_02006e12(s32 n);
void Func_02002184(s32 n, s32 a, s32 b);
void Func_02002190(s32 n, s32 a, s32 b);
s32 Func_02007f06(s32 flag);
void Func_02007f36(void);
void Func_020080ba(void);
void Func_02008072(s32 arg0, s32 arg1);
void Func_02006336();
void Func_02006336_a();
void Func_02008038(s32 message);
void Func_02006302(s32 id);
void Func_02006308(s32 id);
void Func_02006326(s32 id, s32 value);
void Func_02006318(s32 id);
void Func_02006328(s32 id);
void Func_02006346(s32 id, s32 value);
void Func_02006338(s32 id);
void Func_020080a6(s32 id, s32 arg1, s32 arg2);
void Func_02006348(s32 id);
void Func_02006366(s32 id, s32 value);
void Func_02006358(s32 id);
void Func_0200635e(s32 id);
void Func_02006364(s32 id);
void Func_02006382(s32 id, s32 value);
void Func_02006374(s32 id);
void Func_02007fb8(s32 flag);
void Func_02007fe4(void);
void Func_0200a460(void);
void Func_0200a4f0();
void Func_02008892();
void Func_0200a4fc();
void Func_020088a6();
void Func_02008634();
void Func_020088b6();
void Func_0200a5ce();
void Func_0200a5e8();
void Func_0200a4ae();
void Func_0200a546();
void Func_0200a4bc();
void Func_0200a662();
void Func_0200a4c8();
void Func_0200867e();
void Func_0200a4d4();
void Func_0200868a();
void Func_0200a4e0();
void Func_0200a578_a();
void Func_0200a4ee();
void Func_0200a694();
void Func_0200a4fa();
void Func_020086b0();
void Func_0200a506_a();
void Func_020086bc();
void Func_0200a512();
void Func_0200a6b8();
void Func_0200a51e();
void Func_0200895e();
s32 Func_0200a51c();
void Func_0200a69e();
s32 Func_0200a52e();
void Func_0200a6b0();
s32 Func_0200a540();
void Func_0200a6c2();
s32 Func_0200a552();
void Func_0200a6d4();
void Func_0200a6dc();
void Func_0200a6e4(void);
void Func_02008b0e();
void Func_02008b18();
void Func_02008b22();
void Func_02009532(void);
void Func_0200a6ec();
void Func_0200a718_a(void);
s32 Func_0200ab18();
s32 Func_0200ab58();
s32 Func_0200aba2();
s32 Func_0200abe2();
u8 *Func_0200962c();
void Func_0200abda();
void Func_02008f84();
void Func_0200abee();
void Func_02008f9a();
u8 *Func_0200966c();
void Func_0200ac1a();
u8 *Func_0200abc0();
void Func_02008fce();
void Func_0200ac38();
void Func_02008fe4();
u8 *Func_020096b6();
void Func_0200ac64();
void Func_0200900e();
void Func_0200ac78();
void Func_02009024();
u8 *Func_020096f6();
void Func_0200aca4();
u8 *Func_0200ac4a();
void Func_02009058();
void Func_0200acc2();
void Func_0200906e();
void Func_02009078();
void Func_02009082();
void Func_0200908c();
void Func_02009096();
s32 Func_0200a634();
void Func_0200bb5a(void);
void Func_02009f84();
void Func_02009f8e();
void Func_02008e84();
void Func_02009f9e();
void Func_0200bc08();
void Func_0200a9b8(void);
void Func_0200bb72();
void Func_0200bb9e(void);
s32 Func_0200a730();
s32 Func_0200a73a();
void Func_0200bc60(void);
void Func_0200a08a();
void Func_0200a094();
void Func_02008f8a();
void Func_0200a0a4();
void Func_0200bd0e();
void Func_0200bd18();
void Func_0200aac8(void);
void Func_0200bc82();
void Func_0200bcae(void);
s32 Func_02006e5c(s32 n);
u8 *Func_02001ca0(void);
void Func_02006e92(void);
void Func_02001b60(u8 *o);
void Func_02006f7e(s32 n);
void Func_02005248(s32 n);
void Func_02006f34(u8 *o, s32 n);
u8 *Func_02006ed2(s32 n);
void Func_02006f14(u8 *o, s32 a, s32 b);
void Func_02006f42(u8 *o);
void Func_02006f54(u8 *o, s32 a, s32 b);
void Func_02006ee0(void);
s32 Func_02006eb8(s32 n);
s32 Func_02006ec8(s32 n);
s32 Func_02006ed8(s32 n);
void Func_0200224a(s32 n, s32 a, s32 b);
void Func_02002256(s32 n, s32 a, s32 b);
u8 *Func_02009ca8();
void Func_0200a554();
void Func_0200a59a();
void Func_0200a5b6();
void Func_0200a5f2();
void Func_0200c170();
void Func_0200c19c();
void Func_0200c1c0();
void Func_0200c256();
void Func_0200c25a();
void Func_0200c2b8(s32, s32, s32);
void Func_0200c2c2();
s32 Func_020072bc(s32 n);
u8 *Func_02002100(void);
void Func_020072f2(void);
void Func_02001fc0(u8 *o);
void Func_020073de(s32 n);
void Func_020056a8(s32 n);
void Func_02007394(u8 *o, s32 n);
u8 *Func_02007332(s32 n);
void Func_02007374(u8 *o, s32 a, s32 b);
void Func_020073a2(u8 *o);
void Func_020073b4(u8 *o, s32 a, s32 b);
void Func_02007340(void);
s32 Func_02007318(s32 n);
s32 Func_02007328(s32 n);
s32 Func_02007338(s32 n);
void Func_020026aa(s32 n, s32 a, s32 b);
void Func_020026b6(s32 n, s32 a, s32 b);
u8 *Func_02007772();
s32 Func_020023f0();
s32 Func_0200773c();
s32 Func_02007784();
s32 Func_02007792();
s32 Func_020077a0();
s32 Func_020062b6();
void Func_0200a044();
void Func_0200a04c();
void Func_0200a054();
void Func_0200a05e();
s32 Func_0200c876();
void Func_0200c890();
s32 Func_0200c896_a();
void Func_0200c8b0();
void Func_0200c8b0_a();
void Func_0200c8b8();
void Func_020063f0();
void Func_02006412();
void Func_02006418();
void Func_0200641a();
void Func_02006448();
void Func_02006458();
void Func_02006476();
void Func_02006484();
void Func_02006498();
void Func_020064a6();
void Func_020064b4_a();
void Func_020064ba();
void Func_020064c8();
void Func_020064d6();
void Func_020064e4();
void Func_020064f2();
void Func_02006514();
void Func_02006554();
void Func_0200656a();
void Func_02006570();
void Func_0200658c();
void Func_0200659a();
void Func_020065b2();
void Func_020065ee();
void Func_020065fe();
void Func_02006622();
void Func_02006630();
void Func_0200664a();
void Func_02006658();
void Func_02006720();
s32 Func_02007fe6();
void Func_02008018();
void Func_020080bc();
void Func_020080f4();
void Func_02008120();
void Func_02008122();
void Func_02008136();
void Func_02008154();
void Func_0200815e();
void Func_0200817c();
void Func_02008180();
void Func_02008188();
void Func_02008192();
void Func_0200819c();
void Func_020081b6();
void Func_020081bc();
void Func_020081c0();
void Func_020081c2();
void Func_020081c6();
void Func_020081dc();
void Func_020081e0();
void Func_020081e6();
void Func_02008214();
void Func_02008228();
void Func_0200825a();
void Func_02008266();
void Func_02008268();
void Func_02008278();
void Func_02008280();
void Func_02008286();
void Func_020082a4();
void Func_020082a8();
void Func_020082b8();
void Func_020082c0();
void Func_020082c2();
void Func_020082f2();
void Func_020082f6();
void Func_020082fe();
void Func_02008308();
void Func_02008312();
void Func_0200831e();
void Func_02008320();
void Func_02008322();
void Func_02008324();
void Func_0200832a();
void Func_0200832c();
void Func_02008330();
void Func_02008336();
void Func_02008340();
void Func_02008346();
void Func_0200834e();
void Func_02008358();
void Func_0200835c();
void Func_02008366();
void Func_0200838c();
void Func_02008394();
void Func_020083b6();
void Func_020083b8();
void Func_020083c6();
void Func_020083e6();
void Func_0200840a();
void Func_0200840e();
void Func_02008426();
void Func_02008454();
void Func_02008460();
void Func_02008476();
void Func_020084a8();
void Func_02008320_a();
void Func_02008308_a();
void Func_02008312_a();
void Func_0200832c_a();
void Func_02008340_a();
u8 *Func_02008336_a();
u8 *Func_02008358_a();
void Func_020083c6_a();
void Func_0200838c_a();
void Func_020083b8_a();
void Func_020067e8();
void Func_02006814();
void Func_0200683a();
void Func_0200686e();
void Func_02006888();
void Func_02006898();
void Func_020068aa();
void Func_020068c4();
void Func_020068d2();
void Func_020068e0();
void Func_020068f4();
void Func_02006904();
void Func_0200693e();
void Func_02006990();
void Func_020069b6();
void Func_020069de();
void Func_020069ec();
void Func_02006a10();
void Func_02006a20();
void Func_02006a36();
void Func_02006a3c();
void Func_02006a4e();
void Func_02006a6a_a();
void Func_02006a78_a();
void Func_02006a86();
void Func_02006afe();
void Func_02006b0e();
void Func_02006b1c();
void Func_02006b32();
void Func_02006b4c();
void Func_02006b6e();
void Func_02006b86();
void Func_02006b98();
void Func_02006bae();
void Func_02006bfa();
void Func_02006c0e();
void Func_02006c52();
void Func_02006c72();
void Func_02006c8c();
void Func_02006c9a();
void Func_02006cc8();
void Func_02006da8_a();
void Func_02006dae();
void Func_02006dbe();
void Func_02006dd0();
void Func_02006df4();
void Func_02006e08();
void Func_02006e18_a();
void Func_02006e2e();
void Func_02006e34();
void Func_02006e4a();
void Func_02006e58();
void Func_02006e62();
void Func_02006e74_a();
void Func_02006e8c_c();
void Func_02006ea2();
void Func_02006ea8();
void Func_02006eb8_a();
void Func_02006ebe();
void Func_02006f08();
void Func_02006f16();
void Func_02006f24();
void Func_02006f32();
void Func_02006f4c();
void Func_02006f5a();
void Func_020084f0();
void Func_0200851e();
void Func_02008540();
void Func_02008542();
void Func_0200854e();
void Func_02008558();
void Func_02008564();
void Func_0200856e();
void Func_02008578();
void Func_0200857a();
void Func_02008586();
void Func_0200858e();
void Func_020085a0();
void Func_020085a8();
void Func_020085ae();
void Func_020085b4();
void Func_020085c0();
void Func_020085c2();
void Func_020085c6();
void Func_020085ca();
void Func_020085dc();
void Func_020085e8();
void Func_020085f2();
void Func_020085f6();
void Func_02008608();
void Func_0200860a();
void Func_0200860e();
void Func_0200861e();
void Func_02008620();
void Func_0200862c_a();
void Func_02008634_a();
void Func_0200864c();
void Func_02008674();
void Func_02008680();
void Func_020086a2();
void Func_020086b6_a();
void Func_020086ba();
void Func_020086ca();
void Func_020086f2();
void Func_02008702();
void Func_0200870c();
void Func_0200870e();
void Func_02008714();
void Func_02008724();
void Func_0200872c();
void Func_02008736();
void Func_02008744();
void Func_02008746();
void Func_02008756();
void Func_0200875a();
void Func_02008768();
void Func_02008784();
void Func_02008794();
void Func_020087ae();
void Func_020087bc();
void Func_020087c0();
void Func_020087d6();
void Func_020087e6();
void Func_02008800();
void Func_02008802();
void Func_0200880e_a();
void Func_02008818();
void Func_02008822();
void Func_02008826();
void Func_02008828();
void Func_02008834();
void Func_02008840();
void Func_0200884c();
void Func_0200885a_a();
void Func_0200885c();
void Func_0200886a();
void Func_0200886c();
void Func_0200888c();
void Func_020088b4();
void Func_020088ba();
void Func_020088c4();
void Func_020088d2();
void Func_020088f4();
void Func_0200890e();
void Func_02008916();
void Func_0200893e();
void Func_02008958();
void Func_02008960();
void Func_0200896a();
void Func_0200897c();
void Func_02008980();
void Func_02008986();
void Func_02008988();
void Func_02008992();
void Func_0200899e();
void Func_020089a8();
void Func_020089b8();
void Func_020089c0();
void Func_020089e8();
void Func_020089fa();
void Func_02008a0a();
void Func_02008a0c();
void Func_02008a2a();
void Func_02008a3e();
void Func_02008a46();
void Func_02008a4e();
void Func_02008a50();
void Func_02008a56();
void Func_02008a58();
void Func_02008a62();
void Func_02008a68();
void Func_02008a6c();
void Func_02008a72();
void Func_02008a76();
void Func_02008a7c();
void Func_02008a86();
void Func_02008a92();
void Func_02008a9e();
void Func_02008aa0();
void Func_02008ab4();
void Func_02008abc();
void Func_02008ad6();
void Func_02008ae8();
void Func_02008b00();
void Func_02008b0e_a();
void Func_02008b1c();
void Func_02008b34();
void Func_02008b46();
void Func_02008b50();
void Func_02008b52();
void Func_02008b56();
void Func_02008b8e();
void Func_02008b90();
void Func_02008b92();
void Func_02008bb8();
void Func_02008bc6();
void Func_02008bea();
void Func_02008bee();
void Func_02008bf4();
void Func_02008bfa();
void Func_02008c0e();
void Func_02008c26();
void Func_02008c30();
void Func_02008c3e();
void Func_02008c48();
void Func_02008c4a();
void Func_02008c52();
void Func_02008c68();
void Func_02008c6a();
void Func_02008c76();
void Func_02008c82();
void Func_02008c94();
void Func_02008c9a();
void Func_02008ca2();
void Func_02008cb8();
void Func_02008cba();
void Func_02008ce4();
void Func_02008cee();
void Func_02008cf8();
void Func_02008d02();
void Func_02008d2c();
void Func_02008d44();
void Func_02008d98();
void Func_02008dc8();
void Func_020085a8_a();
void Func_020085c0_a();
void Func_020087d6_a();
void Func_0200888c_a();
void Func_02008b1c_a();
void Func_02008c26_a();
void Func_0200707e_a();
void Func_0200710c_b();
void Func_0200711e_a();
void Func_02007160();
void Func_0200716e();
void Func_020071be();
void Func_020071cc();
void Func_020071e0();
void Func_02007232_a();
void Func_02007246_a();
void Func_0200725e();
void Func_02007264();
void Func_02007274();
void Func_0200727a();
void Func_02007296();
void Func_0200729c();
void Func_020072a4();
void Func_020072b2_a();
void Func_020072c8();
void Func_020072ce_a();
void Func_020072de();
void Func_020072ec();
void Func_020072fa();
void Func_02007402();
void Func_02007410();
void Func_02007426();
void Func_02007434();
void Func_02007442();
void Func_02007450();
void Func_0200745e();
void Func_020074de();
void Func_020074fe();
void Func_02007508();
void Func_02007522();
void Func_02007540();
void Func_02007558();
void Func_02007580();
void Func_0200758e();
void Func_020075c2();
void Func_020075c8();
void Func_020075d8_a();
void Func_020075de();
void Func_020075e6();
void Func_02007614();
void Func_0200761a();
void Func_02007630();
void Func_0200768a();
void Func_02007698();
void Func_020076c0();
void Func_020076d6();
void Func_020076e6();
void Func_020076f4();
void Func_0200776c();
void Func_02007814();
void Func_02008caa();
s32 Func_02008d6e();
void Func_02008d78();
void Func_02008d7e();
void Func_02008d92();
void Func_02008d9a();
void Func_02008da2();
void Func_02008daa();
void Func_02008db4();
void Func_02008dba();
void Func_02008de4();
void Func_02008dfe();
void Func_02008e16();
void Func_02008e1a();
void Func_02008e22();
void Func_02008e2c();
void Func_02008e2e();
s32 Func_02008e36();
void Func_02008e3a();
void Func_02008e46();
void Func_02008e5c();
void Func_02008e6e();
void Func_02008e74();
void Func_02008e94();
void Func_02008ec8();
void Func_02008ed0();
void Func_02008ede();
void Func_02008ef4();
void Func_02008ef6();
void Func_02008f0c();
void Func_02008f24();
void Func_02008f26();
void Func_02008f3c();
void Func_02008f4a();
void Func_02008f56();
void Func_02008f7c();
void Func_02008f82();
void Func_02008f92();
void Func_02008f96();
void Func_02008fa4();
void Func_02008fae();
void Func_02008fb6();
void Func_02008fba();
u8 *Func_02008fbe();
void Func_02008fc0();
void Func_02008fe4_a();
void Func_02008fea();
void Func_02008ff4();
u8 *Func_02008ffa();
void Func_02008ffc();
void Func_02009026();
void Func_0200902c();
void Func_02009030();
u8 *Func_02009036();
void Func_0200905a();
void Func_02009062();
void Func_02009064();
void Func_02009068();
void Func_0200906c();
void Func_0200906e_a();
void Func_02009078_a();
void Func_020090a0();
void Func_020090a4();
void Func_020090a8();
s32 Func_020090b2();
void Func_020090dc();
void Func_0200910c();
void Func_02009118();
void Func_0200911e();
void Func_0200914c();
void Func_02009154();
void Func_02009156();
void Func_02009172();
void Func_02009178();
s32 Func_0200917a();
void Func_020091b6();
void Func_020091c4();
void Func_020091d2();
void Func_020091ec();
void Func_020091fa();
void Func_020091fe();
void Func_02009214();
void Func_0200921a();
void Func_02009222();
void Func_02009224();
void Func_02009244();
void Func_0200925e();
void Func_0200926c();
void Func_02009270();
void Func_02009286();
void Func_020092a4();
void Func_020092be();
void Func_020092c6();
void Func_020092c8();
void Func_020092d4();
s32 Func_020092fa();
void Func_02009308();
void Func_0200931a();
void Func_02009350();
void Func_0200936c();
void Func_0200936e();
void Func_02009372();
void Func_02009382();
void Func_02009386();
void Func_0200938c();
void Func_02009390();
void Func_02009396();
void Func_0200939e();
void Func_020093a0();
void Func_020093aa();
void Func_020093b4();
void Func_020093b6();
void Func_020093bc();
s32 Func_020093c2();
void Func_020093c4();
void Func_020093ca();
void Func_020093d8();
void Func_020093dc();
void Func_020093e4();
void Func_020093e6();
void Func_020093e8();
void Func_020093ec();
u8 *Func_0200940c();
void Func_0200941c();
void Func_0200941e();
void Func_0200942c();
u8 *Func_0200943c();
void Func_0200943e();
void Func_02009448();
void Func_0200944c();
void Func_0200944e();
void Func_02009452();
void Func_0200945e();
void Func_0200946e();
void Func_02009474();
void Func_0200947c();
void Func_0200947e();
void Func_0200948e();
void Func_0200949e();
void Func_020094ac();
void Func_020094bc();
void Func_020094be();
void Func_02008e2e_a();
void Func_02008e74_a();
void Func_020072ec_a();
void Func_020092c6_a();
void Func_020093bc_a();
u8 *Func_020093dc_a();
void Func_0200947e_a();
s32 Func_020099c8();
void Func_02009a7e();
void Func_02009a88();
void Func_02009a92();
void Func_02009a9c();
void Func_02009aa6();
void Func_02007efa();
s32 Func_02009a0a();
void Func_02007e0e();
void Func_02009a86();
void Func_02007e32();
void Func_02007e40();
void Func_02007e4c();
void Func_02007e58();
void Func_02007e64();
void Func_02007e74();
void Func_02009aec();
s32 Func_02009a94();
void Func_02008600();
s32 Func_02009aa4();
s32 Func_02009aba();
s32 Func_02009ac4();
void Func_02007f1a_a();
void Func_02009b84();
void Func_02006e2a();
void Func_0200703e();
void Func_02007952();
void Func_02007978();
void Func_0200799e();
void Func_020079c4();
void Func_02007f2a();
void Func_02007f3a();
void Func_02007f72();
void Func_02007fc0();
void Func_02007fd6();
void Func_02007fe0();
void Func_02007ffc();
void Func_02008006();
void Func_02008022();
void Func_0200802c();
void Func_02008048();
void Func_02009a92_a();
s32 Func_02009b1c();
s32 Func_02009b54();
s32 Func_02009b80();
s32 Func_02009ba6_a();
s32 Func_02009bcc();
s32 Func_02009bf2();
void Func_02009c0e();
void Func_02009c1e();
s32 Func_02009c20();
s32 Func_02009c2a();
void Func_02009cb2();
void Func_02009cca();
void Func_02008188_a();
void Func_0200818e();
void Func_0200819e();
void Func_020081ae();
void Func_020081b8();
void Func_020081c6_a();
void Func_02008204();
void Func_02008212();
void Func_02008220();
void Func_0200823c();
void Func_020082fa();
void Func_02008312_b();
void Func_02008332();
void Func_02008348();
void Func_02008352();
void Func_0200835c_a();
void Func_02008366_a();
void Func_0200839c();
void Func_020083b6_a();
void Func_020083c4();
void Func_020083f6();
void Func_02008400();
void Func_0200840a_a();
void Func_02008438();
void Func_02008446();
void Func_02008456();
void Func_02008478();
void Func_0200847e();
void Func_02008498();
void Func_0200849e();
void Func_020084ae();
void Func_020084ca();
void Func_020084cc();
void Func_020084fe();
void Func_02008506();
void Func_0200858c();
void Func_02008596();
void Func_020085dc_a();
void Func_02009d84();
void Func_02009e4c();
void Func_02009e76();
void Func_02009e90();
void Func_02009e9c();
void Func_02009ea8();
u8 *Func_02009eaa();
void Func_02009ed2();
u8 *Func_02009ede();
void Func_02009f06();
void Func_02009f0c();
void Func_02009f0e();
void Func_02009f14();
void Func_02009f18();
void Func_02009f20();
void Func_02009f3a();
void Func_02009f3c();
void Func_02009f4a();
void Func_02009f4c();
void Func_02009f54();
void Func_02009f7a();
void Func_02009f80();
void Func_02009f82();
void Func_02009fc0();
void Func_02009ff6();
void Func_0200a00e();
void Func_0200a020();
void Func_0200a026();
void Func_0200a02e();
void Func_0200a034();
void Func_0200a03c();
void Func_0200a044_a();
void Func_0200a04c_a();
void Func_0200a04e();
void Func_0200a058();
void Func_0200a064();
void Func_0200a074();
void Func_0200a088();
void Func_0200a08a_a();
void Func_0200a0a0();
void Func_0200a0b2();
void Func_0200a0bc();
void Func_0200a0e4();
void Func_0200a0f6();
void Func_0200a10c();
void Func_0200a10e();
void Func_0200a124();
void Func_0200a132();
void Func_0200a13c();
void Func_0200a15e();
void Func_0200a160();
void Func_0200a162();
void Func_0200a166();
void Func_0200a16c();
void Func_0200a18a();
void Func_0200a194();
void Func_0200a19e();
void Func_0200a1a2();
void Func_0200a1a6();
void Func_0200a1aa();
void Func_0200a1b2_b();
void Func_0200a1b4();
void Func_0200a1c8();
void Func_0200a1da();
void Func_0200a1de();
void Func_0200a1ea();
void Func_0200a1ec_a();
void Func_0200a214();
void Func_0200a25c();
void Func_020081ae_a();
u8 *Func_02009f14_a();
void Func_0200a026_a();
void Func_0200a1a6_a();
void Func_0200a1b2_a();
void Func_02009ba2();
void Func_02009bb2();
void Func_02009bc0();
void Func_02009bd2();
void Func_02009be2();
void Func_02009bf6();
void Func_02009c2e();
void Func_02009c42();
void Func_02009cb2_a();
void Func_02009cce();
void Func_02009cde();
void Func_02009cfe();
void Func_02009d64();
void Func_02009d6e();
void Func_02009d84_a();
void Func_02009d90();
void Func_02009dc8();
void Func_02009dd6();
void Func_02009df2();
void Func_02009df6();
void Func_02009e16();
void Func_02009e7c();
void Func_02009e9c_a();
void Func_02009f16();
void Func_0200b850();
void Func_0200b8c0();
void Func_0200b8fe();
void Func_0200b942();
void Func_0200b94e();
void Func_0200b95a();
void Func_0200b96c();
void Func_0200b978();
void Func_0200b97c();
void Func_0200b984();
void Func_0200b98a();
void Func_0200b990();
void Func_0200b9b6();
void Func_0200b9ca();
void Func_0200b9d8();
void Func_0200b9e4();
void Func_0200b9e6();
void Func_0200b9ec();
void Func_0200b9ee();
void Func_0200b9f0();
void Func_0200b9fc();
void Func_0200b9fe();
void Func_0200ba0c();
void Func_0200ba12();
void Func_0200ba2a();
void Func_0200ba34();
s32 Func_0200ba38();
void Func_0200ba3a();
void Func_0200ba40();
void Func_0200ba6e();
void Func_0200ba72();
void Func_0200ba82();
void Func_0200ba98();
void Func_0200baaa();
void Func_0200babe();
void Func_0200bac0();
void Func_0200bac4();
void Func_0200bac6();
void Func_0200bad4();
void Func_0200bad6();
void Func_0200bade();
void Func_0200bae2();
void Func_0200bae4();
void Func_0200baea();
void Func_0200baee();
void Func_0200baf4();
s32 Func_0200bb00();
void Func_0200bb1c();
void Func_0200bb34();
void Func_0200bb38();
void Func_0200bbce();
void Func_0200bbe8();
void Func_0200bc58();
void Func_0200b978_a();
void Func_0200b9fc_a();
void Func_0200ba34_a();
void Func_02009ed8();
void Func_0200a106();
void Func_0200a110();
void Func_0200a126();
void Func_0200a16c_a();
void Func_0200a186();
void Func_0200a188();
void Func_0200a212();
void Func_0200a218();
void Func_0200a2a6_a();
void Func_0200a2ac();
void Func_0200a37a();
void Func_0200a384_a();
void Func_0200a38e();
void Func_0200a398();
void Func_0200a3dc();
void Func_0200a3e2();
void Func_0200a402();
void Func_0200a4e8();
s32 Func_0200a7a2();
s32 Func_0200a7ac();
s32 Func_0200a7b6();
void Func_0200bcdc();
void Func_0200bce0();
void Func_0200bd28();
void Func_0200bd38();
u8 *Func_0200bd42();
void Func_0200bd54();
void Func_0200bd64();
void Func_0200bd66();
void Func_0200bd76();
void Func_0200bd7e();
void Func_0200bd8e();
void Func_0200bd92();
void Func_0200bd94();
void Func_0200bdbe();
void Func_0200bdec();
void Func_0200be84();
void Func_0200be88();
void Func_0200beb4();
void Func_0200bed6();
void Func_0200bee6();
void Func_0200beea();
void Func_0200bef0();
void Func_0200bef8();
void Func_0200bf04();
void Func_0200bf0e();
void Func_0200bf1a();
void Func_0200bf1c();
void Func_0200bf20();
void Func_0200bf24();
void Func_0200bf26();
void Func_0200bf2e();
void Func_0200bf30();
void Func_0200bf3a();
void Func_0200bf3e();
void Func_0200bf42();
void Func_0200bf46();
void Func_0200bf48();
void Func_0200bf52();
void Func_0200bf76();
void Func_0200bf94();
void Func_0200bfa0();
void Func_0200bfac();
void Func_0200bfb8();
void Func_0200bfba();
void Func_0200bfc2();
void Func_0200bfc4();
void Func_0200bfca();
void Func_0200bfda();
s32 Func_0200c01e();
void Func_0200c054();
void Func_0200c05e();
void Func_0200c068();
void Func_0200c072();
void Func_0200c07e();
void Func_0200c080();
void Func_0200c08c();
void Func_0200c08e();
void Func_0200c098();
void Func_0200c0a2();
void Func_0200c0a4();
void Func_0200c0a6();
void Func_0200c0ae();
void Func_0200c0b2();
void Func_0200c0ba();
void Func_0200c0bc();
void Func_0200c0be();
void Func_0200c0c2();
void Func_0200c0c4();
void Func_0200c0c8();
void Func_0200c0d0();
void Func_0200c0d8();
void Func_0200c0da();
void Func_0200c0e2();
void Func_0200c0e6();
void Func_0200c0ea();
void Func_0200c0ec();
void Func_0200c0f0();
void Func_0200c0f6();
void Func_0200c0f8();
void Func_0200c102();
void Func_0200c104();
void Func_0200c110();
void Func_0200c14a();
void Func_0200bd92_a();
u8 *Func_0200beea_a();
void Func_0200bf0e_a();
void Func_0200bf52_a();
void Func_0200bfac_a();
void Func_0200c0ea_a();
void Func_0200c0d0_a();
void Func_02005c48(void);
void Func_0200ae2a();
void Func_0200ad38();
void Func_0200ae5a();
void Func_0200ade4();
void *Func_0200ad8a();
void Func_0200ad3e(void);
void Func_0200ad9c(u32 flags);
void Func_0200aeb6(s32 x, s32 y, s32 z, u32 enabled);
void Func_0200aed0(void);
void Func_020082ba();
void Func_0200924a();
void Func_020092e8_a();
void Func_02009382_a();
void Func_02009390_a();
void Func_0200939e_a();
void Func_020093ac();
void Func_020093b8();
void Func_020093c2_a();
void Func_020094de();
void Func_02009596();
void Func_020095ca();
void Func_020095d6();
void Func_0200962a();
void Func_0200963c();
void Func_0200964a();
void Func_0200965a();
void Func_02009666();
void Func_0200967e();
void Func_02009694();
void Func_020096aa();
void Func_020096ba();
void Func_020096c8();
void Func_020096d8();
void Func_020096e6();
void Func_020096f6_a();
void Func_02009706();
void Func_02009712();
void Func_0200978a();
void Func_02009794();
void Func_020097a0();
void Func_020097c6();
void Func_0200aedc();
void Func_0200af14();
void Func_0200af1c();
void Func_0200af24();
u8 *Func_0200af28();
void Func_0200af2c();
u8 *Func_0200af38();
void Func_0200af44();
void Func_0200af4e();
void Func_0200af50();
void Value_0200e5d0();
u8 *Value_0200e4e8();
void Func_0200af6c();
void Func_0200af6e();
void Func_0200af76();
void Func_0200af7c();
void Func_0200af80();
void Func_0200af8a();
void Func_0200afb2();
void Func_0200afc2();
void Func_0200afe4();
void Func_0200afe8();
void Func_0200aff2();
void Func_0200affc();
void Func_0200b006();
u8 *Func_0200b018();
u8 *Func_0200b024();
u8 *Func_0200b038();
void Func_0200b040();
u8 *Func_0200b050();
u8 *Func_0200b060();
u8 *Func_0200b074();
void Func_0200b07a();
u8 *Func_0200b086();
u8 *Func_0200b090();
void Func_0200b09a();
void Func_0200b0b2();
void Func_0200b0d0();
void Func_0200b0d6_a();
void Func_0200b0dc();
void Value_0200e640();
void Value_0200e550();
void Func_0200b0ec();
void Func_0200b0f8();
void Func_0200b100();
void Func_0200b10e();
void Func_0200b11c();
void Func_0200b12a();
void Func_0200b138();
void Func_0200b146();
u8 *Func_0200b14e();
void Func_0200b154();
void Func_0200b162();
void Func_0200b1c0();
void Func_0200b1da();
void Func_0200b1e4();
void Func_0200b1ee();
void Func_0200b1f8();
void Func_0200b200();
void Value_0200e5d0_b();
void Value_0200e4c0();
void Func_0200b214();
void Func_0200b218();
void Func_0200b21e();
void Func_0200b224();
void Func_0200b234();
void Func_0200b23e();
void Func_0200b258();
void Func_0200b2a0();
void Func_0200b2aa();
void Func_0200b2b8();
void Func_0200b2ca();
void Func_0200b2f4();
void Func_0200b334();
void Func_0200b342();
void Func_0200b350();
void Func_0200b368();
void Func_0200b370();
void Func_0200b372();
void Func_0200b376();
void Func_0200b37a();
void Func_0200b37c();
void Func_0200b386();
void Func_0200b3de();
void Func_0200b3e4();
void Func_0200b3e6();
void Func_0200b3ea();
void Func_0200b402();
void Func_0200b41a();
void Func_0200b42c();
void Func_0200b50e();
u8 *Func_0200b52c();
s32 Func_0200b4e4();
void Func_0200b59a();

/* Scene-step dispatcher for overlay resource 0x3b1.
 *
 * The owner takes a step selector plus two step parameters and jumps through a
 * 26-entry table into one bounded block of scene setup calls per step. Step 18
 * finishes by re-entering the dispatcher with step 14, which the compiler turns
 * into a jump back to the range check.
 *
 * Uncertain: the roles of the two parameters differ per step (actor slot,
 * count, flag, upper loop bound), so they keep neutral names here. The record
 * fields written at +6 (halfword) and +24 (word) are the same scene-object
 * fields the neighbouring scene sources touch; their meaning is not recovered.
 */

/* Loader-relocated overlay calls: each Func_ symbol names the pre-relocation
 * call word the image holds.
 *
 * Three of those pre-relocation words repeat in this owner while reaching
 * different runtime helpers (0x0200af5a, 0x0200b0e8 and 0x0200b20c each cover
 * two distinct destinations), so one Func_ spelling cannot name both sites.
 * Those six sites are declared by their runtime address instead, which the
 * overlay symbol resolver binds directly. Registering this owner as a
 * translation unit with explicit absolute_symbols would let them go back to
 * suffixed Func_ spellings without changing a byte. */

/* The scene work record pointer; +0x1c0 holds the scene request word. */

/*
 * Actor slot search for resource_3b1.  The 48-byte owner at 0x02005038 has no
 * pool; the halfword at 0x02005066 is alignment before the next owner.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/*
 * Field scene beat for overlay resource_3b1.  Each callee is named for its own
 * call site: every call reaches its target through its own local veneer, even
 * where the same logical callee is used from more than one site.
 */

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
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    return f();
}

static __inline__ void Call1_02000b84(void (*f)(), s32 a0)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call1_02000cc8(void (*f)(), s32 a0)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001a08(s32 (*f)(), s32 a0)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call1_02003d10(void (*f)(), s32 a0)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_020040e8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value2_02004198(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value2_02005068(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value2_020056dc(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

/*
 * resource_3b1 helper: set bit 3 of the flags byte of actors 28 through 35.
 */

/*
 * The owner at 0x020037b4 is 36 bytes: 34 bytes of code and one alignment
 * halfword; it needs no literal pool.  The index, bit and zero are held in
 * locals initialised in that order.  The zero is an OR identity that folds
 * away unless it has its own local, and initialising the index in a for-init
 * emits it last.  Func_02009ca8 names the in-overlay entry point, not its
 * veneer.
 */
static __inline__ void SetDirection(s32 actor, s32 direction, s32 duration)
{
    Func_0200c2b8(actor, direction, duration);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001b34(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 */
static __inline__ void Call1_02001f3c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ u8 *Pointer1(u8 *(*f)(), s32 id)
{
    return f(id);
}

/*
 * Flag-gated scene setup for overlay resource_3b1. Each callee name refers
 * to that call site's own call word rather than to a shared runtime
 * address.
 */

/* Scene selection chain of the resource_3b1 overlay: tests scene ids in turn and runs the matching call sequence. */
static __inline__ void Call3_0200351c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020038ac(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ u8 *Pointer1_020038ac(u8 *(*f)(), s32 a)
{
    return f(a);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call lets the compiler precompute a
 * shifted constant into a pseudo and share it with the rest of the block. */
static __inline__ void Call4_020048e8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern const s32 Data_0200e840[];
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* The halfword field at +6 of a scene record is written from an int-sized
 * value; storing a plain constant through the cast would make the compiler
 * fetch a halfword literal from the pool instead. */
static __inline__ void SetPose(u8 *rec, s32 pose)
{
    extern const s32 Data_0200e840[];
    extern u8 Data_03001ebc[];

    *(u16 *)(rec + 6) = pose;
}

s32 Func_02002364(void);      /* site 0x1086 */

void Func_02002224();         /* site 0x1092 */

void Func_0200590c();         /* site 0x109e */

s32 Func_02007520();          /* site 0x107e; story flag test */

void Func_02007556();         /* site 0x108c */

void Func_02007642();         /* site 0x1098 */

void Func_020075f8();         /* site 0x10a6 */

u8 *Func_02007596();          /* site 0x10ac; returns the actor record */

void Func_020075d8();         /* site 0x10be */

void Func_02007606();         /* site 0x10c4 */

void Func_02007618();         /* site 0x10ce */

void Func_020075a4();         /* site 0x10d2 */

s32 Func_0200757c();          /* site 0x10da; story flag test */

s32 Func_0200758c();          /* site 0x10ea; story flag test */

s32 Func_0200759c();          /* site 0x10fa; story flag test */

void Func_0200290e();         /* site 0x1108; the three selected variants share it */

void Func_0200291a();         /* site 0x1114; the default variant has its own site */

/* Contiguous unnamed state-owner run for resource_3b1. */
s32 ResetStagedActorState(struct StagedActor *actor)
{
    InitializeStagedActorState(actor, 1);
    actor->x.value = 0;
    actor->y = 0;
    actor->z.value = 0;
    actor->unknown_24 = 0;
    actor->unknown_28 = 0;
    actor->unknown_2c = 0;
    actor->unknown_3c = 0x80000000;
    actor->unknown_38 = 0x80000000;
    return 0;
}

s32 UpdateStagedActorVerticalPosition(struct StagedActor *actor)
{
    s16 vertical_motion_direction;
    s32 descending_y;
    s32 ascending_y;

    if (actor->vertical_motion_direction != 0) {
        descending_y = (actor->y
            - ((u32)(GetDescendingMotionStep() << 0xF) >> 0x10)) + 0xFFFF8000;
        actor->y = descending_y;
        if (descending_y < 0) {
            vertical_motion_direction = 0;
            goto block_5;
        }
    } else {
        ascending_y = actor->y
            + ((u32)(GetAscendingMotionStep() << 0xF) >> 0x10) + 0x8000;
        actor->y = ascending_y;
        if (ascending_y > 0x80000) {
            vertical_motion_direction = 1;
block_5:
            actor->vertical_motion_direction = vertical_motion_direction;
        }
    }
    return 1;
}

s32 StagedActor_SetAngleByKind(u8 *o)
{
    u32 v = (u32)(Func_020064e6(o) << 6) >> 16;

    if (v == 6) {
        s32 k = 0x3000;
        *(u16 *)(o + 6) = k;
    } else if (v == 9) {
        s32 k = 0x5000;
        *(u16 *)(o + 6) = k;
    }
    return 1;
}

void StagedActor_AdvanceCounter98(u8 *o)
{
    u8 *p = o + 98;
    s32 n = *p + 1;

    *p = n;
    if ((u8)n > 80) {
        *(u16 *)(o + 102) += 1;
    }
}

s32 StagedActor_CountdownUntilPositionUnset(u8 *o)
{
    s32 n = *(s32 *)(o + 76);

    if (n != 0) {
        *(s32 *)(o + 76) = n - 1;
    } else {
        return 1;
    }
    if (*(s32 *)(o + 56) == 0x80000000
        && *(s32 *)(o + 60) == *(s32 *)(o + 56)
        && *(s32 *)(o + 64) == *(s32 *)(o + 60)) {
        return 1;
    }
    return 0;
}

void UpdateActorNineEffectMode(struct EffectRecord *record)
{
    struct SceneActor *actor;

    actor = Func_02006778(9);
    if (record->state != 0)
        return;
    if (actor->mode == 1) {
        record->angle = 0xd000;
        record->active = 1;
        actor->mode = 0;
    } else if (actor->mode == 2) {
        if (record->active != 0)
            Func_0200671c(record, 3);
        record->active = 0;
        actor->mode = 0;
    } else if (actor->mode == 3) {
        record->angle = 0;
        actor->mode = 0;
    }
}

s32 StagedActor_SetReadyState(struct Work_3b1 *work)
{
    struct Rec_3b1 *rec = work->f80;

    work->f89 = 8;
    Func_02006786(work, 0);
    rec->mode9 = 1;
    rec->mode15 = 1;
    work->f35 = (work->f35 & ~1) | 2;
    Func_020068d6(work, 15);
    return 1;
}

/*
 * Overlay resource_3b1. Picks the scene data table for the current scene
 * index, with two arms further narrowed by story flags.
 */
s32 SceneData_SelectTableByWord224(void)
{
    if (Data_02000240[224] == (s32)&Value_0000006f) {
        return (s32)Data_0200e984;
    }
    return (s32)Data_0200e96c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTableEB94(void)
{
    return (s32)&Data_0200eb94;
}

/*
 * The 292-byte owner at 0x0200054c covers the dispatcher, a 23-entry jump
 * table, the case bodies, an alignment halfword and the literal pool. Case
 * order and the shared arms reproduce the reference: 23 shares an arm with
 * 4 while 22 does not, and the 15/17/19 arm skips 16, 18 and 20. 2208 is
 * synthesised in the reference and stays decimal; 0x928 and 0x93e are pool
 * loads.
 */
u8 *SceneData_SelectTableBySceneIndexAndFlags(void)
{
    s16 *tbl = Data_02000240;
    s32 scene = tbl[225];

    switch (scene) {
    case 1:
    case 2:
        if (Func_02006a6a(2208) != 0) {
            return Data_0200f6fc;
        }
        if (Func_02006a78(0x928) != 0 && Func_02006a82(0x93e) == 0) {
            return Data_0200f570;
        }
        return Data_0200f444;
    case 4:
    case 23:
        if (Func_02006a94(0x93e) != 0) {
            return Data_0200fedc;
        }
        return Data_0200f9c0;
    case 5:
        if (Func_02006aa8(2208) != 0) {
            return Data_0200f930;
        }
        if (Func_02006ab6(0x93e) != 0) {
            return Data_0200f984;
        }
        return Data_0200f81c;
    case 15:
    case 17:
    case 19:
        return Data_0200fb58;
    case 21:
        return Data_0200fd44;
    default:
        break;
    }

    return Data_0200f420;
}

void FieldScene_RunScene3b1_02000670(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006b3c();
    Func_02006cc0();
    if (Value1(Func_02006b1e, 0x921) != 0) {
        Call1(Func_02006c30, 0x1dd4);
        Func_02006c48(10, 0);
    } else {
        if (Value1(Func_02006b38, 0x922) != 0) {
            Call1(Func_02006c4a, 0x1d91);
            Value2(Func_02006c5a, 10, 0);
            if (Value2(Func_02006b92, 0, 0) == 0) {
                Func_020025f6();
                goto L_020006ea;
            }
            Func_02006c3c(10, 2);
            Func_02006c84(10, 0);
            Call3(Func_02006ca8, 10, 0xd000, 0);
        } else {
            Call1(Func_02006c88, 0x1d31);
            Func_02006ca0(10, 0);
        }
    }
    L_020006ea:;
    Func_02006bbc();
}

void Dialogue_RunActor12Line(void)
{
    Func_02006bd4();
    Func_02006cba((s32)&Value_00001dd1);
    Func_02006ce2(12, 0);
    Func_02006bee();
}

void FieldScene_RunScene3b1_02000728(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006bf4();
    if (Value1(Func_02006bd2, 0x928) != 0) {
        Call1(Func_02006ce4, 0x1eb2);
        Func_02004fae(8);
        Call3(Func_02006d1e, 8, 0xd000, 60);
        Func_02006cb6(8, 4);
        Func_02004fc8(8);
        Func_02006cc4(8, 3);
    } else {
        if (Value1(Func_02006c0c, 0x925) != 0) {
            Call1(Func_02006d1e_a, 0x1e06);
            Func_02006d36(8, 0);
        } else {
            if (Value1(Func_02006c26, 0x921) != 0) {
                Call1(Func_02006d38, 0x1dcd);
                Func_02006d50(8, 0);
                record = Value1(Func_02006c3e, 0x925);
                if (record != 0) {
                    goto L_020007cc;
                }
                record = Value1(Func_02006c48_a, 0x924);
                if (record == 0) {
                    goto L_020007cc;
                }
                {
                    u16 *target = (u16 *)((*(u8 *volatile *)Data_03001ebc + 0x172));
                    s32 shown = 1;

                    *target = shown;
                }
            } else {
                Call1(Func_02006d6a, 0x1d30);
                Func_02006d82(8, 0);
            }
        }
    }
    L_020007cc:;
    Func_02006c9e();
}

void FieldScene_RunScene3b1_020007f8(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006cc4_a();
    if (Value1(Func_02006ca2, 0x925) != 0) {
        Func_02006d86(8, 2);
        Call1(Func_02006dbc, 0x1e13);
        Func_02005086(8);
        Func_02006dac(8, 0, 10);
        Value2(Func_02006ddc, 8, 0);
        if (Value2(Func_02006d14, 0, 0) == 0) {
            Func_02006cfe(40);
            Func_020050b0(8);
            Value2(Func_020050ce, 8, 0x3000);
            Func_02006e0e(8, 0);
            goto L_0200088e;
        }
        bump_step(2);
        Func_02006e28(8, 0);
        Call3(Func_02006e4c, 8, 0x3000, 0);
    } else {
        Call1(Func_02006e2c, 0x1d4e);
        Func_02006e44(8, 0);
    }
    L_0200088e:;
    Func_02006d60();
}

void Dialogue_ShowLine1E19Or1D50(void)
{
    Func_02006d74();
    if (Func_02006d52((s32)&Value_00000925) != 0) {
        Func_02006e64((s32)&Value_00001e19);
        Func_02006e8c(10, 0);
    } else {
        Func_02006e74((s32)&Value_00001d50);
        Func_02006e8c_a(10, 0);
    }
    Func_02006da8();
}

/*
 * Flag-branched scene setup for overlay resource_3b1. Each callee name
 * refers to that call site's own call word rather than to a shared runtime
 * address.
 */

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee name refers
 * to its own call word rather than to a shared runtime address.
 */

/* Scene setup for resource_3b1: installs actors 10 through 17. */

/*
 * Set up actors 24 and 25 -- resource_3b1. A flat setter sequence with no
 * branches; the owner includes its one literal pool word.
 */

/* The pool word, referenced by address so that it is emitted. */

/*
 * The aliases name the call words encoded in the overlay image, and the
 * declarations are old-style because the call sites vary in arity.
 */

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee slot uses
 * its own local veneer, so the names are per call site and not the shared
 * main-image symbol.
 */
void State_RunFlagBranchedActor8Setup(void)
{
    extern u8 *Data_03001ebc;

    if (Func_02006d94(0x300) != 0) {
        u8 *obj = Func_02001bd8();
        u8 *p;

        Func_02006dca();
        Func_02001a98(obj);
        Func_02006eb6((s32)&Value_00001e9e);
        Func_02005180(8);
        Func_02006e6c(obj, 2);
        p = Func_02006e0a(0);
        if (p != 0) {
            Func_02006e4c_a(obj, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_02006e7a(obj);
        Func_02006e8c_b(obj, 0, 0);
        Func_02006e18();
    } else if (Func_02006df0((s32)&Value_0000092b) != 0) {
        Func_02002184(8, (s32)&Value_00001e78, 0x990);
    } else if (Func_02006e02((s32)&Value_0000092a) != 0) {
        Func_02002184(8, (s32)&Value_00001e78, (s32)&Value_00000917);
    } else if (Func_02006e12((s32)&Value_00000929) != 0) {
        Func_02002184(8, (s32)&Value_00001e78, (s32)&Value_00000935);
    } else {
        Func_02002190(8, (s32)&Value_00001e78, (s32)&Value_0000092c);
    }
}

void Dialogue_RunActorTenFlaggedDialogue(void)
{
    if (Func_02006e5c(0x300) != 0) {
        u8 *o = Func_02001ca0();
        u8 *p;

        Func_02006e92();
        Func_02001b60(o);
        Func_02006f7e((s32)&Value_00001e9f);
        Func_02005248(10);
        Func_02006f34(o, 2);
        p = Func_02006ed2(0);
        if (p != 0) {
            Func_02006f14(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_02006f42(o);
        Func_02006f54(o, 0, 0);
        Func_02006ee0();
    } else if (Func_02006eb8((s32)&Value_0000092b) != 0) {
        Func_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000992);
    } else if (Func_02006ec8((s32)&Value_0000092a) != 0) {
        Func_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000919);
    } else if (Func_02006ed8((s32)&Value_00000929) != 0) {
        Func_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000937);
    } else {
        Func_02002256(10, (s32)&Value_00001e7b, (s32)&Value_0000092e);
    }
}

void Scene_RunActor11FlagDialogue(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    if (Func_02006f28(0x8A0) != 0) {
        Func_02006f58();
        Func_0200708a(11, 0x102);
        Func_02006f60(40);
        Func_0200704e((s32)&Value_00001f47);
        Func_02007066(11, 0);
        Func_02006f82();
    } else if (Func_02006f5c(0x300) != 0) {
        u8 *o = Func_02001da0();
        u8 *p;

        Func_02006f92();
        Func_02001c60(o);
        Func_0200707e((s32)&Value_00001ea0);
        Func_02005348(11);
        Func_02007034(o, 2);
        p = Func_02006fd2(0);
        if (p != 0) {
            Func_02007014(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_02007042(o);
        Func_02007054(o, 0, 0);
        Func_02006fe0();
    } else if (Func_02006fb8((s32)&Value_0000092b) != 0) {
        Func_0200234a(11, (s32)&Value_00001e7e, (s32)&Value_00000993);
    } else if (Func_02006fc8((s32)&Value_0000092a) != 0) {
        Func_0200234a(11, (s32)&Value_00001e7e, (s32)&Value_0000091a);
    } else if (Func_02006fd8((s32)&Value_00000929) != 0) {
        Func_0200234a(11, (s32)&Value_00001e7e, (s32)&Value_00000938);
    } else {
        Func_02002356(11, (s32)&Value_00001e7e, (s32)&Value_0000092f);
    }
}

void Scene_RunScene3b1SequenceA(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 record;

    Func_02007050();
    if (Value1(Func_02007030, 0x300) != 0) {
        rec7 = Value0(Func_02001e74);
        Func_02001d2e();
        Call1_02000b84(Func_0200714c, 0x1ea1);
        Func_02005416(12);
        Func_02007102(rec7, 2);
        record = Value1(Func_020070a0, 0);
        if (record != 0) {
            Func_020070e2(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02007110(rec7);
        Func_02007122(rec7, 0, 0);
    } else {
        Func_02007164(12, 2);
        Func_020070aa(20);
        Call1_02000b84(Func_02007198, 0x1e81);
        Value2(Func_020071a8, 12, 0);
        if (Value2(Func_020070e0, 0, 0) == 0) {
            Func_02005476(12);
            Func_02007162(12, 2);
            record = Value1(Func_02007100, 0);
            if (record != 0) {
                Func_02007142(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_02007170(12);
            Func_02007182(12, 0, 0);
            Call1_02000b84(Func_020070ea, 0x300);
            if (Value1(Func_020070e8, 0x92b) != 0) {
                Call1_02000b84(Func_020070fa, 0x994);
                goto L_02000c9a;
            }
            if (Value1(Func_020070fa_a, 0x92a) != 0) {
                Call1_02000b84(Func_0200710c, 0x91b);
                goto L_02000c9a;
            }
            if (Value1(Func_0200710c_a, 0x929) != 0) {
                Call1_02000b84(Func_0200711e, 0x939);
                goto L_02000c9a;
            }
            Call1_02000b84(Func_02007128, 0x930);
        } else {
            bump_step(1);
            Func_02005504(12);
        }
    }
    L_02000c9a:;
    Func_0200716c();
}

void Scene_RunScene3b1SequenceB(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 record;

    Func_02007194();
    if (Value1(Func_02007174, 0x300) != 0) {
        rec7 = Value0(Func_02001fb8);
        Func_02001e72();
        Call1_02000cc8(Func_02007290, 0x1ea2);
        Func_0200555a(9);
        Func_02007246(rec7, 2);
        record = Value1(Func_020071e4, 0);
        if (record != 0) {
            Func_02007226(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ((void (*)())Func_02007254)(rec7);
        Func_02007266(rec7, 0, 0);
    } else {
        Call1_02000cc8(Func_020072ce, 0x1e84);
        ((void (*)())Func_020072f0)(9, 0, 60);
        Func_020072b8(9, 1);
        Value2(Func_020072f0_a, 9, 0);
        if (Value2(Func_02007228, 0, 0) == 0) {
            Func_020055be(9);
            Func_020072aa(9, 2);
            record = Value1(Func_02007248, 0);
            if (record != 0) {
                Func_0200728a(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_020072b8_a(9);
            Func_020072ca(9, 0, 0);
            Call1_02000cc8(Func_02007232, 0x300);
            if (Value1(Func_02007230, 0x92b) != 0) {
                Call1_02000cc8(Func_02007242, 0x991);
                goto L_02000de0;
            }
            if (Value1(Func_02007242_a, 0x92a) != 0) {
                Call1_02000cc8(Func_02007254_a, 0x918);
                goto L_02000de0;
            }
            if (Value1(Func_02007254_b, 0x929) != 0) {
                Call1_02000cc8(Func_02007266_a, 0x936);
                goto L_02000de0;
            }
            Call1_02000cc8(Func_0200726e, 0x92d);
        } else {
            bump_step(1);
            Func_0200564a(9);
        }
    }
    L_02000de0:;
    Func_020072b2();
}

void Dialogue_RunActorThirteenFlag300Branch(void)
{
    if (Func_020072bc(0x300) != 0) {
        u8 *o = Func_02002100();
        u8 *p;

        Func_020072f2();
        Func_02001fc0(o);
        Func_020073de((s32)&Value_00001ea3);
        Func_020056a8(13);
        Func_02007394(o, 2);
        p = Func_02007332(0);
        if (p != 0) {
            Func_02007374(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_020073a2(o);
        Func_020073b4(o, 0, 0);
        Func_02007340();
    } else if (Func_02007318((s32)&Value_0000092b) != 0) {
        Func_020026aa(13, (s32)&Value_00001e88, (s32)&Value_00000995);
    } else if (Func_02007328((s32)&Value_0000092a) != 0) {
        Func_020026aa(13, (s32)&Value_00001e88, (s32)&Value_0000091c);
    } else if (Func_02007338((s32)&Value_00000929) != 0) {
        Func_020026aa(13, (s32)&Value_00001e88, (s32)&Value_0000093a);
    } else {
        Func_020026b6(13, (s32)&Value_00001e88, (s32)&Value_00000931);
    }
}

void Scene_RunFlag300BranchDialogue(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    if (Func_02007388(0x300) != 0) {
        u8 *o = Func_020021cc();
        u8 *p;

        Func_020073be();
        Func_0200208c(o);
        Func_020074aa((s32)&Value_00001ea4);
        Func_02005774(14);
        Func_02007460(o, 2);
        p = Func_020073fe(0);
        if (p != 0) {
            Func_02007440(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_0200746e(o);
        Func_02007480(o, 0, 0);
        Func_0200740c();
    } else if (Func_020073e4((s32)&Value_0000092b) != 0) {
        Func_02002776(14, (s32)&Value_00001e8b, (s32)&Value_00000996);
    } else if (Func_020073f4((s32)&Value_0000092a) != 0) {
        Func_02002776(14, (s32)&Value_00001e8b, (s32)&Value_0000091d);
    } else if (Func_02007404((s32)&Value_00000929) != 0) {
        Func_02002776(14, (s32)&Value_00001e8b, (s32)&Value_0000093b);
    } else {
        Func_02002782(14, (s32)&Value_00001e8b, (s32)&Value_00000932);
    }
}

void Scene_RunActor15FlagDialogue(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    if (Func_02007454(0x300) != 0) {
        u8 *o = Func_02002298();
        u8 *p;

        Func_0200748a();
        Func_02002158(o);
        Func_02007576((s32)&Value_00001ea5);
        Func_02005840(15);
        Func_0200752c(o, 2);
        p = Func_020074ca(0);
        if (p != 0) {
            Func_0200750c(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_0200753a(o);
        Func_0200754c(o, 0, 0);
        Func_020074d8();
    } else if (Func_020074b0((s32)&Value_0000092b) != 0) {
        Func_02002842(15, (s32)&Value_00001e8e, (s32)&Value_00000997);
    } else if (Func_020074c0((s32)&Value_0000092a) != 0) {
        Func_02002842(15, (s32)&Value_00001e8e, (s32)&Value_0000091e);
    } else if (Func_020074d0((s32)&Value_00000929) != 0) {
        Func_02002842(15, (s32)&Value_00001e8e, (s32)&Value_0000093c);
    } else {
        Func_0200284e(15, (s32)&Value_00001e8e, (s32)&Value_00000933);
    }
}

/*
 * With story flag 0x300 set, opens a scripted sequence, creates a local
 * object, shows message 0x1ea6, configures it as slot 2, moves it onto actor
 * 0's signed halfword coordinates when actor 0 exists, then releases and
 * closes the sequence.  Otherwise flags 0x92b, 0x92a and 0x929 select the
 * setup call's third argument.  The 204-byte owner includes an alignment
 * halfword and its nine pool words.
 */
void Scene_RunActor16FlagDialogue(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    s32 obj;
    u8 *actor;

    if (Func_02007520(0x300) != 0) {
        obj = Func_02002364();
        Func_02007556();
        Func_02002224(obj);
        Func_02007642(0x1ea6);
        Func_0200590c(16);
        Func_020075f8(obj, 2);

        actor = Func_02007596(0);
        if (actor != 0) {
            Func_020075d8(obj, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Func_02007606(obj);
        Func_02007618(obj, 0, 0);
        Func_020075a4();
    } else {
        if (Func_0200757c(0x92b) != 0) {
            Func_0200290e(16, 0x1e91, 0x998);
        } else if (Func_0200758c(0x92a) != 0) {
            Func_0200290e(16, 0x1e91, 0x91f);
        } else if (Func_0200759c(0x929) != 0) {
            Func_0200290e(16, 0x1e91, 0x93d);
        } else {
            Func_0200291a(16, 0x1e91, 0x934);
        }
    }
}

struct SceneActor_02001144 *FindActorNearPosition(s32 x, s32 y)
{
    struct SceneWork *work;
    struct SceneActor_02001144 **actor;
    struct SceneActor_02001144 *current;
    u32 i;
    s32 actor_x;
    s32 actor_y;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;

    work = *(struct SceneWork **)0x03001ebc;
    i = 8;
    left = x - 12;
    right = x + 12;
    top = y - 12;
    bottom = y + 12;
    actor = work->actors;
    while (i <= 65) {
        current = *actor++;
        actor_x = current->x;
        actor_y = current->y;
        if (left < actor_x && right > actor_x &&
            top < actor_y && bottom > actor_y)
            return current;
        i++;
    }
    return 0;
}

/* Actor placement check for resource_3b1. */

/* Bucket offsets, packed as {s16 hi; s16 lo} per entry. */

/*
 * Offset obj+10 and obj+18 by the bucket's packed hi/lo pair, test the
 * candidate point, and on success pack {x << 16, obj+12, z << 16} into a
 * stack struct for a second check.  Returns 1 only if both checks pass.  The
 * owner includes its one pool word, the bucket table base.  Callees are named
 * by the address their call site computes, not by a runtime address.
 */
s32 Actor_CheckBucketOffsetPoint(s32 bucket)
{
    u8 *obj = Func_02007772(0);
    u32 ofs = Data_0200e668[bucket];
    s32 x = *(s16 *)(obj + 10) + ((s32)ofs >> 16);
    s32 z = *(s16 *)(obj + 18) + (s32)(s16)ofs;

    if (Func_020023f0(x, z) != 0) {
        return 0;
    }

    {
        s32 point[3];
        point[0] = x << 16;
        point[1] = *(s32 *)(obj + 12);
        point[2] = z << 16;

        if (Func_0200773c(obj, point) != 0) {
            return 0;
        }
    }

    return 1;
}

/*
 * Level selection from scene flags, overlay resource_3b1. Each callee name
 * refers to its own call word rather than to a shared runtime address.
 */

/*
 * Actor 8 flag setup for overlay resource_3b1. Each callee name refers to
 * its own call word rather than to a shared runtime address.
 */

/* Scene state helper for overlay resource_3b1. */

/*
 * Picks a level from the highest flag that is set and applies it. The
 * 72-byte owner at 0x020012dc includes its three pool words, which are the
 * addresses taken as Value_0000092b, Value_0000092a and Value_00000929.
 */
s32 State_ApplyLevelFromFlags(void)
{
    s32 ret = 0;

    if (Func_02007784((s32)&Value_0000092b) != 0) {
        ret = 3;
    } else if (Func_02007792((s32)&Value_0000092a) != 0) {
        ret = 2;
    } else if (Func_020077a0((s32)&Value_00000929) != 0) {
        ret = 1;
    }

    return Func_020062b6(ret, 1);
}

/* Story selector owner at 0x02001324, 84 bytes; eight calls. Per-site call
 * veneers (raw asm confirms each callee slot uses a distinct local stub
 * even across the three near-identical "twin" owners at 0x1324/1378/13cc). */
void Dialogue_ShowLine1ECETo1ED0(void)
{
    Func_020077f0();
    if (Func_020077ce(0x92c)) Func_020078e0(0x1ece);
    else if (Func_020077e0(0x935)) Func_020078f2(0x1ecf);
    else Func_020078fa(0x1ed0);
    Func_02007912(0x12, 0); Func_0200782e();
}

/* Story selector owner at 0x02001378, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x13cc with distinct local stub addresses). */
void Dialogue_RunActor19TwoFlagLineA(void)
{
    Func_02007844();
    if (Func_02007822(0x92d)) Func_02007934(0x1ece);
    else if (Func_02007834(0x936)) Func_02007946(0x1ecf);
    else Func_0200794e(0x1ed0);
    Func_02007966(0x13, 0); Func_02007882();
}

/* Story selector owner at 0x020013cc, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x1378 with distinct local stub addresses). */
void Dialogue_RunActor20TwoFlagLine(void)
{
    Func_02007898();
    if (Func_02007876(0x92e)) Func_02007988(0x1ece);
    else if (Func_02007888(0x937)) Func_0200799a(0x1ecf);
    else Func_020079a2(0x1ed0);
    Func_020079ba(0x14, 0); Func_020078d6();
}

/* Story selector owner at 0x02001420, 60 bytes; six calls. */
void Dialogue_ShowLine1ED1Or1ED2(void)
{
    Func_020078ec();
    if (Func_020078ca(0x92f)) Func_020079dc(0x1ed1);
    else Func_020079e4(0x1ed2);
    Func_020079fc(21, 0); Func_02007918();
}

/* Story selector owner at 0x0200145c, 84 bytes; eight calls. */
void Dialogue_RunActor22TwoFlagLine(void)
{
    Func_02007928();
    if (Func_02007908(0x930)) Func_02007a1a(0x1ece);
    else if (Func_0200791a(0x939)) Func_02007a2c(0x1ecf);
    else Func_02007a34(0x1ed0);
    Func_02007a4c(22, 0); Func_02007968_a();
}

/* Story selector owner at 0x020014b0, 84 bytes; eight calls. */
void Dialogue_RunActor23BranchedDialogue(void)
{
    Func_0200797c();
    if (Func_0200795a(0x931)) Func_02007a6c(0x1ece);
    else if (Func_0200796c(0x93a)) Func_02007a7e(0x1ecf);
    else Func_02007a86(0x1ed0);
    Func_02007a9e(23, 0); Func_020079ba_a();
}

/* Story selector owner at 0x02001504, 84 bytes; eight calls. */
void Dialogue_RunActor24BranchedDialogue(void)
{
    Func_020079d0();
    if (Func_020079ae(0x932)) Func_02007ac0(0x1ece);
    else if (Func_020079c0(0x93b)) Func_02007ad2(0x1ecf);
    else Func_02007ada(0x1ed0);
    Func_02007af2(24, 0); Func_02007a0e();
}

/* Story selector owner at 0x02001558, 60 bytes; six calls. */
void Dialogue_RunActor25FlaggedLine(void)
{
    Func_02007a24();
    if (Func_02007a02(0x933)) Func_02007b14(0x1ed1);
    else Func_02007b1c(0x1ed2);
    Func_02007b34(25, 0); Func_02007a50();
}

/* Second-phase story selector at 0x02001594, 84 bytes; eight calls. */
void Dialogue_RunActor18TwoFlagLine(void)
{
    Func_02007a60();
    if (Func_02007a3e(0x92c)) Func_02007b50(0x1edb);
    else if (Func_02007a50_a(0x935)) Func_02007b62(0x1edc);
    else Func_02007b6a(0x1edd);
    Func_02007b82(18, 0); Func_02007a9e_a();
}

/* Second-phase story selector at 0x020015e8, 84 bytes; eight calls. */
void Dialogue_RunActor19TwoFlagLineB(void)
{
    Func_02007ab4();
    if (Func_02007a92(0x92d)) Func_02007ba4(0x1edb);
    else if (Func_02007aa4(0x936)) Func_02007bb6(0x1edc);
    else Func_02007bbe(0x1edd);
    Func_02007bd6(19, 0); Func_02007af2_a();
}

/* Second-phase story selector at 0x0200163c, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor20(void)
{
    Func_02007b08();
    if (Func_02007ae6(0x92e)) Func_02007bf8(0x1edb);
    else if (Func_02007af8(0x937)) Func_02007c0a(0x1edc);
    else Func_02007c12(0x1edd);
    Func_02007c2a(20, 0); Func_02007b46();
}

/* Second-phase story selector at 0x02001690, 60 bytes; six calls. */
void Dialogue_RunActor21FlaggedLine(void)
{
    Func_02007b5c();
    if (Func_02007b3a(0x92f)) Func_02007c4c(0x1ede);
    else Func_02007c54(0x1edf);
    Func_02007c6c(21, 0); Func_02007b88();
}

/* Second-phase story selector at 0x020016cc, 84 bytes; eight calls. */
void Dialogue_RunActor22BranchedDialogue(void)
{
    Func_02007b98();
    if (Func_02007b78(0x930)) Func_02007c8a(0x1edb);
    else if (Func_02007b8a(0x939)) Func_02007c9c(0x1edc);
    else Func_02007ca4(0x1edd);
    Func_02007cbc(22, 0); Func_02007bd8();
}

/* Second-phase story selector at 0x02001720, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor23(void)
{
    Func_02007bec();
    if (Func_02007bca(0x931)) Func_02007cdc(0x1edb);
    else if (Func_02007bdc(0x93a)) Func_02007cee(0x1edc);
    else Func_02007cf6(0x1edd);
    Func_02007d0e_a(23, 0); Func_02007c2a_a();
}

/* Second-phase story selector at 0x02001774, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor24(void)
{
    Func_02007c40();
    if (Func_02007c1e(0x932)) Func_02007d30(0x1edb);
    else if (Func_02007c30(0x93b)) Func_02007d42(0x1edc);
    else Func_02007d4a(0x1edd);
    Func_02007d62(24, 0); Func_02007c7e();
}

/* Second-phase story selector at 0x020017c8, 60 bytes; six calls. */
void Dialogue_ShowLine1EDEOr1EDF(void)
{
    Func_02007c94();
    if (Func_02007c72(0x933)) Func_02007d84(0x1ede);
    else Func_02007d8c_a(0x1edf);
    Func_02007da4(25, 0); Func_02007cc0();
}

void Scene_RunPrimarySequence(s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02007cd6();
    Func_02007dbc(a1);
    Value2(Func_02007dcc, a0, 0);
    if (Value2(Func_02007d04, 0, 0) == 0) {
        Func_0200609a(a0);
        Func_02007d86(a0, 2);
        record = Value1(Func_02007d24, 0);
        if (record != 0) {
            Func_02007d66(a0, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02007d94(a0);
        Func_02007da6(a0, 0, 0);
        Call1(Func_02007d0e, 0x300);
        Func_02007d14(a2);
    } else {
        bump_step(1);
        Func_020060f0(a0);
    }
    Func_02007d58();
}

void Scene_RunScene3b1SequenceC(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    record = Func_02007d82(0);
    if ((u32)((*(volatile u16 *)(record + 6) + -0x2000) << 16) > -0x40000000) {
        if (Value1(Func_02007d50, 0x928) != 0) {
            if (Value1(Func_02007d5a, 0x93e) == 0) {
                Func_02007f1c(17);
                goto L_0200194e;
            }
        }
        Func_02007f24(15);
    } else {
        Func_02007d9a();
        if (Value1(Func_02007d78, 0x93e) != 0) {
            Call1(Func_02007e8a, 0x1f81);
        } else {
            if (Value1(Func_02007d8c, 0x8a0) != 0) {
                Call1(Func_02007e9e, 0x1f48);
            } else {
                if (Value1(Func_02007d9e, 0x928) != 0) {
                    Call1(Func_02007eb0, 0x1f7f);
                } else {
                    if (Value1(Func_02007db0, 0x925) != 0) {
                        Call1(Func_02007ec2, 0x1f7d);
                    } else {
                        Call1(Func_02007eca, 0x1f7b);
                    }
                }
            }
        }
        if (Value1(Func_02007dc8, 0x928) != 0) {
            record = Value1(Func_02007dd2, 0x93e);
            if (record != 0) {
                goto L_02001942;
            }
            Func_02007ef6(17, 0);
        } else {
            L_02001942:;
            Func_02007f00(15, 0);
        }
        Func_02007e1c();
    }
    L_0200194e:;
}

void Scene_RunScene3b1SequenceD(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1_02001a08(Func_02007eae, 0x301) != 0) {
        Func_02007ede();
        Func_02007ff4(8);
        Call3(Func_02007eb6, 0x1e48, 1, 8);
        Call3(Func_02007f28, 0, 0x19999, 0xcccc);
        Call3(Func_02007f6c, 0, 0x198, 134);
        Value2(Func_020062c6, 0, 0x4000);
        Func_02007f1a();
    }
}

/*
 * Gated on flag 0x922.  Every callee slot has its own local call stub;
 * Func_02006336 and Func_02006336_a are the same stub declared twice without
 * a prototype, because the two call sites pass different argument counts.
 */

/*
 * Scene setup for resource_3b1.  The 212-byte owner at 0x02001a60 includes
 * the alignment halfword at 0x02001b1a and the six pool words that follow it,
 * ending before the next owner's prologue at 0x02001b34.
 */
void Scene_RunFlagGatedThreeActorSetup(void)
{
    extern u8 *Data_03001ebc;

    if (Func_02007f06(0x922) == 0)
        return;

    Func_02007f36();
    Func_020080ba();
    Func_02008072(0x19999, 0x3333);
    Func_02006336(0xe0 << 17, -1, 0x027e0000, 0x10000028u);
    Func_02008038(0x1d26);

    Func_02006302(8);
    Func_02006308(10);
    Func_02006326(8, 0x3000);
    Func_02006318(8);
    Func_02006336_a(10, 0xd000);
    Func_02006328(10);
    Func_02006346(9, 0x5000);
    Func_02006338(9);

    Func_020080a6(8, 0, 20);
    Func_02006348(8);
    Func_02006366(9, 0x8000);
    Func_02006358(9);
    Func_0200635e(10);
    Func_02006364(8);
    Func_02006382(10, 0xb000);
    Func_02006374(8);

    Func_02007fb8(0x920);
    Func_02007fe4();
}

void Scene_RunThreeActorPresentation(void)
{
    s32 request_a;
    s32 request_b;
    s32 action;

    if (Value1(Func_02007fe6, 0x911) == 0) {
    } else {
        Func_02008018();
        Func_0200819c();
        Call2(Func_02008154, 0x26666, 0x4ccc);
        Func_02006418( 0x5b70000, -1, 0x1d00000, 0x10000014);
        Func_020080f4(13, 1);
        Call1_02001b34(Func_02008122, 0x1d56);
        Func_020063f0(0x200d);
        Func_02006412(12, 0xd000);
        request_a = 0x800c;
        Call3(Func_02008188, 12, 0x102, 20);
        Func_02008120(12, 2);
        Func_0200641a(request_a);
        Func_02008136(14, 1);
        Call3(Func_02008180, 0xa00e, 0, 20);
        Func_02006448(12, 0);
        request_b = 0xa00e;
        Call3(Func_020081bc, 12, 0x101, 40);
        Call3(Func_020081c6, 14, 0x103, 40);
        Func_0200815e(14, 3);
        Func_02006458(request_b);
        Call2(Func_020081e6, 12, 0x102);
        Func_020080bc(40);
        Func_0200817c(12, 3);
        Func_02006476(request_a);
        Func_02008192(14, 1);
        Func_02006484(request_b);
        Func_020064a6(14, 0xb000);
        Func_02006498(request_b);
        Func_020064b4_a(12, 0xd000);
        Call3(Func_02008228, 12, 0x100, 30);
        Func_020081c0(12, 1);
        Func_020064ba(request_a);
        Func_020081b6(13, 4);
        Func_020064c8(0x200d);
        Func_020081dc(13, 2);
        Func_020064d6(0x200d);
        Func_020081c2(12, 4);
        Func_020064e4(request_a);
        Func_020081e0(14, 4);
        Func_020064f2(request_b);
        Func_02006514(14, 0x8000);
        Func_02008214(14, 2);
        Func_02008266(request_b, 0, 20);
        Func_02008280(12, 0, 0);
        Call3(Func_020082a4, 12, 0x102, 80);
        Func_02008286(request_a, 0, 20);
        Call3(Func_020082b8, 14, 0x103, 0);
        Call3(Func_020082c2, 13, 0x103, 60);
        Func_0200825a(14, 2);
        Func_02006554(request_b);
        Func_02006570(14, 0xb000);
        Func_02008278(14, 1);
        Func_0200656a(request_b);
        Func_0200658c(13, 0x3000);
        Call3(Func_020082fe, 13, 0x101, 0);
        Call3(Func_02008308, 12, 0x101, 60);
        Func_020082a8(13, 1);
        Func_0200659a(13);
        Call3(Func_02008320, 14, 0x103, 40);
        Func_020082c0(14, 1);
        Func_020065b2(request_b);
        Func_02008320_a(12, 0xd000, 0);
        Call3(Func_0200832c, 13, 0x5000, 40);
        Call3(Func_02008336, 12, 0, 0);
        Func_020065ee(13, 0x3000);
        Func_020082f6(12, 2);
        Func_02008340(request_a, 0, 20);
        Call3(Func_0200835c, 14, 0x4000, 40);
        Func_020065fe(request_b);
        Func_02008312(12, 2);
        Func_02008322(13, 2);
        Func_02008268(60);
        Func_02008330(13, 1);
        Func_02006622(13);
        Func_0200831e(14, 3);
        Func_02006630(request_b);
        Call3(Func_020083b8, 12, 0x102, 40);
        Call2(Func_02008358, 12, 2);
        Func_0200664a(request_a);
        Func_02008346(13, 3);
        Func_02006658(13);
        Func_020083c6(14, 0xb000, 40);
        Func_0200834e(14, 3);
        Func_02008366(13, 3);
        Call3(Func_02008308_a, 14, 0x19999, 0xcccc);
        Call3(Func_02008312_a, 13, 0x19999, 0xcccc);
        action = (s32)Data_0200e6a8;
        Func_02008324(14, action);
        Func_0200832c_a(13, action);
        Func_020082f2(20);
        Call3(Func_0200840e, 12, 0x4000, 0);
        Call3(Func_02008340_a, 0, 0x26666, 0x13333);
        *(u8 *)(Func_02008336_a(0) + 90) &= 254;
        Call3(Func_02008394, 0, 184, 0x208);
        Func_0200832a(1);
        {
            u8 *record = Func_02008358_a(0);
            u32 flag = 1;

            flag = flag | record[90];
            record[90] = (u8)flag;
        }
        Func_02008454(0, 0x8000, 20);
        Call3(Func_02008460, 0, 0x4000, 0);
        Func_0200840a(12, 4, 20);
        Call3(Func_02008476, 0, 0xa000, 20);
        Func_02008426(12, 2);
        Func_02006720(12);
        Call3(Func_020083b6, 12, 0x19999, 0xcccc);
        Func_020083c6_a(12, action);
        Func_0200838c(40);
        Call3(Func_020084a8, 0, 0x4000, 0);
        Func_020083e6(12);
        Call1_02001b34(Func_0200838c_a, 0x922);
        Func_020083b8_a();
    }
}

void Scene_RunExtendedActorChoreography(void)
{
    s32 request_a;
    s32 request_b;
    s32 request_c;

    Func_020085ae(28);
    Call2(Func_0200854e, 0x26666, 0x4ccc);
    Func_02006814(0x1c80000, -1, 0x2880000, 0x10000014);
    Func_020084f0(9, 1);
    Call1_02001f3c(Func_0200851e, 0x1d93);
    Func_020067e8(9);
    Call3(Func_02008558, 0, 0xd000, 0);
    Call3(Func_02008564, 10, 0xd000, 0);
    Func_0200856e(11, 0, 0);
    Call3(Func_0200857a, 12, 0x3000, 0);
    Call3(Func_02008586, 13, 0x8000, 40);
    Call3(Func_020085a8, 9, 0x103, 40);
    Func_02008540(9, 2);
    Func_0200683a(9);
    Func_020085a8_a(12, 0, 0);
    Call3(Func_020085b4, 11, 0xd000, 0);
    Call3(Func_020085c0, 13, 0xd000, 20);
    Func_02008578(11, 1);
    Func_0200686e(0x100b);
    Call3(Func_020085f6, 13, 0x102, 20);
    Func_0200858e(13, 2);
    Func_02006888(13);
    Call3(Func_0200860e, 9, 0x105, 60);
    Func_02006898(9);
    Call3(Func_02008620, 12, 0x104, 20);
    Call1_02001f3c(Func_020068aa, 0x900c);
    Func_020085c6(8, 1);
    Func_020085a0(8, 3);
    Func_020068c4(8);
    Func_020068e0(12, 0x3000);
    Call1_02001f3c(Func_020068d2, 0x900c);
    Func_020068f4(11, 0xb000);
    Func_020085dc(11, 3);
    Func_02008542(10);
    Func_0200860a(13, 1);
    Func_020085f2(13, 3);
    Func_02006904(13);
    Call3(Func_02008674, 13, 0x8000, 0);
    Call3(Func_02008680, 12, 0x5000, 0);
    Func_0200693e(11, 0x5000);
    Call3(Func_020085c0_a, 13, 0x6666, 0x3333);
    Call3(Func_020085ca, 12, 0xcccc, 0x6666);
    Call3(Func_02008608, 12, 0x1bc, 0x29c);
    Call3(Func_0200861e, 13, 0x1d8, 0x29c);
    Func_02008634_a(12);
    Func_0200864c(12, 1);
    Func_020085c2(80);
    Func_02006990(12, 0xd000);
    Call3(Func_02008702, 12, 0x101, 60);
    Func_020086a2(11, 1);
    Func_020085e8(20);
    Call3(Func_020086f2, 0x400b, 0, 40);
    Func_020086ba(11, 2);
    Func_02008714(11, 0xd000, 0);
    Func_020069b6(0x100b);
    Func_02008724(12, 0xd000, 0);
    Call3(Func_02008746, 9, 0x101, 60);
    Func_020086b6_a(11, 4);
    Func_0200862c_a(20);
    Func_020069de(0x100b);
    Func_020086ca(9, 3);
    Func_020069ec(9);
    Func_0200875a(13, 0xd000, 0);
    Call2(Func_02008784, 13, 0x102);
    Func_0200870e(13, 2, 20);
    Func_02006a10(13);
    Func_0200870c(9, 3);
    request_a = 0x100c;
    Func_02006a20(9);
    Func_02006a3c(11, 0xd000);
    Func_02008744(12, 1);
    Func_02006a36(request_a);
    Call3(Func_020087bc, 8, 0x105, 40);
    Func_0200872c(8, 3);
    Func_02006a4e(8);
    Call3(Func_020087d6, 13, 0x102, 40);
    Func_02008768(13, 4, 0);
    Func_02006a6a_a(13);
    Func_02008756(9, 3);
    Func_02006a78_a(9);
    Func_02008794(11, 1);
    Func_02006a86(0x100b);
    Call3(Func_0200880e_a, 8, 0x102, 40);
    Func_020087e6(8, 0);
    Func_020087ae(11, 2);
    Func_02008800(0x100b, 0, 40);
    Call3(Func_02008834, 9, 0x100, 0);
    Func_02008826(9, 0x5000, 20);
    Func_020087d6_a(9, 2);
    Func_02008828(9, 0, 20);
    Func_020087c0(11, 3);
    Func_02008736(20);
    Call3(Func_0200886a, 12, 0x100, 40);
    Func_02008802(12, 2);
    request_b = 0x400b;
    Func_02006afe(request_a);
    Func_0200886c(11, 0x5000, 20);
    Func_02006b0e(request_b);
    Func_02008822(12, 2);
    Func_02006b1c(request_a);
    Func_02008818(11, 3);
    Func_02008840(11, 1);
    Func_02006b32(request_b);
    Call3(Func_020088ba, 12, 0x102, 60);
    Func_0200885a_a(9, 1);
    Func_02006b4c(9);
    Call3(Func_020088d2, 11, 0x101, 40);
    Func_020088c4(11, 0xd000, 20);
    Func_0200884c(9, 3);
    Func_02006b6e(9);
    Call3(Func_020088f4, 11, 0x103, 20);
    Func_0200888c(11, 2);
    Func_02006b86(0x100b);
    Call3(Func_0200890e, 9, 0x108, 40);
    Func_02006b98(9);
    Func_020088b4(8, 1);
    Func_0200888c_a(8, 3);
    Func_02006bae(8);
    Func_02008958(9, 0xd000, 40);
    Func_02006bfa(9);
    Func_02008916(12, 1);
    Func_0200885c(20);
    Func_02006c0e(request_a);
    Func_0200897c(11, 0x5000, 0);
    Func_02008986(9, 0x5000, 0);
    Call3(Func_02008992, 13, 0x8000, 0);
    Call3(Func_0200899e, 0, 0xc000, 0);
    Func_020089a8(10, 0xb000, 40);
    Func_02008960(11, 1);
    Func_02006c52(request_b);
    Func_0200893e(12, 3);
    Func_020089b8(request_a, 0, 20);
    Func_02008980(9, 2);
    Func_02006c72(9);
    Call3(Func_020089fa, 12, 0x108, 40);
    Func_0200896a(12, 3);
    Func_02006c8c(request_a);
    Func_02008988(8, 3);
    Func_02006c9a(8);
    Call3(Func_02008a0a, 8, 0x8000, 20);
    Func_02008aa0(19);
    Func_020089c0(8, 2);
    request_c = 0x8008;
    Call3(Func_02008a3e, 8, 0x100, 80);
    Func_02006cc8(request_c);
    Call3(Func_02008a4e, 12, 0x101, 0);
    Call3(Func_02008a58, 11, 0x101, 0);
    Call3(Func_02008a62, 13, 0x101, 0);
    Call3(Func_02008a6c, 10, 0x101, 0);
    Call3(Func_02008a76, 0, 0x101, 40);
    Func_02008a68(12, 0xd000, 0);
    Func_02008a72(11, 0xd000, 0);
    Func_02008a7c(13, 0xb000, 0);
    Func_02008a86(10, 0xb000, 0);
    Call3(Func_02008a92, 0, 0xc000, 40);
    Call2(Func_02008abc, 8, 0x102);
    Func_02008a46(8, 4, 40);
    Func_02008a56(8, 2);
    Func_02008a9e(request_c, 0);
    Call3(Func_020089e8, 8, 0x19999, 0xcccc);
    Call3(Func_02008a2a, 8, 0x1db, 0x256);
    Call3(Func_02008ad6, 8, 0x8000, 0);
    Call3(Func_02008a0c, 9, 0x10000, 0x8000);
    Call3(Func_02008a50, 9, 0x1ce, 0x26a);
    Func_02006da8_a(9, 0xb000);
    Call3(Func_02008b1c, 9, 0x100, 40);
    Func_02008ab4(9, 2);
    Call1_02001f3c(Func_02006dae, 0x8009);
    Call3(Func_02008b34, 11, 0x101, 60);
    Func_02006dbe(11);
    Call3(Func_02008b46, 12, 0x102, 20);
    Func_02006dd0(request_a);
    Call3(Func_02008b56, 8, 0x103, 20);
    Func_02008ae8(8, 4, 0);
    Func_02008b52(8, 0x5000, 20);
    Func_02006df4(8);
    Func_02008bee(28);
    Func_02008b0e_a(8, 3);
    Func_02006e08(8);
    Call3(Func_02008b8e, 13, 0x101, 60);
    Func_02006e18_a(13);
    Func_02006e34(8, 0x3000);
    Func_02008b1c_a(8, 4);
    Func_02006e2e(8);
    Call3(Func_02008b00, 12, 0x1bc, 0x274);
    Func_02006e58(12, 0xd000);
    Call1_02001f3c(Func_02006e4a, 0x900c);
    Func_02008bb8(8, 0x5000, 20);
    Func_02008b50(8, 3);
    Func_02006e62(8);
    Call3(Func_02008bea, 11, 0x102, 60);
    Func_02006e74_a(0x100b);
    Call3(Func_02008bfa, 13, 0x107, 40);
    Func_02008b92(13, 2);
    Func_02006e8c_c(13);
    Func_02006ea8(9, 0x3000);
    Func_02008b90(9, 4);
    Call1_02001f3c(Func_02006ea2, 0x1009);
    Func_02006ebe(12, 0);
    Func_02008bc6(8, 1);
    Func_02006eb8_a(8);
    Func_02008c26(11, 0, 0);
    Call3(Func_02008c48, 12, 0x105, 0);
    Call3(Func_02008c52, 9, 0x105, 60);
    Call2(Func_02008c6a, 0x13333, 0x2666);
    Func_02006f32(0x1d00000, -1, 0x2a80000, 0x10000000);
    Func_02008c0e(10, 1);
    Func_02006f16(10, 0);
    Func_02006f08(10);
    Func_02008c76(0, 0, 0);
    Call3(Func_02008c9a, 10, 0x102, 40);
    Func_02006f24(10);
    Call3(Func_02008c94, 10, 0x8000, 20);
    Call3(Func_02008cb8, 10, 0x100, 0);
    Func_02008c4a(10, 4, 40);
    Func_02006f4c(10);
    Func_02008c68(10, 1);
    Func_02006f5a(10);
    Call3(Func_02008bf4, 13, 0x10000, 0x8000);
    Call3(Func_02008c30, 13, 0x1b6, 0x293);
    Call3(Func_02008ce4, 8, 0x8000, 0);
    Func_02008cee(9, 0xb000, 0);
    Func_02008cf8(12, 0x3000, 0);
    Func_02008d02(11, 0xb000, 0);
    Func_02008d98(17);
    Call3(Func_02008c3e, 10, 0x10000, 0x8000);
    Call3(Func_02008c82, 10, 0x1e8, 0x2ae);
    Func_02008d2c(10, 0xb000, 0);
    Func_02008ca2(13);
    Func_02008cba(13, 1);
    Func_02008d44(13, 0xd000, 0);
    Func_02008dc8();
    Call1_02001f3c(Func_02008c26_a, 0x921);
}

void Scene_RunBranchingActorPresentation(void)
{
    u8 *record;
    s32 request_a;
    s32 request_b;
    s32 value;
    s32 request_c;
    s32 action;

    Func_02008caa();
    Func_02008e2e();
    Func_02008d78(8, 0, 0);
    Call3(Func_02008de4, 8, 0x100, 40);
    request_a = 0x1008;
    Func_02008d7e(8, 3);
    Call1_02001b34(Func_02008db4, 0x1ddb);
    Func_0200707e_a(request_a);
    Func_02008d92(9, 1);
    Func_02008d9a(12, 1);
    Func_02008da2(11, 1);
    Func_02008daa(13, 1);
    Func_02008dba(10, 1);
    Call3(Func_02008e16, 9, 0xd000, 0);
    Call3(Func_02008e22, 12, 0xd000, 0);
    Call3(Func_02008e2e_a, 11, 0xd000, 0);
    Call3(Func_02008e3a, 13, 0xd000, 0);
    Call3(Func_02008e46, 10, 0xb000, 20);
    Func_02008dfe(8, 1);
    Value2(Func_02008e36, request_a, 0);
    if (Value2(Func_02008d6e, 0, 0) == 0) {
        Func_02008e1a(9, 2);
        Call1_02001b34(Func_0200710c_b, 0x9009);
        Call3(Func_02008e94, 8, 0x108, 40);
        Func_0200711e_a(request_a);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
        Func_02008e6e(9, 1);
        Call1_02001b34(Func_02007160, 0x9009);
        Func_02008e74(8, 2);
        Call1_02001b34(Func_0200716e, 0x9008);
    }
    Call3(Func_02008ef4, 13, 0x105, 40);
    Call2(Func_02008f0c, 0xcccc, 0x1999);
    Call4(Func_02008f26, 0x1d80000, -1, 0x27c0000, 1);
    Call3(Func_02008e2c, 13, 0x10000, 0x8000);
    Call3(Func_02008e74_a, 13, 0x1d8, 0x296);
    Func_020071cc(13, 0xb000);
    Func_020071be(13);
    Func_020071e0(8, 0x5000);
    Func_02008ec8(8, 3);
    Func_02008ed0(9, 3);
    Func_02008f4a(11, 0, 0);
    Call3(Func_02008f56, 13, 0x8000, 20);
    Func_02008ede(11, 3);
    Func_02008ef6(13, 3);
    Func_02008e5c(20);
    Func_02008f24(12, 1);
    Func_02007232_a(12, 0x3000);
    Call3(Func_02008f7c, 0x100c, 0, 20);
    Func_02008f96(11, 0xb000, 20);
    Call3(Func_02008fba, 11, 0x101, 40);
    Func_02007246_a(11);
    request_b = 0x900c;
    Func_02007264(12, 0xd000);
    Func_02008f3c(12, 4);
    Func_0200725e(request_b);
    Func_0200727a(13, 0xb000);
    Func_02008f82(13, 1);
    Func_02007274(13);
    Call3(Func_02008ffc, 9, 0x100, 20);
    Func_0200729c(9, 0x3000);
    Func_02008fa4(9, 1);
    Func_02007296(9);
    Func_02008f92(12, 3);
    Func_020072a4(request_b);
    Func_02008fc0(8, 2);
    Func_020072b2_a(8);
    Func_020072ce_a(12, 0xd000);
    Func_02008fb6(12, 3);
    Func_020072c8(request_b);
    Func_02008fe4_a(11, 2);
    Func_020072ec(11, 0xb000);
    Func_020072de(11);
    Func_020072fa(12, 0);
    Func_020072ec_a(request_b);
    Func_0200905a(8, 0x3000, 0);
    Func_02009064(9, 0, 0);
    Func_0200906e_a(11, 0xd000, 0);
    Func_02009078_a(13, 0xd000, 0);
    Call3(Func_02008fae, 0, 0x10000, 0x8000);
    Call3(Func_02008ff4, 0, 0x1e6, 0x260);
    Call3(Func_020090a0, 0, 0x8000, 0);
    record = Pointer1(Func_02008fbe, 0);
    if (record != 0) {
        Func_0200902c(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02008fea, 1, 0x10000, 0x8000);
    Call3(Func_02009030, 1, 0x1e6, 0x270);
    Call3(Func_020090dc, 1, 0x8000, 0);
    record = Pointer1(Func_02008ffa, 1);
    if (record != 0) {
        Func_02009068(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02009026, 2, 0x10000, 0x8000);
    Call3(Func_0200906c, 2, 0x1e6, 0x280);
    Call3(Func_02009118, 2, 0x8000, 0);
    record = Pointer1(Func_02009036, 2);
    if (record != 0) {
        Func_020090a4(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02009062, 3, 0x10000, 0x8000);
    Call3(Func_020090a8, 3, 0x1e6, 0x290);
    Call3(Func_02009154, 3, 0x8000, 20);
    Call3(Func_02009178, 12, 0x108, 40);
    Func_02007402(request_b);
    Func_0200911e(9, 1);
    Call1_02001b34(Func_02007410, 0x1009);
    Func_0200910c(8, 3);
    Func_02007434(8, 0x5000);
    Func_02007426(8);
    Func_02007442(8, 0x3000);
    Value2(Func_0200917a, 8, 0);
    if (Value2(Func_020090b2, 0, 0) == 1) {
        Func_02009156(8, 2);
        Func_02007450(8);
        Func_0200914c(12, 3);
        Func_0200745e(request_b);
        Func_02009172(9, 1);
        Call3(Func_020091c4, 0x9009, 0, 40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 3;
        Func_020091d2(8, 3);
        Func_02009224(8, 0, 40);
    }
    Func_020091ec(13, 1);
    Func_020074de(13);
    Func_020091fa(8, 1);
    value = 176;
    Func_02007508(8, 0x5000);
    Func_020074fe(8);
    Func_0200921a(13, 1);
    Func_02007522(13, (value << 8));
    Func_0200926c(13, 0, 20);
    Func_02009214(8, 3);
    Call3(Func_020091b6, 8, 0xcccc, 0x6666);
    request_c = 0x4008;
    Call3(Func_020091fe, 8, 0x1d8, 0x278);
    Func_02007540(request_c);
    Call3(Func_020092c6, 13, 0x103, 40);
    Func_0200925e(13, 2);
    Func_02007558(13);
    Func_02009244(8, 4);
    Func_020092be(request_c, 0, 40);
    Func_02009286(11, 1);
    Func_0200758e(11, (value << 8));
    Call1_02001b34(Func_02007580, 0x100b);
    Call3(Func_02009308, 10, 0x102, 20);
    Call3(Func_02009222, 10, 0x26666, 0x13333);
    Func_020092a4(10, 2, 0);
    Call3(Func_02009270, 10, 0x1ce, 0x2a2);
    Func_020075c8(10, (value << 8));
    Func_020092c8(10, 2);
    Func_020075c2(10);
    Func_020075de(9, 0x5000);
    Func_020092c6_a(9, 4);
    Func_020075d8_a(9);
    Func_020092d4(8, 3);
    Func_020075e6(request_c);
    Call3(Func_0200936e, 13, 0x102, 40);
    Func_02009350(13, 0, 40);
    Func_0200761a(9, 0x3000);
    Func_0200931a(9, 2);
    Call1_02001b34(Func_02007614, 0x1009);
    Func_02007630(12, 0);
    Call3(Func_0200938c, 8, 0x8000, 0);
    Func_02009396(9, 0x5000, 0);
    Func_020093a0(11, (value << 8), 0);
    Func_020093aa(13, (value << 8), 0);
    Func_020093b4(10, (value << 8), 20);
    Func_0200936c(12, 1);
    Call3(Func_020093b6, 0x100c, 0, 20);
    Call3(Func_020093e8, 8, 0x101, 40);
    Func_0200768a(8, 0xd000);
    Value2(Func_020093c2, 0x1008, 0);
    if (Value2(Func_020092fa, 0, 0) == 0) {
        Func_02009386(8, 3);
        Call1_02001b34(Func_02007698, 0x1008);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Call1_02001b34(Func_020076c0, 0x1008);
    }
    Func_020093bc(0, 3);
    Func_020093c4(8, 3);
    Call1_02001b34(Func_020076d6, 0x1008);
    Call2(Func_020076f4, 8, 0x8000);
    Call1_02001b34(Func_020076e6, 0x4008);
    Func_0200776c(2, 0, 0);
    Func_020093dc(12, 3);
    Func_020093e4(11, 3);
    Func_020093ec(9, 3);
    Func_0200941c(10, 2);
    Func_0200942c(13, 2);
    Func_02009372(20);
    action = (s32)Data_0200e6e4;
    Func_020093bc_a(10, action);
    Func_02009382(4);
    Func_020093ca(11, action);
    Func_02009390(4);
    Func_020093d8(12, action);
    Func_0200939e(4);
    Func_020093e6(9, action);
    Func_0200943e(3, 2);
    record = Pointer1(Func_020093dc_a, 2);
    if (record != 0) {
        Func_0200941e(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200944c(3);
    Func_0200945e(3, 0, 0);
    Func_0200946e(2, 2);
    record = Pointer1(Func_0200940c, 1);
    if (record != 0) {
        Func_0200944e(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200947c(2);
    Func_0200948e(2, 0, 0);
    Func_0200949e(1, 2);
    record = Pointer1(Func_0200943c, 0);
    if (record != 0) {
        Func_0200947e(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020094ac(1);
    Func_020094be(1, 0, 0);
    Func_0200947e_a(13, action);
    Call3(Func_020094bc, 8, 0x1c8, 0x288);
    Func_02007814(8, 0);
    Func_02009452(232);
    Call1_02001b34(Func_02009448, 0x925);
    Func_02009474();
}

/*
 * A four-way gated cascade. Each gate either runs its own setter sequence
 * and returns or falls through to the next, and all four share one exit.
 * The 336-byte owner at 0x0200351c includes its eight trailing pool words.
 */
void State_RunFlagGatedSetupCascade(void)
{
    if (Func_020099c8((s32)&Value_0000093e) != 0) {
        Func_02009a7e(8, 0, 0);
        Func_02009a88(9, 0, 0);
        Func_02009a92(10, 0, 0);
        Func_02009a9c(11, 0, 0);
        Func_02009aa6(12, 0, 0);
        Func_02007efa(14, 0, 0);
        return;
    }

    if (Func_02009a0a(0x8a0) != 0) {
        Func_02007e0e(8, 0x98, 0x1bc, 0x3000);
        Func_02009a86(8, 0x0200e958);
        Func_02007e32(10, 0xb8, 0x1e0, 0xb000);
        Func_02007e40(12, 0xaa, 0x1e8, 0xb000);
        Func_02007e4c(13, 0x88, 0x1e8, 0xd000);
        Func_02007e58(15, 0x78, 0x1e0, 0xd000);
        Func_02007e64(14, 0xb8, 0x20e, 0xb000);
        Func_02007e74(11, 0x88, 0x248, 0x8000);
        Func_02009aec(11, 0x0200e840);
        return;
    }

    {
        s32 t = Func_02009a94((s32)&Value_00000928);
        if (t != 0) {
            Func_02008600(t);
            return;
        }
    }

    if (Func_02009aa4((s32)&Value_00000925) != 0) {
        Func_02007efa(18, 0, 0);
        return;
    }

    if (Func_02009aba((s32)&Value_00000911) != 0 &&
        Func_02009ac4((s32)&Value_00000922) != 0) {
        Func_02007f1a_a(14, 0, 0);
        Func_02009b84(12, 0, 0);
    }
}

void RunSceneSelectionChain(void)
{
    Func_02009a92_a(1);
    Func_02006e2a();
    if (Func_02009b1c(2366) != 0) {
        Func_02007f72(4, 4, 0);
        Func_02007f2a(8, 412, 222, 12288);
        Func_02007f3a(9, 458, 161, 32768);
    } else {
        if (Func_02009b54(2208) != 0) {
            Call3_0200351c(Func_02009c0e, 8, 30932992, 9961472);
            Func_02009c1e(9, 5);
            Func_02007fc0(4, 4, 0);
        } else {
            if (Func_02009b80(2347) != 0) {
                Func_02007fd6(16, 0, 0);
                Func_02007fe0(4, 4, 0);
                Func_02007952(3);
            } else {
                if (Func_02009ba6_a(2346) != 0) {
                    Func_02007ffc(16, 0, 0);
                    Func_02008006(4, 3, 0);
                    Func_02007978(2);
                } else {
                    if (Func_02009bcc(2345) != 0) {
                        Func_02008022(16, 0, 0);
                        Func_0200802c(4, 2, 0);
                        Func_0200799e(1);
                    } else {
                        if (Func_02009bf2(2344) != 0) {
                            Func_02008048(16, 0, 0);
                            Func_02009cb2(10, 0, 0);
                            Func_020079c4(0);
                        } else {
                            Func_02009cca(9, 5);
                            if (Func_02009c20(2341) != 0 && Func_02009c2a(2342) == 0) {
                                Func_0200703e();
                            }
                        }
                    }
                }
            }
        }
    }
}

void Actor_SetFlagBit3ForActors28To35(void)
{
    u32 i;
    u32 bit;
    u32 zero;

    i = 28;
    bit = 8;
    zero = 0;
    for (; i <= 35; i++) {
        u8 *obj = Func_02009ca8(i);
        u32 v = obj[0x59];
        obj[0x59] = (u8)((v | bit) | zero);
    }
}

void Scene_RunFourActorCoordinatePresentation(void)
{
    u8 *record;
    s32 mode;

    Func_02009d84();
    Func_020081ae(25, 0, 0);
    Func_020081b8(24, 1, 0);
    Func_0200818e(0x1b80000, -1, 0xa80000, 0x1000001);
    Func_02008188_a(27, 0x1b8, 164, 0x5000);
    Func_0200819e(8, 0x1ac, 190, 0xd000);
    Func_020081ae_a(9, 0x1c4, 190, 0xb000);
    Func_02009e76(9, 1);
    mode = 128;
    Func_020081c6_a(0, 0x1b8, 134, 0x8000);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = (mode << 1);
    Func_02009f7a();
    Call3(Func_02009e4c, 0, 0xcccc, 0x6666);
    Call3(Func_02009e90, 0, 0x198, 134);
    Call3(Func_02009e9c, 0, 0x198, 148);
    Call3(Func_02009ea8, 0, 0x1a8, 148);
    Call3(Func_02009f54, 0, 0x4000, 20);
    Func_02009f0c(27, 1);
    Call1_020038ac(Func_02009f3a, 0x1e27);
    Func_02008204(27);
    Func_02009f20(8, 1);
    Func_02008212(8);
    Func_02009f0e(27, 3);
    Func_02008220(27);
    Func_0200823c(27, 0xd000);
    record = Pointer1_020038ac(Func_02009eaa, 0);
    if (record != 0) {
        Func_02009f18(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02009ed2, 1, 0xcccc, 0x6666);
    Func_02009f14(1, 0x1b8, 148);
    Call3(Func_02009fc0, 1, 0x4000, 0);
    record = Pointer1_020038ac(Func_02009ede, 1);
    if (record != 0) {
        Func_02009f4c(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02009f06, 2, 0xcccc, 0x6666);
    Call3(Func_02009f4a, 2, 0x1c8, 148);
    Call3(Func_02009ff6, 2, 0x4000, 0);
    record = Pointer1_020038ac(Func_02009f14_a, 2);
    if (record != 0) {
        Func_02009f82(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02009f3c, 3, 0xcccc, 0x6666);
    Call3(Func_02009f80, 3, 0x1d8, 148);
    Call3(Func_0200a02e, 3, 0x4000, 20);
    Func_02008352(0, 0, 60);
    Func_0200835c_a(1, 0x4000, 20);
    Func_02008366_a(2, 1, 20);
    Func_0200a058(27, 0x5000, 20);
    Func_020082fa(27);
    Func_0200a00e(9, 1);
    Func_0200a088(9, (mode << 1), 40);
    Func_02008312_b(9);
    Func_0200a026(1, 3);
    Call3(Func_0200a0a0, 1, 0x103, 60);
    Func_0200a020(27, 3);
    Func_02008332(27);
    Func_0200a04e(10, 1);
    Func_0200a026_a(10, 3);
    Func_02008348(10);
    Func_0200a034(8, 3);
    Func_0200a03c(9, 3);
    Func_0200a044_a(11, 3);
    Func_0200a04c_a(12, 3);
    Func_0200a064(13, 3);
    Func_020083f6(0, 0, 40);
    Func_02008400(2, 1, 0);
    Func_0200840a_a(1, 0x4000, 20);
    Func_0200a08a_a(27, 4);
    Func_0200839c(27);
    Call3(Func_0200a124, 8, 0x102, 60);
    Func_0200a0bc(8, 1);
    Func_020083b6_a(8);
    Func_0200a0b2(27, 3);
    Func_020083c4(27);
    Func_0200a132(8, 0, 0);
    Func_0200a13c(9, 0x8000, 40);
    Call3(Func_0200a160, 8, 0x102, 0);
    Call3(Func_0200a16c, 8, 0x102, 40);
    Func_0200a10c(27, 1);
    Func_0200a0e4(27, 3);
    Func_0200a15e(27, 0, 20);
    Func_0200a0f6(8, 3);
    Func_0200a10e(9, 3);
    Func_0200a074(40);
    Func_0200a1a6(9, (mode << 1), 20);
    Func_02008446(9, 0xb000);
    Func_02008438(9);
    Func_02008456(27, 0x3000);
    Call3(Func_0200a1c8, 27, 0x101, 60);
    Func_0200a1aa(27, 0, 60);
    Call3(Func_0200a1de, 27, 0x106, 20);
    Func_0200847e(27, 0xb000);
    Func_0200a166(27, 3);
    Func_02008478(27);
    Func_020084fe(3, 2, 80);
    Func_0200849e(8, 0xd000);
    Func_0200a19e(8, 2);
    Func_02008498(8);
    Func_0200a194(9, 3);
    Func_0200a1b4(9, 2);
    Func_020084ae(9);
    Func_020084ca(27, 0x5000);
    Func_0200a1b2_b(27, 3);
    Func_0200a1da(27, 1);
    Func_020084cc(27);
    Call3(Func_0200a162, 27, 0xcccc, 0x6666);
    Call3(Func_0200a1a6_a, 27, 0x198, 158);
    Call3(Func_0200a1b2_a, 27, 0x198, 148);
    Func_0200a25c(27, 0, 20);
    Func_0200a214(27, 1);
    Func_02008506(27);
    Func_0200858c(1, 0x8000, 20);
    Func_02008596(2, 1, 0);
    Call3(Func_0200a1ea, 27, 0x198, 134);
    Func_0200a1ec_a(27, 0x1b8, 134);
    Func_0200a18a(40);
    Func_020085dc_a(9, 10, 0);
    Call1_020038ac(Func_0200a1a2, 0x926);
}

void FieldScene_RunScene3b1_02003d10(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 record;

    Func_0200a1dc();
    Func_02008606(15, 0, 1);
    Func_0200a2a6(8, 1);
    Func_0200a1ec(20);
    Call3(Func_0200a22e, 8, 0xcccc, 0x6666);
    Call3(Func_0200a272, 8, 0x1d4, 0x266);
    Call3(Func_0200a280, 8, 0x1d8, 0x254);
    Call3(Func_0200a32c, 8, 0x8000, 20);
    Func_0200a2d6(8, 4, 20);
    rec7 = Value0(Func_0200833a);
    Func_0200a232(20);
    Func_0200a3d8(214);
    Func_0200a1d8(rec7, 0x200e738);
    Func_0200a246(40);
    Func_0200a2ee(8, 3);
    Func_0200a254(20);
    Call3(Func_0200a2d2, 8, 0x1d2, 0x270);
    Value2(Func_0200862c, 8, 0x5000);
    Func_0200a32c_a(8, 2);
    Call1_02003d10(Func_0200a362, 0x1e3b);
    Func_0200a384(8, 0, 20);
    Func_020086b6(9, 11, 0);
}

void FieldScene_RunScene3b1_02003dec(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_0200a2b8();
    Func_020086e2(15, 1, 1);
    Call3(Func_0200a3d6, 8, 0x5000, 40);
    Func_0200a386(8, 2);
    Call1(Func_0200a3bc, 0x1e3d);
    Func_0200a3de(8, 0, 20);
    Func_02008710(9, 11, 0);
}

void FieldScene_RunScene3b1_02003e34(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_0200a300();
    Func_0200872a(24, 0, 0);
    Func_02008734(18, 0, 0);
    Func_0200a39e(0, 0, 0);
    Call3(Func_0200a3aa, 16, 0x960000, 0x24a0000);
    Call4(Func_02008720, 0x9c0000, -1, 0x2180000, 0x1000001);
    Func_02008766(8, 0, 0);
    Call3(Func_0200a380, 16, 0xcccc, 0x6666);
    Call3(Func_0200a3c2, 16, 168, 0x242);
    Call3(Func_0200a3cc, 16, 168, 0x22a);
    Call3(Func_0200a478, 16, 0x8000, 20);
    Func_0200a428(16, 2);
    Call1(Func_0200a45e, 0x1e3c);
    Func_0200a480(16, 0, 20);
    Func_020087b2(9, 12, 0);
}

void FieldScene_RunScene3b1_02003eec(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_0200a3b8();
    Func_0200a442(0, 0, 0);
    Func_02008f02();
    Call3(Func_0200a452, 18, 0x960000, 0x24a0000);
    Call4(Func_020087c8, 0x9c0000, -1, 0x2180000, 0x1000001);
    Func_0200880e(8, 0, 0);
    Call3(Func_0200a428_a, 18, 0xcccc, 0x6666);
    Call3(Func_0200a46a, 18, 168, 0x242);
    Call3(Func_0200a474, 18, 168, 0x22a);
    Call3(Func_0200a520, 18, 0x8000, 20);
    Func_0200a4d0(18, 2);
    Call1(Func_0200a506, 0x1e3c);
    Func_0200a528(18, 0, 20);
    Func_0200885a(9, 12, 0);
}

/*
 * A flat setter cascade, one workspace-slot store, then a four-way gated
 * chain ending in an unconditional default arm. The store spells both its
 * offset and its stored value as 224 << 1 rather than folded constants.
 * The 340-byte owner at 0x02003f94 includes its trailing pool words.
 */
void Scene_RunFlagBranchedSetupCascade(void)
{
    extern u8 *Data_03001ebc;

    Func_0200a460();
    Func_0200a4f0(9, 5);
    Func_02008892(24, 1, 0);
    Func_0200a4fc(0, 0, 0);
    Func_020088a6(17, 0, 0);
    Func_02008634(0);
    Func_020088b6(8, 1, 20);
    Func_0200a5ce(0x6666, 0xccc);
    Func_0200a5e8(0x1b80000, -1, 0xb00000, 1);
    Func_0200a4ae(20);
    Func_0200a546(9, 7);
    Func_0200a4bc(30);
    Func_0200a662(0xbc);
    Func_0200a4c8(30);
    Func_0200867e(16);
    Func_0200a4d4(0x50);
    Func_0200868a(0);
    Func_0200a4e0(0x3c);
    Func_0200a578_a(9, 7);
    Func_0200a4ee(30);
    Func_0200a694(0xbc);
    Func_0200a4fa(30);
    Func_020086b0(16);
    Func_0200a506_a(0x50);
    Func_020086bc(0);
    Func_0200a512(0x5a);
    Func_0200a6b8(0xbc);
    Func_0200a51e(30);

    *(u32 *)(Data_03001ebc + (224 << 1)) = (224 << 1) + 67;

    Func_0200895e(9, 0, 0);

    if (Func_0200a51c((s32)&Value_0000092b) != 0) {
        Func_0200a69e(20);
    } else if (Func_0200a52e((s32)&Value_0000092a) != 0) {
        Func_0200a6b0(18);
    } else if (Func_0200a540((s32)&Value_00000929) != 0) {
        Func_0200a6c2(17);
    } else if (Func_0200a552((s32)&Value_00000928) != 0) {
        Func_0200a6d4(16);
    } else {
        Func_0200a6dc(13);
    }
}

void FieldScene_RunScene3b1_020040e8(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_0200a5b4();
    Func_020089de(15, 1, 1);
    Func_0200a67e(8, 1);
    Func_0200a5c4(10);
    Call3_020040e8(Func_0200a6e0, 8, 0x3000, 20);
    Func_0200a690(8, 2);
    Func_0200a6c6((s32)Data_00001e40);
    Func_0200a6e8(8, 0, 20);
    Func_02008a1a(9, 14, 0);
}

void FieldScene_RunScene3b1_0200413c(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_0200a608();
    Call4(Func_0200a752, -1, -1, -1, 0);
    Func_0200a578(1);
    Func_02008a4a(15, 1, 1);
    Func_0200a6ea(8, 1);
    Call1(Func_0200a718, 0x1e43);
    Func_020089e2(8);
    Call3(Func_0200a752_a, 8, 0xd000, 40);
    Func_02008a74(9, 15, 0);
}

void FieldScene_RunScene3b1_02004198(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_200e8e4;

    Func_0200a664();
    Func_02008a8e(24, 1, 0);
    Func_02008a98(25, 1, 0);
    Func_02007968();
    Func_02008aa6(19, 11, 12);
    Func_0200a716(10, 6);
    Value2_02004198(Func_0200a6ce, 12, 0x200e840);
    base5_200e8e4 = (s32)Data_0200e8e4;
    Func_0200a6d8(36, base5_200e8e4);
    Value2_02004198(Func_0200a6e0_a, 37, base5_200e8e4);
    Value2_02004198(Func_0200a6e8_a, 38, base5_200e8e4);
    Func_0200a788(36, 3);
    Func_0200a790(37, 3);
    Func_0200a798(38, 3);
    Func_020092e8();
    Func_0200a6d8_a();
}

/*
 * A flat setter sequence, no branches; the final call takes no arguments.
 * The 60-byte owner at 0x02004218 includes its one pool word, the address
 * taken as Value_00000928.
 */
void Scene_RunActors24And25Setup(void)
{
    extern u8 *Data_03001ebc;

    Func_0200a6e4();
    Func_02008b0e(24, 1, 0);
    Func_02008b18(25, 0, 0);
    Func_02008b22(19, 11, 12);
    Func_02009532();
    Func_0200a6ec((s32)&Value_00000928);
    Func_0200a718_a();
}

/*
 * Four "install or fallback" blocks, each gated by a flag check, then four
 * unconditional installs.  src is threaded through every block unmodified,
 * and blocks two and four store the 0xffff0000 sentinel into the installed
 * object's +24 field.  The owner includes the trailing pool words that hold
 * the flag addresses and the sentinel.  Callees are named by the address
 * their call site computes, not by a runtime address.
 */
void Scene_InstallFlaggedActors10To17(u8 *src)
{
    extern u8 *Data_03001ebc;

    if (Func_0200ab18((s32)&Value_00000928) != 0) {
        u8 *obj = Func_0200962c(0, 0);
        Func_0200abda(obj, 0xcd << 17, 0xac << 16);
        Func_02008f84(7, obj, src);
        Func_0200abee(10, 0, 0);
    } else {
        Func_02008f9a(5, 10, src);
    }

    if (Func_0200ab58((s32)&Value_00000929) != 0) {
        u8 *obj = Func_0200966c(1, 0);
        Func_0200ac1a(obj, 0xeb << 17, 0xac << 16);
        *(u32 *)(Func_0200abc0(obj) + 24) = 0xffff0000;
        Func_02008fce(7, obj, src);
        Func_0200ac38(11, 0, 0);
    } else {
        Func_02008fe4(6, 11, src);
    }

    if (Func_0200aba2((s32)&Value_0000092a) != 0) {
        u8 *obj = Func_020096b6(2, 0);
        Func_0200ac64(obj, 0xcd << 17, 0xcc << 16);
        Func_0200900e(7, obj, src);
        Func_0200ac78(12, 0, 0);
    } else {
        Func_02009024(5, 12, src);
    }

    if (Func_0200abe2((s32)&Value_0000092b) != 0) {
        u8 *obj = Func_020096f6(3, 0);
        Func_0200aca4(obj, 0xeb << 17, 0xcc << 16);
        *(u32 *)(Func_0200ac4a(obj) + 24) = 0xffff0000;
        Func_02009058(7, obj, src);
        Func_0200acc2(13, 0, 0);
    } else {
        Func_0200906e(6, 13, src);
    }

    Func_02009078(5, 14, src);
    Func_02009082(6, 15, src);
    Func_0200908c(5, 16, src);
    Func_02009096(6, 17, src);
}

/*
 * Both callees live inside this overlay and are declared without a prototype,
 * so each call site fixes its own arity.
 */

/*
 * Resource 3b1 unindexed helper at 0x02004880 (16 bytes, 69 calls).
 *
 * Derived span: no inventory row (item 28). `push {lr}` at 0x02004880,
 * epilogue `pop {r0} / bx r0` at 0x0200488c-0x0200488e returns void.
 * Adjoins 0x0200486c below and 0x02004890 above with no pad or pool on
 * either side.
 *
 * The second argument is masked to 16 bits (`lsls`/`lsrs` #16) before the
 * call, so it is genuinely `u16`, not a truncated `s32`.
 */

/*
 * Shared helper for resource_3b1.  The owner at 0x0200486c is 18 bytes; the
 * halfword before the next owner at 0x02004880 is alignment, not part of it.
 */
void Scene_RunStepThen10(s32 a)
{
    Func_0200ae2a(a, 0);
    Func_0200ad38(10);
}

void Scene_CallPairWith10(s32 a, u16 b)
{
    Func_0200ae5a(a, b, 10);
}

/*
 * Resource 3b1 unindexed helper at 0x02004890 (28 bytes, 55 calls).
 *
 * Derived span: no inventory row (item 28). `push {r5,r6,lr}` at
 * 0x02004890, epilogue `pop {r5,r6} / pop {r0} / bx r0` at
 * 0x020048a6-0x020048aa returns void. Adjoins 0x02004880 below and
 * 0x020048ac above with no pad or pool.
 *
 * `b` and `c` are shifted left 16 (16.16 fixed point) before the first
 * call but not masked, so they stay full `s32`. `Func_0200ad8a` returns a
 * pointer whose halfword at offset 6 is then set to `d`.
 */
void OvObj_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d)
{
    Func_0200ade4(a, b << 16, c << 16, d);
    *(s16 *)((u8 *)Func_0200ad8a(a) + 6) = d;
}

void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags)
{
    u32 selected;

    Func_0200aeb6(x, y, z, ~flags & 1);
    selected = flags & 0x1111;
    if ((flags & 0x10000000) != 0)
        Func_0200aed0();
    if ((flags & 0x01000000) != 0)
        Func_0200ad3e();
    Func_0200ad9c(selected);
}

void Scene_RunSceneStep(s32 step, u32 arg, u32 opt)
{
    extern const s32 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 slot;

    switch (step) {
    case 0:
        Func_0200af44(0, 0, 0);
        Call3(Func_0200af50, 1, 0x8000, 0);
        Value_0200e5d0(2, 0, 0);
        Call3(Func_0200af8a, 3, 0x8000, opt);
        break;
    case 1:
        Func_0200af6c(0, arg, 0);
        Func_0200af76(1, arg, 0);
        Func_0200af80(2, arg, 0);
        Call3(Func_0200af8a, 3, arg, opt);
        break;
    case 2:
        Func_0200af14(0, 3);
        Func_0200af1c(1, 3);
        Func_0200af24(2, 3);
        Func_0200af2c(3, 3);
        if (arg != 0) {
            Func_0200af4e(3);
        }
        if (opt == 0) {
            break;
        }
        Func_0200aedc(opt);
        break;
    case 3:
        Call2(Func_0200afe8, 0, 0x102);
        Call2(Func_0200aff2, 1, 0x102);
        Call2(Func_0200affc, 2, 0x102);
        Call2(Func_0200b006, 3, 0x102);
        Func_0200aedc(opt);
        break;
    case 4:
        for (slot = 0; slot < arg; slot++) {
            Func_0200af7c(slot + 10, 0, 0);
        }
        break;
    case 5:
        {
            u8 *rec;

            rec = Func_0200af28(arg);
            SetPose(rec, 0x5000);
        }
        Func_0200afb2(arg, 5);
        Func_0200afc2(arg, opt);
        break;
    case 6:
        {
            u8 *rec;

            rec = Func_0200af38(arg);
            SetPose(rec, 0x5000);
            *(s32 *)(rec + 24) = -0x10000;
        }
        Func_0200afb2(arg, 5);
        Func_0200afc2(arg, opt);
        break;
    case 7:
        {
            u8 *rec;

            rec = Value_0200e4e8(arg);
            SetPose(rec, 0x5000);
        }
        Func_0200924a(arg);
        if (opt == 0) {
            Func_0200afe4(arg, 0);
        }
        break;
    case 8:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x202;
        Func_0200b0d0();
        if (arg != 0) {
            Value_0200e640();
        }
        Func_0200af6e(0);
        break;
    case 9:
        Func_0200b0ec();
        Func_0200b0f8();
        if (arg == 0) {
            break;
        }
        Func_0200b0dc(arg);
        break;
    case 10:
        Func_020093b8(24, 1, 0);
        Func_020093c2_a(25, 0, 0);
        Func_020082ba(0);
        Func_02009382_a(0, 0x1b0, 168, 0x4000);
        Func_02009390_a(1, 0x1c0, 168, 0x4000);
        Func_0200939e_a(2, 0x1a8, 152, 0x4000);
        Func_020093ac(3, 0x1ca, 152, 0x4000);
        break;
    case 11:
        if (arg != 0) {
            u8 *rec;

            Func_0200b07a(13, 1);
            rec = Func_0200b018(13);
            SetPose(rec, 0x3000);
            rec = Func_0200b024(13);
            *(s32 *)(rec + 24) = 0x10000;
        }
        {
            u8 *rec;

            Func_0200b09a(14, 1);
            rec = Func_0200b038(14);
            SetPose(rec, 0x5000);
            Func_0200b0b2(15, 1);
            rec = Func_0200b050(15);
            SetPose(rec, 0x3000);
            rec = Func_0200b060(15);
            *(s32 *)(rec + 24) = 0x10000;
            Func_0200b0d6_a(16, 1);
            rec = Func_0200b074(16);
            SetPose(rec, 0x5000);
            Value_0200e550(17, 1);
            rec = Func_0200b086(17);
            SetPose(rec, 0x3000);
            rec = Func_0200b090(17);
            *(s32 *)(rec + 24) = 0x10000;
        }
        Call3(Func_0200b100, 28, 0x19a0000, 0xae0000);
        Call3(Func_0200b10e, 29, 0x1d60000, 0xae0000);
        Call3(Func_0200b11c, 30, 0x19a0000, 0xce0000);
        Call3(Func_0200b12a, 31, 0x1d60000, 0xce0000);
        Call3(Func_0200b138, 32, 0x19a0000, 0x11e0000);
        Call3(Func_0200b146, 33, 0x1d60000, 0x11e0000);
        Call3(Func_0200b154, 34, 0x19a0000, 0x13c0000);
        Call3(Func_0200b162, 35, 0x1d60000, 0x13c0000);
        Func_0200b040(1);
        if (arg != 0) {
            Call3(Func_0200b200, 13, 0xb000, 0);
        }
        Call3(Value_0200e5d0_b, 14, 0xd000, 0);
        Call3(Func_0200b218, 15, 0xb000, 0);
        Call3(Func_0200b224, 16, 0xd000, 0);
        Func_020094de(17, 0xb000);
        break;
    case 12:
        {
            u8 *rec;

            rec = Func_0200b14e(arg);
            Func_0200b1c0(arg, 1);
            if (opt != 0) {
                SetPose(rec, 0x3000);
            } else {
                SetPose(rec, 0x5000);
            }
            *(s32 *)(rec + 24) = 0x10000;
        }
        break;
    case 13:
        Func_0200b1da(9, 0, 0);
        Func_0200b1e4(12, 0, 0);
        Func_0200b1ee(11, 0, 0);
        Func_0200b1f8(13, 0, 0);
        Func_0200b21e(10, 0, 0);
        break;
    case 14:
        Func_0200b214(14, 0, 0);
        Func_0200b21e(13, 0, 0);
        break;
    case 15:
        Func_020095ca(24, 1, 0);
        Func_0200b234(9, 0, 0);
        Func_0200b23e(10, 0, 0);
        Func_02009596(8, 0x1bc, 0x266, 0xd000);
        Func_0200b258(0, 0, 0);
        if (arg != 0) {
            Func_020092e8_a();
        }
        Func_020095d6(0x1c00000, 0x200000, 0x2700000, 0x1000001);
        if (opt == 0) {
            break;
        }
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x202;
        Func_0200b372();
        Func_0200b386();
        Value_0200e4c0(20);
        break;
    case 16:
        Func_0200b2a0(8, 0, 0);
        Func_0200b2aa(9, 0, 0);
        Call3(Func_0200b2b8, 27, 0x1b60000, 0x980000);
        break;
    case 17:
        slot = 0;
        do {
            Func_0200b2ca(slot + 28, 0, 0);
            slot++;
        } while (slot <= 7);
        break;
    case 18:
        Func_0200962a(12, 152, 0x214, 0xb000);
        Func_0200963c(8, 134, 0x1ea, 0x3000);
        Func_0200964a(9, 166, 0x1ea, 0x5000);
        Func_0200965a(10, 182, 0x1f8, 0x5000);
        Func_02009666(11, 118, 0x1f8, 0x3000);
        Scene_RunSceneStep(14, 0, 0);
        break;
    case 19:
        Func_0200967e(8, 0x1a0, 0x148, 0);
        Func_02009694(9, 0x1c0, 0x160, 0xd000);
        Func_020096aa(10, 0x1c6, 248, 0x3000);
        Func_020096ba(arg, 0x198, 0x122, 0);
        Func_020096c8(opt, 0x198, 0x156, 0);
        Func_020096d8(13, 0x1a4, 0x164, 0xd000);
        Func_020096e6(14, 0x198, 0x130, 0);
        Func_020096f6_a(15, 0x1a2, 0x17a, 0xd000);
        Func_02009706(16, 0x1b8, 0x106, 0x3000);
        Func_02009712(17, 0x1c0, 0x17a, 0xd000);
        break;
    case 20:
        for (slot = arg; slot <= opt; slot++) {
            Func_0200b342(slot);
        }
        break;
    case 21:
        Func_0200978a(20, 0x92c, 0x93d);
        Func_02009794(20, 0x917, 0x91f);
        Func_020097a0(20, 0x990, 0x998);
        Func_0200b370(0x300);
        Func_0200b376(0x301);
        Func_0200b37c(0x302);
        break;
    case 22:
        Func_0200b2f4(1);
        Func_020097c6(23, 0, 0);
        Func_0200b3e6(12, Data_0200e840);
        break;
    case 23:
        Func_0200b3de(1);
        Func_0200b3e4(2);
        Func_0200b3ea(3);
        break;
    case 24:
        Call4_020048e8(Func_0200b50e, -1, -1, -1, 0);
        Func_0200b334(1);
        if (arg != 0) {
            *(u8 *)(Func_0200b52c() + 0x55) = 0;
        }
        break;
    case 25:
        Func_0200b402(Data_0200f2a0);
        Func_0200b350(1);
        if (arg == 1) {
            Func_0200b41a(Data_0200f300);
            Func_0200b368(1);
        } else if (arg == 2) {
            Func_0200b41a(Data_0200f360);
            Func_0200b368(1);
        } else if (arg == 3) {
            Func_0200b42c(Data_0200f3c0);
            Func_0200b37a(1);
        }
        break;
    }
}

/*
 * A flat setter sequence, no branches. The 52-byte owner at 0x02005004
 * includes its three pool words, which are plain numeric arguments and not
 * addresses of Value_ globals.
 */
void State_ApplyActor8FourFlags(void)
{
    Func_0200a044(8, 0x92c);
    Func_0200a04c(8, 0x935);
    Func_0200a054(8, 0x917);
    Func_0200a05e(8, 0x990);
}

/*
 * Scan slots 0 through 8 inclusive.  On the first a1 that Func_0200b4e4
 * accepts, call Func_0200b59a and stop.  a0 and a1 advance together.
 */
void Actor_RunFirstMatchingSlot(s32 a0, s32 a1)
{
    unsigned int i = 0;

    do {
        if (Func_0200b4e4(a1)!= 0) {
            Func_0200b59a(a0, 0, 0);
            break;
        }
        i++;
        a0++;
        a1++;
    } while (i <= 8);
}

void FieldScene_RunScene3b1_02005068(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = Value2_02005068(Func_0200a018, 0, 0);
    Func_0200b53e();
    Func_02009968(24, 1, 0);
    Func_02009972(25, 2, 0);
    Func_02008842();
    Value3(Func_02009980, 19, rec8, 12);
    Func_0200b5f0(10, 6);
    base5_200e840 = (s32)Data_0200e840;
    Func_0200b5aa(rec8, base5_200e840);
    Func_0200b5a0(11);
    Value2_02005068(Func_0200b5b8, 12, base5_200e840);
    base5_200e8e4 = (s32)Data_0200e8e4;
    Func_0200b5c2(36, base5_200e8e4);
    Func_0200b5ca(37, base5_200e8e4);
    Func_0200a1b2();
    Func_0200b5a2();
}

/* Sets up three actor slots (2, 1, 3) with position/pose data pulled from a
 * per-slot lookup record (fields at +8 and +16), then drives a chain of
 * actor animation, camera, and text/dialog calls for the scene. */
void Scene_RunPositionTransferPresentation(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    /* Per-slot lookup record; fields at +8 and +16 feed the setup call. */
    s32 record;
    s32 base5_1e46;
    s32 base5_200e7c8;

    Func_020099a6(0x1b80000, -1, 0xb00000, 0x1000001);
    Motion_SetHPosTerrain_1(0, 0x1b80000, 0x860000);
    Battle_WaitMode0_1(); /* main:0808a360 */
    Motion_SetSpeed_1(0, 0x19999, 0xcccc);
    Object_SetModeById_1(0, 5);
    Motion_SetPositionAndCommit_1(0, 0x198, 134);
    Motion_SetPositionAndCommit_2(0, 0x198, 152);
    Motion_SetPositionAndCommit_3(0, 0x1b0, 166);
    Object_SetModeById_2(0, 1);
    Motion_ArmCb_1(0, 0x4000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        Motion_SetHPosTerrain_2(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Motion_SetHPosTerrain_3(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_3(1);
    if (record != 0) {
        Motion_SetHPosTerrain_4(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_0200b5b6(1); /* main:080000c0 */
    Motion_SetSpeed_2(2, 0x19999, 0xcccc);
    Motion_ResetPosMode2_1(2, 0x1a8, 152);
    Motion_SetSpeed_3(1, 0x19999, 0xcccc);
    Motion_ResetPosMode2_2(1, 0x1c0, 168);
    Motion_SetSpeed_4(3, 0x20000, 0x10000);
    Motion_SetPosReset_1(3, 0x1ca, 152);
    Object_SetModeById_3(1, 1);
    Object_SetModeById_4(2, 1);
    Motion_ArmCb_2(2, 0x4000, 0);
    Motion_ArmCb_3(1, 0x4000, 0);
    Motion_ArmCb_4(3, 0x4000, 40);
    Func_0200b7ee(10); /* main:0808a1d8 */
    /* Text/dialog resource pointer, passed by base address and by base+1. */
    base5_1e46 = (s32)Data_00001e46;
    Func_0200b6b2(base5_1e46, 1, 10); /* main:08015210 */
    Battle_WaitMode0_2(10);
    Func_02009b1a(0, 0, 40);
    Value3(Func_02009b26, 1, 0x4000, 20);
    Motion_SetSpeedLim_1(0x39999, 0x7333); /* main:0808a208 */
    Call4(Func_02009b00, 0x1b80000, -1, 0x1400000, 0x10000014);
    Motion_SetVarCbObj_1(8, 2); /* main:0808a138 */
    Call2(Func_02009ae8, 8, 0xd000);
    SceneWork_SetStepValue_1((base5_1e46 + 1)); /* main:0808a170 */
    Func_02009ae0(8);
    Motion_ArmCb_5(8, 0, 20);
    Call4(Func_02009b3a, 0x1b80000, -1, 0x860000, 0x10000000);
    /* Second text/dialog resource pointer, shared across three calls. */
    base5_200e7c8 = (s32)Data_0200e7c8;
    Motion_EnableActCb_1(1, base5_200e7c8);
    Motion_EnableActCb_2(2, base5_200e7c8);
    Motion_MarkActiveAndSetActionCallback_1(3, base5_200e7c8); /* main:0808a0b0 */
    Battle_WaitMode0_3(40);
    GameFlag_Set_1(0x301);
    Func_02009ba6(23, 0, 0);
    GameFlag_Clear_1(0x12f);
}

void Scene_RunFormationAndEffectPresentation(void)
{
    s32 action;

    Func_02009ba2(0, 0x1bc, 0x12c, 0);
    Func_02009bb2(1, 0x1ca, 0x136, 0);
    Func_02009bc0(2, 0x1bc, 0x14a, 0);
    Func_02009bd2(3, 0x1b0, 0x136, 0);
    Func_02009be2(27, 0x1b8, 134, 0x8000);
    Func_02009bf6(10, 0x1c6, 248, 0x3000);
    Func_0200b8c0(10, 6);
    Func_02009c2e(0x1b80000, -1, 0x1340000, 0x1000001);
    Func_0200b9b6();
    Func_0200b9ca();
    Func_0200b850(20);
    Call3(Func_0200b96c, 0, 0x2000, 0);
    Call3(Func_0200b978, 1, 0xa000, 0);
    Call3(Func_0200b984, 2, 0xa000, 0);
    Call3(Func_0200b990, 3, 0x2000, 40);
    Func_02009cb2_a(2, 1, 20);
    Call1_020038ac(Func_0200b978_a, 0x1e6e);
    Func_02009c42(27);
    Func_02009cce(1, 0xc000, 0);
    Call2(Func_0200b9e6, 0x26666, 0x4ccc);
    Call4(Func_0200b9fc, 0x1b80000, -1, 0xb00000, 1);
    Call3(Func_0200b8fe, 27, 0x19999, 0xcccc);
    Call3(Func_0200b942, 27, 0x198, 134);
    Call3(Func_0200b94e, 27, 0x198, 152);
    Call3(Func_0200b95a, 27, 0x1a8, 164);
    Call2(Func_0200ba2a, 0x19999, 0x3333);
    Call4(Func_0200ba40, 0x1b80000, -1, 0x12c0000, 1);
    Call3(Func_0200b97c, 27, 0x1a8, 222);
    Call3(Func_0200b98a, 27, 0x1a8, 0x106);
    Func_0200ba34(27, 0x3000, 20);
    Func_0200b9ec(27, 1);
    Func_02009cde(27);
    Func_02009d64(2, 1, 20);
    Func_0200b9e4(27, 3);
    Func_0200ba0c(27, 1);
    Func_02009cfe(27);
    Func_02009d84_a(3, 2, 60);
    Func_02009d90(1, 0xe000, 60);
    Func_0200ba82(27, 0, 40);
    Func_0200ba3a(27, 1);
    Func_0200ba12(27, 2);
    Call3(Func_0200b9ee, 27, 0x1b0, 0x10c);
    Call3(Func_0200b9fc_a, 27, 0x1c4, 0x10c);
    Func_0200ba34_a(27, 1);
    Func_02009d6e(27, 0xd000);
    Func_0200ba6e(27, 2);
    Func_0200bac0(27, 0, 20);
    Func_02009df2(1, 0xc000, 20);
    Func_0200ba72(27, 4);
    Func_0200b9d8(40);
    Func_0200bae2(27, 0, 80);
    Func_0200baaa(27, 1);
    Func_0200b9f0(20);
    Func_0200ba98(27, 3);
    Func_0200b9fe(10);
    Func_02009dc8(27, 0x5000);
    Value2(Func_0200bb00, 27, 0);
    if (Value2(Func_0200ba38, 0, 0) == 0) {
        Func_0200bac4(27, 3);
        Func_02009dd6(27);
    } else {
        Func_0200bad4(27, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Func_02009df6(27);
        Func_02009e7c(3, 2, 40);
        Func_0200bb1c(27, 1);
        Func_0200baf4(27, 3);
        Func_02009e16(27);
    }
    Func_02009e9c_a(2, 1, 20);
    action = (s32)Data_0200e7f0;
    Func_0200babe(1, action);
    Func_0200bac6(2, action);
    Func_0200bade(3, action);
    Call2(Func_0200bbce, 0x9999, 0x1333);
    Call4(Func_0200bbe8, 0x1b80000, -1, 0xb00000, 1);
    Call3(Func_0200baee, 0, 0x10000, 0x8000);
    Call3(Func_0200bb34, 0, 0x1a8, 0x110);
    Call3(Func_0200bb38, 0, 0x1a8, 164);
    Func_0200bad6(60);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
    Func_02009f16(9, 0, 0);
    Call1_020038ac(Func_0200bae4, 0x301);
    Call1_020038ac(Func_0200baea, 0x927);
    Func_0200bc58(4);
}

void Scene_RunActors24And25SetupWithValue929(void)
{
    extern u8 *Data_03001ebc;

    s32 handle = Func_0200a634(0, 0);

    Func_0200bb5a();
    Func_02009f84(24, 1, 0);
    Func_02009f8e(25, 0, 0);
    Func_02008e84(0);
    Func_02009f9e(19, handle, 12);
    Func_0200bc08(11, 0, 0);
    Func_0200a9b8();
    Func_0200bb72((s32)&Value_00000929);
    Func_0200bb9e();
}

void FieldScene_RunScene3b1_020056dc(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec2;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = Value2_020056dc(Func_0200a690_a, 0, 0);
    rec2 = Value2_020056dc(Func_0200a69a, 1, 0);
    Func_0200bbc0();
    Func_02009fea(24, 1, 0);
    Func_02009ff4(25, 3, 0);
    Func_02008ec4();
    Value3(Func_0200a002, 19, rec8, rec2);
    Func_0200bc72(10, 6);
    base5_200e840 = (s32)Data_0200e840;
    Func_0200bc2c(rec8, base5_200e840);
    Func_0200bc22(11);
    Func_0200bc3a(rec2, base5_200e840);
    Func_0200bc30(12);
    base5_200e8e4 = (s32)Data_0200e8e4;
    Func_0200bc4a(36, base5_200e8e4);
    Value2_020056dc(Func_0200bc52, 37, base5_200e8e4);
    Func_0200bcf2(36, 3);
    Func_0200bcfa(37, 3);
    Func_0200a84a();
    Func_0200bc3a_a();
}

/*
 * A flat setter sequence, no branches. The 108-byte owner at 0x02005780
 * includes its one pool word, the address taken as Value_0000092a.
 */
void Scene_RunActors24And25SetupWithValue92a(void)
{
    extern u8 *Data_03001ebc;

    s32 handle = Func_0200a730(0, 0);
    s32 other = Func_0200a73a(1, 0);

    Func_0200bc60();
    Func_0200a08a(24, 1, 0);
    Func_0200a094(25, 0, 0);
    Func_02008f8a(0);
    Func_0200a0a4(19, handle, other);
    Func_0200bd0e(11, 0, 0);
    Func_0200bd18(12, 0, 0);
    Func_0200aac8();
    Func_0200bc82((s32)&Value_0000092a);
    Func_0200bcae();
}

void Scene_RunExtendedFormationPresentation(void)
{
    s32 slot_b;
    s32 slot_c;
    s32 slot_a;
    u8 *record;
    s32 action;

    slot_a = Value2(Func_0200a7a2, 0, 0);
    slot_b = Func_0200a7ac(1, 0);
    slot_c = Value2(Func_0200a7b6, 2, 0);
    Func_0200bcdc();
    Func_0200a106(10, 0, 0);
    Func_0200a110(17, 0, 0);
    Call3(Func_0200bd7e, 8, 0x1d80000, 0x980000);
    Func_0200bd8e(9, 5);
    Call3(Func_0200bd94, 27, 0x1b80000, 0x860000);
    Func_0200bdec(27, 15);
    record = Func_0200bd42(27);
    Func_0200bce0(record, 0);
    Func_02009ed8(16);
    Func_0200a126(0x1b60000, -1, 0xae0000, 0x1000001);
    Func_0200a16c_a(8, 1, 20);
    Call1_020038ac(Func_0200beea, 19);
    Func_0200bef0(181);
    Call3(Func_0200bd28, 0x20000, 0x20000, 0x10000);
    Func_0200bd66(10);
    Call3(Func_0200bd38, -1, -1, 0xe666);
    Func_0200bd76(80);
    Func_0200bf1c(181);
    Call3(Func_0200bd54, 0x20000, 0x20000, 0x10000);
    Func_0200bd92(10);
    Call3(Func_0200bd64, -1, -1, 0xe666);
    Func_0200bf42(63);
    Call1_020038ac(Func_0200bd92_a, 0x11a);
    Call2(Func_0200bee6, 3, 0x102);
    Func_0200bdbe(40);
    Func_0200a186(3, 0x6000);
    Call1_020038ac(Func_0200beb4, 0x1ec1);
    Func_0200bed6(3, 0, 40);
    Func_0200a188(27);
    Call3(Func_0200bef8, 0, 0x2000, 0);
    Call3(Func_0200bf04, 1, 0xa000, 0);
    Func_0200bf0e(2, 0, 0);
    Call3(Func_0200bf1a, 3, 0xe000, 40);
    Func_0200bf24(0, 0x6000, 0);
    Call3(Func_0200bf30, 1, 0xe000, 0);
    Func_0200bf3a(2, 0x6000, 0);
    Call3(Func_0200bf46, 3, 0x8000, 40);
    Call3(Func_0200bf52, 2, 0xe000, 0);
    Call3(Func_0200bf76, 2, 0x100, 60);
    Func_0200a218(2, 0x2000);
    Func_0200bf20(2, 1);
    Func_0200a212(2);
    Func_0200bf26(0, 1);
    Func_0200bf2e(1, 1);
    Func_0200bf3e(3, 1);
    Func_0200be84(10);
    Call3(Func_0200bfa0, 0, 0xc000, 0);
    Call3(Func_0200bfac, 1, 0xc000, 0);
    Call3(Func_0200bfb8, 2, 0xe000, 0);
    Call3(Func_0200bfc4, 3, 0xa000, 20);
    Func_0200bf94(27, 0);
    record = Func_0200beea_a(27);
    Func_0200be88(record, 1);
    Call3(Func_0200bf0e_a, 27, 0x10000, 0x8000);
    Call3(Func_0200bf52_a, 27, 0x1ae, 134);
    Func_0200a2ac(27, 0x3000);
    Func_0200bfac_a(27, 2);
    Func_0200a2a6_a(27);
    Func_0200bfba(slot_a, 1);
    Func_0200bfc2(slot_b, 1);
    Func_0200bfca(slot_c, 1);
    Func_0200bfda(13, 1);
    Call2(Func_0200c054, slot_a, 0x102);
    Call2(Func_0200c05e, slot_b, 0x102);
    Call2(Func_0200c068, slot_c, 0x102);
    Call2(Func_0200c072, 13, 0x102);
    Func_0200bf48(40);
    Func_0200a37a(12, slot_a, 0);
    Func_0200a384_a(12, slot_b, 1);
    Func_0200a38e(12, slot_c, 0);
    Func_0200a398(11, 1, 0);
    Call3(Func_0200c08c, slot_a, 0xd000, 0);
    Call3(Func_0200c098, slot_b, 0xb000, 0);
    Call3(Func_0200c0a4, slot_c, 0xd000, 0);
    Func_0200c0ae(0, 0, 0);
    Call3(Func_0200c0ba, 1, 0x8000, 0);
    Func_0200c0c4(2, 0, 0);
    Call3(Func_0200c0d0, 3, 0x8000, 40);
    Func_0200c080(27, 2);
    Func_0200c0c8(27, 0);
    Call3(Func_0200c0ec, 0, 0xc000, 0);
    Call3(Func_0200c0f8, 1, 0xc000, 0);
    Call3(Func_0200c104, 2, 0xc000, 0);
    Call3(Func_0200c110, 3, 0xc000, 20);
    Call3(Func_0200c07e, 27, 0x1b8, 134);
    Func_0200c0a2(27, 0, 0);
    Func_0200a3e2(1, 0x8000);
    Func_0200c0ea(1, 1);
    Func_0200a3dc(1);
    Func_0200c14a(2, 0, 0);
    Func_0200a402(3, 0x8000);
    Func_0200c0da(0, 3);
    Func_0200c0e2(1, 3);
    Func_0200c0ea_a(2, 3);
    Func_0200c102(3, 3);
    Call3(Func_0200c0a6, 1, 0x10000, 0x8000);
    Call3(Func_0200c0b2, 2, 0x10000, 0x8000);
    Call3(Func_0200c0be, 3, 0x10000, 0x8000);
    action = (s32)Data_0200e818;
    Func_0200c0d0_a(1, action);
    Func_0200c0d8(2, action);
    Func_0200c0f0(3, action);
    Call1_020038ac(Func_0200c08e, 0x302);
    *(s32 *)0x0200ff84 = 0;
    Value2(Func_0200c01e, (s32)Func_02005c48, 0xc80);
    Func_0200a4e8(23, 0, 0);
    Func_0200c0f6(27);
    Call1_020038ac(Func_0200c0bc, 0x12f);
    Call1_020038ac(Func_0200c0c2, 0x927);
    Func_0200c0e6();
}

void RunActorsEightAndNineMapEvent(void)
{
    Func_0200c170();
    Func_0200a59a(15, 1, 0);
    Func_0200a554(9, 468, 616, 32768);
    Func_0200a5b6(8, 1, 20);
    Func_0200c256(9, 2);
    Func_0200c19c(20);
    SetDirection(8, 53248, 80);
    Func_0200c2c2(8, 0, 20);
    Func_0200c25a(8, 3);
    Func_0200c1c0(20);
    Func_0200a5f2(9, 21, 0);
}

void FieldScene_RunScene3b1_02006110(void)
{
    extern u8 Data_0200e840[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec2;
    s32 rec4;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec2 = Value2(Func_0200b0cc, 0, 0);
    rec8 = Func_0200b0d6(1, 0);
    rec7 = Func_0200b0e0(2, 0);
    rec4 = Value2(Func_0200b0ea, 3, 0);
    Func_0200c610();
    Func_0200aa3a(10, 0, 0);
    Func_0200a9f2(8, 0x1d8, 144, 0x5000);
    Func_0200aa06(27, 0x198, 142, 0x3000);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x201;
    Func_0200c7b8();
    Func_0200c7cc();
    Func_0200c652(40);
    Func_0200c71a(27, 1);
    Call1(Func_0200c748, 0x1f78);
    Func_0200aa12(27);
    Func_0200c726(rec2, 2);
    Func_0200c72e(rec8, 2);
    Func_0200c736(rec7, 2);
    Func_0200c746(rec4, 2);
    Func_0200c68c(20);
    Func_0200c7a6(rec2, 0, 0);
    Call3(Func_0200c7b2, rec8, 0x8000, 0);
    Func_0200c7bc(rec7, 0, 0);
    Call3(Func_0200c7c8, rec4, 0x8000, 40);
    Call3(Func_0200c6fe, rec2, 0x10000, 0x8000);
    Call3(Func_0200c70c, rec8, 0x10000, 0x8000);
    Call3(Func_0200c71a_a, rec7, 0x10000, 0x8000);
    Call3(Func_0200c728, rec4, 0x10000, 0x8000);
    Call3(Func_0200c764, rec2, 0x1d6, 172);
    Call3(Func_0200c770, rec8, 0x19a, 172);
    Call3(Func_0200c77c, rec7, 0x1d6, 204);
    Call3(Func_0200c790, rec4, 0x19a, 204);
    Func_0200c7b8_a(rec2, 1);
    Func_0200c7c0(rec8, 1);
    Func_0200c7c8_a(rec7, 1);
    Call3(Func_0200c854, rec8, 0xd000, 0);
    Call3(Func_0200c860, rec2, 0xb000, 0);
    Call3(Func_0200c86c, rec4, 0xd000, 0);
    Call3(Func_0200c878, rec7, 0xb000, 20);
    Func_0200c830(27, 1);
    Func_0200ab22(27);
    Func_0200c80e(rec2, 3);
    Func_0200c816(rec8, 3);
    Func_0200c81e(rec7, 3);
    Func_0200c836(rec4, 3);
    Func_0200ab48(27);
    Func_0200c834(rec2, 3);
    Func_0200c83c(rec8, 3);
    Func_0200c844(rec7, 3);
    Func_0200c85e(rec4, 3);
    Func_0200c8da(27, 0, 0);
    Func_0200ab92(0, 0x8000);
    Func_0200c87a(0, 3);
    Func_0200c882(27, 3);
    Call3(Func_0200c826, 27, 0x10000, 0x8000);
    Func_0200c868(27, 0x198, 132);
    Call3(Func_0200c874, 27, 0x1bc, 132);
    Func_0200c896(27, 0, 0);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x202;
    Func_0200c992();
    Func_0200c99e();
    Call2(Func_0200c712, 0x92c, 0x935);
    Call2(Func_0200c71c, 0x917, 0x990);
    Call1(Func_0200c828, 0x8a0);
    Func_0200c996(10);
}

/*
 * Runs two first-match linear scans over indices 0 to 8, each breaking on its
 * first hit and calling a per-element handler, then cross-pairs the miss
 * counts: the count from scanning `a` indexes into `b`, and the count from
 * scanning `b` indexes into `a`.  Each callee is named for its own call site,
 * because every call reaches its target through its own local veneer and two
 * of the sites share one veneer.
 */
void State_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b)
{
    s32 cnt_a = 0;
    s32 cnt_b = 0;
    u32 i;

    for (i = 0; i <= 8; i++) {
        u8 *p = a + i;
        if (Func_0200c876(p)!= 0) {
            Func_0200c890(p);
            break;
        }
        cnt_a++;
    }

    for (i = 0; i <= 8; i++) {
        u8 *p = b + i;
        if (Func_0200c896_a(p)!= 0) {
            Func_0200c8b0(p);
            break;
        }
        cnt_b++;
    }

    Func_0200c8b0_a(b + cnt_a);
    Func_0200c8b8(a + cnt_b);
}

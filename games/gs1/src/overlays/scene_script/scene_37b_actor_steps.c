#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define RunEventScript01 Func_02000150
#define FieldScene_RunExtendedCameraSequence Func_02000614
#define FieldScene_RunScene37bSequenceA Func_0200195c
#define FieldScene_RunPanAndBurstSequence Func_02000c8c
#define GameFlag_IsSet_1(args...) Func_02003456(args)
#define BattleRuntime_Reset_1() Call0(Func_0200347e)
#define Scene_GetRecord_1(a0) Call1(Func_0200349c, a0)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020034ae, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020034da, a0, a1, a2)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Call0(Func_020034a6)
#define TARGET_ID 9
#define GATE_CODE 2059
#define FieldScene_CueActorTwoWhenCell13_7 Func_0200166c
#define FieldScene_RunScene37bSequenceC Func_020014b8
#define FieldScene_RunScene37bSequenceD Func_020016a4
#define FieldScene_RunScene37bSequenceF Func_020016dc
#define SceneActor_PlaceActorPairAtCells Func_02001b44
#define SceneEventRuntime_SelectInitialSceneByFlags Func_02000054
#define FieldScene_RunLoopedLayoutSequence Func_02000098
#define SceneEffect_UpdateScrollingSpriteRows Func_02000eb0
#define SceneState_RunWhenSlotZeroFacingC000 Func_02000fe4
#define SceneState_RunWhenActorZeroFacing4000 Func_02001000
#define FieldScene_RunPrimarySequenceHead Func_0200101c
#define FieldScene_CallWhenCheck9_31_9 Func_02001574
#define FieldScene_RunGuardedStep11 Func_0200158c
#define FieldScene_RunGuardedStep13 Func_020015a4
#define FieldScene_RunGuardedStep15 Func_020015bc
#define ConfigureSceneAndCheckActors Func_020015d4
#define FieldScene_RunClosingSequence Func_02001624
#define FieldScene_RunFiveValueStep9 Func_020019e4
#define FieldScene_RunFiveValueStep11 Func_02001a04
#define FieldScene_ApplyRect13_31_12_30_12 Func_02001a24
#define FieldScene_RunFiveValueStep15 Func_02001a44
#define FieldScene_ApplyRect10_14_7_13_7 Func_02001a64
#define SceneActor_UseActorTenCellAndNext Func_02001a84
#define SceneActor_MoveActor10ByRow Func_02001aac
#define FieldScene_ApplyRect12_21_7_22_7 Func_02001ad4
#define SceneActor_ApplyActorTwelveZCellPair Func_02001af4
#define SceneActor_RunSlot12ColumnStep Func_02001b1c
#define SceneActor_IsActorAtTile Func_02001be8
#define SceneData_InitTableA980 Func_02001c14
#define SceneData_FillTableA980 Func_02001d14
#define SceneData_InitTableA980AndRunB Func_02001e10
#define SceneData_BuildTableA980 Func_02001f0c
#define FieldScene_RunSceneSetupWhenFlag816Clear Func_02002244
#define FieldScene_RunSceneSetupWhenFlag817Clear Func_020022f4
#define FieldScene_RunSplitPairSteps Func_020023a4

struct Ent {
    s32 f00;
    u8 f04;
    u8 f05;
    u16 f06:9;
    u16 f07:7;
    s32 f08;
};

struct Cam {
    u8 pad[228];
    s32 x;
    s32 y;
};

extern struct Cam *Data_03001e70;
extern struct Ent Data_0200aa50[];
u16 *Func_02003456();
void Func_020035ba();
void Func_0200357a();
void Func_0200372e();
void Func_020034da();
extern u8 SceneEventRuntime_ScriptData[];
extern u8 SceneEventRuntime_MessageData[];
extern u8 SceneEventRuntime_ActorData[];
extern u8 SceneEventRuntime_EffectData[];
extern u8 Data_03001ebc[];
extern s32 Data_0200a974;
extern u8 Data_02000240[];
extern u16 Data_02000240_t[][1];
extern s32 Data_0200a980[];

s32 Func_0200248e(s32);
s32 Func_02002498(s32);
s32 Func_020024a6(s32);
void Func_020024b2(s32, s32, s32, s32, s32, s32);
void Func_02002618(s32);
void Func_020024ce(s32, s32, s32, s32, s32, s32);
void Func_0200251c(s32);
void Func_020024e6(s32, s32, s32, s32, s32, s32);
void Func_02002534(s32);
void Func_02002504(s32, s32, s32, s32, s32, s32);
void Func_02002514(s32, s32, s32, s32, s32, s32);
void Func_02002526(s32, s32, s32, s32, s32, s32);
void Func_02002542(s32, s32, s32, s32, s32, s32);
void Func_02002578(s32);
void Func_0200181c(void);
void Func_0200253e();
void Func_0200259c();
void Func_020025c4();
void Func_020025d8();
void Func_0200262a();
void Func_0200262c();
void Func_0200263c();
void Func_02002642();
void Func_0200264a();
void Func_0200264c();
void Func_02002652();
void Func_02002664();
void Func_0200266e();
void Func_0200266e_a();
void Func_02002686();
void Func_020026a0();
void Func_020026a0_a();
void Func_020026a6();
void Func_020026ae();
void Func_020026b2();
void Func_020026b2_a();
void Func_020026c0();
void Func_020026c2();
void Func_020026c4();
void Func_020026c4_a();
void Func_020026c6();
void Func_020026da();
void Func_020026dc();
void Func_020026ea();
void Func_020026fa();
void Func_02002700();
void Func_02002704();
void Func_02002706();
void Func_02002710();
void Func_02002722();
void Func_0200272c();
void Func_02002732();
void Func_02002734();
void Func_02002734_a();
void Func_0200273a();
void Func_0200273e();
void Func_0200274c();
void Func_02002750();
void Func_02002758();
void Func_02002762();
void Func_02002766();
void Func_02002772();
void Func_0200277c();
void Func_02002798();
void Func_020027a4();
void Func_020027a6();
void Func_020027ba();
void Func_020027c2();
void Func_020027c8();
void Func_020027e8();
void Func_020027f2();
void Func_020027f6();
void Func_0200280a();
void Func_02002812();
void Func_0200281c();
void Func_0200281e();
void Func_0200282c();
void Func_0200283a();
void Func_0200283a_a();
void Func_02002840();
void Func_02002848();
void Func_0200284a();
void Func_02002850();
void Func_02002858();
void Func_0200285a();
void Func_0200285a_a();
void Func_0200285e();
void Func_02002868();
void Func_0200286a();
void Func_0200287a();
void Func_02002880();
void Func_02002886();
void Func_0200288a();
void Func_0200288c();
void Func_0200289a();
void Func_020028bc();
void Func_020028be();
void Func_020028c4();
void Func_020028ca();
void Func_020028e2();
void Func_020028e6();
void Func_020028f0();
void Func_020028fa();
void Func_020028fa_a();
void Func_020028fc();
void Func_020028fc_a();
void Func_0200290c();
void Func_0200291e();
void Func_0200292c();
void Func_02002934();
void Func_0200293a();
void Func_02002944();
void Func_0200294e();
void Func_02002952();
void Func_0200295a();
void Func_02002966();
void Func_02002974();
void Func_0200297c();
void Func_0200298e();
void Func_02002998();
void Func_020029a8();
void Func_020029aa();
void Func_020029b8();
void Func_020029c0();
void Func_020029c2();
void Func_020029dc();
void Func_020029f8();
s32 Func_02002a06();
void Func_02002a20();
void Func_02002a28();
s32 Func_02002a36();
s32 Func_02002a36_a();
void Func_02002a3a();
void Func_02002a46();
void Func_02002a4e();
void Func_02002a58();
void Func_02002a60();
void Func_02002a64();
void Func_02002a70();
void Func_02002a7e();
void Func_02002a90();
void Func_02002a02();
void Func_02002a52();
void Func_02002a6c();
void Func_02002a88();
void Func_02002a9c();
void Func_02002ae2();
void Func_02002aee();
void Func_02002af2();
void Func_02002af8();
void Func_02002b08();
void Func_02002b08_a();
void Func_02002b1a();
void Func_02002b24();
void Func_02002b32();
void Func_02002b36();
void Func_02002b3c();
void Func_02002b56();
void Func_02002b5c();
void Func_02002b64();
void Func_02002b6a();
void Func_02002b76();
void Func_02002b76_a();
void Func_02002b7a();
void Func_02002b7a_a();
void Func_02002b82();
void Func_02002b8a();
void Func_02002b92();
void Func_02002ba0();
void Func_02002bb6();
void Func_02002bbc();
void Func_02002bc6();
void Func_02002bce();
void Func_02002be2();
void Func_02002bea();
void Func_02002bf0();
void Func_02002c02();
void Func_02002c02_a();
void Func_02002c0e();
void Func_02002c10();
void Func_02002c18();
void Func_02002c18_a();
void Func_02002c1c();
void Func_02002c20();
void Func_02002c28();
void Func_02002c30();
void Func_02002c3a();
void Func_02002c4e();
void Func_02002c54();
void Func_02002c58();
void Func_02002c62();
void Func_02002c6c();
void Func_02002c7c();
void Func_02002c82();
void Func_02002c8a();
void Func_02002c92();
void Func_02002c98();
void Func_02002cae();
void Func_02002cb2();
void Func_02002cbc();
void Func_02002cbc_a();
void Func_02002cc4();
void Func_02002cce();
void Func_02002cce_b();
void Func_02002cce_a();
void Func_02002cd2();
void Func_02002cda();
void Func_02002cde();
void Func_02002cea();
void Func_02002d00();
void Func_02002d08();
void Func_02002d1e();
void Func_02002d24();
void Func_02002d34();
void Func_02002d3e();
void Func_02002d68();
void Func_02002d68_a();
void Func_02002d6e();
void Func_02002d82();
void Func_02002d84();
void Func_02002d8c();
void Func_02002d8e();
void Func_02002d98();
void Func_02002da4();
void Func_02002daa();
void Func_02002db0();
void Func_02002dbc();
void Func_02002dbe();
void Func_02002dc2();
void Func_02002dc8();
void Func_02002dd4();
void Func_02002dd4_a();
void Func_02002de6();
void Func_02002dfc();
void Func_02002e00();
void Func_02002e0e();
void Func_02002e14();
void Func_02002e18();
void Func_02002e18_a();
void Func_02002e20();
void Func_02002e24();
void Func_02002e30();
void Func_02002e3e();
void Func_02002e4a();
void Func_02002e4e();
void Func_02002e64();
void Func_02002e70();
void Func_02002e76();
void Func_02002e9e();
void Func_02002ea2();
void Func_02002ea8();
void Func_02002eb2();
void Func_02002eb2_a();
s32 Func_02002eb4();
void Func_02002eca();
s32 Func_02002ed0();
void Func_02002ede();
void Func_02002ee4();
void Func_02002eec();
void Func_02002ef8();
void Func_02002f04();
void Func_02002f08();
void Func_02002f24();
void Func_02002f36();
void Func_02002f3c();
void Func_02002f3e();
void Func_02002f4c();
void Func_02002f4e();
void Func_02002f56();
void Func_02002f5c();
void Func_02002f66();
void Func_02002f6a();
void Func_02002f72();
void Func_02002f82();
void Func_02002f84();
void Func_02002f90();
void Func_02002f9e();
void Func_02002faa();
void Func_02002fdc();
void Func_02002fde();
void Func_02002fee();
void Func_02002ff4();
void Func_02002ff8();
void Func_02002ffc();
void Func_0200300c();
void Func_02003012();
void Func_02003026();
void Func_0200303c();
void Func_0200304a();
s32 Func_02003050();
void Func_0200305e();
void Func_0200306e();
void Func_02003072();
void Func_02003076();
s32 Func_02003080();
s32 Func_02003080_a();
void Func_0200308a();
void Func_0200308a_a();
void Func_02003090();
void Func_02003098();
void Func_020030a2();
void Func_020030a4();
void Func_020030aa();
void Func_020030b0();
void Func_020030b2();
void Func_020030ba();
void Func_020030c8();
void Func_020030da();
s32 *Func_02003112(s32);
void Func_0200311a(s32);
void Func_02003148(s32);
void Func_02003150();
void Func_02003152(s32);
void Func_02003172(s32);
void Func_02003198(s32, s32, s32, s32, s32, s32);
void Func_0200319e(s32);
void Func_020031a8_a();
void Func_020031a8_b(s32);
void Func_020031bc_a();
void Func_020031bc_b(s32, s32, s32, s32, s32, s32);
void Func_020031cc(s32);
void Func_020031e6(s32);
void Func_020031fa(s32, s32, s32, s32, s32, s32);
void Func_0200320a(s32);
void Func_02003214_a();
void Func_02003214_b(s32, s32, s32, s32, s32, s32);
void Func_02003224(s32);
void Func_02003232(s32, s32, s32, s32, s32, s32);
void Func_02003244(s32, s32, s32, s32, s32, s32);
void Func_02003248(s32);
void Func_02003262(s32);
void Func_02003292(s32);
void Func_020032ac(s32);
void Func_020032ce(s32);
void Func_02003318(struct Ent *, s32);
void Func_02003358(struct Ent *, s32);
void Func_0200339c(struct Ent *, s32);
void Func_0200352c(void);
u16 *Func_02003472(s32);
void Func_02003548(void);
void Func_0200347e();
void Func_0200349c();
void Func_020034a6();
void Func_020034ae();
void Func_0200157c();
void Func_02001a82();
void Func_0200252c();
void Func_020034b4();
void Func_020034b8();
s32 Func_020034ee();
u8 *Func_02003506();
s32 Func_02003512();
s32 Func_0200351c();
void Func_0200354a();
void Func_02003568();
void Func_0200357c();
void Func_02003586();
void Func_0200359e();
void Func_020035ac();
void Func_020035b4();
s32 Func_020035be();
s32 Func_020035c8();
s32 Func_020035c8_a();
void Func_020035cc();
void Func_020035d6();
void Func_020035f0();
void Func_020035f8();
void Func_02003610();
void Func_02003620();
void Func_0200363c();
void Func_0200364a();
void Func_02003658();
void Func_02003662();
s32 Func_0200366c();
void Func_0200367a();
void Func_0200368e();
s32 Func_02003698();
void Func_0200369e();
void Func_020036ba();
s32 Func_020036c4();
void Func_020036ca();
void Func_020036de();
void Func_020036ee();
void Func_0200370a();
s32 Func_02003714();
void Func_0200371a();
void Func_0200371a_a();
void Func_02003746();
void Func_0200375a();
s32 Func_02003764();
void Func_0200376a();
void Func_0200377c();
void Func_02003796();
void Func_020037a8();
s32 Func_020037b2();
void Func_020037b8();
void Func_020037ca();
void Func_020037e6();
s32 Func_02003810();
void Func_02003834();
s32 Func_02003858();
void Func_0200386e();
s32 Func_0200387a();
void Func_0200388e();
s32 Func_02003890();
void Func_02003898();
s32 Func_0200389a();
void Func_020038aa();
s32 Func_020038ae();
void Func_020038b0();
void Func_020038b0_a();
s32 Func_020038be();
s32 Func_020038e6();
s32 Func_020038e8();
void Func_020038f8();
void Func_02003902();
s32 Func_02003910();
s32 Func_02003914();
void Func_02003920();
void Func_02003926();
void Func_020039d4();
void Func_020039e0();
s32 Func_020038be_a();
s32 Func_02003166(s32, s32, s32);
void Func_0200319a(void);
void Func_020032b2(void);
s32 Func_02003196(s32, s32, s32);
void Func_020033c6(void);
s32 Func_020031ae(s32, s32, s32);
void Func_02003a08(s32, s32, s32, s32);
void Func_0200383a();
s32 Func_020031fe();
void Func_02003912();
void Func_02003a30();
void Func_02003a78();
void Func_02003a88();
s32 Func_02003a96();
s32 Func_02003aa0();
void Func_02003ac0();
s32 Func_02003ade();
s32 Func_02003ae8();
void Func_02003af8();
s32 Func_02003b16();
s32 Func_02003b20();
void Func_02003b1a();
void Func_02003b2a();
u8 *Func_02003b50();
s32 Func_02003b54();
void Func_02003b68();
s32 Func_02003b82();
s32 Func_02003b8e();
s32 Func_02003b96();
void Func_02003ba2();
s32 Func_02003bae();
void Func_02003bd0();
s32 Func_02003bfa();
s32 Func_02003c1a();
s32 Func_02003c1a_a();
void Func_02003c28();
void Func_02003c44();
void Func_02003c44_a();
void Func_02003c5a();
void Func_02003c5e();
void Func_02003c70();
void Func_02003c7a();
void Func_02003c80();
void Func_02003c86();
void Func_02003c86_a();
void Func_02003c90();
void Func_02003c9c();
void Func_02003ca0();
void Func_02003ca0_a();
void Func_02003ca6();
void Func_02003cac();
void Func_02003cb2();
void Func_02003cb8();
void Func_02003cbc();
void Func_02003ccc();
void Func_02003cd2();
void Func_02003cee();
s32 Func_02003cf6();
s32 Func_02003d00();
void Func_02003d02();
s32 Func_02003d0a();
s32 Func_02003d14();
void Func_02003d18();
void Func_02003d20();
void Func_02003d24();
void Func_02003d26();
void Func_02003d2a();
void Func_02003d3c();
void Func_02003d7c();
void Func_02003d7e();
void Func_02003df0();
void Func_02003e20();
void Func_02003dc6();
s32 Func_02003dce();
void Func_02003df6();
s32 Func_02003dfe();
void Func_02003e00();
s32 Func_02003e16();
void Func_02003e30();
void Func_02003e32();
void Func_02003e3e();
void Func_02003e50();
void Func_02003e64();
void Func_02003e7c();
void Func_02003edc();
void Func_0200353a(s32, s32, s32, s32, s32);
void Func_0200355a(s32, s32, s32, s32, s32);
void Func_0200384a(void);
void Func_0200359a(s32, s32, s32, s32, s32);
void Func_02003966(void);
void Func_02003cbe(void);
u8 *Func_02003ef8(s32 index);
void Func_020035e6();
s32 *Func_02003f20(s32);
void Func_0200362a(s32, s32, s32, s32, s32);
void Func_02003dde(void);
s32 *Func_02003f68(s32);
void Func_02003656(s32, s32, s32, s32, s32);
s32 *Func_02003f90(s32);
void Func_0200367e(s32, s32, s32, s32, s32);
void Func_02003fb0(void);
void Func_02003fe0();
u8 *Func_02003fe8(s32);
void Func_02003fea();
void Func_0200401c(s32, s32, s32);
void Func_0200402a(void);
void Func_02004038(s32, s32, s32);
void Func_02004042(s32, s32);
void Func_0200405e(s32);
void Func_02004076(s32, s32);
void Func_020040c6(s32);
s32 *Func_0200405c(s32);
void Func_02003cec(void);
void Func_02003de6(void);
void Func_02003ee4(void);
void Func_02003fcc(void);
void Func_02002372();
s32 Func_02004684();
s32 Func_0200468e();
void Func_0200469e();
void Func_0200469e_a();
void Func_020046ba();
s32 Func_020046d4();
s32 Func_02004706();
void Func_02004708();
void Func_02004712();
void Func_0200473a();
void Func_0200477e();
void Func_02004798();
void Func_020047a4();
void Func_020047b4();
void Func_020047ea();
void Func_02002424();
s32 Func_02004734();
s32 Func_0200473e();
void Func_0200474e();
void Func_0200474e_a();
void Func_0200476a();
s32 Func_02004784();
s32 Func_020047b8();
void Func_020047ba();
void Func_020047c4();
void Func_020047ec();
void Func_0200482e();
void Func_02004848();
void Func_02004854();
void Func_02004866();
void Func_0200489a();
void Func_02004898(s32, s32);
void Func_020047fe(s32);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* A constant call routed through a wrapper places its operands straight into
 * the argument registers; a direct call hoists the pool constant first. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Argument shared by every configuration call below. */

/* Condition code passed to the gating check; the body below runs only when
 * that check reports 0. */

/* If the code-2059 check passes, runs a short setup/configuration sequence
 * for id 9: two no-argument calls bracket a select call and two calls each
 * taking a pair of numeric arguments. */

/* Contiguous unnamed leaf-owner run for resource_37b. */

/*
 * resource_37b owner at 0x02001a84, 40 bytes: fetches scene record 10
 * and, if it exists, hands a coarse coordinate derived from it to a
 * five-argument in-overlay routine.
 *
 * Complete owner: `push {lr}` + `sub sp, #4` at 0x02001a84 through the
 * single epilogue `add sp, #4 / pop {r0} / bx r0` at
 * 0x02001aa4-0x02001aa8, no literal pool. Next owner at 0x02001aac. The
 * halfword at 0x02001a82 is alignment belonging to the previous owner,
 * not to this one -- which is why sweep D reports the gap as starting
 * two bytes early. Measured to the epilogue. Takes no argument.
 *
 * Call sites resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * resource_37b 1a84 1aac`'s `+2` rule: `sites=2 veneer=1 prologue=1`,
 * and two are transcribed. The second is in-overlay, to Func_02001b44.
 *
 * Published, not called: its address is stored as 0x02009a84 in exactly
 * one word, at overlay offset 0x28ec -- in the data region past the last
 * owner, so it is installed from a TABLE and no drafted row points at
 * it. That is the same shape as resource_3c9's 0x02000518.
 *
 * The stack slot is a FIFTH ARGUMENT, not a local. `sub sp, #4` reserves
 * one word, `str r3, [sp, #0]` writes it immediately before the call,
 * and nothing ever reads it back -- so it is the argument that did not
 * fit in r0-r3, and the callee takes five. The value stored is the
 * coordinate BEFORE the increment, while r2 carries the same coordinate
 * PLUS ONE, so the callee receives both.
 *
 * Uncertainties: `>> 20` on the +16 field is the established coarse-cell
 * shape in this tree (a 16.16-style fixed-point field reduced to a cell
 * index), but nothing here proves the unit. The three constants 10, 13
 * and 13 are transcribed as written; the repeat of 13 in r1 and r3 is
 * real and not a transcription slip. Func_02001b44 is not drafted, so
 * it is declared old-style rather than given a signature this row cannot
 * establish.
 */

   /* scene-record accessor */

/* Constants passed through a call wrapper reach the argument registers
 * directly; a direct call would share one pseudo across both calls. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void ConfigureScene(s32 actor, s32 x, s32 y, s32 mode)
{
    Func_02003a08(actor, x, y, mode);
}

static __inline__ void ConfigureScene_02003a30(s32 actor, s32 x, s32 y, s32 mode)
{
    Func_02003a30(actor, x, y, mode);
}

void SceneActor_UseActorTenCellAndNext(void)
{
    u8 *record = Func_02003ef8(10);
    s32 cell;

    if (record == 0) {
        return;
    }

    cell = *(s32 *)(record + 16) >> 20;
    Func_020035e6(10, 13, cell + 1, 13, cell);
}

void SceneActor_MoveActor10ByRow(void) {
    s32 *p = Func_02003f20(10);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_0200360e(10, 13, v - 1, 13, v);
    }
}

void SceneActor_ApplyActorTwelveZCellPair(void) {
    s32 *p = Func_02003f68(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_02003656(12, 22, v + 1, 22, v);
    }
}

void SceneActor_RunSlot12ColumnStep(void) {
    s32 *p = Func_02003f90(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_0200367e(12, 22, v - 1, 22, v);
    }
}

void SceneActor_PlaceActorPairAtCells(s32 actor, s32 x1, s32 y1, s32 x2, s32 y2)
{
    Func_02003fb0();
    Func_020040c6(185);
    Call3(Func_02003fe0, actor, 0x3333, 0x1999);
    Call3(Func_02003fea, 0, 0x3333, 0x1999);
    Func_02003fe8(actor)[90] &= 0xfe;
    Func_02004042(0, 8);
    Func_0200401c(0, (x2 << 4) + 8, (y2 << 4) + 8);
    x1 <<= 4;
    y1 <<= 4;
    Func_02004038(actor, x1 + 8, y1 + 8);
    Func_0200405e(actor);
    Func_02004076(0, 1);
    Func_0200402a();
}

s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z) {
    s32 *p = Func_0200405c(no);
    if (p == NULL || (p[2] >> 20) != x) {
        return 0;
    }
    if ((p[4] >> 20) != z) {
        return 0;
    }
    return 1;
}

void SceneData_InitTableA980(void) {
    s32 *p = Data_0200a980;
    p[0] = 0;
    p[1] = 55;
    p[2] = 32;
    p[3] = 40;
    p[4] = 4;
    p[5] = 3;
    p[6] = 2;
    p[7] = 30;
    p[8] = 34;
    p[9] = 10;
    p[10] = 2;
    p[11] = 1;
    p[12] = 2;
    p[13] = 28;
    p[14] = 34;
    p[15] = 10;
    p[16] = 2;
    p[17] = 1;
    p[18] = 2;
    p[19] = 30;
    p[20] = 16;
    p[21] = 10;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80b;
    p[25] = 0x4000;
    p[26] = 500;
    p[27] = 132;
    p[28] = 8;
    p[29] = 55;
    p[30] = 32;
    p[31] = 40;
    p[32] = 4;
    p[33] = 3;
    p[34] = 2;
    p[35] = 30;
    p[36] = 34;
    p[37] = 10;
    p[38] = 2;
    p[39] = 1;
    p[40] = 2;
    p[41] = 28;
    p[42] = 16;
    p[43] = 10;
    p[44] = 2;
    p[45] = 1;
    p[46] = 9;
    p[47] = 488;
    p[48] = 152;
    Func_02003cec();
}

void SceneData_FillTableA980(void) {
    s32 *p = Data_0200a980;
    p[0] = 4;
    p[1] = 55;
    p[2] = 36;
    p[3] = 40;
    p[4] = 4;
    p[5] = 3;
    p[6] = 4;
    p[7] = 30;
    p[8] = 36;
    p[9] = 10;
    p[10] = 2;
    p[11] = 1;
    p[12] = 4;
    p[13] = 28;
    p[14] = 36;
    p[15] = 10;
    p[16] = 2;
    p[17] = 1;
    p[18] = 4;
    p[19] = 30;
    p[20] = 18;
    p[21] = 10;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80c;
    p[25] = 0x4000;
    p[26] = 654;
    p[27] = 132;
    p[28] = 12;
    p[29] = 55;
    p[30] = 36;
    p[31] = 40;
    p[32] = 4;
    p[33] = 3;
    p[34] = 4;
    p[35] = 30;
    p[36] = 36;
    p[37] = 10;
    p[38] = 2;
    p[39] = 1;
    p[40] = 4;
    p[41] = 28;
    p[42] = 18;
    p[43] = 10;
    p[44] = 2;
    p[45] = 1;
    p[46] = 11;
    p[47] = 664;
    p[48] = 152;
    Func_02003de6();
}

void SceneData_InitTableA980AndRunB(void) {
    s32 *p = Data_0200a980;
    p[0] = 0;
    p[1] = 58;
    p[2] = 32;
    p[3] = 43;
    p[4] = 4;
    p[5] = 1;
    p[6] = 2;
    p[7] = 31;
    p[8] = 34;
    p[9] = 11;
    p[10] = 2;
    p[11] = 1;
    p[12] = 2;
    p[13] = 29;
    p[14] = 34;
    p[15] = 11;
    p[16] = 2;
    p[17] = 1;
    p[18] = 2;
    p[19] = 31;
    p[20] = 16;
    p[21] = 11;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80d;
    p[25] = 0xc000;
    p[26] = 500;
    p[27] = 216;
    p[28] = 8;
    p[29] = 58;
    p[30] = 32;
    p[31] = 43;
    p[32] = 4;
    p[33] = 1;
    p[34] = 2;
    p[35] = 31;
    p[36] = 34;
    p[37] = 11;
    p[38] = 2;
    p[39] = 1;
    p[40] = 2;
    p[41] = 29;
    p[42] = 16;
    p[43] = 11;
    p[44] = 2;
    p[45] = 1;
    p[46] = 13;
    p[47] = 488;
    p[48] = 200;
    Func_02003ee4();
}

void SceneData_BuildTableA980(void) {
    s32 *p = Data_0200a980;
    p[0] = 4;
    p[1] = 58;
    p[2] = 36;
    p[3] = 43;
    p[4] = 4;
    p[5] = 1;
    p[6] = 4;
    p[7] = 31;
    p[8] = 36;
    p[9] = 11;
    p[10] = 2;
    p[11] = 1;
    p[12] = 4;
    p[13] = 29;
    p[14] = 36;
    p[15] = 11;
    p[16] = 2;
    p[17] = 1;
    p[18] = 4;
    p[19] = 31;
    p[20] = 18;
    p[21] = 11;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80e;
    p[25] = 0xc000;
    p[26] = 0x28e;
    p[27] = 216;
    p[28] = 12;
    p[29] = 58;
    p[30] = 36;
    p[31] = 43;
    p[32] = 4;
    p[33] = 1;
    p[34] = 4;
    p[35] = 31;
    p[36] = 36;
    p[37] = 11;
    p[38] = 2;
    p[39] = 1;
    p[40] = 4;
    p[41] = 29;
    p[42] = 18;
    p[43] = 11;
    p[44] = 2;
    p[45] = 1;
    p[46] = 15;
    p[47] = 664;
    p[48] = 200;
    Func_02003fcc();
}

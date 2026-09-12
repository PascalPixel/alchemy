#include "types.h"

#define BattleRuntime_Reset_1() Value0(Func_02006a84)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02006b94, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02006bae, a0, a1, a2, a3)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02006ad8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Value3(Func_02006b1c, a0, a1, a2)
#define Audio_PlayCue_1(a0) Value1(Func_02006c6a, a0)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02006b0e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02006b18_a, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_02006b70, a0, a1)
#define ObjectMotion_ResetAndSetPosition_1(a0, a1, a2) Call3(Func_02006b4c, a0, a1, a2)
#define ObjectMotion_ResetAndSetPosition_2(a0, a1, a2) Value3(Func_02006b58, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Func_02006b06, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02006c0a, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(a0, a1) Value2(Func_02006bba, a0, a1)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(a0) Value1(Func_02006b98, a0)
#define SharedWorkData_SetFirstAndSecondFields_1(a0, a1) Call2(Func_02006c72, a0, a1)
#define BattleEffect_ComputeWeightedResultAndDispatch_1(a0, a1) Call2(Func_02006c6a_a, a0, a1)
#define WORK_BYTE_22B (Data_02000240[0x22B])
#define FieldScene_RunEarlySequence Func_020006dc
#define FieldScene_RunScene3a4SequenceB Func_020009ec
#define FieldScene_RunScene3a4SequenceA Func_02000a94
#define FieldScene_RunLine1528Sequence Func_02000b0c
#define FieldScene_RunScene3a4SequenceC Func_02000b3c
#define FieldScene_RunScene3a4SequenceD Func_02000bd8
#define FieldScene_RunScene3a4_02000c9c Func_02000c9c
#define FieldScene_RunSharedSetPiece Func_02000ec0
#define FieldScene_RunScene3a4_02001398 Func_02001398
#define FieldScene_RunScene3a4_02002310 Func_02002310
#define FieldScene_RunScene3a4_02002428 Func_02002428
#define FieldScene_RunScene3a4_02002490 Func_02002490
#define FieldScene_RunScene3a4_020025c0 Func_020025c0
#define FieldScene_RunScene3a4_020026c0 Func_020026c0
#define FieldScene_RunScene3a4_02002934 Func_02002934
#define FieldScene_RunScene3a4_020029dc Func_020029dc
#define FieldScene_RunScene3a4SequenceG Func_02002be0
#define FieldScene_RunOpeningAuxiliarySequence Func_02002d58
#define FieldScene_RunScene3a4SequenceF Func_02002e1c
#define FieldScene_RunScene3a4SequenceE Func_02002e84
#define FieldScene_RunLateAuxiliarySequence Func_02002f10

extern u8 Data_00000000[];
extern u8 Data_0200d238[];
extern u8 Data_03001ebc[];
extern u8 Data_00001953[];
extern s16 Data_02000240_t[][1];
extern u8 Value_00000056;

void Func_0200418e();
void Func_020041d6();
void Func_020041ec();
void Func_02004208();
void Func_02004230();
void Func_02004236();
void Func_02004266();
void Func_020042ae();
s32 Func_020042b8();
s32 Func_020042c0();
void Func_020042ca();
void Func_020042e0();
void Func_020042e8();
void Func_020042ec();
void Func_020042f6();
void Func_020042fc();
void Func_02004320();
void Func_02004322();
void Func_0200432a();
void Func_0200432c();
void Func_02004350();
void Func_02004386();
void Func_020043c2();
void Func_020043d8();
void Func_020043da();
void Func_020043de();
void Func_020043f4();
void Func_020043fa();
void Func_02004408();
void Func_0200440e();
void Func_02004410();
void Func_02004418();
void Func_0200441e();
void Func_0200443e();
void Func_02004464();
void Func_0200449a();
void Func_020044a4();
void Func_020044d6();
void Func_020044ec();
void Func_02004522();
void Func_02004528();
void Func_0200452c();
void Func_02004598();
void Func_0200463c();
void Func_020046ac();
void Func_02004322_a();
void Func_0200440e_a();
void Func_02004528_a();
void Func_02004534();
s32 Func_02004544();
void Func_02004558();
void Func_02004564();
void Func_02004586();
void Func_020045ae();
void Func_020045d0();
void Func_020045dc();
s32 Func_020045ec();
void Func_02004600();
void Func_0200460c();
void Func_0200462e();
void Func_02004656();
void Func_02004680(void);
void Func_02004660(s32, s32);
void Func_0200483e(s32);
void Func_020015b6(void);
void Func_020045a0(s32);
void Func_02004844(void);
void Func_020046a8(void);
void Func_02004636();
void Func_0200468a();
void Func_02004692();
void Func_02004696();
s32 Func_020046a6();
void Func_020046b2();
void Func_020046ba();
void Func_020046c6();
void Func_020046e8();
void Func_02004710();
void Func_0200473e();
void Func_02004870();
void Func_020048da();
void Func_020046d2();
void Func_02004726();
void Func_0200472e();
void Func_02004732();
s32 Func_02004742();
void Func_0200474e();
void Func_02004756();
void Func_02004762();
void Func_02004784();
void Func_020047ac();
void Func_020047da();
void Func_0200490c();
void Func_02004976();
void Func_02001122();
void Func_020047de();
void Func_02004812();
void Func_02004866();
s32 Func_0200486e();
void Func_02004870_a();
void Func_02004892();
void Func_020048b6();
void Func_020048c4();
void Func_020048d8();
void Func_020048ea();
void Func_02004902();
void Func_020049dc();
void Func_02004a68();
s32 Func_02004a6e();
s32 Func_02004a76();
s32 Func_02004a7e();
s32 Func_02004a86();
void Func_02004aac();
void Func_02004acc();
void Func_02004ae2();
void Func_02004b08();
void Func_02004b12();
void Func_02004b24();
void Func_02004b2e();
void Func_02004b38();
void Func_02004b48();
void Func_02004b48_a();
void Func_02004b4c();
void Func_02004b62();
void Func_02004b62_a();
void Func_02004b6a();
void Func_02004b84();
void Func_02004b90();
void Func_02004b9e();
void Func_02004ba4();
s32 Func_02004bbe();
s32 Func_02004bbe_a();
void Func_02004bfa();
void Func_02004c10();
void Func_02004c14();
u8 *Func_02004c1c();
void Func_02004c4a();
void Func_02004c56();
void Func_02004c5e();
void Func_02004c60();
void Func_02004c66();
void Func_02004c76();
void Func_02004c7c();
void Func_02004c84();
void Func_02004c92();
u8 *Func_02004c98();
void Func_02004c9a();
void Func_02004c9e();
void Func_02004cb2();
void Func_02004cba();
void Func_02004cd2();
void Func_02004cda();
void Func_02004cda_a();
void Func_02004cec();
void Func_02004d00();
void Func_02004d0c();
void Func_02004d0e();
void Func_02004d1e();
void Func_02004d20();
void Func_02004d26();
void Func_02004d28();
void Func_02004d2c();
void Func_02004d38();
void Func_02004d42();
void Func_02004d44();
s32 Func_02004d4c();
s32 Func_02004d4c_a();
void Func_02004d52();
void Func_02004d5e();
s32 Func_02004d86();
void Func_02004d9c();
void Func_02004da6();
void Func_02004dae();
void Func_02004dc4();
s32 Func_02004de6();
void Func_02004dea();
void Func_02004df8();
void Func_02004e08();
void Func_02004e30();
void Func_02004e32();
void Func_02004e60();
s32 Func_02004e74();
void Func_02004e9c();
void Func_02004e9e();
void Func_02004ea6();
void Func_02004eaa();
void Func_02004eae();
void Func_02004ec6();
void Func_02004ec6_a();
void Func_02004ed2();
void Func_02004ed4();
void Func_02004ee6();
void Func_02004f00();
void Func_02004f02();
void Func_02004f0c();
void Func_02004f1c();
void Func_02004f32();
void Func_02004f3e();
void Func_02004faa();
void Func_0200500a();
void Func_0200abe1();
void Func_020024fe();
s32 Func_02004eee();
s32 Func_02004efc();
s32 Func_02004f08();
void Func_02004f0c_a();
void Func_02004f1c_a();
void Func_02004f6e();
s32 Func_02004f8e();
void Func_02004fa2();
void Func_02004fae();
void Func_02004fb0();
void Func_02004fbc();
void Func_02004fda();
void Func_02004fe4();
void Func_02004fec();
void Func_02004ff2();
s32 Func_02004ff6();
void Func_02004ff8();
void Func_02005016();
s32 Func_0200501a();
void Func_02005024();
void Func_0200502a();
void Func_02005032();
s32 Func_02005040();
s32 Func_02005040_a();
void Func_0200504c();
void Func_0200504e();
void Func_0200504e_a();
void Func_0200505c();
u8 *Func_02005060();
void Func_02005068();
void Func_02005076();
void Func_0200508c();
void Func_02005094();
void Func_02005146();
void Func_0200516a();
void Func_02005170();
void Func_02005174();
void Func_0200517a();
void Func_02005182();
void Func_02005184();
void Func_02005190();
void Func_02005196();
void Func_02005198();
void Func_0200519c();
void Func_0200519e();
void Func_020051a0();
void Func_020051a6();
void Func_020051b0();
void Func_020051b2();
void Func_020051b2_a();
void Func_020051ba();
void Func_020051bc();
void Func_020051be();
void Func_020051c8();
void Func_020051d2();
void Func_020051d6();
void Func_020051da();
void Func_020051e0();
void Func_020051e8();
void Func_0200520a();
void Func_02005214();
void Func_02005218();
void Func_0200521a();
void Func_0200522c();
void Func_02005276();
void Func_02004f5c();
void Func_02005172();
void Func_02005d7c();
s32 Func_02005e68();
s32 Func_02005e8e();
void Func_02005e94();
void Func_02005ea8();
s32 Func_02005ed6();
void Func_02005ee6();
void Func_02005ef2();
s32 Func_02005f02();
void Func_02005f10();
void Func_02005f1a();
void Func_02005f1c();
void Func_02005f8a();
void Func_02005faa();
void Func_02005f7e();
s32 Func_02005f80();
void Func_02005fd2();
void Func_020050b4();
void Func_02005356();
void Func_0200596a();
void Func_02005f1a_a();
s32 Func_02005fe8();
s32 Func_0200600a();
s32 Func_0200602c();
void Func_0200605a();
s32 Func_02006062();
void Func_0200607c();
void Func_02006088();
s32 Func_02006098();
void Func_0200609a();
void Func_020060a6();
void Func_020060b2();
u8 *Func_020060d0();
void Func_020060e0();
void Func_02006100();
void Func_0200610e();
void Func_02005a50();
s32 Func_02006128();
s32 Func_02006148();
void Func_0200614a();
void Func_0200617a();
s32 Func_02006180();
void Func_02006186();
s32 Func_02006196();
u8 *Func_0200619a();
void Func_020061a4();
void Func_020061b0();
void Func_020061d2();
void Func_020061fc();
void Func_02002ade();
void Func_02005b38();
void Func_02005b74();
void Func_02005bb0();
void Func_020061fe();
void Func_02006212();
void Func_02006226();
s32 Func_02006230();
u8 *Func_02006260();
void Func_02006262();
s32 Func_0200626a();
u8 *Func_0200629c();
void Func_020062a0();
s32 Func_020062a8();
void Func_020062cc();
u8 *Func_020062d8();
u8 *Func_020062d8_a();
void Func_020062dc();
s32 Func_020062e8();
void Func_020062f6();
void Func_02006302();
u8 *Func_02006316();
u8 *Func_02006316_a();
void Func_02006354();
s32 Func_0200648a();
void Func_02006492();
void Func_020064a6();
s32 Func_020064ea();
s32 Func_020064f4();
s32 Func_02006508();
s32 Func_02006510();
s32 Func_02006514();
void Func_0200651a();
void Func_0200652c();
void Func_02006532();
s32 Func_0200653c();
s32 Func_0200653c_a();
void Func_020059f0();
void Func_020065c2();
void Func_02006518();
s32 Func_0200657a();
void Func_020065ee();
s32 Func_0200665a();
s32 Func_02006682();
s32 Func_0200668a();
s32 Func_020066b2();
s32 Func_020066d4();
s32 Func_020066ec();
s32 Func_02006724();
s32 Func_0200673c();
void Func_02006758();
void Func_02006768();
void Func_02006794();
void Func_02006950();
void Func_02006958();
void Func_020034b4();
void Func_020068d8();
s32 Func_020068fa();
void Func_02006900();
s32 Func_02006930();
void Func_0200693c();
void Func_0200695a();
void Func_02006964();
void Func_0200696e();
void Func_02006974();
void Func_02006a32();
s32 Func_02006a3c();
void Func_02006a3e();
void Func_02006a4a();
void Func_02006a4a_a();
void Func_02006a56();
void Func_02006a60();
void Func_02006a64();
void Func_02006a70();
void Func_0200364c();
void Func_0200382e();
void Func_020068c4();
void Func_02006936();
void Func_02006990();
void Func_02006996();
void Func_020069e8();
void Func_02006a12();
void Func_02006a20();
void Func_02006b0c();
void Func_02006b18();
void Func_020037a0();
void Func_0200393e();
void Func_0200692c();
void Func_0200699e();
void Func_020069f8();
void Func_020069fe();
void Func_02006a50();
void Func_02006a7a();
void Func_02006a88();
void Func_02006b74();
void Func_02006b80();
void Func_020069d4();
void Func_02006a84();
void Func_02006a9c();
void Func_02006ad8();
void Func_02006b06();
void Func_02006b0e();
void Func_02006b18_a();
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
void Func_02006c6a_a();
void Func_02006c72();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Byte at offset 0x22B of the shared work buffer. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000c9c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002310(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002490(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020025c0(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020026c0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002934(s32 (*f)(), s32 a0)
{
    extern s16 Data_02000240[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_020029dc(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240[];

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
static __inline__ void Call0(void (*f)())
{
    extern u8 Data_02000240[];

    f();
}

static __inline__ s32 Value0_02002f10(s32 (*f)())
{
    extern u8 Data_02000240[];

    return f();
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];

    return f(a0, a1, a2);
}

void FieldScene_RunEarlySequence(void)
{
    s32 i;
    s32 record;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Func_02004408(230);
    Call3(Func_02004230, 0x20000, 0x20000, 0x10000);
    Func_02004266(10);
    rec = p5 + 356;
    for (i = 0; i <= 23; i++) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Func_0200418e(4);
        if (i == 8) {
            record = Value1(Func_020042b8, 8);
            *(volatile s32 *)(record + 24) = 0x1999;
            record = Func_020042c0(8);
            *(volatile s32 *)(record + 28) = 0x1999;
            Call3(Func_02004320, 8, 0x980000, 0xd80000);
            Call2(Func_020042e8, 8, 0x200bd48);
        }
    }
    Func_02004208(1, 0, 0x20086a1);
    *(u16 *)Data_0200d238 = 0;
    do {
        Func_020041d6(1);
        v = *(u16 *)Data_0200d238 + 1;
        *(u16 *)Data_0200d238 = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Func_020041ec(1);
    Func_02004236(1, 0, 0);
    Call1(Func_020044a4, 0x121);
    Call3(Func_020042ec, -1, -1, 0xe666);
    Func_02004322(30);
    Call6(Func_020042f6, 0, 0, 1, 2, 3, 14);
    Call1(Func_0200432c, 0x8fd);
}

void Func_020007e8(void)
{
    s32 i;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Call6(Func_02004322_a, 93, 41, 16, 4, 77, 28);
    Func_02004528(230);
    Call3(Func_02004350, 0x20000, 0x20000, 0x10000);
    Func_02004386(10);
    rec = p5 + 356;
    for (i = 23; i >= 0; i--) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Func_020042ae(4);
    }
    Func_020042fc(1, 0, 0x20086a1);
    *(u16 *)Data_0200d238 = 0;
    do {
        Func_020042ca(1);
        v = *(u16 *)Data_0200d238 + 1;
        *(u16 *)Data_0200d238 = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Func_020042e0(1);
    Func_0200432a(1, 0, 0);
    Call1(Func_02004598, 0x121);
    Call3(Func_020043d8, -1, -1, 0xe666);
    Func_0200440e(30);
    Call6(Func_020043da, 77, 41, 16, 4, 77, 28);
    Call1(Func_02004418, 0x8fe);
}

void Func_020008d4(void)
{
    s32 i;
    u8 *p8;
    u8 *rec;
    s32 v;

    p8 = *(u8 **)0x03001e70;
    Call6(Func_020043fa, 113, 31, 103, 17, 1, 1);
    Call6(Func_0200440e_a, 111, 32, 104, 18, 3, 2);
    Call6(Func_0200441e, 64, 32, 103, 18, 1, 2);
    Func_0200463c(230);
    Call3(Func_02004464, 0x20000, 0x20000, 0x10000);
    Func_0200449a(10);
    rec = p8 + 356;
    for (i = 23; i >= 0; i--) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Func_020043c2(4);
    }
    Func_02004410(1, 0, 0x20086a1);
    *(u16 *)Data_0200d238 = 0;
    do {
        Func_020043de(1);
        v = *(u16 *)Data_0200d238 + 1;
        *(u16 *)Data_0200d238 = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Func_020043f4(1);
    Func_0200443e(1, 0, 0);
    Call1(Func_020046ac, 0x121);
    Call3(Func_020044ec, -1, -1, 0xe666);
    Func_02004522(30);
    Call6(Func_020044d6, 103, 14, 103, 17, 4, 3);
    Call1(Func_0200452c, 0x907);
}

void FieldScene_RunScene3a4SequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02004544, 0x323) != 0) {
        Call6(Func_02004534, 2, 0, 1, 1, 24, 80);
        Call6(Func_02004528_a, 2, 1, 24, 11, 1, 2);
        Call1(Func_02004586, 0x323);
    } else {
        Call6(Func_02004564, 0, 0, 1, 1, 24, 80);
        Call6(Func_02004558, 0, 1, 24, 11, 1, 2);
        Call1(Func_020045ae, 0x323);
    }
}

void FieldScene_RunScene3a4SequenceA(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_020045ec, 0x325) != 0) {
        Call6(Func_020045dc, 12, 72, 1, 1, 11, 73);
        Call6(Func_020045d0, 48, 32, 11, 4, 1, 2);
        Call1(Func_0200462e, 0x325);
    } else {
        Call6(Func_0200460c, 10, 72, 1, 1, 11, 73);
        Call6(Func_02004600, 49, 32, 11, 4, 1, 2);
        Call1(Func_02004656, 0x325);
    }
}

void FieldScene_RunLine1528Sequence(void)
{
    Func_02004680();
    Func_02004660(0x1528, 1);
    Func_0200483e(125);
    Func_020015b6();
    Func_020045a0(20);
    Func_02004844();
    Func_020046a8();
}

void FieldScene_RunScene3a4SequenceC(void)
{
    u32 i;
    s32 record;

    Func_020046b2();
    Call2(Func_02004692, 0x1528, 1);
    Func_02004870(125);
    if (Value1(Func_020046a6, 0x326) != 0) {
        Call6(Func_02004696, 15, 93, 1, 1, 16, 92);
        Call6(Func_0200468a, 47, 29, 16, 28, 1, 2);
        Call1(Func_020046e8, 0x326);
    } else {
        Call6(Func_020046c6, 17, 93, 1, 1, 16, 92);
        Call6(Func_020046ba, 46, 29, 16, 28, 1, 2);
        Call1(Func_02004710, 0x326);
    }
    Func_02004636(20);
    Func_020048da();
    Func_0200473e();
}

void FieldScene_RunScene3a4SequenceD(void)
{
    u32 i;
    s32 record;

    Func_0200474e();
    Call2(Func_0200472e, 0x1528, 1);
    Func_0200490c(125);
    if (Value1(Func_02004742, 0x327) != 0) {
        Call6(Func_02004732, 28, 82, 1, 1, 29, 81);
        Call6(Func_02004726, 47, 28, 29, 17, 1, 2);
        Call1(Func_02004784, 0x327);
    } else {
        Call6(Func_02004762, 30, 82, 1, 1, 29, 81);
        Call6(Func_02004756, 46, 28, 29, 17, 1, 2);
        Call1(Func_020047ac, 0x327);
    }
    Func_020046d2(20);
    Func_02004976();
    Func_020047da();
}

void FieldScene_RunScene3a4_02000c9c(void)
{
    u32 i;
    s32 record;

    Func_02004812();
    Call6(Func_020047de, 24, 27, 2, 1, 24, 26);
    Func_020049dc(185);
    Call3_02000c9c(Func_02004866, 10, 0x3333, 0x1999);
    Call3_02000c9c(Func_02004870_a, 0, 0x3333, 0x1999);
    *(u8 *)(Func_0200486e(10) + 90) &= 254;
    Func_020048d8(0, 8);
    Call3_02000c9c(Func_020048b6, 0, 0x190, 0x1a8);
    Call3_02000c9c(Func_020048c4, 10, 0x198, 0x1a8);
    Func_020048ea(10);
    Func_02004902(0, 1);
    Func_02001122();
    Func_02004892();
}

void FieldScene_RunSharedSetPiece(s32 a0)
{
    u32 i;
    s32 rec;
    s32 rec2;
    u8 *rec4;
    s32 rec8;
    u8 *record;
    s32 none;
    s32 base5_200abe1;
    s32 base6_ffc00000;
    s32 slot8;
    s32 zero;

    slot8 = a0;
    rec2 = Value1(Func_02004a6e, 0);
    rec = Value1(Func_02004a76, 8);
    rec4 = Value1(Func_02004a7e, 9);
    rec8 = Func_02004a86(10);
    Call2(Func_02004b62, 0, 0x102);
    Func_02004a68(40);
    Call2(Func_02004b84, 0x10000, 0x2000);
    Call4(Func_02004b9e, 0x3100000, -1, 0x740000, 1);
    Call3(Func_02004acc, 0, 0x20000, 0x10000);
    Func_02004b24(0, 6);
    Call3(Func_02004b08, 0, 0x318, 140);
    Func_02004b38(0, 1);
    Call3(Func_02004ba4, 0, 0xc000, 100);
    Call3(Func_02004bbe, 0, 0x101, 60);
    Func_02004c84(183);
    Call3(Func_02004aac, 0x30000, 0x30000, 0x10000);
    Func_02004ae2(20);
    *(s32 *)((s32)rec4 + 24) = 0x13333;
    *(s32 *)((s32)rec4 + 28) = 0x13333;
    {
        u8 *p = &rec4[35];
        u8 two = 2;
        u8 value = *(volatile u8 *)p;

        none = 0;
        *p = (u8)(value | two);
    }
    *(s32 *)((s32)rec4 + 108) = 0x2008099;
    Func_02004b90(8, 4);
    *(volatile s32 *)(rec + 68) = 0x8000;
    *(volatile s32 *)(rec + 8) = 0x3120000;
    *(volatile s32 *)(rec + 12) = 0x200000;
    *(volatile s32 *)(rec + 16) = 0x5a0000;
    *(volatile s32 *)(rec + 24) = 0x20000;
    *(volatile s32 *)(rec + 28) = 0x20000;
    Func_02004b2e(10);
    Func_02004cec(183);
    Call3(Func_02004b12, 0x40000, 0x20000, 0x10000);
    Func_02004b48(20);
    *(s32 *)(rec8 + 8) += 0xe0000;
    *(s32 *)(rec8 + 12) += -0x80000;
    {
        s32 target = *(s32 *)(rec8 + 80);
        s32 shown = 0xc000;

        *(volatile u16 *)(target + 30) = shown;
    }
    Func_02004d20(107);
    Call3(Func_02004b48_a, 0x10000, 0x10000, 0x10000);
    Call3(Func_02004c7c, 0, 0x102, 80);
    Func_02004d42(55);
    Call3(Func_02004b6a, 0x10000, 0x30000, 0x10000);
    Func_02004c92(8, 0);
    Func_02004c9a(0, 0);
    Call2(Func_02004cb2, 0, 0x101);
    Call3(Func_02004bfa, 0, 0x28000, 0x14000);
    *(volatile u16 *)(rec2 + 100) = none;
    Call2(Func_02004c10, 0, 0x200bdec);
    if (Value1(Func_02004bbe_a, 0x205) != 0) {
        Call3(Func_02004c66, 1, 0x36e0000, 0x2100000);
        record = Func_02004c1c(1);
        {
            s32 shown = 0x5000;

            *(u16 *)((s32)record + 6) = shown;
        }
    }
    Call2(Func_02004d0e, 0x14000, 0x2800);
    Call4(Func_02004d26, 0x3120000, -1, 0x22c0000, 1);
    Func_02004c14(slot8);
    Func_02004d0c(8, 1);
    Call3(Func_02004c60, 8, 0x195c2, 0xcae1);
    *(volatile u16 *)(rec + 100) = none;
    Func_02004c76(8, 0x200bd78);
    do {
        Func_02004b4c(1);
    } while (*(s16 *)(rec2 + 100) == 0);
    ((void (*)())Func_02004d4c)(0, 0);
    do {
        Func_02004b62_a(1);
    } while (*(s16 *)(rec + 100) == 0);
    Func_02004d52(0, 2);
    {
        u8 *record = Func_02004c98(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Call1(Func_02004e30, 0x121);
    zero = 0;
    Call3(Func_02004c56, -1, -1, 0xe666);
    *(s32 *)((s32)rec4 + 8) = 0x3120000;
    base6_ffc00000 = -0x400000;
    *(s32 *)((s32)rec4 + 108) = zero;
    *(s32 *)((s32)rec4 + 16) = 0x26a0000;
    *(s32 *)((s32)rec4 + 12) = -0x400000;
    Call3(Func_02004cda, 8, 0x19999, 0xcccc);
    *(volatile s32 *)(rec + 68) = 0x1999;
    *(volatile s32 *)(rec + 72) = 0x3333;
    *(volatile s32 *)(rec + 40) = 0x40000;
    Call3(Func_02004d1e, 8, 0x312, 0x25c);
    Call3(Func_02004d00, 8, 0x33333, 0x19999);
    Call3(Func_02004d2c, 8, 0x312, 0x284);
    Func_02004cda_a(15);
    Call3(Func_02004cba, 0x50000, 0x70000, 0x10000);
    Call6(Func_02004c9e, 25, 36, 43, 36, 11, 9);
    Call6(Func_02004cd2, 25, 35, 10, 5, 43, 35);
    Func_02004d9c(8, 0, 0);
    Func_02004da6(9, 0, 0);
    base5_200abe1 = (s32)Func_0200abe1;
    Call2(Func_02004c4a, base5_200abe1, 0xc80);
    Func_02004d38(80);
    Func_02004c5e(base5_200abe1);
    Func_02004d44(60);
    Func_02004f02(17);
    Call3(Func_02004d28, -1, -1, 0xe666);
    Func_02004d5e(120);
    if (Value1(Func_02004d4c_a, 0x205) != 0) {
        Call3(Func_02004dae, 1, 0x10000, 0x8000);
        Call3(Func_02004dea, 1, 0x338, 0x22e);
    }
    Call3(Func_02004dc4, 0, 0x9999, 0x4ccc);
    Call3(Func_02004e08, 0, 0x356, 0x248);
    if (Value1(Func_02004d86, 0x205) != 0) {
        Func_02004e32(1, 1);
        Call3(Func_02004e9e, 1, 0x4000, 0);
    }
    Call3(Func_02004eaa, 0, 0x6000, 40);
    Call3(Func_02004ec6, 1, 0x102, 0);
    Call3(Func_02004ed2, 0, 0x102, 60);
    Call4(Func_02004f00, 0x3140000, base6_ffc00000, 0x2620000, 1);
    Func_02004f0c();
    Func_02004faa(148);
    Func_02004df8(240);
    if (Value1(Func_02004de6, 0x205) != 0) {
        Call2(Func_02004f1c, 0x40000, 0x8000);
        Call4(Func_02004f32, 0x3560000, 0, 0x2480000, 1);
        Func_02004f3e();
        Call3(Func_02004e9c, 1, 0x348, 0x228);
        Call3(Func_02004ea6, 1, 0x356, 0x232);
        Func_02004ec6_a(1, 2);
        record = Value1(Func_02004e74, 0);
        if ((s32)record != 0) {
            Func_02004eae(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Func_02004ed4(1);
        Func_02004ee6(1, 0, 0);
    }
    Func_0200500a();
    Call1(Func_02004e60, 0x908);
}

void FieldScene_RunScene3a4_02001398(void)
{
    u32 i;
    u8 *record;
    s32 base5_1953;

    Func_02004f0c_a();
    base5_1953 = (s32)Data_00001953;
    Value2(Func_02004eee, base5_1953, 1);
    if (Value1(Func_02004efc, 0x908) != 0) {
    } else {
        if (Value1(Func_02004f08, 0xf14) != 0) {
        } else {
            Call1(Func_02004f1c_a, 0x205);
            Call3(Func_02004f6e, 0, 0xcccc, 0x6666);
            Call3(Func_02004fb0, 0, 0x316, 140);
            Call3(Func_02004fbc, 0, 0x30c, 140);
            Call3(Func_02005040, 0, 0xc000, 0);
            record = Value1(Func_02004f8e, 0);
            if ((s32)record != 0) {
                Func_02004fec(1, *(volatile s32 *)((s32)record + 8), *(volatile s32 *)((s32)record + 16));
            }
            Call3(Func_02004fae, 1, 0xcccc, 0x6666);
            Call3(Func_02004ff2, 1, 0x320, 140);
            Call3(Func_02005076, 1, 0xc000, 20);
            Func_0200505c((base5_1953 + 1));
            Func_02005024(1, 4);
            Func_02004fa2(20);
            Func_0200508c(1, 0, 10);
            Func_0200504e(1, 6, 0);
            Call3(Func_02004ff8, 1, 0x19999, 0xcccc);
            *(u8 *)(Func_02004ff6(1) + 90) &= 254;
            Call3(Func_0200504e_a, 1, 0x318, 110);
            Func_02004fe4(1);
            *(u8 *)(Func_0200501a(1) + 90) |= 1;
            Func_020051b2(161);
            Call3(Func_02004fda, 0x20000, 0x10000, 0x10000);
            *(u8 *)(Func_02005040_a(1) + 90) &= 254;
            Call3(Func_02005094, 1, 0x318, 120);
            Func_0200502a(1);
            {
                u8 *record = Func_02005060(1);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Call3(Func_02005016, -1, -1, 0xe666);
            Func_0200504c(80);
            Func_0200520a(141);
            Call3(Func_02005032, 0x10000, 0x10000, 0x10000);
            Func_02005068(40);
            Call3(Func_0200516a, 0, 0x101, 0);
            Call3(Func_02005174, 1, 0x101, 60);
            Call3(Func_02005170, 0, 0x8000, 0);
            Func_0200517a(1, 0, 20);
            Func_02005184(0, 0, 0);
            Call3(Func_02005190, 1, 0x8000, 40);
            Call3(Func_0200519c, 0, 0x8000, 0);
            Func_020051a6(1, 0, 40);
            Call3(Func_020051b2_a, 0, 0xc000, 0);
            Call3(Func_020051be, 1, 0xc000, 40);
            Call3(Func_020051da, 1, 0x102, 60);
            Call3(Func_020051d6, 1, 0x4000, 20);
            Func_02005196(1, 2);
            Func_020051e0(1, 0, 10);
            Call3(Func_02005146, 1, 0x28000, 0x14000);
            Func_0200519e(1, 5);
            Call3(Func_02005182, 1, 0x31c, 138);
            Func_02005214(0, 0, 0);
            Call3(Func_02005198, 1, 0x324, 140);
            Call3(Func_0200522c, 0, 0x4000, 0);
            Call3(Func_020051b0, 1, 0x324, 166);
            Call3(Func_020051bc, 1, 0x2fc, 166);
            Call3(Func_020051c8, 1, 0x2fc, 198);
            Call3(Func_020051d2, 1, 0x312, 198);
            Call3(Func_02005276, 0, 0x102, 0);
            Call3(Func_020051e8, 1, 0x312, 246);
            Func_02005218(1, 1);
            Func_0200521a(1, 0, 0);
            Func_020051a0(40);
            Func_020024fe(10);
        }
    }
    Func_020051ba();
}

void FieldScene_RunScene3a4_02002310(void)
{
    extern u8 Data_02000240[];

    u32 i;
    s32 record;

    if (Value1_02002310(Func_02005e68, 0x8fe) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
        Func_02005f1a(9, 0, 0);
    } else {
        Func_02005d7c();
        if (Value1_02002310(Func_02005e8e, 0x109) == 0) {
            if (Data_02000240_t[225][0] != 99) {
                goto L_0200235a;
            }
            Func_02005172();
        } else {
            L_0200235a:;
            Call6(Func_02005e94, 38, 24, 1, 2, 37, 24);
            Call6(Func_02005ea8, 44, 23, 1, 2, 45, 23);
            if (Value1_02002310(Func_02005ed6, 0x8fe) == 0) {
                Func_02005faa(9, 2);
                Func_02005f8a(9, 3);
                Func_02004f5c(0xee0000, 0, 0x1a20000, 0x8000);
            }
        }
    }
    if (Value1_02002310(Func_02005f02, 0x323) != 0) {
        Call6(Func_02005ef2, 0, 0, 1, 1, 24, 80);
        Call6(Func_02005ee6, 0, 1, 24, 11, 1, 2);
    } else {
        Call6(Func_02005f1c, 2, 0, 1, 1, 24, 80);
        Call6(Func_02005f10, 2, 1, 24, 11, 1, 2);
    }
}

void FieldScene_RunScene3a4_02002428(void)
{
    extern u8 Data_02000240[];

    if (Value1(Func_02005f80, 0x8fe) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
    } else {
        Call6(Func_02005f7e, 52, 42, 1, 1, 53, 42);
    }
    {
        s32 index = 225;
        if ((u32)((((u16 *)Data_02000240)[index] - 6) << 16) <= 0x10000) {
            Call1(Func_02005fd2, 0x12f);
        }
    }
}

void FieldScene_RunScene3a4_02002490(void)
{
    extern u8 Data_02000240[];

    u32 i;
    u8 *record;

    if (Value1_02002490(Func_02005fe8, 0x907) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
        Func_0200609a(10, 0, 0);
    } else {
        if (Value1_02002490(Func_0200600a, 0x109) == 0) {
            if (Data_02000240_t[225][0] == 99) {
                Func_02005356();
            }
        }
        Func_02005f1a_a();
        if (Value1_02002490(Func_0200602c, 0x907) == 0) {
            Func_02006100(10, 2);
            Func_020060e0(10, 3);
            Call4(Func_020050b4, 0x2ec0000, 0x80000, 0x1180000, 0x8000);
        }
    }
    Func_0200596a(9);
    if (Value1_02002490(Func_02006062, 0x200) != 0) {
        Func_0200610e(9, 5);
        Call6(Func_0200605a, 23, 13, 1, 1, 25, 13);
        {
            u8 *record = Func_020060d0(9);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    if (Value1_02002490(Func_02006098, 0x325) != 0) {
        Call6(Func_02006088, 10, 72, 1, 1, 11, 73);
        Call6(Func_0200607c, 49, 32, 11, 4, 1, 2);
    } else {
        Call6(Func_020060b2, 12, 72, 1, 1, 11, 73);
        Call6(Func_020060a6, 48, 32, 11, 4, 1, 2);
    }
}

void FieldScene_RunScene3a4_020025c0(void)
{
    extern u8 Data_02000240[];

    u32 i;
    u8 *rec7;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        if (Value1_020025c0(Func_02006128, 0x109) == 0) {
            Call3(Func_020061d2, 8, 0x1660000, 0x680000);
        }
    }
    Func_02005a50(9);
    if (Value1_020025c0(Func_02006148, 0x200) != 0) {
        rec7 = Func_0200619a(9);
        Func_020061fc(9, 5);
        Call6(Func_0200614a, 45, 41, 1, 1, 43, 41);
        {
            u8 value = *(volatile u8 *)&rec7[35];

            rec7[35] = (u8)(value | 2);
        }
    }
    if (Value1_020025c0(Func_02006180, 0x907) != 0) {
        *(volatile u16 *)(*(volatile s32 *)0x03001e70 + 20) &= 0xfdff;
    }
    if (Value1_020025c0(Func_02006196, 0x326) != 0) {
        Call6(Func_02006186, 17, 93, 1, 1, 16, 92);
        Call6(Func_0200617a, 46, 29, 16, 28, 1, 2);
    } else {
        Call6(Func_020061b0, 15, 93, 1, 1, 16, 92);
        Call6(Func_020061a4, 47, 29, 16, 28, 1, 2);
    }
}

void FieldScene_RunScene3a4_020026c0(void)
{
    u32 i;
    u8 *record;

    record = Func_02006260(9);
    Func_020061fe((s32)record, 0);
    Func_02002ade();
    Func_02005b38(9);
    if (Value1_020026c0(Func_02006230, 0x200) != 0) {
        Func_020062dc(9, 5);
        Call6(Func_02006226, 0, 0, 1, 1, 26, 26);
        {
            u8 *record = Func_0200629c(9);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Func_02005b74(11);
    if (Value1_020026c0(Func_0200626a, 0x201) != 0) {
        Call2((void (*)())Func_02006316, 11, 5);
        Call6(Func_02006262, 1, 0, 1, 1, 17, 10);
        {
            u8 *record = Func_020062d8(11);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Func_02005bb0(12);
    if (Value1_020026c0(Func_020062a8, 0x204) != 0) {
        Func_02006354(12, 5);
        Call6(Func_020062a0, 1, 0, 1, 1, 26, 15);
        {
            u8 *record = Func_02006316_a(12);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
    }
    Call2(Func_02006212, 0x200b429, 0xc80);
    if (Value1_020026c0(Func_020062e8, 0x327) != 0) {
        Call6((void (*)())Func_020062d8_a, 30, 82, 1, 1, 29, 81);
        Call6(Func_020062cc, 46, 28, 29, 17, 1, 2);
    } else {
        Call6(Func_02006302, 28, 82, 1, 1, 29, 81);
        Call6(Func_020062f6, 47, 28, 29, 17, 1, 2);
    }
}

void FieldScene_RunScene3a4_02002934(void)
{
    extern s16 Data_02000240[];

    s32 rec7;
    s32 record;
    s16 flag;

    rec7 = Value1_02002934(Func_0200648a, 0x909);
    if (rec7 != 0) {
        Func_02006532(8, 0, 0);
        ((void (*)())Func_0200653c)(9, 0, 0);
    } else {
        record = Func_020064f4(8);
        Func_02006492(record, 0);
        Func_020065c2(9, 3);
        record = Func_02006508(9);
        Func_020064a6(record, 0);
        *(u8 *)(Func_02006514(9) + 89) = rec7;
    }
    flag = Data_02000240[225];
    if (flag == 1 || flag == 98) {
        if (Value1_02002934(Func_020064ea, 0x109) == 0) {
            rec7 = Value1_02002934(Func_0200653c_a, 0);
            Func_0200651a();
            *(volatile s32 *)(rec7 + 12) = 0x100000;
            Func_0200652c();
        }
    } else if (flag == 99) {
        if (Value1_02002934(Func_02006510, 0x109) == 0) {
            Func_020059f0();
        }
    }
    /* unlifted: 0x020029bc..0x020029c2 (2) */
}

void FieldScene_RunScene3a4_020029dc(void)
{
    extern s16 Data_02000240[];

    s32 record;

    record = Func_0200657a(9);
    Func_02006518(record, 0);
    if (Data_02000240[225] == 2) {
        Call3_020029dc(Func_020065ee, 9, 0xb80000, 0x1480000);
    }
}

void FieldScene_RunScene3a4SequenceG(void)
{
    u32 i;
    u8 *rec7;
    s32 record;
    s32 value;
    s32 base6_3001e40;

    base6_3001e40 = 0x3001e40;
    if (Value2(Func_0200665a, *(volatile s32 *)base6_3001e40, 3) == 0) {
        value = Value0(Func_02006682);
        rec7 = Value4(Func_020066ec, 200, ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x2fd0000), -0x400000, 0x2600000);
        if ((s32)rec7 != 0) {
            if (Value2(Func_0200668a, *(volatile s32 *)base6_3001e40, 9) == 0) {
                {
                    s32 v2 = Func_020066b2();
                    if (((u32)(v2 << 1) >> 16) != 0) {
                        Func_02006950(145);
                    } else {
                        Func_02006958(144);
                    }
                }
            }
            rec7[85] = 0;
            {
                s32 v3 = Func_020066d4();
                s32 tmp2824 = (((u32)(v3 << 15) >> 16) + 0x4ccc);
                *(volatile s32 *)((s32)rec7 + 72) = 0x6666;
                *(s32 *)((s32)rec7 + 28) = tmp2824;
                *(s32 *)((s32)rec7 + 24) = tmp2824;
            }
            rec7[97] = 1;
            Func_02006794((s32)rec7, 0);
            rec7[35] &= 254;
            {
                u8 *p80 = (u8 *)(*(volatile s32 *)((s32)rec7 + 80));
                s32 mask9 = -13;
                p80[9] = (mask9 & p80[9]) | 4;
            }
            Func_02006758((s32)rec7, 1);
            Call2(Func_02006768, (s32)rec7, 0x200c01c);
            value = Value0(Func_02006724);
            *(volatile s32 *)((s32)rec7 + 36) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
            *(volatile s32 *)((s32)rec7 + 40) = 0x80000;
            value = Value0(Func_0200673c);
            *(volatile s32 *)((s32)rec7 + 44) = (((u32)(((value << 1) + value) << 9) >> 16) + -0x300);
        }
    }
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_020068fa, 0);
    Func_020068d8();
    Func_0200695a(10, 0, 0);
    Func_02006964(8, 0, 0);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x201;
    Func_02006a3e();
    Func_02006a4a();
    Func_02006900(20);
    Func_02006a60(202, 3);
    Value2(Func_02006930, 202, 0);
    *(u8 *)(Func_02006a3c() + 85) = 0;
    Call2(Func_02006a32, 0x19999, 0x3333);
    Call4(Func_02006a4a_a, 0x640000, 0, 0xf90000, 1);
    Func_02006a56();
    Func_0200693c(20);
    Func_020034b4();
    Func_02006a64(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16), 1);
    Func_02006a70();
    Call1(Func_0200696e, 0x200cd6c);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x204;
    Func_02006974();
}

void FieldScene_RunScene3a4SequenceF(void)
{
    u32 i;
    s32 record;
    s32 v6;

    Func_02006990();
    Func_02006a12(9, 0, 0);
    Call3(Func_02006a20, 8, 0x1480000, 0x1a80000);
    Call1(Func_02006996, 0x323);
    Func_0200382e();
    Func_02006936();
    v6 = 224;
    Func_020068c4(1);
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x201;
    Func_02006b0c();
    Func_02006b18();
    Func_0200364c();
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x204;
    Func_020069e8();
}

void FieldScene_RunScene3a4SequenceE(void)
{
    u32 i;
    s32 record;
    s32 v6;

    Func_020069f8();
    Func_02006a7a(10, 0, 0);
    Call3(Func_02006a88, 8, 0x1e80000, 0x8a0000);
    Call1(Func_020069fe, 0x325);
    Func_0200393e();
    Func_0200699e();
    v6 = 224;
    Func_0200692c(1);
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x201;
    Func_02006b74();
    Func_02006b80();
    Func_020037a0();
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x204;
    Func_02006a50();
}

/* Runs a fixed chain of 19 calls with literal arguments, sets byte 0x22B of
 * the shared work buffer to 3, then issues two more calls. */
void FieldScene_RunLateAuxiliarySequence(void)
{
    extern u8 Data_02000240[];

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

#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000031
#define SecondaryRuntimeSelector Value_00000030
#define TertiaryRuntimeSelector Value_0000002f
#define PrimaryOverlayData Data_020089ec
#define SecondaryOverlayData Data_02008a64
#define TertiaryOverlayData Data_02008b24
#define DefaultOverlayData Data_020089bc
#define RunSceneCommand Func_02000e04
#define FinishSceneSetup Func_02000e64
#define SceneState_ApplyArgMode0AndReturnZero Func_02000030
#define SceneData_SelectByRuntimeSelector Func_02000040
#define SceneData_GetTable8bcc Func_02000098
#define SceneData_SelectSecondaryDataByRuntimeSelector Func_020000a0
#define SceneData_SelectDataByRuntimeSelector Func_020000f4
#define SceneState_ConfigureRegion1_0_21x14 Func_02000148
#define SceneState_ConfigureRegion0_0_21x14 Func_02000168
#define SceneState_ApplyTwoRects Func_02000188
#define FieldScene_RunTwoLayoutSteps Func_020001bc
#define FieldScene_RunActor9Flag882Scene Func_020001f0
#define FieldScene_RunScene398SequenceA Func_02000214
#define FieldScene_RunActorFifteenScene Func_02000280
#define FieldScene_RunActorSixteenScene Func_020002ac
#define FieldScene_RunActor17Steps28AndD2 Func_020002d8
#define FieldScene_RunScene398SequenceB Func_02000304
#define ActorPresentation_SetSceneCell31AndFlag305 Func_02000424
#define SceneState_SetGlobalByte17 Func_0200044c
#define SceneState_ClearRuntimeByte17 Func_0200045c
#define FieldScene_DispatchByScenarioId Func_0200046c
#define RunGuardedSceneSetup Func_020004b4
#define SceneState_SetRuntimeWord448To516 Func_020004e8
#define FieldScene_RunScene398SequenceC Func_02000538
#define SceneActor_FindSlotAtTile Func_020007c4
#define StagedActor_PushActorAhead Func_020007f8
#define Resource398_ImportBankNoOp Func_02000904

#include "select_overlay_data_by_runtime_selector.h"

extern s16 Data_02000240[];
extern unsigned char Value_00000031;
extern unsigned char Value_00000030;
extern unsigned char Value_0000002f;
extern s32 Data_02009064[]; /* Packed direction steps, 16 entries. */
extern u8 Data_02008c5c[];
extern u8 Data_02008cbc[];
extern u8 Data_02008c14[];
extern u8 Data_02008efc[];
extern u8 Data_02008f80[];
extern u8 Data_02008e9c[];

void Func_02000c7c_a();
void Func_02000d6a_a();
void Func_0200101a_a();
void Func_02000a8e(s32, s32, s32, s32, s32, s32);
void Func_02000aae(s32, s32, s32, s32, s32, s32);
void Func_02000ac6(s32, s32, s32, s32, s32, s32);
void Func_02000ae2(s32, s32, s32, s32, s32, s32);
void Func_02000afa(s32, s32, s32, s32, s32, s32);
void Func_02000b16(s32, s32, s32, s32, s32, s32);
void Func_02000b5c(void);
s32 Func_02000b7e(s32, s32, s32);
void Func_02000b54(s32);
void Func_02000b78(void);
void Func_02000bec(void);
s32 Func_02000c24(s32, s32);
void Func_02000bf2(s32);
void Func_02000c30(s32, s32);
void Func_02000c14(void);
void Func_02000c18(void);
s32 Func_02000c50(s32, s32);
void Func_02000c1e(s32);
void Func_02000c5c(s32, s32);
void Func_02000c44(void);
void Func_02000c4a(s32);
void Func_02000c98(s32);
void Func_02000c88(s32, s32);
void Func_02000d90(s32);
void Func_02000936(void);
void Func_020009d2(void);
s32 Func_02000e04();
void Func_02000e04_a(s32, s32, s32, s32, s32, s32);
void Func_02000e64();
void Func_02000e86(s32, s32);
void Func_02000e8e(s32, s32);
s32 Func_02000e54(s32);
void Func_02000e9a(s32, s32, s32);
u8 *Func_02000e9a_b(s32);
void Func_02000e68(u8 *, s32);
u8 *Func_02001180();
s32 *Func_02000ff6();
s32 Func_020011c0();
void Func_020011a4();
void Func_020011a4_a(s32);
void Func_0200124a();
void Func_020011ca();
void Func_020011dc();
void Func_020011ea();
void Func_020011f0();
void Func_02000be8(void);
void Func_02000b7a();
void Func_02000b82();
void Func_02000b90();
void Func_02000ba0();
void Func_02000ba4();
s32 Func_02000bb6();
u8 *Func_02000bc2();
void Func_02000bc8();
void Func_02000be4();
void Func_02000bfc();
s32 Func_02000c84();
s32 Func_02000c8c();
void Func_02000c8c_a(s32);
void Func_02000c9c();
s32 Func_02000ca4();
void Func_02000cae();
s32 Func_02000cb0();
s32 Func_02000cba();
void Func_02000cd2();
void Func_02000ce0();
void Func_02000ce2();
void Func_02000ce4();
s32 Func_02000d0e();
s32 Func_02000d0e_a(s32);
void Func_02000d1e();
void Func_02000d26();
void Func_02000d28();
void Func_02000d2e();
void Func_02000d34();
void Func_02000d36();
void Func_02000d38();
void Func_02000d72();
void Func_02000d7c();
void Func_02000d82();
void Func_02000e94();
void Func_02000ea0();
void Func_02000eac();
void Func_02000eb8();
void Func_02000ec4();
u8 *Func_02000ec6();
void Func_02000ed0();
u8 *Func_02000ed2();
void Func_02000edc();
u8 *Func_02000ede();
void Func_02000ee8();
u8 *Func_02000eea();
void Func_02000ef4();
u8 *Func_02000ef6();
u8 *Func_02000f02();
u8 *Func_02000f0e();
u8 *Func_02000f1a();
u8 *Func_02000f26();
void Func_02000f36();
void Func_02000f3c();
void Func_02000f42();
void Func_02000f44();
void Func_02000f48();
void Func_02000f4c();
void Func_02000f4e();
void Func_02000f54();
void Func_02000f54_a(s32);
void Func_02000f5a();
void Func_02000f5c();
void Func_02000f60();
void Func_02000f64();
void Func_02000f66();
void Func_02000f6c();
void Func_02000f6c_a(s32);
void Func_02000f72();
void Func_02000f74();
void Func_02000f78();
void Func_02000f7c();
void Func_02000f7e();
void Func_02000f84();
void Func_02000f84_a(s32);
void Func_02000f8a();
void Func_02000f8c();
void Func_02000f94();
void Func_02000f9c();
void Func_02000fa4();
void Func_02000fac();
void Func_02000fb4();
s32 Func_02000fd4();
void Func_02001014();
s32 Func_02001020();
void Func_0200102a();
u8 *Func_0200102c();
u8 *Func_02001036();
void Func_0200103a();
s32 Func_02001058();
void Func_02001060();
u8 *Func_0200106c();
void Func_02001070();
void Func_02001072();
void Func_02001076();
void Func_0200108a();
void Func_02001090();
void Func_02001092();
void Func_020010a2();
void Func_020010a4();
s32 Func_020010c0();
void Func_020010d4();
s32 Func_020010d8();
void Func_02001108();
void Func_02001120();

/*
 * Table getter at 0x02000098. The eight-byte owner includes its one pool word
 * at 0x0200009c, which holds 0x02008bcc; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/* 0x02000e9a serves two imports in sibling arms: the three-argument setter in
 * the first and the one-argument record accessor in the second. */

/*
 * Data_02009064 is indexed by heading >> 12, X step in the high halfword and Z
 * step in the low, promoted back to 16.16 by masking and shifting rather than
 * by multiply. The s16 at +0x0a and +0x12 are the integer parts of the 16.16
 * words at +0x08 and +0x10, so the tile lookup takes tile coordinates.
 */

/*
 * Push the actor one step ahead of the player. The 268-byte owner at
 * 0x020007f8 includes its alignment halfword and its three pool words.
 * `blocker` is zero wherever it is stored at +0x24 and +0x2c, but it is the
 * register the reference stores, so it stays spelled as itself.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */

/* Deliberate no-op callback. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call instead precomputes a costly constant
 * into a pseudo shared with later uses in the block. A value-returning call
 * sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];
    void Func_02000c7c();
    void Func_02000d6a();

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];
    void Func_0200101a();

    f(a0, a1, a2);
}

#undef PrimaryOverlayData
#define PrimaryOverlayData Data_02008c2c
#undef SecondaryOverlayData
#define SecondaryOverlayData Data_02008c5c
#undef TertiaryOverlayData
#define TertiaryOverlayData Data_02008cbc
#undef DefaultOverlayData
#define DefaultOverlayData Data_02008c14

extern u8 Data_02008c2c[];

#undef PrimaryOverlayData
#define PrimaryOverlayData Data_02008ea8
#undef SecondaryOverlayData
#define SecondaryOverlayData Data_02008efc
#undef TertiaryOverlayData
#define TertiaryOverlayData Data_02008f80
#undef DefaultOverlayData
#define DefaultOverlayData Data_02008e9c

extern u8 Data_02008ea8[];

s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    s32 Func_02000976(s32, s32);

    Func_02000976(no, 0);
    return 0;
}

s32 SceneData_SelectByRuntimeSelector(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)Data_020089ec;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)Data_02008a64;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)Data_02008b24;
    }
    return (s32)Data_020089bc;
}

s32 Func_02000094(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    return 0;
}

u8 *SceneData_GetTable8bcc(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    return (u8 *)0x02008bcc;
}

s32 SceneData_SelectSecondaryDataByRuntimeSelector(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)Data_02008c2c;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)Data_02008c5c;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)Data_02008cbc;
    }
    return (s32)Data_02008c14;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)Data_02008ea8;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)Data_02008efc;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)Data_02008f80;
    }
    return (s32)Data_02008e9c;
}

void SceneState_ConfigureRegion1_0_21x14(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    s32 w = 21;
    s32 h = 14;

    Func_02000a8e(1, 0, 1, 1, w, h);
}

void SceneState_ConfigureRegion0_0_21x14(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    s32 w = 21;
    s32 h = 14;

    Func_02000aae(0, 0, 1, 1, w, h);
}

void SceneState_ApplyTwoRects(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    {
        s32 a5 = 1;
        s32 a6 = 3;

        Func_02000ac6(111, 37, 97, 21, a5, a6);
    }
    {
        s32 a5 = 32;
        s32 a6 = 24;

        Func_02000ae2(46, 38, 3, 2, a5, a6);
    }
}

void FieldScene_RunTwoLayoutSteps(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    {
        s32 fifth = 1;
        s32 sixth = 3;

        Func_02000afa(95, 21, 97, 21, fifth, sixth);
    }
    {
        s32 fifth = 32;
        s32 sixth = 25;

        Func_02000b16(46, 38, 3, 1, fifth, sixth);
    }
}

void FieldScene_RunActor9Flag882Scene(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    Func_02000b5c();
    Func_02000b7e(9, 0, 0);
    Func_02000b54(0x882);
    Func_02000b78();
}

void FieldScene_RunScene398SequenceA(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    Func_02000b82();
    Func_02000ba4(8, 0, 0);
    Call1(Func_02000b7a, 0x883);
    Func_02000b90(40);
    Func_02000bc8(15, 2);
    *(u8 *)(Func_02000bb6(15) + 85) = 0;
    {
        u8 *record = Func_02000bc2(15);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 2);
    }
    Func_02000bfc(15, 2);
    Call6(Func_02000ba0, 0, 0, 1, 1, 18, 14);
    Func_02000be4();
}

void FieldScene_RunActorFifteenScene(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    void Func_02000c40(s32);

    Func_02000bec();
    Func_02000c24(0xF, 0);
    Func_02000bf2(0x28);
    Func_02000c40(0xD2);
    Func_02000c30(0xF, 6);
    Func_02000c14();
}

void FieldScene_RunActorSixteenScene(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    void Func_02000c40_a(void);
    void Func_02000c6c(s32);

    Func_02000c18();
    Func_02000c50(0x10, 0);
    Func_02000c1e(0x28);
    Func_02000c6c(0xD2);
    Func_02000c5c(0x10, 6);
    Func_02000c40_a();
}

void FieldScene_RunActor17Steps28AndD2(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    void Func_02000c6c_a(void);

    Func_02000c44();
    Func_02000c7c(0x11, 0);
    Func_02000c4a(0x28);
    Func_02000c98(0xD2);
    Func_02000c88(0x11, 6);
    Func_02000c6c_a();
}

void FieldScene_RunScene398SequenceB(void)
{
    extern u8 Data_03001ebc[];
    void Func_02000c7c();
    void Func_02000d6a();

    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 v5;

    rec7 = Value1(Func_02000c84, 11);
    rec8 = Func_02000c8c(12);
    if ((*(volatile s32 *)(rec7 + 8) >> 20) == 35) {
        if ((*(volatile s32 *)(rec7 + 16) >> 20) != 23) {
            goto L_02000330;
        }
        Call1(Func_02000c7c_a, 0x303);
    } else {
        L_02000330:;
        Call1(Func_02000c8c_a, 0x303);
    }
    if ((*(volatile s32 *)(rec8 + 8) >> 20) == 35) {
        if ((*(volatile s32 *)(rec8 + 16) >> 20) != 23) {
            goto L_02000350;
        }
        Call1(Func_02000c9c, 0x304);
    } else {
        L_02000350:;
        Call1(Func_02000cae, 0x304);
    }
    if (Value1(Func_02000ca4, 0x303) == 0) {
        record = Value1(Func_02000cb0, 0x304);
        if (record == 0) {
            goto L_020003c2;
        }
    }
    if (Value1(Func_02000cba, 0x302) == 0) {
        Func_02000ce2();
        Func_02000ce0(40);
        Func_02000d2e(210);
        v5 = 36;
        Func_02000d1e(17, 6);
        Call6(Func_02000cd2, 0, 1, 1, 1, v5, 22);
        Call6(Func_02000ce4, 0, 2, 1, 1, v5, 24);
        Func_02000d28();
    }
    Call1(Func_02000d0e, 0x302);
    goto L_02000414;
    L_020003c2:;
    if (Value1(Func_02000d0e_a, 0x302) != 0) {
        Func_02000d36();
        Func_02000d34(40);
        Func_02000d82(220);
        v5 = 36;
        Func_02000d72(17, 2);
        Call6(Func_02000d26, 1, 1, 1, 1, v5, 22);
        Call6(Func_02000d38, 1, 2, 1, 1, v5, 24);
        Func_02000d7c();
    }
    Call1(Func_02000d6a_a, 0x302);
    L_02000414:;
}

void ActorPresentation_SetSceneCell31AndFlag305(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    s32 width = 8;
    s32 height = 13;

    Func_02000d6a(31, 0, 1, 1, width, height);
    Func_02000d90(0x305);
}

void SceneState_SetGlobalByte17(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    FIELD_AT_OFFSET(*(void **)0x03001E70, s8 *, 0x17) = 1;
}

void SceneState_ClearRuntimeByte17(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    FIELD_AT_OFFSET(*(void **)0x03001E70, s8 *, 0x17) = 0;
}

/*
 * Map-variant selector, and the overlay's exported entry. The 72-byte owner at
 * 0x0200046c includes its alignment halfword and four pool words, and returns
 * 0. The selector is the signed halfword at byte offset 448 of Data_02000240;
 * the three compared constants are spelled as addresses of Value_ symbols,
 * which is what puts them in the literal pool.
 */
s32 FieldScene_DispatchByScenarioId(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    void Func_02000976_a(void);

    s16 variant = Data_02000240[224];

    if (variant == (s32)&Value_00000031) {
        Func_02000936();
    } else if (variant == (s32)&Value_00000030) {
        Func_02000976_a();
    } else if (variant == (s32)&Value_0000002f) {
        Func_020009d2();
    }
    return 0;
}

void RunGuardedSceneSetup(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    if (RunSceneCommand(0x305) != 0) {
        s32 width = 8;
        s32 height = 13;

        Func_02000e04_a(31, 0, 1, 1, width, height);
        FinishSceneSetup(8, 0);
    }
}

void SceneState_SetRuntimeWord448To516(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    /* 448 is built as 224 << 1 and the stored 516 as that same register plus
     * 68; the two are not one running offset. */
    *(s32 *)(Data_03001ebc + 448) = 516;

    Func_02000e86(8, 1);
    Func_02000e8e(10, 2);

    if (Func_02000e54(0x882) != 0) {
        Func_02000e9a(9, 0, 0);
    } else {
        Func_02000e68(Func_02000e9a_b(9), 0);
    }
}

void FieldScene_RunScene398SequenceC(void)
{
    extern u8 Data_03001ebc[];
    void Func_0200101a();

    u32 i;
    u8 *record;
    s32 v5;

    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x204;
    record = Func_02000ec6(18);
    Func_02000e94((s32)record, 0);
    record = Func_02000ed2(19);
    Func_02000ea0((s32)record, 0);
    record = Func_02000ede(20);
    Func_02000eac((s32)record, 0);
    record = Func_02000eea(21);
    Func_02000eb8((s32)record, 0);
    record = Func_02000ef6(22);
    Func_02000ec4((s32)record, 0);
    record = Func_02000f02(23);
    Func_02000ed0((s32)record, 0);
    record = Func_02000f0e(24);
    Func_02000edc((s32)record, 0);
    record = Func_02000f1a(25);
    Func_02000ee8((s32)record, 0);
    record = Func_02000f26(26);
    Func_02000ef4((s32)record, 0);
    Func_02000f44(18, 5);
    Func_02000f4c(19, 5);
    Func_02000f54(20, 5);
    Func_02000f5c(21, 5);
    Func_02000f64(22, 5);
    Func_02000f6c(23, 3);
    Func_02000f74(24, 3);
    Func_02000f7c(25, 3);
    Func_02000f84(26, 3);
    Func_02000f8c(9, 2);
    Func_02000f94(10, 2);
    Func_02000f9c(11, 2);
    Func_02000fa4(12, 2);
    Func_02000fac(13, 2);
    Func_02000fb4(14, 2);
    Func_02000f36(18);
    Func_02000f3c(19);
    Func_02000f42(20);
    Func_02000f48(21);
    Func_02000f4e(22);
    Func_02000f54_a(23);
    Func_02000f5a(24);
    Func_02000f60(25);
    Func_02000f66(26);
    Func_02000f6c_a(9);
    Func_02000f72(10);
    Func_02000f78(11);
    Func_02000f7e(12);
    Func_02000f84_a(13);
    Func_02000f8a(14);
    if (Value1(Func_02000fd4, 0x883) != 0) {
        Func_0200101a_a(8, 0, 0);
        Func_0200102a(15, 5);
        *(u8 *)(Func_02001020(15) + 85) = 0;
        record = Func_0200102c(15);
        *(volatile s32 *)((s32)record + 12) = -0x40000;
        {
            u8 *record = Func_02001036(15);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
        Func_02001070(15, 2);
        Call6(Func_02001014, 0, 0, 1, 1, 18, 14);
    } else {
        Func_02001076(8, 2);
        record = Func_0200106c(8);
        Func_0200103a((s32)record, 0);
        Func_0200108a(15, 1);
    }
    Func_02001092(16, 1);
    if (Value1(Func_02001058, 0x302) != 0) {
        v5 = 36;
        Func_020010a4(17, 1);
        Call6(Func_02001060, 0, 1, 1, 1, v5, 22);
        Call6(Func_02001072, 0, 2, 1, 1, v5, 24);
    } else {
        v5 = 36;
        Func_020010d4(17, 5);
        Call6(Func_02001090, 1, 1, 1, 1, v5, 22);
        Call6(Func_020010a2, 1, 2, 1, 1, v5, 24);
    }
    if (Value1(Func_020010c0, 0x303) != 0) {
        Call3(Func_02001108, 11, 0x23a0000, 0x1780000);
    }
    if (Value1(Func_020010d8, 0x304) != 0) {
        Call3(Func_02001120, 12, 0x23a0000, 0x1780000);
    }
}

s32 *SceneActor_FindSlotAtTile(s32 x, s32 z)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void StagedActor_PushActorAhead(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();

    u8 *player;
    u8 *target;
    u8 *blocker;
    s32 heading;
    s32 tx;
    s32 tz;
    s32 pos[3];

    player = Func_02001180(0);
    heading = *(u16 *)(player + 6) >> 12;

    tx = (*(s16 *)(player + 10)
        + (Data_02009064[heading] >> 16)) >> 4;
    tz = (*(s16 *)(player + 18)
        + ((Data_02009064[heading] << 16) >> 16)) >> 4;
    target = (u8 *)Func_02000ff6(tx, tz);
    if (target == 0) return;

    tx = (*(s16 *)(target + 10)
        + (Data_02009064[heading] >> 16)) >> 4;
    tz = (*(s16 *)(target + 18)
        + ((Data_02009064[heading] << 16) >> 16)) >> 4;
    blocker = (u8 *)Func_0200101a(tx, tz);
    if (blocker != 0) return;

    target[0x22] = 2;

    pos[0] = *(s32 *)(target + 8)
        + (Data_02009064[heading] & (s32)0xffff0000);
    pos[1] = *(s32 *)(target + 12);
    pos[2] = *(s32 *)(target + 16) + (Data_02009064[heading] << 16);

    if (Func_020011c0(target, pos) > 0) return;

    Func_020011a4(player, 8);
    Func_020011a4_a(15);
    Func_0200124a(185);

    *(s32 *)(target + 48) = 0x3333;
    *(s32 *)(target + 52) = 0x3333;
    Func_020011ca(target, pos[0], pos[1], pos[2]);

    *(s32 *)(player + 48) = 0x3333;
    *(s32 *)(player + 52) = 0x3333;
    Func_020011dc(player, pos[0], pos[1], pos[2]);

    Func_020011ea(target);

    *(s32 *)(target + 8) = pos[0];
    *(s32 *)(target + 16) = pos[2];
    *(s32 *)(target + 36) = (s32)blocker;
    *(s32 *)(target + 44) = (s32)blocker;

    Func_020011f0(player, 1);
    Func_02000be8();
}

void Resource398_ImportBankNoOp(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000c7c(s32, s32);
    s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
    s32 *Func_0200101a();
}

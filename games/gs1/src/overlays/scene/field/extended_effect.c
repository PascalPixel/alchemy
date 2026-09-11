#include "types.h"

#define SceneData_GetTableB6D4 Func_02000030
#define SceneData_GetTableB704 Func_0200003c
#define SceneData_GetTableB710 Func_02000044
#define SceneData_GetTableB998 Func_0200004c
#define Scene_RunTwoPhaseActorSequence Func_02000054
#define FieldScene_RunScene381_02000e30 Func_02000e30
#define Resource381_NoOpCallbackA Func_02000f28
#define Resource381_NoOpCallbackB Func_02000f2c
#define Scene_RunWhenWord225Is10 Func_02000f30
#define State_ConfigureEightCornerRegions Func_02002c9c
#define Scene_RunActor15TwoStep Func_0200300c
#define OvObj_SetRecordAngleFromHeading Func_02000f54
#define State_InitStateWordsAndSlots Func_02001084
#define State_UpdateRandomTimerLevel Func_0200135c
#define State_ApplyRectsByCondition Func_0200284c
#define State_ApplyRectPairByFlag Func_020028dc
#define Scene_RunRandomHalfBranch Func_0200293c
#define Scene_RunLateRandomHalfBranch Func_02002970
#define Scene_RunVariantStep Func_02002d48
#define Scene_RunStepByRuntimeBits Func_02002d94
#define State_ForwardByRuntimeWordBits Func_02002dd0
#define State_SetValue140Mode0 Func_02002ff0
#define Actor_MoveTo232_125AndFace4000 Func_02002820
#define Scene_RunFourWayEffectSequence Func_020029a4
#define Effect_AdvanceTenEntryTimers Func_02002ba0
#define Actor_PlaceAtTileAndRunSteps Func_02002c1c
#define Effect_UpdateArcOverAnchor Func_02002e0c
#define Effect_UpdateAnchoredRiseArc Func_02002e5c
#define Scene_CallHelper6620 Func_02003000

#include "facing_object.h"

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[72];
    u8 *unk50;
} Ent;

typedef struct {
    u8 filler0[6];
    u16 unk6;
} Ent_02002820;

typedef struct {
    u8 unk[9];
    u8 unk0 : 2;
    u8 mode : 2;
    u8 unk4 : 4;
} SpriteMode;

typedef struct {
    u8 filler0[12];
    s32 unkC;
    u8 filler10[8];
    s32 unk18;
    s32 unk1C;
    u8 filler20[28];
    s32 unk3C;
} Ent_02002ba0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[16];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
} Ent_02002c1c;

/* resource_381 owner at 0x02002e0c (80 bytes), the twin of resource_371:3fb4.
 * Ported from that byte-exact owner: same field layout, same two calls. */
struct Actor_02002e0c {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor_02002e0c *anchor;  /* 0x68 */
};

/* resource_381 owner at 0x02002e5c (82 bytes), the twin of resource_371:4004.
 * Ported from that byte-exact owner: same field layout, same two calls. */
struct Actor_02002e5c {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor_02002e5c *anchor;  /* 0x68 */
};

extern u8 *Data_03001ec4[];
extern s16 Data_02000240[];
extern s32 Data_0200bb68;
extern s32 Data_0200bb00;
extern s32 Data_0200bb6c;
extern s32 Data_0200bb70;
extern s32 Data_0200bac0[];
extern s32 Data_03001e40;
extern s32 Data_0200b684[][2];
extern s32 Data_0200bb40[];

void Func_0200b484(u8 *, s32);
void Func_0200b4cc();
void Func_0200b4dc();
u8 *Func_0200b4ec(s32);
void Func_0200b4fc();
void Func_0200b524();
void Func_0200b534();
void Func_0200b53c();
void Func_0200b544();
void Func_0200b554();
void Func_0200b55c();
void Func_0200b564();
void Func_0200b58c();
void Func_0200b594();
void Func_0200b59c();
void Func_0200b5fc();
void *Func_020010bc();
void Func_02001570();
void Func_02001ba8();
void Func_02001bb6();
void Func_02001bc4();
void Func_02001c0c();
void Func_02001c12();
void Func_02001c46();
void Func_0200222a();
void Func_02002f1e();
void Func_02002f28();
void Func_02002f4a();
void Func_02002f5e();
void Func_02002fba();
void Func_02002fda();
void Func_02003188();
void Func_020031d2();
void Func_020031f2();
void Func_020031fc();
void Func_0200321c();
void Func_02003226();
void Func_0200350e();
void Func_02003530();
void Func_0200357a();
void Func_020035bc();
void Func_020035ca();
void Func_020035ee();
void Func_020035fa();
void Func_02003606();
void Func_0200360a();
void Func_02003612();
void Func_0200361e();
void Func_02003620();
void Func_0200362a();
void Func_02003638();
void Func_0200363e();
void Func_0200364a();
void Func_02003654();
void Func_02003668();
void Func_02003684();
void *Func_0200368e();
void Func_020036a2();
void Func_020036d6();
void Func_02003706();
void Func_0200370e();
void Func_02003718();
void Func_0200371c();
void Func_0200371e();
void Func_0200372c();
void Func_0200372e();
void Func_02003734();
void Func_02003742();
void Func_0200374c();
void Func_02003754();
void Func_0200375a();
void Func_02003762();
void Func_02003764();
void Func_02003768();
void Func_02003770();
void Func_02003776();
void Func_0200377a();
void Func_0200377c();
void Func_0200378a();
u8 *Func_0200379c(s32);
void Func_020037a0();
void Func_020037b4();
void Func_020037be();
void Func_020037c6();
void Func_020037ce();
void Func_020037f8();
void Func_02003804();
void Func_02003858();
void Func_02003870();
void Func_0200387c();
void Func_02003888();
void Func_0200388a();
void Func_02003894();
s32 Func_020038a0();
void Func_020038a8();
void Func_020038b4();
s32 Func_020038e2(void (*)(), s32);
void Func_020038e6();
s32 Func_020038ec(void (*)(), s32);
void Func_020038f2();
void Func_02003910();
void Func_0200391c();
void Func_02003924();
void Func_02003930();
void Func_02003938(s32, s32);
void Func_0200393c();
void Func_02003942();
void Func_02003948();
void Func_02003976();
void Func_02003984();
void Func_0200398c();
void Func_02003998();
void Func_0200399a();
void Func_020039a6();
void Func_020039a8();
void Func_020039ba();
void Func_020039ca();
void Func_020039d2();
void Func_02003a28();
void Func_02003a3c();
void Func_02003a50();
void Func_02003a52();
void Func_02003a5c();
void Func_02003a66();
void Func_02003a68();
void Func_02003a72();
void Func_02003a74();
void Func_02003a7c();
void Func_02003a80();
void Func_02003a8c();
void Func_02003a94();
void Func_02003aa2();
void Func_02003aa4();
void Func_02003b00();
void Func_02003b0c();
void Func_02003b18();
void Func_02003b24();
void Func_02003b2c();
void Func_02003b30();
void Func_02003b34();
void Func_02003b3c();
void Func_02003b42();
void Func_02003b50();
void Func_02003b6a();
void Func_02003b70();
void Func_02003b82();
void Func_02003b84();
void Func_02003b8e();
void Func_02003b96();
void Func_02003bb2();
void Func_02003bc6();
void Func_02003bd2();
void Func_02003bd4();
void Func_02003bd6();
void Func_02003be4();
void Func_02003be8();
void Func_02003bf2();
void Func_02003bf4();
void Func_02003c0a();
void Func_02003c14();
void Func_02003c16();
void Func_02003c18();
void Func_02003c2a();
void Func_02003c34();
void Func_02003c3e();
void Func_02003c46();
void Func_02003c5a();
void Func_02003c72();
void Func_02003c78();
void Func_02003c84();
void Func_02003c90();
void Func_02003ca8();
void Func_02003cb0();
void Func_02003cb4();
void Func_02003cbe();
void Func_02003cc4();
void Func_02003cda();
void Func_02003ce0();
void Func_02003d08();
void Func_02003d12();
void Func_02003d1c();
void Func_02003d24();
void Func_02003d26();
void Func_02003d46();
void Func_02003d4a();
void Func_02003d4c();
void Func_02003d5e();
void Func_02003d74();
void Func_02003d82();
void Func_02003d86();
void Func_02003d94();
void Func_02003d96();
void Func_02003da6();
void Func_02003daa();
void Func_02003db8();
void Func_02003dbe();
void Func_02003dc0();
void Func_02003dce();
void Func_02003dd8();
void Func_02003e0c();
void Func_02003e32();
void Func_02003e38();
void Func_02003e3e();
void Func_02003e4a();
void Func_02003e5e();
void Func_02003e80();
void Func_02003e8e();
void Func_02003e92();
void Func_02003ea6();
void Func_02003eb2();
void Func_02003ebe();
void Func_02003ec2();
void Func_02003ec6();
void Func_02003ecc();
void Func_02003eda();
void Func_02003ee4();
void Func_02003eee();
void Func_02003ef2();
void Func_02003ef4();
void Func_02003efe();
void Func_02003f00();
void Func_02003f0a();
void Func_02003f14();
void Func_02003f1a();
void Func_02003f32();
void Func_02003f36();
void Func_02003f3a();
void Func_02003f4c();
u8 *Func_02003f4e(s32);
void Func_02003f80();
void Func_02003f90();
void Func_02003f9e();
void Func_02003fa0();
void Func_02003fa4();
void Func_02003fa6();
void Func_02003fb2();
void Func_02003fc8();
void Func_02003fd8();
void Func_02003fda();
void Func_02003fe0();
void Func_02003fe4();
void Func_0200400e();
void Func_02004018();
void Func_02004028();
void Func_02004048();
void Func_02004058();
void Func_0200405e();
void Func_02004070();
void Func_02004094();
void Func_02004096();
void Func_0200409c();
void Func_0200409e();
void Func_020040a6();
void Func_020040ae();
void Func_020040b0();
void Func_020040b2();
void Func_020040de();
void Func_020040e6();
void Func_020040f8();
void Func_02004102();
void Func_0200411a();
void Func_0200411e();
void Func_02004138();
void Func_0200413a();
void Func_02004146();
void Func_02004148();
void Func_02004152();
void Func_02004166();
void Func_02004168();
void Func_0200417c();
void Func_0200418a();
void Func_02004194();
void Func_020041ac();
void Func_020041b0();
void Func_02004200();
void Func_02004204();
void Func_02004206();
void Func_02004218();
void Func_02004230();
void Func_0200423c();
void Func_02004248();
void Func_0200424a();
void Func_02004254();
void Func_02004258();
void Func_02004260();
void Func_02004266();
void Func_0200428c();
void Func_020042a6();
void Func_020042b2();
void Func_020042b6();
void Func_020042dc();
void Func_0200433c();
void Func_02004344();
void Func_02004346();
void Func_0200435a();
void Func_02004366();
void Func_02004372();
void Func_0200438a();
void Func_0200438c();
void Func_020043a4();
void Func_0200430a();
s32 Func_0200432a();
void Func_02004332();
s32 Func_02004364();
s32 Func_0200436c();
s32 Func_02004378();
void Func_0200437c();
void Func_020043a6();
void Func_020043b8();
void Func_020043fc();
void Func_020043fc_a();
void Func_02004434();
void Func_0200443e();
void Func_020044c2();
void Func_020044e4();
void Func_02004528(void);
void Func_02000f9c(void);
void Func_02006132(s32, s32, s32, s32, s32, s32);
void Func_02006144(s32, s32, s32, s32, s32, s32);
void Func_02006156(s32, s32, s32, s32, s32, s32);
void Func_02006168(s32, s32, s32, s32, s32, s32);
void Func_0200617e(s32, s32, s32, s32, s32, s32);
void Func_02006190(s32, s32, s32, s32, s32, s32);
void Func_020061a4(s32, s32, s32, s32, s32, s32);
void Func_020061b4(s32, s32, s32, s32, s32, s32);
void Func_020064fe(s32);
void Func_02005de6(void);
s32 Func_02004778(void);
s32 Func_0200479e(void);
s32 Func_020047b6(void);
s32 Func_020047ce(void);
s32 Func_020047e6(void);
void Func_02005cd8(s32, s32, s32, s32, s32, s32);
void Func_02005cea(s32, s32, s32, s32, s32, s32);
void Func_02005cfa(s32, s32, s32, s32, s32, s32);
void Func_02005d0e(s32, s32, s32, s32, s32, s32);
void Func_02005d1e(s32, s32, s32, s32, s32, s32);
void Func_02005d30(s32, s32, s32, s32, s32, s32);
void Func_02005d40(s32, s32, s32, s32, s32, s32);
void Func_02005d34(void);
void Func_02005d68(s32, s32, s32, s32, s32, s32);
void Func_02005d7a(s32, s32, s32, s32, s32, s32);
void Func_02005d8e(s32, s32, s32, s32, s32, s32);
void Func_02005da0(s32, s32, s32, s32, s32, s32);
void Func_02005d94(void);
s32 Func_02005d48(void);
u32 Func_02005d2e(s32, s32);
void Func_020051a8(s32);
void Func_020051b0(s32);
s32 Func_02005d7c(void);
u32 Func_02005d62(s32, s32);
void Func_0200526c(s32);
void Func_02005274(s32);
void Func_0200637c(s32);
void Func_0200634c(s32, s32);
void Func_0200638c(s32);
void Func_0200635e(s32, s32);
void Func_0200636c(s32);
void Func_0200624e(s32);
void Func_0200624c(s32, s32);
void Func_02006256(s32, s32);
void Func_02005c74(s32);
s32 Func_020061c4(u32, s32);
void Func_02006294(s32, s32);
void Func_02005cb0(s32);
void Func_0200660c(s32, s32);
Ent *Func_02005d12(s32);
void Func_02005d6a(s32, s32, s32);
void Func_02005de8(s32, s32);
void Func_0200b4f4(s32);
u8 *Func_0200b454(s32, s32, s32, s32);
void Func_0200b444(u8 *, s32);
void Func_0200b3e4(s32);
s32 Func_0200b3ec(void (*)(), s32);
void Func_0200b3f4(void (*)());
void Func_02002ba0(void);
Ent *Func_020061fc(s32);
void Func_02006204(s32, s32, s32, s32);
void Func_02006234(s32, s32);
void Func_02006242(s32);
void Func_02006038(s32);
void Func_02006052(s32);
void Func_020060d6(void);
void Func_0200605c(s32);
void Func_0200626e(s32, s32);
void Func_0200627c(s32);
void Func_02006072(s32);
void Func_02006284();
s32 Func_02006234_a();
void Func_020062d4();
s32 Func_02006284_a();
void Func_02006620(void);

/*
 * Table getters published from the header words of overlay resource_381.
 * Only one overlay is resident at 0x02000000 at a time, so an address that
 * matches another overlay's names different bytes, not the same table.
 */

/* Set up the scene actors, run the two presentation phases, and dismiss the
 * actors through the shared exit sequence. Retained pending an exact build. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    void Func_0200b5ac();
    void Func_0200b5ec();
    void Func_0200b5f4();
    void Func_0200b624();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Func_0200b5ac();
    void Func_0200b5ec();
    void Func_0200b5f4();
    void Func_0200b624();

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Func_0200b5ac();
    void Func_0200b5ec();
    void Func_0200b5f4();
    void Func_0200b624();

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Func_0200b5ac();
    void Func_0200b5ec();
    void Func_0200b5f4();
    void Func_0200b624();

    f(a0, a1, a2, a3);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    void Func_0200b5ac();
    void Func_0200b5ec();
    void Func_0200b5f4();
    void Func_0200b624();

    return f(a0, a1);
}

static __inline__ s32 InstallTask(s32 (*f)(void (*)(), s32), void (*callback)(), s32 key)
{
    void Func_0200b5ac();
    void Func_0200b5ec();
    void Func_0200b5f4();
    void Func_0200b624();

    return f(callback, key);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call3_020029a4(void (*f)(s32, s32, s32), s32 a, s32 b, s32 c)
{
    extern u8 *Data_0200bb10[];
    void Func_0200b5ac(s32, s32, s32);
    void Func_0200b5ec(s32, s32);
    void Func_0200b5f4(s32);
    void Func_0200b624(s32);

    f(a, b, c);
}

/*
 * Returns the in-image table address 0x0200b6d4, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB6D4(void)
{
    return (u8 *)0x0200b6d4;
}

s32 Func_02000038(void)
{
    return 0;
}

/*
 * Returns the in-image table address 0x0200b704, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB704(void)
{
    return (u8 *)0x0200b704;
}

/*
 * Returns the in-image table address 0x0200b710, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB710(void)
{
    return (u8 *)0x0200b710;
}

/*
 * Returns the in-image table address 0x0200b998, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr. Published from the overlay header, not
 * reached by any call.
 */
u8 *SceneData_GetTableB998(void)
{
    return (u8 *)0x0200b998;
}

void Scene_RunTwoPhaseActorSequence(void)
{
    void Func_0200b5ac();
    void Func_0200b5ec();
    void Func_0200b5f4();
    void Func_0200b624();

    u32 i;
    u8 *record;
    s32 base5_3001ec4;
    s32 base5_3001ebc;
    u8 *p7;

    base5_3001ec4 = (s32)Data_03001ec4;
    p7 = *(u8 **)base5_3001ec4;
    Func_02003530();
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Func_0200368e(141);
    Call3(Func_0200350e, 0x10000, 0x10000, 0x10000);
    Call3(Func_020035bc, 0, 0xe80000, 0x9c0000);
    Call3(Func_020035ca, 1, 0xda0000, 0xac0000);
    Call3(Func_0200363e, 0, 0xb000, 0);
    Call3(Func_0200364a, 1, 0xd000, 0);
    Call3(Func_020035ee, 5, 0x1db0000, 0x14c0000);
    Call3(Func_020035fa, 9, 0x1eb0000, 0x14c0000);
    Call3(Func_02003606, 11, 0x1cb0000, 0x15c0000);
    Call3(Func_02003612, 10, 0x1fb0000, 0x15c0000);
    Call3(Func_0200361e, 13, 0x1d70000, 0x1320000);
    Call3(Func_0200362a, 14, 0x1df0000, 0x16a0000);
    Func_020036a2(15, 1);
    base5_3001ebc = base5_3001ec4 - 8;
    Call4(Func_020036d6, 0xe80000, -1, 0x9c0000, 0);
    Func_0200357a();
    *(s32 *)((*(s32 *)base5_3001ebc + 0x1c8)) = 8;
    Func_0200b5fc();
    Func_0200372e();
    Call2(Func_0200371e, 0x7fff, 0);
    Func_0200372c(4);
    Func_0200360a(4);
    Call2(Func_02003734, 0x10000, 0);
    Func_02003742(4);
    Func_02003620(4);
    Func_020010bc();
    Call2(Func_0200374c, 0x7fff, 0);
    Func_0200375a(4);
    Func_02003638(16);
    Func_0200b624(144);
    Call2(Func_02003768, 0x10000, 0);
    Func_02003776(4);
    Func_02003654(4);
    Call2(Func_0200377c, 0x7fff, 0);
    Func_0200378a(4);
    Func_02003668(4);
    Func_020037c6(144);
    Call2(Func_0200b5ec, 0x10000, 0);
    Func_0200b5f4(48);
    Func_02003684(48);
    Func_0200370e(0, 6, 0);
    Func_02003718(1, 6, 20);
    Func_02002f1e(1, 20, 20);
    Func_02002f28(0, 20, 40);
    Call1(Func_02003762, 0x10cd);
    Call3((void (*)())Func_0200b594, 11, 0, 20);
    Call2((void (*)())Func_0200b58c, 10, 0);
    Func_02002f4a(1, 20, 0);
    Func_020037a0(5, 0, 20);
    Func_02002f5e(0, 20, 0);
    Func_020037b4(14, 0, 20);
    Func_020037be(9, 0, 20);
    Func_0200b564(0, 1, 0);
    Func_02003706(40);
    Func_0200b554(0, 2);
    Func_0200b55c(1, 2);
    Func_0200371c(20);
    Call3(Func_020037f8, 0, 0xb000, 0);
    Call3(Func_02003804, 1, 0xd000, 30);
    Func_02002fba(1, 20, 0);
    Func_020037ce(0, 2);
    Func_0200b55c(1, 2);
    Func_02003754(20);
    Func_02002fda(0, 20, 20);
    record = Func_0200b4ec(15);
    Func_0200b484(record, 0);
    Call3(Func_0200b534, 15, 0x1450000, 0x12e0000);
    record = Func_0200379c(15);
    record[85] = 5;
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Call1(Func_020038e6, 0x121);
    Call3(Func_02003764, -1, -1, 0xe666);
    Func_02003770();
    Func_0200b4cc(150);
    Func_0200b594(11, 0, 20);
    Func_0200b58c(5, 0);
    Func_0200388a(10, 0, 10);
    Func_0200b564(0, 1, 20);
    Call3(Func_020038a8, 0, 0x3000, 0);
    Call3(Func_020038b4, 1, 0x3000, 10);
    Call3(Func_02003858, 5, 0x1db0000, 0x14c0000);
    Call3(Func_0200b534, 9, 0x1eb0000, 0x14c0000);
    Call3(Func_02003870, 11, 0x1cb0000, 0x15c0000);
    Call3(Func_0200387c, 10, 0x1fb0000, 0x15c0000);
    Call3(Func_02003888, 13, 0x1d70000, 0x1320000);
    Call3(Func_02003894, 14, 0x1df0000, 0x16a0000);
    Call2(Func_02003924, 0x66666, 0xcccc);
    Call4(Func_0200393c, 0x1480000, -1, 0x12b0000, 1);
    Func_02003948();
    Func_020039a6(167);
    Call2(Func_02003976, 0x205294, 2);
    Func_02003984(20);
    Func_0200377a(20);
    Call2(Func_0200398c, 0x10000, 2);
    Func_0200399a(20);
    Func_0200b4cc(200);
    Value2(Func_02003938, 0x1001, 0);
    if (Value2(Func_020038a0, 0, 0) == 0) {
        Call1(Func_02003942, 0x10d6);
    } else {
        Call1(Func_02003998, 0x10d7);
    }
    Call3(Func_020039ba, 0x1001, 0, 80);
    Call1(Func_020039a8, 0x10d8);
    Func_020039ca(9, 0, 20);
    Func_02003188(1, 20, 0);
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Func_02003a72(141);
    Call3(Func_020038f2, 0x10000, 0x20000, 0x10000);
    Func_02003930(80);
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Call1(Func_0200b624, 0x121);
    Call3(Func_02003910, -1, -1, 0xe666);
    Func_0200391c();
    Func_020031d2(0, 20, 60);
    Func_02003a28(14, 0, 30);
    Call3(Func_02003a3c, 15, 0xa000, 40);
    Func_020031f2(1, 20, 20);
    Func_020031fc(0, 20, 20);
    Call3(Func_02003a52, 0x1001, 0, 30);
    Call3(Func_02003a66, 15, 0x1000, 40);
    Func_0200321c(1, 20, 20);
    Func_02003226(0, 20, 20);
    Func_02003a7c(5, 0, 30);
    Func_02001570();
    InstallTask(Func_020038e2, (void (*)())0x020090c5, 0xc80);
    InstallTask(Func_020038ec, (void (*)())0x0200935d, 0xc80);
    Func_020039d2(240);
    Func_02003aa4(10, 0, 30);
    Call3(Func_02003a50, 5, 0x1db0000, 0x14c0000);
    Call3(Func_02003a5c, 9, 0x1eb0000, 0x14c0000);
    Call3(Func_02003a68, 11, 0x1cb0000, 0x15c0000);
    Call3(Func_02003a74, 10, 0x1fb0000, 0x15c0000);
    Call3(Func_02003a80, 13, 0x1d70000, 0x1320000);
    Call3(Func_02003a8c, 14, 0x1df0000, 0x16a0000);
    Call3(Func_02003b00, 5, 0x8000, 0);
    Call3(Func_02003b0c, 9, 0x8000, 0);
    Call3(Func_02003b18, 11, 0x8000, 0);
    Call3(Func_02003b24, 10, 0x8000, 0);
    Call3(Func_02003b30, 13, 0x8000, 0);
    Call3(Func_02003b3c, 14, 0x8000, 0);
    record = Func_0200b4ec(5);
    Call4(Func_02003b82, (*(s16 *)((s32)record + 10) << 16), -1, (*(s16 *)((s32)record + 18) << 16), 1);
    Func_02003b8e();
    Func_02003a94(40);
    Func_02003b2c(13, 2);
    Func_02003aa2(20);
    Func_02003b6a(13, 0);
    Func_02003b42(11, 2);
    Func_02003b84(11, 0, 10);
    Func_02003b34(14, 4);
    Func_02003b96(14, 0, 20);
    Func_02003b70(10, 11, 40);
    Func_02003b50(10, 4);
    Func_02003bb2(10, 0, 10);
    Call3(Func_02003bd6, 5, 0x100, 0);
    Call3(Func_02003bd2, 5, 0x3000, 40);
    Call3(Func_02003bd4, 0x4005, 0, 10);
    Call3(Func_02003be8, 10, 0xb000, 0);
    Call3(Func_02003bf4, 11, 0xd000, 40);
    Func_02003bc6(10, 11, 40);
    Call3(Func_02003c18, 10, 0x105, 40);
    Call3(Func_02003c14, 10, 0xb000, 10);
    Func_02003c16(10, 0, 10);
    Call3(Func_02003c2a, 10, 0x8000, 10);
    Func_02003bf2(11, 2);
    Func_02003c34(11, 0, 10);
    Func_02003be4(10, 3);
    Func_02003c46(10, 0, 40);
    Call3(Func_02003c5a, 14, 0xb000, 60);
    Func_02003c0a(14, 3);
    Func_0200b594(14, 0, 10);
    Call3(Func_02003c78, 13, 0x3000, 0);
    Call3(Func_02003c84, 5, 0x3000, 0);
    Call3(Func_02003c90, 9, 0x5000, 0);
    Call3(Func_0200b59c, 11, 0x3000, 0);
    Call3(Func_02003ca8, 10, 0x5000, 20);
    Call3(Func_02003cb4, 14, 0xd000, 20);
    Func_0200b544(14, 4);
    Func_02003cbe(14, 0, 10);
    Call3(Func_02003ce0, 13, 0x103, 0);
    Call3(Func_02003c3e, 13, 0x10000, 0x8000);
    Call3(Func_02003c72, 13, 0x1d7, 0x13a);
    Func_0200b554(13, 3);
    Func_0200b594(13, 0, 10);
    Call3(Func_02003d08, 14, 0xb000, 10);
    Func_02003cb0(14, 4);
    Func_02003d12(14, 0, 10);
    Func_02003cda(13, 3);
    Func_02003d24(13, 0, 10);
    Func_0200b55c(9, 2);
    Call3(Func_0200b594, 0x4009, 0, 10);
    Call3(Func_02003d4a, 10, 0xb000, 20);
    Func_02003d4c(10, 0, 30);
    Func_02003d5e(11, 0, 20);
    Func_02003d26(11, 2);
    Func_0200b4cc(30);
    Func_02003d1c(11, 3);
    Func_0200b4cc(30);
    Call3(Func_02003d86, 11, 0xd000, 30);
    Func_0200b544(11, 4);
    Func_02003cc4(10);
    Func_02003d96(11, 0, 10);
    Call3(Func_02003daa, 13, 0x5000, 20);
    Func_02003dc0(13, 2);
    Func_0200b4cc(20);
    Func_02003dd8(10, 11, 0);
    Func_02003d46(30);
    Func_02003dbe(10, 3);
    Func_02003dce(11, 3);
    Call3(Func_02003e32, 11, 0xd000, 0);
    Call3(Func_02003e3e, 10, 0xb000, 0);
    Func_02003d74(40);
    Func_02003e0c(13, 1);
    Func_02003d82(30);
    Call3(Func_02003e5e, 13, 0x8000, 0);
    Func_02003d94(40);
    Call3(Func_02003e80, 13, 0x102, 0);
    Func_02003da6(60);
    Call3(Func_0200b59c, 14, 0xb000, 0);
    Func_02003db8(30);
    Func_02003e38(14, 4);
    Call3(Func_02003e92, 0x200e, 0, 30);
    Call3(Func_02003ea6, 9, 0xb000, 0);
    Call3(Func_02003eb2, 5, 0xb000, 0);
    Call3(Func_02003ebe, 13, 0x3000, 20);
    Call3(Func_02003eda, 5, 0x102, 40);
    Call3(Func_02003ecc, 0x2005, 0, 40);
    Call3(Func_02003eee, 13, 0x105, 60);
    Func_02003e8e(13, 3);
    Call3(Func_02003f00, 10, 0x101, 0);
    Call3(Func_02003f0a, 11, 0x101, 60);
    Func_02003ec2(14, 2);
    Func_0200b594(14, 0, 60);
    Func_0200b544(13, 3);
    Func_02003e4a(40);
    Call3(Func_02003f36, 5, 0x102, 0);
    Call3(Func_0200b5ac, 9, 0x102, 40);
    Func_0200b564(10, 11, 20);
    Func_02003ee4(10, 3);
    Func_02003ef4(11, 3);
    Func_0200b4cc(20);
    Call3(Func_0200b4fc, 10, 0x9999, 0x4ccc);
    Call3(Func_02003ec6, 11, 0x9999, 0x4ccc);
    Call3(Func_02003ef2, 11, 0x1db, 0x15c);
    Call3(Func_02003efe, 10, 0x1eb, 0x15c);
    Func_02003f14(11);
    Func_02003f1a(10);
    Func_02003f32(11, 1);
    Func_02003f3a(10, 1);
    Call3(Func_02003fa6, 11, 0xd000, 0);
    Call3(Func_02003fb2, 10, 0xb000, 0);
    Call3(Func_0200b59c, 13, 0x3000, 10);
    Call3(Func_02003fd8, 11, 0x103, 0);
    Func_02003f90(11, 2);
    Call2(Func_02003fc8, 0x200b, 0);
    Call3(Func_0200b4fc, 11, 0x19999, 0xcccc);
    Call3(Func_02003f4c, 5, 0x13333, 0x9999);
    Call3(Func_02003f80, 11, 0x1db, 0x152);
    record = Func_02003f4e(11);
    ((struct FacingObject *)record)->facing_flags = (u8)(254 & ((struct FacingObject *)record)->facing_flags);
    Call3(Func_02003fa0, 11, 0x1db, 0x15c);
    Func_02003fe0(5, 1);
    Func_02003fda(5, 4, 0);
    Call3(Func_0200b524, 5, 0x1cb, 0x13c);
    Call3(Func_02003fa4, 5, 0x8000, 0x4000);
    Call3(Func_0200405e, 13, 0x103, 0);
    Func_0200400e(13, 3);
    Func_02004058(13, 0, 30);
    Func_02004028(11, 1);
    Func_02003f9e(20);
    ((struct FacingObject *)record)->facing_flags |= 1;
    Call3(Func_02003fe4, 11, 0x10000, 0x8000);
    Call3(Func_02004018, 11, 0x1db, 0x14c);
    Call3(Func_0200409c, 11, 0xb000, 20);
    Call2(Func_02004094, 0x200b, 0);
    Call3(Func_020040b0, 10, 0xb000, 0);
    Func_02004070(10, 2);
    Func_0200b594(10, 0, 20);
    Call3(Func_020040de, 11, 0x102, 20);
    Call3(Func_0200b59c, 5, 0x3000, 0);
    Call3(Func_020040e6, 11, 0x3000, 20);
    Func_02004096(10, 4);
    Func_0200409e(11, 3);
    Call3(Func_02004102, 11, 0xb000, 20);
    Func_020040b2(11, 3);
    Func_0200b544(13, 3);
    Func_02004048(20);
    Call3(Func_020040a6, 5, 0x1b0, 0x13c);
    Call3(Func_0200b524, 13, 0x1a6, 0x137);
    Func_0200b53c(5, 1);
    Call3(Func_02004146, 5, 0x3000, 0);
    Call3(Func_02004152, 13, 0x3000, 20);
    Func_0200411a(10, 2);
    Call3(Func_02004166, 10, 0xd000, 20);
    Call3(Func_02004168, 0x100a, 0, 40);
    Func_02004138(9, 2);
    Func_020040ae(20);
    Call3(Func_0200418a, 9, 0xb000, 30);
    Call3(Func_020040f8, 9, 0x8000, 0x4000);
    Call3(Func_0200b524, 9, 0x1eb, 0x128);
    Call3(Func_020041b0, 9, 0x5000, 40);
    Call3(Func_0200411e, 10, 0x10000, 0x8000);
    Call3(Func_0200b4fc, 11, 0x10000, 0x8000);
    Call3(Func_0200413a, 13, 0x10000, 0x8000);
    Call3(Func_02004148, 14, 0x10000, 0x8000);
    Call3(Func_0200417c, 10, 0x1d7, 0x134);
    Call3(Func_02004200, 10, 0x5000, 0);
    Call3(Func_02004194, 11, 0x1c7, 0x134);
    Call3(Func_02004218, 11, 0x3000, 0);
    Call3(Func_020041ac, 14, 0x1e7, 0x134);
    Call3(Func_02004230, 10, 0xc000, 0);
    Call3(Func_0200423c, 11, 0xd000, 0);
    Call3(Func_02004248, 14, 0xb000, 0);
    Call3(Func_02004254, 5, 0xd000, 0);
    Call3(Func_02004260, 13, 0xd000, 0);
    Call2(Func_0200428c, 0x8000, 0x1000);
    Call4(Func_020042a6, 0x1d80000, -1, 0x12c0000, 1);
    Func_020042b2();
    Func_02004206(40);
    Call1(Func_02004204, 0x246);
    Call3(Func_0200424a, 10, 0x8000, 0x4000);
    Call3(Func_02004258, 11, 0x8000, 0x4000);
    Call3(Func_02004266, 14, 0x8000, 0x4000);
    Func_020042b6(10, 3);
    Func_02001ba8(10);
    Func_020042dc(9, 2);
    Func_02001bb6(9);
    Func_0200b544(11, 3);
    Func_02001bc4(11);
    Call3(Func_0200433c, 5, 0x9000, 40);
    Func_0200b554(5, 2);
    Call3(Func_02004346, 0x2005, 0, 40);
    Call3(Func_0200435a, 13, 0x3000, 0);
    Call3(Func_02004366, 5, 0xb000, 30);
    Call3(Func_02004372, 13, 0xe000, 0);
    Func_02001c0c(5);
    Func_02001c12(13);
    Call3(Func_0200438a, 14, 0x7000, 40);
    Func_0200438c(14, 0, 30);
    Func_02004344(14, 4);
    Func_0200b4cc(20);
    Func_020043a4(14, 0, 30);
    Func_02001c46(14);
    Func_0200222a();
    Func_0200b4dc(5);
}

void FieldScene_RunScene381_02000e30(s32 a0)
{
    s32 p10;
    u8 *rec7;
    s32 recA;
    s32 rec2;
    s32 base6_0;

    p10 = a0;
    recA = Func_0200432a(8);
    *(s32 *)(recA + 24) = 0x10000;
    *(s32 *)(recA + 28) = 0x10000;
    Value3(Func_02004378, p10, 0x1d7, 0x122);
    Call3(Func_020043fc, p10, 0xc000, 0);
    Func_02004332(10);
    Call3(Func_020043a6, 8, 0x1d70000, 0x1220000);
    rec7 = Value1(Func_02004364, p10);
    rec2 = Func_0200436c(p10);
    Func_0200430a(rec2, 0);
    Call2(Func_020043fc_a, p10, 0x100);
    rec7[85] = 0;
    Func_020044c2(201);
    base6_0 = 0;
    do {
        *(volatile s32 *)((s32)rec7 + 12) += 0x8000;
        Func_0200437c(1);
        base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
    } while (base6_0 != 60);
    Func_020044e4(190);
    base6_0 = 0;
    do {
        *(volatile s32 *)((s32)rec7 + 12) += 0x1999;
        *(volatile s32 *)((s32)rec7 + 24) += -0x28f;
        *(volatile s32 *)((s32)rec7 + 28) += -0x28f;
        *(volatile s32 *)(recA + 24) += -0x28f;
        *(volatile s32 *)(recA + 28) += -0x28f;
        Func_020043b8(1);
        base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
    } while (base6_0 != 90);
    Func_02004434(p10, 0, 0);
    Func_0200443e(8, 0, 0);
}

void Resource381_NoOpCallbackA(void)
{
}

void Resource381_NoOpCallbackB(void)
{
}

s32 Scene_RunWhenWord225Is10(void)
{
    if (Data_02000240[225] == 10) {
        Func_02004528();
        Func_02000f9c();
    }
    return 0;
}

s32 OvObj_SetRecordAngleFromHeading(Ent *p)
{
    *(u16 *)(p->unk50 + 30) = p->unk6 + 0x4000;
    return 1;
}

void State_InitStateWordsAndSlots(void)
{
    s32 *p;
    u32 i;

    Data_0200bb68 = 63;
    Data_0200bb00 = 0;
    Data_0200bb6c = 0;
    Data_0200bb70 = 120;
    p = Data_0200bac0;
    for (i = 0; i < 16; i++) {
        *p++ = 0;
    }
}

void State_UpdateRandomTimerLevel(void)
{
    u32 v;

    if (Data_0200bb70 != 0) {
        Data_0200bb70--;
        return;
    }
    if (Data_0200bb6c != 0) {
        Data_0200bb6c--;
    } else {
        Data_0200bb6c = (u32)(Func_02004778() << 2) >> 16;
    }
    v = Data_0200bb6c;
    switch (v) {
    case 3:
        Data_0200bb68 = v;
        Data_0200bb70 = ((u32)(Func_0200479e() * 20) >> 16) + 40;
        break;
    case 2:
        Data_0200bb68 = 15;
        Data_0200bb70 = ((u32)(Func_020047b6() * 40) >> 16) + 80;
        break;
    case 1:
        Data_0200bb68 = 63;
        Data_0200bb70 = ((u32)(Func_020047ce() * 80) >> 16) + 160;
        break;
    default:
        Data_0200bb68 = 127;
        Data_0200bb70 = ((u32)(Func_020047e6() * 160) >> 16) + 320;
        break;
    }
}

void Actor_MoveTo232_125AndFace4000(s32 no)
{
    Ent_02002820 *rec;

    rec = Func_02005d12(no);
    Func_02005d6a(no, 0xe80000, 0x7d0000);
    rec->unk6 = 0x4000;
    Func_02005de8(no, 3);
}

void State_ApplyRectsByCondition(s32 a)
{
    if (a != 0) {
        s32 x;
        s32 y;
        x = 1;
        Func_02005cd8(8, 47, 64, 7, x, x);
        y = 2;
        Func_02005cea(7, 48, 63, 8, y, x);
        Func_02005cfa(7, 49, 63, 9, y, x);
    } else {
        s32 x;
        x = 1;
        Func_02005d0e(56, 0, 64, 7, x, x);
        Func_02005d1e(56, 0, 63, 8, x, x);
        Func_02005d30(56, 0, 63, 9, 2, x);
        Func_02005d40(58, 25, 64, 8, x, x);
    }
    Func_02005d34();
}

void State_ApplyRectPairByFlag(s32 a)
{
    if (a != 0) {
        s32 n;
        n = 2;
        Func_02005d68(9, 45, 65, 5, n, n);
        Func_02005d7a(11, 46, 67, 6, 1, n);
    } else {
        s32 n;
        n = 2;
        Func_02005d8e(89, 2, 65, 5, n, n);
        Func_02005da0(102, 32, 67, 6, 1, n);
    }
    Func_02005d94();
}

void Scene_RunRandomHalfBranch(void)
{
    if ((Data_03001e40 & 1) == 0) {
        if (Func_02005d2e(Func_02005d48(), 100) > 50) {
            Func_020051a8(1);
        } else {
            Func_020051b0(0);
        }
    }
}

void Scene_RunLateRandomHalfBranch(void)
{
    if ((Data_03001e40 & 1) == 0) {
        if (Func_02005d62(Func_02005d7c(), 100) > 50) {
            Func_0200526c(1);
        } else {
            Func_02005274(0);
        }
    }
}

void Scene_RunFourWayEffectSequence(u32 mode)
{
    extern u8 *Data_0200bb10[];
    void Func_0200b5ac(s32, s32, s32);
    void Func_0200b5ec(s32, s32);
    void Func_0200b5f4(s32);
    void Func_0200b624(s32);

    u32 i, zero;
    s32 x, y, z;
    s32 *pos;
    u8 *obj, *sprite;

    for (i = 0; i <= 15; i++)
        Func_0200b4f4(i + 16);
    switch (mode) {
    case 0: Func_0200b5ec(0x4039d2, 1); break;
    case 1: Func_0200b5ec(0x4049d2, 1); break;
    case 2: Func_0200b5ec(0x404a4e, 1); break;
    case 3: Func_0200b5ec(0x403a52, 1); break;
    }
    Func_0200b5f4(60);
    Func_0200b624(214);
    i = 0;
    zero = i;
    for (pos = &Data_0200b684[0][0]; i <= 9; i++, pos += 2) {
        x = pos[0];
        y = pos[1];
        z = 0;
        switch (mode) {
        case 0: x += 0xe80000; z = 0x900000; break;
        case 1: x += 0xe80000; z = 0x1d00000; break;
        case 2: x += 0x2c70000; z = 0x900000; break;
        case 3: x += 0x2c70000; z = 0x1d00000; break;
        }
        Data_0200bb40[i] = zero;
        obj = Func_0200b454(284, x, y, z);
        Data_0200bb10[i] = obj;
        obj[85] = zero;
        sprite = *(u8 **)(obj + 80);
        sprite[38] = zero;
        ((SpriteMode *)sprite)->mode = 1;
        Func_0200b444(obj, 6);
        Func_0200b3e4(6);
    }
    if (mode == 0) {
        Call3_020029a4(Func_0200b5ac, 0, 256, 0);
        Call3_020029a4(Func_0200b5ac, 1, 256, 0);
    }
    Func_0200b3e4(20);
    Func_0200b3ec(Func_02002ba0, 3200);
    Func_0200b624(246);
    Data_0200bb40[0] = 1; Func_0200b3e4(6);
    Data_0200bb40[1] = 1; Func_0200b3e4(6);
    Data_0200bb40[2] = 1; Func_0200b3e4(6);
    Data_0200bb40[3] = 1; Func_0200b3e4(6);
    Data_0200bb40[4] = 1; Func_0200b3e4(6);
    Data_0200bb40[5] = 1; Func_0200b3e4(6);
    Data_0200bb40[6] = 1; Func_0200b3e4(6);
    Data_0200bb40[7] = 1; Func_0200b3e4(6);
    Data_0200bb40[8] = 1; Func_0200b3e4(6);
    Data_0200bb40[9] = 1; Func_0200b3e4(6);
    for (;;) {
        for (i = 0; i <= 9; i++) {
            if (Data_0200bb40[i] != 0) {
                i = 888;
                break;
            }
        }
        if (i != 888)
            break;
        Func_0200b3e4(1);
    }
    Func_0200b3e4(40);
    Func_0200b3f4(Func_02002ba0);
    Func_0200b5ec(65536, 1);
    Func_0200b5f4(40);
}

void Effect_AdvanceTenEntryTimers(void)
{
    extern Ent *Data_0200bb10[];

    u32 i;
    s32 v;
    Ent_02002ba0 *p;

    for (i = 0; i <= 9; i++) {
        v = Data_0200bb40[i];
        if (v != 0) {
            p = Data_0200bb10[i];
            if ((u32)v <= 8) {
                p->unk18 += -0x1ccc;
                p->unk1C += 0x8000;
                p->unkC += 0x4ccc;
                p->unk3C += 0x4ccc;
            } else {
                p->unkC += 0x140000;
                p->unk3C += 0x140000;
            }
            v = Data_0200bb40[i] + 1;
            Data_0200bb40[i] = v;
            if ((u32)v > 14) {
                Data_0200bb40[i] = 0;
            }
        }
    }
}

void Actor_PlaceAtTileAndRunSteps(s32 a, s32 b)
{
    Ent_02002c1c *p;

    p = Func_020061fc(a);
    b = b << 16;
    a = a << 16;
    Func_02006204(a, -1, b, 1);
    Func_02006234(0, 0);
    Func_02006242(20);
    Func_02006038(40);
    p->unk10 = b;
    p->unk8 = a;
    p->unk38 = 0x80000000;
    p->unk40 = 0x80000000;
    p->unk24 = 0;
    p->unk2C = 0;
    Func_02006052(5);
    Func_020060d6();
    Func_0200605c(5);
    Func_0200626e(0x10000, 0);
    Func_0200627c(20);
    Func_02006072(30);
}

void State_ConfigureEightCornerRegions(void)
{
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 v;

    x = 0;
    Func_02006132(14, 8, 1, 1, 10, x);
    Func_02006144(14, 28, 1, 1, 11, x);
    Func_02006156(44, 8, 1, 1, 12, x);
    Func_02006168(44, 28, 1, 1, 13, x);
    z = 14;
    y = 8;
    Func_0200617e(13, 8, 1, 1, z, y);
    w = 28;
    Func_02006190(13, 28, 1, 1, z, w);
    v = 44;
    Func_020061a4(43, 8, 1, 1, v, y);
    Func_020061b4(43, 28, 1, 1, v, w);
}

void Scene_RunVariantStep(s32 a, s32 b, s32 c)
{
    if (a == 1) {
        Func_0200637c(0x134);
        Func_0200634c(0x203a52, 1);
    } else {
        Func_0200638c(0x121);
        Func_0200635e(0x10000, 1);
    }
    Func_0200636c(b);
    if (c != 0) {
        Func_0200624e(c);
    }
}

void Scene_RunStepByRuntimeBits(s32 a)
{
    if ((Data_03001e40 & 2) != 0) {
        Func_0200624c(a, 7);
    } else {
        Func_02006256(a, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02005c74(a);
    }
}

void State_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Func_02006294(a, Func_020061c4(*p >> 1, 6));
    }
    if ((*p & 15) == 0) {
        Func_02005cb0(a);
    }
}

void Effect_UpdateArcOverAnchor(struct Actor_02002e0c *self)
{
    struct Actor_02002e0c *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Func_02006284(self);
        return;
    }

    amplitude = Func_02006234_a(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}

void Effect_UpdateAnchoredRiseArc(struct Actor_02002e5c *obj)
{
    struct Actor_02002e5c *anchor;
    s32 frame;
    s32 amp;

    anchor = obj->anchor;
    obj->frame = (u16)(obj->frame + 1);
    frame = (s16)obj->frame;

    if (frame > 31) {
        Func_020062d4(obj);
        return;
    }

    amp = Func_02006284_a(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}

void State_SetValue140Mode0(void)
{
    Func_0200660c(140, 0);
}

void Scene_CallHelper6620(void)
{
    Func_02006620();
}

void Scene_RunActor15TwoStep(void)
{
    Func_020064fe(15);
    Func_02005de6();
}

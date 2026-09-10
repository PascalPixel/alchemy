#include "types.h"

#define Object_SetModeById_1(args...) Func_02003dda(args)
#define Object_SetModeById_2(args...) Func_02003daa(args)
#define Object_SetModeById_3(args...) Func_02003dda(args)
#define Object_SetModeById_4(args...) Func_02003e0a(args)
#define AUX_COUNTDOWN (*(volatile s32 *)Data_0200c41c)
#define FieldScene_RunOpeningAuxiliarySequence Func_0200004c
#define FieldScene_RunScene3ba_02000270 Func_02000270
#define FieldScene_RunScene3ba_02000974 Func_02000974
#define FieldScene_RunCommandSequence Func_02000db8
#define FieldScene_RunScene3ba_020015e0 Func_020015e0
#define FieldScene_RunNearestActor165Scene Func_02001a28

typedef struct Sub {
    u8 pad00[0x28];
    s16 *f28;
} Sub;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[0x3c];
    Sub *f50;
    u8 f54;
} Obj;

extern u8 Data_0200c41c[];
extern s16 Data_02000240_t[][1];

void Func_02003c7e();
void Func_02003c90();
void Func_02003c9a();
void Func_02003cc0();
void Func_02003cd2();
void Func_02003cf0();
void Func_02003d02();
void Func_02003d14();
void Func_02003daa();
void Func_02003dda();
void Func_02003e0a();
void Func_02003e58();
void Func_02003eb0();
void Func_02003ed2();
void Func_02003ede();
void Func_02003ee0();
void Func_02003f12();
void Func_02003f2a();
s32 Func_02003f3c();
s32 Func_02003f96();
void Func_02003f9e();
void Func_02003fc6();
void Func_02004006();
void Func_02004020();
void Func_02004034();
void Func_02003246();
void Func_020044f4();
void Func_02004618();
void Func_0200462c();
void Func_0200463c();
void Func_02004642();
void Func_02004692();
void Func_020046a0();
void Func_020046ae();
void Func_020046cc();
void Func_020046d4();
void Func_020046d4_a();
void Func_020046de();
void Func_020046e0();
void Func_020046e6();
void Func_02004774();
void Func_02004a76();
s32 Func_02004a7e();
void Func_02004aac();
void Func_02004aba();
void Func_02004ac8();
void Func_02004ad6();
void Func_02004ae4();
s32 Func_02004b28();
s32 Func_02004b28_a();
void Func_02004b3a();
void Func_02004b48();
void Func_02004b54();
void Func_02004b62();
void Func_02004b70();
void Func_02004bbc();
void Func_02004bbe();
void Func_02004bd2();
void Func_02004bd4();
void Func_02004be0();
void Func_02004be6();
void Func_02004be6_a();
void Func_02004be8();
void Func_02004bee();
void Func_02004bf2();
void Func_02004bf6();
void Func_02004bfe();
void Func_02004c08();
void Func_02004c18();
void Func_02004c20();
void Func_02004c28();
void Func_02004c32();
void Func_02004c34();
void Func_02004c3a();
void Func_02004c42();
void Func_02004c46();
void Func_02004c4c();
void Func_02004c50();
void Func_02004c52();
void Func_02004c5c();
void Func_02004c64();
void Func_02004c66();
s32 Func_02004c68();
void Func_02004c6e();
void Func_02004c78();
void Func_02004cae();
void Func_02004cc8();
void Func_02004cca();
void Func_02004cf2();
void Func_02004d04();
void Func_02004d18();
void Func_02004d1e();
void Func_02004d24();
void Func_02004d2a();
void Func_02004d2e();
void Func_02004d32();
void Func_02004d48();
void Func_02004d5a();
void Func_02004d5a_a();
void Func_02004d5c();
void Func_02004d60();
void Func_02004d7a();
void Func_02004d7a_a();
void Func_02004d82();
void Func_02004d86();
void Func_02004d8a();
void Func_02004d90();
void Func_02004d92();
void Func_02004d94();
void Func_02004da2();
void Func_02004da4();
void Func_02004dac();
void Func_02004dae();
void Func_02004db0();
void Func_02004db0_a();
s32 Func_02004db2();
void Func_02004dbc();
void Func_02004dbe();
void Func_02004dc2();
void Func_02004dc6();
void Func_02004ddc();
void Func_02004dde();
s32 Func_02004dec();
void Func_02004dee();
void Func_02004dee_a();
void Func_02004df6();
void Func_02004df8();
void Func_02004e02();
void Func_02004e04();
s32 Func_02004e0c();
void Func_02004e0e();
void Func_02004e0e_a();
void Func_02004e12();
void Func_02004e1e();
void Func_02004e1e_a();
void Func_02004e28();
s32 Func_02004e2c();
void Func_02004e2e();
void Func_02004e2e_a();
void Func_02004e3e();
void Func_02004e4e();
void Func_02004e5e();
void Func_02004e6e();
void Func_02004e7e();
void Func_02004e82();
void Func_02004e98();
void Func_02004ea2();
void Func_02004ea8();
void Func_02004eac();
void Func_02004eb2();
void Func_02004f26();
void Func_02001748();
void Func_0200179a();
void Func_020017e2();
void Func_02003152();
s32 Func_02003368();
void Func_020034d2();
s32 Func_0200354c();
s32 Func_02003f6c();
void Func_0200418e();
void Func_0200529c();
void Func_020052e0();
void Func_02005322();
void Func_02005340();
void Func_02005340_a();
void Func_0200536c();
void Func_0200537c();
void Func_0200537e();
void Func_02005390();
void Func_02005390_a();
void Func_020053aa();
s32 Func_020053b6();
s32 Func_020053b6_a();
void Func_020053dc();
void Func_020053ee();
void Func_020053f4();
void Func_02005406();
void Func_02005412();
Obj *Func_02005708(s32);
void Func_020056f6(void);
Obj *Func_02005716(s32);
void Func_020057f0(s32);
void Func_02005808(s32, s32);
void Func_02005780();
void Func_02005780_a();
void Func_020058a4(void);
void Func_020058b0(void);
void Func_02005770(s32, s32);
void Func_020058a2(s32);
void Func_02005782(s32);
void Func_0200343a(s32);
void Func_020058de(void);
void Func_020058f2(void);
void Func_020057e4(void);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* A countdown word this overlay owns at Data_0200c41c: each call decrements
 * it by one, and specific values select which sub-sequence runs this call.
 * Reaching 0 restarts the countdown at 120 after running its own branch. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

static __inline__ void Call6_02000270(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call3_02000db8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call3_020015e0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

/* Runs one branch of a scripted auxiliary sequence selected by the current
 * countdown value, then advances (or, from 0, restarts) the countdown. */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    extern u8 Data_03001ebc[];

    switch ((u32)AUX_COUNTDOWN) {
    case 66:
        Call6(Func_02003cc0, 92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003cd2, 92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_1(16, 10); /* object 16, action 10 */
        break;
    case 60:
        Call6(Func_02003c7e, 92, 33, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003c90, 92, 33, 2, 2, 54, 38); /* main:080091c8 */
        Call6(Func_02003c9a, 50, 25, 6, 1, 50, 12); /* main:080091c0 */
        Object_SetModeById_2(16, 11); /* object 16, action 11 */
        break;
    case 6:
        Call6(Func_02003cc0, 92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003cd2, 92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_3(16, 10); /* object 16, action 10 */
        break;
    case 0:
        Call6(Func_02003cf0, 92, 29, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003d02, 92, 29, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_4(16, 12); /* object 16, action 12 */
        Call6(Func_02003d14, 50, 24, 6, 1, 50, 12); /* main:080091c0 */
        AUX_COUNTDOWN = 120;
        break;
    }
    AUX_COUNTDOWN = AUX_COUNTDOWN - 1;
}

void FieldScene_RunScene3ba_02000270(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *rec7;
    s32 record;
    s32 none;

    Call1(Func_02003ede, 0x301);
    rec7 = Value1(Func_02003f3c, 13);
    Func_02003f2a();
    Call2(Func_02004006, 0x20000, 0x4000);
    Call4(Func_02004020, 0x2580000, -1, 0xc80000, 1);
    Func_02003e58((s32)rec7, 3);
    Func_02004034();
    none = 0;
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    Call4(Func_02003eb0, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x80000, *(s32 *)((s32)rec7 + 16));
    rec7 = Value1(Func_02003f96, 14);
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    Call4(Func_02003ed2, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x200000, *(s32 *)((s32)rec7 + 16));
    Func_02003ee0((s32)rec7);
    Func_02003f9e(45);
    Call6_02000270(Func_02003f12, 43, 12, 1, 1, 41, 12);
    Func_02003fc6();
}

void FieldScene_RunScene3ba_02000974(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_0200463c(24);
    Func_02004642(25);
    Func_02004618(1);
    Func_0200462c();
    Call3(Func_02004692, 8, 0x5280000, 0xc00000);
    Call3(Func_020046a0, 0, 0x5080000, 0xc00000);
    Call3(Func_020046d4, 8, 0x4000, 0);
    Call3(Func_020046e0, 0, 0x4000, 0);
    if (a0 < 0) {
        Func_020046cc(8, 10);
        Func_020046d4_a(0, 35);
    } else {
        Func_020046de(8, 8);
        Func_020046e6(0, 28);
    }
    Func_020044f4(1);
    Call4(Func_02004774, 0x5180000, 0, 0x800000, 0);
    Func_02003246(a0);
    Func_020046ae();
}

void FieldScene_RunCommandSequence(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 p10;
    s32 p10b;
    s32 p9;
    s32 p9b;
    s32 record;

    record = Func_02004a7e();
    p9 = *(s16 *)(record + 10);
    p10 = *(s16 *)(record + 18);
    Func_02004a76();
    Call3_02000db8(Func_02004aac, a0, 0x10000, 0x8000);
    Call3_02000db8(Func_02004aba, 0, 0x10000, 0x8000);
    Call3_02000db8(Func_02004ac8, 1, 0x10000, 0x8000);
    Call3_02000db8(Func_02004ad6, 2, 0x10000, 0x8000);
    Call3_02000db8(Func_02004ae4, 3, 0x10000, 0x8000);
    Call3_02000db8((void (*)())Func_02004b28, 0, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x300000));
    Func_02004b3a(1, ((s32)((s32)p9 << 16) + -0x100000), (-0x280000 + (s32)((s32)p10 << 16)));
    Func_02004b48(2, ((s32)((s32)p9 << 16) + 0x100000), (-0x280000 + (s32)((s32)p10 << 16)));
    Func_02004b54(3, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x200000));
    Func_02004b62(a0, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x500000));
    record = Func_02004b28_a(0);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Func_02004be6(0, 0);
    Func_02004c32();
    Func_02004c46();
    Call1(Func_02004bbc, 0x20cb);
    Func_02004bd4(a0, 0);
    Call3_02000db8(Func_02004bfe, 3, 0x101, 60);
    Func_02004be6_a(3, 0);
    Func_02004bbe(a0, 3);
    Func_02004bf6(a0, 0);
    Call3_02000db8(Func_02004c20, 2, 0x101, 60);
    Func_02004c08(2, 0);
    Func_02004bf2(a0, 2, 0);
    Func_02004b70(20);
    Func_02004be8(a0, 3);
    Func_02004c28(a0, 0);
    Call3_02000db8(Func_02004c52, 1, 0x101, 60);
    Func_02004c3a(1, 0);
    Call3_02000db8(Func_02004c64, 3, 0x101, 60);
    Func_02004c4c(3, 0);
    Call3_02000db8(Func_02004c78, a0, 0x102, 60);
    if (Value2(Func_02004c68, a0, 0) != 0) {
    } else {
        L_02000f28:;
        Call1(Func_02004c5c, 0x20d5);
        Func_02004c34(2, 3);
        Func_02004bd2(2);
        Func_02004c42(1, 3);
        Func_02004be0(2);
        Func_02004c50(3, 3);
        Func_02004bee(1);
        Func_02004c66(0, 3);
        Func_02004c6e(a0, 3);
        Func_02004cae(a0, 0);
        Call3_02000db8(Func_02004cca, a0, 0xa000, 0);
        Func_02004c18(20);
        Func_02004cc8(a0, 0);
        Call2(Func_02004d04, 0x30000, 0x6000);
        Call4(Func_02004d1e, 0x1380000, -1, 0x680000, 1);
        Func_02004d2a();
        Func_02004cf2(a0, 0);
        Call2(Func_02004d2e, 0x18000, 0x3000);
        Call4(Func_02004d48, 0x3080000, -1, 0x680000, 1);
        Func_02004d18(a0, 0);
        Func_02004d5c();
        Func_02004d24(a0, 0);
        Call2(Func_02004d60, 0x30000, 0x6000);
        Call4(Func_02004d7a, 0x4d80000, -1, 0xa80000, 1);
        Func_02004d86();
        Call3_02000db8(Func_02004d32, a0, 0x6000, 0);
        Func_02004d5a(a0, 0);
        Call4(Func_02004da4, 0x5180000, -1, 0xa80000, 1);
        Func_02004db0();
        Func_02004d5a_a(a0, 0, 0);
        Func_02004d82(a0, 0);
        Func_02004d8a(a0, 0);
        Func_02004d92(a0, 0);
        Func_02004dc2(0, 0);
        Func_02004d7a_a(a0, 2);
        if (Value2(Func_02004db2, a0, 0) != 0) {
            goto L_02000f28;
        }
        Func_02004d90(a0, 2);
        Call1(Func_02004dae, 0x20d4);
        Func_02004dc6(a0, 0);
    }
    Call1(Func_02004dbc, 0x20e1);
    Func_02004dac(a0, 2);
    Func_02004ddc(a0, 0);
    Call3_02000db8(Func_02004df8, 0, 0x4000, 0);
    Func_02004e02(1, 0, 0);
    Call3_02000db8(Func_02004e0e, 2, 0x8000, 0);
    Func_02004dde(1, 2);
    Func_02004e0e_a(1, 0);
    Func_02004dee(2, 2);
    Func_02004e1e(2, 0);
    Func_02004dee_a(3, 3);
    Func_02004e2e(3, 0);
    Func_02004df6(3, 3);
    Func_02004d94(1);
    Func_02004e04(1, 3);
    Func_02004da2(2);
    Func_02004e12(2, 3);
    Func_02004db0_a(1);
    Func_02004e28(0, 3);
    Func_02004dbe(6);
    Func_02004e2e_a(1, 2);
    record = Value1(Func_02004dec, 0);
    if (record != 0) {
        Func_02004e1e_a(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004e4e(2, 2);
    record = Value1(Func_02004e0c, 0);
    if (record != 0) {
        Func_02004e3e(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004e6e(3, 2);
    record = Value1(Func_02004e2c, 0);
    if (record != 0) {
        Func_02004e5e(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004e7e(a0, (p9 - 16), (p10 - 64));
    Func_02004e98(1, 0, 0);
    Func_02004ea2(2, 0, 0);
    Func_02004eac(3, 0, 0);
    Func_02004ea8(a0, (p9 - 16), (p10 - 16));
    Func_02004eb2(a0, p9, p10);
    Call3_02000db8(Func_02004f26, a0, 0xc000, 10);
    Func_02004e82();
    p9b = (p9 - 16);
    p10b = ((s32)p9 << 16);
}

void FieldScene_RunScene3ba_020015e0(s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec8;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_02003152();
    } else {
        Func_0200529c();
        rec8 = Value2(Func_02003368, a0, 3);
        if (rec8 == 0) {
            Call1(Func_02005340, 0x2095);
            Func_02001748();
            Call2(Func_02005390, 0x30000, 0x6000);
            Call4(Func_020053aa, 0x3480000, -1, 0xd80000, 1);
            ((void (*)())Func_020053b6)();
            Func_0200537e(a0, 0);
            Func_0200179a();
            Func_020052e0(60);
            Func_02005390_a(a0, 0);
            Value3(Func_02003f6c, 0, 0x2e0, 200);
            Value3(Func_020053b6_a, 0, 0, 0);
            Func_020017e2();
            Call3_020015e0(Func_02005340_a, 0, 0x10000, 0x8000);
            Call3_020015e0(Func_0200536c, 0, 0x330, 200);
            Func_02005322(30);
            Call3_020015e0(Func_020053f4, 0, 0x105, 60);
            Func_020053dc(a0, 0);
            Func_0200418e(0);
            Func_02005412(0, 0);
            Func_020034d2(a0, 3);
        } else {
            if (rec8 == 1) {
                Call1(Func_020053ee, 0x2094);
                Func_02005406(a0, 0);
            }
        }
        Value3(Func_0200354c, rec8, a0, 3);
        Func_0200537c();
    }
}

void FieldScene_RunNearestActor165Scene(void)
{
    extern s32 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = Data_02000240[125];
    Obj *p = Func_02005708(n);
    s32 i;
    s32 *q;
    s32 base;

    Func_020056f6();
    for (i = 8; i <= 66; i++) {
        Obj *o = Func_02005716(i);

        if (o != 0 && o->f54 == 1 && *o->f50->f28 == 165) {
            s32 dx = (p->f08 - o->f08) / 65536;
            s32 dy = (p->f10 - o->f10) / 65536;

            if (dy <= 0) {
                s32 a = dx;
                s32 d;

                if (a < 0) a = -a;
                if (dy < 0) dy = -dy;
                d = a + dy;
                if (d < bestd) {
                    best = i;
                    bestd = d;
                }
            }
        }
    }
    Func_020057f0(0x2085);
    Func_02005808(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Func_02005780(20);
    Func_020058a4();
    Func_020058b0();
    base = n << 4;
    Func_02005770(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        Func_02005780_a(base + 888, v);
    }
    n++;
    if (n > 3) {
        Func_020058a2(10);
        Func_02005782(282);
    } else {
        Func_0200343a(n);
        Func_020058de();
        Func_020058f2();
        *q = 0;
    }
    Func_020057e4();
}

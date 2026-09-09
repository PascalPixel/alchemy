#include "types.h"

#define FieldScene_RunScene37aSequenceA Func_02000054
#define FieldScene_RunScene37aSequenceB Func_02000108
#define FieldScene_RunScene37aSequenceC Func_02000150
#define FieldScene_RunScene37aSequenceD Func_020001ec
#define FieldScene_RunScene37a_020009f4 Func_020009f4
#define FieldScene_RunScene37aSequenceE Func_02001a58
#define FieldScene_RunScene37a_02002924 Func_02002924
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000ffc[];
extern u8 Data_00004010[];

void Func_02000a8e();
void Func_02001c98();
s32 Func_02002608();
s32 Func_0200264e();
s32 Func_02002a16();
void Func_02002a3a();
void Func_02002a3e();
s32 Func_02002a46();
void Func_02002a4a();
void Func_02002a50();
s32 Func_02002a5c();
s32 Func_02002a72();
void Func_02002a90();
void Func_02002a9a_a();
void Func_02002a9a_b();
void Func_02002aa8();
void Func_02002aae();
void Func_02002aca();
void Func_02002b2e();
void Func_02002b44();
void Func_02002b8c();
void Func_02002ba2();
s32 Func_02002ac4();
void Func_02002aec_a();
void Func_02002aec_b();
void Func_02002afa();
void Func_02002b00();
void Func_02002b1c();
void Func_02002bde();
void Func_02002bf4();
void Func_02000bc4();
s32 Func_02002704();
s32 Func_02002b14();
void Func_02002b3c_a();
void Func_02002b3c_b();
void Func_02002b4a();
s32 Func_02002b4c();
void Func_02002b50();
void Func_02002b6c();
void Func_02002b70();
void Func_02002b74();
s32 Func_02002b7c();
void Func_02002b80();
void Func_02002b86();
void Func_02002bb0();
void Func_02002c2e();
void Func_02002c44();
void Func_02002c64();
void Func_02002c7a();
s32 Func_02002ba6();
void Func_02002bc6();
void Func_02002bd6();
void Func_02002bdc();
void Func_02002cba();
void Func_02002cd0();
void Func_0200306c();
void Func_02003096();
void Func_020030c8();
void Func_0200311e();
void Func_02003186();
void Func_020033fc();
s32 Func_0200340e();
void Func_02003420();
void Func_0200343c();
void Func_02003440();
void Func_0200344c();
void Func_0200344e();
void Func_0200345e();
void Func_02003460();
void Func_02003482();
void Func_0200349c();
void Func_020034ae();
void Func_020034d2();
s32 Func_020034de();
void Func_0200350c();
void Func_0200351a();
void Func_02003524();
void Func_02003530();
void Func_02003536();
void Func_02003538();
void Func_02003548();
void Func_0200354a();
s32 Func_0200354e_a();
s32 Func_0200354e_b();
void Func_02003554();
void Func_02003558();
void Func_0200355e();
void Func_02003560();
void Func_02003572();
void Func_0200357e_a();
void Func_0200357e_b();
void Func_02003584();
void Func_0200358e();
void Func_020035a8();
void Func_020035aa();
void Func_020035b6();
void Func_020035ba();
s32 Func_020035be();
void Func_020035c6();
void Func_020035c8();
void Func_020035d6();
void Func_0200361e();
void Func_0200362a();
void Func_02003656();
s32 Func_0200401a();
void Func_020040ee();
void Func_0200410c();
void Func_02004132();
void Func_02004162();
s32 Func_02004414();
void Func_02004448();
void Func_0200448c();
void Func_0200449c();
void Func_020044ac();
void Func_020044ba();
void Func_020044c2();
void Func_020044cc();
void Func_020044da();
void Func_020044e6();
void Func_020044ec();
void Func_020044f6();
void Func_020044fa();
void Func_02004500();
void Func_0200451c();
void Func_0200451e();
void Func_02004526();
void Func_0200452a();
void Func_02004534();
void Func_02004538();
void Func_02004544();
void Func_02004552();
void Func_02004556();
void Func_0200455e();
void Func_02004564();
void Func_0200456a();
void Func_0200457e();
void Func_02004588_a();
void Func_02004588_b();
s32 Func_0200458e();
void Func_02004594();
void Func_0200459e();
void Func_020045ac_a();
void Func_020045ac_b();
void Func_020045b0();
void Func_020045c0();
void Func_020045ce();
void Func_020045e0();
s32 Func_020052e2();
void Func_020052fc();
void Func_02005336();
void Func_0200538c();
void Func_02005394();
void Func_020053b6();
void Func_020053ca();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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
static __inline__ void Call1_02000108(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020001ec(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020009f4(void (*f)(), s32 a0)
{
    f(a0);
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02002924(void (*f)(), s32 a0)
{
    f(a0);
}

void FieldScene_RunScene37aSequenceA(void)
{
    u32 i;
    s32 record;

    if (Value0(Func_02002608)!= 0) {
        record = Value1(Func_02002a16, 0x201);
        if (record != 0) {
            goto L_020000f0;
        }
        Func_02002a3e();
        Call2(Func_02002b2e, 0x2051cc, 1);
        Func_02002b44(20);
        Call1(Func_02002a3a, 0x201);
        Call1(Func_02002a4a, 0x200);
        Call1(Func_02002a50, 0x202);
        if (Value1(Func_02002a46, 0x80a) == 0) {
            Func_02000a8e();
        }
        if (Value0(Func_0200264e)!= 0) {
            if (Value1(Func_02002a5c, 0x811) == 0) {
                Func_02001c98();
            }
        }
        Func_02002a90();
    } else {
        if (Value1(Func_02002a72, 0x200) == 0) {
            Func_02002a9a_a();
            Call2(Func_02002b8c, 0x10000, 1);
            Func_02002ba2(20);
            Call1(Func_02002a9a_b, 0x200);
            Call1(Func_02002aa8, 0x201);
            Call1(Func_02002aae, 0x202);
            Func_02002aca();
        }
    }
    L_020000f0:;
}

void FieldScene_RunScene37aSequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02002ac4, 0x200) == 0) {
        Func_02002aec_a();
        Call2(Func_02002bde, 0x10000, 1);
        Func_02002bf4(20);
        Call1_02000108(Func_02002aec_b, 0x200);
        Call1_02000108(Func_02002afa, 0x201);
        Call1_02000108(Func_02002b00, 0x202);
        Func_02002b1c();
    }
}

void FieldScene_RunScene37aSequenceC(void)
{
    u32 i;
    s32 record;

    if (Value0(Func_02002704)!= 0) {
        record = Value1(Func_02002b14, 0x200);
        if (record != 0) {
            goto L_020001d6;
        }
        Func_02002b3c_a();
        Call2(Func_02002c2e, 0x10000, 1);
        Func_02002c44(20);
        Call1(Func_02002b3c_b, 0x200);
        Call1(Func_02002b4a, 0x201);
        Call1(Func_02002b50, 0x202);
        Func_02002b6c();
    } else {
        if (Value1(Func_02002b4c, 0x201) == 0) {
            Func_02002b74();
            Call2(Func_02002c64, 0x2051cc, 1);
            Func_02002c7a(20);
            Call1(Func_02002b70, 0x201);
            Call1(Func_02002b80, 0x200);
            Call1(Func_02002b86, 0x202);
            if (Value1(Func_02002b7c, 0x80a) == 0) {
                Func_02000bc4();
            }
            Func_02002bb0();
        }
    }
    L_020001d6:;
}

void FieldScene_RunScene37aSequenceD(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02002ba6, 0x202) == 0) {
        Call2(Func_02002cba, 0x202db1, 1);
        Func_02002cd0(20);
        Call1_020001ec(Func_02002bc6, 0x202);
        Call1_020001ec(Func_02002bd6, 0x200);
        Call1_020001ec(Func_02002bdc, 0x201);
    }
}

void FieldScene_RunScene37a_020009f4(void)
{
    u32 i;
    s32 record;
    s32 base6_ffc;
    s32 base5_4010;
    s32 base5_4010_2;

    Call1_020009f4(Func_0200344e, 0xff6);
    Call3(Func_020033fc, 0, 0x10000, 0x8000);
    Call3(Func_02003420, 0, 0x1e8, 176);
    Func_02003440(0, 0);
    record = Value1(Func_0200340e, 0);
    if (record != 0) {
        Func_0200344c(16, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Func_020034ae(0, 0, 1);
    Call3(Func_0200343c, 16, 0x10000, 0x8000);
    Call3(Func_02003460, 16, 0x1d8, 168);
    Func_020034d2(16, 0, 60);
    Func_0200349c(16, 4, 40);
    Func_0200306c(16, 6);
    Call2(Func_0200350c, 0x26666, 0x4ccc);
    Call4(Func_02003524, 0x23f0000, -1, 0xb50000, 1);
    Func_02003530();
    Func_0200345e(120);
    Call2(Func_02003096, 0x1010, 80);
    Call4(Func_02003548, 0x1ec0000, -1, 0xa80000, 1);
    Func_02003554();
    Func_02003482(20);
    base5_4010 = (s32)Data_00004010;
    Call3(Func_02003538, 16, 0x3000, 20);
    Func_020030c8(base5_4010, 6);
    Func_0200354a(16, 0, 60);
    Func_0200351a(16, 2);
    Call3(Func_0200355e, 16, 0x3000, 10);
    Value2(Func_0200354e_a, base5_4010, 0);
    if (Value2(Func_020034de, 0, 0) == 0) {
        Call1_020009f4(Func_02003558, 0xffa);
    } else {
        Call1_020009f4(Func_02003560, 0xffb);
    }
    base5_4010_2 = (s32)Data_00004010;
    Call3(Func_0200358e, 0, 0xa000, 10);
    Func_0200311e(base5_4010_2, 10);
    base6_ffc = (s32)Data_00000ffc;
    Func_0200357e_a(base6_ffc);
    Func_020035a8(16, 0, 40);
    Call3(Func_020035ba, 16, 0x105, 40);
    Func_02003572(16, 4);
    Call3(Func_020035c6, 16, 0x3000, 10);
    Func_0200357e_b(16, 4);
    Value2(Func_020035be, base5_4010_2, 0);
    if (Value2(Func_0200354e_b, 0, 0) == 0) {
        Func_020035c8((base6_ffc + 1));
        Call1_020009f4(Func_02003536, 0x896);
    } else {
        Func_020035d6((base6_ffc + 2));
    }
    Call2(Func_02003186, (s32)Data_00004010, 4);
    Func_0200361e(16, 1);
    Call3(Func_020035aa, 16, 0x1e6, 131);
    Call3(Func_020035b6, 16, 0x240, 120);
    Call3(Func_0200362a, 16, 0xc000, 2);
    Call2(Func_02003656, 0x40000, 0x8000);
    Call1_020009f4(Func_02003584, 0x80a);
}

void FieldScene_RunScene37aSequenceE(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02004414, 0x810) != 0) {
    } else {
        if (Value0(Func_0200401a) == 0) {
        } else {
            Func_02004448();
            Call3(Func_0200449c, 16, 0x2410000, 0x930000);
            Call3(Func_02004500, 16, 0x4000, 1);
            Call4(Func_02004538, 0x23e0000, -1, 0xb80000, 1);
            Call1(Func_020044f6, 0x1027);
            Call3(Func_020044ba, 0, 0x240, 232);
            Func_020044da(0, 0);
            Func_0200455e();
            Func_0200448c(10);
            Call3(Func_020044c2, 16, 0x10000, 0x8000);
            Call3(Func_020044e6, 16, 0x240, 152);
            Func_020044ac(6);
            Func_0200451e(16, 6, 30);
            Func_020040ee(16, 6);
            Func_02004526(0, 3);
            Func_020044cc(2);
            Func_02004534(16, 4);
            Func_0200410c(16, 6);
            Call2(Func_0200459e, 0, 0x102);
            Func_020044ec(40);
            Func_02004564(16, 2);
            Func_020044fa(30);
            Func_02004132(16, 6);
            Func_0200456a(0, 3);
            Call3(Func_02004556, 16, 0x240, 184);
            Func_0200451c(6);
            Func_02004594(16, 2);
            Func_0200452a(40);
            Call2(Func_02004162, 0x4010, 6);
            Call3(Func_0200457e, 16, 0x240, 208);
            Func_02004544(40);
            Func_020045ac_a(0, 3);
            Func_02004552(6);
            Call3(Func_02004588_a, 16, 0x8000, 0x4000);
            Func_020045c0(16, 2);
            record = Value1(Func_0200458e, 0);
            if (record != 0) {
                Func_020045b0(16, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_020045ce(16);
            Func_020045e0(16, 0, 0);
            Call1(Func_02004588_b, 0x810);
            Func_020045ac_b();
        }
    }
}

void FieldScene_RunScene37a_02002924(void)
{
    u32 i;
    s32 record;

    Func_020052fc();
    if (Value1(Func_020052e2, 0x896) != 0) {
        Call1_02002924(Func_0200538c, 0xffd);
    } else {
        Call1_02002924(Func_02005394, 0xfff);
    }
    Func_020053b6(16, 0, 10);
    Call3(Func_020053ca, 16, 0xc000, 10);
    Func_02005336();
}

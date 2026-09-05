#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000ffc[];
extern u8 Data_00004010[];
extern u8 Data_03001ebc[];
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
s32 Func_0200354e();
void Func_02003554();
void Func_02003558();
void Func_0200355e();
void Func_02003560();
void Func_02003572();
void Func_0200357e();
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

void FieldScene_RunScene37a_020009f4(void)
{
    u32 i;
    s32 record;
    s32 base6_ffc;
    s32 base5_4010;
    s32 base5_4010_2;

    Call1(Func_0200344e, 0xff6);
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
    Value2(Func_0200354e, base5_4010, 0);
    if (Value2(Func_020034de, 0, 0) == 0) {
        Call1(Func_02003558, 0xffa);
    } else {
        Call1(Func_02003560, 0xffb);
    }
    base5_4010_2 = (s32)Data_00004010;
    Call3(Func_0200358e, 0, 0xa000, 10);
    Func_0200311e(base5_4010_2, 10);
    base6_ffc = (s32)Data_00000ffc;
    Func_0200357e(base6_ffc);
    Func_020035a8(16, 0, 40);
    Call3(Func_020035ba, 16, 0x105, 40);
    Func_02003572(16, 4);
    Call3(Func_020035c6, 16, 0x3000, 10);
    Func_0200357e(16, 4);
    Value2(Func_020035be, base5_4010_2, 0);
    if (Value2(Func_0200354e, 0, 0) == 0) {
        Func_020035c8((base6_ffc + 1));
        Call1(Func_02003536, 0x896);
    } else {
        Func_020035d6((base6_ffc + 2));
    }
    Call2(Func_02003186, (s32)Data_00004010, 4);
    Func_0200361e(16, 1);
    Call3(Func_020035aa, 16, 0x1e6, 131);
    Call3(Func_020035b6, 16, 0x240, 120);
    Call3(Func_0200362a, 16, 0xc000, 2);
    Call2(Func_02003656, 0x40000, 0x8000);
    Call1(Func_02003584, 0x80a);
}

#include "types.h"

#define FieldScene_RunCommandSequence Func_02000db8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004a76();
s32 Func_02004a7e();
void Func_02004aac();
void Func_02004aba();
void Func_02004ac8();
void Func_02004ad6();
void Func_02004ae4();
s32 Func_02004b28();
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
void Func_02004d5c();
void Func_02004d60();
void Func_02004d7a();
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
s32 Func_02004db2();
void Func_02004dbc();
void Func_02004dbe();
void Func_02004dc2();
void Func_02004dc6();
void Func_02004ddc();
void Func_02004dde();
s32 Func_02004dec();
void Func_02004dee();
void Func_02004df6();
void Func_02004df8();
void Func_02004e02();
void Func_02004e04();
s32 Func_02004e0c();
void Func_02004e0e();
void Func_02004e12();
void Func_02004e1e();
void Func_02004e28();
s32 Func_02004e2c();
void Func_02004e2e();
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

void FieldScene_RunCommandSequence(s32 a0)
{
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
    Call3(Func_02004aac, a0, 0x10000, 0x8000);
    Call3(Func_02004aba, 0, 0x10000, 0x8000);
    Call3(Func_02004ac8, 1, 0x10000, 0x8000);
    Call3(Func_02004ad6, 2, 0x10000, 0x8000);
    Call3(Func_02004ae4, 3, 0x10000, 0x8000);
    Call3((void (*)())Func_02004b28, 0, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x300000));
    Func_02004b3a(1, ((s32)((s32)p9 << 16) + -0x100000), (-0x280000 + (s32)((s32)p10 << 16)));
    Func_02004b48(2, ((s32)((s32)p9 << 16) + 0x100000), (-0x280000 + (s32)((s32)p10 << 16)));
    Func_02004b54(3, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x200000));
    Func_02004b62(a0, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x500000));
    record = Func_02004b28(0);
    {
        s32 shown = 0xc000;
    
        *(volatile u16 *)(record + 6) = shown;
    }
    Func_02004be6(0, 0);
    Func_02004c32();
    Func_02004c46();
    Call1(Func_02004bbc, 0x20cb);
    Func_02004bd4(a0, 0);
    Call3(Func_02004bfe, 3, 0x101, 60);
    Func_02004be6(3, 0);
    Func_02004bbe(a0, 3);
    Func_02004bf6(a0, 0);
    Call3(Func_02004c20, 2, 0x101, 60);
    Func_02004c08(2, 0);
    Func_02004bf2(a0, 2, 0);
    Func_02004b70(20);
    Func_02004be8(a0, 3);
    Func_02004c28(a0, 0);
    Call3(Func_02004c52, 1, 0x101, 60);
    Func_02004c3a(1, 0);
    Call3(Func_02004c64, 3, 0x101, 60);
    Func_02004c4c(3, 0);
    Call3(Func_02004c78, a0, 0x102, 60);
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
        Call3(Func_02004cca, a0, 0xa000, 0);
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
        Call3(Func_02004d32, a0, 0x6000, 0);
        Func_02004d5a(a0, 0);
        Call4(Func_02004da4, 0x5180000, -1, 0xa80000, 1);
        Func_02004db0();
        Func_02004d5a(a0, 0, 0);
        Func_02004d82(a0, 0);
        Func_02004d8a(a0, 0);
        Func_02004d92(a0, 0);
        Func_02004dc2(0, 0);
        Func_02004d7a(a0, 2);
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
    Call3(Func_02004df8, 0, 0x4000, 0);
    Func_02004e02(1, 0, 0);
    Call3(Func_02004e0e, 2, 0x8000, 0);
    Func_02004dde(1, 2);
    Func_02004e0e(1, 0);
    Func_02004dee(2, 2);
    Func_02004e1e(2, 0);
    Func_02004dee(3, 3);
    Func_02004e2e(3, 0);
    Func_02004df6(3, 3);
    Func_02004d94(1);
    Func_02004e04(1, 3);
    Func_02004da2(2);
    Func_02004e12(2, 3);
    Func_02004db0(1);
    Func_02004e28(0, 3);
    Func_02004dbe(6);
    Func_02004e2e(1, 2);
    record = Value1(Func_02004dec, 0);
    if (record != 0) {
        Func_02004e1e(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
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
    Call3(Func_02004f26, a0, 0xc000, 10);
    Func_02004e82();
    p9b = (p9 - 16);
    p10b = ((s32)p9 << 16);
}

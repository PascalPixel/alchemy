#include "TYPES.H"

#define BiribinoNiwa_RunGardenEvent Func_020001e4

s32 Func_02000ca2();
s32 Func_02000cae();
s32 Func_02000cb8();
void Func_02000cbc();
void Func_02000cc4();
void Func_02000cf6();
s32 Func_02000d5c();
void Func_02000d62();
void Func_02000d78();
void Func_02000d84();
u8 *Func_02000d8a();
void Func_02000d94();
void Func_02000d98();
void Func_02000da4();
void Func_02000da6();
void Func_02000dac();
void Func_02000db4();
void Func_02000dbc();
void Func_02000dc2();
s32 Func_02000dc4();
void Func_02000dd0();
void Func_02000dfc();
void Func_02000e38();
void Func_02000e4c();
void Func_02000e56();
void Func_02000e58();
void Func_02000e64();
void Func_02000e66();
void Func_02000e68();
s32 Func_02000e72();
void Func_02000e74();
void Func_02000e78();
void Func_02000e90();
u8 *Func_02000e94();
void Func_02000e96();
void Func_02000ea0();
void Func_02000eb0();
void Func_02000eb4();
void Func_02000ebc();
void Func_02000ec8();
void Func_02000eca();
void Func_02000ecc();
void Func_02000ed8();
void Func_02000ee6();
void Func_02000ef4();
void Func_02000efa();
s32 Func_02000f16();
void Func_02000f50();
void Func_02000f54();
void Func_02000f72();
void Func_02000f96();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

extern u8 Data_02008bf4[];

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}
void Func_02000da4_a();
void Func_02000da4_b();
void Func_02000e58_a();
void Func_02000e58_b();
s32 Func_02000e72_a();
s32 Func_02000e72_b();
void Func_02000e90_a();
void Func_02000e90_b();

void BiribinoNiwa_RunGardenEvent(void)
{
    u8 *record;

    Func_02000cbc();
    if (Value1(Func_02000ca2, 0x84a) != 0) {
        if (Value1(Func_02000cae, 0x304) != 0) {
            if (Value1(Func_02000cb8, 0x201) == 0) {
                Call1(Func_02000d62, 0x1414);
                Func_02000d84(12, 0, 10);
                Call3(Func_02000da6, 12, 0x107, 40);
                Func_02000d98(12, 0, 10);
                Func_02000d78(12, 2);
                Call1(Func_02000cf6, 0x201);
            }
            Call1(Func_02000d94, 0x1416);
            Func_02000dac(12, 0);
            goto L_0200041c;
        }
        Call1(Func_02000da4_a, 0x1413);
        Func_02000dbc(12, 0);
    } else {
        Call1(Func_02000db4, 0x140d);
        Value2(Func_02000dc4, 12, 0);
        if (Value2(Func_02000d5c, 0, 0) != 0) {
            goto L_02000408;
        }
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Func_02000dfc(12, 0, 10);
        record = Func_02000d8a(0);
        if (*(s32 *)((s32)record + 16) <= 0x10dffff) {
            Call3(Func_02000da4_b, 12, 0xcccc, 0x6666);
            Call3(Func_02000dc2, 0, 0x15a, 0x112);
            Call3(Func_02000dd0, 0, 0x148, 0x11a);
            Call3(Func_02000e4c, 0, 0xc000, 0);
        }
        Call3(Func_02000e58_a, 11, 0x1000, 0);
        Call3(Func_02000e64, 12, 0x7000, 20);
        Call3(Func_02000e78, 11, 0x102, 20);
        Func_02000e38(11, 1);
        Call3((void (*)())Func_02000e72_a, 11, 0, 10);
        Call3(Func_02000e96, 12, 0x108, 60);
        Func_02000e56(12, 1);
        Func_02000e90_a(12, 0, 20);
        Func_02000e58_b(11, 3);
        Func_02000e68(12, 3);
        Call3(Func_02000ebc, 11, 0x3000, 0);
        Call3(Func_02000ec8, 12, 0x5000, 10);
        Func_02000e90_b(11, 1);
        Func_02000eca(11, 0, 20);
        Call3(Func_02000ee6, 11, 0xf000, 0);
        Call3(Func_02000e74, 12, 0x10000, 0x8000);
        *(u8 *)(Func_02000e72_b(12) + 90) &= 254;
        Call3(Func_02000ea0, 12, 0x15a, 0x107);
        Func_02000e66(1);
        {
            u8 *record = Func_02000e94(12);
            /* FAKEMATCH: the flag byte is read through a volatile access. */
            u8 value = *(volatile u8 *)&record[90];
        
            record[90] = (u8)(value | 1);
        }
        Call3(Func_02000eb0, 11, 0x9999, 0x4ccc);
        Call3(Func_02000ecc, 11, 0x148, 0x107);
        Call3(Func_02000ed8, 11, 0x148, 252);
        Call3(Func_02000f54, 11, 0xc000, 10);
        Func_02000cc4();
        Call3(Func_02000ef4, 11, 0x148, 246);
        Value3(Func_02000f16, 11, 0, 0);
        /* FAKEMATCH: the do/while keeps this call after the argument setup
         * of the previous one. */
        do {
            Call1(Func_02000eb4, 0x84a);
        } while (0);
    }
    Call3(Func_02000f50, 12, 0x10000, (s32)Data_02008bf4);
    goto L_0200041c;
    L_02000408:;
    Func_02000f72(12, 0);
    Call3(Func_02000f96, 12, 0x3000, 10);
    L_0200041c:;
    Func_02000efa();
}

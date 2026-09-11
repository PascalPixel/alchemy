#include "types.h"

#define Scene_RunScene3b8SequenceC Func_020040b4

extern u8 Data_0000008b[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];
void Func_02004974();
void Func_02005a62();
void Func_020061e2();
void Func_0200842c();
void Func_02008432();
void Func_02008438();
void Func_02008488();
void Func_0200849a();
void Func_020084a2();
s32 Func_020084c2();
s32 Func_020084c6();
void Func_020084ca();
s32 Func_020084cc();
void Func_020084ec();
void Func_020084ee();
void Func_02008516();
s32 Func_02008544();
s32 Func_02008552();
void Func_0200855a();
void Func_02008564();
s32 Func_0200858a();
s32 Func_02008594();
u8 *Func_020085dc();
s32 Func_020085e2();
s32 Func_02008602();
void Func_02008612();
u8 *Func_0200861a();
void Func_02008622();
void Func_02008626();
void Func_0200862c();
void Func_02008632();
void Func_02008636();
s32 Func_02008642();
void Func_02008646();
void Func_0200865e();
s32 Func_02008662();
void Func_02008672();
void Func_02008684();
void Func_02008686();
void Func_0200868a();
void Func_0200868c();
void Func_02008692();
void Func_020086a6();
void Func_02008748();
s32 Func_020087ac();

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

s32 Func_020040b4(void)
{
    u32 i;
    s32 rec7;
    u8 *record;
    s32 r13;
    s32 base5_2000402;
    s32 v5;
    s32 base6_0;
    u8 *p6;
    u8 slot16[16];

    Func_0200842c(1);
    Func_02008432(2);
    Func_02008438(4);
    base5_2000402 = 0x2000402;
    if (Data_02000240_t[225][0] == 90) {
        Call3(Func_02008488, 0x962, 0, *(u16 *)base5_2000402);
    }
    if ((*(u16 *)base5_2000402 << 16) == 0x5b0000) {
        Call1(Func_0200849a, 0x962);
        Call1(Func_020084a2, 0x950);
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000008b) {
    } else {
        if (Data_02000240_t[225][0] == 11) {
            Call1(Func_020084ca, 0x12f);
        }
        if (Value1(Func_020084c2, 0x950) != 0) {
            rec7 = Value1(Func_020084cc, 0xf31);
            if (rec7 != 0) {
                ((void (*)())Func_02008594)(16, 0, 0);
            } else {
                record = Value1(Func_02008544, 16);
                record[92] = 1;
                record[85] = rec7;
                p6 = *(s32 *)((s32)record + 80);
                *(s32 *)((s32)record + 12) = 0x40000;
                p6[39] = rec7;
                p6[5] &= -33;
                p6[9] &= 15;
                rec7 = Value2(Func_020084c6, 17, 0x608);
                Func_02008516(205);
                Func_020084ee(p6[28], 128, (rec7 + 0x400));
                Func_020084ec(17);
            }
            if (Data_02000240_t[225][0] == 33) {
                if (Value1(Func_02008552, 0x96f) == 0) {
                    Call1(Func_02008564, 0x96f);
                    Call3(Func_02008622, 14, 0xd00000, 0x2c00000);
                    Func_020061e2();
                }
            }
            Func_02008636(14, 5);
            record = Func_020085dc(14);
            Func_0200855a((s32)record, 0);
        } else {
            if (Value1(Func_0200858a, 0x962) != 0) {
                if (Value1(Func_02008594, 0x966) == 0) {
                    Call3(Func_0200865e, 10, 0x780000, 0x480000);
                }
            }
        }
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
        {
            u8 *record = Func_0200861a(9);
            u8 value = *(volatile u8 *)&record[89];
        
            record[89] = (u8)(value | 4);
        }
        if (Data_02000240_t[225][0] == 99) {
            record = Value1(Func_02008602, slot16);
            if ((s32)record > 0) {
                base6_0 = 0;
                v5 = (s32)record;
                do {
                    record = Value1(Func_020085e2, *(s16 *)(base6_0 + slot16));
                    *(u16 *)((s32)record + 56) = *(u16 *)((s32)record + 52);
                    v5 = (v5 - 1);
                    *(u16 *)((s32)record + 58) = *(u16 *)((s32)record + 54);
                    Func_02008612(*(s16 *)(base6_0 + slot16));
                    base6_0 = (base6_0 + 2);
                } while (v5 != 0);
            }
            Func_02008626(1);
            Func_0200862c(2);
            Func_02008632(3);
            Func_02008646();
            Func_02005a62();
            {
                s32 shown = 8;
            
                *(u16 *)0x02000402 = shown;
            }
        }
        if (Data_02000240_t[225][0] == 98) {
            record = Value1(Func_02008662, r13);
            if ((s32)record > 0) {
                base6_0 = 0;
                v5 = (s32)record;
                do {
                    record = Value1(Func_02008642, *(s16 *)(base6_0 + r13));
                    *(u16 *)((s32)record + 56) = *(u16 *)((s32)record + 52);
                    v5 = (v5 - 1);
                    *(u16 *)((s32)record + 58) = *(u16 *)((s32)record + 54);
                    Func_02008672(*(s16 *)(base6_0 + r13));
                    base6_0 = (base6_0 + 2);
                } while (v5 != 0);
            }
            Func_02008686(1);
            Func_0200868c(2);
            Func_02008692(3);
            Func_020086a6();
            Call1(Func_02008684, 0x966);
            Call1(Func_0200868a, 0x967);
            Call3(Func_02008748, 10, 0x380000, 0x780000);
            Value3(Func_020087ac, 10, 0xf000, 0);
            Func_02004974();
            {
                s32 shown = 8;
            
                *(u16 *)0x02000402 = shown;
            }
        }
    }
    return 0;
}

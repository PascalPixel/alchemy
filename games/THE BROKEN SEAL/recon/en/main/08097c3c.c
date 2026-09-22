#include "types.h"

#define Func_08097c3c Func_08097c3c

void Func_080030f8();
void Func_0800447c();
void Func_08009080();
s32 Func_08009098();
s32 Func_08009150();
void Func_08009158();
s32 Func_080091d8();
s32 Func_08009240();
s32 Func_080092a0();
s32 Func_080092a8();
u8 *Func_08092054();
void Func_08097174();
void Func_08097384();
void Func_0809748c();
s32 Func_08097b54();
s32 Func_08098070();
void Func_08098184();
void Func_080981b0();
void Func_080f9010();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void Func_08097c3c(void)
{
    u32 i;
    s32 p8;
    s32 p8b;
    s32 rec4;
    u8 *record;
    s32 value;
    s32 r7;
    s32 r6;
    s32 r11;
    s32 r10;
    s32 r5;
    s32 r9;
    s32 slot24;
    s32 slot20;
    s32 slot8;
    u8 slot4[24];
    u8 *slot0;
    s32 slot16;
    s32 slot12;
    u8 *p6;
    u8 *p4;
    u8 *p5;
    u8 slot28[24];

    (*(s32 *)((*(s32 *)slot4) + 20)) = *(s32 *)0x03001f30;
    (*(s32 *)((*(s32 *)slot4) + 16)) = *(s32 *)((*(s32 *)((*(s32 *)slot4) + 20)) + 16);
    p6 = *(s32 *)((*(s32 *)((*(s32 *)slot4) + 20)) + 20);
    (*(s32 *)((*(s32 *)slot4) + 4)) = (*(s32 *)((*(s32 *)((*(s32 *)slot4) + 20))) + 0x8000);
    (*(s32 *)slot4) = 0;
    if ((s32)p6 == 0) {
    } else {
        Func_08097384();
        *(s32 *)((*(s32 *)((*(s32 *)slot4) + 16)) + 104) = (s32)p6;
        Call2(Func_08009098, (*(s32 *)((*(s32 *)slot4) + 16)), 0x809f0bc);
        rec4 = Value1(Func_08098070, (*(s32 *)((*(s32 *)slot4) + 16)));
        if (rec4 == 0) {
            Func_0809748c();
        } else {
            *(s32 *)(rec4 + 104) = (s32)p6;
            *(s32 *)0x00000028 = *(s32 *)((s32)p6 + 8);
            *(s32 *)0x0000002c = (*(s32 *)((s32)p6 + 12) + 0x100000);
            *(s32 *)0x00000030 = *(s32 *)((s32)p6 + 16);
            Func_0800447c(0x100000, (*(s32 *)((*(s32 *)slot4) + 4)), 40);
            ((void (*)())Func_08009150)(rec4, *(s32 *)0x00000028, *(s32 *)0x0000002c, *(s32 *)0x00000030);
            Func_08098184(rec4);
            *(s32 *)(rec4 + 48) = 0x40000;
            *(s32 *)(rec4 + 52) = 0x8000;
            slot0 = (rec4 + 85);
            slot0[0] = 4;
            *(s32 *)((s32)p6 + 108) = 0x8096b89;
            *(s32 *)((s32)p6 + 48) = 0x6666;
            *(s32 *)((s32)p6 + 52) = 0x3333;
            p6[90] = *(u8 *)(slot4);
            p6[34] = 2;
            goto L_08097ee4;
            L_08097d00:;
            value = Func_08097b54(*(s32 *)0x03001ae8);
            p8 = ((u32)(value << 16) >> 16);
            if (p8 == 0xffff) {
                *(s32 *)(r7) = *(s32 *)(r6 + 8);
                *(s32 *)(r7 + 4) = (*(s32 *)(r6 + 12) + r11);
                *(s32 *)(r7 + 8) = *(s32 *)(r6 + 16);
                Func_0800447c(r11, (*(s32 *)((*(s32 *)slot4) + 4)), r7);
                Value4(Func_08009150, r10, *(s32 *)(r7), *(s32 *)(r7 + 4), *(s32 *)(r7 + 8));
                Func_08009080(r10, 1);
                *(s32 *)(r10 + 36) = r5;
                *(s32 *)(r10 + 40) = r5;
                *(s32 *)(r10 + 44) = r5;
            } else {
                *(s32 *)(r7) = *(s32 *)(r6 + 8);
                *(s32 *)(r7 + 4) = (*(s32 *)(r6 + 12) + r11);
                *(s32 *)(r7 + 8) = *(s32 *)(r6 + 16);
                Func_0800447c(r11, (*(s32 *)((*(s32 *)slot4) + 4)), r7);
                Call3(Func_0800447c, 0x20000, p8, r7);
                ((void (*)())Func_08009150)(r10, *(s32 *)(r7), *(s32 *)(r7 + 4), *(s32 *)(r7 + 8));
                Func_08009158(r10);
                *(s32 *)(r7) = *(s32 *)(r6 + 8);
                *(s32 *)(r7 + 4) = *(s32 *)(r6 + 12);
                *(s32 *)(r7 + 8) = *(s32 *)(r6 + 16);
                Func_0800447c(r11, p8, r7);
                *(s32 *)(slot28) = *(s32 *)(r6 + 8);
                *(s32 *)(slot28 + 4) = *(s32 *)(r6 + 12);
                *(s32 *)(slot28 + 8) = *(s32 *)(r6 + 16);
                Call3(Func_0800447c, 0x200000, p8, slot28);
                if (Value2(Func_080091d8, r6, r7) <= 0) {
                    record = Value2(Func_080092a0, r6, r9);
                    if ((s32)record == 0) {
                        goto L_08097e36;
                    }
                    if ((s32)record == (*(s32 *)((*(s32 *)slot4) + 16))) {
                        p4 = *(s32 *)((*(s32 *)((*(s32 *)slot4) + 16)) + 16);
                        if ((*(s32 *)((*(s32 *)((*(s32 *)slot4) + 16)) + 8) & -0x100000) == (*(s32 *)(r9) & -0x100000)) {
                            if (((s32)p4 & -0x100000) == (*(s32 *)(r9 + 8) & -0x100000)) {
                                goto L_08097e16;
                            }
                        }
                        if ((*(s32 *)((*(s32 *)((*(s32 *)slot4) + 16)) + 8) & -0x100000) != (*(s32 *)(slot28) & -0x100000)) {
                            goto L_08097e36;
                        }
                        if (((s32)p4 & -0x100000) != (*(s32 *)(slot28 + 8) & -0x100000)) {
                            goto L_08097e36;
                        }
                        record = Value1(Func_080092a8, (*(s32 **)((*(s32 *)slot4) + 16))[34]);
                        if ((s32)record == 0) {
                            goto L_08097e32;
                        }
                    }
                }
                L_08097e16:;
                Func_08009080(r10, 4);
                if ((*(s32 *)0x03001e40 & 15) == 0) {
                    Func_080f9010(114);
                    goto L_08097ee4;
                    L_08097e32:;
                    (*(s32 *)slot4) = 1;
                    L_08097e36:;
                    Func_080f9010(175);
                    (*(s32 *)((*(s32 *)slot4) + 12)) = *(s32 *)(r7);
                    (*(s32 *)((*(s32 *)slot4) + 8)) = *(s32 *)(r7 + 8);
                    Func_08009080(r10, *(u8 *)(0x0809f118 + (s32)((u32)(s32)((s32)((*(s32 *)((*(s32 *)slot4) + 4)) - (s32)p8) << 16) >> 30)));
                    Func_080030f8(15);
                    *(u8 *)(r6 + 91) = 0;
                    *(s32 *)(r6 + 48) = 0x3333;
                    *(s32 *)(r6 + 52) = 0x3333;
                    ((void (*)())Func_08009150)(r6, *(s32 *)(r7), *(s32 *)(r7 + 4), *(s32 *)(r7 + 8));
                    slot0[0] = 0;
                    *(s32 *)(r10 + 48) = 0x3333;
                    *(s32 *)(r10 + 52) = 0x3333;
                    Func_0800447c(r11, p8, r7);
                    ((void (*)())Func_08009150)(r10, *(s32 *)(r7), (*(s32 *)(r7 + 4) + r11), *(s32 *)(r7 + 8));
                    if ((*(s32 *)slot4) == 1) {
                        *(u8 *)(Func_08092054(*(s16 *)((*(s32 *)((*(s32 *)slot4) + 20)) + 24)) + 90) &= 254;
                        *(s32 *)((*(s32 *)((*(s32 *)slot4) + 16)) + 48) = 0x3333;
                        *(s32 *)((*(s32 *)((*(s32 *)slot4) + 16)) + 52) = 0x3333;
                        ((void (*)())Func_08009150)((*(s32 *)((*(s32 *)slot4) + 16)), *(s32 *)(slot28), *(s32 *)(slot28 + 4), *(s32 *)(slot28 + 8));
                    }
                    Func_08009158(r6);
                    *(s32 *)(r6 + 8) = (*(s32 *)((*(s32 *)slot4) + 12));
                    *(s32 *)(r6 + 16) = (*(s32 *)((*(s32 *)slot4) + 8));
                    *(s32 *)(r6 + 36) = 0;
                    *(s32 *)(r6 + 44) = 0;
                    goto L_08097ef8;
                }
            }
            L_08097ee4:;
            Func_080030f8(1);
            p5 = *(s32 *)0x03001c94;
            if (((s32)p5 & 0x303) == 0) {
                goto L_08097d00;
            }
            L_08097ef8:;
            Value2(Func_08009240, (s32)p6, (*(s32 **)((*(s32 *)slot4) + 20))[68]);
            Value2(Func_08009098, (s32)p6, *(s32 *)((*(s32 *)((*(s32 *)slot4) + 20)) + 60));
            *(s32 *)((s32)p6 + 108) = *(s32 *)((*(s32 *)((*(s32 *)slot4) + 20)) + 56);
            Func_08097174();
            if ((*(s32 *)slot4) == 1) {
                {
                    u8 *record = Func_08092054(*(s16 *)((*(s32 *)((*(s32 *)slot4) + 20)) + 24));
                    u8 value = *(volatile u8 *)&record[90];
                
                    record[90] = (u8)(value | 1);
                }
            }
            Func_0809748c();
            Func_080981b0(r10);
        }
    }
    p8b = (*(s32 *)((*(s32 *)slot4) + 4));
}

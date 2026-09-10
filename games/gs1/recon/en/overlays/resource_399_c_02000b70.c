#include "types.h"

#define FieldScene_RunScene399SequenceB Func_02000b70

extern u8 Data_00000032[];
extern u8 Data_00000033[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];
void Func_02001ca0();
void Func_02002550();
void Func_020025cc();
void Func_0200294e();
s32 Func_02002d48();
s32 Func_02002d52();
s32 Func_02002db8();
s32 Func_02002dd2();
void Func_02002dd4();
void Func_02002dda();
s32 Func_02002df8();
void Func_02002e00();
s32 Func_02002e06();
void Func_02002e52();
void Func_02002e5a();
s32 Func_02002e6c();
void Func_02002e6e();
void Func_02002e74();
u8 *Func_02002e8c();
void Func_02002eb2();
s32 Func_02002eba();
s32 Func_02002ec4();
void Func_02002ecc();
void Func_02002ee6();
void Func_02002ef0();
void Func_02002f04();
u8 *Func_02002f06();
void Func_02002f0a();
s32 Func_02002f16();
u8 * Func_02002f20();
u8 * Func_02002f20_a();
void Func_02002f26();
void Func_02002f26_a();
u8 *Func_02002f42();
void Func_02002f52();
s32 Func_02002f58();
void Func_02002f7e();
void Func_02002f82();
s32 Func_02002f9c();
void Func_02002f9e();
void Func_02002fa4();
u8 *Func_02002fa8();
s32 Func_02002fb2();
s32 Func_02002fc2();
void Func_02002fce();
void Func_02002fd4();
void Func_02002fd8();
u8 *Func_02002fe0();
void Func_02002ffe();
void Func_0200300a();
s32 Func_02003018();
void Func_02003036();
void Func_0200304a();
u8 *Func_0200304e();
s32 Func_02003060();
u8 *Func_02003066();
u8 *Func_02003070();
void Func_02003072();
u8 *Func_02003084();
void Func_0200308c();
void Func_020030a8();
u8 *Func_020030b8();
u8 *Func_020030d6();
void Func_020030ea();
void Func_020030ee();
void Func_020030f2();
u8 *Func_0200310a();
void Func_02003112();
u8 *Func_0200311a();
u8 *Func_02003124();
s32 Func_02003128();
s32 Func_02003138();
s32 Func_0200314c();
s32 Func_02003162();
void Func_02003164();
u8 *Func_0200318e();
void Func_02003192();
void Func_020031a2();
void Func_020031e4();
void Func_02003212();
void Func_02003226();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_02000b70(s32 a0)
{
    u32 i;
    s32 rec7;
    u8 *record;
    s32 v5;
    s32 v6;

    if (Data_02000240_t[224][0] == (s32)Data_00000032) {
        rec7 = Func_02002dd2(0);
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x100;
        Func_02002e5a(10, 9);
        if (Value1(Func_02002db8, 0x109) != 0) {
            Call1(Func_02002dd4, 0x200);
            Call1(Func_02002dda, 0x201);
        }
        {
            s32 shown = 0;
        
            *(u16 *)(rec7 + 100) = shown;
        }
        {
            s32 shown = 0;
        
            *(u16 *)((rec7 + 100) + 2) = shown;
        }
        Value2(Func_02002d48, 0x2009795, 0xc80);
        Value2(Func_02002d52, 0x20098c5, 0xc80);
        Func_02002f0a(11, 1);
        if (Value1(Func_02002df8, 0x203) != 0) {
            Func_02002550();
        }
        if (Value1(Func_02002e06, 0x109) != 0) {
            goto L_02000f78;
        }
        if (Data_02000240_t[225][0] != 9) {
            goto L_02000f78;
        }
        Func_020025cc();
    } else {
        if (Data_02000240_t[224][0] != (s32)Data_00000033) {
        } else {
            *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
            if (*(s16 *)(0x2000402) != 1) {
                v5 = *(s16 *)(0x2000402);
            } else {
                Func_02002f26(21, 15);
                {
                    u8 *record = Func_02002e8c(21);
                    u8 value = *(volatile u8 *)&record[89];
                
                    record[89] = (u8)(value | 8);
                }
                Func_02002f7e(21, 1);
                if (Value1(Func_02002e6c, 0x881) != 0) {
                    Call6(Func_02002e74, 10, 7, 1, 1, 10, 8);
                    Call6(Func_02002e6e, 3, 125, 9, 69, 3, 3);
                    Func_02002e52();
                    Func_02002e00(1);
                    Func_02002ef0(8, 2);
                    Func_02002f52(10, 0, 0);
                    goto L_02000f78;
                }
                if (Value1(Func_02002eba, 0x82c) != 0) {
                    if (Value1(Func_02002ec4, 0x82a) != 0) {
                        record = Func_02002f06(10);
                        Func_02002ecc((s32)record, 0);
                        Call3(Func_02002f82, 9, 0xae0000, 0xa40000);
                        record = Func_02002f20(9);
                        Func_02002ee6((s32)record, 0);
                        Func_02002f9e(9, 5);
                        Call3(Func_02002fa4, 8, 0xa80000, 0x980000);
                        record = Func_02002f42(8);
                        {
                            s32 shown = 0x3000;
                        
                            *(u16 *)((s32)record + 6) = shown;
                        }
                        if (Value1(Func_02002f16, 0x82b) != 0) {
                            goto L_02000f78;
                        }
                        Func_02001ca0();
                        goto L_02000f78;
                    }
                }
                Call6(Func_02002f26_a, 10, 7, 1, 1, 10, 8);
                Call6((void (*)())Func_02002f20_a, 3, 125, 9, 69, 3, 3);
                Func_02002f04();
                Func_02002eb2(1);
                if (Value1(Func_02002f58, 0x82c) != 0) {
                    Call3(Func_0200300a, 8, 0x950000, 0x740000);
                    v5 = 0;
                    record = Func_02002fa8(8);
                    *(u16 *)((s32)record + 6) = v5;
                    *(u16 *)(Func_02002fb2(9) + 102) = v5;
                    Call2(Func_02002fce, 9, 0x200a4f4);
                    goto L_02000f78;
                }
                Func_02002fd8(8, 2);
                goto L_02000f78;
            }
            if (v5 == 2) {
                if (Value1(Func_02002f9c, 0x881) != 0) {
                    goto L_02000f78;
                }
                {
                    u8 *record = Func_02002fe0(11);
                    s32 shown = 1;
                
                    *(u16 *)((s32)record + 102) = shown;
                }
                Call2(Func_02002ffe, 11, 0x200a4f4);
            } else {
                if (v5 == 4) {
                    if (Value1(Func_02002fc2, 0x881) != 0) {
                        Call3(Func_02003072, 12, 0x16c0000, 0x2420000);
                        v6 = 4;
                        Func_020030f2(12, 2);
                        v5 = 3;
                        *(u8 *)(Func_02003018(12) + 89) |= v6;
                        Call6(Func_02002fd4, 6, 125, 22, 88, v5, v5);
                        Call3(Func_020030a8, 13, 0x1ec0000, 0x2420000);
                        Value2(Func_02003128, 13, 2);
                        {
                            u8 *record = Func_0200304e(13);
                            u8 value = *(volatile u8 *)&record[89];
                        
                            record[89] = (u8)(value | v6);
                        }
                        goto L_02000e9e;
                    }
                    record = Func_02003066(12, 125, 28, 88);
                    *(s32 *)((s32)record + 24) = -0x10000;
                    record = Func_02003070(12);
                    Func_02003036((s32)record, 0);
                    Func_020030ee(12, 5);
                    record = Func_02003084(13);
                    Func_0200304a((s32)record, 0);
                } else {
                    if (v5 != 3) {
                        goto L_02000f34;
                    }
                    if (Value1(Func_02003060, 0x881) != 0) {
                        Call3(Func_02003112, 15, 0x1cc0000, 0x1020000);
                        Func_02003192(15, 2);
                        {
                            u8 *record = Func_020030b8(15);
                            u8 value = *(volatile u8 *)&record[89];
                        
                            record[89] = (u8)(value | 4);
                        }
                        Value3(Func_02003138, 14, 0x1980000, 0x1080000);
                        record = Func_020030d6(14);
                        {
                            s32 shown = 0x1000;
                        
                            *(u16 *)((s32)record + 6) = shown;
                        }
                        L_02000e9e:;
                        Call6(Func_0200308c, 9, 125, 28, 88, v5, v5);
                        goto L_02000f78;
                    }
                    Call3(Func_02003164, 14, 0x1cc0000, 0x1020000);
                    Func_020031e4(14, 2);
                    {
                        u8 *record = Func_0200310a(14);
                        u8 value = *(volatile u8 *)&record[89];
                    
                        record[89] = (u8)(value | 4);
                    }
                    record = Func_0200311a(15);
                    *(s32 *)((s32)record + 24) = -0x10000;
                    record = Func_02003124(15);
                    Func_020030ea((s32)record, 0);
                }
                Func_020031a2(13, 5);
                goto L_02000f78;
                L_02000f34:;
                if (v5 == 7) {
                    if (Value1(Func_0200314c, 0x881) != 0) {
                        record = Func_0200318e(20);
                        {
                            s32 shown = 0x3000;
                        
                            *(u16 *)((s32)record + 6) = shown;
                        }
                        if (Value1(Func_02003162, 0x82e) == 0) {
                            Call3(Func_02003212, 20, 0x28a0000, 0xa10000);
                            Func_0200294e();
                        } else {
                            Call3(Func_02003226, 20, 0x2840000, 0xa60000);
                        }
                    }
                }
            }
        }
    }
    L_02000f78:;
    return 0;
}

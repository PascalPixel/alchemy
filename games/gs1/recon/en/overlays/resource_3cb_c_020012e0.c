#include "types.h"

#define FieldScene_RunScene3cbSequenceA Func_020012e0

extern u8 Data_00000041[];
extern u8 Data_0000004b[];
extern u8 Data_0000004c[];
extern u8 Data_00000054[];
extern u8 Data_02000240[];
extern u16 Data_02000240_t[][2];
void Func_02001450();
void Func_020014dc();
void Func_020015c0();
void Func_020015c4();
void Func_020015e6();
void Func_02001656();
void Func_02001672();
void Func_02001678();
void Func_020016b2();
void Func_0200170e();
void Func_02001714();
void Func_020017a0();
void Func_020017a6();
s32 Func_0200228a();
void Func_020025a2();
void Func_020026ae();
void Func_02002724();
void Func_0200277c();
void Func_020027c6();
void Func_02002850();
void Func_02002a42();
void Func_02002a5c();
void Func_02002ac6();
void Func_02002ad0();
void Func_02002b70();
void Func_02002b78();
void Func_02002bea();
s32 Func_02002bf6();
s32 Func_02002c0c();
void Func_02002c22();
void Func_02002c32();
void Func_02002c50();
void Func_02002c66();
void Func_02002c68();
void Func_02002c70();
void Func_02002c72();
s32 Func_02002c84();
s32 Func_02002c84_a();
void Func_02002c94();
void Func_02002c94_a();
void Func_02002ca4();
s32 Func_02002cb4();
void Func_02002cc6();
void Func_02002cf4();
void Func_02002cfe();
s32 Func_02002d0e();
void Func_02002d12();
void Func_02002d22();
void Func_02002d38();
s32 Func_02002d4a();
void Func_02002d54();
s32 Func_02002d64();
void Func_02002d6a();
void Func_02002d70();
void Func_02002d74();
void Func_02002d7c();
void Func_02002d7e();
void Func_02002d80();
s32 Func_02002d84();
s32 Func_02002d84_a();
void Func_02002d88();
void Func_02002d96();
void Func_02002d98();
s32 Func_02002da6();
void Func_02002ddc();
void Func_02002de2();
void Func_02002dfc();
s32 Func_02002dfe();
void Func_02002e08();
void Func_02002e0e();
void Func_02002e1c();
void Func_02002e26();
void Func_02002e34();
void Func_02002e42();
void Func_02002e5e();
void Func_02002e84();
void Func_02002e98();
void Func_02002ea4();
s32 Func_02002eb4();
void Func_02002eb6();
void Func_02002ed4();
void Func_02002ee2();
void Func_02002eee();
void Func_02002efa();
void Func_02002f00();
void Func_02002f0c();
void Func_02002f8a();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_020012e0(s32 a0, s32 a1)
{
    u32 i;
    s32 record;
    s32 value;
    s32 base6_0;
    s32 base2_2000240;
    s32 v5;
    s32 v6;
    s32 base5_2000240;
    s32 base3_2000240;
    s32 v1;
    s32 base5_2008149;
    u8 *p5;
    u8 *p5b;

    *(s32 *)0x02009f50 = 0;
    *(s32 *)0x02009f4c = 0;
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x201;
    Func_02002a5c(2);
    Func_020025a2(Data_02000240_t[172][0]);
    Call6(Func_02002ac6, 11, 11, 1, 1, 13, 10);
    Func_02001450(4);
    Func_02002a42(1);
    Func_02002ad0(5);
    *(u16 *)0x0200222c = (s32)Data_00000054;
    *(u16 *)0x0200222e = (s32)Data_00000041;
    *(u16 *)0x02002230 = (s32)Data_0000004c;
    *(u16 *)0x02002232 = (s32)Data_0000004b;
    base6_0 = 0;
    do {
        Func_02002b70((base6_0 + 0x2f0));
        if (Value1(Func_0200228a, base6_0) != 0) {
            Func_02002b78((base6_0 + 0x2f0));
        }
        base6_0 = (base6_0 + 1);
    } while (base6_0 <= 7);
    if (*(s16 *)(0x2000402) != 8) {
        v6 = 0x2000240;
    } else {
        Func_02002bea();
        Func_02002c66();
        Func_02002c72();
        Func_020014dc(5);
        Data_02000240_t[169][0] += 1;
        base2_2000240 = (s32)Data_02000240;
        *(u16 *)((base2_2000240 + 0x2aa)) += 1;
        value = Value1(Func_02002bf6, 0x3f8);
        v5 = ((((value << 24) >> 24) << 1) + 2);
        if (((((value << 24) >> 24) << 1) + 2) > 14) {
            v5 = 14;
        }
        record = Value1(Func_02002c0c, 0x3e8);
        if (record == 2) {
            Call2(Func_02002c22, 0x3e8, 0);
            v5 = (v5 + 1);
            v6 = (v6 + 1);
        } else {
            Call2(Func_02002c32, 0x3e8, (record + 1));
            v6 = ((value << 24) >> 24);
        }
        Func_02002c94(8, *(s32 *)0x02000434, 0);
        Func_02002ca4((v5 + 0x293e));
        Value2(Func_02002cb4, 8, 0);
        if (Value2(Func_02002c84, 0, 0) == 0) {
            if (v6 > 90) {
                v6 = 90;
            }
            Call2(Func_02002c70, 0x3f8, v6);
            goto L_02001530;
        }
        Call1(Func_02002c68, 0x173);
        Call2(Func_02002c84_a, 0x3f8, -1);
        base5_2000240 = (s32)Data_02000240;
        Func_02002c50(*(u16 *)((base5_2000240 + 0x2aa)), 5);
        p5 = *(u16 *)((base5_2000240 + 0x2aa));
        if ((u32)Data_02000240_t[170][0] < (s32)p5) {
            Data_02000240_t[170][0] = (s32)p5;
            Call1(Func_02002cfe, 0x293c);
            Value2(Func_02002d0e, 8, 0);
            Func_020026ae();
        } else {
            Call1(Func_02002d12, 0x2939);
            Func_02002d22(8, 0);
        }
        Func_020015c4(0);
        goto L_02001530;
    }
    if (*(s16 *)(0x2000402) == 9) {
        *(u16 *)((v6 + 0x2a6)) += 1;
        Func_02002cf4();
        Func_02002d70();
        Func_02002d7c();
        Func_020015e6(5);
        Value3(Func_02002d4a, 8, *(s32 *)((v6 + 0x1f4)), 0);
        Func_02002cc6(*(u16 *)((v6 + 0x2aa)), 5);
        p5 = *(u16 *)((v6 + 0x2aa));
        if ((u32)*(u16 *)((v6 + 0x2a8)) < (s32)p5) {
            *(u16 *)((v6 + 0x2a8)) = (s32)p5;
            Call1(Func_02002d74, 0x293c);
            Value2(Func_02002d84, 8, 0);
            Func_02002724();
        } else {
            Call1(Func_02002d88, 0x293a);
            Func_02002d98(8, 0);
        }
        base3_2000240 = (s32)Data_02000240;
        {
            s32 shown = 0;
        
            *(u16 *)((base3_2000240 + 0x2aa)) = shown;
        }
        Call1(Func_02002d38, 0x173);
        Call2(Func_02002d54, 0x3f8, -1);
        L_0200152c:;
        Func_02001656(0);
        L_02001530:;
        Func_02002d7e();
    } else {
        if (*(s16 *)(0x2000402) == 10) {
            Func_02002d80();
            Func_02002dfc();
            Func_02002e08();
            Func_02001672(0);
            Func_02001678(4);
            if (Value1(Func_02002d64, 0x3e8) != 0) {
                p5b = *(s32 *)0x03001ebc;
                Call1(Func_02002d84_a, 0x3e8);
                {
                    s32 shown = 2;
                
                    *(u16 *)(((s32)p5b + 0x182)) = shown;
                }
                Call1(Func_02002d96, 0x304);
                Func_02002c94_a(20);
                Func_020015c0();
                Func_020016b2(0);
                goto L_0200152c;
            }
            *(u16 *)((v6 + 0x2ac)) += 1;
            v1 = (*(u16 *)((v6 + 0x2b2)) + 1);
            *(u16 *)((v6 + 0x2b2)) += 1;
            if ((u32)*(u16 *)((v6 + 0x2b0)) < ((u32)(v1 << 16) >> 16)) {
                *(u16 *)((v6 + 0x2b0)) = v1;
            }
            Func_02002850(*(u16 *)((v6 + 0x2b0)));
            Func_0200277c();
            Call1(Func_02002ddc, 0x304);
            Call1(Func_02002de2, 0x305);
            goto L_02001530;
        }
        if (*(s16 *)(0x2000402) == 11) {
            Func_02002e1c();
            Func_02002e98();
            Func_02002ea4();
            Func_0200170e(0);
            Func_02001714(4);
            record = Value1(Func_02002dfe, 0x173);
            if (record == 0) {
                *(u16 *)((v6 + 0x2ae)) += 1;
                *(u16 *)((v6 + 0x2b2)) = record;
                Func_020027c6();
            }
            Call1(Func_02002e26, 0x304);
            Call1(Func_02002e34, 0x305);
            goto L_02001530;
        }
        Func_02002d6a();
        Call1(Func_02002e42, 0x172);
        Call2(Func_02002e5e, 0x3f8, -1);
        if (*(u8 *)((v6 + 0x22a)) != 0) {
            Func_02002e84();
            Func_02002f00();
            Func_02002f0c();
            Func_02002ed4(8, *(s32 *)((v6 + 0x1f4)), 0);
            Call1(Func_02002ee2, 0x2929);
            Func_02002efa(8, 0);
            Func_02002eb6();
        }
        *(u8 *)((v6 + 0x22a)) = 0;
        *(u8 *)0x03001d08 = 0;
        Func_020017a0(0);
        Func_020017a6(4);
    }
    base5_2008149 = 0x2008149;
    Value2(Func_02002da6, base5_2008149, 0xc80);
    Func_02002e0e(base5_2008149, 1);
    if (*(s16 *)(0x2000402) == 8) {
        record = Value1(Func_02002eb4, 0x173);
        if (record != 0) {
            goto L_020016b8;
        }
    }
    Func_02002eee(1);
    Func_02002f8a();
    L_020016b8:;
    return 0;
}

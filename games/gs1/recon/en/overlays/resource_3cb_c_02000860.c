#include "types.h"

#define Scene_RunScene3cbSequenceB Func_02000860

extern u8 Data_00000043[];
extern u8 Data_00000045[];
extern u8 Data_00000058[];
extern u8 Data_000000be[];
extern u8 Data_02000240[];
s32 Func_02000970();
s32 Func_020009aa();
s32 Func_020009b4();
s32 Func_020009ce();
void Func_020009e8();
void Func_02000a06();
void Func_02000b84();
void Func_02000b8a();
s32 Func_020011cc();
void Func_02002012();
void Func_0200206c();
s32 Func_02002086();
void Func_02002094();
s32 Func_02002096();
s32 Func_02002098();
s32 Func_02002098_a();
s32 Func_020020a4();
s32 Func_020020b8();
void Func_020020c2();
void Func_020020c6();
void Func_020020ca();
void Func_020020d0();
void Func_020020d6();
void Func_020020e8();
void Func_020020ec();
void Func_02002106();
void Func_0200210e();
s32 Func_02002112();
void Func_02002116();
s32 Func_0200211e();
void Func_02002122();
void Func_02002158();
s32 Func_02002188();
void Func_0200218a();
s32 Func_02002198();
void Func_020021d2();
void Func_020021e6();
void Func_020021f0();
void Func_0200221e();
void Func_02002228();
void Func_0200222c();
void Func_0200223a();
void Func_0200223c();
void Func_02002248();
void Func_0200224a();
void Func_02002258();
void Func_02002258_a();
void Func_0200226a();
void Func_02002272();
void Func_02002278();
void Func_02002284();
void Func_0200228e();
void Func_02002292();
void Func_02002296();
void Func_0200229c();
void Func_020022a2();
void Func_020022a4();
void Func_020022a8();
void Func_020022b0();
void Func_020022c4();
s32 Func_020022c6();
void Func_02002312();
void Func_02002328();
s32 Func_02002374();
void Func_02002384();
s32 Func_020023aa();
s32 Func_020023c0();
void Func_020023d0();
void Func_020023d2();

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

s32 Func_02000860(void)
{
    u32 i;
    s32 p10;
    s32 rec;
    s32 rec8;
    s32 record;
    s32 none;
    s32 base6_0;
    s32 v5;
    s32 v6;
    s32 v8;
    s32 base5_2008149;
    s32 base3_2000240;
    u8 *v0;
    s32 v1;
    u8 *v2;

    p10 = *(s32 *)0x03001ebc;
                base6_0 = 0;
            base6_0 = 0;
    Value1(Func_02002086, 0x173);
    none = 0;
        v8 = none;
    if (none != 0) {
        Func_020020c6();
    } else {
        if (Value1(Func_02002098, 0x200) == 0) {
            return MISSING;
        }
        if (Value1(Func_020020a4, 0x205) != 0) {
            return MISSING;
            L_020008a0:;
            {
                s32 shown = 2;
            
                *(u16 *)((0x182 + p10)) = shown;
            }
            Call1(Func_020020c2, 0x205);
            Call1(Func_020020d0, 0x201);
            Call1(Func_020020d6, 0x202);
            v8 = 1;
            Func_020009e8(4);
            Call1(Func_020020e8, 0x200);
        } else {
            Func_02002116();
            Call1(Func_020020ec, 0x203);
            v6 = 0;
            Func_02000a06(2);
            if (Value1(Func_02000970, 2) == 0) {
                rec = Value4(Func_020020b8, 0x2928, 5, 4, 1);
                for (;;) {
                    if (!(Value1(Func_020009ce, 2) == 0)) break;
                    Func_02002012(1);
                    v5 = 0;
                    if (Value1(Func_02002112, 0x201) == 0) {
                        v5 = 1;
                    }
                    if (Value1(Func_0200211e, 0x205) != 0) {
                        v5 = 1;
                    }
                    if (Value1(Func_020009aa, 2) == 0) {
                        record = Value1(Func_020009b4, 1);
                        if (record != 0) {
                            goto L_02000938;
                        }
                        base6_0 = (base6_0 + 1);
                        if (base6_0 <= 25) {
                            goto L_0200093a;
                        }
                        v5 = 1;
                    } else {
                        L_02000938:;
                        base6_0 = 0;
                    }
                    L_0200093a:;
                    if (v5 != 0) {
                        goto L_020008a0;
                    }
                }
                v6 = base6_0;
            }
            v8 = none;
        }
        if (0 != 0) {
            Func_02002106(0, 1);
        }
        Func_0200206c(5);
    }
    if (v8 != 0) {
    } else {
        rec8 = Value2(Func_02002096, 54, 0x7c8);
        base5_2008149 = 0x2008149;
        base6_0 = rec8;
        ((void (*)())Func_02002098_a)(base5_2008149);
        Func_0200210e(5);
        Func_02002094(8);
        Func_02002122(5);
        if (Value1(Func_02002198, 0x173) != 0) {
            Func_0200221e(8, *(s32 *)0x02000434, 0);
            Call1(Func_0200222c, 0x293b);
            Func_0200223c(8, 0);
            Func_020020ca(45);
            Call3(Func_02002228, 0, 0x10000, 0x8000);
            Func_0200223a(0, 216, 184);
            Func_02002248(0);
            Func_0200224a(0, 216, 168);
            Func_02002258(0);
        } else {
            Call3(Func_02002258_a, 0, 0x10000, 0x8000);
            Func_0200226a(0, 216, 200);
            Func_02002278(0);
            Call3(Func_02002272, 0, 0x1999, 0xccc);
            Func_02002284(0, 216, 168);
            if (Value0(Func_020011cc) < 0) {
                Call3(Func_02002292, 0, 0x10000, 0x8000);
                Func_020022a4(0, 216, 200);
                Func_020021d2(5);
                Func_02002158(8);
                Func_020021e6(5);
                Func_020022c4(0);
                Func_0200218a(54);
                Func_02000b84(0);
                Func_02000b8a(4);
                Value2(Func_02002188, base5_2008149, 0xc80);
                Func_020021f0(base5_2008149, 1);
                Call1(Func_02002296, 0x201);
                Call1(Func_0200229c, 0x202);
                Call1(Func_020022a2, 0x303);
                Call1(Func_020022a8, 0x203);
                Call1(Func_020022b0, 0x200);
                {
                    s32 shown = 2;
                
                    *(u16 *)((0x182 + p10)) = shown;
                }
                v5 = base5_2008149;
                goto L_02000b5c;
            }
            Call3(Func_02002312, 0, 0x8000, 0x4000);
            Func_02002328(0);
        }
        if (Value1(Func_020022c6, 0x173) != 0) {
            Value2(Func_02002374, (s32)Data_000000be, 8);
            Func_02002384((s32)Data_000000be, 9);
        } else {
            Value2(Func_020023c0, (s32)Data_000000be, 10);
            Func_020023d0((s32)Data_000000be, 11);
        }
        base3_2000240 = (s32)Data_02000240;
        *(u8 *)((base3_2000240 + 0x22b)) = 4;
        Func_020023d2(1, 1);
        *(u16 *)0x02002226 = (s32)Data_00000058;
        *(u16 *)0x02002224 = (s32)Data_00000045;
        *(u16 *)0x02002228 = (s32)Data_00000045;
        *(u16 *)0x0200222a = (s32)Data_00000043;
        v0 = base6_0;
        v1 = 0;
        v2 = 0x2018000;
        do {
            v1 = (v1 + 1);
            v2[0] = v0[0];
            v0 = ((s32)v0 + 1);
            v2 = ((s32)v2 + 1);
        } while ((u32)v1 <= 0x7c7);
        Func_0200228e(54, v1, (s32)v2);
        v5 = (s32)Data_000000be;
    }
    L_02000b5c:;
    record = Func_020023aa();
    L_02000b88:;
    return record;
}

#include "types.h"

#define FieldScene_RunMiddleSequence Func_02002bac

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
void Func_02007464();
s32 Func_020075b4();
s32 Func_020075c0();
s32 Func_020075de();
void Func_02007640();
s32 Func_0200764c();
void Func_02007654();
void Func_0200767e();
void Func_02007690();
void Func_020076dc();
s32 Func_020076ec();
void Func_02007718();
void Func_02007726();
s32 Func_0200772a();
void Func_02007734();
void Func_02007772();
void Func_0200778a();
void Func_02007788();
void Func_02007782();
void Func_0200777c();
s32 Func_0200778c();
void Func_020077a2();
void Func_020077a0();
void Func_020077b2();
s32 Func_020077b4();
void Func_020077bc();
void Func_020077d0();
void Func_020077ce();
void Func_020077e6();
s32 Func_020077e2();
s32 Func_02007816();
void Func_020077f8();
void Func_02007802();
void Func_02007862();
void Func_020078fc();

void Func_020077ae();
void Func_020077be();
void Func_020077ee();
void Func_020077f6();
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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    s32 rec;
    s32 record;
    s32 p9;
    s32 p11;
    s32 count;
    s32 state;
    s32 obj;
    s32 hi;
    s32 lo;
    s32 tail;
    s32 sx;
    s32 sy;
    s32 i;
    u8 buf[8];

    rec = Value1(Func_020075de, owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Value0(Func_020075b4);
        for (i = 0; i < count; i++) {
            buf[i] = Data_02000240[504 + i];
        }
        if (count <= 1) {
            Func_02007772(0x2083);
            Func_0200778a(owner, 0);
            return;
        }
        if (Value1(Func_020075c0, base + 512) != 0) {
            Func_02007772(0x2084);
            Func_0200778a(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Func_02007464(6);
        } else {
            Call1(Func_020076dc, 0x207d);
            Value2(Func_020076ec, owner, 0);
            state = Value2(Func_0200764c, 0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Func_02007640((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Func_02007654((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Func_02007816);
            for (i = 0; i < count; i++) {
                Func_0200767e((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Func_02007690((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Func_02007772(0x207e);
    Func_0200778a(owner, 0);
    return;
L_main:
    ((void (*)())Func_0200764c)(obj, 1);
    Call1(Func_0200778a, 0x207f);
    Func_020077a2(owner, 0);
    Call3(Func_02007718, 0, 0x10000, 0x8000);
    Call3(Func_02007726, obj, 0x10000, 0x8000);
    Call3(Func_02007734, owner, 0x10000, 0x8000);
    record = Value1(Func_0200772a, 0);
    if (record != 0) {
        Func_02007788(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    Func_02007782(obj, p9, hi);
    lo = p9 + 16;
    Value3(Func_0200778c, 0, lo, hi);
    Func_020077e6(obj, 0, 30);
    Func_020077be(obj, 3);
    tail = hi - 32;
    Func_020077d0(0, 3);
    Func_020077b2(owner, p9, tail);
    Value3(Func_020077b4, owner, lo, tail);
    Func_020078fc(0, obj);
    Func_020077ce(obj, p9, tail);
    Func_020077f6(owner, 1);
    Call3(Func_02007862, owner, 0x8000, 0);
    Func_020077ee(obj, p9, p11 - 48);
    Func_020077f8(owner, p9, tail);
    Func_02007802(owner, p9, p11);
    Func_020077a0(obj);
    Func_0200777c(base + 512);
    rec = Value1(Func_020077e2, obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    Func_020077ae((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    Func_020077bc((obj << 4) + 888, sy);
}

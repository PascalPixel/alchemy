#include "types.h"

#define FieldScene_RunMiddleSequence Func_02002114

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
void Func_02005f2c();
s32 Func_02006054();
s32 Func_02006060();
s32 Func_0200607e();
void Func_020060e0();
s32 Func_020060ec();
void Func_020060f4();
void Func_0200611e();
void Func_02006130();
void Func_02006164();
s32 Func_02006174();
void Func_020061b8();
void Func_020061c6();
s32 Func_020061ca();
void Func_020061d4();
void Func_020061fa();
void Func_02006212();
void Func_02006218();
void Func_0200621a();
void Func_0200621c();
s32 Func_02006224();
void Func_0200622a();
void Func_02006240();
void Func_0200624a();
s32 Func_0200624c();
void Func_0200624e();
void Func_0200625c();
void Func_02006260();
void Func_02006266();
void Func_0200626e();
s32 Func_02006282();
void Func_02006286();
s32 Func_0200628e();
void Func_02006290();
void Func_0200629a();
void Func_020062ea();
void Func_02006374();

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

    rec = Value1(Func_0200607e, owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Value0(Func_02006054);
        for (i = 0; i < count; i++) {
            buf[i] = Data_02000240[504 + i];
        }
        if (count <= 1) {
            Func_020061fa(0x2083);
            Func_02006212(owner, 0);
            return;
        }
        if (Value1(Func_02006060, base + 512) != 0) {
            Func_020061fa(0x2084);
            Func_02006212(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Func_02005f2c(6);
        } else {
            Call1(Func_02006164, 0x207d);
            Value2(Func_02006174, owner, 0);
            state = Value2(Func_020060ec, 0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Func_020060e0((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Func_020060f4((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Func_0200628e);
            for (i = 0; i < count; i++) {
                Func_0200611e((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Func_02006130((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Func_020061fa(0x207e);
    Func_02006212(owner, 0);
    return;
L_main:
    ((void (*)())Func_020060ec)(obj, 1);
    Call1(Func_02006212, 0x207f);
    Func_0200622a(owner, 0);
    Call3(Func_020061b8, 0, 0x10000, 0x8000);
    Call3(Func_020061c6, obj, 0x10000, 0x8000);
    Call3(Func_020061d4, owner, 0x10000, 0x8000);
    record = Value1(Func_020061ca, 0);
    if (record != 0) {
        Func_02006218(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    Func_0200621a(obj, p9, hi);
    lo = p9 + 16;
    Value3(Func_02006224, 0, lo, hi);
    Func_0200626e(obj, 0, 30);
    Func_0200624e(obj, 3);
    tail = hi - 32;
    Func_02006260(0, 3);
    Func_0200624a(owner, p9, tail);
    Value3(Func_0200624c, owner, lo, tail);
    Func_02006374(0, obj);
    Func_02006266(obj, p9, tail);
    Func_02006286(owner, 1);
    Call3(Func_020062ea, owner, 0x8000, 0);
    Func_02006286(obj, p9, p11 - 48);
    Func_02006290(owner, p9, tail);
    Func_0200629a(owner, p9, p11);
    Func_02006240(obj);
    Func_0200621c(base + 512);
    rec = Value1(Func_02006282, obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    Func_0200624e((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    Func_0200625c((obj << 4) + 888, sy);
}

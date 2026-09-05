#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
void Func_020059fc();
s32 Func_02005b24();
s32 Func_02005b38();
s32 Func_02005b4e();
void Func_02005bb0();
void Func_02005bc4();
void Func_02005bee();
void Func_02005c00();
void Func_02005c2c();
s32 Func_02005c3c();
void Func_02005c88();
void Func_02005c96();
s32 Func_02005c9a();
void Func_02005ca4();
void Func_02005cc2();
void Func_02005cda();
void Func_02005ce8();
void Func_02005cea();
void Func_02005cf2();
void Func_02005d10();
void Func_02005d1a();
s32 Func_02005d1c();
void Func_02005d1e();
void Func_02005d2c();
void Func_02005d30();
void Func_02005d36();
s32 Func_02005d52();
void Func_02005d56();
void Func_02005d60();
void Func_02005d6a();
void Func_02005db2();
void Func_02005e2c();

s32 Func_02005bbc();
s32 Func_02005cf4();

s32 Func_02005d3e();

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

    rec = Value1(Func_02005b4e, owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Value0(Func_02005b24);
        for (i = 0; i < count; i++) {
            buf[i] = Data_02000240[504 + i];
        }
        if (count <= 1) {
            Func_02005cc2(0x2083);
            Func_02005cda(owner, 0);
            return;
        }
        if (Value1(Func_02005b38, base + 512) != 0) {
            Func_02005cc2(0x2084);
            Func_02005cda(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Func_020059fc(6);
        } else {
            Call1(Func_02005c2c, 0x207d);
            Value2(Func_02005c3c, owner, 0);
            state = Value2(Func_02005bbc, 0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Func_02005bb0((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Func_02005bc4((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Func_02005d3e);
            for (i = 0; i < count; i++) {
                Func_02005bee((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Func_02005c00((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Func_02005cc2(0x207e);
    Func_02005cda(owner, 0);
    return;
L_main:
    ((void (*)())Func_02005bc4)(obj, 1);
    Call1(Func_02005cda, 0x207f);
    Func_02005cf2(owner, 0);
    Call3(Func_02005c88, 0, 0x10000, 0x8000);
    Call3(Func_02005c96, obj, 0x10000, 0x8000);
    Call3(Func_02005ca4, owner, 0x10000, 0x8000);
    record = Value1(Func_02005c9a, 0);
    if (record != 0) {
        Func_02005ce8(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    Func_02005cea(obj, p9, hi);
    lo = p9 + 16;
    Value3(Func_02005cf4, 0, lo, hi);
    ((void (*)())Func_02005d3e)(obj, 0, 30);
    Func_02005d1e(obj, 3);
    tail = hi - 32;
    Func_02005d30(0, 3);
    Func_02005d1a(owner, p9, tail);
    Value3(Func_02005d1c, owner, lo, tail);
    Func_02005e2c(0, obj);
    Func_02005d36(obj, p9, tail);
    Func_02005d56(owner, 1);
    Call3(Func_02005db2, owner, 0x8000, 0);
    Func_02005d56(obj, p9, p11 - 48);
    Func_02005d60(owner, p9, tail);
    Func_02005d6a(owner, p9, p11);
    Func_02005d10(obj);
    ((void (*)())Func_02005cf4)(base + 512);
    rec = Value1(Func_02005d52, obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    Func_02005d1e((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    Func_02005d2c((obj << 4) + 888, sy);
}

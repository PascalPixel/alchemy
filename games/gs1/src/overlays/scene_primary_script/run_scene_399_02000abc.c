#include "types.h"

#define FieldScene_RunScene399_02000abc Func_02000abc

s32 Func_02002cea();
void Func_02002d04();
s32 Func_02002d0a();
s32 Func_02002d18();
s32 Func_02002d26();
void Func_02002d2e();
void Func_02002d32();
void Func_02002d80();
void Func_02002de4();
void Func_02002df4();
void Func_02002e20();
void Func_02002e30();
s32 Func_02002e50();
void Func_02002e64();
void Func_02002ed4();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void FieldScene_RunScene399_02000abc(void)
{
    s32 record;

    record = Func_02002d0a(0);
    if ((u32)((*(volatile u16 *)(record + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Func_02002d04();
        if (Value1(Func_02002cea, 0x82d) == 0) {
            Call1(Func_02002de4, 0x1553);
            Func_02002df4(19, 0);
            Call1(Func_02002d0a, 0x82d);
        }
        Func_02002d2e();
        Func_02002ed4(19);
    } else {
        Func_02002d32();
        if (Value1(Func_02002d18, 0x881) != 0) {
            Call1(Func_02002e20, 0x1671);
            Func_02002e30(19, 0);
        } else {
            record = Value1(Func_02002d26, 3);
            if (record != 0) {
                Call1(Func_02002e20, 0x1572);
                Func_02002e30(19, 0);
            } else {
                Call1(Func_02002e30, 0x1554);
                (void)Value2(Func_02002e50, 19, 0);
                Call3(Func_02002e64, 19, 0x3000, 10);
            }
        }
        Func_02002d80();
    }
}

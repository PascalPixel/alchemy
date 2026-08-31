#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02000a48

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 32 calls, 0 loop(s), and 2 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02004cfe();
s32 Func_02004d1e();
s32 Func_02004d38();
void *Func_02004d98();
void Func_02004da6();
void Func_02004dc0();
s32 Func_02004dc6();
void Func_02004dca();
s32 Func_02004de0();
void Func_02004e08();
void Func_02004e0c();
void Func_02004e20();
void Func_02004e2a();
void *Func_02004e40();
void Func_02004e42();
void Func_02004e46();
void Func_02004e68();
void Func_02004e6c();
void Func_02004e72();
void Func_02004e84();
void Func_02004e9e();
void Func_02004eb0();
void Func_02004eb4();
void Func_02004ec8();
void Func_02004ed2();
void Func_02004eea();
void Func_02004eee();
void Func_02004f14();
void Func_02004f2c();
void Func_02004f46();

void Func_02000a48(void)
{
    void *p25;
    void *p9;

    Func_02004d38();
    if (Func_02004d1e(2341) != 0) {
        Func_02004e08(7688);
        Func_02004e20(21, 0);
    } else {
        if (Func_02004d38(2338) != 0) {
            Func_02004e0c(21, 2);
            Func_02004e2a(7535);
            Func_02004e42(21, 0);
            p9 = Func_02004d98(21);
            Func_02004cfe();
            *(u16 *)((u8 *)(p9) + 100) = 150;
            Func_02004dca(21, 33604824);
        } else {
            Func_02004e9e(21, 259, 0);
            Func_02004e46(21, 3);
            Func_02004e6c(7478);
            Func_02004e84(21, 0);
        }
    }
    Func_02004dc0();
    Func_02004de0();
    if (Func_02004dc6(2341) != 0) {
        Func_02004eb0(7689);
        Func_02004ec8(24, 0);
    } else {
        if (Func_02004de0(2338) != 0) {
            Func_02004eb4(24, 2);
            Func_02004ed2(7536);
            Func_02004eea(24, 0);
            p25 = Func_02004e40(24);
            Func_02004da6();
            *(u16 *)((u8 *)(p25) + 100) = 150;
            Func_02004e72(24, 33604824);
        } else {
            Func_02004f46(24, 259, 0);
            Func_02004eee(24, 3);
            Func_02004f14();
            Func_02004f2c(24, 0);
        }
    }
    Func_02004e68();
}

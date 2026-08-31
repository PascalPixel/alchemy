#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02004260

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 14 calls, 0 loop(s), and 10 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void *Func_02008b38();
void Func_02008b50();
void Func_02008b5e();
void Func_02008b8a();
void Func_02008b8c();
void Func_02008bd6();
void Func_02008bd8();
void Func_02008c06();
void Func_02008c48();
void Func_02008c80();
void Func_02008d1e();
void Func_02008df0();
void Func_02008e00();

void Func_02004260(void)
{
    void *p1;

    p1 = Func_02008b38(22);
    Func_02008bd8(224);
    Func_02008bd8();
    Func_02008b50();
    *(u8 *)((u8 *)(*(void **)(p1 + 80)) + 38) = 0;
    *(u8 *)((u8 *)((u8 *)(*(void **)(p1 + 80)) + 38) + 1) = 0;
    *(u8 *)((*(void **)(p1 + 80)) + 5) &= 0xffffffffffffffdf;
    *(u8 *)((*(void **)(p1 + 80)) + 9) &= 0xf;
    *(s32 *)(p1 + 40) = 163840;
    *(s32 *)(p1 + 72) = 16384;
    Func_02008b5e(17, 1544);
    Func_02008c06();
    Func_02008b8c();
    Func_02008b8a(17);
    Func_02008e00(83);
    Func_02008df0();
    Func_02008c80();
    Func_02008c48();
    Func_02008bd6();
    Func_02008d1e(0, 1);
}

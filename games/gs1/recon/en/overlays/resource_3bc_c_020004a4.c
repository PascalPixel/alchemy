#include "types.h"

#define FieldScene_RunFinalAuxiliarySequence Func_020004a4

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 11 calls, 0 loop(s), and 16 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02004e0e();
void Func_02004e2c();
void Func_02004e2e();
void Func_02004e46();
void Func_02004e90();
void Func_02004ea2();
void Func_02004ecc();
void *Func_02004ee4();
void Func_02004ef6();
void *Func_02004f1c();
void *Func_02004f3c();

void Func_020004a4(void)
{
    void *p2;
    void *p5;
    void *p7;

    Func_02004ecc(12);
    p2 = Func_02004ee4(12);
    Func_02004e2c();
    *(u8 *)((u8 *)(p2) + 35) = 2;
    *(u8 *)((u8 *)((u8 *)(p2) + 35) + 50) = 0;
    *(s32 *)(p2 + 52) = 26214;
    *(s32 *)(p2 + 48) = 0;
    Func_02004e0e();
    p5 = Func_02004f1c(11);
    *(u8 *)((u8 *)(p5) + 35) = 2;
    *(s32 *)(p5 + 52) = 26214;
    *(s32 *)(p5 + 48) = 0;
    Func_02004e2e();
    p7 = Func_02004f3c(10);
    *(s32 *)(p7 + 52) = 26214;
    *(s32 *)(p7 + 48) = 0;
    Func_02004e46();
    Func_02004ef6(872);
    Func_02004e90(15, 12, 1, 1, 13);
    Func_02004ea2(1, 25, 1, 1);
}

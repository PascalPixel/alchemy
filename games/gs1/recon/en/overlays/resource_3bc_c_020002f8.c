#include "types.h"

#define FieldScene_RunClosingAuxiliarySequence Func_020002f8

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 16 calls, 0 loop(s), and 16 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02004c06();
void Func_02004c6a();
void Func_02004c8a();
void Func_02004cae();
void Func_02004cc2();
void Func_02004cfe();
void Func_02004d00();
void Func_02004d26();
s32 Func_02004d36();
void *Func_02004d54();
void Func_02004d70();
void *Func_02004d78();
void Func_02004d96();
void *Func_02004d98();
void Func_02004e0c();

void Func_020002f8(void)
{
    void *p5;
    void *p7;
    void *p9;

    Func_02004cc2(866);
    if (Func_02004d36(10) != 0) {
        Func_02004d70();
    }
    Func_02004d96();
    p5 = Func_02004d54(11);
    *(u8 *)((u8 *)(p5) + 85) = 0;
    *(s32 *)(p5 + 52) = 26214;
    *(s32 *)(p5 + 48) = 52428;
    Func_02004c6a();
    p7 = Func_02004d78(10);
    *(u8 *)((u8 *)(p7) + 85) = 0;
    *(s32 *)(p7 + 52) = 26214;
    *(s32 *)(p7 + 48) = 52428;
    Func_02004c8a();
    p9 = Func_02004d98();
    *(s32 *)(p9 + 48) = 52428;
    *(s32 *)(p9 + 52) = 26214;
    Func_02004cae();
    Func_02004cfe();
    Func_02004e0c();
    Func_02004d00(0, 24, 1, 1, 9, 12);
    Func_02004c06(2);
    Func_02004d26();
    *(s32 *)(p9 + 20) = *(s32 *)(p9 + 12);
    Func_02004d96();
}

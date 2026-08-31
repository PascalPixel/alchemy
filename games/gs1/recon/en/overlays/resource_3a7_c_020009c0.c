#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_020009c0

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 24 calls, 2 loop(s), and 21 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void *Func_020009de();
void Func_02000a06();
void Func_02000a3c();
s32 Func_0200138a();
s32 Func_020013ba();
void Func_020015b0();
void *Func_02001af6();
void *Func_02001b22();
void Func_02001b36();
void *Func_02001b40();
void Func_02001b48();
s32 Func_02001b58();
void Func_02001b6a();
void Func_02001b8a();
void Func_02001b92();
void Func_02001bb8();
void Func_02001bc0();
void Func_02001bd0();
void *Func_02001bd6();
s32 Func_02001bf6();
void *Func_02001c06();
void *Func_02001c1a();
void Func_02001c66();
void Func_02001c82();

void Func_020009c0(void)
{
    void *p1;
    void *p17;
    void *p2;
    void *p20;
    void *p21;
    void *p3;
    void *p5;
    s32 i1;
    s32 i2;

    p1 = Func_02001af6(10);
    p2 = Func_020009de();
    p3 = Func_02001b22();
    *(s32 *)(p3 + 72) = 2621;
    Func_02000a06();
    for (i1 = 0; i1 <= 3; i1++) {
        p5 = Func_02001b40(0);
        Func_02000a3c();
        Func_020015b0();
        *(s32 *)(p5 + 12) = 16711680;
        *(s32 *)(p5 + 72) = 0;
        *(s32 *)(p5 + 40) = 0;
        *(u8 *)((u8 *)(p5) + 91) = 0;
        Func_02001b8a(106);
    }
    if (Func_0200138a(10) != 0) {
        Func_02001b92(10, 1);
        if (Func_02001b58(519) == 0) {
            Func_02001b6a(519);
            Func_02001bb8(204);
        } else {
            Func_02001bc0(106);
        }
    }
    if (Func_020013ba(9) != 0) {
        Func_02001bd0(106);
    }
    p17 = Func_02001bd6(10);
    Func_02001b48();
    Func_02001b36();
    *(s32 *)p17 = 0;
    p20 = Func_02001c06();
    *(s32 *)(p20 + 72) = 2621;
    for (i2 = 0; i2 <= 5; i2++) {
        p21 = Func_02001c1a(0);
        if (Func_02001bf6(512) != 0) {
            *(s32 *)(p21 + 12) = 16711680;
            *(s32 *)(p21 + 72) = 0;
            *(s32 *)(p21 + 40) = 0;
            Func_02001c66(106);
        } else {
            *(s32 *)(p21 + 72) = 0;
            *(s32 *)(p21 + 40) = 0;
            *(s32 *)(p21 + 12) = 16711680;
            Func_02001c82(106);
        }
    }
}

#include "types.h"

#define FieldScene_RunStatefulSequence Func_0200109c

/* Audited retained field-scene body at 0x0200109c.
 * The complete owner preserves 46 decoded calls, 8 loop(s), and 36 explicit memory operation(s)
 * through its interworking return and literal tail. Approved GCC changes
 * scheduling, allocation, control-flow lowering, and literal placement. */

void Func_02001198();
void Func_02001230();
void Func_02002090();
void Func_0200227e();
void Func_02004cf2();
void Func_02004d5e();
void *Func_02004d64();
void Func_02004d84();
void Func_02004da2();
void *Func_02004da4();
void Func_02004dc6();
void Func_02004df4();
void Func_02004e1e();
void Func_02004e20();
void *Func_02004e36();
void *Func_02004e54();
void Func_02004e72();
void Func_02004e86();
void *Func_02004ea0();
void Func_02004eb0();
void Func_02004ebe();
void *Func_02004eca();
void Func_02004ece();
void Func_02004eec();
void Func_02004f52();
void Func_02004f78();
void Func_02004f8e();
void Func_02004f90();
void Func_02004f9a();
void Func_02004fa2();
void Func_02004fb8();
void Func_02004fbe();
void Func_02004fc8();
void Func_02004fce();
void Func_02004ff6();
void *Func_0200501a();
void Func_02005042();
void Func_0200505e();
void Func_0200507a();
void Func_020050ba();
void Func_020050be();
void Func_02005144();
void Func_020051a2();
void Func_02005228();

void Func_0200109c(void)
{
    s32 base;
    void *p1;
    void *p15;
    void *p2;
    void *p22;
    void *p4;
    void *p40;
    void *p6;
    s32 i1;
    s32 i2;
    s32 i3;
    s32 i4;
    s32 i5;
    s32 i6;
    s32 i7;
    s32 i8;

    p1 = Func_02004d64();
    p2 = Func_02004ea0();
    Func_02004e1e(8667);
    p4 = Func_02004e36(16, 0);
    Func_02004d84();
    for (i1 = 0; i1 <= 4; i1++) {
        p6 = Func_02004da4(0, 0);
        *(s32 *)(p6 + 108) = 0;
        *(s32 *)(p6 + 24) = 65536;
        *(s32 *)(p6 + 28) = 65536;
    }
    Func_02004cf2(65536, 5, 33603436, 33558529);
    *(u16 *)(p6 + 6) = 0;
    Func_02004f9a(110);
    Func_02004fa2(114);
    Func_02004fb8(110);
    Func_02004fc8(110);
    Func_02004d5e(30);
    for (i2 = 0; i2 <= 4; i2++) {
        Func_02002090(0);
        Func_02004ff6(151);
        p15 = Func_02004e54();
        *(s32 *)(p15 + 24) = 0;
        base = 26214;
        *(s32 *)(p15 + 28) = base;
        *(s32 *)(p15 + 24) = base;
        Func_02004da2(1, 0);
        Func_02001198();
    }
    Func_02004dc6(30);
    Func_02005042(114);
    Func_0200505e(114);
    Func_02004df4(30);
    for (i3 = 0; i3 <= 4; i3++) {
        p22 = Func_02004eca(11, 0);
        Func_0200507a(151);
        base = 26214;
        Func_02001230();
        *(s32 *)(p22 + 28) = 0;
        *(s32 *)(p22 + 24) = 0;
        Func_02004e20(1, 0);
        base = 26214;
        Func_02004f52(11, 0, 0, 0);
    }
    Func_020050ba(110);
    Func_02004e54(30);
    Func_02004e86();
    Func_02004e72();
    *(u16 *)(*(void **)(*(void **)(p22 + 24) + 0)) = 0;
    *(u16 *)((*(void **)(*(void **)(p22 + 24) + 0)) + 2) = 0;
    *(u16 *)((*(void **)(*(void **)(p22 + 24) + 0)) + 8) = 512;
    *(u16 *)((*(void **)(*(void **)(p22 + 24) + 0)) + 10) = 12288;
    Func_02004eb0(33591189, 3200);
    for (i4 = 192; i4 != 99; i4++) {
        Func_02004ebe(1);
    }
    Func_02004ece(10);
    Func_02005144(110);
    for (i5 = 99; i5 != 2; i5++) {
        Func_02004eec(1);
    }
    *(u16 *)(*(void **)(*(void **)(p22 + 24) + 0)) = *(s16 *)(*(void **)(*(void **)(p22 + 24) + 0));
    *(u16 *)((*(void **)(*(void **)(p22 + 24) + 0)) + 2) = 0;
    Func_02004f78(131072, 131072, 65536, 2);
    Func_02004fbe(20);
    Func_02004f8e(262144, 262144, 65536);
    *(u16 *)(*(void **)(*(void **)(p22 + 24) + 0)) = 99;
    Func_020051a2(190);
    for (i6 = 192; i6 < 0; i6++) {
        for (i7 = 0; i7 <= 4; i7++) {
            p40 = Func_0200501a(0);
            *(s32 *)(p40 + 24) = *(s32 *)(p40 + 24);
            *(s32 *)(p40 + 28) = *(s32 *)(p40 + 28);
            Func_0200227e();
        }
        Func_02004f90(1);
    }
    for (i8 = 0; i8 <= 4; i8++) {
        Func_020050be(0, 0, 0, 1572864, 0);
    }
    Func_02004e86();
    Func_02005228(80);
    Func_02004fce(33591189);
}

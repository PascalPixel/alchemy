#include "types.h"

#define FieldScene_RunLateSequence Func_02002adc

/* Audited retained field-scene body at 0x02002adc.
 * The complete production body preserves 43 decoded calls, 1 loop(s), and 37 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02004eda();
void Func_02004f06();
void Func_02005466();
void Func_02005492();
void Func_0200690c();
void Func_02006a42();
void Func_02006a56();
void Func_02006a5e();
void Func_02006a62();
void Func_02006a6c();
void *Func_02006a76();
void Func_02006a8a();
void Func_02006aa6();
void Func_02006ab0();
void *Func_02006b14();
void *Func_02006b48();
void *Func_02006b5c();
void Func_02006b64();
void Func_02006b94();
void Func_02006bba();
void Func_02006bc8();
void Func_02006be4();
void Func_02006bec();
void Func_02006bf2();
void Func_02006bf4();
void Func_02006bfe();
void Func_02006c28();
void Func_02006c2a();
void Func_02006c36();
void Func_02006c4e();
void Func_02006c5c();
void Func_02006c62();
void Func_02006c6a();
void Func_02006c80();
void Func_02006cb2();
void *Func_02006cde();
void Func_02006cea();
void Func_02006d00();
void Func_02006d36();
void Func_02006d42();
void Func_02006d62();
void Func_02006db2();

void Func_02002adc(void)
{
    void *p2;
    void *p22;
    void *p3;
    void *p30;
    void *p35;
    s32 i1;

    Func_02006b94(247);
    p2 = Func_02006b48();
    p3 = Func_02006b5c();
    Func_02006a42(30);
    Func_02006bc8(86);
    Func_02004eda(8);
    Func_02005466(3, 1);
    Func_02006a62();
    Func_02006a6c(30);
    Func_02006bf4();
    Func_02004f06(4);
    Func_02005492(3, 0);
    Func_02006a8a();
    Func_02006b64(8, 261, 0);
    Func_0200690c(1);
    Func_02006c28();
    Func_02006c2a(19);
    Func_02006ab0(30);
    Func_02006c36(289);
    Func_02006bf2();
    Func_02006bfe();
    p22 = Func_02006b14();
    Func_02006be4();
    *(u8 *)((u8 *)(p22) + 35) |= 0x1;
    *(u16 *)(p22 + 6) = 16384;
    Func_02006aa6();
    Func_02006a56();
    Func_02006a5e();
    Func_02006bba();
    Func_02006bec(0, 16384, 0);
    *(u8 *)((*(void **)(p22 + 80)) + 5) &= 0xfffffffffffffff3;
    *(u8 *)((*(void **)(p22 + 80)) + 17) &= 0xfffffffffffffff3;
    Func_02006d36(252, 4, 33562768);
    for (i1 = 0; i1 <= 15; i1++) {
        *(s32 *)(p22 + 24) = 4096;
        *(s32 *)(p22 + 28) = 126976;
        p30 = Func_02006a76(1, 33562768);
    }
    *(u16 *)p30 = 67109384;
    *(s32 *)(p22 + 24) = 69632;
    *(s32 *)(p22 + 28) = 61440;
    Func_02006c4e(1);
    *(s32 *)(p22 + 24) = 65536;
    *(s32 *)(p22 + 28) = 65536;
    Func_02006c5c(13);
    *(u8 *)((*(void **)(p22 + 80)) + 5) &= 0xfffffffffffffff3;
    *(u8 *)((*(void **)(p22 + 80)) + 17) &= 0xfffffffffffffff3;
    Func_02006cea();
    Func_02006c80(20);
    p35 = Func_02006cde();
    Func_02006d42();
    Func_02006db2();
    Func_02006d62();
    Func_02006d00(30);
    *(u8 *)((*(void **)(*(void **)(p22 + 80) + 0)) + 7) = 0;
    *(u8 *)((*(void **)(*(void **)(p22 + 80) + 0)) + 6) = 15;
    *(s32 *)(p35 + 8) = 33606820;
    *(s32 *)(p35 + 16) = 33606652;
    *(u16 *)(p35 + 6) = 33606696;
    *(s32 *)(p35 + 56) = 2147483648;
    *(s32 *)(p35 + 64) = 2147483648;
    *(s32 *)(p35 + 36) = 0;
    *(s32 *)(p35 + 44) = 0;
    *(u8 *)((u8 *)(p35) + 85) = 3;
    *(u8 *)((u8 *)(p35) + 34) = 0;
    *(s32 *)(p35 + 12) = 0;
    *(s32 *)(p35 + 20) = 0;
    Func_02006cb2();
    Func_02006c62();
    Func_02006c6a();
    Func_02006bc8(1);
}

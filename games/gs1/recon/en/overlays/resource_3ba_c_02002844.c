#include "types.h"

#define FieldScene_RunLateSequence Func_02002844

/* Audited retained field-scene body at 0x02002844.
 * The complete production body preserves 43 decoded calls, 1 loop(s), and 37 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_020049aa();
void Func_020049d6();
void Func_02004f36();
void Func_02004f62();
void Func_020063dc();
void Func_02006512();
void Func_0200651e();
void Func_02006526();
void Func_02006532();
void Func_0200653c();
void *Func_02006546();
void Func_0200655a();
void Func_0200657e();
void Func_02006580();
void *Func_020065e4();
void *Func_02006600();
void *Func_02006614();
void Func_0200662c();
void Func_02006644();
void Func_02006678();
void Func_0200668a();
void Func_02006698();
void Func_020066a4();
void Func_020066aa();
void Func_020066ac();
void Func_020066b6();
void Func_020066bc();
void Func_020066d8();
void Func_020066da();
void Func_020066e6();
void Func_0200671e();
void Func_0200672a();
void Func_0200672c();
void Func_02006732();
void Func_02006750();
void Func_0200678a();
void *Func_020067ae();
void Func_020067ba();
void Func_020067d0();
void Func_020067e6();
void Func_02006812();
void Func_02006832();
void Func_0200687a();

void Func_02002844(void)
{
    void *p2;
    void *p22;
    void *p3;
    void *p30;
    void *p35;
    s32 i1;

    Func_02006644(247);
    p2 = Func_02006600();
    p3 = Func_02006614();
    Func_02006512(30);
    Func_02006678(86);
    Func_020049aa(8);
    Func_02004f36(3, 1);
    Func_02006532();
    Func_0200653c(30);
    Func_020066a4();
    Func_020049d6(4);
    Func_02004f62(3, 0);
    Func_0200655a();
    Func_0200662c(8, 261, 0);
    Func_020063dc(1);
    Func_020066d8();
    Func_020066da(19);
    Func_02006580(30);
    Func_020066e6(289);
    Func_020066aa();
    Func_020066b6();
    p22 = Func_020065e4();
    Func_020066ac();
    *(u8 *)((u8 *)(p22) + 35) |= 0x1;
    *(u16 *)(p22 + 6) = 16384;
    Func_0200657e();
    Func_0200651e();
    Func_02006526();
    Func_0200668a();
    Func_020066bc(0, 16384, 0);
    *(u8 *)((*(void **)(p22 + 80)) + 5) &= 0xfffffffffffffff3;
    *(u8 *)((*(void **)(p22 + 80)) + 17) &= 0xfffffffffffffff3;
    Func_020067e6(252, 4, 33562768);
    for (i1 = 0; i1 <= 15; i1++) {
        *(s32 *)(p22 + 24) = 4096;
        *(s32 *)(p22 + 28) = 126976;
        p30 = Func_02006546(1, 33562768);
    }
    *(u16 *)p30 = 67109384;
    *(s32 *)(p22 + 24) = 69632;
    *(s32 *)(p22 + 28) = 61440;
    Func_0200671e(1);
    *(s32 *)(p22 + 24) = 65536;
    *(s32 *)(p22 + 28) = 65536;
    Func_0200672c(13);
    *(u8 *)((*(void **)(p22 + 80)) + 5) &= 0xfffffffffffffff3;
    *(u8 *)((*(void **)(p22 + 80)) + 17) &= 0xfffffffffffffff3;
    Func_020067ba();
    Func_02006750(20);
    p35 = Func_020067ae();
    Func_02006812();
    Func_0200687a();
    Func_02006832();
    Func_020067d0(30);
    *(u8 *)((*(void **)(*(void **)(p22 + 80) + 0)) + 7) = 0;
    *(u8 *)((*(void **)(*(void **)(p22 + 80) + 0)) + 6) = 15;
    *(s32 *)(p35 + 8) = 33605636;
    *(s32 *)(p35 + 16) = 33605468;
    *(u16 *)(p35 + 6) = 33605512;
    *(s32 *)(p35 + 56) = 2147483648;
    *(s32 *)(p35 + 64) = 2147483648;
    *(s32 *)(p35 + 36) = 0;
    *(s32 *)(p35 + 44) = 0;
    *(u8 *)((u8 *)(p35) + 85) = 3;
    *(u8 *)((u8 *)(p35) + 34) = 0;
    *(s32 *)(p35 + 12) = 0;
    *(s32 *)(p35 + 20) = 0;
    Func_0200678a();
    Func_0200672a();
    Func_02006732();
    Func_02006698(1);
}

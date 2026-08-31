#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000360

/* Audited retained supplemental scene body.
 * The complete production span preserves 54 calls, 0 loop(s), and 6 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02001d28();
void *Func_02003954();
void Func_02003974();
void Func_0200397c();
s32 Func_02003982();
void Func_02003998();
s32 Func_020039ac();
void Func_020039b4();
s32 Func_020039c8();
void Func_020039e2();
void Func_020039ee();
void Func_020039f2();
void *Func_02003a12();
void *Func_02003a20();
void Func_02003a22();
void Func_02003a28();
void Func_02003a36();
void *Func_02003a40();
void Func_02003a4c();
void Func_02003a50();
void Func_02003a68();
void Func_02003a6c();
void Func_02003a7e();
void Func_02003aa0();
void Func_02003aa2();
void Func_02003aa8();
void Func_02003aaa();
void Func_02003ab0();
void Func_02003aba();
void Func_02003ac4();
void Func_02003ad0();
void Func_02003ad4();
void Func_02003ade();
void Func_02003af4();
void Func_02003af8();
s32 Func_02003b18();
void Func_02003b3e();
void Func_02003b56();
void Func_02003b62();
void Func_02003ba8();
void Func_02003bb8();
void Func_02003bbc();
void Func_02003bca();
void Func_02003bd4();
void Func_02003bd8();
void Func_02003bf6();
void Func_02003c02();
void Func_02003c12();
void Func_02003c1a();

void Func_02000360(void)
{
    void *p1;
    void *p19;
    void *p2;
    void *p22;

    p1 = Func_02003954();
    p2 = Func_02003a12(4452);
    Func_02003a22(8, 0);
    if (Func_02003982(0, 0) == 0) {
        Func_020039ee(8, 3);
        Func_02003974(20);
    } else {
        Func_0200397c(20);
        Func_02003a4c(8, 0);
        if (Func_020039ac(0, 0) != 0) {
        } else {
            Func_02003998(20);
            Func_02003a68(8, 0);
            if (Func_020039c8(0, 0) != 0) {
            } else {
                Func_020039b4(20);
                Func_020039e2(8);
                Func_020039f2(8);
                Func_02003a12(8, 32768, 16384);
                Func_02003ac4(8, 0, 0);
                Func_020039f2(10);
                p19 = Func_02003a20(8);
                *(u8 *)((u8 *)(p19) + 90) &= 0xfe;
                Func_02003a6c(8, 152, 120);
                Func_02003a12(1);
                p22 = Func_02003a40(8);
                *(u8 *)((u8 *)(p22) + 90) |= 0x1;
                Func_02003a28(20);
                Func_02003ab0(8, 3);
                Func_02003a36(20);
                Func_02003aa0(0, 168, 120);
                Func_02003aa2(0, 192, 168);
                Func_02003a50(20);
                Func_02003aba(8, 168, 120);
                Func_02003b3e(8, 12288, 0);
                Func_02003ad4(0);
                Func_02003ad0(0, 192, 168);
                Func_02003a7e(20);
                Func_02003b62(8, 12288, 0);
                Func_02003af8(0);
                Func_02001d28();
                Func_02003bbc(0, 0);
                Func_02003bca(120);
                Func_02003aa8(120);
                Func_02003bf6(86);
                Func_02003c02();
                Func_02003aaa(2544);
                Func_02003bd8(30);
            }
        }
    }
    Func_02003ad4();
    Func_02003ade();
    Func_02003af4(33602716);
    Func_02003a22(1);
    Func_02003ba8(7165);
    Func_02003bb8(9, 0);
    if (Func_02003b18(0, 0) == 0) {
        Func_02003bd4(9, 0);
    } else {
        Func_02003c1a(2, 16, 1, 24, 1, 3, 7, 16, 1, 14, 0);
        Func_02003c12(9, 0);
    }
    Func_02003b56();
}

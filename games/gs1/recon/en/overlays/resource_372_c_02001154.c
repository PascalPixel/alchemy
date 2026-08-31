#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02001154

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 33 calls, 0 loop(s), and 15 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02002538();
void Func_02002ce8();
void Func_02002cfc();
void Func_020057ea();
s32 Func_0200583c();
s32 Func_0200584c();
void Func_0200585e();
void *Func_02005866();
void *Func_020058a6();
void *Func_020058ae();
void Func_020058d2();
void Func_020058e0();
void *Func_020058ee();
void Func_020058f4();
void Func_0200590a();
s32 Func_02005930();
void Func_02005932();
s32 Func_0200593a();
s32 Func_02005946();
void *Func_02005960();
void Func_02005962();
void Func_02005976();
void Func_0200598e();
void Func_020059a0();
void Func_020059b2();
void Func_020059c0();
void Func_02005a0a();
void Func_02005a1e();
void Func_02005a32();
void Func_02005a5c();
void Func_02005a68();
void Func_02005aba();
void Func_02005acc();

void Func_02001154(void)
{
    void *p11;
    void *p2;
    void *p21;
    void *p4;
    void *p5;

    if (Func_0200583c(786) != 0) {
    } else {
        p2 = Func_02005866();
        if (Func_0200584c(2098) == 0) {
            p4 = Func_020058a6(13);
            p5 = Func_020058ae(0);
            Func_0200585e(262144, 262144, 65536);
            Func_02005a5c(141);
            Func_020057ea(40);
            Func_02005a68(145);
            Func_020059c0(0, 3);
            p11 = Func_020058ee(0);
            *(u8 *)((u8 *)(p11) + 35) |= 0x2;
            Func_02005962(13, 0, 46071808);
            *(s32 *)(p4 + 48) = 98304;
            *(s32 *)(p4 + 52) = 98304;
            *(s32 *)(p4 + 12) = *(s32 *)(p4 + 12);
            *(s32 *)(p4 + 60) = *(s32 *)(p4 + 12);
            *(s32 *)(p4 + 68) = *(s32 *)(p4 + 12);
            Func_02005976(13, 64, 703, 32768);
            Func_020058f4(40);
            Func_02005aba(289);
            Func_020058d2(-1, -1, 58982);
            Func_020058e0();
            Func_02005acc();
            Func_0200590a(2098);
            Func_02005a32(0);
            p21 = Func_02005960(0);
            *(u8 *)((u8 *)(p21) + 35) |= 0x1;
            *(u8 *)p5 = 0;
        }
        Func_02002538();
        Func_02005932(786);
        if (Func_02005930(2103) != 0 && Func_0200593a(2113) == 0 && Func_02005946(780) == 0) {
            Func_020059a0(0);
            Func_02002ce8(62, 669);
            Func_02005a0a(0, 27, 627);
            Func_02002cfc(75, 715);
            Func_02005a1e(0, 67);
            Func_0200598e(780);
        }
        Func_020059b2();
    }
}

#include "types.h"

#define FieldScene_RunEarlySequence Func_0200076c

/* Audited retained field-scene body at 0x0200076c.
 * The complete production body preserves 47 decoded calls, 0 loop(s), and 13 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_020007e8();
void Func_020007fa();
void Func_020007fc();
void Func_02000808();
void Func_020009a2();
s32 Func_02000f3a();
s32 Func_02000f68();
void Func_02000fd0();
void Func_02005014();
void Func_02005036();
void Func_02005154();
void Func_0200519a();
void Func_020051a0();
void *Func_020051a8();
void Func_020051c0();
void Func_020051d4();
void Func_020051d6();
void Func_020051e2();
void Func_020051e4();
void Func_02005208();
void Func_0200522a();
void Func_02005238();
void Func_0200523c();
void Func_02005246();
void Func_0200527a();
void Func_02005288();
void Func_02005298();
void Func_0200529e();
void Func_020052be();
void Func_020052c4();
void Func_020052d0();
void Func_020052f4();
void Func_0200530a();
void Func_02005310();
void Func_0200531c();
void Func_0200531e();
void Func_02005344();
void Func_0200534c();
void Func_020053a6();
void Func_020053b4();
void Func_020053da();
void Func_020053ee();
void Func_0200549c();
void Func_02005538();
void Func_0200553e();
void Func_02005550();
void Func_02005556();

void Func_0200076c(void)
{
    s32 base;
    void *p2;

    Func_020051a0(500);
    p2 = Func_020051a8(31);
    Func_02005036(1048576, 0, 33555508, 524288);
    Func_020007e8();
    Func_020007fc();
    Func_020007fa();
    Func_02000808(2097152);
    if (Func_02000f3a(2) == 0 && Func_02000f68(8, 3) == 0) {
    }
    Func_020051c0(74, 8, 1, 4, 9);
    Func_020051d4(120, 60, 8, 5, 74, 60);
    Func_02005298();
    Func_02005310(33555508, 8);
    Func_0200529e(6);
    base = 13107;
    *(s32 *)(p2 + 48) = 32768;
    *(s32 *)(p2 + 52) = base;
    Func_0200519a();
    Func_020051d6();
    Func_020052c4(6);
    Func_02005344(33555508, 2);
    Func_02005154(27, 3276);
    Func_020051e2();
    Func_0200531e(33555508, 32768, base);
    Func_0200549c(239);
    Func_020051e4();
    Func_0200522a();
    Func_02005238();
    Func_02005208();
    Func_02005246();
    Func_020009a2();
    Func_020052f4();
    Func_020053b4(31, 3);
    Func_020053a6(31, 18, 6);
    Func_0200534c(30);
    Func_0200523c();
    Func_0200527a();
    *(u8 *)((u8 *)(p2) + 35) = 2;
    Func_020052be(86, 10, 1, 2, 84, 10);
    Func_020052d0(86, 9, 1, 1, 84, 12);
    Func_02005538(288);
    Func_0200553e(240);
    Func_02005288();
    Func_02005550(288);
    Func_02005556(213);
    Func_0200530a(85, 9, 1, 4, 84, 9);
    Func_0200531c(85, 9, 1, 4, 84, 61);
    Func_020053da(15);
    Func_020053ee();
    Func_02005014();
    Func_02000fd0();
}

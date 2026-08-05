#include "types.h"











/* Build the complementary background cast, then enable its shared actors. */
extern void Func_020025f0();
extern void Func_020025ee();
extern void Func_020025dc();
extern void Func_02002604();
extern void Func_0200261c();
extern void Func_02002628();
extern void Func_0200263c();
extern void Func_02002650();
extern void Func_02002676();
extern void Func_0200268a();
extern void Func_020026a4();
extern void Func_020026ae();
extern void Func_020026c4();
extern void Func_020026d8();
extern void Func_020026f2();
extern void Func_020026fe();
extern void Func_0200271a();
extern void Func_02002724();
extern void Func_02002738();
extern void Func_0200274c();
extern void Func_02002760();
extern void Func_02002774();
extern void Func_0200278e();
extern void Func_0200279a();
extern void Func_020027ae();
extern void Func_020027c0();
extern void Func_0200285e();
extern void Func_0200286c();
extern void Func_0200287a();
extern void Func_02002888();
extern void Func_02002896();
extern void Func_02002824();
extern void Func_02002838();
extern void Func_02002852();
extern void Func_02002872();
extern void Func_02002884();
extern void Func_02002898();
extern void Func_020028aa();
extern void Func_02002948();
extern void Func_02002956();
extern void Func_020028de();
extern int Func_0200290c();
extern void Func_020028f0();
extern void Func_02002a12();
extern void Func_02002a22();
extern void Func_02002a28();
void Func_0200088c(void)
{
    s16 scene = *(s16 *)(0x02000240 + 448);

    Func_020025f0(0x200);
    Func_020025ee(0x201);

    if (scene == 89) {
        Func_020025dc(64, 126, 4, 2, 22, 7);
        Func_020025f0(64, 126, 4, 2, 8, 10);
        Func_02002604(64, 126, 4, 2, 23, 21);
        Func_0200261c(64, 126, 4, 1, 23, 23);
        Func_02002628(64, 126, 4, 2, 16, 42);
        Func_0200263c(64, 126, 4, 2, 36, 44);
        Func_02002650(64, 126, 4, 2, 14, 55);
    } else if (scene == 90) {
        Func_02002676(64, 126, 4, 2, 42, 5);
        Func_0200268a(64, 126, 4, 2, 20, 11);
        Func_020026a4(64, 126, 4, 1, 20, 13);
        Func_020026ae(64, 126, 4, 2, 14, 12);
        Func_020026c4(64, 126, 4, 2, 56, 18);
        Func_020026d8(64, 126, 4, 2, 7, 22);
        Func_020026f2(64, 126, 4, 1, 7, 24);
        Func_020026fe(64, 126, 4, 2, 44, 23);
        Func_0200271a(64, 126, 4, 1, 44, 25);
        Func_02002724(64, 126, 4, 2, 38, 24);
        Func_02002738(64, 126, 4, 2, 26, 28);
        Func_0200274c(64, 126, 4, 2, 17, 35);
        Func_02002760(64, 126, 4, 2, 50, 36);
        Func_02002774(64, 126, 4, 2, 34, 43);
        Func_0200278e(64, 126, 4, 1, 34, 45);
        Func_0200279a(64, 126, 4, 2, 6, 46);
        Func_020027ae(64, 126, 4, 2, 27, 55);
        Func_020027c0(64, 126, 4, 2, 43, 56);
    }

    Func_0200285e(8, 0, 0);
    Func_0200286c(9, 0, 0);
    Func_0200287a(10, 0, 0);
    Func_02002888(11, 0, 0);
    Func_02002896(12, 0, 0);

    if (scene == 91) {
        Func_02002824(0, 121, 5, 7, 8, 32);
        Func_02002838(0, 121, 5, 7, 43, 32);
        Func_02002852(6, 120, 3, 1, 9, 5);
        Func_0200285e(9, 120, 3, 1, 44, 5);
        Func_02002872(9, 0, 3, 3, 9, 6);
        Func_02002884(64, 126, 4, 2, 12, 20);
        Func_02002898(64, 126, 4, 2, 18, 24);
        Func_020028aa(64, 126, 4, 2, 21, 28);
    }

    Func_02002948(13, 0, 0);
    Func_02002956(14, 0, 0);
    Func_020028de(69, 99, 4, 1, 6, 20);
    if (Func_0200290c(0x90a) == 0)
        Func_020028f0(69, 99, 4, 2, 10, 23);
    Func_02002a12(107, 0, 0);
    Func_02002a22(107);
    if (scene != 92)
        Func_02002a28(0);
}

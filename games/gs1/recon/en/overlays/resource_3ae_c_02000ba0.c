#include "types.h"

/* Complete no-loop event setup. The pointer-derived coordinates and final
 * signed fixed-point corrections are kept explicit because they are part of
 * this owner's observable state changes. */

void Func_02002124();
void Func_02002148();
void *Func_0200215c();
void Func_02002170();
void Func_0200218e();
void Func_020021b6();
void Func_020021bc();
void Func_020021c0();
void Func_020021e2();
void Func_020021e8();
void Func_020021fe();
void Func_02002216();
void Func_02002230();
void Func_02002232();
void Func_02002234();
void Func_0200223e();
void Func_02002242();
void Func_02002244();
void *Func_0200224a();
void Func_0200224c();
void Func_02002258();
void Func_02002266();
void Func_0200226c();
void Func_02002272();
void Func_02002278();
void Func_0200228a();
void Func_0200228c();
void Func_02002298();
void Func_0200229e();
void Func_020022aa();
void Func_020022c2();
void Func_020022dc();
void Func_020022e2();
void Func_020022e8();
void Func_020022ee();
void Func_020022f6();
void Func_020022fc();
void Func_02002306();
void Func_0200230a();
void Func_02002310();
void Func_02002316();
void Func_02002318();
void Func_02002342();
void Func_0200234a();
void Func_0200234c();
void Func_02002352();
s32 Func_02002358();
void Func_0200236c();
void Func_02002374();
void Func_02002382();
void *Func_02002386();
void Func_02002392();
void Func_02002398();
void *Func_0200239a();
void Func_020023a0();
void Func_020023a4();
void Func_020023a6();
void Func_020023aa();
void Func_020023b0();
void Func_020023b2();

void Func_02000ba0(void)
{
    void *p4;
    void *p6;
    void *p66;
    void *p60;
    void *p67;

    Func_02002124();
    Func_02002230();
    Func_0200218e(8, 21495808, 93323264);
    p4 = Func_0200215c(8);
    *(u8 *)((u8 *)(p4) + 91) = 1;
    Func_0200223e();
    p6 = Func_0200224a();
    Func_02002148(20);
    Func_02002278(1, -16, 0, 32768);
    Func_020021b6(1);
    Func_02002232(0, 40960, 0);
    Func_02002170(20);
    Func_02002216(8073);
    Func_0200224a(0, 40960, 0);
    Func_020021bc(1, 104857, 52428);
    Func_020021e8(1, 232, 1424);
    Func_0200226c(1, 32768, 0);
    Func_0200229e(12058624, -1, 94371840, 1);
    Func_020022aa();
    Func_020021c0(10);
    Func_02002242(1, 6, 15);
    Func_0200224c(1, 6, 40);
    Func_0200228c(1, 0);
    Func_020021e2(20);
    Func_020022dc(17301504, -1, 94896128, 1);
    Func_020022e8();
    Func_020021fe(20);
    Func_020022e2(8, 256, 50);
    Func_02002244(8, 78643, 39321);
    Func_02002272(8, 264, 1424);
    Func_020022f6(8, 32768, 0);
    Func_02002234(10);
    Func_02002306(1, 0, 0);
    Func_02002244(20);
    Func_0200224a(10);
    Func_020022c2(8, 4);
    Func_02002258(10);
    Func_02002310(8, 0);
    Func_02002266(20);
    Func_0200234a(1, 258, 40);
    Func_02002278(30);
    Func_0200234c(1, 32768, 0);
    Func_0200228a(50);
    Func_02002342(1, 0);
    Func_02002298(20);
    Func_0200236c(1, 8192, 0);
    Func_020022aa(30);
    Func_020022e8(1, 65536, 32768);
    Func_02002316(1, 264, 1464);
    Func_02002398(1, 0, 0);
    Func_020023a4(0, 32768, 0);
    Func_020023b0(8, 16384, 0);
    Func_020022ee(30);
    Func_020023a6(1, 0);
    Func_020022fc(10);
    Func_02002374(0, 3);
    Func_0200230a(30);
    Func_02002382(1, 3);
    Func_02002318(30);
    Func_02002352(1, 78643, 39321);
    Func_02002392(1, 2);
    p60 = (void *)Func_02002358(0);
    if (p60 != 0) {
        Func_02002382(1, *(s16 *)((u8 *)p60 + 10), *(s16 *)((u8 *)p60 + 18));
    }
    Func_020023a0(1);
    Func_020023b2(1, 0, 0);
    Func_02002358(20);
    p66 = Func_02002386(8);
    *(u8 *)((u8 *)(p66) + 91) = 0;
    Func_020023a4(8, 2);
    p67 = Func_0200239a(8);
    if (*(s32 *)((u8 *)p67 + 8) < 0)
        *(s16 *)((u8 *)p67 + 100) = (*(s32 *)((u8 *)p67 + 8) + 0xffff) >> 16;
    if (*(s32 *)((u8 *)p67 + 16) < 0)
        *(s16 *)((u8 *)p67 + 102) = (*(s32 *)((u8 *)p67 + 16) + 0xffff) >> 16;
    Func_020023aa();
}

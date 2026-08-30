#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunPartyIntroduction:
 * 61 calls, one conditional setup loop, one workspace write, and the
 * returned actor handle. Recovered from the complete executable owner. */

#define FieldScene_RunPartyIntroduction Func_02000a98

u32 Func_02004f5c();
void Func_020053f2();
void Func_02005408();
void Func_0200542c();
void Func_0200543a();
void Func_02005450();
void Func_02005454();
void Func_02005460();
void Func_02005466();
void Func_02005490();
void Func_02005496();
void Func_020054aa();
void Func_020054bc();
void Func_020054c8();
void Func_020054ce();
void Func_020054d6();
void Func_020054fa();
void Func_02005506();
void Func_0200550e();
void Func_02005520();
void Func_02005532();
void Func_02005544();
void Func_0200554a();
void Func_0200554c();
void Func_02005564();
void Func_0200557c();
void Func_02005584();
void Func_02005592();
void Func_020055a8();
void Func_020055ae();
void Func_020055b2();
void Func_020055ba();
void Func_020055c0();
void Func_020055c6();
void Func_020055d0();
void Func_020055d2();
void Func_020055d4();
void Func_020055e0();
void Func_020055fa();
void Func_02005630();
void Func_02005648();
void Func_0200564e();
void Func_02005654();
void Func_02005666();
void Func_0200567e();
void Func_02005698();
void Func_020056a6();
void Func_020056be();
void Func_020056ca();
void Func_020056d0();
void Func_02005738();
void Func_02005746();
void Func_02005764();
void Func_02005770();
void Func_0200578e();
void Func_020057a2();
void Func_020057ce();

void Func_02000a98(void)
{
    s32 i1;
    u32 actor;

    Func_020055ba(141);
    for (i1 = 0; i1 != 6; i1++) {
        Func_0200557c(4213202, 1);
        Func_02005592(8);
        Func_02005450(8);
        Func_02005592(65536, 1);
        Func_020055a8(8);
        Func_02005466(8);
        if (i1 == 1) {
            Func_020053f2(65536, 65536, 65536);
        }
    }
    Func_02005408(131072, 65536, 65536);
    Func_02005496(30);
    Func_020055ae(157286, 19660);
    Func_020055c6(10944512, -1, 34668544, 1);
    Func_020055d2();
    Func_0200543a(65536, 131072, 65536);
    Func_020054c8(20);
    Func_0200564e(144);
    Func_0200542c(33607816, 65, 31);
    Func_02005454(0, 0, 1, 1, 10, 31);
    Func_02005460(87, 42, 10, 33, 1, 2);
    Func_0200550e(40);
    Func_02005490(0, 0, 0);
    Func_02005630(419430, 52428);
    Func_02005648(25624576, -1, 11599872, 1);
    Func_02005654();
    Func_020054bc(65536, 131072, 65536);
    Func_0200554a(20);
    Func_020056d0(144);
    Func_020054aa(33607816, 79, 9);
    Func_020054ce(0, 0, 1, 1, 24, 9);
    Func_020054d6(87, 42, 24, 11, 1, 2);
    Func_02005584(40);
    Func_02005506(0, 0, 0);
    Func_020056a6(157286, 19660);
    Func_020056be(38207488, -1, 12648448, 1);
    Func_020056ca();
    Func_02005532(65536, 131072, 65536);
    Func_020055c0(20);
    Func_02005746(144);
    Func_02005520(33607816, 91, 10);
    Func_02005544(0, 0, 1, 1, 36, 10);
    Func_0200554c(87, 42, 36, 12, 1, 2);
    Func_020055fa(40);
    *(u32 *)(*(u32 *)0x0200d088 + 448) = 514;
    Func_02005764();
    Func_02005770();
    Func_02005738(15204352, -1, 31260672, 0);
    Func_02005564();
    Func_020054fa(1);
    Func_020055b2(131072, 65536, 65536);
    Func_0200578e();
    Func_020057a2();
    Func_02005648(40);
    Func_020057ce(289);
    Func_020055d4(-1, -1, 58982);
    Func_020055e0();
    Func_02005666(20);
    Func_020055d0(0, 40, 13, 66, 3, 3);
    Func_0200567e(20);
    actor = Func_02004f5c(223, 15204352, 1048576, 30408704);
    Func_02005698(40);
    Func_02005630(actor, 1);
    Func_02005630(289, 1);
}

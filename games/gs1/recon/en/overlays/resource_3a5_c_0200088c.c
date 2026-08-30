#include "types.h"

/* AUDITED ACTOR-LAYOUT SCRIPT for FieldScene_ConfigureAlternateActorLayout:
 * scene-state dispatch, shared actor initialization, and terminal transition. */

#define FieldScene_ConfigureAlternateActorLayout Func_0200088c

void Func_020025dc();
void Func_020025ee();
void Func_020025f0();
void Func_02002604();
void Func_0200261c();
void Func_02002628();
void Func_0200263c();
void Func_02002650();
void Func_02002676();
void Func_0200268a();
void Func_020026a4();
void Func_020026ae();
void Func_020026c4();
void Func_020026d8();
void Func_020026f2();
void Func_020026fe();
void Func_0200271a();
void Func_02002724();
void Func_02002738();
void Func_0200274c();
void Func_02002760();
void Func_02002774();
void Func_0200278e();
void Func_0200279a();
void Func_020027ae();
void Func_020027c0();
void Func_02002824();
void Func_02002838();
void Func_02002852();
void Func_0200285e();
void Func_0200286c();
void Func_02002872();
void Func_0200287a();
void Func_02002884();
void Func_02002888();
void Func_02002896();
void Func_02002898();
void Func_020028aa();
void Func_020028de();
void Func_020028f0();
s32 Func_0200290c();
void Func_02002948();
void Func_02002956();
void Func_02002a12();
void Func_02002a22();
void Func_02002a28();

void Func_0200088c(void)
{
    s32 i1;
    s16 scene_state;

    Func_020025f0(512);
    Func_020025ee(0x201);
    scene_state = *(s16 *)(0x02000240 + 448);
    if (scene_state == 89) {
    Func_020025dc(64, 126, 4, 2, 22, 7);
    Func_020025f0(68, 126, 4, 2, 8, 10);
    Func_02002604(72, 126, 4, 2, 23, 21);
    Func_0200261c(72, 126, 4, 2, 23, 22);
    Func_02002628(76, 126, 4, 2, 16, 42);
    Func_0200263c(80, 126, 4, 2, 36, 44);
    Func_02002650(84, 126, 4, 2, 14, 55);
    Func_02002896(9, 0x19000000, 0x16c00000);
    } else if (scene_state == 90) {
    Func_02002676(64, 126, 4, 2, 42, 5);
    Func_0200268a(68, 126, 4, 2, 20, 11);
    Func_020026a4(68, 126, 4, 2, 20, 12);
    Func_020026ae(72, 126, 4, 2, 14, 12);
    Func_020026c4(76, 126, 4, 2, 56, 18);
    Func_020026d8(80, 126, 4, 2, 7, 22);
    Func_020026f2(80, 126, 4, 2, 7, 23);
    Func_020026fe(84, 126, 4, 2, 44, 23);
    Func_0200271a(84, 126, 4, 2, 44, 24);
    Func_02002724(88, 126, 4, 2, 38, 24);
    Func_02002738(92, 126, 4, 2, 26, 28);
    Func_0200274c(96, 126, 4, 2, 17, 35);
    Func_02002760(100, 126, 4, 2, 50, 36);
    Func_02002774(104, 126, 4, 2, 34, 43);
    Func_0200278e(104, 126, 4, 2, 34, 44);
    Func_0200279a(108, 126, 4, 2, 6, 46);
    Func_020027ae(112, 126, 4, 2, 27, 55);
    Func_020027c0(116, 126, 4, 2, 43, 56);
    Func_0200285e(9, 23068672, 13369344);
    Func_0200286c(10, 48234496, 25952256);
    Func_0200287a(11, 9437184, 24903680);
    Func_02002888(12, 37748736, 46923776);
    Func_02002896(13, 42467328, 26738688);
    } else if (scene_state == 91) {
    Func_02002824(64, 124, 4, 4, 8, 14);
    Func_02002838(68, 124, 4, 4, 6, 18);
    Func_02002852(68, 124, 4, 1, 6, 20);
    Func_0200285e(72, 124, 4, 4, 10, 21);
    Func_02002872(10, 121, 5, 7, 8, 32);
    Func_02002884(5, 121, 5, 7, 43, 32);
    Func_02002898(0, 120, 3, 1, 9, 5);
    Func_020028aa(3, 120, 3, 1, 44, 5);
    Func_02002948(8, 11010048, 6029312);
    Func_02002956(9, 8388608, 20709376);
    Func_020028de(6, 0, 3, 3, 9, 6);
    if (Func_0200290c(0x90a) == 0) {
        Func_020028f0(0, 119, 3, 1, 9, 5);
    }
    }
    for (i1 = 100; i1 <= 107; i1++) {
        Func_02002a12(i1, -1, -1);
    }
    Func_02002a22();
    if (scene_state != 92) {
        Func_02002a28();
    }
}

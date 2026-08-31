#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSixPassEffectSequence:
 * 57 calls, a six-pass setup loop with one conditional color update, a
 * guarded actor-position transfer, and the closing scene-state write. */

#define FieldScene_RunSixPassEffectSequence Func_02002400

s32 Func_020066f4();
void Func_02006d58();
void Func_02006d68();
void Func_02006d76();
void Func_02006da2();
void Func_02006db6();
void Func_02006dc8();
void Func_02006dcc();
void Func_02006e16();
void Func_02006e30();
void Func_02006e52();
void Func_02006e68();
void Func_02006e8c();
void Func_02006e9c();
void Func_02006eac();
void *Func_02006eba();
void Func_02006ec2();
void Func_02006ec4();
void Func_02006ece();
void Func_02006ed8();
void Func_02006eda();
void Func_02006ee0();
void Func_02006ee2();
void Func_02006ee6();
void Func_02006ef6();
void Func_02006ef8();
void Func_02006f06();
void Func_02006f0e();
void Func_02006f10();
void Func_02006f14();
void Func_02006f20();
void Func_02006f66();
void Func_02006f70();
void Func_02006f94();
void Func_02006fea();
void Func_02006ff6();
void Func_02007018();
void Func_02007022();
void Func_02007026();
void Func_02007030();
void Func_0200703a();
void Func_0200703c();
void Func_02007044();
void Func_02007048();
void Func_0200704a();
void Func_02007054();
void Func_02007060();
void Func_02007094();
void Func_020070fe();
void Func_02007100();
void Func_02007126();
void Func_02007132();

extern u8 *Data_03001ebc;

void Func_02002400(void)
{
    void *actor;
    s32 effect;
    u8 i;

    Func_02006da2();
    Func_02006f20(141);
    for (i = 0; i != 6; i++) {
        Func_02006ee2(4209106, 1);
        Func_02006ef8(8);
        Func_02006db6(8);
        Func_02006ef8(65536, 1);
        Func_02006f0e(8);
        Func_02006dcc(8);
        if (i == 1) {
            Func_02006d58(65536, 65536, 65536);
        }
    }
    Func_02006f70(289);
    Func_02006d76(-1, -1, 58982);
    Func_02006d68(0, 40, 13, 46, 3, 3);
    Func_02006e16(20);
    effect = Func_020066f4(222, 15204352, 1048576, 9437184);
    Func_02006e30(40);
    Func_02006dc8(effect, 1);
    Func_02006dc8(4216, 1);
    Func_02006eba(5, 20119552, 18153472);
    Func_02006ec4(9, 20119552, 18153472);
    Func_02006ece(11, 20119552, 18153472);
    Func_02006ed8(10, 20119552, 18153472);
    Func_02006ee2(14, 20119552, 18153472);
    Func_02006eac(0, 78643, 39321);
    Func_02006ee6(0, 232, 156);
    Func_02006e8c(10);
    actor = Func_02006eba(0);
    if (actor != 0) {
        Func_02006f10(1, *(s32 *)((u8 *)actor + 8), *(s32 *)((u8 *)actor + 16));
    }
    Func_02006eda(1, 78643, 39321);
    Func_02006f14(1, 218, 172);
    Func_02006f66(1, 0, 0);
    Func_02006ec4(20);
    Func_0200704a(145);
    Func_02006e52(262144, 262144, 65536);
    Func_02006ee0(20);
    Func_02006e68(65536, 65536, 65536);
    Func_02006ef6(40);
    Func_02006fea(0, 53248, 0);
    Func_02006ff6(1, 20480, 50);
    Func_02007094(144);
    Func_02006e9c(196608, 196608, 65536);
    Func_02007018(0, 32768, 0);
    Func_02007022(1, 0, 50);
    Func_02006ec2(65536, 65536, 65536);
    Func_0200703c(0, 0, 0);
    Func_02007048(1, 32768, 50);
    Func_02007054(0, 45056, 0);
    Func_02007060(1, 53248, 0);
    Func_020070fe(144);
    Func_02006f06(196608, 196608, 65536);
    Func_02006f94(30);
    Func_02007026(0, 2, 0);
    Func_02007030(1, 2, 20);
    Func_0200703a(0, 6, 0);
    Func_02007044(1, 6, 40);
    *(s32 *)(Data_03001ebc + 448) = 256;
    Func_02007126();
    Func_02007132();
    Func_02007100(2);
}

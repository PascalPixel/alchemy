#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_ArrangeFourActors:
 * 47 calls, source-relative actor placement, three coordinate handoffs,
 * and the final scene positioning sequence. */

#define FieldScene_ArrangeFourActors Func_02001a0c

void Func_0200642a();
void *Func_02006432();
void Func_02006460();
void Func_0200646e();
void Func_0200647c();
void Func_0200648a();
void Func_02006498();
void *Func_020064dc();
void Func_020064ec();
void Func_020064fe();
void Func_0200650c();
void Func_02006518();
void Func_02006526();
void Func_0200653e();
void Func_02006560();
s32 Func_0200656c();
void Func_02006580();
s32 Func_0200658c();
void Func_02006590();
void Func_02006598();
void Func_020065a0();
void Func_020065a6();
void Func_020065a8();
s32 Func_020065ac();
void Func_020065b0();
void Func_020065b8();
void Func_020065be();
void Func_020065c0();
void Func_020065c2();
void Func_020065c6();
void Func_020065d0();
void Func_020065de();
void Func_020065e0();
void Func_020065e6();
void Func_020065fe();
void Func_02006600();
void Func_02006606();
void Func_02006626();
void Func_02006628();
void Func_02006630();
void Func_02006632();
void Func_0200663a();
void Func_0200663c();
void Func_020066c4();

void Func_02001a0c(s32 scene)
{
    s16 *p1;
    void *p13;
    s32 x;
    s32 y;
    s16 *actor;

    p1 = Func_02006432();
    x = p1[5];
    y = p1[9];
    Func_0200642a();
    Func_02006460(scene, 65536, 32768);
    Func_0200646e(0, 65536, 32768);
    Func_0200647c(1, 65536, 32768);
    Func_0200648a(2, 65536, 32768);
    Func_02006498(3, 65536, 32768);
    Func_020064ec(0, x << 16, (y << 16) - 0x300000);
    Func_020064fe(1, (x << 16) - 0x100000, (y << 16) - 0x280000);
    Func_0200650c(2, (x << 16) + 0x100000, (y << 16) - 0x280000);
    Func_02006518(3, x << 16, (y << 16) - 0x200000);
    Func_02006526(scene, x << 16, (y << 16) - 0x500000);
    p13 = Func_020064dc(0);
    *(u16 *)(p13 + 6) = 49152;
    Func_020065c2(0, 0);
    Func_02006626();
    Func_0200663a();
    Func_02006590(0x20ed);
    Func_02006560(scene, 3);
    Func_020065b0(scene, 0);
    Func_02006580(scene, 2);
    Func_020065c0(scene, 0);
    Func_02006590(scene, 2);
    Func_020065d0(scene, 0);
    Func_020065a0(scene, 2);
    Func_020065e0(scene, 0);
    Func_02006598(3, 3);
    Func_020065a0(1, 3);
    Func_020065a8(2, 3);
    Func_020065b8(0, 3);
    Func_0200653e(6);
    Func_020065be(1, 2);
    actor = (s16 *)Func_0200656c(0);
    if (actor != 0) {
        Func_020065a6(1, actor[5], actor[9]);
    }
    Func_020065de(2, 2);
    actor = (s16 *)Func_0200658c(0);
    if (actor != 0) {
        Func_020065c6(2, actor[5], actor[9]);
    }
    Func_020065fe(3, 2);
    actor = (s16 *)Func_020065ac(0);
    if (actor != 0) {
        Func_020065e6(3, actor[5], actor[9]);
    }
    Func_02006606(scene, x - 16, y - 64);
    Func_02006628(1, 0, 0);
    Func_02006632(2, 0, 0);
    Func_0200663c(3, 0, 0);
    Func_02006630(scene, x - 16, y - 16);
    Func_0200663a(scene, x, y);
    Func_020066c4(scene, 49152, 10);
    Func_02006600();
}

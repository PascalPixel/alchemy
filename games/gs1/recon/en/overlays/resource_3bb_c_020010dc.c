#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_ConfigureActorFormation:
 * 47 calls, coordinate-relative placement, three actor lookups, and closing
 * position updates across the complete callable owner. */

#define FieldScene_ConfigureActorFormation Func_020010dc

void Func_02005032();
void *Func_0200503a();
void Func_02005068();
void Func_02005076();
void Func_02005084();
void Func_02005092();
void Func_020050a0();
void *Func_020050e4();
void Func_020050f6();
void Func_02005104();
void Func_02005110();
void Func_0200511e();
void Func_02005146();
void Func_02005158();
void Func_02005170();
s32 Func_02005174();
void Func_02005180();
void Func_02005190();
s32 Func_02005194();
void Func_02005198();
void Func_020051a0();
void Func_020051a6();
void Func_020051b0();
void Func_020051b2();
s32 Func_020051b4();
void Func_020051b6();
void Func_020051c0();
void Func_020051c6();
void Func_020051d0();
void Func_020051d6();
void Func_020051e6();
void Func_020051f6();
void Func_02005206();
void Func_02005208();
void Func_0200521a();
void Func_02005220();
void Func_0200522a();
void Func_02005230();
void Func_02005234();
void Func_0200523a();
void Func_020052b4();

void Func_020010dc(int actor)
{
    void *object;
    void *p13;
    s32 x;
    s32 y;

    object = Func_0200503a(actor);
    x = *(s16 *)(object + 10) << 16;
    y = *(s16 *)(object + 18) << 16;
    Func_02005032();
    Func_02005068(actor, 65536, 32768);
    Func_02005076(0, 65536, 32768);
    Func_02005084(1, 65536, 32768);
    Func_02005092(2, 65536, 32768);
    Func_020050a0(3, 65536, 32768);
    Func_020050e4(0, x, y - 0x300000);
    Func_020050f6(1, x - 0x100000, y - 0x280000);
    Func_02005104(2, x + 0x100000, y - 0x280000);
    Func_02005110(3, x, y - 0x200000);
    Func_0200511e(actor, x, y - 0x500000);
    p13 = Func_020050e4(0);
    *(u16 *)(p13 + 6) = 49152;
    Func_020051b2(0, 0);
    Func_02005206();
    Func_0200521a();
    Func_02005180(0x20e9);
    Func_02005158(actor, 3);
    Func_020051a0(actor, 0);
    Func_02005170(actor, 2);
    Func_020051b0(actor, 0);
    Func_02005180(actor, 2);
    Func_020051c0(actor, 0);
    Func_02005190(actor, 2);
    Func_020051d0(actor, 0);
    Func_02005190(3, 3);
    Func_02005198(1, 3);
    Func_020051a0(2, 3);
    Func_020051b0(0, 3);
    Func_02005146(6);
    Func_020051b6(1, 2);
    object = (void *)Func_02005174(0);
    if (object != 0) {
        Func_020051a6(1, *(s16 *)(object + 10), *(s16 *)(object + 18));
    }
    Func_020051d6(2, 2);
    object = (void *)Func_02005194(0);
    if (object != 0) {
        Func_020051c6(2, *(s16 *)(object + 10), *(s16 *)(object + 18));
    }
    Func_020051f6(3, 2);
    object = (void *)Func_020051b4(0);
    if (object != 0) {
        Func_020051e6(3, *(s16 *)(object + 10), *(s16 *)(object + 18));
    }
    Func_02005206(actor, (x >> 16) - 16, (y >> 16) - 64);
    Func_02005220(1, 0, 0);
    Func_0200522a(2, 0, 0);
    Func_02005234(3, 0, 0);
    Func_02005230(actor, (x >> 16) - 16, (y >> 16) - 16);
    Func_0200523a(actor, x >> 16, y >> 16);
    Func_020052b4(actor, 49152, 10);
    Func_02005208();
}

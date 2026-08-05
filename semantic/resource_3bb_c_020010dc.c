#include "types.h"

/*
 * resource_3bb owner at 0x020010dc, 532 bytes: configure the five-member
 * handler family, place its members around the current object's signed
 * position pair, initialize the family's shared state, then copy the
 * positions of three optional members into their matching slots.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl spill at
 * 0x020010dc through `bx r0` at 0x020012d6. Its five-word literal pool
 * occupies 0x020012d8-0x020012ef; the next owner's prologue is at
 * 0x020012f0. One argument (r0), void.
 *
 * Not found by the structural inventory walk: reached by one `bl` from
 * resource_3bb_c_02000bd4.c. The calls below were checked against all 47
 * ROM call sites. Names describe addresses only; the handler records and
 * the meaning of their signed halfwords remain deliberately unnamed.
 */



















extern s32 Func_0200503a();
extern s32 Func_02005032();
extern s32 Func_02005068();
extern s32 Func_02005076();
extern s32 Func_02005084();
extern s32 Func_02005092();
extern s32 Func_020050a0();
extern s32 Func_020050e4();
extern s32 Func_020050f6();
extern s32 Func_02005104();
extern s32 Func_02005110();
extern s32 Func_0200511e();
extern s32 Func_020051b2();
extern s32 Func_02005206();
extern s32 Func_0200521a();
extern s32 Func_02005180();
extern s32 Func_02005158();
extern s32 Func_020051a0();
extern s32 Func_02005170();
extern s32 Func_020051b0();
extern s32 Func_020051c0();
extern s32 Func_02005190();
extern s32 Func_020051d0();
extern s32 Func_02005198();
extern s32 Func_02005146();
extern s32 Func_020051b6();
extern s32 Func_02005174();
extern s32 Func_020051a6();
extern s32 Func_020051d6();
extern s32 Func_02005194();
extern s32 Func_020051c6();
extern s32 Func_020051f6();
extern s32 Func_020051b4();
extern s32 Func_020051e6();
extern s32 Func_02005220();
extern s32 Func_0200522a();
extern s32 Func_02005234();
extern s32 Func_02005230();
extern s32 Func_0200523a();
extern s32 Func_020052b4();
extern s32 Func_02005208();
void Func_020010dc(s32 self)
{
    u8 *object = (u8 *)Func_0200503a(self);
    s32 x = *(short *)(object + 10);
    s32 y = *(short *)(object + 18);
    u8 *member;

    Func_02005032();

    Func_02005068(self, 0x10000, 0x8000);
    Func_02005076(0, 0x10000, 0x8000);
    Func_02005084(1, 0x10000, 0x8000);
    Func_02005092(2, 0x10000, 0x8000);
    Func_020050a0(3, 0x10000, 0x8000);

    Func_020050e4(0, x << 16, (y << 16) - 0x300000);
    Func_020050f6(1, (x << 16) - 0x280000, (y << 16) - 0x100000);
    Func_02005104(2, (x << 16) + 0x100000, (y << 16) - 0x100000);
    Func_02005110(3, x << 16, (y << 16) - 0x200000);
    Func_0200511e(self, x << 16, (y << 16) - 0x500000);

    member = (u8 *)Func_020050e4(0);
    *(u16 *)(member + 6) = 0xc000;
    Func_020051b2(0, 0);
    Func_02005206();
    Func_0200521a();

    Func_02005180(0x20e9);
    Func_02005158(self, 3);
    Func_020051a0(self, 0);
    Func_02005170(self, 2);
    Func_020051b0(self, 0);
    Func_02005180(self, 2);
    Func_020051c0(self, 0);
    Func_02005190(self, 2);
    Func_020051d0(self, 0);

    Func_02005190(3, 3);
    Func_02005198(1, 3);
    Func_020051a0(2, 3);
    Func_020051b0(0, 3);
    Func_02005146(6);
    Func_020051b6(1, 2);

    member = (u8 *)Func_02005174(0);
    if (member != 0) {
        Func_020051a6(1, *(short *)(member + 10), *(short *)(member + 18));
    }

    Func_020051d6(2, 2);
    member = (u8 *)Func_02005194(0);
    if (member != 0) {
        Func_020051c6(2, *(short *)(member + 10), *(short *)(member + 18));
    }

    Func_020051f6(3, 2);
    member = (u8 *)Func_020051b4(0);
    if (member != 0) {
        Func_020051e6(3, *(short *)(member + 10), *(short *)(member + 18));
    }

    Func_02005206(self, x - 16, y - 64);
    Func_02005220(1, 0, 0);
    Func_0200522a(2, 0, 0);
    Func_02005234(3, 0, 0);
    Func_02005230(self, x - 16, y - 16);
    Func_0200523a(self, x, y);
    Func_020052b4(self, x << 16, 10);
    Func_02005208();
}

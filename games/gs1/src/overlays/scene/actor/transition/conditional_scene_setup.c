#include "types.h"

#define FieldScene_PlaceActorEightByFlags Func_020005b8
#define FieldScene_PlaceActorTenByFlags Func_02000768
void Func_020029ce(s32);
s32 Func_020028de(s32);
s32 Func_020028e8(s32);
void Func_02000760(s32, s32, s32);
void Func_02002910(s32);
void Func_0200290e(s32);
void Func_0200077c(s32, s32, s32);
void Func_02002924(s32);
void Func_02002932(s32);
void Func_02002a20(s32);
void Func_02002876(s32);
void Func_020018e6(void);
void Func_02002b7e(s32);
s32 Func_02002a8e(s32);
void Func_02000906(s32, s32, s32);
void Func_02002ab4(s32);
void Func_02002ab4_a(s32);
void Func_02002ac2(s32);
void Func_02002ac8(s32);
void Func_0200092e(s32, s32, s32);
void Func_02002ad4(s32);
void Func_02002ae4(s32);
void Func_02002aea(s32);
void Func_02002af0(s32);
void Func_02002bde(s32);
void Func_02002a34(s32);
void Func_02001aa4(void);

void FieldScene_PlaceActorEightByFlags(void)
{
    Func_020029ce((s32) 0xF1);

    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. The second test is only reached
     * when the first fails, and both truths take the same path. */
    if (Func_020028de((s32) 0x310) != 0 || Func_020028e8((s32) 0x30D) != 0) {
        Func_02000760(8, -48, 0);
        Func_02002910((s32) 0x308);
        Func_0200290e((s32) 0x309);
    } else {
        Func_0200077c(8, -96, 0);
        Func_02002924((s32) 0x308);
        Func_02002932((s32) 0x309);
    }

    Func_02002a20((s32) 0x121);
    Func_02002876(2);
    Func_020018e6();
}

void FieldScene_PlaceActorTenByFlags(void)
{
    Func_02002b7e(241);
    if (Func_02002a8e(0x308) != 0) {
        Func_02000906(10, 0, -64);
        Func_02002ab4(0x30b);
        Func_02002ab4_a(0x30c);
        Func_02002ac2(0x30d);
        Func_02002ac8(0x30e);
    } else {
        Func_0200092e(10, 0, -128);
        Func_02002ad4(0x30b);
        Func_02002ae4(0x30c);
        Func_02002aea(0x30d);
        Func_02002af0(0x30e);
    }
    Func_02002bde(0x121);
    Func_02002a34(2);
    Func_02001aa4();
}

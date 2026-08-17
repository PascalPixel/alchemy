#include "types.h"
struct EffectObject_020007d8 { u8 unknown_00[10]; s16 unkA; u8 unknown_0c[6]; s16 unk12; u8 unknown_14[0x41]; u8 mode55; u8 unknown_56[0x16]; s32 callback; };
struct EffectObject_020007d8 *Func_0200568a();
s32 Func_0200565a();
void Func_02005684();
void Func_0200578a();
s32 Func_02005712();
void Func_0200575a();
s32 Func_020056d8();
void Func_02005676();
void Func_020057fa();
void Func_0200580e();
void Func_0200583c();
void Func_02005702();
void Func_02005794();
void Func_020056b0();
void Func_0200585a();
void Func_02005874();
void Func_02005720();
void Func_020007d8(void)
{
    struct EffectObject_020007d8 *object;
    s32 busy;
    object = Func_0200568a(0);
    busy = Func_0200565a(0x109);
    if (busy != 0) return;
    Func_02005684();
    Func_0200578a(-1, -1, -1, 0);
    object->mode55 = 0;
    Func_02005712(0, object->unkA << 16, (object->unk12 << 16) + 0xfff00000);
    Func_0200575a(0, 15);
    Func_02005676(Func_020056d8(0), 0);
    Func_020057fa();
    Func_0200580e();
    Func_0200583c(228);
    object->callback = 0x020086a1;
    Func_02005702(0, 0x6666, 0x3333);
    Func_0200583c(0, 0, 8);
    Func_02005794(0, 0);
    Func_020056b0(Func_02005712(0), 1);
    Func_0200585a(0, 0, 8);
    object->mode55 = 3;
    object->callback = 0;
    Func_02005874();
    Func_02005720();
}

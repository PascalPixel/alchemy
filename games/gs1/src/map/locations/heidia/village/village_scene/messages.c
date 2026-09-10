#include "types.h"

#define ShowLockedDoorMessageOnce Func_02000f94
#define ShowLockedDoorMessage Func_02000fb4
#define ShowGeraldDontMessage Func_02000fc8
#define ShowThatWasExcitingMessage Func_02000fe4
#define ShowSorryIScaredYouMessage Func_02001000
#define ShowWellMessage Func_0200101c
extern s32 Value_00000953;
extern s32 Value_00000f2a;
extern s32 Value_00000f2b;
extern s32 Value_00000f2c;
extern s32 Value_00000f2d;

s32 Func_020065a2(s32);
void Func_020065aa(s32, s32);
void Func_020065bc(s32, s32);
s32 Func_02002008(s32, s32);
void Func_020065f2(s32);
s32 Func_02002024(s32, s32);
void Func_0200660e(s32);
s32 Func_02002040(s32, s32);
void Func_0200662a(s32);
s32 Func_0200205c(s32, s32);
void Func_02006646(s32);

void ShowLockedDoorMessageOnce(void)
{
    if (Func_020065a2(0xea) == -1) {
        Func_020065aa((s32)&Value_00000953, 1);
    }
}

void ShowLockedDoorMessage(void)
{
    Func_020065bc((s32)&Value_00000953, 1);
}

void ShowGeraldDontMessage(void)
{
    if (Func_02002008(8, 8) != 0) {
        Func_020065f2((s32)&Value_00000f2a);
    }
}

void ShowThatWasExcitingMessage(void)
{
    if (Func_02002024(9, 7) != 0) {
        Func_0200660e((s32)&Value_00000f2b);
    }
}

void ShowSorryIScaredYouMessage(void)
{
    if (Func_02002040(10, 6) != 0) {
        Func_0200662a((s32)&Value_00000f2c);
    }
}

void ShowWellMessage(void)
{
    if (Func_0200205c(11, 5) != 0) {
        Func_02006646((s32)&Value_00000f2d);
    }
}

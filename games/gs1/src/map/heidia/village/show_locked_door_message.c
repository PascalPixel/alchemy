#include "types.h"

extern s32 Value_00000953;

void Func_020065bc(s32, s32);

#define ShowLockedDoorMessage Func_02000fb4

void ShowLockedDoorMessage(void)
{
    Func_020065bc((s32)&Value_00000953, 1);
}

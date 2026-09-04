#include "types.h"

#define ActorPresentation_SetupActorEightForFlags308And309 Func_020006c0
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_0200085e(s32, s32, s32);
void Func_020019ba();
void Func_0200294a(s32);
void Func_020029f6(s32);
void Func_020029fc(s32);
void Func_02002ad6(s32);
void Func_02002af4(s32);

void ActorPresentation_SetupActorEightForFlags308And309(void) {
    Func_02002ad6(0xF1);
    Func_020029f6(0x308);
    Func_020029fc(0x309);
    Func_0200085e(8, 0x60, 0);
    Func_02002af4(0x121);
    Func_0200294a(2);
    Func_020019ba();
}

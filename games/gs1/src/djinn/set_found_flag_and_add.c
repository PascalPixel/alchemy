#include "types.h"

s32 GameFlag_Set(s32);
s32 Func_080771a8(s32, s32, s32);

void Djinn_SetFoundFlagAndAdd(s32 arg0, s32 arg1, s32 arg2) {
    GameFlag_Set((arg1 * 0x14) + arg2 + 0x30);
    Func_080771a8(arg0, arg1, arg2);
}

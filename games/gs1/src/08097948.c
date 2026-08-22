#include "types.h"

s32 Func_080979a4(s32, s32, s32);

void Func_08097948(s32 value, s32 *maximum, s32 *center, s32 *minimum) {
    *maximum = Func_080979a4(value + 0x780000, 0, 0x1F0000);
    *center = Func_080979a4(value, 0, 0x1F0000);
    *minimum = Func_080979a4(value + 0xFF880000, 0, 0x1F0000);
}

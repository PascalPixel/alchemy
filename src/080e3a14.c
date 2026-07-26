#include "types.h"

void Func_080e3a14(void) {
    u8 *state = *(u8 **)0x03001EEC;
    u8 *object = *(u8 **)(state + 0x7828);

    if (*(s16 *)(object + 0x24) > 0x7F) {
        (void)*(volatile s32 *)0x03001AE8;
    }
}

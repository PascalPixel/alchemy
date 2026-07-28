#include "types.h"
#include "m7_interfaces.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080a4e44(void) {
    void *gameState = *(void **)0x03001F2C;
    struct Bounds080a23f4 *window =
        M2C_FIELD(gameState, struct Bounds080a23f4 **, 0x20);

    Func_080a23f4(window, 0xD, 3, 0x11, 0xA);
}

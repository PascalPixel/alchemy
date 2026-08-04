#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08017620(s32 flags) {
    void *state;

    state = *(void **)0x03001E8C;
    if (state != NULL) {
        if (flags & 1) {
            M2C_FIELD(state, s8 *, 0x12FA) = 1;
        }
        if (2 & flags) {
            M2C_FIELD(state, s8 *, 0x12FB) = 1;
        }
    }
}

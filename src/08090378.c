#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08090378(s32 clear_0800, s32 clear_0400, s32 clear_0200) {
    void *state;

    state = *(void **)0x03001E70;
    if (state != NULL) {
        if (clear_0200 != 0) {
            M2C_FIELD(state, u16 *, 0x14) &= 0xFDFF;
        }
        if (clear_0400 != 0) {
            M2C_FIELD(state, u16 *, 0x14) &= 0xFBFF;
        }
        if (clear_0800 != 0) {
            M2C_FIELD(state, u16 *, 0x14) &= 0xF7FF;
        }
    }
}

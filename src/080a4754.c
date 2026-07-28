#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 Func_08004458();
void *Func_08077018(s32);
s32 Func_08077060(u8, u16);
s32 Func_080a1d08(s32, s32, s32);
s32 Func_080f9010(s32);

void Func_080a4754(void) {
    void *state;

    state = *(void **)0x03001F2C;
    if ((M2C_FIELD(Func_08077018(0x1FF & M2C_FIELD(state, u16 *, 0x178)), u8 *, 0xC) == 2) && (Func_08004458() < 0x2000U)) {
        Func_08077060(M2C_FIELD(state, u8 *, 0x21A), M2C_FIELD(state, u16 *, 0x174));
        Func_080f9010(0x8A);
        Func_080a1d08(0xB86, 0, -1);
    }
}

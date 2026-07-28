#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

u8 *Func_08077080(s32);
s32 Func_0808a488(u8);

s32 Func_080a5fe0(void) {
    u8 *entry;
    s32 difference;
    s32 result;

    entry = Func_08077080((s32) (0x3FFF & *(u16 *)(*(u8 **)0x03001F2C + 0x178)));
    if (Func_0808a488(entry[0xC]) != 0) {
        return 0;
    }
    result = 2;
    if (entry[8] != 0xFF) {
        u8 kind = entry[0];
        difference = kind ^ 2;
        result = (0 - difference) | difference;
        result = (s32) ((u32) result >> 0x1F);
        result = 1 - result;
    }
    return result;
}

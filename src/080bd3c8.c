#include "types.h"

struct Entry_080bd3c8 {
    u8 unknown_00[9];
    u8 status;
};

void *Func_08077080();

u32 Func_080bd3c8(s32 value) {
    u8 status;

    if (value == 0x7E) {
        return 1U;
    }
    status = ((struct Entry_080bd3c8 *)Func_08077080())->status;
    return (u32) ((0 - status) | status) >> 0x1F;
}

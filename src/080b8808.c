#include "types.h"

s32 Func_080b8808(u32 arg0) {
    if (arg0 <= 7) {
        return 0;
    }
    if (arg0 - 128 <= 5) {
        return 0;
    }
    return -1;
}

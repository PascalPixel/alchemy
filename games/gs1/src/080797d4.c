#include "types.h"

extern const u8 Data_08084a9c[];

s32 Func_080797d4(u32 arg0) {
    if (arg0 > 0xFU) {
        return 0;
    }
    return (s32)(Data_08084a9c + arg0 * 8);
}

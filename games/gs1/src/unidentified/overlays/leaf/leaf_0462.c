#include "types.h"

extern s16 Data_02000240[];

/* One import, four call sites, four distinct veneer addresses. */
s32 Func_02001a64();
s32 Func_02001a76();
s32 Func_02001a84();
s32 Func_02001a92();

s32 Func_02000160(void)
{
    if (Data_02000240[225] == 19) {
        if (Func_02001a64(0x950) != 0) {
            return 0x0200AC5C;
        }
        return 0x0200AB9C;
    }

    if (Func_02001a76(0x834) != 0) {
        return 0x0200A5A8;
    }
    if (Func_02001a84(0x87A) != 0) {
        return 0x0200A980;
    }
    if (Func_02001a92(0x815) != 0) {
        return 0x0200A800;
    }
    return 0x0200A6B0;
}

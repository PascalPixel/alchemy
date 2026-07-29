#include "types.h"

struct Runtime_080200cc {
    u8 pad_000[0x224];
    s32 entries[4];
    u8 pad_234[0x234 - 0x224 - 16];
    s16 offsets[8];
};

extern struct Runtime_080200cc *Data_03001f2c;
s32 Func_08009008(s32, void *, void *, s32);

void Func_080200cc(void)
{
    struct Runtime_080200cc *base;
    s16 *offsets;
    s32 *entries;
    s32 index;
    s32 handle;

    index = 0;
    base = Data_03001f2c;
    offsets = base->offsets;
    entries = base->entries;

    do {
        handle = *entries++;
        if (handle != 0) {
            s32 source[2];
            s32 request[4];

            source[0] = 0x10000;
            source[1] = 0x10000;
            request[0] = offsets[0] << 16;
            request[1] = 0x01f40000;
            request[2] = (offsets[4] << 16) + 0x01f40000;
            request[3] = 0;
            Func_08009008(handle, request, source, 0x4000);
        }
        index++;
        offsets++;
    } while (index <= 3);
}

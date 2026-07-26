#include "types.h"

void Func_080a9d84(u8 *);
void Func_080a17c4(void *);

void Func_080a9d3c(u8 *flags)
{
    u8 *base;
    void **slot;
    void *entry;
    s32 index;
    s32 value;
    u16 kind;

    base = *(u8 **)0x03001f2c;
    Func_080a9d84(flags);
    index = 0;
    slot = (void **)(base + 200);
    value = 88;
    do {
        entry = *slot++;
        if (entry != 0 && flags[index] != 0) {
            kind = 8;
            *(u16 *)((u8 *)entry + 6) = kind;
            *(u16 *)((u8 *)entry + 8) = value;
            *(u8 *)((u8 *)entry + 15) = 240;
            Func_080a17c4(entry);
            value += 16;
        }
        index++;
    } while (index <= 4);
}

#include "types.h"

void Func_0807a3a8(void);
s32 Func_08077330(s32 arg0);

u32 *Func_0807a458(u32 kind, u32 first, u32 second)
{
    u8 *state;
    u8 *entries;
    u8 *entry;
    u32 *countAddress;
    u32 count;
    u32 offset;

    Func_0807a3a8();
    state = Func_08077330(kind > 7);
    entries = state + 8;
    countAddress = (u32 *)(state + 0x108);
    count = *countAddress;
    offset = count * 4;
    entries[offset] = first;
    count++;
    entry = entries + offset;
    entry[1] = second;
    entry[2] = kind;
    entry[3] = 0xFF;
    *countAddress = count;
    return countAddress;
}

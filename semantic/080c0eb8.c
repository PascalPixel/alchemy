#include "types.h"

struct Entry_080c0eb8 {
    u32 value;
    u32 sum;
    u32 field8;
    u32 fieldC;
};

void Func_080c0eb8(struct Entry_080c0eb8 *entries)
{
    u32 previous = entries[0].value;

    *entries++ = (struct Entry_080c0eb8){ 0x10000, 0, 0, 0 };
    *entries++ = (struct Entry_080c0eb8){ 0x10000, 0, 0, 0 };
    *entries++ = (struct Entry_080c0eb8){ 0x10000, 0, 0, 0 };
    entries -= 3;
    entries[0].sum = previous + entries[0].value;
}

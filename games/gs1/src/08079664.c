#include "types.h"
#include "party_state.h"
void Func_08079374(s32 value);

s32 Func_08079664(s32 arg0)
{
    s32 count;
    s32 index;
    s32 limit;

    count = Func_080795fc(arg0);
    Func_08079374(arg0);

    index = 0;
    while (index < count
        && Data_02000240.active_owners[index] != arg0) {
        index++;
    }

    limit = count - 1;
    if (index < limit) {
        s32 remaining;
        u8 *base;
        u8 *cursor;

        base = (u8 *)&Data_02000240;
        base += index;
        cursor = base + 0x1f8;
        remaining = limit - index;
        do {
            remaining--;
            *cursor = cursor[1];
            cursor++;
        } while (remaining != 0);
    }

    return Func_080795fc(limit);
}

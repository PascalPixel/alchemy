#include "game_flags.h"
#include "party_state.h"

s32 Func_08079664(s32 arg0)
{
    s32 count;
    s32 index;
    s32 limit;

    count = Party_CountActiveOwners(arg0);
    GameFlag_Clear(arg0);

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

    return Party_CountActiveOwners(limit);
}

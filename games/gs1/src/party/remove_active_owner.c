#include "game_flags.h"
#include "party_state.h"

s32 Party_RemoveActiveOwner(s32 owner_id)
{
    s32 active_count;
    s32 owner_index;
    s32 last_index;

    active_count = Party_CountActiveOwners(owner_id);
    GameFlag_Clear(owner_id);

    owner_index = 0;
    while (owner_index < active_count
        && Data_02000240.active_owners[owner_index] != owner_id) {
        owner_index++;
    }

    last_index = active_count - 1;
    if (owner_index < last_index) {
        s32 remaining_count;
        u8 *owner_base;
        u8 *owner_cursor;

        owner_base = (u8 *)&Data_02000240;
        owner_base += owner_index;
        owner_cursor = owner_base + 0x1f8;
        remaining_count = last_index - owner_index;
        do {
            remaining_count--;
            *owner_cursor = owner_cursor[1];
            owner_cursor++;
        } while (remaining_count != 0);
    }

    return Party_CountActiveOwners(last_index);
}

#include "game_flags.h"
#include "party_state.h"

s32 GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

s32 Party_CountActiveOwners(void)
{
    s32 owner;
    s32 count;

    count = 0;
    owner = 0;
    do {
        if (GameFlag_Test(owner) != 0)
            count++;
        owner++;
    } while (owner <= 7);
    return count;
}

s32 Party_AddActiveOwner(s32 value)
{
    s32 count = Party_CountActiveOwners();
    s32 index;

    GameFlag_SetBit(value);
    index = 0;
    while (index < count) {
        if (Data_02000240.active_owners[index] == value)
            return count;
        index++;
    }
    Data_02000240.active_owners[index] = value;
    return count + 1;
}

s32 Party_RemoveActiveOwner(s32 owner_id)
{
    s32 active_count;
    s32 owner_index;
    s32 last_index;

    active_count = Party_CountActiveOwners();
    GameFlag_ClearBit(owner_id);
    owner_index = 0;
    while (owner_index < active_count
           && Data_02000240.active_owners[owner_index] != owner_id)
        owner_index++;

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
    return Party_CountActiveOwners();
}

s32 Party_ListActiveOwners(s16 *owners)
{
    s32 count = 0;

    if (owners != NULL) {
        s32 index;

        count = Party_CountActiveOwners();
        index = 0;
        if (count != 0) {
            do {
                *owners++ = Data_02000240.active_owners[index];
                index++;
            } while (index != count);
        }
        *owners = 0xff;
    }
    return count;
}

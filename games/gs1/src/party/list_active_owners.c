#include "party_state.h"

s32 Party_ListActiveOwners(s16 *owners)
{
    s32 count = 0;

    if (owners != 0) {
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

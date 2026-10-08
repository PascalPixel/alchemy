#include "types.h"
#include "party_state.h"

s32 Party_CountActiveOwnersFar(void);

s32 Func_080ca5a0(s32 owner)
{
    s32 count = Party_CountActiveOwnersFar();
    s32 i;

    if (count > 4)
        count = 4;
    for (i = 0; i < count; i++) {
        if (gPartyState.active_owners[i] == owner)
            return 1;
    }
    return 0;
}

/* Draft, differ 22 of 29: the native indexes the owner list from sp on every pass (lsls, mov sp, ldrh) and counts i up to count; this form strength-reduces the index to a pointer and counts down. Tried: owners[i++] in a while loop, a u16 list, one minute of permute (best differ 19). */
#include "types.h"
#include "owner_state.h"
#include "party_state.h"

void UnitCalc(s32 owner);

void Func_02000704(s32 levels)
{
    s16 owners[16];
    s32 count = Party_ListActiveOwners(owners);
    s32 i;

    for (i = 0; i < count; i++) {
        s32 owner = (u16)owners[i];

        Party_AdvanceOwnerCountToTarget(owner, ((struct BtlUnit *)UnitGet(owner))->level + levels);
        UnitCalc(owner);
    }
}

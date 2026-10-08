#include "types.h"
#include "owner_state.h"
#include "party_state.h"

extern u16 Data_020023c4[];

void UnitCalc(s32 owner);
void GameFlag_ClearBit(s32 flag);

void Party_RestoreSnapshot(void)
{
    u16 *src = Data_020023c4;
    s32 owner;

    if (*src++ == 0x6774) {
        for (owner = 4; owner <= 7; owner++) {
            u16 *dst = (u16 *)((u8 *)UnitGet(owner) + 216);
            s32 i;

            for (i = 14; i >= 0; i--)
                *dst++ = *src++;
            Owner_RefreshDerivedData(owner);
            UnitCalc(owner);
        }
        gPartyState.packed_events[0] = *src++;
        gPartyState.packed_events[1] = *src++;
        *(u16 *)&gPartyState.active_owners[0] = *src++;
        *(u16 *)&gPartyState.active_owners[2] = src[0];
        src = Data_020023c4;
        *src = 0;
    }
    GameFlag_ClearBit(0x9ff);
}

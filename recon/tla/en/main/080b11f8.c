/* Near miss, differ 10: restores the second party's (owners 4-7) fifteen
   halfwords of state and four party halfwords from the snapshot at
   Data_020023c4 when it carries 0x6774, clears its mark and flag 0x9ff.
   The native loads the first party halfword before building 0x240 (the
   load-first gap) and builds the zero for the mark before the last copy.
   Reassigning the cursor to the snapshot for the clear fixed the register
   choice. Joins a module after 080b1088 on a match. */
#include "TYPES.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"

extern u16 Data_020023c4[];

void Owner_RecalculateStats(s32 owner);
void GameFlag_ClearBit(s32 flag);

void Party_RestoreSnapshot(void)
{
    u16 *src = Data_020023c4;
    s32 owner;

    if (*src++ == 0x6774) {
        for (owner = 4; owner <= 7; owner++) {
            u16 *dst = (u16 *)((u8 *)Owner_GetState(owner) + 216);
            s32 i;

            for (i = 14; i >= 0; i--)
                *dst++ = *src++;
            Owner_RefreshDerivedData(owner);
            Owner_RecalculateStats(owner);
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

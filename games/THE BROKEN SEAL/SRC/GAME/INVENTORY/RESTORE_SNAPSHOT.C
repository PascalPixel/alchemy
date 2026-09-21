#include "TYPES.H"
#include "OWNER_STATE.H"
#include "PSYNERGY_MENU.H"
#define PARTY_STATE Data_02000240

#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

extern u16 Data_02001078[];

void GameFlag_ClearBit(s32 flag);

#define InventorySnapshot_Restore Func_0807a7a0

/* Restores a saved inventory snapshot when it starts with its sentinel: the
 * fifteen inventory words of each of the four owners, refreshing their
 * derived data and stats, then the two Psynergy shortcuts and the two words
 * at party state 0x1f8. The sentinel is then cleared, as is flag 0x952. */
void InventorySnapshot_Restore(void)
{
    s32 owner;
    u16 *source = Data_02001078;

    if (*source++ == INVENTORY_SNAPSHOT_SENTINEL) {
        owner = 0;
        do {
            u16 *inventory =
                ((struct OwnerInventoryState *)OwnerState_Get(owner))->inventory;
            s32 remaining = 14;

            do {
                *inventory++ = *source++;
                remaining--;
            } while (remaining >= 0);
            Owner_RefreshDerivedData(owner);
            Owner_RecalculateStats(owner);
            owner++;
        } while (owner <= 3);

        PARTY_STATE.psynergy_shortcuts[0] = *source++;
        PARTY_STATE.psynergy_shortcuts[1] = *source++;
        *(u16 *)((u8 *)&PARTY_STATE + 0x1f8) = *source++;
        *(u16 *)((u8 *)&PARTY_STATE + 0x1fa) = *source;
        source = Data_02001078;
        *source = 0;
        GameFlag_ClearBit(0x952);
    }
}

#include "TYPES.H"
#include "OWNER_STATE.H"
#include "GAME_STATE.H"

#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

extern u16 gInventorySnapshot[];

void GameFlag_ClearBit(s32 flag);


/* Restores a saved inventory snapshot when it starts with its sentinel: the
 * fifteen inventory words of each of the four owners, refreshing their
 * derived data and stats, then the two Psynergy shortcuts and the two words
 * at party state 0x1f8. The sentinel is then cleared, as is flag 0x952. */
void InventorySnapshot_Restore(void)
{
    s32 owner;
    u16 *source = gInventorySnapshot;

    if (*source++ == INVENTORY_SNAPSHOT_SENTINEL) {
        owner = 0;
        do {
            u16 *inventory =
                ((struct BattleUnit *)Owner_GetState(owner))->inventory;
            s32 remaining = 14;

            do {
                *inventory++ = *source++;
                remaining--;
            } while (remaining >= 0);
            Owner_RefreshDerivedData(owner);
            Owner_RecalculateStats(owner);
            owner++;
        } while (owner <= 3);

        gGameState.first_shortcut = *source++;
        gGameState.second_shortcut = *source++;
        *(u16 *)((u8 *)gGameState.active_owners) = *source++;
        *(u16 *)((u8 *)gGameState.active_owners + 2) = *source;
        source = gInventorySnapshot;
        *source = 0;
        GameFlag_ClearBit(0x952);
    }
}

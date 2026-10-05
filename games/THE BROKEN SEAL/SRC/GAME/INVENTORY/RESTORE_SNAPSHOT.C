#include "TYPES.H"
#include "INVENTORY.H"
#include "CHARACTER.H"
#include "GAME_STATE.H"
#include "ITEM.H"

#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

extern u16 gInventorySnapshot[];

void Inventory_AddAndEquip(s32 owner, s32 target);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Owner_RefreshActiveRatios(s32 mode);

void InventorySnapshot_Save(void)
{
    u16 *save;
    struct BattleUnit *unit;
    s16 *inventory;
    s32 owner;
    s32 i;
    s32 n;
    s16 first;
    s16 second;

    save = gInventorySnapshot;
    if (*save != INVENTORY_SNAPSHOT_SENTINEL) {
        *save++ = INVENTORY_SNAPSHOT_SENTINEL;
        first = gGameState.first_shortcut;
        second = gGameState.second_shortcut;
        for (owner = 0; owner < 4; owner++) {
            unit = (struct BattleUnit *)Owner_GetState(owner);
            for (n = 0; n < 15; n++)
                *save++ = unit->inventory[n];
            for (n = 0; n < 15; n++) {
                if (Item_GetDirect(unit->inventory[n])->type != 6)
                    unit->inventory[n] = 0;
            }
            n = 0;
            inventory = (s16 *)unit->inventory;
            for (i = 0; i < 15; i++) {
                if (inventory[i] != 0)
                    inventory[n++] = inventory[i];
            }
            for (; n < 15; n++)
                inventory[n] = 0;
            Owner_RefreshDerivedData(owner);
            Owner_RecalculateStats(owner);
        }
        *save++ = first;
        *save++ = second;
        inventory = (s16 *)gGameState.active_owners;
        save[0] = inventory[0];
        save[1] = inventory[1];
        Inventory_AddAndEquip(0, 16);
        GameFlag_SetBit(0x952);
    }
    Owner_RefreshActiveRatios(1);
}

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

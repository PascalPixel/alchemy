#include "INVENTORY.H"
#include "BATTLE_TYPES.H"

extern struct ActionDefinition BattleAction_DefinitionTable[];

s32 PartyInventory_CountItem(s32 item_id)
{
    u16 owners[16];
    s32 item_count = 0;
    s32 owner_count = Party_ListActiveOwners(owners);

    if (item_count < owner_count) {
        u16 *owner_cursor = owners;
        s32 n = owner_count;

        do {
            item_count += Inventory_CountItem(*owner_cursor++, item_id);
            n--;
        } while (n != 0);
    }
    return item_count;
}

/* An action id's definition; ids past the table read the first entry. */
struct ActionDefinition *BattleAction_GetDirect(s32 action_id)
{
    u32 entry_index = action_id & 0x3fff;
    struct ActionDefinition *table;

    if (entry_index > 0x2de)
        entry_index = 0;
    table = BattleAction_DefinitionTable;
    return &table[entry_index];
}

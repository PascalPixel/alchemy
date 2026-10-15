#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "inventory.h"
#include "battle_type.h"


s32 Equipment_HasValue(s32 owner, s32 value);

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

struct ActionDefinition *BattleAction_GetDirect(s32 action_id)
{
    u32 entry_index = action_id & 0x3fff;
    struct ActionDefinition *table;

    if (entry_index > 0x2de)
        entry_index = 0;
    table = BattleAction_DefinitionTable;
    return &table[entry_index];
}

s32 Party_FindOwnerWithAction(s32 action)
{
    s16 owners[10];
    s32 count = Party_ListActiveOwners(owners);
    s32 i;

    for (i = 0; i < count; i++) {
        if (Equipment_HasValue(owners[i], action) != 0)
            break;
    }
    if (i == count)
        return -1;
    return owners[i];
}

s32 Equipment_HasValue(s32 owner, s32 value)
{
    struct BtlUnit *unit = UnitGet(owner);
    struct OwnerActionSlot *entry;
    s32 index = 0;
    s32 mask = OWNER_ACTION_ID_MASK;

    entry = unit->action_slots;
    do {
        s32 current = entry->encoded_action;

        entry++;
        if ((current & mask) == value)
            return 1;
        index++;
    } while (index <= 31);
    return 0;
}

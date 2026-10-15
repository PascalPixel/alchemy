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

s32 PartyInventory_CountItem(s32 itemId)
{
    u16 owners[16];
    s32 itemCount = 0;
    s32 ownerCount = Party_ListActiveOwners(owners);

    if (itemCount < ownerCount) {
        u16 *ownerCursor = owners;
        s32 n = ownerCount;

        do {
            itemCount += Inventory_CountItem(*ownerCursor++, itemId);
            n--;
        } while (n != 0);
    }
    return itemCount;
}

struct ActionDefinition *BattleAction_GetDirect(s32 actionId)
{
    u32 entryIndex = actionId & 0x3fff;
    struct ActionDefinition *table;

    if (entryIndex > 0x2de)
        entryIndex = 0;
    table = gBattleAction_DefinitionTable;
    return &table[entryIndex];
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
    struct BattleUnit *unit = UnitGet(owner);
    struct OwnerActionSlot *entry;
    s32 index = 0;
    s32 mask = OWNER_ACTION_ID_MASK;

    entry = unit->actionSlots;
    do {
        s32 current = entry->encodedAction;

        entry++;
        if ((current & mask) == value)
            return 1;
        index++;
    } while (index <= 31);
    return 0;
}

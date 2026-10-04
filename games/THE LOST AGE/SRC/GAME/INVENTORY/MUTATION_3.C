#include "INVENTORY.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_UNIT.H"

enum { ITEM_COUNTER_COUNT = 300 };

extern u8 gItemCounters[ITEM_COUNTER_COUNT];

s32 Inventory_Break(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] |= 0x400;
    return 0;
}

s32 Inventory_Repair(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] &= ~0x400;
    return 0;
}

u8 Item_GetTargetMode(s32 item_id)
{
    return BattleAction_GetDirect(
        Item_GetDirect(item_id)->action_id)->target_mode;
}

/* Add delta to one of the 300 item counters, held between 0 and 99; the
   result is the new count, or 0 when it hit the floor or the slot is out of
   range. */
s32 ItemCounter_Adjust(s32 index, s32 delta)
{
    s32 counter_slot = index;
    u8 *data = gItemCounters;

    index = 0;
    if (counter_slot < ITEM_COUNTER_COUNT) {
        s32 value = data[counter_slot];

        value += delta;

        if (value < 0) {
            value = 0;
        } else if (value > 99) {
            value = 99;
            index = 99;
        } else {
            index = value;
        }
        data[counter_slot] = value;
    }
    return index;
}

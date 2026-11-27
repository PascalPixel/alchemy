// Colosseum Final 1, 2 and 3, and Goma Cave Entrance: giving a character an item and putting it on.

#include "game/inventory.h"

// Gives the character the item, then equips every inventory slot that holds it.
UNUSED static void scGiveAndEquipItem(Sint32 character, Sint32 item)
{
    UNIT  *unit = gmUnitGet(character);
    Sint32 i;

    gmInventoryAddItem(character, item);
    for (i = 0; i < INVENTORY_SLOTS; i++)
    {
        if (unit->inventory[i] == item)
        {
            gmInventoryEquip(character, i);
        }
    }
}

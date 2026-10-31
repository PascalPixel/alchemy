#include "types.h"
#include "item.h"
#include "ram_buffer.h"

// The item menu work block, as this function reads it.
struct InventoryMenuState {
  u8 unk000[0x16c];
  u16 selectedItem;
  u8 unk16e[0xa7];
  u8 partyCount;
  u8 itemOwner;
};

#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1

s32 Item_ClassifyUseMode(s32 owner, s32 item);
s32 BattleEffect_HasTriggerFar(s32 item);

void Func_080fb410(s8 *commandStates) {
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;

  menu = Ram_HeapSlots->menuRuntime;
  item = Item_Get(ITEM_ID_MASK & menu->selectedItem);

  if (item->type == 0) {
    commandStates[0] = COMMAND_AVAILABLE;
    commandStates[1] = COMMAND_DISABLED;
  } else {
    commandStates[0] = COMMAND_DISABLED;
    commandStates[1] = COMMAND_AVAILABLE;
  }

  if (Item_ClassifyUseMode(menu->itemOwner, menu->selectedItem) != -1)
    commandStates[0] = COMMAND_AVAILABLE;
  else
    commandStates[0] = COMMAND_DISABLED;

  if (menu->selectedItem & INVENTORY_BROKEN)
    commandStates[0] = COMMAND_DISABLED;

  if (Item_CanOwnerEquip(menu->itemOwner, menu->selectedItem & ITEM_ID_MASK) ==
      0) {
    commandStates[1] = COMMAND_DISABLED;
  }

  commandStates[3] = COMMAND_AVAILABLE;
  commandStates[5] = COMMAND_AVAILABLE;
  commandStates[2] = COMMAND_AVAILABLE;

  if (menu->selectedItem & INVENTORY_EQUIPPED) {
    commandStates[4] = COMMAND_AVAILABLE;
    commandStates[1] = COMMAND_DISABLED;
  } else {
    commandStates[4] = COMMAND_DISABLED;
  }

  if (item->flags & ITEM_CANNOT_UNEQUIP) {
    commandStates[4] = COMMAND_DISABLED;
    /* FAKEMATCH(one-pass): the equipped test in a once-only block, which
     * keeps the store above from sinking below the selected item's address;
     * as the plain if, 2 of 157 instructions differ in tla-en. */
    do {
      if (menu->selectedItem & INVENTORY_EQUIPPED) {
        commandStates[3] = COMMAND_DISABLED;
        commandStates[5] = COMMAND_DISABLED;
      }
    } while (0);
  }

  if (BattleEffect_HasTriggerFar(menu->selectedItem & ITEM_ID_MASK) != 0)
    commandStates[0] = COMMAND_AVAILABLE;

  if (menu->partyCount <= 1)
    commandStates[3] = COMMAND_DISABLED;

  if (item->flags & ITEM_INDISPENSABLE)
    commandStates[5] = COMMAND_DISABLED;
}

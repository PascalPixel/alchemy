// Near miss, differ 8 of 157: the item menu's command availability (use,
// equip, give, drop, trade, remove) for the selected item, ☀️'s
// ItemMenu_BuildCommands in games/tbs/src/menu/menu.c. Instruction order only:
// four loads and stores (the selected item's halfword, the -1 store to the
// drop slot, the 0x1ff mask's first move) sit one or two slots apart.
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
s32 BattleFx_HasTriggerFar(s32 item);

void ItemMenu_BuildCommands(s8 *commandStates) {
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;

  menu = Ram_HeapSlots->menuRuntime;
  item = Item_Get(0x1ff & menu->selectedItem);

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

  if (menu->selectedItem & 0x400)
    commandStates[0] = COMMAND_DISABLED;

  if (Item_CanOwnerEquip(menu->itemOwner, menu->selectedItem & 0x1ff) == 0) {
    commandStates[1] = COMMAND_DISABLED;
  }

  commandStates[3] = COMMAND_AVAILABLE;
  commandStates[5] = COMMAND_AVAILABLE;
  commandStates[2] = COMMAND_AVAILABLE;

  if (menu->selectedItem & 0x200) {
    commandStates[4] = COMMAND_AVAILABLE;
    commandStates[1] = COMMAND_DISABLED;
  } else {
    commandStates[4] = COMMAND_DISABLED;
  }

  if (item->flags & 2) {
    commandStates[4] = COMMAND_DISABLED;
    if (menu->selectedItem & 0x200) {
      commandStates[3] = COMMAND_DISABLED;
      commandStates[5] = COMMAND_DISABLED;
    }
  }

  if (BattleFx_HasTriggerFar(menu->selectedItem & 0x1ff) != 0)
    commandStates[0] = COMMAND_AVAILABLE;

  if (menu->partyCount <= 1)
    commandStates[3] = COMMAND_DISABLED;

  if (item->flags & 8)
    commandStates[5] = COMMAND_DISABLED;
}

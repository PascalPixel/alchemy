/// @file
/// The item menu, as far as this object holds it: the field menu's Item
/// screen, its command loop, Use, the first screen's setup and the item icons.
// Part of menu.c.

#include "menu_internal.h"
#include "resource_ids.h"
#include "sounds.h"
#include "field/field_sequence.h"
#include "shop/shop.h"

/// Each item command's state in the grid.
#define ITEM_COMMAND_STATE_DISABLED (-1)
#define ITEM_COMMAND_STATE_AVAILABLE 1

// Defined below or in later parts of the menu object.
s32 ItemMenu_ChooseCommand(void);
/// Copies the unit's items into the list items (32 entries), skipping empty
/// slots and clearing the rest. Returns how many there are. mode is not used.
s32 ItemMenu_CollectItems(struct Unit *owner, u16 *items, s32 mode);
s32 ItemMenu_ConfirmDrop(s32 item);
s32 ItemMenu_ConfirmEquip(s32 unused);
s32 ItemMenu_CountItems(s32 ownerId);
void ItemMenu_DrawIcons(u16 *items, s32 style);
void ItemMenu_DrawItemHead(void);
void ItemMenu_DrawMessage(s32 unused, s32 message);
s32 ItemMenu_GetItemQuantity(s32 owner, s32 item);
s32 ItemMenu_IsStatRaisingItem(s32 itemId);
void ItemMenu_RefreshMember(s32 ownerId, s32 mode);
s32 ItemMenu_RunList(s32 pane);
s32 ItemMenu_SelectGiveQuantity(s32 baseQuantity, s32 quantityRange,
                                s32 dropMode);
s32 ItemMenu_SelectMember(s32 pane);
/// Whom ItemMenu_SelectTarget chooses.
enum ItemTargetMode {
  ITEM_TARGET_USE = 0, ///< the party member to use the item on
  ITEM_TARGET_GIVE = 1 ///< the party member to give it to
};
s32 ItemMenu_SelectTarget(s32 mode);
void ItemMenu_PlaceItemWindowBelowQuestion(void);
void ItemMenu_PlaceItemWindowBelowItemQuestion(void);
void ItemMenu_FitMessageWindowToQuestion(void);
void ItemMenu_FitMessageWindowToItemQuestion(void);
s32 ItemMenu_ShowDetails(s32 value);
void ItemMenu_TryBreak(void);
s32 ItemMenu_Use(void);
s32 Item_ClassifyUseMode(s32 owner, s32 itemId);
void Menu_PlaceOwnerEntryObjects(struct Window *window, s32 originX,
                                 s32 originY);

/// Where the edition's text goes. The international editions have longer
/// words, and some languages move a message to fit its own.
///
/// The x of the small message windows "You were cursed!" and "Removed".
#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_MODAL_SHORT_X 14
#define ITEM_MODAL_REMOVED_X 14
#else
#define ITEM_MODAL_SHORT_X 15
#define ITEM_MODAL_REMOVED_X 17
#endif

/// "Can't trade it!" starts four tiles further left in French, over the status
/// window, which is redrawn after it.
#if LANGUAGE == LANGUAGE_FRENCH
#define ITEM_CANNOT_TRADE_X 11
#else
#define ITEM_CANNOT_TRADE_X 15
#endif

/// The column "Traded" opens at, two tiles further left in Italian.
#if LANGUAGE == LANGUAGE_ITALIAN
#define ITEM_TRADED_X 13
#else
#define ITEM_TRADED_X 15
#endif

void Resource_LoadPairedBlocks(void);
void ItemMenu_Close(void);
s32 ItemMenu_RunCommands(s32 *memberOut, s32 *targetOut, s32 *itemOut);
void ItemMenu_Initialize(s32 x, s32 y, s32 mode, s32 columns);
void ItemMenu_HideAllIcons(void);

/// The field menu's Item screen: choose a party member, one of their items and
/// a command. Owns the menu work while it runs and frees it before returning.
/// Returns ItemMenu_RunCommands' result: 1 when an item is to be used on the
/// field, -1 when the player backed out or pressed Start.
s32 ItemMenu_Run(void) {
#if TLA_PATCH_1
  void *backup;
#endif
  struct ItemMenuWork *screen;
  s32 item;
  s32 target;
  s32 owner;
  s32 result;
#if TLA_PATCH_1
  s32 size;
#endif

  screen = HeapSlot_Alloc(HEAP_SLOT_MENU, MENU_WORK_SIZE);
#if TLA_PATCH_1
  size = MENU_SAVED_TILE_BYTES;
  backup = Heap_AllocEwram(size);
#endif
  Ram_HeapSlots->actorSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Menu_InitializeWork(0);
  screen->partyCount = Party_ListMembersFar(screen->memberIds);
  ItemMenu_Initialize(0, 3, 0, 7);
  Resource_LoadPairedBlocks();
  Palette_LightenHighlight(HIGHLIGHT_PALETTE);
  Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);
  screen->questionWindow =
      (struct Window *)Window_CreateFar(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
#if TLA_PATCH_1
  Menu_SuspendFieldTasks();
  Iwram_CopyWords(backup, (void *)BG_CHAR_BLOCK(1), size);
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), size, PIXELS_4BPP(3));
  Text_UseExtraTilesFar(1);
#endif

  Menu_StartWatchingStart();
  result = ItemMenu_RunCommands(&owner, &target, &item);
  Menu_StopWatchingStart();
  if (result == 1) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    // The item goes to the field to be used, as Field_UseItem reads it.
    work->item = (owner << ACT_UNIT_SHIFT) | (item & INVENTORY_ITEM_ID_MASK);
    work->itemSlot = screen->selectedSlots[0];
  }

  WindowSprite_ClearListFar(screen->statusWindow);
  ((struct WindowWork *)Ram_HeapSlots->windowWork)->holdTilemapUpload = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  Menu_ResetTwoResourceEntries();
  HeapSlot_Release(HEAP_SLOT_MENU);
  Ram_HeapSlots->actorSystem->suspended = 0;
#if TLA_PATCH_1
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), backup, size);
  ((struct WindowWork *)Ram_HeapSlots->windowWork)->holdTilemapUpload = 0;
  Heap_Release(backup);
  wait(1);
  Menu_ResumeFieldTasks();
#endif
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  ((struct WindowWork *)Ram_HeapSlots->windowWork)->holdTilemapUpload = 0;
  Party_ClearUnusableShortcutsFar();
  return result;
}

/// The resident IWRAM word copy, called through its address.
typedef s32 (*ItemWordCopyFn)(void *dst, const void *src, s32 size);

/// Iwram_CopyWords's routine reached from IwramIrqMain rather than at its
/// fixed address: the trade's snapshot copies call it this way.
#define ITEM_IWRAM_COPY_WORDS                                                  \
  ((ItemWordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))

/// The item command grid, by row * 3 + column: Use, Equip and Details on top,
/// Give, Remove and Drop below.
enum ItemMenuCommand {
  ITEM_COMMAND_USE,
  ITEM_COMMAND_EQUIP,
  ITEM_COMMAND_INSPECT, ///< Details
  ITEM_COMMAND_GIVE,
  ITEM_COMMAND_REMOVE,
  ITEM_COMMAND_DROP
};

/// The steps of the item menu's command loop (ItemMenu_RunCommands).
enum ItemMenuStep {
  ITEM_STEP_MEMBER = 0,
  ITEM_STEP_ITEM = 1,
  ITEM_STEP_USE = 2,
  ITEM_STEP_EQUIP = 3,
  ITEM_STEP_GIVE = 4,
  ITEM_STEP_DROP = 5,
  ITEM_STEP_TARGET = 6, ///< Give: choose whom to give it to
  ITEM_STEP_TRADE = 7,  ///< the receiver is full: choose what they give back
  ITEM_STEP_IDLE = 8,   ///< nothing sets it
  ITEM_STEP_COMMAND = 9,
  ITEM_STEP_INSPECT = 10, ///< Details (ItemMenu_ShowDetails)
  ITEM_STEP_REMOVE = 11,
  ITEM_STEP_QUANTITY = 12 ///< nothing sets it
};

#ifdef MODERN
// The item menu calls this with no prototype in scope, so its argument takes
// the old promotions; a current GCC, which refuses implicit declarations, gets
// it here.
void Unit_RefreshPsynergyFar(s32 unit);
#endif
void ItemMenu_DrawEquipPreview(s32 member, s32 slot, s32 mode, s32 target);
#if TLA_PATCH_2
void ItemMenu_HideTradedIcons(void);
#endif

/// Runs the item menu: choose a party member, one of their items and a command
/// (Use, Equip, Give, Drop, Remove or Details), then carry it out, until the
/// player backs out or presses Start. Returns 1 when an item is to be used
/// outside the menu, with its party member, target and item id in the out
/// parameters; -1 when the player backed out or pressed Start; 0 otherwise.
s32 ItemMenu_RunCommands(s32 *ownerOut, s32 *targetOut, s32 *itemOut) {
  s32 done;
  s32 selection;
  s32 menuResult;
  struct Unit *sourceUnit;
  struct Unit *targetUnit;
  struct ItemMenuWork *menu;
  struct ItemDefinition *item;
  void *sourceCopy;
  void *targetCopy;
  u32 state;
  s32 command;
  s32 result;
  s32 heldCount;
  s32 amount;
  s32 transferFailed;
  s32 removedCount;
  s32 giveTarget;
  u8 sourceQuantity;
  u8 targetQuantity;
  s8 stackSize;
  s32 size;

  done = 0;
  selection = 0;
  menuResult = 0;
  state = ITEM_STEP_MEMBER;
  menu = Ram_HeapSlots->menuWork;

  while (done == 0 && GameFlag_Get(FLAG_MENU_START_PRESSED) == 0) {
    switch (state) {
    // Choose whose items to look at.
    case ITEM_STEP_MEMBER:
      menu->selectedSlots[0] = 0;
      ItemMenu_FitMessageWindowToQuestion();
      ItemMenu_PlaceItemWindowBelowQuestion();
      menu->selectedItemIcon->motion = ICON_HIDDEN;
      ItemMenu_DrawMessage(0, (s32)&MsgWhoseItem);
#if TLA_PATCH_1
      // Japanese redraws the information window after each modal message
      // instead.
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
#endif
      Menu_DrawCoins((s32)menu->infoWindow);
      command = ItemMenu_SelectMember(0);
      if (command == -1) {
        // DEAD: selection and command cleared though the menu ends before
        // reading either; the ROM keeps the 0 and its store to selection.
        selection = command = 0;
        menuResult = -1;
        done = 1;
      }
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
      ItemMenu_HideAllIcons();
      state = ITEM_STEP_ITEM;
      break;

    // Choose one of their items.
    case ITEM_STEP_ITEM:
      if (ItemMenu_CountItems(menu->sideMember[0]) == 0) {
        state = ITEM_STEP_MEMBER;
        break;
      }
      ItemMenu_FitMessageWindowToQuestion();
      ItemMenu_PlaceItemWindowBelowQuestion();
      menu->selectedItemIcon->motion = ICON_HIDDEN;
      menu->sideIcons[0]->motion = ICON_SHOWN;
      ItemMenu_DrawMessage(0, (s32)&MsgWhichItem);
      selection = ItemMenu_RunList(0);
      state = ITEM_STEP_MEMBER;
      if (selection == -1) {
        break;
      }
      menu->listMode = -1;
      state = ITEM_STEP_COMMAND;
      break;

    // Choose what to do with it.
    case ITEM_STEP_COMMAND:
      command = ItemMenu_ChooseCommand();
      if (command == -1) {
        state = ITEM_STEP_ITEM;
        menu->completionFlag = 1;
      }
      if (command == ITEM_COMMAND_USE) {
        if (Scene_HasItemTriggerFar(menu->selectedItems[0] &
                                    INVENTORY_ITEM_ID_MASK) != 0) {
          done = 1;
          *ownerOut = menu->sideMember[0];
          *targetOut = command;
          *itemOut = menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK;
          menuResult = 1;
          break;
        }
        result =
            Item_ClassifyUseMode(menu->sideMember[0], menu->selectedItems[0]);
        if (result == ITEM_USE_MODE_CHOOSE_MEMBER) {
          state = ITEM_STEP_USE;
        }
        if (result == ITEM_USE_MODE_WHOLE_PARTY) {
          ItemMenu_Use();
          WindowSprite_ClearListFar(menu->infoWindow);
          ItemMenu_ShowMessage(menu->messageOffset + (s32)MsgItemUseResult, 0,
                               -1);
#if !TLA_PATCH_1
          WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
#endif
          menu->sideIcons[0]->motion = ICON_HIDDEN;
          menu->itemCount = ItemMenu_CollectItems(Unit_Get(menu->sideMember[0]),
                                                  menu->items, 0);
          ItemMenu_DrawIcons(menu->items, 0);
          state = ITEM_STEP_MEMBER;
        }
        if (result == ITEM_USE_MODE_UNUSABLE ||
            result == ITEM_USE_MODE_FROM_FIELD) {
          done = 1;
          *ownerOut = menu->sideMember[0];
          *targetOut = menu->sideMember[1];
          *itemOut = menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK;
          menuResult = 1;
        }
      }
      if (command == ITEM_COMMAND_EQUIP) {
        state = ITEM_STEP_EQUIP;
      }
      if (command == ITEM_COMMAND_GIVE) {
        state = ITEM_STEP_TARGET;
      }
      if (command == ITEM_COMMAND_DROP) {
        state = ITEM_STEP_DROP;
      }
      if (command == ITEM_COMMAND_REMOVE) {
        state = ITEM_STEP_REMOVE;
      }
      if (command == ITEM_COMMAND_INSPECT) {
        state = ITEM_STEP_INSPECT;
      }
      break;

    // Use the item on the party member chosen.
    case ITEM_STEP_USE:
      ItemMenu_HideAllIcons();
      ItemMenu_FitMessageWindowToItemQuestion();
      ItemMenu_PlaceItemWindowBelowItemQuestion();
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->questionWindow);
      ItemMenu_DrawItemHead();
      Text_DrawMessageAsTilesFar((s32)MsgUseOnWhom, menu->questionWindow, 16,
                                 16);
      if (ItemMenu_SelectTarget(ITEM_TARGET_USE) != -1) {
        command = 0;
        if (ItemMenu_IsStatRaisingItem(menu->selectedItems[0] &
                                       INVENTORY_ITEM_ID_MASK) != 0) {
          command = STATUS_PANEL_STATS;
        }
        result = ItemMenu_Use();
        StatusPanel_Draw(menu->statusWindow, menu->sideMember[1], 0, command);
        if (result != -1) {
          WindowSprite_ClearListFar(menu->infoWindow);
          ItemMenu_ShowMessage(menu->messageOffset + (s32)MsgItemUseResult, 0,
                               -1);
#if !TLA_PATCH_1
          WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
#endif
          menu->sideIcons[0]->motion = ICON_HIDDEN;
          ItemMenu_TryBreak();
          state = ITEM_STEP_ITEM;
        }
        menu->itemCount = ItemMenu_CollectItems(Unit_Get(menu->sideMember[0]),
                                                menu->items, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        menu->completionFlag = 1;
      } else {
        state = ITEM_STEP_COMMAND;
      }
      break;

    // Give: choose to whom.
    case ITEM_STEP_TARGET:
      ItemMenu_FitMessageWindowToItemQuestion();
      ItemMenu_PlaceItemWindowBelowItemQuestion();
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->questionWindow);
      ItemMenu_DrawItemHead();
      Text_DrawMessageAsTilesFar((s32)MsgGiveToWhom, menu->questionWindow, 16,
                                 16);
      giveTarget = ItemMenu_SelectTarget(ITEM_TARGET_GIVE);
      state = ITEM_STEP_GIVE;
      if (giveTarget == -1) {
        ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0],
                                  0, menu->sideMember[0]);
        state = ITEM_STEP_COMMAND;
      }
      break;

    // Drop the item, or some of a stack, after confirming.
    case ITEM_STEP_DROP:
      ItemMenu_HideAllIcons();
      item = Item_GetDirectFar(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
      result = 0;
      if ((item->flags & ITEM_STACKABLE) != 0) {
        heldCount = (menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
        if (heldCount > 1) {
          ItemMenu_DrawItemHead();
          result = ItemMenu_SelectGiveQuantity(0, heldCount, 1);
        }
      }
      if (result == -1) {
        state = ITEM_STEP_COMMAND;
        break;
      }
      menu->sideMember[1] = 0;
      Resource_LoadByModeIntoSlotFar(
          ICON_KIND_ITEM,
          (menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) |
              (result << INVENTORY_QUANTITY_SHIFT),
          (u8)menu->selectedItemIcon->index, 0);
      menu->selectedItemIcon->motion = ICON_SHOWN;
      menu->selectedItemIcon->x = 120;
      menu->selectedItemIcon->y = 28;
      Icon_PrepareActor(menu->selectedItemIcon);
      Window_ClearInteriorTilesFar(menu->listWindow, 0, 72, 120, 96);
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->questionWindow);
      // Yes: take result + 1 of the stack out, one at a time.
      if (ItemMenu_ConfirmDrop(selection) == 0) {
        command = menu->sideMember[0];
        // DEAD: Unit_Get's result is not used; the ROM keeps the call.
        Unit_Get(command);
        for (removedCount = 0; removedCount < result + 1; removedCount++) {
          Inventory_RemoveFar(command, menu->selectedSlots[0]);
          Item_AdjustCounterFar(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK,
                                1);
        }
        Unit_RefreshPsynergyFar(command);
        Unit_CalculateFar(command);
        ItemMenu_PlaceItemWindowBelowQuestion();
        ItemMenu_RefreshMember(menu->sideMember[0], 0);
        menu->selectedItemIcon->motion = ICON_HIDDEN;
        menu->sideIcons[0]->motion = ICON_HIDDEN;
        wait(1);
        WindowSprite_ClearListFar(menu->infoWindow);
        ItemMenu_ShowMessage((s32)MsgDroppedIt, 15, 13);
        menu->completionFlag = 1;
        state = ITEM_STEP_ITEM;
      } else {
        state = ITEM_STEP_COMMAND;
      }
      Unit_CalculateFar(menu->sideMember[0]);
      menu->selectedItemIcon->motion = ICON_HIDDEN;
      Party_ClearUnusableShortcutsFar();
      break;

    // Give the item, or some of a stack, to the chosen party member.
    case ITEM_STEP_GIVE:
      transferFailed = 0;
      item = Item_GetDirectFar(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
      if ((item->flags & ITEM_STACKABLE) != 0) {
        // A receiver with a full stack of it takes none; one with no free
        // slot and none of it must trade.
        heldCount = ItemMenu_GetItemQuantity(menu->sideMember[1],
                                             menu->selectedItems[0] &
                                                 INVENTORY_ITEM_ID_MASK);
        if (heldCount == INVENTORY_STACK_MAX) {
          transferFailed = 1;
        }
        if (ItemMenu_CountItems(menu->sideMember[1]) == INVENTORY_SLOTS &&
            heldCount == 0) {
          state = ITEM_STEP_TRADE;
          break;
        }
        stackSize = (menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
        if (transferFailed == 0) {
          if (heldCount + stackSize > INVENTORY_STACK_MAX) {
            amount = INVENTORY_STACK_MAX - heldCount;
          } else {
            amount = stackSize;
          }
          if (stackSize > 1) {
            result = ItemMenu_SelectGiveQuantity(0, amount, 0);
          } else {
            result = 0;
          }
          if (result == -1) {
            state = ITEM_STEP_TARGET;
            break;
          }
          // Move result + 1 one at a time; the broken bit goes with each,
          // the equipped bit and the quantity do not.
          for (command = 0; command < result + 1; command++) {
            heldCount = Inventory_AddItemFar(
                menu->sideMember[1],
                menu->selectedItems[0] &
                    (INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
            if (heldCount != -1) {
              Inventory_RemoveFar(menu->sideMember[0], menu->selectedSlots[0]);
              menu->selectedSlots[1] = heldCount;
            } else {
              transferFailed = 1;
            }
          }
        }
      } else {
        result = Inventory_AddItemFar(
            menu->sideMember[1],
            menu->selectedItems[0] &
                (INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
        if (result == -1) {
          state = ITEM_STEP_TRADE;
          break;
        }
        menu->selectedSlots[1] = result;
        result =
            Inventory_RemoveFar(menu->sideMember[0], menu->selectedSlots[0]);
        if (result == INVENTORY_REMOVE_EMPTY_SLOT) {
          transferFailed = 1;
        }
      }
      Unit_RefreshPsynergyFar(menu->sideMember[0]);
      Unit_RefreshPsynergyFar(menu->sideMember[1]);
      Unit_CalculateFar(menu->sideMember[0]);
      Unit_CalculateFar(menu->sideMember[1]);
      result = 1;
      if (transferFailed == 0) {
        menu->sideMember[0] = menu->sideMember[1];
        menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
#if TLA_PATCH_2
        ItemMenu_FitMessageWindowToLongItemQuestion();
#endif
        WindowSprite_RedrawSavedRectFar((struct Window *)menu->questionWindow);
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
      }
      if (GameFlag_Get(FLAG_MENU_START_PRESSED) != 0) {
        break;
      }
      ItemMenu_RefreshMember(menu->sideMember[1], 1);
      menu->sideIcons[0]->motion = ICON_HIDDEN;
      wait(1);
      // Say how it went: the receiver's stack was full, the item was given,
      // or it was given and equipped (ItemMenu_ConfirmEquip returned 0).
      if (transferFailed == 1) {
        WindowSprite_ClearListFar(menu->infoWindow);
        ItemMenu_ShowMessage((s32)MsgCannotHoldMore, 15, 14);
      } else {
        WindowSprite_ClearListFar(menu->infoWindow);
        if (result == 1) {
          ItemMenu_ShowMessage((s32)MsgGiven, 15, 14);
        } else {
          ItemMenu_DrawEquipPreview(menu->sideMember[1], menu->selectedSlots[1],
                                    0, menu->sideMember[1]);
          ItemMenu_ShowMessage((s32)MsgEquippedIt, 15, 14);
          item = Item_GetDirectFar(menu->selectedItems[0]);
          if ((item->flags & ITEM_CURSED) != 0) {
            Sound_PlayFar(SE_CURSE);
            WindowSprite_ClearListFar(menu->infoWindow);
            ItemMenu_ShowMessage((s32)MsgEquippedIt + 7, // "You were cursed!"
                                 ITEM_MODAL_SHORT_X, 14);
          }
        }
      }
      Party_ClearUnusableShortcutsFar();
      state = ITEM_STEP_MEMBER;
      break;

    // Trade: the receiver gives back one of their items.
    case ITEM_STEP_TRADE:
      transferFailed = 0;
      ItemMenu_FitMessageWindowToQuestion();
      ItemMenu_PlaceItemWindowBelowQuestion();
      ItemMenu_DrawMessage(0, (s32)&MsgSwapForWhat);
      if (ItemMenu_RunList(1) == -1) {
        state = ITEM_STEP_TARGET;
        break;
      }
      // Snapshot both inventories so any failed part of the trade can roll
      // back.
      sourceUnit = Unit_Get(menu->sideMember[0]);
      targetUnit = Unit_Get(menu->sideMember[1]);
      size = BATTLE_UNIT_SIZE;
      sourceCopy = Heap_Alloc(size);
      targetCopy = Heap_Alloc(size);
      ITEM_IWRAM_COPY_WORDS(sourceCopy, sourceUnit, size);
      ITEM_IWRAM_COPY_WORDS(targetCopy, targetUnit, size);
      // Take both whole stacks out, one item at a time (at most
      // INVENTORY_STACK_MAX): Inventory_RemoveFar returns INVENTORY_REMOVE_LAST
      // for the last one and INVENTORY_REMOVE_EMPTY_SLOT for an empty slot.
      for (sourceQuantity = 0; sourceQuantity < INVENTORY_STACK_MAX;
           sourceQuantity++) {
        result =
            Inventory_RemoveFar(menu->sideMember[0], menu->selectedSlots[0]);
        if (result == INVENTORY_REMOVE_LAST) {
          break;
        }
        if (result == INVENTORY_REMOVE_EMPTY_SLOT) {
          transferFailed = 1;
          break;
        }
      }
      sourceQuantity++;
      for (targetQuantity = 0; targetQuantity < INVENTORY_STACK_MAX;
           targetQuantity++) {
        // The receiver cannot trade away a worn item that will not come off.
        if ((menu->selectedItems[1] & INVENTORY_EQUIPPED) != 0) {
          item = Item_GetDirectFar(menu->selectedItems[1] &
                                   INVENTORY_ITEM_ID_MASK);
          if ((item->flags & ITEM_CANNOT_UNEQUIP) != 0) {
            transferFailed = 1;
          }
        }
        result =
            Inventory_RemoveFar(menu->sideMember[1], menu->selectedSlots[1]);
        if (result == INVENTORY_REMOVE_LAST) {
          break;
        }
        if (result == INVENTORY_REMOVE_EMPTY_SLOT) {
          transferFailed = 1;
          break;
        }
      }
      targetQuantity++;
      // Then give each the other's items.
      while (sourceQuantity != 0) {
        result = Inventory_AddItemFar(
            menu->sideMember[1],
            menu->selectedItems[0] &
                (INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
        if (result == -1) {
          transferFailed = 1;
          break;
        }
        menu->selectedSlots[1] = result;
        sourceQuantity--;
      }
      while (targetQuantity != 0) {
        result = Inventory_AddItemFar(
            menu->sideMember[0],
            menu->selectedItems[1] &
                (INVENTORY_ITEM_ID_MASK | INVENTORY_BROKEN));
        if (result == -1) {
          transferFailed = 1;
          break;
        }
        menu->selectedSlots[0] = result;
        targetQuantity--;
      }
      wait(1);
      if (transferFailed == 1) {
        Iwram_CopyWordsEntry(sourceUnit, sourceCopy, size);
        Iwram_CopyWordsEntry(targetUnit, targetCopy, size);
        WindowSprite_ClearListFar(menu->infoWindow);
        ItemMenu_ShowMessage((s32)MsgCannotTrade, ITEM_CANNOT_TRADE_X, 14);
#if LANGUAGE == LANGUAGE_FRENCH
        WindowSprite_RedrawSavedRectFar((struct Window *)menu->statusWindow);
#endif
      } else {
        Unit_RefreshPsynergyFar(menu->sideMember[0]);
        Unit_RefreshPsynergyFar(menu->sideMember[1]);
        Unit_CalculateFar(menu->sideMember[0]);
        Unit_CalculateFar(menu->sideMember[1]);
        ItemMenu_FitMessageWindowToItemQuestion();
#if TLA_PATCH_2
        ItemMenu_FitMessageWindowToLongItemQuestion();
        ItemMenu_HideTradedIcons();
#endif
        WindowSprite_RedrawSavedRectFar((struct Window *)menu->questionWindow);
        menu->sideMember[0] = menu->sideMember[1];
        menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
        if (GameFlag_Get(FLAG_MENU_START_PRESSED) == 0) {
          WindowSprite_ClearListFar(menu->infoWindow);
          ItemMenu_PlaceItemWindowBelowItemQuestion();
          ItemMenu_RefreshMember(menu->sideMember[1], 1);
          if (result == 0) {
            ItemMenu_DrawEquipPreview(menu->sideMember[1],
                                      menu->selectedSlots[1], 0,
                                      menu->sideMember[1]);
            ItemMenu_ShowMessage((s32)MsgEquippedIt, 15, 14);
            item = Item_GetDirectFar(menu->selectedItems[0]);
            if ((item->flags & ITEM_CURSED) != 0) {
              Sound_PlayFar(SE_CURSE);
              WindowSprite_ClearListFar(menu->infoWindow);
              ItemMenu_ShowMessage((s32)MsgEquippedIt + 7, // "You were cursed!"
                                   ITEM_MODAL_SHORT_X, 14);
            }
          } else {
            ItemMenu_ShowMessage((s32)MsgTraded, ITEM_TRADED_X, 14);
          }
        }
      }
      Heap_Release(targetCopy);
      Heap_Release(sourceCopy);
      Party_ClearUnusableShortcutsFar();
      state = ITEM_STEP_MEMBER;
      break;

    case ITEM_STEP_EQUIP:
      result = Inventory_EquipFar(menu->sideMember[0], menu->selectedSlots[0]);
      if (result == -1) {
        state = ITEM_STEP_ITEM;
        break;
      }
      if (result == -2) {
        WindowSprite_ClearListFar(menu->infoWindow);
        ItemMenu_ShowMessage((s32)MsgCannotRemoveItem, 0, -1);
#if !TLA_PATCH_1
        WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
#endif
        state = ITEM_STEP_ITEM;
        break;
      }
      Unit_RefreshPsynergyFar(menu->sideMember[0]);
      Unit_CalculateFar(menu->sideMember[0]);
      menu->sideIcons[0]->motion = ICON_HIDDEN;
      menu->itemCount =
          ItemMenu_CollectItems(Unit_Get(menu->sideMember[0]), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      wait(1);
      ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0], 0,
                                menu->sideMember[0]);
      WindowSprite_ClearListFar(menu->infoWindow);
      ItemMenu_ShowMessage((s32)MsgEquippedIt, 15, 8);
      item = Item_GetDirectFar(menu->selectedItems[0]);
      if ((item->flags & ITEM_CURSED) != 0) {
        Sound_PlayFar(SE_CURSE);
        WindowSprite_ClearListFar(menu->infoWindow);
        ItemMenu_ShowMessage((s32)MsgEquippedIt + 7, // "You were cursed!"
                             ITEM_MODAL_SHORT_X, 8);
      }
      state = ITEM_STEP_ITEM;
      break;

    case ITEM_STEP_REMOVE:
      (Unit_Get(menu->sideMember[0]))->inventory[menu->selectedSlots[0]] &=
          ~INVENTORY_EQUIPPED;
      Unit_RefreshPsynergyFar(menu->sideMember[0]);
      Unit_CalculateFar(menu->sideMember[0]);
      menu->sideIcons[0]->motion = ICON_HIDDEN;
      menu->itemCount =
          ItemMenu_CollectItems(Unit_Get(menu->sideMember[0]), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      wait(1);
      menu->equipPreview = 1;
      ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0], 0,
                                menu->sideMember[0]);
      menu->equipPreview = 0;
      WindowSprite_ClearListFar(menu->infoWindow);
      ItemMenu_ShowMessage((s32)MsgRemoved, ITEM_MODAL_REMOVED_X, 8);
      Party_ClearUnusableShortcutsFar();
      state = ITEM_STEP_ITEM;
      break;

    case ITEM_STEP_INSPECT:
      menu->sideIcons[0]->motion = ICON_HIDDEN;
      ItemMenu_ShowDetails(menu->selectedItems[0]);
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->statusWindow);
      ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0], 0,
                                menu->sideMember[0]);
      menu->sideIcons[0]->motion = ICON_SHOWN;
      state = ITEM_STEP_COMMAND;
      break;

    case ITEM_STEP_QUANTITY:
      ItemMenu_SelectGiveQuantity(0, INVENTORY_STACK_MAX, 0);
      state = ITEM_STEP_ITEM;
      break;

    case ITEM_STEP_IDLE:
      break;

    default:
      done = 1;
      break;
    }
  }

  if (GameFlag_Get(FLAG_MENU_START_PRESSED) != 0) {
    menuResult = -1;
  }
  return menuResult;
}

s32 Item_Use(s32 slot, s32 owner, s32 target);
s32 Item_PlayUseAnimation(s32 item);

/// Uses the selected item from the party member in side 0 on the one in side 1.
/// On failure plays the error sound, shows why in the info window and returns
/// -1; otherwise plays the item's animation, recalculates both party members
/// and returns 1.
s32 ItemMenu_Use(void) {
  struct ItemMenuWork *menu;
  s32 result;
  u32 item;
  u32 mask;

  menu = Ram_HeapSlots->menuWork;
  result = Item_Use(menu->selectedSlots[0], menu->sideMember[0],
                    menu->sideMember[1]);

  if (result == -1) {
    Sound_PlayFar(SE_MENU_ERROR);
    WindowSprite_ClearListFar(menu->infoWindow);
    ItemMenu_ShowMessage(menu->messageOffset + (s32)MsgItemUseResult, result,
                         result);
    // Until patch 1, which closes the message's window instead.
#if !TLA_PATCH_1
    WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
#endif
    menu->completionFlag = 1;
    return result;
  }

  item = menu->selectedItems[0];
  mask = INVENTORY_ITEM_ID_MASK;
  mask &= item;
  Item_PlayUseAnimation(mask);
  Unit_CalculateFar(menu->sideMember[0]);
  Unit_CalculateFar(menu->sideMember[1]);
  return 1;
}

/// Sets up the item menu's first screen: the cursor window with the party's
/// figures (MenuCursor_Create, Menu_PlaceOwnerEntryObjects), the info window
/// along the bottom and an 8 by 2 cursor grid. The parameters are not used.
void ItemMenu_Initialize(s32 x, s32 y, s32 mode, s32 columns) {
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuWork;
  s32 ownerIndex;

  Menu_PlaceOwnerEntryObjects(MenuCursor_Create(menu), 2, 2);
  for (ownerIndex = 0; ownerIndex < 4; ownerIndex++)
    menu->memberY[ownerIndex] = MENU_MEMBER_Y;

  {
    s32 style;
    menu->buttonHelpWindow = NULL;
    menu->statusWindow = NULL;
    style = 2;
    menu->infoWindow = (struct Window *)Window_CreateFar(0, 17, 30, 3, style);
    menu->itemWindow = NULL;
    menu->selectedColumn = 0;
    menu->selectedRow = 0;
    menu->columnCount = 8;
    menu->rowCount = style;
  }
}

/// Menu_SpawnIconEntries makes the icons a row of the eight-wide icon grid at
/// a time (columnCount), as the Psynergy screen places its own.
#define ICON_GRID_ROW 8

/// Creates the 32 item icon sprites in window, off screen until placed: slots 0
/// to 7 at MENU_ICON_HIDDEN_X, the rest 8 pixels further, all at
/// MENU_ICON_HIDDEN_Y, each showing item graphic slot of the same number.
void Menu_SpawnIconEntries(struct ItemMenuWork *menu, struct Window *window) {
  struct WindowSprite **firstIcons;
  s32 firstSlot;
  struct WindowSprite **secondIcons;
  s32 secondSlot;
  struct WindowSprite **remainingIcons;
  s32 remainingSlot;

  firstSlot = 0;
  {
    s32 y = MENU_ICON_HIDDEN_Y;

    firstIcons = &menu->entryIcons[0];
    for (; firstSlot < ICON_GRID_ROW; firstSlot++) {
      *firstIcons++ = WindowSprite_CreateFromResourceFar(
          ICON_KIND_ITEM, firstSlot, window, MENU_ICON_HIDDEN_X, y);
    }
  }

  secondSlot = ICON_GRID_ROW;
  {
    s32 y = MENU_ICON_HIDDEN_Y;

    secondIcons = &menu->entryIcons[ICON_GRID_ROW];
    for (; secondSlot < 2 * ICON_GRID_ROW; secondSlot++) {
      *secondIcons++ = WindowSprite_CreateFromResourceFar(
          ICON_KIND_ITEM, secondSlot, window, 256, y);
    }
  }

  remainingSlot = 2 * ICON_GRID_ROW;
  {
    s32 y = MENU_ICON_HIDDEN_Y;

    remainingIcons = &menu->entryIcons[2 * ICON_GRID_ROW];
    for (; remainingSlot < MENU_ENTRY_ICONS; remainingSlot++) {
      *remainingIcons++ = WindowSprite_CreateFromResourceFar(
          ICON_KIND_ITEM, remainingSlot, window, 256, y);
    }
  }
}

/// Hides all 32 item icons.
void ItemMenu_HideAllIcons(void) {
  struct WindowSprite **icons =
      ((struct ItemMenuWork *)Ram_HeapSlots->menuWork)->entryIcons;
  s32 slot;

  for (slot = 0; slot < MENU_ENTRY_ICONS; slot++) {
    struct WindowSprite *icon = *icons++;
    if (icon != 0) {
      icon->motion = ICON_HIDDEN;
    }
  }
}

#if TLA_PATCH_2
/// Hides the item icons in every fifth slot, 0 to 30, after a trade.
void ItemMenu_HideTradedIcons(void) {
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuWork;
  s32 slot;

  for (slot = 0; slot < MENU_ENTRY_ICONS; slot++) {
    struct WindowSprite *icon = menu->entryIcons[slot];
    if (icon != 0 && slot % 5 == 0)
      icon->motion = ICON_HIDDEN;
  }
}
#endif

#undef ITEM_IWRAM_COPY_WORDS

void Menu_ReleaseEntryObjects(void);

/// Closes the item menu: releases the party figures, hides the icons and the
/// status panel's portrait (the work's cursor), and closes every window the
/// menu opened.
void ItemMenu_Close(void) {
  struct ItemMenuWork *menu;
  struct WindowSprite *cursor;

  menu = Ram_HeapSlots->menuWork;
  Menu_ReleaseEntryObjects();
  ItemMenu_HideAllIcons();
  wait(1);
  cursor = menu->cursor;
  cursor->motion = ICON_HIDDEN;
  Window_CloseIfOpen((void **)&menu->mainWindow, 1);
  Window_CloseIfOpen((void **)&menu->itemWindow, 1);
  Window_CloseIfOpen((void **)&menu->questionWindow, 1);
  Window_CloseIfOpen((void **)&menu->statusWindow, 1);
  Window_CloseIfOpen((void **)&menu->buttonHelpWindow, 1);
  Window_CloseIfOpen((void **)&menu->infoWindow, 1);
  Window_CloseIfOpen((void **)&menu->equipWindow, 1);
  Window_CloseIfOpen((void **)&menu->listWindow, 1);
  Window_CloseIfOpen((void **)&menu->unopenedWindow, 1);
  Window_CloseIfOpen((void **)&menu->modalWindow, 1);
  Window_CloseIfOpen((void **)&menu->secondUnopenedWindow, 1);
}

s32 ItemMenu_CollectItems(struct Unit *owner, u16 *items, s32 mode);
s32 ItemMenu_RunMemberSelection(u16 *ownerIds, u16 *items);

/// Lets the player choose a party member in side (0 or 1), starting from the
/// party member chosen last.
s32 ItemMenu_SelectMember(s32 pane) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 result = 0;
  s32 index;
  void *owner;

  index = menu->sideIndex[pane];
  WindowSprite_ClearListFar(menu->infoWindow);
  menu->sideCount[pane] = menu->partyCount;
  if (index == -1) {
    menu->sideIndex[pane] = result;
    index = 0;
  } else {
    MenuCursor_Slide(index * 24 - 10, 16);
  }
  owner = Unit_Get((u16)menu->memberIds[index]);
  menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
  result = ItemMenu_RunMemberSelection(menu->memberIds, menu->items);
  Icon_PrepareActor(menu->sideIcons[pane]);
  wait(1);
  return result;
}

#if LANGUAGE == LANGUAGE_JAPANESE
/// `L+A:`, the key name Japanese draws before the arrange help.
extern const u8 gItemMenu_ArrangeKeysString[];
/// `R:`, the key name Japanese draws before the equipment help.
extern const u8 gItemMenu_EquipmentKeyString[];
#endif

void ItemMenu_OpenCategory(s32 ownerId);

void ItemMenu_RefreshMember(s32 owner, s32 mode);

s32 ItemMenu_CountItems(s32 owner);

/// The party member row of the item menu: moving shows each member's items and
/// status, holding R their equipment; L + A sorts the member's items, a new
/// order each press; A chooses a member who has items and B backs out. The
/// members are paged four at a time by Menu_MoveGridCursor, which moves the
/// selection and says what to redraw (2 for a new page). Returns the chosen
/// member's id, or -1.
s32 ItemMenu_RunMemberSelection(u16 *ownerIds, u16 *items) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 selection = menu->sideIndex[0];
  s32 count;
  s32 redraw;
  s32 result;
  u8 sortMode;
  s32 showEquipment;
  struct Unit *owner;
  void *window;

  count = menu->sideCount[0];
  redraw = 2;
  result = 0;
  sortMode = 0;
  showEquipment = 0;
  owner = Unit_Get(ownerIds[selection]);

  if (Window_UpdateOrCreate(&menu->itemWindow, 13, 3, 17, 10, redraw))
    Menu_SpawnIconEntries(menu, menu->itemWindow);
  if (Window_UpdateOrCreate(&menu->buttonHelpWindow, 13, 13, 17, 4, redraw)) {
    menu->selectedItemIcon = WindowSprite_CreateFromResourceFar(
        2, 0, menu->buttonHelpWindow, 0, result);
    menu->selectedItemIcon->motion = ICON_HIDDEN;
  }
#if LANGUAGE == LANGUAGE_JAPANESE
  // Japanese draws the key names as their own strings, the help beside them.
  Text_DrawStringInWindowFar(gItemMenu_ArrangeKeysString,
                             menu->buttonHelpWindow, 0, 0);
  Text_DrawMessageAsTilesFar((s32)MsgArrangeItemsHelp, menu->buttonHelpWindow,
                             32, 0);
  Text_DrawStringInWindowFar(gItemMenu_EquipmentKeyString,
                             menu->buttonHelpWindow, 16, 8);
  Text_DrawMessageAsTilesFar((s32)MsgArrangeItemsHelp + 1,
                             menu->buttonHelpWindow, 32, 8);
#else
  Text_DrawMessageAsTilesFar((s32)MsgArrangeItemsHelp, menu->buttonHelpWindow,
                             0, 0);
  Text_DrawMessageAsTilesFar((s32)MsgArrangeItemsHelp + 1,
                             menu->buttonHelpWindow, 0, 8);
#endif
  menu->sideIcons[0]->motion = ICON_SHOWN;
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      sortMode = 0;
      window = menu->statusWindow;
      owner = Unit_Get(ownerIds[selection]);
      if (showEquipment) {
        menu->itemCount = ItemMenu_CollectItems(Unit_Get(ownerIds[selection]),
                                                menu->items, 0);
        ItemMenu_OpenCategory(ownerIds[selection]);
        StatusPanel_Draw(window, ownerIds[selection], 0, 8);
      } else {
        ItemMenu_RefreshMember(ownerIds[selection], 0);
        StatusPanel_Draw(window, ownerIds[selection], 0, 0);
      }
      if (redraw == 2) {
        Menu_CreateOwnerEntryObjects(selection / 4 * 4);
        wait(1);
      }
      Menu_HighlightPageEntry(menu->mainWindow, selection, count);
    }
    MenuCursor_Position(selection % 4 * 24 - 10, 16);
    wait(1);
    if (gInput.pressed & KEY_A) {
      if (gInput.held & KEY_L) {
        sortMode = (sortMode + 4) % 4;
        ItemMenu_Sort(owner->inventory, sortMode);
        sortMode++;
        ItemMenu_RefreshMember(ownerIds[selection], 0);
        Sound_PlayFar(SE_MENU_CONFIRM);
      } else if (ItemMenu_CountItems(ownerIds[selection])) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        result = ownerIds[selection];
        break;
      } else {
        Sound_PlayFar(SE_MENU_ERROR);
      }
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gInput.pressed & KEY_R) {
      showEquipment = 1;
      redraw = 1;
    } else if (!(gInput.held & KEY_R) && showEquipment == 1) {
      showEquipment = 0;
      redraw = 1;
    } else {
      redraw = Menu_MoveGridCursor(&selection, count, 4);
    }
  }
  menu->sideIndex[0] = selection;
  menu->selectedMember = ownerIds[selection];
  menu->sideMember[0] = ownerIds[selection];
  menu->pageArrows[0]->motion = ICON_HIDDEN;
  menu->pageArrows[1]->motion = ICON_HIDDEN;
  return result;
}

/// Shows the equipment view of party member ownerId's items in the item
/// window.
void ItemMenu_OpenCategory(s32 ownerId) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;

  ItemMenu_PositionCategory();
  WindowSprite_RedrawSavedRectFar(menu->itemWindow);
  ItemMenu_DrawCategory(menu->itemWindow, ownerId, 0);
}

/// The Japanese release writes the item's description into the info window as
/// text, the others as tiles.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define TRADE_LABEL_X 16
#define IN_STOCK_LABEL_X 24
#define ItemMenu_DrawInfo Text_DrawMessageFar
#else
#define TRADE_LABEL_X 0
#define ItemMenu_DrawInfo Text_DrawMessageAsTilesFar
#endif

/// The Japanese release clears the info window; from patch 1 it redraws the
/// window's saved rectangle.
#if TLA_PATCH_1
#define ItemMenu_ClearInfo WindowSprite_RedrawSavedRectFar
#else
#define ItemMenu_ClearInfo WindowSprite_ClearListFar
#endif

#if (LANGUAGE == LANGUAGE_SPANISH)
#define IN_STOCK_LABEL_X 30
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define IN_STOCK_LABEL_X 29
#elif (LANGUAGE != LANGUAGE_JAPANESE)
#define IN_STOCK_LABEL_X 32
#endif

void EquipmentMenu_UpdateCompatibilityIndicators(void);

s32 ItemMenu_GetItemQuantity(s32 owner, s32 item);

s32 ItemMenu_IsStatRaisingItem(s32 item);

void EquipmentMenu_StopCompatibilityIndicators(void);

/// Chooses the party member to use or give the selected item (enum
/// ItemTargetMode) to.
s32 ItemMenu_SelectTarget(s32 mode) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 selection = menu->sideIndex[1];
  void *window = menu->itemWindow;
  s32 count = menu->partyCount;
  u8 result = 0;
  s32 descriptionShown = 0;
  s8 redraw;
  s32 heldCount;
  struct WindowSprite *marker;

  Window_SetBounds(window, 13, 5, 17, 12);
  WindowSprite_RedrawSavedRectFar(menu->itemWindow);
  // DEAD: Unit_Get's result is not used; the ROM keeps the call.
  Unit_Get(menu->memberIds[menu->sideIndex[0]]);
  Task_Add(EquipmentMenu_UpdateCompatibilityIndicators, TASK_PASS_FRAME);
  redraw = 2;
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      window = menu->itemWindow;
      // DEAD: Unit_Get's result is not used; the ROM keeps the call.
      Unit_Get(menu->memberIds[selection]);
      marker = menu->sideIcons[1];
      marker->sprite.oam.x = marker->x =
          ((selection * 3 + menu->mainWindow->x) << 3) - 2;
      if (redraw == 2) {
        Menu_CreateOwnerEntryObjects(selection / 4 * 4);
        wait(1);
      }
      Menu_HighlightPageEntry(menu->mainWindow, selection, count);
      if (mode == 1) {
        ItemMenu_RefreshMember(menu->memberIds[selection], 1);
        Window_DrawDividerLineFar(window, 0, 9, 16, 9);
        Window_ClearInteriorTilesFar(window, 0, 72, 120, 80);
        if (selection != menu->sideIndex[0]) {
          heldCount = ItemMenu_GetItemQuantity(menu->memberIds[selection],
                                               menu->selectedItems[0] &
                                                   INVENTORY_ITEM_ID_MASK);
          if (ItemMenu_CountItems(menu->memberIds[selection]) ==
                  INVENTORY_SLOTS &&
              heldCount == 0) {
            Text_DrawMessageAsTilesFar((s32)MsgTradeForWhat, window,
                                       TRADE_LABEL_X, 72);
          } else if (heldCount != 0) {
            Text_DrawNumberInWindowFar(heldCount, 2, window, 8, 72);
            Text_DrawMessageAsTilesFar((s32)MsgInStock, window,
                                       IN_STOCK_LABEL_X, 72);
          } else {
            Text_DrawMessageAsTilesFar((s32)MsgNoneInStock, window, 16, 72);
          }
        }
        ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0],
                                  0, menu->memberIds[selection]);
      }
      if (mode == 0) {
        if (ItemMenu_IsStatRaisingItem(menu->selectedItems[0] &
                                       INVENTORY_ITEM_ID_MASK))
          StatusPanel_Draw(menu->statusWindow, menu->memberIds[selection],
                           menu->selectedSlots[0], 8);
        else
          StatusPanel_Draw(menu->statusWindow, menu->memberIds[selection],
                           menu->selectedSlots[0], 0);
        if (!GameFlag_Get(FLAG_INFO_WINDOW_HOLDS_MESSAGE) &&
            !descriptionShown) {
          ItemMenu_ClearInfo(menu->infoWindow);
          ItemMenu_DrawInfo((menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
                                (s32)MsgItemPlainName,
                            menu->infoWindow, 0, 0);
          descriptionShown = 1;
        } else {
          GameFlag_Clear(FLAG_INFO_WINDOW_HOLDS_MESSAGE);
        }
      }
      redraw = 0;
    }
    MenuCursor_Position(selection % 4 * 24 - 10, 16);
    wait(1);
    if (gInput.pressed & KEY_A) {
      if (mode == 1 && selection == menu->sideIndex[0]) {
        Sound_PlayFar(SE_MENU_ERROR);
        continue;
      }
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = menu->memberIds[selection];
      break;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    redraw = Menu_MoveGridCursor(&selection, count, 4);
  }
  marker = menu->sideIcons[1];
  Icon_PrepareActor(marker);
  marker->motion = ICON_HIDDEN;
  EquipmentMenu_StopCompatibilityIndicators();
  wait(1);
  menu->sideIndex[1] = selection;
  menu->selectedMember = menu->memberIds[selection];
  menu->sideMember[1] = menu->memberIds[selection];
  selection = menu->sideIndex[0];
  count = menu->sideCount[0];
  Menu_CreateOwnerEntryObjects(selection / 4 * 4);
  Menu_HighlightPageEntry(menu->mainWindow, selection, count);
  menu->pageArrows[0]->motion = ICON_HIDDEN;
  menu->pageArrows[1]->motion = ICON_HIDDEN;
  return (s8)result;
}

#undef IN_STOCK_LABEL_X

/// Task while ItemMenu_SelectTarget chooses a party member to use or give the
/// item to, and while the command grid's cursor is on Give: every 32 frames,
/// has each party figure nod (ANIM_NOD) if that party member can equip the
/// item, or stand (ANIM_STAND) if not.
void EquipmentMenu_UpdateCompatibilityIndicators(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s8 i;

  if ((gFrameCount & 31) == 0) {
    for (i = 0; i <= 3; i++) {
      s32 owner = menu->memberSpriteIds[i];

      if (owner != -1) {
        if (Item_CanUnitEquip(owner,
                              menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK))
          Sprite_StartAnimationFar(menu->memberSprites[i], ANIM_NOD);
        else
          Sprite_StartAnimationFar(menu->memberSprites[i], ANIM_STAND);
      }
    }
  }
}

/// Ends EquipmentMenu_UpdateCompatibilityIndicators: puts every party figure
/// back to ANIM_STAND and removes the task.
void EquipmentMenu_StopCompatibilityIndicators(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s8 i;

  for (i = 0; i < menu->partyCount; i++)
    Sprite_StartAnimationFar(menu->memberSprites[i], ANIM_STAND);
  Task_Remove(EquipmentMenu_UpdateCompatibilityIndicators);
}

/// Returns 1 for the items that raise a stat for good (Apple, Hard Nut, Mint
/// and Lucky Pepper), whose use shows the stats panel instead of HP and PP.
s32 ItemMenu_IsStatRaisingItem(s32 itemId) {
  switch (itemId) {
  case ITEM_APPLE:
  case ITEM_HARD_NUT:
  case ITEM_MINT:
  case ITEM_LUCKY_PEPPER:
    return 1;
  }
  return 0;
}

/// Clears the message window and writes message into it.
void ItemMenu_DrawMessage(s32 unused, s32 message) {
  struct ItemMenuWork *menu;

  menu = Ram_HeapSlots->menuWork;
  WindowSprite_RedrawSavedRectFar(menu->questionWindow);
  Text_DrawMessageAsTilesFar(message, (s32)menu->questionWindow, 0, 0);
}

/// Hides the icon of each of the 32 list entries whose item is 0. Each of the
/// 32 icon slots must hold a sprite (Menu_SpawnIconEntries).
void Menu_HideEmptyEntryIcons(const u16 *items) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 slot;

  for (slot = 0; slot < MENU_ENTRY_ICONS; slot++) {
    if (items[slot] == 0) {
      Icon_PrepareActor(menu->entryIcons[slot]);
      menu->entryIcons[slot]->motion = ICON_HIDDEN;
    }
  }
}

/// Counts the nonempty slots in the owner's inventory.
s32 ItemMenu_CountItems(s32 ownerId) {
  s32 count = 0;
  struct Unit *state = Unit_Get(ownerId);
  s32 i;

  for (i = 0; i < INVENTORY_SLOTS; i++) {
    s32 itemId = state->inventory[i] & INVENTORY_ITEM_ID_MASK;

    if (itemId != 0)
      count++;
  }
  return count;
}

/// Returns how many of item party member owner holds, adding up every stack
/// of it in the inventory.
s32 ItemMenu_GetItemQuantity(s32 owner, s32 item) {
  struct Unit *state;
  s32 i;
  s32 quantity;

  quantity = 0;
  state = Unit_Get(owner);
  for (i = 0; i < INVENTORY_SLOTS; i++) {
    if (state->inventory[i] != 0 &&
        (state->inventory[i] & INVENTORY_ITEM_ID_MASK) == item) {
      quantity += (state->inventory[i] & INVENTORY_QUANTITY_MASK) >>
                  INVENTORY_QUANTITY_SHIFT;
      quantity++;
    }
  }
  return quantity;
}

s32 ItemMenu_CollectItems(struct Unit *owner, u16 *items, s32 mode) {
  s32 count;
  s32 i;

  for (i = 0; i < MENU_ENTRY_ICONS; i++) {
    items[i] = 0;
  }
  count = 0;
  for (i = 0; i < INVENTORY_SLOTS; i++) {
    items[i] = 0;
    if (owner->inventory[i] != 0) {
      items[count] = owner->inventory[i];
      count++;
    }
  }
  return count;
}

/// Loads each listed item's icon into its item icon sprite, the first 15
/// entries of inventoryEntries, and hides the icons of empty entries. iconStyle
/// 0 draws the list's icons (ICON_KIND_ITEM); any other iconStyle the
/// equipment view's.
void ItemMenu_DrawIcons(u16 *inventoryEntries, s32 iconStyle) {
  s32 i;
  u16 *entries;
  struct WindowSprite **icons;
  s32 itemId;

  icons = ((struct ItemMenuWork *)Ram_HeapSlots->menuWork)->entryIcons;
  entries = inventoryEntries;
  for (i = 0; i < INVENTORY_SLOTS; i++) {
    itemId = *entries++;
    if (itemId != 0) {
      if (iconStyle == 0) {
        Resource_LoadByModeIntoSlotFar(ICON_KIND_ITEM, itemId,
                                       (u8)(*icons)->index, 0);
      } else {
        Resource_LoadByModeIntoSlotFar(ICON_KIND_EQUIPMENT_ITEM, itemId,
                                       (u8)(*icons)->index, 0);
      }
    }
    icons++;
  }
  Menu_HideEmptyEntryIcons(inventoryEntries);
}

/// Shows party member ownerId's items in the item window, laid out for layout
/// mode (see ItemMenu_PlaceItemIcons), or "no items" when there are none.
void ItemMenu_RefreshMember(s32 ownerId, s32 mode) {
  struct ItemMenuWork *menu;
  struct Unit *owner;
  u16 *items;

  menu = Ram_HeapSlots->menuWork;
  owner = Unit_Get(ownerId);
  items = menu->items;
  menu->itemCount = ItemMenu_CollectItems(owner, items, 0);
  WindowSprite_RedrawSavedRectFar(menu->itemWindow);
  ItemMenu_PlaceItemIcons(mode);
  ItemMenu_DrawIcons(items, 0);
  if (ItemMenu_CountItems(ownerId) == 0)
    Text_DrawMessageAsTilesFar((s32)MsgItemMenuEmpty, menu->itemWindow, 8, 24);
}

/// Shows in the status panel what the item in owner's inventory slot would mean
/// to party member target: for a usable item, target's status; for equipment,
/// target's stats with it equipped; for a Psynergy item, the Psynergy it
/// teaches.
void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 style = 0;
  struct Unit *unit = Unit_Get(owner);
  s32 entry = unit->inventory[slot];
  s32 previewSlot;
  void *buffer;
  struct ItemDefinition *item =
      Item_GetDirectFar(entry & INVENTORY_ITEM_ID_MASK);

  if (mode == 1)
    style = STATUS_PANEL_LOWER_ONLY;
  switch (item->type) {
  case ITEM_TYPE_NONE: // usable items
    StatusPanel_Draw(menu->statusWindow, target, slot, style);
    break;
  case ITEM_TYPE_ELEMENTAL_STAR:
    StatusPanel_Draw(menu->statusWindow, target, slot, style);
    break;
  case ITEM_TYPE_CLASS_CHANGE:
    if (owner == target) {
      style |= STATUS_PANEL_EQUIP_CLASS;
      StatusPanel_Draw(menu->statusWindow, target, slot, style);
    } else {
      s32 size;

      unit = Unit_Get(target);
      size = BATTLE_UNIT_SIZE;
      buffer = Heap_Alloc(size);
      Iwram_CopyWordsEntry(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        entry &= ~INVENTORY_EQUIPPED;
        previewSlot = Inventory_AddItemFar(target, entry);
        if (previewSlot != -1) {
          style |= STATUS_PANEL_EQUIP_CLASS;
          StatusPanel_Draw(menu->statusWindow, target, previewSlot, style);
        } else {
          StatusPanel_Draw(menu->statusWindow, target, slot, style);
        }
      } else {
        StatusPanel_Draw(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWordsEntry(unit, buffer, size);
      Heap_Release(buffer);
    }
    break;
  case ITEM_TYPE_WEAPON:
  case ITEM_TYPE_ARMOR:
  case ITEM_TYPE_SHIELD:
  case ITEM_TYPE_HELM:
  case ITEM_TYPE_BOOTS:
  case ITEM_TYPE_TRIDENT:
  case ITEM_TYPE_RING:
  case ITEM_TYPE_SHIRT:
    if (owner == target) {
      style |= STATUS_PANEL_EQUIP;
      StatusPanel_Draw(menu->statusWindow, target, slot, style);
    } else {
      s32 size;

      unit = Unit_Get(target);
      size = BATTLE_UNIT_SIZE;
      buffer = Heap_Alloc(size);
      Iwram_CopyWordsEntry(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        entry &= ~INVENTORY_EQUIPPED;
        previewSlot = Inventory_AddItemFar(target, entry);
        if (previewSlot != -1) {
          style |= STATUS_PANEL_EQUIP;
          StatusPanel_Draw(menu->statusWindow, target, previewSlot, style);
        } else {
          StatusPanel_Draw(menu->statusWindow, target, slot, style);
        }
      } else {
        StatusPanel_Draw(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWordsEntry(unit, buffer, size);
      Heap_Release(buffer);
    }
    break;
  case ITEM_TYPE_PSYNERGY:
    if (target == owner) {
      style |= STATUS_PANEL_LEARN;
      StatusPanel_Draw(menu->statusWindow, target, slot, style);
    } else {
      s32 size;

      unit = Unit_Get(target);
      size = BATTLE_UNIT_SIZE;
      buffer = Heap_Alloc(size);
      Iwram_CopyWordsEntry(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        previewSlot = Inventory_AddItemFar(target, entry);
        if (previewSlot != -1) {
          style |= STATUS_PANEL_LEARN;
          StatusPanel_Draw(menu->statusWindow, target, previewSlot, style);
        } else {
          StatusPanel_Draw(menu->statusWindow, target, slot, style);
        }
      } else {
        StatusPanel_Draw(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWordsEntry(unit, buffer, size);
      Heap_Release(buffer);
    }
    break;
  }
}

/// Makes room in memberId's inventory: if no slot is free, removes the whole
/// stack in the first slot that is not equipped.
s32 Inventory_RemoveFirstUnflagged(s32 memberId) {
  struct Unit *unit = Unit_Get(memberId);
  s32 result = 0;
  s32 slotIndex;

  for (slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++) {
    if (unit->inventory[slotIndex] == 0) {
      result = 1;
      break;
    }
    if ((unit->inventory[slotIndex] & INVENTORY_EQUIPPED) == 0) {
      s32 quantity = unit->inventory[slotIndex] >> INVENTORY_QUANTITY_SHIFT;
      s32 count = quantity + 1;
      s32 removedCount;

      if (quantity == 0)
        count = 1;
      for (removedCount = 0; removedCount < count; removedCount++)
        result = Inventory_RemoveFar(memberId, slotIndex);
      if (result != INVENTORY_REMOVE_LAST)
        return 0;
      result = 1;
      break;
    }
  }
  return result;
}

/// Where the cursor points at a command. The German, French and Spanish
/// releases ask two helpers of their own: the first reads x from a table that
/// sets the middle column 9 pixels left of the even 40-pixel spacing the other
/// releases compute, the second gives y.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) ||          \
    (LANGUAGE == LANGUAGE_SPANISH)
s32 ItemMenu_GetCommandCursorX(s32 column, s32 row);
s32 ItemMenu_GetCommandCursorY(s32 column, s32 row);
#define COMMAND_CURSOR_X(column, row) ItemMenu_GetCommandCursorX(column, row)
#define COMMAND_CURSOR_Y(column, row) ItemMenu_GetCommandCursorY(column, row)
#else
#define COMMAND_CURSOR_X(column, row) ((column) * 40 + 86)
#define COMMAND_CURSOR_Y(column, row) ((row) * 8 + 30)
#endif

void ItemMenu_BuildCommands(s8 *commandStates);

void ItemMenu_FitMessageWindowToItemChoices(void);

void ItemMenu_DrawItemHead(void);
void ItemMenu_DrawCommands(const s8 *commandStates, s32 window);

/// The item command grid: Use, Equip and Details on the top row, Give, Remove
/// and Drop below (grid index = row * 3 + column, as ItemMenuCommand counts
/// them).
s32 ItemMenu_ChooseCommand(void) {
  struct ItemMenuWork *menu;
  s8 commandStates[6];
  u16 *redrawFlag;
  s32 column;
  s32 lastCommand;
  s32 row;
  s32 commandIndex;
  s32 redraw;

  menu = Ram_HeapSlots->menuWork;
  commandIndex = 0;
  column = 0;
  row = 0;
  redraw = 1;
  ItemMenu_BuildCommands(commandStates);
  redrawFlag = &menu->flags;
  if (*redrawFlag != 1) {
    void *messageWindow;

    ItemMenu_HideAllIcons();
    WindowSprite_RedrawSavedRectFar(menu->listWindow);
    messageWindow = menu->questionWindow;
    ItemMenu_FitMessageWindowToItemChoices();
    WindowSprite_RedrawSavedRectFar(messageWindow);
    Window_DrawDividerLineFar(messageWindow, 0, 3, 16, 3);
    ItemMenu_DrawItemHead();
    ItemMenu_DrawCommands(commandStates, messageWindow);
  }
  *redrawFlag = 0;

  lastCommand = menu->listMode;
  if (lastCommand == -1) {
    if (commandStates[ITEM_COMMAND_INSPECT] == ITEM_COMMAND_STATE_AVAILABLE) {
      column = 2;
      row = 0;
    }
    if (commandStates[ITEM_COMMAND_GIVE] == ITEM_COMMAND_STATE_AVAILABLE) {
      column = 0;
      row = 1;
    }
    if (commandStates[ITEM_COMMAND_EQUIP] == ITEM_COMMAND_STATE_AVAILABLE) {
      column = 1;
      row = 0;
    }
    if (commandStates[ITEM_COMMAND_REMOVE] == ITEM_COMMAND_STATE_AVAILABLE) {
      column = 1;
      row = 1;
    }
    if (commandStates[ITEM_COMMAND_USE] == ITEM_COMMAND_STATE_AVAILABLE) {
      column = 0;
      row = 0;
    }
  } else {
    column = (s8)(lastCommand % 3);
    row = (s8)(lastCommand / 3);
    commandIndex = row * 3 + column;
  }
  MenuCursor_Slide(COMMAND_CURSOR_X(column, row),
                   COMMAND_CURSOR_Y(column, row));

  for (;;) {
    if (GameFlag_Get(FLAG_MENU_START_PRESSED) != 0)
      break;
    if (redraw != 0) {
      redraw = 0;
      column = (column + 3) % 3;
      row = (row + 2) % 2;
      commandIndex = row * 3 + column;
      EquipmentMenu_StopCompatibilityIndicators();
      if (commandIndex > 2) {
        menu->equipPreview = 1;
        ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0],
                                  0, menu->sideMember[0]);
        if (commandIndex == ITEM_COMMAND_GIVE)
          Task_Add(EquipmentMenu_UpdateCompatibilityIndicators,
                   TASK_PASS_FRAME);
      } else if (commandIndex != 0) {
        menu->equipPreview = 0;
        ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0],
                                  0, menu->sideMember[0]);
      } else {
        StatusPanel_Draw(menu->statusWindow, menu->sideMember[0], 0,
                         STATUS_PANEL_EXPERIENCE);
      }
    }
    MenuCursor_Position(COMMAND_CURSOR_X(column, row),
                        COMMAND_CURSOR_Y(column, row));
    wait(1);

    if ((gInput.pressed & KEY_A) != 0) {
      if (commandStates[commandIndex] == -1) {
        Sound_PlayFar(SE_MENU_ERROR);
      } else {
        switch (commandIndex) {
        case ITEM_COMMAND_USE:
          Sound_PlayFar(SE_NOTE_MID);
          break;
        case ITEM_COMMAND_EQUIP:
          Sound_PlayFar(SE_NOTE_HIGH);
          break;
        case ITEM_COMMAND_INSPECT:
        case ITEM_COMMAND_GIVE:
        case ITEM_COMMAND_DROP:
          Sound_PlayFar(SE_MENU_CONFIRM);
          break;
        case ITEM_COMMAND_REMOVE:
          Sound_PlayFar(SE_ITEM_REMOVE);
          break;
        default:
          Sound_PlayFar(SE_MENU_CONFIRM);
          break;
        }
        menu->listMode = commandIndex;
        break;
      }
    }
    if ((gInput.pressed & KEY_B) != 0) {
      Sound_PlayFar(SE_MENU_CANCEL);
      commandIndex = -1;
      menu->listMode = commandIndex;
      break;
    }

    if ((gInput.repeat & KEY_UP) != 0) {
      row -= 1;
      redraw = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    } else if ((gInput.repeat & KEY_DOWN) != 0) {
      row += 1;
      redraw = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    } else if ((gInput.repeat & KEY_RIGHT) != 0) {
      column += 1;
      redraw = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    } else if ((gInput.repeat & KEY_LEFT) != 0) {
      column -= 1;
      redraw = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
  }

  menu->equipPreview = 0;
  EquipmentMenu_StopCompatibilityIndicators();
  return commandIndex;
}

#undef COMMAND_CURSOR_Y

s32 Item_ClassifyUseMode(s32 owner, s32 item);
/// Returns whether the scene has an event for item on what the leader faces.
s32 Scene_HasItemTriggerFar(s32 item);

/// Fills commandStates (indexed by ItemMenuCommand) with
/// ITEM_COMMAND_STATE_AVAILABLE or _DISABLED for the selected item: Use for
/// items Item_ClassifyUseMode accepts and are not broken, and for field items,
/// Equip for equipment the party member can wear and is not wearing, Remove for
/// worn equipment that is not cursed, Give with someone to give to, Drop unless
/// the item is indispensable, and neither for cursed equipment being worn.
/// Details is always available. Each later test overrides the earlier ones.
void ItemMenu_BuildCommands(s8 *commandStates) {
  struct ItemMenuWork *menu;
  struct ItemDefinition *item;

  menu = Ram_HeapSlots->menuWork;
  item = Item_GetDirectFar(INVENTORY_ITEM_ID_MASK & menu->selectedItems[0]);

  if (item->type == 0) {
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;
    commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_DISABLED;
  } else {
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;
    commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_AVAILABLE;
  }

  if (Item_ClassifyUseMode(menu->sideMember[0], menu->selectedItems[0]) != -1)
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;
  else
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;

  if (menu->selectedItems[0] & INVENTORY_BROKEN)
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;

  if (Item_CanUnitEquip(menu->sideMember[0],
                        menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) == 0) {
    commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_DISABLED;
  }

  commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_AVAILABLE;
  commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_AVAILABLE;
  commandStates[ITEM_COMMAND_INSPECT] = ITEM_COMMAND_STATE_AVAILABLE;

  if (menu->selectedItems[0] & INVENTORY_EQUIPPED) {
    commandStates[ITEM_COMMAND_REMOVE] = ITEM_COMMAND_STATE_AVAILABLE;
    commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_DISABLED;
  } else {
    commandStates[ITEM_COMMAND_REMOVE] = ITEM_COMMAND_STATE_DISABLED;
  }

  if (item->flags & ITEM_CANNOT_UNEQUIP) {
    commandStates[ITEM_COMMAND_REMOVE] = ITEM_COMMAND_STATE_DISABLED;
    if (menu->selectedItems[0] & INVENTORY_EQUIPPED) {
      commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_DISABLED;
      commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_DISABLED;
    }
  }

  if (Scene_HasItemTriggerFar(menu->selectedItems[0] &
                              INVENTORY_ITEM_ID_MASK) != 0)
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;

  if (menu->partyCount <= 1)
    commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_DISABLED;

  if (item->flags & ITEM_INDISPENSABLE)
    commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_DISABLED;
}

/// The x of the command grid's middle column: German, Spanish and French
/// draw Equip and Remove 8 pixels further left.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH)
#define ITEM_COMMAND_TEXT_X 32
#else
#define ITEM_COMMAND_TEXT_X 40
#endif

#define TEXT_COLOR_DISABLED                                                    \
  TEXT_COLOR_GREY ///< the grey of a command that refuses A

/// Writes the six command names into window, the disabled ones in grey.
void ItemMenu_DrawCommands(const s8 *commandStates, s32 window) {
  s32 useState;
  u32 firstCommandMessage;

  Text_SetColorFar(TEXT_COLOR_NORMAL);
  useState = commandStates[ITEM_COMMAND_USE];
  if (useState == ITEM_COMMAND_STATE_DISABLED)
    Text_SetColorFar(TEXT_COLOR_DISABLED);

  firstCommandMessage = MsgItemCommandUse;
  Text_DrawMessageAsTilesFar(firstCommandMessage, window, 0, 24); // "Use"
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_EQUIP] == ITEM_COMMAND_STATE_DISABLED)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  Text_DrawMessageAsTilesFar(firstCommandMessage + 1, window,
                             ITEM_COMMAND_TEXT_X,
                             24); // "Equip"
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_GIVE] == ITEM_COMMAND_STATE_DISABLED)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  Text_DrawMessageAsTilesFar(firstCommandMessage + 2, window, 0,
                             32); // "Give"
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_DROP] == ITEM_COMMAND_STATE_DISABLED)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  Text_DrawMessageAsTilesFar(firstCommandMessage + 3, window, 80,
                             32); // "Drop"
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_INSPECT] == ITEM_COMMAND_STATE_DISABLED)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  Text_DrawMessageAsTilesFar(firstCommandMessage + 4, window, 80,
                             24); // "Details"
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_REMOVE] == ITEM_COMMAND_STATE_DISABLED)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  // "Remove"
  Text_DrawMessageAsTilesFar(firstCommandMessage + 5, window,
                             ITEM_COMMAND_TEXT_X, 32);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
}

/// Says how memberId can use inventoryEntry from the item menu.
s32 Item_ClassifyUseMode(s32 memberId, s32 inventoryEntry) {
  s32 itemId = inventoryEntry;
  struct ItemDefinition *item;
  s32 result = ITEM_USE_MODE_UNUSABLE;

  itemId &= INVENTORY_ITEM_ID_MASK;
  item = Item_GetDirectFar(itemId);

  if (Scene_HasItemTriggerFar(itemId) != 0) {
    return ITEM_USE_MODE_FROM_FIELD;
  }

  {
    struct AbilityDefinition *action = (struct AbilityDefinition *)Ability_Get(
        item->abilityId & PSYNERGY_ABILITY_MASK);

    if (item->abilityId != 0) {
      if (item->type != ITEM_TYPE_NONE) {
        if (item->useType != ITEM_USE_TEACHES) {
          if (Item_CanUnitEquip(memberId, itemId) != 0) {
            result = ITEM_USE_MODE_CHOOSE_MEMBER;
          }
        }
      } else {
        result = ITEM_USE_MODE_CHOOSE_MEMBER;
      }

      if (result == ITEM_USE_MODE_CHOOSE_MEMBER) {
        s32 mask = ABILITY_USABLE_IN_BATTLE;
        u8 flags = action->flags;

        if ((flags & ABILITY_USABLE_IN_FIELD) != 0) {
          u8 range = action->range;
          result = (range == ABILITY_RANGE_ALL) ? ITEM_USE_MODE_WHOLE_PARTY
                                                : ITEM_USE_MODE_CHOOSE_MEMBER;
        } else {
          result = (flags & mask) ? ITEM_USE_MODE_UNUSABLE
                                  : ITEM_USE_MODE_FROM_FIELD;
        }
      }
    }
  }

  return result;
}

void Inventory_BreakFar(s32 owner, s32 slot);

/// After the selected item is used: an item that may break (ITEM_USE_MAY_BREAK)
/// breaks one time in eight, with the break sound and "It broke!" in the info
/// window. Called only after a successful use on a chosen party member.
void ItemMenu_TryBreak(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  u32 item = menu->selectedItems[0];
  u32 mask;

  mask = INVENTORY_ITEM_ID_MASK;
  mask &= item;
  // One time in eight: 8192 of Random16's 65536 values.
  if (Item_GetDirectFar(mask)->useType == ITEM_USE_MAY_BREAK &&
      Random16() < 8192) {
    Inventory_BreakFar(menu->sideMember[0], menu->selectedSlots[0]);
    Sound_PlayFar(SE_ITEM_BREAK);
    ItemMenu_ShowMessage((s32)MsgItBroke, 0, -1);
    WindowSprite_RedrawSavedRectFar(menu->infoWindow);
  }
}

#undef ITEM_COMMAND_TEXT_X

void ItemMenu_DrawItemDetails(s32 window, s32 item);

/// Opens the details window (top left, 13 by 10 tiles) and describes the item
/// in side itemIndex there. Returns 1.
s32 ItemMenu_OpenDetail(s32 itemIndex) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;

  Window_UpdateOrCreate(&menu->equipWindow, 0, 0, 13, 10, WINDOW_FLAG_INSTANT);
  Palette_LoadIconsIntoHighlight();

  if (menu->selectedItems[itemIndex] != 0)
    ItemMenu_DrawItemDetails(menu->equipWindow, menu->selectedItems[itemIndex]);

  return 1;
}

/// The Details window's width in tiles: Japanese opens it over the left part
/// only, beside the item window, the other editions over the whole top half.
#if LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_DETAILS_WINDOW_WIDTH 13
#else
#define ITEM_DETAILS_WINDOW_WIDTH 30
#endif

void Window_CloseFar(s32, s32);

/// The item menu's Details screen: describes the item entry value in a window
/// over the top half (ITEM_DETAILS_WINDOW_WIDTH) and waits for A or B. Up and
/// Down move an unused counter from 0 to 4. Returns the counter after A or
/// Start, or -1 after B.
s32 ItemMenu_ShowDetails(s32 value) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 changed = 1;
#if LANGUAGE != LANGUAGE_JAPANESE
  struct WindowSprite *itemIcon = menu->selectedItemIcon;
#endif
  s32 window;
  s32 counter = 0;

#if LANGUAGE != LANGUAGE_JAPANESE
  itemIcon->motion = ICON_HIDDEN;
#endif
  window = Window_CreateFar(0, 0, ITEM_DETAILS_WINDOW_WIDTH, 10,
                            WINDOW_FLAG_INSTANT);
  Task_Remove(Menu_UpdateEntryObjectTransforms);

  {
    struct WindowSprite *cursor = menu->cursor;
    cursor->motion = ICON_HIDDEN;
  }
  Palette_LoadIconsIntoHighlight();
  wait(1);

  for (;;) {
    if (GameFlag_Get(FLAG_MENU_START_PRESSED) != 0)
      break;

    if (changed != 0) {
      changed = 0;
      counter = (counter + 5) % 5;
      ItemMenu_DrawItemDetails(window, value);
    }

    if (gInput.pressed & KEY_A)
      break;
    if (gInput.pressed & KEY_B) {
      counter = -1;
      break;
    }

    if (gInput.repeat & KEY_UP) {
      counter -= 1;
      changed = 1;
    }
    if (gInput.repeat & KEY_DOWN) {
      counter += 1;
      changed = 1;
    }
    wait(1);
  }

  WindowSprite_RedrawSavedRectFar(window);
  wait(1);
  Window_CloseFar(window, 1);
  WindowSprite_RedrawSavedRectFar(menu->mainWindow);
  Window_DrawDividerLineFar(menu->mainWindow, 0, 3, 12, 3);
  Palette_LightenHighlight(HIGHLIGHT_PALETTE);
  {
    s32 priority = TASK_PASS_FRAME;

    Task_Add(Menu_UpdateEntryObjectTransforms, priority);
  }

  {
    struct WindowSprite *cursor = menu->cursor;
    cursor->motion = ICON_SHOWN;
  }
#if LANGUAGE == LANGUAGE_JAPANESE
  menu->flags = 1;
#else
  Window_DrawBorderFar(13, 0, 17, 10);
#endif

  return counter;
}

#undef ITEM_DETAILS_WINDOW_WIDTH

void ItemMenu_DrawItemDetailsWithCount(s32 window, s32 item, s32 count);

/// Describes item in window with the entry's own count, through
/// ItemMenu_DrawItemDetailsWithCount.
void ItemMenu_DrawItemDetails(s32 window, s32 item) {
  ItemMenu_DrawItemDetailsWithCount(window, item, -1);
}

/// The headings are indented in English and Japanese; German, Spanish and
/// Italian start all three at the window's left edge, French the uses heading
/// only.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define ITEM_DETAILS_HEADING_X 0
#define ITEM_DETAILS_USES_X 0
#define ITEM_DETAILS_NUMBER_X 0
#elif LANGUAGE == LANGUAGE_FRENCH
#define ITEM_DETAILS_HEADING_X 8
#define ITEM_DETAILS_USES_X 0
#define ITEM_DETAILS_NUMBER_X 16
#elif LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_DETAILS_HEADING_X 16
#define ITEM_DETAILS_USES_X 16
#define ITEM_DETAILS_NUMBER_X 16
#else
#define ITEM_DETAILS_HEADING_X 8
#define ITEM_DETAILS_USES_X 16
#define ITEM_DETAILS_NUMBER_X 16
#endif

/// Japanese draws the values nearer the names, and a multiplier further left to
/// leave room for its suffix.
#if LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_DETAILS_VALUE_X 64
#define ITEM_DETAILS_MULTIPLIER_X 48
#else
#define ITEM_DETAILS_VALUE_X 72
#define ITEM_DETAILS_MULTIPLIER_X 72
#endif

/// `.`, the decimal point of a multiplier.
extern u8 gMenu_PointString[];

void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y);

/// Describes an item (an inventory entry) in window, one 8-pixel row a line:
/// its equip bonuses and effects, a curse, the Psynergy it bestows, its uses
/// and its count, or failing all of these whether it is rare or important.
/// count is the count to show, which only Japanese reads;
/// ItemMenu_DrawItemDetails passes -1, the entry's own.
void ItemMenu_DrawItemDetailsWithCount(s32 window, s32 item, s32 count) {
  struct ItemDefinition *def;
  s32 i;
  s32 found;
  s32 amount;
  s32 consumable;
  s8 row;

  row = 0;
  consumable = 0;
  def = Item_GetDirectFar(item & INVENTORY_ITEM_ID_MASK);
  if (def->type != ITEM_TYPE_NONE) {
    found = 0;
    if (def->primaryBonus == 0 && def->secondaryBonus == 0) {
      for (i = 0; i < EQUIP_EFFECT_SLOTS; i++) {
        if (def->effects[i].kind != 0 || def->useType == ITEM_USE_TEACHES) {
          found = 1;
          break;
        }
      }
    } else {
      found = 1;
    }
    if (found == 1) {
      Text_DrawMessageAsTilesFar((s32)MsgEquipEffectHeading, window,
                                 ITEM_DETAILS_HEADING_X, 0);
      row = 1;
    }
    if (def->primaryBonus != 0) {
      Text_DrawMessageAsTilesFar((s32)MsgStatLabel, window, 0, row * 8);
      amount = def->primaryBonus;
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      row++;
    }
    if (def->secondaryBonus != 0) {
      Text_DrawMessageAsTilesFar((s32)MsgDefenseLabel, window, 0, row * 8);
      amount = def->secondaryBonus;
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      row++;
    }
  }
  for (i = 0; i < EQUIP_EFFECT_SLOTS; i++) {
    if (def->effects[i].kind == EQUIP_EFFECT_NONE)
      continue;
    amount = def->effects[i].amount;
    switch (def->effects[i].kind) {
    case EQUIP_EFFECT_NONE:
      break;
    case EQUIP_EFFECT_ADD_HP:
    case EQUIP_EFFECT_ADD_HP_REGEN:
    case EQUIP_EFFECT_ADD_PP:
    case EQUIP_EFFECT_ADD_PP_REGEN:
    case EQUIP_EFFECT_ADD_AGILITY:
    case EQUIP_EFFECT_ADD_LUCK:
    case EQUIP_EFFECT_ADD_ACTIONS:
      Text_DrawMessageAsTilesFar(def->effects[i].kind + (s32)MsgEquipEffectName,
                                 window, 0, row * 8);
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      break;
    case EQUIP_EFFECT_ADD_VENUS_POWER:
    case EQUIP_EFFECT_ADD_MERCURY_POWER:
    case EQUIP_EFFECT_ADD_MARS_POWER:
    case EQUIP_EFFECT_ADD_JUPITER_POWER:
    case EQUIP_EFFECT_ADD_VENUS_RESIST:
    case EQUIP_EFFECT_ADD_MERCURY_RESIST:
    case EQUIP_EFFECT_ADD_MARS_RESIST:
    case EQUIP_EFFECT_ADD_JUPITER_RESIST:
      Window_SetTilemapEntryFar(
          window,
          (u8)((def->effects[i].kind - EQUIP_EFFECT_ADD_VENUS_POWER) %
               ELEMENT_COUNT) +
              1,
          0, row, 2);
      Text_DrawMessageAsTilesFar(def->effects[i].kind + (s32)MsgEquipEffectName,
                                 window, 8, row * 8);
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      break;
    case EQUIP_EFFECT_SCALE_HP:
    case EQUIP_EFFECT_SCALE_HP_REGEN:
    case EQUIP_EFFECT_SCALE_PP:
    case EQUIP_EFFECT_SCALE_PP_REGEN:
    case EQUIP_EFFECT_SCALE_ATTACK:
    case EQUIP_EFFECT_SCALE_DEFENSE:
    case EQUIP_EFFECT_SCALE_AGILITY:
    case EQUIP_EFFECT_SCALE_LUCK:
      Text_DrawMessageAsTilesFar(def->effects[i].kind + (s32)MsgEquipEffectName,
                                 window, 0, row * 8);
      // The scale is in tenths: "1.2" for 12.
      if (amount >= STAT_SCALE_ONE) {
        Text_DrawNumberInWindowFar(1, 1, window, ITEM_DETAILS_MULTIPLIER_X,
                                   row * 8);
        Text_DrawStringInWindowFar(gMenu_PointString, window,
                                   ITEM_DETAILS_MULTIPLIER_X + 8, row * 8);
        Text_DrawNumberInWindowFar(amount - STAT_SCALE_ONE, 1, window,
                                   ITEM_DETAILS_MULTIPLIER_X + 16, row * 8);
      } else {
        Text_DrawNumberInWindowFar(0, 1, window, ITEM_DETAILS_MULTIPLIER_X,
                                   row * 8);
        Text_DrawStringInWindowFar(gMenu_PointString, window,
                                   ITEM_DETAILS_MULTIPLIER_X + 8, row * 8);
        Text_DrawNumberInWindowFar(amount, 1, window,
                                   ITEM_DETAILS_MULTIPLIER_X + 16, row * 8);
      }
#if LANGUAGE == LANGUAGE_JAPANESE
      Text_DrawMessageAsTilesFar((s32)MsgTimesSuffix, window,
                                 ITEM_DETAILS_MULTIPLIER_X + 24, row * 8);
#endif
      break;
    case EQUIP_EFFECT_ADD_UNLEASH_CHANCE:
    case EQUIP_EFFECT_CURSE_REMOVABLE:
    case EQUIP_EFFECT_LURE:
      Text_DrawMessageAsTilesFar(def->effects[i].kind + (s32)MsgEquipEffectName,
                                 window, 0, row * 8);
#if LANGUAGE == LANGUAGE_GERMAN
      // German continues these three names on a second line, the messages
      // after the last effect's name in the order of their effects: "steigen"
      // and "aufgehoben werden.".
      row++;
      if (def->effects[i].kind == EQUIP_EFFECT_ADD_UNLEASH_CHANCE)
        Text_DrawMessageAsTilesFar((s32)MsgEquipEffectName + EQUIP_EFFECT_LURE +
                                       1,
                                   window, 0, row * 8);
      else if (def->effects[i].kind == EQUIP_EFFECT_CURSE_REMOVABLE)
        Text_DrawMessageAsTilesFar((s32)MsgEquipEffectName + EQUIP_EFFECT_LURE +
                                       2,
                                   window, 0, row * 8);
#ifndef BUGFIX
      // BUG: German has only those two second lines, so the Lure Cap's
      // "Erhöht gegn." goes on with the message after them, "Items".
      else if (def->effects[i].kind == EQUIP_EFFECT_LURE)
        Text_DrawMessageAsTilesFar((s32)MsgEquipEffectName + EQUIP_EFFECT_LURE +
                                       3,
                                   window, 0, row * 8);
#endif
#endif
      break;
    }
    row++;
  }
  if (def->flags & ITEM_CURSED) {
    Text_DrawMessageAsTilesFar((s32)MsgItemCursed, window, 0, row * 8);
    row++;
  }
  if (def->useType == ITEM_USE_TEACHES) {
    Text_DrawMessageAsTilesFar((s32)MsgBestowsPsynergy, window, 0, row * 8);
    consumable = 1;
    row++;
  }
  if (def->useType != ITEM_USE_EMPTIES && def->useType != ITEM_USE_FREE) {
    if (!consumable) {
      Text_DrawMessageAsTilesFar((s32)MsgUsesHeading, window,
                                 ITEM_DETAILS_USES_X, row * 8);
      row++;
    }
    switch (def->useType) {
    case ITEM_USE_FREE:
      break;
    case ITEM_USE_CONSUMED:
      Text_DrawMessageAsTilesFar((s32)MsgSingleUse, window, 0, row * 8);
      row++;
      break;
    case ITEM_USE_MAY_BREAK: {
      s32 m;

      if (item & INVENTORY_BROKEN) {
        m = (s32)MsgBrokenNotice;
        Text_DrawMessageAsTilesFar(m, window, 0, row * 8);
        row++;
#if LANGUAGE == LANGUAGE_GERMAN
        // German ends the sentence on a third line, six messages on.
        Text_DrawMessageAsTilesFar(m + 1, window, 0, row * 8);
        row++;
        m += 6;
        Text_DrawMessageAsTilesFar(m, window, 0, row * 8);
        row++;
#else
        m++;
        Text_DrawMessageAsTilesFar(m, window, 0, row * 8);
        row++;
#endif
      } else {
        m = (s32)MsgMightBreak;
        Text_DrawMessageAsTilesFar(m, window, 0, row * 8);
        row++;
#if LANGUAGE == LANGUAGE_GERMAN
        Text_DrawMessageAsTilesFar(m + 1, window, 0, row * 8);
        row++;
        m += 7;
        Text_DrawMessageAsTilesFar(m, window, 0, row * 8);
        row++;
#else
        m++;
        Text_DrawMessageAsTilesFar(m, window, 0, row * 8);
        row++;
#endif
      }
      break;
    }
    }
  }
  if (def->flags & ITEM_STACKABLE) {
    if (row != 0)
      row++;
    i = (s32)MsgNumberHeading;
    Text_DrawMessageAsTilesFar(i, window, ITEM_DETAILS_NUMBER_X, row * 8);
    row++;
#if LANGUAGE == LANGUAGE_JAPANESE
    // Japanese draws the count itself: the entry's own for -1, "none" for 0.
    if (count == -1) {
      amount = (item & INVENTORY_QUANTITY_MASK) / INVENTORY_QUANTITY_STEP;
      Text_DrawNumberInWindowFar(amount + 1, 2, window, 0, row * 8);
      Text_DrawMessageAsTilesFar(i + 1, window, 16, row * 8);
      row++;
    } else if (count == 0) {
      Text_DrawMessageAsTilesFar(i + 8, window, 0, row * 8);
      row++;
    } else {
      amount = count;
      Text_DrawNumberInWindowFar(amount, 3, window, 0, row * 8);
      Text_DrawMessageAsTilesFar(i + 1, window, 24, row * 8);
      row++;
    }
#else
    amount = (item & INVENTORY_QUANTITY_MASK) / INVENTORY_QUANTITY_STEP;
    MessageArgument_PushFar(amount + 1, MESSAGE_ARGUMENT_NUMBER);
    Text_DrawMessageAsTilesFar(i + 1, window, 0, row * 8);
    row++;
#endif
  }
  found = 0;
  if (row == 0) {
    if (def->flags & ITEM_RARE) {
      Text_DrawMessageAsTilesFar((s32)MsgRareItem, window, 0, 0);
      found = 1;
    }
    if (!found) {
      if (def->flags & ITEM_INDISPENSABLE) {
        Text_DrawMessageAsTilesFar((s32)MsgImportantItem, window, 0, 0);
        found = 1;
      }
      if (!found)
        Text_DrawMessageAsTilesFar((s32)MsgDetailsUnknown, window, 0, 0);
    }
  }
}

#undef ITEM_DETAILS_MULTIPLIER_X

#define Text_DrawStringInWindowFar                                             \
  ((void (*)(u8 *, s32, s32, s32))Text_DrawStringInWindowFar)

/// Writes a signed stat change at (x, y) of window: the number, with a plus or
/// minus sign just before its first digit.
void ItemMenu_DrawStat(s32 statChange, s32 unused, s32 window, s32 x, s32 y) {
  s32 digits;
  s32 magnitude;

  Text_DrawNumberInWindowFar(statChange, 3, window, x, y);
  // Count the digits of |statChange| to place the sign.
  digits = 1;
  magnitude = statChange;
  if (statChange < 0) {
    magnitude = -statChange;
  }
  if (magnitude > 9) {
    digits = 2;
  }
  magnitude = statChange;
  if (statChange < 0) {
    magnitude = -statChange;
  }
  if (magnitude > 99) {
    digits = 3;
  }
  if (statChange > 0) {
    Text_DrawStringInWindowFar(&gMenu_PlusSignString, window,
                               x - digits * 8 + 16, y);
  } else {
    Text_DrawStringInWindowFar(&gMenu_MinusSignString, window,
                               x - digits * 8 + 16, y);
  }
}

#define ITEM_MENU ((struct ItemMenuWork *)Ram_HeapSlots->menuWork)

/// Moves the item window to row 5 of the right-hand side.
void ItemMenu_PlaceItemWindowBelowItemQuestion(void) {
  Window_SetBounds(ITEM_MENU->itemWindow, 13, 5, 17, 10);
}

/// Moves the item window to row 3 of the right-hand side, below a one-line
/// question.
void ItemMenu_PlaceItemWindowBelowQuestion(void) {
  Window_SetBounds(ITEM_MENU->itemWindow, 13, 3, 17, 10);
}

/// Makes the message window, along the top right, 5 rows tall: the item's name
/// and a question below it.
void ItemMenu_FitMessageWindowToItemQuestion(void) {
  Window_SetBounds(ITEM_MENU->questionWindow, 13, 0, 17, 5);
}

/// Makes the item menu's message window six tiles high.
void ItemMenu_FitMessageWindowToLongItemQuestion(void) {
  Window_SetBounds(ITEM_MENU->questionWindow, 13, 0, 17, 6);
}

/// Makes the item menu's message window seven tiles high.
void ItemMenu_FitMessageWindowToItemChoices(void) {
  Window_SetBounds(ITEM_MENU->questionWindow, 13, 0, 17, 7);
}

/// Makes the item menu's message window three tiles high.
void ItemMenu_FitMessageWindowToQuestion(void) {
  Window_SetBounds(ITEM_MENU->questionWindow, 13, 0, 17, 3);
}

#undef Text_DrawStringInWindowFar
#undef ITEM_MENU

// The Japanese window asks いくつ? ("How many?") on the count's row instead of
// the top row, draws the counter こ after each count held, and keeps its cursor
// further left.
#if LANGUAGE == LANGUAGE_JAPANESE
/// いくつ?, the message three before MsgShortcutHelp.
#define HOW_MANY_MESSAGE ((s32)MsgShortcutHelp - 3)
#define HOW_MANY_X 0
#define HOW_MANY_Y 32
/// こ, the counter after a count: the message before MsgItemCommandUse.
#define PIECE_MESSAGE ((s32)MsgItemCommandUse - 1)
#define HOW_MANY_CURSOR_X 88
#else
#define HOW_MANY_MESSAGE ((s32)MsgHowMany)
#define HOW_MANY_X 32
#define HOW_MANY_Y 0
#define HOW_MANY_CURSOR_X 128
#endif

/// The item menu's quantity bar picture, which QuantityBar_FillFar fills in
/// under the "How many?" question.
extern const u8 gMenu_QuantityTiles[];

/// Colours the first count segments of the quantity bar in tiles, each two
/// pixels wide: the left pixel in colour 1, the right in colour color.
void QuantityBar_FillFar(s32 count, s32 color, void *tiles);

/// Asks how many of the selected item stack to give or drop: Left and Right
/// choose a number from 1 to quantityRange (shown as baseQuantity + 1 up to
/// baseQuantity + quantityRange on a dial of digit sprites), with the party
/// member's remaining count and, when giving (dropMode 0), the receiver's new
/// count beside their names.
s32 ItemMenu_SelectGiveQuantity(s32 baseQuantity, s32 quantityRange,
                                s32 dropMode) {
  s32 value = baseQuantity;
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  u8 *tiles = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  s32 entry;
  s32 changed = 1;
  s32 targetCount = 0;
  void *window = menu->questionWindow;
  s32 ownerCount;
  struct WindowSprite *arrow;

  ItemMenu_FitMessageWindowToItemChoices();
  WindowSprite_RedrawSavedRectFar(window);
  if (dropMode == 0)
    targetCount = ItemMenu_GetItemQuantity(
        menu->sideMember[1], menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
  ownerCount = ItemMenu_GetItemQuantity(
      menu->sideMember[0], menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
  entry = VramCache_FindFreeEntry();
  if (entry != VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(entry, 256, 0);
    WindowSprite_CreateFar(entry, OUTPUT_SIZE_32X8, window, 48, 32);
    arrow = WindowSprite_CreateFar(entry, OUTPUT_SIZE_32X8, window, 80, 32);
    arrow->sprite.oam.index = arrow->sprite.oam.index + 4;
    MenuCursor_Slide(HOW_MANY_CURSOR_X, 40);
    while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
      if (changed) {
        changed = 0;
        value = (value + quantityRange) % quantityRange;
        WindowSprite_RedrawSavedRectFar(window);
        Text_DrawMessageAsTilesFar(HOW_MANY_MESSAGE, window, HOW_MANY_X,
                                   HOW_MANY_Y);
        Dma_Set(gMenu_QuantityTiles, tiles, DMA_COPY_WORDS(64), REG_DMA3);
        QuantityBar_FillFar(INVENTORY_STACK_MAX, QUANTITY_BAR_OUT_OF_REACH,
                            tiles);
        QuantityBar_FillFar(baseQuantity + quantityRange, QUANTITY_BAR_OPEN,
                            tiles);
        QuantityBar_FillFar(baseQuantity + value + 1, QUANTITY_BAR_CHOSEN,
                            tiles);
        QuantityBar_FillFar(baseQuantity, QUANTITY_BAR_HELD, tiles);
        VramCache_Load(entry, 256, tiles);
        Text_DrawNumberInWindowFar(value + 1, 2, window, 32, 32);
        Text_DrawMessageAsTilesFar(
            (menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
                (s32)MsgItemName,
            window, 16, 8);
        Text_DrawNumberInWindowFar(ownerCount - value - 1, 2, window, 16, 24);
#if LANGUAGE == LANGUAGE_JAPANESE
        Text_DrawMessageAsTilesFar(PIECE_MESSAGE, window, 32, 24);
#endif
        if (dropMode == 0) {
          Text_DrawNumberInWindowFar(targetCount + value + 1, 2, window, 80,
                                     24);
#if LANGUAGE == LANGUAGE_JAPANESE
          Text_DrawMessageAsTilesFar(PIECE_MESSAGE, window, 96, 24);
#endif
        }
        Text_DrawStringAtOffsetFar(Unit_Get(menu->sideMember[0]), window, 16,
                                   16);
        if (dropMode == 0)
          Text_DrawStringAtOffsetFar(Unit_Get(menu->sideMember[1]), window, 80,
                                     16);
      }
      if (gInput.pressed & KEY_A) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        break;
      }
      if (gInput.pressed & KEY_B) {
        value = -1;
        Sound_PlayFar(SE_MENU_CANCEL);
        break;
      }
      MenuCursor_Position(HOW_MANY_CURSOR_X, 40);
      if (gInput.repeat & KEY_LEFT) {
        value--;
        changed = 1;
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      }
      if (gInput.repeat & KEY_RIGHT) {
        value++;
        changed = 1;
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      }
      wait(1);
    }
  }
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(window);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  menu->selectedItemIcon->motion = ICON_HIDDEN;
  if (GameFlag_Get(FLAG_MENU_START_PRESSED))
    value = -1;
  return value;
}

#undef HOW_MANY_Y
#undef HOW_MANY_CURSOR_X

// Draws the selected item's icon, owner and item name in the heading.
/// Shows the selected item at the top of the message window: its icon, its
/// party member's name and its name.
void ItemMenu_DrawItemHead(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;

  Resource_LoadByModeIntoSlotFar(ICON_KIND_ITEM, menu->selectedItems[0],
                                 (u8)menu->selectedItemIcon->index, 0);
  menu->selectedItemIcon->motion = ICON_SHOWN;
  menu->selectedItemIcon->x = 112;
  menu->selectedItemIcon->y = 8;
  Icon_PrepareActor(menu->selectedItemIcon);
  Text_DrawStringAtOffsetFar(Unit_Get(menu->sideMember[0]),
                             menu->questionWindow, 16, 0);
  Text_DrawMessageAsTilesFar((menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
                                 (s32)MsgItemName,
                             menu->questionWindow, 16, 8);
}

/// The French question starts at the window's left edge.
#if LANGUAGE == LANGUAGE_FRENCH
#define ITEM_DROP_QUESTION_X 0
#else
#define ITEM_DROP_QUESTION_X 8
#endif

/// Asks whether to drop item (an inventory entry): its name, the question, and
/// Yes or No, No first. Returns 0 for Yes and 1 for No, B or Start.
s32 ItemMenu_ConfirmDrop(s32 item) {
  s32 window;
  s32 itemId;
  s32 question;
  s32 answer;
  s32 rows = 2;
  s32 selection = 1;
  s32 changed = 1;

  window = Window_CreateFar(13, 3, 17, 10, WINDOW_FLAG_INSTANT);
  itemId = item & INVENTORY_ITEM_ID_MASK;
  Item_GetDirectFar(itemId);
  Text_DrawMessageAsTilesFar(itemId + (s32)MsgItemName, window, 24, 0);
  question = (s32)MsgConfirmDrop;
  Text_DrawMessageAsTilesFar(question, window, ITEM_DROP_QUESTION_X, 16);
  question++;
  Text_DrawMessageAsTilesFar(question, window, ITEM_DROP_QUESTION_X, 24);
  answer = (s32)MsgYes;
  Text_DrawMessageAsTilesFar(answer, window, 24, 40);
  answer++;
  Text_DrawMessageAsTilesFar(answer, window, 24, 56);
  MenuCursor_Slide(104, 86);
  for (;;) {
    if (GameFlag_Get(FLAG_MENU_START_PRESSED))
      break;
    if (changed) {
      changed = 0;
      selection = (selection + rows) % rows;
    }
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      selection = 1;
      break;
    }
    MenuCursor_Position(104, (selection << 4) + 70);
    if (gInput.repeat & KEY_UP) {
      selection--;
      changed = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_DOWN) {
      selection++;
      changed = 1;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    }
    wait(1);
  }
  if (GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    selection = 1;
  }
  Window_CloseFar(window, 1);
  return selection;
}

#undef ITEM_DROP_QUESTION_X

/// The question and its answers, as each translation lays them out. English and
/// Japanese stack Yes over No to the right of the question, and Up and Down
/// move the cursor; German, Spanish, French and Italian set them side by side
/// below it, and Left and Right move the cursor. Japanese indents the question.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define EQUIP_YES_X 24
#define EQUIP_YES_Y 24
#define EQUIP_NO_X 72
#define EQUIP_NO_Y 24
#define EQUIP_CURSOR_X(answer) ((answer) * 48 + 110)
#define EQUIP_CURSOR_Y(answer) 32
#define EQUIP_PREVIOUS_KEY KEY_LEFT
#define EQUIP_NEXT_KEY KEY_RIGHT
#else
#define EQUIP_YES_X 96
#define EQUIP_YES_Y 0
#define EQUIP_NO_X 96
#define EQUIP_NO_Y 16
#define EQUIP_CURSOR_X(answer) 184
#define EQUIP_CURSOR_Y(answer) (((answer) << 4) + 5)
#define EQUIP_PREVIOUS_KEY KEY_UP
#define EQUIP_NEXT_KEY KEY_DOWN
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
#define EQUIP_QUESTION_X 8
#else
#define EQUIP_QUESTION_X 0
#endif

/// The question is named in the English catalog alone; in every edition it is
/// the second message after MsgConfirmDrop.
#if LANGUAGE == LANGUAGE_ENGLISH
#define EQUIP_QUESTION ((s32)MsgEquipThisItem)
#else
#define EQUIP_QUESTION ((s32)MsgConfirmDrop + 2)
#endif

/// After giving or trading the selected item to the party member in side 1,
/// offers to equip it there, showing the stats it would give.
s32 ItemMenu_ConfirmEquip(s32 unused) {
  s32 rows = 2;
  s32 selection = 0;
  s32 changed = 1;
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuWork;
  void *unit = Unit_Get(menu->sideMember[1]);
  void *saved;
  s32 window;
  s32 slot;
  s32 member;

  slot = menu->selectedSlots[1];
  member = menu->sideMember[1];
  ItemMenu_DrawEquipPreview(member, slot, 0, member);
  saved = Heap_Alloc(BATTLE_UNIT_SIZE);
  Iwram_CopyWords(saved, unit, BATTLE_UNIT_SIZE);
  window = menu->questionWindow;
  if ((u32)(Inventory_EquipFar(menu->sideMember[1], menu->selectedSlots[1]) +
            2) <= 1) {
    selection = 1;
  } else {
    Text_DrawMessageAsTilesFar((s32)MsgYes, window, EQUIP_YES_X, EQUIP_YES_Y);
    Text_DrawMessageAsTilesFar((s32)MsgYes + 1, window, EQUIP_NO_X, EQUIP_NO_Y);
    Window_ClearInteriorTilesFar(window, 16, 16, 96, 24);
    Text_DrawMessageAsTilesFar(EQUIP_QUESTION, window, EQUIP_QUESTION_X, 16);
    MenuCursor_Slide(EQUIP_CURSOR_X(0), EQUIP_CURSOR_Y(0));
    for (;;) {
      if (GameFlag_Get(FLAG_MENU_START_PRESSED))
        break;
      if (changed) {
        changed = 0;
        selection = (selection + rows) % rows;
      }
      if (gInput.pressed & KEY_A) {
        Sound_PlayFar(SE_NOTE_HIGH);
        break;
      }
      if (gInput.pressed & KEY_B) {
        Sound_PlayFar(SE_MENU_CANCEL);
        selection = 1;
        break;
      }
      MenuCursor_Position(EQUIP_CURSOR_X(selection), EQUIP_CURSOR_Y(selection));
      if (gInput.repeat & EQUIP_PREVIOUS_KEY) {
        selection--;
        changed = 1;
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      }
      if (gInput.repeat & EQUIP_NEXT_KEY) {
        selection++;
        changed = 1;
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      }
      wait(1);
    }
  }
  if (GameFlag_Get(FLAG_MENU_START_PRESSED))
    selection = 1;
  if (selection == 1)
    Iwram_CopyWords(unit, saved, BATTLE_UNIT_SIZE);
  Heap_Release(saved);
  Unit_RefreshPsynergyFar(menu->sideMember[1]);
  Unit_CalculateFar(menu->sideMember[1]);
  return selection;
}

#undef EQUIP_QUESTION

/// Loads the two page arrows into free tile slots.
void Resource_LoadPairedBlocks(void) {
  struct ItemMenuWork *state = Ram_HeapSlots->menuWork;
  s32 value = VramCache_FindFreeEntry();

  state->resourceSlots[0] = value;
  VramBlock_LoadResourceFar(value, ICON_TILE_BYTES, RESOURCE_MENU_ARROW_TILES);
  value = VramCache_FindFreeEntry();
  state->resourceSlots[1] = value;
  VramBlock_LoadResourceFar(value, ICON_TILE_BYTES,
                            RESOURCE_MENU_SMALL_DOWN_ARROW_TILES);
}

/// Splits the selected slot owner's items into pages of five rows.
s32 ItemMenu_BuildPagedList(struct PagedList *result, s32 index) {
  s32 encoded;
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 limit;
  s32 remainder;
  s32 quotient;
  s32 groups;
  s32 value;

  limit = ItemMenu_CountItems(menu->sideMember[index]);
  encoded = Unit_Get(menu->sideMember[index]);
  value = menu->selectedIndexByMember[menu->sideMember[index]];
  if ((value + 1) > limit) {
    value = limit - 1;
  }
  quotient = value / LIST_PAGE_SIZE;
  remainder = value % LIST_PAGE_SIZE;
  groups = limit / LIST_PAGE_SIZE;
  if (limit % LIST_PAGE_SIZE != 0) {
    groups++;
  }
  result->memberState = encoded;
  result->page = quotient;
  result->pageCount = groups;
  result->row = remainder;
  result->entryCount = limit;
  result->selectedIndex = value;
  return 1;
}

/// Draws the selected item-list row's name and highlights only that row.
s32 ItemMenu_DrawItemDetailPage(s32 listWindow, void *memberWork,
                                struct PagedList *state) {
  struct ItemMenuWork *menu;
  s32 page;
  s32 combined;
  s32 row;
  s32 window;

  page = state->page;
  menu = Ram_HeapSlots->menuWork;
  window = menu->infoWindow;
  combined = page * LIST_PAGE_SIZE;
  combined += state->row;
  state->selectedIndex = combined;

  // The Japanese release clears the info window; from patch 1 it redraws the
  // window's saved rectangle.
#if TLA_PATCH_1
  WindowSprite_RedrawSavedRectFar(window);
#else
  WindowSprite_ClearListFar(window);
#endif
  wait(1);

  combined = state->selectedIndex;
  if (menu->items[combined] != 0) {
    s32 masked = (menu->items[combined] & INVENTORY_ITEM_ID_MASK) +
                 (s32)MsgItemPlainName;

    // Japanese draws the name as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
    Text_DrawMessageFar(masked, menu->infoWindow, 0, 0);
#else
    Text_DrawMessageAsTilesFar(masked, menu->infoWindow, 0, 0);
#endif
  }

  for (row = 0; row <= 4; row++) {
    if (row == state->row) {
      Render_SetTilemapFlagRect(menu->itemWindow, 1, row * 2 + 1, 14, 1,
                                HIGHLIGHT_PALETTE);
    } else {
      Render_SetTilemapFlagRect(menu->itemWindow, 1, row * 2 + 1, 14, 1,
                                WINDOW_PALETTE);
    }
  }

  wait(1);
  return 1;
}

/// The Japanese page draws its page icons and names further right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_PAGE_ICONS_X 120
#define ENTRY_X 32
#else
#define ITEM_PAGE_ICONS_X 116
#define ENTRY_X 24
#endif
/// Draws one page of the item list into window: a divider, the page's icons and
/// page numbers, and up to five item names.
s32 ItemMenu_DrawNamePage(s32 window, s32 unused,
                          const struct PagedList *state) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  u32 page;
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  const u16 *itemId;

  WindowSprite_RedrawSavedRectFar(window);
  Window_DrawDividerLineFar(window, 0, 11, 16, 11);

  page = state->page;
  firstEntry = page * LIST_PAGE_SIZE;
  visibleCount = (u8)(state->entryCount - firstEntry);
  if (visibleCount > LIST_PAGE_SIZE) {
    visibleCount = LIST_PAGE_SIZE;
  }

  Menu_SetPageIcons(LIST_PAGE_SIZE, firstEntry, window, ITEM_PAGE_ICONS_X, 34);
  PagedList_DrawPageNumbers(window, state->entryCount, LIST_PAGE_SIZE,
                            state->page, PAGE_NUMBERS_RIGHT);

  row = 0;
  if (visibleCount > row) {
    itemId = &menu->items[firstEntry];
    do {
      Text_DrawMessageAsTilesFar((itemId[0] & INVENTORY_ITEM_ID_MASK) +
                                     (s32)MsgItemName,
                                 menu->itemWindow, ENTRY_X, row * 16 + 8);
      row++;
      itemId++;
    } while (visibleCount > row);
  }

  return 1;
}

#undef ITEM_PAGE_ICONS_X
#undef ENTRY_X

/// The item list of the party member in sideIndex (0: the holder, 1: the
/// receiver of a trade), five items a page. The cursor's item is named in the
/// info window and previewed in the status panel, its icon animates, and the
/// party members who can equip it move. Up and Down move the cursor, Left and
/// Right turn pages, and in sideIndex 0 L and R switch to the previous or next
/// party member who has items. Returns the chosen inventory entry, or -1 when
/// the player backs out.
s32 ItemMenu_RunList(s32 pane) {
  struct ItemMenuWork *menu;
  void *window;
  s32 changed;
  s32 moved;
  s32 member;
  s32 count;
  u8 i;
  s32 result;
  s32 redraw;
  s32 done;
  struct Unit *owner;
  s32 previous;
  struct WindowSprite *icon;
  s32 work[5];
  struct PagedList state;

  menu = Ram_HeapSlots->menuWork;
  result = 0;
  previous = 0;
  menu->equipPreview = result;
  Window_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, WINDOW_FLAG_INSTANT);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && GameFlag_Get(FLAG_MENU_START_PRESSED) == 0) {
    owner = Unit_Get(menu->sideMember[pane]);
    menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
    ItemMenu_DrawIcons(menu->items, 0);
    menu->selectedItemIcon->motion = ICON_HIDDEN;
    ItemMenu_BuildPagedList(&state, pane);
    MenuCursor_Position(98, state.selectedIndex * 16 + 36);
    changed = 1;
    redraw = 1;

    while (GameFlag_Get(FLAG_MENU_START_PRESSED) == 0) {
      MenuCursor_Position(98, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (owner->inventory[previous] != 0) {
          icon = menu->entryIcons[previous];
          Icon_PrepareActor(icon);
        }
        if (redraw != 0) {
          wait(1);
          ItemMenu_DrawNamePage(window, 0, &state);
          if (pane == 0)
            Text_DrawMessageAsTilesFar((s32)MsgChangeCharacterHelp, window,
                                       CHANGE_HELP_X, 88);
          redraw = 0;
        }
        ItemMenu_DrawItemDetailPage(window, work, &state);
        menu->selectedItems[pane] = menu->items[state.selectedIndex];
        ItemMenu_DrawEquipPreview(menu->sideMember[pane], state.selectedIndex,
                                  0, menu->sideMember[pane]);
        if (menu->items[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          icon->motion = WINDOW_SPRITE_MOTION_SWELL;
          icon->animationStep = 0;
          icon->depth = OAM_DEPTH_FRONT - 5;
        }
        for (i = 0; i < menu->partyCount; i++)
          Sprite_StartAnimationFar(menu->memberSprites[i], ANIM_STAND);
      }

      if ((gFrameCount & 31) == 0) {
        for (i = 0; i <= 3; i++) {
          s32 slot = menu->memberSpriteIds[i];
          if (slot != -1 &&
              Item_CanUnitEquip(slot, INVENTORY_ITEM_ID_MASK &
                                          menu->items[state.selectedIndex]) !=
                  0)
            Sprite_StartAnimationFar(menu->memberSprites[i], ANIM_NOD);
        }
      }

      wait(1);
      previous = state.selectedIndex;
      moved = PagedList_HandleInput(0, state.entryCount, LIST_PAGE_SIZE,
                                    &state.row, &state.page);
      if (moved == 1) {
        redraw = 1;
        changed = 1;
      }
      if (moved == 0)
        changed = 1;
      if (moved == -1)
        changed = 0;

      if ((gInput.pressed & KEY_A) != 0 &&
          menu->items[state.selectedIndex] != 0) {
        Sound_PlayFar(SE_NOTE_LOW);
        result = menu->items[state.selectedIndex];
        done = 1;
        break;
      }
      if ((gInput.pressed & KEY_B) != 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }
      if ((gInput.repeat & KEY_R) != 0 || (gInput.repeat & KEY_L) != 0) {
        if (pane == 1) {
          Sound_PlayFar(SE_MENU_ERROR);
          wait(1);
        } else {
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          menu->selectedIndexByMember[menu->sideMember[pane]] =
              state.selectedIndex;
          member = menu->sideIndex[pane];
          count = menu->partyCount;
          do {
            if ((gInput.repeat & KEY_R) != 0)
              member++;
            else
              member--;
            member = (member + count) % count;
            menu->selectedMember = menu->memberIds[member];
            menu->sideMember[pane] = menu->memberIds[member];
            menu->sideIndex[pane] = member;
            menu->itemCount = ItemMenu_CollectItems(
                Unit_Get(menu->sideMember[pane]), menu->items, 0);
          } while (menu->itemCount == 0);
          Menu_CreateOwnerEntryObjects(member / 4 * 4);
          Menu_HighlightPageEntry(menu->mainWindow, member, count);
          menu->pageArrows[0]->motion = ICON_HIDDEN;
          menu->pageArrows[1]->motion = ICON_HIDDEN;
          wait(1);
          break;
        }
      }
    }
  }

  Window_ClearInteriorTilesFar(window, 0, 88, 120, 96);
  Icon_PrepareActor(menu->gridCursor);
  menu->selectedSlots[pane] = state.selectedIndex;
  menu->selectedIndexByMember[menu->sideMember[pane]] = state.selectedIndex;
  menu->selectedItems[pane] = result;
  if (GameFlag_Get(FLAG_MENU_START_PRESSED) != 0)
    result = -1;
  wait(1);
  return result;
}

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) ||          \
    (LANGUAGE == LANGUAGE_SPANISH)
extern s32 gItemMenu_CommandColumnXTable[6];

/// The cursor's x at the item command in column and row, from the table; 0
/// outside three columns and three rows.
s32 ItemMenu_GetCommandCursorX(s32 column, s32 row) {
  if (column > 2 || row > 2 || column < 0 || row < 0)
    return 0;
  return gItemMenu_CommandColumnXTable[row * 3 + column];
}

/// The cursor's y at the item command in row: 30 in the first row, 38 below.
s32 ItemMenu_GetCommandCursorY(s32 column, s32 row) {
  s32 y = 30;

  if (row != 0)
    y = 38;
  return y;
}
#endif

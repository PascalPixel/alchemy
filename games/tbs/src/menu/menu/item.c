/// @file
/// The item menu: the field menu's Item screen, the command loop, the member
/// and item lists, and Use, Equip, Give, Drop, Remove and Details.
// Part of menu.c.

#include "../menu_internal.h"

// Where the edition's text goes. The international editions have longer
// words, and some languages move a message to fit its own.
//
// The x of the small message windows "Dropped it.", "You were cursed!" and
// "Removed".
#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_MODAL_SHORT_X 14
#define ITEM_MODAL_REMOVED_X 14
#else
#define ITEM_MODAL_SHORT_X 15
#define ITEM_MODAL_REMOVED_X 17
#endif

#if LANGUAGE == LANGUAGE_FRENCH
#define ITEM_CANNOT_TRADE_X 11
#else
#define ITEM_CANNOT_TRADE_X 15
#endif

#if LANGUAGE == LANGUAGE_ITALIAN
#define ITEM_TRADED_X 13
#else
#define ITEM_TRADED_X 15
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_TRADE_PROMPT_X 0
#else
#define ITEM_TRADE_PROMPT_X 16
#endif

// The x of the command grid's middle column.
#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_COMMAND_TEXT_X 32
#else
#define ITEM_COMMAND_TEXT_X 40
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_QUANTITY_WINDOW_WIDTH 30
#else
#define ITEM_QUANTITY_WINDOW_WIDTH 13
#endif

// The hand cursor and the "How many?" question of the quantity chooser.
#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_GIVE_CURSOR_X 128
#define ITEM_GIVE_ASK_X 32
#define ITEM_GIVE_ASK_Y 0
#else
#define ITEM_GIVE_CURSOR_X 88
#define ITEM_GIVE_ASK_X 0
#define ITEM_GIVE_ASK_Y 32
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_DROP_QUESTION_X 0
#else
#define ITEM_DROP_QUESTION_X 8
#endif

// The item list's icon column and name column.
#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_LIST_PAGE_ICONS_X 116
#define ITEM_LIST_NAME_X 24
#else
#define ITEM_LIST_PAGE_ICONS_X 120
#define ITEM_LIST_NAME_X 32
#endif

/// The field's work (heap slot HEAP_SLOT_EVENT) as the Item screen writes it:
/// the item request that the field loop hands to Field_UseItem once the menu
/// has closed. struct FieldWork (field_runtime.h) names these fields item and
/// itemArgument.
struct ItemPickerResult {
  s32 unk000[0x60]; ///< FieldWork up to its item request
  u16 chosenItem;   ///< FieldWork.item: owner << 10 | item id
  u8 unk182[0x18];  ///< FieldWork's raised event and fallen characters
  u16 chosenSlot;   ///< FieldWork.itemArgument: the owner's inventory slot
};

/// The field menu's Item screen (Menu_RunTopSelection, icon 3): choose a
/// party member, one of their items and a command. Items used here take
/// effect at once; an item to be used on the field is written as the field's
/// item request (struct ItemPickerResult), run when the field menu has closed.
/// Owns the menu work (HEAP_SLOT_MENU) while it runs and frees it before
/// returning. Returns ItemMenu_RunCommands' result: 1 when an item is to be
/// used on the field, -1 when the player backed out or pressed Start (the
/// field menu then opens again).
s32 ItemMenu_Run(void) {
#if TBS_PATCH_1
  void *backup;
#endif
  struct InventoryMenuState *screen;
  s32 item;
  s32 target;
  s32 owner;
  s32 result;
#if TBS_PATCH_1
  s32 size;
#endif
  void **process;
  void **cache;

  // Open the menu over the field, which stops moving. The international
  // editions keep 8 KB of the field's window tiles aside while the menu is up
  // and free the copy after putting the tiles back.
#if TBS_PATCH_1
  size = 0x2000;
  backup = Runtime_BumpAllocateAlternatePool(size);
#endif
  screen = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_MENU_CONTROL];
  ((struct ObjectSystemWork *)cache[0])->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);
  screen->partyCount = Party_ListActiveOwnersFar(screen->ownerIds);
  ItemMenu_Initialize(0, 3, 0, 7);
  Resource_LoadPairedBlocks();
  Palette_LightenBankHighlight(14);
  Link_DrawShiftedTilePairFar(UI_PAGE_ARROW_VRAM);
  screen->messageWindow = UiWindow_CreateFar(13, 0, 17, 3, 2);
#if TBS_PATCH_1
  // Save the window tiles the menu draws over, and fill them with colour 3
  // (0x33333333: eight 4-bit pixels of colour 3 a word).
  Menu_SuspendFieldTasks();
  Iwram_CopyWords(backup, BG_CHAR_BLOCK(1), size);
  Iwram_FillWords(BG_CHAR_BLOCK(1), size, 0x33333333);
  UiWork_SetAltFlagAndClearTableFar(1);
#endif

  // Let the player choose.
  Menu_CancelSoundReset();
  result = ItemMenu_RunCommands(&owner, &target, &item);
  Menu_EnsureCancelSound();
  if (result == 1) {
    struct ItemPickerResult *selection =
        cache[HEAP_SLOT_EVENT - HEAP_SLOT_MENU_CONTROL];

    selection->chosenItem = (owner << 10) | (item & ITEM_ID_MASK);
    selection->chosenSlot = screen->selectedSlots[0];
  }

  // Close the menu and give the screen back to the field.
  RenderOutput_ClearListFar(screen->statusWindow);
  process = &cache[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU_CONTROL];
  ((struct UiRenderWork *)*process)->menuBusy = 1;
  ItemMenu_Close();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  Menu_ResetTwoResourceEntries();
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  ((struct ObjectSystemWork *)cache[0])->suspended = 0;
#if TBS_PATCH_1
  UiWindow_MarkVisibleTileAttributesFar();
  UiWork_SetAltFlagAndClearTableFar(0);
  Iwram_CopyWords(BG_CHAR_BLOCK(1), backup, size);
  ((struct UiRenderWork *)*process)->menuBusy = 0;
  MemFree(backup);
  WaitFrames(1);
  Menu_ResumeFieldTasks();
#endif
  WaitFrames(1);
  UiWindow_EraseBorderRectFar(0, 0, 30, 20);
  ((struct UiRenderWork *)*process)->menuBusy = 0;
  Event_ClearInvalidPackedValuesFar();
  return result;
}

/// The most items of one kind a stack holds.
#define ITEM_STACK_MAX 30

/// The resident IWRAM word copy, called through its address.
typedef s32 (*ItemWordCopyFn)(void *dst, const void *src, s32 size);

#define ITEM_IWRAM_COPY_WORDS                                                  \
  ((ItemWordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))

static __inline__ s32 Item_CopyWords(ItemWordCopyFn copy, void *dst,
                                     const void *src, s32 size) {
  /* FAKEMATCH(wrapper): the trade's unit copies call the routine through
   * this helper, which takes its address first; through Iwram_CopyWords, 3 of
   * 1269 instructions of ItemMenu_RunCommands differ in tbs-en. */
  return copy(dst, src, size);
}

/// The steps of the item menu's command loop (ItemMenu_RunCommands).
enum ItemMenuStep {
  ITEM_STEP_OWNER = 0,
  ITEM_STEP_ITEM = 1,
  ITEM_STEP_USE = 2,
  ITEM_STEP_EQUIP = 3,
  ITEM_STEP_GIVE = 4,
  ITEM_STEP_DROP = 5,
  ITEM_STEP_TARGET = 6, ///< Give: choose whom to give it to
  ITEM_STEP_TRADE = 7,  ///< the receiver is full: choose what they give back
  ITEM_STEP_IDLE = 8,   ///< nothing sets it
  ITEM_STEP_COMMAND = 9,
  ITEM_STEP_INSPECT = 10, ///< Details (Menu_SelectQuantity)
  ITEM_STEP_REMOVE = 11,
  ITEM_STEP_QUANTITY = 12 ///< nothing sets it
};

/// Runs the item menu: choose a party member, one of their items and a command
/// (Use, Equip, Give, Drop, Remove or Inspect), then carry it out, until the
/// player backs out or presses Start. Returns 1 when an item is to be used
/// outside the menu, with its owner, target and item id in the out parameters;
/// -1 when the player backed out or pressed Start; 0 otherwise.
s32 ItemMenu_RunCommands(s32 *ownerOut, s32 *targetOut, s32 *itemOut) {
  s32 done;
#if !TBS_PATCH_1
  s32 owner;
  s32 chosen;
#endif
  s32 selection;
  s32 menuResult;
  struct BattleUnit *sourceUnit;
  struct BattleUnit *targetUnit;
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;
  struct BattleUnit *sourceCopy;
  struct BattleUnit *targetCopy;
  u32 state;
  s32 command;
  s32 result;
  s32 heldCount;
  s32 amount;
  s32 transferFailed;
  s32 removedCount;
#if TBS_PATCH_1
  s32 giveTarget;
#endif
  u8 sourceQuantity;
  u8 targetQuantity;
  s8 stackSize;

  done = 0;
#if !TBS_PATCH_1
  owner = 0;
  chosen = 0;
#endif
  selection = 0;
  menuResult = 0;
  state = ITEM_STEP_OWNER;
  menu = gMenuWork;

  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    switch (state) {
    // Choose whose items to look at.
    case ITEM_STEP_OWNER:
      menu->selectedSlots[0] = 0;
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      menu->selectedItemIcon->active = ICON_HIDDEN;
      ItemMenu_DrawMessage(0, MsgWhoseItem);
#if TBS_PATCH_1
      RenderOutput_RedrawSavedRectFar(menu->infoWindow);
      UiText_DrawWorkValueWithLabel(menu->infoWindow);
      command = ItemMenu_SelectMember(0);
      if (command == -1) {
        selection = command = 0;
        menuResult = -1;
        done = 1;
      }
#else
      UiText_DrawWorkValueWithLabel(menu->infoWindow);
      owner = ItemMenu_SelectMember(0);
      if (owner == -1) {
        chosen = selection = owner = 0;
        menuResult = -1;
        done = 1;
      }
#endif
      RenderOutput_RedrawSavedRectFar(menu->infoWindow);
      ItemMenu_HideAllIcons();
      state = ITEM_STEP_ITEM;
      break;

    // Choose one of their items.
    case ITEM_STEP_ITEM:
      if (ItemMenu_CountItems(menu->paneOwner[0]) == 0) {
        state = ITEM_STEP_OWNER;
        break;
      }
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      menu->selectedItemIcon->active = ICON_HIDDEN;
      menu->paneIcons[0]->active = ICON_SHOWN;
      ItemMenu_DrawMessage(0, MsgWhichItem);
      selection = ItemMenu_RunList(0);
      state = ITEM_STEP_OWNER;
      if (selection == -1) {
        break;
      }
      menu->listMode = 0xff;
      state = ITEM_STEP_COMMAND;
      break;

    // Choose what to do with it. An item that starts an effect on the field
    // (BattleFx_HasTriggerFar) closes the menu and is returned to the caller;
    // other usable items are used here.
    case ITEM_STEP_COMMAND:
      command = ItemMenu_ChooseCommand();
      if (command == -1) {
        state = ITEM_STEP_ITEM;
        menu->completionFlag = 1;
      }
      if (command == ITEM_COMMAND_USE) {
        if (BattleFx_HasTriggerFar(menu->selectedItems[0] & ITEM_ID_MASK) !=
            0) {
          done = 1;
          // The international editions report target 0 (command is
          // ITEM_COMMAND_USE here) and the bare item id; the Japanese ones the
          // target chosen last and the list's whole inventory entry.
          // ItemMenu_Run reads only the owner and the item id.
#if TBS_PATCH_1
          *ownerOut = menu->paneOwner[0];
          *targetOut = command;
          *itemOut = menu->selectedItems[0] & ITEM_ID_MASK;
#else
          *ownerOut = owner;
          *targetOut = chosen;
          *itemOut = selection;
#endif
          menuResult = 1;
          break;
        }
        // 1: choose a member to use it on; 2: use it on the whole party now;
        // 0 or -1: close the menu and use it from the field.
        result =
            Item_ClassifyUseMode(menu->paneOwner[0], menu->selectedItems[0]);
        if (result == 1) {
          state = ITEM_STEP_USE;
        }
        if (result == 2) {
          ItemMenu_Use();
          RenderOutput_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(menu->messageOffset + MsgItemUseResult,
                                         0, -1);
#if !TBS_PATCH_1
          RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
          menu->paneIcons[0]->active = ICON_HIDDEN;
          menu->itemCount = ItemMenu_CollectItems(
              UnitGetFar(menu->paneOwner[0]), menu->items, 0);
          ItemMenu_DrawIcons(menu->items, 0);
          state = ITEM_STEP_OWNER;
        }
        if (result == -1 || result == 0) {
          done = 1;
          *ownerOut = menu->paneOwner[0];
          *targetOut = menu->paneOwner[1];
          *itemOut = menu->selectedItems[0] & ITEM_ID_MASK;
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
      ItemMenu_SetMessageWindowHeight5();
      ItemMenu_SetItemWindowRow5();
      RenderOutput_RedrawSavedRectFar(menu->messageWindow);
      ItemMenu_DrawItemHead();
      UiText_DrawCharacterAtOffsetFar(MsgUseOnWhom, menu->messageWindow, 16,
                                      16);
#if TBS_PATCH_1
      if (ItemMenu_SelectTarget(0) != -1) {
#else
      chosen = ItemMenu_SelectTarget(0);
      if (chosen != -1) {
#endif
        command = 0;
        if (ItemMenu_IsSpecial(menu->selectedItems[0] & ITEM_ID_MASK) != 0) {
          command = STATUS_PANEL_STATS;
        }
        result = ItemMenu_Use();
        Menu_DrawOwnerStatusPanel(menu->statusWindow, menu->paneOwner[1], 0,
                                  command);
        if (result != -1) {
          RenderOutput_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(menu->messageOffset + MsgItemUseResult,
                                         0, -1);
#if !TBS_PATCH_1
          RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
          menu->paneIcons[0]->active = ICON_HIDDEN;
          ItemMenu_TryBreak();
          state = ITEM_STEP_ITEM;
        }
        menu->itemCount = ItemMenu_CollectItems(UnitGetFar(menu->paneOwner[0]),
                                                menu->items, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        menu->completionFlag = 1;
      } else {
        state = ITEM_STEP_COMMAND;
      }
      break;

    // Give: choose to whom.
    case ITEM_STEP_TARGET:
      ItemMenu_SetMessageWindowHeight5();
      ItemMenu_SetItemWindowRow5();
      RenderOutput_RedrawSavedRectFar(menu->messageWindow);
      ItemMenu_DrawItemHead();
      UiText_DrawCharacterAtOffsetFar(MsgGiveToWhom, menu->messageWindow, 16,
                                      16);
#if TBS_PATCH_1
      giveTarget = ItemMenu_SelectTarget(1);
      state = ITEM_STEP_GIVE;
      if (giveTarget == -1) {
#else
      chosen = ItemMenu_SelectTarget(1);
      state = ITEM_STEP_GIVE;
      if (chosen == -1) {
#endif
        ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                  menu->paneOwner[0]);
        state = ITEM_STEP_COMMAND;
      }
      break;

    // Drop the item, or some of a stack, after confirming.
    case ITEM_STEP_DROP:
      ItemMenu_HideAllIcons();
      item = Item_GetDirectFar(menu->selectedItems[0] & ITEM_ID_MASK);
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
      menu->paneOwner[1] = 0;
      Resource_LoadByModeIntoSlotFar(2,
                                     (menu->selectedItems[0] & ITEM_ID_MASK) |
                                         (result << INVENTORY_QUANTITY_SHIFT),
                                     (u8)menu->selectedItemIcon->index, 0);
      menu->selectedItemIcon->active = ICON_SHOWN;
      menu->selectedItemIcon->x = 120;
      menu->selectedItemIcon->y = 28;
      UiIcon_PrepareObject(menu->selectedItemIcon);
      UiWindow_ClearInteriorTilesFar((void *)menu->listWindow, 0, 72, 120, 96);
      RenderOutput_RedrawSavedRectFar(menu->messageWindow);
      // Yes: take result + 1 of the stack out, one at a time. A dropped rare
      // item goes back on the shops' counter (Item_AdjustCounterFar), so a
      // shop may sell it again.
      if (ItemMenu_ConfirmDrop(selection) == 0) {
#if TBS_PATCH_1
        command = menu->paneOwner[0];
        UnitGetFar(command);
        for (removedCount = 0; removedCount < result + 1; removedCount++) {
          Inventory_RemoveFar(command, menu->selectedSlots[0]);
          Item_AdjustCounterFar(menu->selectedItems[0] & ITEM_ID_MASK, 1);
        }
        UnitCalcFar(command);
#else
        owner = menu->paneOwner[0];
        UnitGetFar(owner);
        for (removedCount = 0; removedCount < result + 1; removedCount++) {
          Inventory_RemoveFar(owner, menu->selectedSlots[0]);
          Item_AdjustCounterFar(menu->selectedItems[0] & ITEM_ID_MASK, 1);
        }
        UnitCalcFar(owner);
#endif
        ItemMenu_SetItemWindowRow3();
        ItemMenu_RefreshOwner(menu->paneOwner[0], 0);
        menu->selectedItemIcon->active = ICON_HIDDEN;
        menu->paneIcons[0]->active = ICON_HIDDEN;
        WaitFrames(1);
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(MsgDroppedIt, ITEM_MODAL_SHORT_X, 13);
        menu->completionFlag = 1;
        state = ITEM_STEP_ITEM;
      } else {
        state = ITEM_STEP_COMMAND;
      }
      UnitCalcFar(menu->paneOwner[0]);
      menu->selectedItemIcon->active = ICON_HIDDEN;
      Event_ClearInvalidPackedValuesFar();
      break;

    // Give the item, or some of a stack, to the chosen member. A full
    // inventory offers a trade instead; the new holder may equip it.
    case ITEM_STEP_GIVE:
      transferFailed = 0;
      item = Item_GetDirectFar(menu->selectedItems[0] & ITEM_ID_MASK);
      if ((item->flags & ITEM_STACKABLE) != 0) {
        // A receiver with a full stack of it takes none; one with no free
        // slot and none of it must trade.
        heldCount = InventoryMenu_GetItemQuantity(
            menu->paneOwner[1], menu->selectedItems[0] & ITEM_ID_MASK);
        if (heldCount == ITEM_STACK_MAX) {
          transferFailed = 1;
        }
        if (ItemMenu_CountItems(menu->paneOwner[1]) == INVENTORY_SLOTS &&
            heldCount == 0) {
          state = ITEM_STEP_TRADE;
          break;
        }
        stackSize = (menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
        if (transferFailed == 0) {
          if (heldCount + stackSize > ITEM_STACK_MAX) {
            amount = ITEM_STACK_MAX - heldCount;
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
                menu->paneOwner[1],
                menu->selectedItems[0] & (ITEM_ID_MASK | INVENTORY_BROKEN));
            if (heldCount != -1) {
              Inventory_RemoveFar(menu->paneOwner[0], menu->selectedSlots[0]);
              menu->selectedSlots[1] = heldCount;
            } else {
              transferFailed = 1;
            }
          }
        }
      } else {
        result = Inventory_AddItemFar(menu->paneOwner[1],
                                      menu->selectedItems[0] &
                                          (ITEM_ID_MASK | INVENTORY_BROKEN));
        if (result == -1) {
          state = ITEM_STEP_TRADE;
          break;
        }
        menu->selectedSlots[1] = result;
        result =
            Inventory_RemoveFar(menu->paneOwner[0], menu->selectedSlots[0]);
        if (result == -1) {
          transferFailed = 1;
        }
      }
      UnitCalcFar(menu->paneOwner[0]);
      UnitCalcFar(menu->paneOwner[1]);
      Unit_RefreshPsynergyFar(menu->paneOwner[0]);
      Unit_RefreshPsynergyFar(menu->paneOwner[1]);
      result = 1;
      if (transferFailed == 0) {
        menu->paneOwner[0] = menu->paneOwner[1];
        menu->selectedItems[0] &= ITEM_ID_MASK;
#if LANGUAGE != LANGUAGE_JAPANESE
        ItemMenu_SetMessageWindowHeight6();
#endif
        RenderOutput_RedrawSavedRectFar(menu->messageWindow);
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
      }
      if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0) {
        break;
      }
      ItemMenu_RefreshOwner(menu->paneOwner[1], 1);
      menu->paneIcons[0]->active = ICON_HIDDEN;
      WaitFrames(1);
      // Say how it went: the receiver's stack was full, the item was given,
      // or it was given and equipped (ItemMenu_ConfirmEquip returned 0).
      if (transferFailed == 1) {
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(MsgCannotHoldMore, 15, 14);
      } else {
        RenderOutput_ClearListFar(menu->infoWindow);
        if (result == 1) {
          InventoryMenu_ShowModalMessage(MsgGiven, 15, 14);
        } else {
          ItemMenu_DrawEquipPreview(menu->paneOwner[1], menu->selectedSlots[1],
                                    0, menu->paneOwner[1]);
          InventoryMenu_ShowModalMessage(MsgEquippedIt, 15, 14);
          item = Item_GetDirectFar(menu->selectedItems[0]);
          if ((item->flags & ITEM_CURSED) != 0) {
            AudioCommand_PlayFar(SE_103); // the curse sound
            RenderOutput_ClearListFar(menu->infoWindow);
            InventoryMenu_ShowModalMessage(MsgEquippedIt +
                                               7, // "You were cursed!"
                                           ITEM_MODAL_SHORT_X, 14);
          }
        }
      }
      Event_ClearInvalidPackedValuesFar();
      state = ITEM_STEP_OWNER;
      break;

    // Trade: the receiver gives back one of their items. Both units are saved
    // first and restored if any part of the swap fails.
    case ITEM_STEP_TRADE:
      transferFailed = 0;
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      ItemMenu_DrawMessage(0, MsgSwapForWhat);
#if TBS_PATCH_4
      if (ItemMenu_RunList(1) == -1) {
#else
      selection = ItemMenu_RunList(1);
      if (selection == -1) {
#endif
        state = ITEM_STEP_TARGET;
        break;
      }
      // Snapshot both inventories so any failed part of the trade can roll
      // back.
      sourceUnit = UnitGetFar(menu->paneOwner[0]);
      targetUnit = UnitGetFar(menu->paneOwner[1]);
      sourceCopy = MemAlloc(BATTLE_UNIT_SIZE);
      targetCopy = MemAlloc(BATTLE_UNIT_SIZE);
      Item_CopyWords(ITEM_IWRAM_COPY_WORDS, sourceCopy, sourceUnit,
                     BATTLE_UNIT_SIZE);
      Item_CopyWords(ITEM_IWRAM_COPY_WORDS, targetCopy, targetUnit,
                     BATTLE_UNIT_SIZE);
      // Take both whole stacks out, one item at a time (at most a stack of
      // 30): Inventory_RemoveFar returns 2 for the last one and -1 for an
      // empty slot. Each count ends one short and is made up after its loop.
      sourceQuantity = 0;
      while (sourceQuantity <= 29) {
        result =
            Inventory_RemoveFar(menu->paneOwner[0], menu->selectedSlots[0]);
        if (result == 2) {
          break;
        }
        if (result == -1) {
          transferFailed = 1;
          break;
        }
        sourceQuantity++;
      }
      sourceQuantity++;
      targetQuantity = 0;
      while (targetQuantity <= 29) {
        // The receiver cannot trade away a worn item that will not come off.
        if ((menu->selectedItems[1] & INVENTORY_EQUIPPED) != 0) {
          item = Item_GetDirectFar(menu->selectedItems[1] & ITEM_ID_MASK);
          if ((item->flags & ITEM_CANNOT_UNEQUIP) != 0) {
            transferFailed = 1;
          }
        }
        result =
            Inventory_RemoveFar(menu->paneOwner[1], menu->selectedSlots[1]);
        if (result == 2) {
          break;
        }
        if (result == -1) {
          transferFailed = 1;
          break;
        }
        targetQuantity++;
      }
      targetQuantity++;
      // Then give each the other's items.
      while (sourceQuantity != 0) {
        result = Inventory_AddItemFar(menu->paneOwner[1],
                                      menu->selectedItems[0] &
                                          (ITEM_ID_MASK | INVENTORY_BROKEN));
        if (result == -1) {
          transferFailed = 1;
          break;
        }
        menu->selectedSlots[1] = result;
        sourceQuantity--;
      }
      while (targetQuantity != 0) {
        result = Inventory_AddItemFar(menu->paneOwner[0],
                                      menu->selectedItems[1] &
                                          (ITEM_ID_MASK | INVENTORY_BROKEN));
        if (result == -1) {
          transferFailed = 1;
          break;
        }
        menu->selectedSlots[0] = result;
        targetQuantity--;
      }
      WaitFrames(1);
      if (transferFailed == 1) {
        Item_CopyWords(ITEM_IWRAM_COPY_WORDS, sourceUnit, sourceCopy,
                       BATTLE_UNIT_SIZE);
        Item_CopyWords(ITEM_IWRAM_COPY_WORDS, targetUnit, targetCopy,
                       BATTLE_UNIT_SIZE);
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(MsgCannotTrade, ITEM_CANNOT_TRADE_X, 14);
#if TBS_PATCH_LEVEL == 3
        RenderOutput_RedrawSavedRectFar(menu->statusWindow);
#endif
      } else {
        UnitCalcFar(menu->paneOwner[0]);
        UnitCalcFar(menu->paneOwner[1]);
        Unit_RefreshPsynergyFar(menu->paneOwner[0]);
        Unit_RefreshPsynergyFar(menu->paneOwner[1]);
        ItemMenu_SetMessageWindowHeight5();
#if LANGUAGE != LANGUAGE_JAPANESE
        ItemMenu_SetMessageWindowHeight6();
        ItemMenu_HidePageIcons();
#endif
        RenderOutput_RedrawSavedRectFar(menu->messageWindow);
        menu->paneOwner[0] = menu->paneOwner[1];
        menu->selectedItems[0] &= ITEM_ID_MASK;
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
        if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
          RenderOutput_ClearListFar(menu->infoWindow);
          ItemMenu_SetItemWindowRow5();
          ItemMenu_RefreshOwner(menu->paneOwner[1], 1);
          if (result == 0) {
            ItemMenu_DrawEquipPreview(menu->paneOwner[1],
                                      menu->selectedSlots[1], 0,
                                      menu->paneOwner[1]);
            InventoryMenu_ShowModalMessage(MsgEquippedIt, 15, 14);
            item = Item_GetDirectFar(menu->selectedItems[0]);
            if ((item->flags & ITEM_CURSED) != 0) {
              AudioCommand_PlayFar(SE_103); // the curse sound
              RenderOutput_ClearListFar(menu->infoWindow);
              InventoryMenu_ShowModalMessage(MsgEquippedIt +
                                                 7, // "You were cursed!"
                                             ITEM_MODAL_SHORT_X, 14);
            }
          } else {
            InventoryMenu_ShowModalMessage(MsgTraded, ITEM_TRADED_X, 14);
          }
        }
      }
      MemFree(targetCopy);
      MemFree(sourceCopy);
      Event_ClearInvalidPackedValuesFar();
      state = ITEM_STEP_OWNER;
      break;

    // Equip it. Inventory_EquipFar returns -1 when the member cannot equip it
    // and -2 when the item it would replace is cursed on.
    case ITEM_STEP_EQUIP:
      result = Inventory_EquipFar(menu->paneOwner[0], menu->selectedSlots[0]);
      if (result == -1) {
        state = ITEM_STEP_ITEM;
        break;
      }
      if (result == -2) {
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(MsgCannotRemoveItem, 0, -1);
#if !TBS_PATCH_1
        RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
        state = ITEM_STEP_ITEM;
        break;
      }
      UnitCalcFar(menu->paneOwner[0]);
      Unit_RefreshPsynergyFar(menu->paneOwner[0]);
      menu->paneIcons[0]->active = ICON_HIDDEN;
      menu->itemCount =
          ItemMenu_CollectItems(UnitGetFar(menu->paneOwner[0]), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      WaitFrames(1);
      ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                menu->paneOwner[0]);
      RenderOutput_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage(MsgEquippedIt, 15, 8);
      item = Item_GetDirectFar(menu->selectedItems[0]);
      if ((item->flags & ITEM_CURSED) != 0) {
        AudioCommand_PlayFar(SE_103); // the curse sound
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(MsgEquippedIt + 7, // "You were cursed!"
                                       ITEM_MODAL_SHORT_X, 8);
      }
      state = ITEM_STEP_ITEM;
      break;

    // Unequip it.
    case ITEM_STEP_REMOVE:
      UnitGetFar(menu->paneOwner[0])->inventory[menu->selectedSlots[0]] &=
          ~INVENTORY_EQUIPPED;
      UnitCalcFar(menu->paneOwner[0]);
      Unit_RefreshPsynergyFar(menu->paneOwner[0]);
      menu->paneIcons[0]->active = ICON_HIDDEN;
      menu->itemCount =
          ItemMenu_CollectItems(UnitGetFar(menu->paneOwner[0]), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      WaitFrames(1);
      // Show the stats as they are without it.
      menu->equipPreview = 1;
      ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                menu->paneOwner[0]);
      menu->equipPreview = 0;
      RenderOutput_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage(MsgRemoved, ITEM_MODAL_REMOVED_X, 8);
      Event_ClearInvalidPackedValuesFar();
      state = ITEM_STEP_ITEM;
      break;

    // Show its description.
    case ITEM_STEP_INSPECT:
      menu->paneIcons[0]->active = ICON_HIDDEN;
      Menu_SelectQuantity(menu->selectedItems[0]);
      RenderOutput_RedrawSavedRectFar(menu->statusWindow);
      ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                menu->paneOwner[0]);
      menu->paneIcons[0]->active = ICON_SHOWN;
      state = ITEM_STEP_COMMAND;
      break;

    // Not reached from any command: choose a quantity, then do nothing with
    // it.
    case ITEM_STEP_QUANTITY:
      result = ItemMenu_SelectGiveQuantity(0, 30, 0);
      if (result != -1) {
        heldCount = result;
      }
      state = ITEM_STEP_ITEM;
      break;

    case ITEM_STEP_IDLE:
      break;

    default:
      done = 1;
      break;
    }
  }

  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0) {
    menuResult = -1;
  }
  return menuResult;
}

/// Uses the selected item from the owner in pane 0 on the member in pane 1.
/// On failure plays the error sound, shows why in the info window and returns
/// -1; otherwise plays the item's animation, recalculates both members and
/// returns 1.
s32 ItemMenu_Use(void) {
  struct InventoryMenuState *menu;
  s32 result;

  menu = gMenuWork;
  result =
      Item_Use(menu->selectedSlots[0], menu->paneOwner[0], menu->paneOwner[1]);

  if (result == -1) {
    AudioCommand_PlayFar(SE_MENU_ERROR);
    RenderOutput_ClearListFar(menu->infoWindow);
    InventoryMenu_ShowModalMessage(menu->messageOffset + MsgItemUseResult,
                                   result, result);
#if !TBS_PATCH_1
    RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
    menu->completionFlag = 1;
    return result;
  }

  Item_PlayUseAnimation(menu->selectedItems[0] & ITEM_ID_MASK);
  UnitCalcFar(menu->paneOwner[0]);
  UnitCalcFar(menu->paneOwner[1]);
  return 1;
}

/// Sets up the item menu's first screen: the cursor window with the party's
/// figures, the info window along the bottom, and an 8 by 2 cursor grid. The
/// Psynergy and Djinn screens open with it too. The parameters are not used.
void ItemMenu_Initialize(s32 x, s32 y, s32 mode, s32 columns) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 ownerIndex;

  PsynergyMenu_InitializeEntryObjects(UiMenu_CreateCursor(menu), 2, 2, 8, 0);
  // The four figures stand at screen y 30; the screens raise the member
  // chosen to 26.
  for (ownerIndex = 3; ownerIndex >= 0; ownerIndex--)
    menu->ownerY[ownerIndex] = 30;

  {
    u8 zero = 0;
    s32 style;
    menu->helpWindow = NULL;
    menu->statusWindow = NULL;
    style = 2;
    menu->infoWindow = UiWindow_CreateFar(0, 17, 30, 3, style);
    menu->itemWindow = NULL;
    menu->selectedColumn = zero;
    menu->selectedRow = zero;
    menu->columnCount = 8;
    menu->rowCount = style;
  }
}

/// Creates the 32 item icon sprites in window, off screen until placed: slots 0
/// to 7 at x 248, the rest at x 256, all at y 168, each showing item graphic
/// slot of the same number. The item, Psynergy and Status screens call it for
/// the window that holds their icons; the sprites are that window's render
/// outputs and go back to the pool when it closes.
void Menu_SpawnIconEntries(struct InventoryMenuState *state,
                           struct UiWindow *window) {
  struct RenderOutput **firstIcons;
  s32 firstSlot;
  s32 firstY;
  struct RenderOutput **secondIcons;
  s32 secondSlot;
  s32 secondY;
  struct RenderOutput **remainingIcons;
  s32 remainingSlot;
  s32 remainingY;

  firstSlot = 0;
  firstY = 0xA8;
  firstIcons = &state->entryIcons[0];
  do {
    *firstIcons++ =
        RenderOutput_CreateFromResourceFar(2, firstSlot, window, 0xF8, firstY);
    firstSlot++;
  } while (firstSlot <= 7);

  secondSlot = 8;
  secondY = 0xA8;
  secondIcons = &state->entryIcons[8];
  do {
    *secondIcons++ = RenderOutput_CreateFromResourceFar(2, secondSlot, window,
                                                        0x100, secondY);
    secondSlot++;
  } while (secondSlot <= 15);

  remainingSlot = 16;
  remainingY = 0xA8;
  remainingIcons = &state->entryIcons[16];
  do {
    *remainingIcons++ = RenderOutput_CreateFromResourceFar(
        2, remainingSlot, window, 0x100, remainingY);
    remainingSlot++;
  } while (remainingSlot <= 31);
}

/// Hides all 32 item icons.
void ItemMenu_HideAllIcons(void) {
  s32 hiddenState = ICON_HIDDEN;
  struct RenderOutput **icons =
      ((struct InventoryMenuState *)gMenuWork)->entryIcons;
  s32 slot;

  for (slot = 31; slot >= 0; slot--) {
    struct RenderOutput *icon = *icons++;
    if (icon != 0) {
      icon->active = hiddenState;
    }
  }
}

#if LANGUAGE != LANGUAGE_JAPANESE
/// Hides the item icons in the first column of the five-wide item grid.
void ItemMenu_HidePageIcons(void) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 slot = 0;
  s32 hiddenState = ICON_HIDDEN;
  struct RenderOutput **iconSlot = menu->entryIcons;

  do {
    struct RenderOutput *icon = *iconSlot++;

    if (icon != 0 && slot % MENU_ITEM_GRID_COLUMNS == 0) {
      icon->active = hiddenState;
    }
    slot++;
  } while (slot <= 31);
}
#endif

/// Closes the item menu: releases the party figures, hides the icons and the
/// status panel's portrait (the work's cursor), and closes every window the
/// menu opened. Every screen's run function calls it before freeing the menu
/// work.
void ItemMenu_Close(void) {
  struct InventoryMenuState *menu;
  struct RenderOutput *cursor;

  menu = gMenuWork;
  Menu_ReleaseEntryObjects();
  ItemMenu_HideAllIcons();
  WaitFrames(1);
  cursor = menu->cursor;
  cursor->active = ICON_HIDDEN;
  UiWindow_CloseIfOpen(&menu->mainWindow, 1);
  UiWindow_CloseIfOpen(&menu->itemWindow, 1);
  UiWindow_CloseIfOpen(&menu->messageWindow, 1);
  UiWindow_CloseIfOpen(&menu->statusWindow, 1);
  UiWindow_CloseIfOpen(&menu->helpWindow, 1);
  UiWindow_CloseIfOpen(&menu->infoWindow, 1);
  UiWindow_CloseIfOpen(&menu->equipWindow, 1);
  UiWindow_CloseIfOpen(&menu->listWindow, 1);
  UiWindow_CloseIfOpen(&menu->unknownWindow38, 1);
  UiWindow_CloseIfOpen(&menu->modalWindow, 1);
  UiWindow_CloseIfOpen(&menu->unknownWindow40, 1);
}

/// Lets the player choose a party member in pane (0 or 1), starting from the
/// member chosen last. Returns the member's id, or -1 when the player backs
/// out.
s32 ItemMenu_SelectMember(s32 pane) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 result = 0;
  s32 ownerIndex;
  struct BattleUnit *unit;

  ownerIndex = menu->paneIndex[pane];
  RenderOutput_ClearListFar(menu->infoWindow);
  menu->paneCount[pane] = menu->partyCount;
  if (ownerIndex == -1) {
    menu->paneIndex[pane] = result;
    ownerIndex = 0;
  } else {
    UiMenu_SlideCursor(ownerIndex * 24 - 10, 16);
  }
  unit = UnitGetFar(menu->ownerIds[ownerIndex]);
  menu->itemCount = ItemMenu_CollectItems(unit, menu->items, 0);
  result = ItemMenu_RunOwnerSelection(menu->ownerIds, menu->items);
  UiIcon_PrepareObject(menu->paneIcons[pane]);
  WaitFrames(1);
  return result;
}

/// The party member row of the item menu. Left and Right move between members,
/// showing each one's items and status; holding R shows their equipment
/// instead; L + A sorts the member's items, a new sort order each press; A
/// chooses a member who has items and B backs out. Returns the chosen member's
/// id, or -1.
s32 ItemMenu_RunOwnerSelection(u16 *ownerIds, u16 *items) {
  struct InventoryMenuState *menu;
  s32 selection;
  s32 ownerCount;
  s32 result;
  struct BattleUnit *unit;
  u8 sortMode;
  s32 showEquipment;
  s32 redraw;
  struct UiWindow *window;
  s32 ownerIndex;

  menu = gMenuWork;
  selection = menu->paneIndex[0];
  ownerCount = menu->paneCount[0];
  redraw = 1;
  result = 0;
  sortMode = 0;
  showEquipment = 0;
  unit = UnitGetFar(ownerIds[selection]);
  if (UiWindow_UpdateOrCreate(&menu->itemWindow, 13, 3, 17, 10, 2))
    Menu_SpawnIconEntries(menu, menu->itemWindow);
  if (UiWindow_UpdateOrCreate(&menu->helpWindow, 13, 13, 17, 4, 2)) {
    menu->selectedItemIcon =
        RenderOutput_CreateFromResourceFar(2, 0, menu->helpWindow, 0, result);
    menu->selectedItemIcon->active = ICON_HIDDEN;
  }
  // The help text: L + A sorts, R shows the equipment.
#if LANGUAGE != LANGUAGE_JAPANESE
  UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp, menu->helpWindow, 0, 0);
  UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp + 1, menu->helpWindow, 0,
                                  8);
#else
  UiText_DrawStringInWindowFar(gItemMenu_ArrangeKeysString, menu->helpWindow, 0,
                               0);
  UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp, menu->helpWindow, 32, 0);
  UiText_DrawStringInWindowFar(gItemMenu_EquipmentKeyString, menu->helpWindow,
                               16, 8);
  UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp + 1, menu->helpWindow, 32,
                                  8);
#endif
  menu->paneIcons[0]->active = redraw; // ICON_SHOWN
  while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    selection = (selection + ownerCount) % ownerCount;
    UiMenu_PositionCursor(selection * 24 - 10, 16);
    if (redraw) {
      sortMode = 0;
      redraw = 0;
      window = menu->statusWindow;
      unit = UnitGetFar(ownerIds[selection]);
      if (showEquipment) {
        menu->itemCount = ItemMenu_CollectItems(UnitGetFar(ownerIds[selection]),
                                                menu->items, 0);
        ItemMenu_OpenCategory(ownerIds[selection]);
        Menu_DrawOwnerStatusPanel(window, ownerIds[selection], 0,
                                  STATUS_PANEL_STATS);
      } else {
        ItemMenu_RefreshOwner(ownerIds[selection], 0);
        Menu_DrawOwnerStatusPanel(window, ownerIds[selection], 0,
                                  STATUS_PANEL_EXPERIENCE);
      }
      // Raise the chosen member's figure.
      for (ownerIndex = 3; ownerIndex >= 0; ownerIndex--)
        menu->ownerY[ownerIndex] = 30;
      menu->ownerY[selection] = 26;
    }
    WaitFrames(1);
    // L + A sorts, cycling through the four sort orders.
    if (gKeysPressed & KEY_A) {
      if (gKeysHeld & KEY_L) {
        sortMode = (sortMode + 4) % 4;
        InventoryMenu_SortByListOrder(unit->inventory, sortMode);
        sortMode++;
        ItemMenu_RefreshOwner(ownerIds[selection], 0);
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
      } else if (ItemMenu_CountItems(ownerIds[selection])) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        result = ownerIds[selection];
        break;
      } else {
        AudioCommand_PlayFar(SE_MENU_ERROR);
      }
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gKeysPressed & KEY_R) {
      showEquipment = 1;
      redraw = 1;
    }
    if (!(gKeysHeld & KEY_R) && showEquipment == 1) {
      showEquipment = 0;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selection--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      selection++;
      redraw = 1;
    }
  }
  menu->paneIndex[0] = selection;
  menu->selectedOwner = ownerIds[selection];
  menu->paneOwner[0] = ownerIds[selection];
  return result;
}

/// Shows the equipment view of member ownerId's items in the item window.
void ItemMenu_OpenCategory(s32 ownerId) {
  struct InventoryMenuState *menu;

  menu = gMenuWork;
  ItemMenu_PosCategory();
  RenderOutput_RedrawSavedRectFar(menu->itemWindow);
  ItemMenu_DrawCategory(menu->itemWindow, ownerId, 0);
}

/// Chooses the party member to use (mode 0) or give (mode 1) the selected item
/// to. Using shows each member's status as the cursor passes; giving shows each
/// member's items, how many of this item they already hold, or that they would
/// have to trade, and the equipment preview. The party members who can equip
/// the item move while the cursor is up. Returns the chosen member's id, or -1
/// when the player backs out; giving cannot choose the item's owner.
s32 ItemMenu_SelectTarget(s32 mode) {

  struct InventoryMenuState *menu;
  struct UiWindow *window;
  s32 ownerCount;
  s32 ownerIndex;
  s32 redraw;
  u8 result;
  s32 descriptionShown;
  s32 heldCount;
  struct RenderOutput *targetMarker;

  menu = gMenuWork;
  window = menu->itemWindow;
  ownerIndex = menu->paneIndex[1];
  ownerCount = menu->partyCount;
  redraw = 1;
  result = 0;
  descriptionShown = 0;
  UiWindow_SetBounds(window, 13, 5, 17, 12);
  RenderOutput_RedrawSavedRectFar((void *)menu->itemWindow);
  UnitGetFar(menu->ownerIds[menu->paneIndex[0]]);
  Scheduler_AddOrUpdateCallback(EquipmentMenu_UpdateCompatibilityIndicators,
                                0xc80);
  while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      ownerIndex = (ownerIndex + ownerCount) % ownerCount;
      window = menu->itemWindow;
      UnitGetFar(menu->ownerIds[ownerIndex]);
      // The marker over the chosen member, three tiles a member. Nothing in
      // TBS stores paneIcons[1], so it is still NULL from
      // UiWindow_InitializeWork: these writes go to the BIOS area at address
      // 0, which ignores them, and no marker is drawn.
      targetMarker = menu->paneIcons[1];
      targetMarker->oam.x = targetMarker->x =
          ((menu->mainWindow->x + ownerIndex * 3) << 3) - 2;
      // Giving: the member's items and how many of this one they hold.
      if (mode == 1) {
        ItemMenu_RefreshOwner(menu->ownerIds[ownerIndex], 1);
        UiWindow_DrawDividerLineFar(window, 0, 9, 16, 9);
        UiWindow_ClearInteriorTilesFar((void *)window, 0, 72, 120, 80);
        if (ownerIndex != menu->paneIndex[0]) {
          heldCount = InventoryMenu_GetItemQuantity(menu->ownerIds[ownerIndex],
                                                    menu->selectedItems[0] &
                                                        ITEM_ID_MASK);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
          if (ItemMenu_CountItems(menu->ownerIds[ownerIndex]) ==
                  INVENTORY_SLOTS &&
              heldCount == 0)
            UiText_DrawCharacterAtOffsetFar(MsgTradeForWhat, window, 0, 72);
          else if (heldCount != 0) {
            UiText_DrawNumberInWindowFar(heldCount, 2, window, 8, 72);
            UiText_DrawCharacterAtOffsetFar(MsgInStock, window, STOCK_LABEL_X,
                                            72);
          } else {
            UiText_DrawCharacterAtOffsetFar(MsgNoneInStock, window, 16, 72);
          }
#else
          if (heldCount != 0) {
            UiText_DrawNumberInWindowFar(heldCount, 2, window, 8, 72);
            UiText_DrawCharacterAtOffsetFar(MsgInStock, window, STOCK_LABEL_X,
                                            72);
          } else {
            UiText_DrawCharacterAtOffsetFar(MsgNoneInStock, window, 16, 72);
          }
          if (ItemMenu_CountItems(menu->ownerIds[ownerIndex]) ==
                  INVENTORY_SLOTS &&
              heldCount == 0)
            UiText_DrawCharacterAtOffsetFar(MsgTradeForWhat, window,
                                            ITEM_TRADE_PROMPT_X, 72);
#endif
        }
        ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                  menu->ownerIds[ownerIndex]);
      }
      // Using: the member's status, and the item's name once.
      if (mode == 0) {
        if (ItemMenu_IsSpecial(menu->selectedItems[0] & ITEM_ID_MASK))
          Menu_DrawOwnerStatusPanel(menu->statusWindow,
                                    menu->ownerIds[ownerIndex],
                                    menu->selectedSlots[0], STATUS_PANEL_STATS);
        else
          Menu_DrawOwnerStatusPanel(
              menu->statusWindow, menu->ownerIds[ownerIndex],
              menu->selectedSlots[0], STATUS_PANEL_EXPERIENCE);
        if (!GameFlag_TestFar(FLAG_MENU_INFO_KEPT) && !descriptionShown) {
          MENU_RESET_INFO_WINDOW((void *)menu->infoWindow);
          MENU_DRAW_INFO_TEXT((menu->selectedItems[0] & ITEM_ID_MASK) +
                                  MsgItemPlainName,
                              menu->infoWindow, 0, 0);
          descriptionShown = 1;
        } else {
          GameFlag_ClearBitFar(FLAG_MENU_INFO_KEPT);
        }
      }
    }
    UiMenu_PositionCursor(ownerIndex * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      if (mode == 1 && ownerIndex == menu->paneIndex[0]) {
        AudioCommand_PlayFar(SE_MENU_ERROR);
        continue;
      }
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      result = menu->ownerIds[ownerIndex];
      break;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      // The u8 255 is returned through (s8) as -1.
      result = 255;
      break;
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      ownerIndex--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
      ownerIndex++;
    }
  }
  // The marker is NULL here too (see above).
  targetMarker = menu->paneIcons[1];
  menu->paneIndex[1] = ownerIndex;
  UiIcon_PrepareObject(targetMarker);
  targetMarker->active = ICON_HIDDEN;
  EquipmentMenu_StartCompatibilityIndicators();
  WaitFrames(1);
  menu->paneIndex[1] = ownerIndex;
  menu->selectedOwner = menu->ownerIds[ownerIndex];
  menu->paneOwner[1] = menu->ownerIds[ownerIndex];
  return (s8)result;
}

/// Scheduler callback while ItemMenu_SelectTarget chooses a member to use or
/// give the item to, and while the command grid's cursor is on Give: every 32
/// frames, sets each party figure's animation by whether that member can
/// equip the item (3) or not (1). EquipmentMenu_StartCompatibilityIndicators
/// removes it.
void EquipmentMenu_UpdateCompatibilityIndicators(void) {
  struct InventoryMenuState *menu;
  s8 memberIndex;

  menu = gMenuWork;
  if ((gFrameCount & 31) == 0 && menu->partyCount != 0) {
    memberIndex = 0;
    do {
      if (Item_CanOwnerEquip(menu->ownerIds[memberIndex],
                             menu->selectedItems[0] & ITEM_ID_MASK) != 0) {
        struct AnimationObject *partyFigure = menu->ownerObjects[memberIndex];
        AnimationObjects_SelectAnimationFar(partyFigure, 3);
      } else {
        struct AnimationObject *partyFigure = menu->ownerObjects[memberIndex];
        AnimationObjects_SelectAnimationFar(partyFigure, 1);
      }
      memberIndex++;
    } while (memberIndex < menu->partyCount);
  }
}

/// Ends EquipmentMenu_UpdateCompatibilityIndicators: puts every party figure
/// back to animation 1 and removes the callback. Despite the name, it starts
/// nothing; removing a callback that is not scheduled is harmless, so the
/// command grid calls it at each cursor move.
void EquipmentMenu_StartCompatibilityIndicators(void) {
  struct InventoryMenuState *menu;
  s8 memberIndex;

  menu = gMenuWork;
  if (menu->partyCount != 0) {
    memberIndex = 0;
    do {
      struct AnimationObject *partyFigure = menu->ownerObjects[memberIndex];
      AnimationObjects_SelectAnimationFar(partyFigure, 1);
      memberIndex++;
    } while (memberIndex < menu->partyCount);
  }
  Scheduler_RemoveCallback(&EquipmentMenu_UpdateCompatibilityIndicators);
}

/// Returns 1 for the items that raise a stat for good (Apple, Hard Nut, Mint
/// and Lucky Pepper), whose use shows the stats panel instead of HP and PP.
s32 ItemMenu_IsSpecial(s32 itemId) {
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
  struct InventoryMenuState *menu;

  menu = gMenuWork;
  RenderOutput_RedrawSavedRectFar(menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar(message, menu->messageWindow, 0, 0);
}

/// Hides the icon of each of the 32 list entries whose item is 0. Each of the
/// 32 icon slots must hold a sprite (Menu_SpawnIconEntries).
void Menu_HideEmptyEntryIcons(const u16 *items) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 slot;

  for (slot = 0; slot < 32; slot++) {
    if (items[slot] == 0) {
      UiIcon_PrepareObject(menu->entryIcons[slot]);
      menu->entryIcons[slot]->active = ICON_HIDDEN;
    }
  }
}

/// Returns how many of member ownerId's 15 inventory slots hold an item.
s32 ItemMenu_CountItems(s32 ownerId) {
  s32 maskedItemId;
  s32 remainingSlots;
  s32 count;
  u16 *inventory;

  count = 0;
  inventory = UnitGetFar(ownerId)->inventory;
  remainingSlots = INVENTORY_SLOTS - 1;
  do {
    maskedItemId = ITEM_ID_MASK & *inventory;
    inventory += 1;
    if (maskedItemId != 0) {
      count += 1;
    }
    remainingSlots -= 1;
  } while (remainingSlots >= 0);
  return count;
}

/// Returns how many of itemId the unit ownerId holds: the quantity of
/// its first stack, or 0. The Spanish and Italian editions add up every stack
/// of it.
s32 InventoryMenu_GetItemQuantity(s32 ownerId, s32 itemId) {
  struct BattleUnit *unit;
  s32 slotIndex;
  s32 quantity;

  quantity = 0;
  unit = UnitGetFar(ownerId);
  slotIndex = 0;
  do {
    if (unit->inventory[slotIndex] != 0 &&
        (unit->inventory[slotIndex] & ITEM_ID_MASK) == itemId) {
#if TBS_PATCH_4
      quantity += (unit->inventory[slotIndex] & INVENTORY_QUANTITY_MASK) >>
                  INVENTORY_QUANTITY_SHIFT;
      quantity++;
#else
      quantity = (unit->inventory[slotIndex] & INVENTORY_QUANTITY_MASK) >>
                 INVENTORY_QUANTITY_SHIFT;
      quantity++;
      break;
#endif
    }
    slotIndex++;
  } while (slotIndex < INVENTORY_SLOTS);
  return quantity;
}

/// Copies the unit's items into the list items (32 entries), skipping empty
/// slots and clearing the rest. Returns how many there are. mode is not used.
s32 ItemMenu_CollectItems(struct BattleUnit *unit, u16 *items, s32 mode) {
  s32 itemCount;
  s32 slotIndex;

  for (slotIndex = 0; slotIndex < 32; slotIndex++) {
    items[slotIndex] = 0;
  }
  itemCount = 0;
  for (slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++) {
    items[slotIndex] = 0;
    if (unit->inventory[slotIndex] != 0) {
      items[itemCount] = unit->inventory[slotIndex];
      itemCount++;
    }
  }
  return itemCount;
}

/// Loads each listed item's icon into its item icon sprite, the first 15
/// entries of inventoryEntries, and hides the icons of empty entries. iconStyle
/// 0 draws the list's icons; any other iconStyle the equipment view's (graphics
/// kind 7).
void ItemMenu_DrawIcons(u16 *inventoryEntries, s32 iconStyle) {
  s32 remaining;
  u16 *entries;
  struct RenderOutput **icons;
  s32 itemId;

  icons = ((struct InventoryMenuState *)gMenuWork)->entryIcons;
  entries = inventoryEntries;
  remaining = INVENTORY_SLOTS - 1;
  do {
    itemId = *entries++;
    if (itemId != 0) {
      if (iconStyle == 0) {
        Resource_LoadByModeIntoSlotFar(2, itemId, (u8)(*icons)->index, 0);
      } else {
        Resource_LoadByModeIntoSlotFar(7, itemId, (u8)(*icons)->index, 0);
      }
    }
    icons++;
    remaining--;
  } while (remaining >= 0);
  Menu_HideEmptyEntryIcons(inventoryEntries);
}

/// Shows member ownerId's items in the item window, laid out for layout mode
/// (see ItemMenu_RefreshEntry), or "no items" when there are none.
void ItemMenu_RefreshOwner(s32 ownerId, s32 mode) {
  struct InventoryMenuState *menu;
  struct BattleUnit *owner;
  u16 *items;

  menu = gMenuWork;
  owner = UnitGetFar(ownerId);
  items = menu->items;
  menu->itemCount = ItemMenu_CollectItems(owner, items, 0);
  RenderOutput_RedrawSavedRectFar(menu->itemWindow);
  ItemMenu_RefreshEntry(mode);
  ItemMenu_DrawIcons(items, 0);
  if (ItemMenu_CountItems(ownerId) == 0)
    UiText_DrawCharacterAtOffsetFar(MsgItemMenuEmpty, menu->itemWindow, 8, 24);
}

/// Nothing calls it.
UNUSED void InventoryMenu_NoOp(void) {}

/// Shows in the status panel what the item in owner's inventory slot would mean
/// to member target: for a usable item, target's status; for equipment,
/// target's stats with it equipped; for a Psynergy item, the Psynergy it
/// teaches. When target is someone else, the item is put into target's own
/// inventory for the preview, making room first if it is full, and target is
/// put back afterwards from a heap copy that is freed before returning. mode 1
/// redraws only the lower part of the panel.
void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target) {
  /* FAKEMATCH(temporary): the saved unit reuses owner (and buffer for
   * Psynergy items), keeping owner in r8 and panelStyle in r10; as a separate
   * void *saved, 22 of 172 instructions differ in tbs-en. */
  struct InventoryMenuState *menu = gMenuWork;
  s32 panelStyle = 0;
  struct BattleUnit *unit = UnitGetFar(owner);
  s32 inventoryEntry = unit->inventory[slot];
  s32 previewSlot;

  if (mode == 1)
    panelStyle = STATUS_PANEL_LOWER_ONLY;
  switch (Item_GetDirectFar(inventoryEntry & ITEM_ID_MASK)->type) {
  case 0: // usable items
    Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, panelStyle);
    break;
  case 1: // weapons
  case 2: // armor
  case 3: // shields, gloves and armlets
  case 4: // helmets
  case 5: // boots
  case 7: // no item has type 7 (items.tsv)
  case 8: // rings
  case 9: // shirts
    if (owner == target) {
      panelStyle |= STATUS_PANEL_EQUIP;
      Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, panelStyle);
    } else {
      s32 size;

      unit = UnitGetFar(target);
      size = BATTLE_UNIT_SIZE;
      owner = (s32)MemAlloc(size);
      Iwram_CopyWords((void *)owner, unit, size);
      // With room made, add the item unequipped and preview it in its new
      // slot; otherwise the panel shows target's status without a preview.
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        inventoryEntry &= ~INVENTORY_EQUIPPED;
        previewSlot = Inventory_AddItemFar(target, inventoryEntry);
        if (previewSlot != -1) {
          panelStyle |= STATUS_PANEL_EQUIP;
          Menu_DrawOwnerStatusPanel(menu->statusWindow, target, previewSlot,
                                    panelStyle);
        } else {
          Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot,
                                    panelStyle);
        }
      } else {
        Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, panelStyle);
      }
      Iwram_CopyWords(unit, (void *)owner, size);
      MemFree((void *)owner);
    }
    break;
  case 6: // Psynergy items
    if (target == owner) {
      panelStyle |= STATUS_PANEL_LEARN;
      Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, panelStyle);
    } else {
      s32 size;
      void *buffer;

      unit = UnitGetFar(target);
      size = BATTLE_UNIT_SIZE;
      owner = (s32)MemAlloc(size);
      buffer = (void *)owner;
      Iwram_CopyWords(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        previewSlot = Inventory_AddItemFar(target, inventoryEntry);
        if (previewSlot != -1) {
          panelStyle |= STATUS_PANEL_LEARN;
          Menu_DrawOwnerStatusPanel(menu->statusWindow, target, previewSlot,
                                    panelStyle);
        } else {
          Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot,
                                    panelStyle);
        }
      } else {
        Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, panelStyle);
      }
      Iwram_CopyWords(unit, (void *)owner, size);
      buffer = (void *)owner;
      MemFree(buffer);
    }
    break;
  }
}

/// Makes room in ownerId's inventory: if no slot is free, removes the whole
/// stack in the first slot that is not equipped. Returns 1 when a slot is free
/// (or was freed), 0 otherwise.
s32 Inventory_RemoveFirstUnflagged(s32 ownerId) {
  struct BattleUnit *unit = UnitGetFar(ownerId);
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
        result = Inventory_RemoveFar(ownerId, slotIndex);
      if (result != 2)
        return 0;
      result = 1;
      break;
    }
  }
  return result;
}

#if LANGUAGE != LANGUAGE_JAPANESE

/// Returns the x of the item command cursor on column and row of the command
/// grid (three commands to a row), or 0 when column or row is outside 0 to 2.
/// The international editions take it from a table (86, 117 and 166), as their
/// columns are not evenly spaced; the Japanese ones step 40 pixels a column.
/// The grid has two rows, so row 2 never comes; it would read past the
/// six-entry table.
s32 ItemMenu_CommandCursorX(s32 column, s32 row) {
  if (column > 2 || row > 2 || column < 0 || row < 0)
    return 0;
  return gItemMenu_CommandColumnXTable[row * 3 + column];
}

/// Returns the y of the item command cursor: 30 on the top row, 38 below.
s32 ItemMenu_CommandCursorY(s32 column, s32 row) {
  s32 y;

  (void)column;
  y = 30;
  if (row != 0) {
    y = 38;
  }
  return y;
}
#endif

/// Each item command's state in the grid.
#define ITEM_COMMAND_STATE_DISABLED (-1)
#define ITEM_COMMAND_STATE_AVAILABLE 1

/// The item command grid: Use, Equip and Details on the top row, Give, Remove
/// and Drop below (grid index = row * 3 + column, as ItemMenuCommand counts
/// them). Disabled commands are grey and refuse A. The status panel follows the
/// cursor: the owner's status on Use, the stats with the item equipped on Equip
/// and Details, and without it on the bottom row, where Give also shows who can
/// equip it. The cursor starts on the command chosen last, or on the first
/// available one. Returns the command chosen, or -1 when the player backs out.
s32 ItemMenu_ChooseCommand(void) {
  struct InventoryMenuState *menu;
  s8 commandStorage[6];
  u16 *redrawFlag;
  s32 column;
  s32 lastCommand;
  s32 row;
  s32 commandIndex;
  s32 redraw;
  s8 *commandStates;
#if LANGUAGE != LANGUAGE_JAPANESE
  s32 cursorX;
  s32 cursorY;
#endif

  commandIndex = 0;
  column = 0;
  row = 0;
  menu = gMenuWork;
  redraw = 1;
  commandStates = commandStorage;
  ItemMenu_BuildCommands(commandStates);
  redrawFlag = &menu->flags;
  // Draw the item's name and the command grid, unless they are up already.
  if (*redrawFlag != 1) {
    struct UiWindow *messageWindow;

    ItemMenu_HideAllIcons();
    RenderOutput_RedrawSavedRectFar((void *)menu->listWindow);
    messageWindow = menu->messageWindow;
    ItemMenu_SetMessageWindowHeight7();
    RenderOutput_RedrawSavedRectFar((void *)messageWindow);
    UiWindow_DrawDividerLineFar(messageWindow, 0, 3, 16, 3);
    ItemMenu_DrawItemHead();
    ItemMenu_DrawCommands(commandStates, messageWindow);
    MENU_RESET_INFO_WINDOW((void *)menu->infoWindow);
    MENU_DRAW_INFO_TEXT((menu->selectedItems[0] & ITEM_ID_MASK) +
                            MsgItemPlainName,
                        menu->infoWindow, 0, 0);
  }
  *redrawFlag = 0;

  // Start on the command chosen last, or on the first one available. listMode
  // 0xff (none yet) reads as -1; the later tests win, so the order of
  // preference is Use, Remove, Equip, Give, then Details.
  lastCommand = (s8)menu->listMode;
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
#if LANGUAGE != LANGUAGE_JAPANESE
  cursorX = ItemMenu_CommandCursorX(column, row);
  cursorY = ItemMenu_CommandCursorY(column, row);
  UiMenu_SlideCursor(cursorX, cursorY);
#else
  UiMenu_SlideCursor(column * 40 + 86, row * 8 + 30);
#endif

  for (;;) {
    if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0)
      break;
    if (redraw != 0) {
      redraw = 0;
      column = (column + 3) % 3;
      row = (row + 2) % 2;
      commandIndex = row * 3 + column;
      EquipmentMenu_StartCompatibilityIndicators();
      // Preview the item in the status panel for the command under the cursor:
      // the bottom row (Give, Remove, Drop) without it, Equip and Details with
      // it, Use the member's status.
      if (commandIndex > 2) {
        menu->equipPreview = 1;
        ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                  menu->paneOwner[0]);
        if (commandIndex == ITEM_COMMAND_GIVE) {
          Scheduler_AddOrUpdateCallback(
              &EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
        }
      } else if (commandIndex != 0) {
        menu->equipPreview = 0;
        ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                  menu->paneOwner[0]);
      } else {
        Menu_DrawOwnerStatusPanel(menu->statusWindow, menu->paneOwner[0], 0,
                                  STATUS_PANEL_EXPERIENCE);
      }
    }
#if LANGUAGE != LANGUAGE_JAPANESE
    cursorX = ItemMenu_CommandCursorX(column, row);
    cursorY = ItemMenu_CommandCursorY(column, row);
    UiMenu_PositionCursor(cursorX, cursorY);
#else
    UiMenu_PositionCursor(column * 40 + 86, row * 8 + 30);
#endif
    WaitFrames(1);

    if ((gKeysPressed & KEY_A) != 0) {
      if (commandStates[commandIndex] == ITEM_COMMAND_STATE_DISABLED) {
        AudioCommand_PlayFar(SE_MENU_ERROR);
      } else {
        switch (commandIndex) {
        case ITEM_COMMAND_USE:
          AudioCommand_PlayFar(SE_174); // Use
          break;
        case ITEM_COMMAND_EQUIP:
          AudioCommand_PlayFar(SE_175); // Equip
          break;
        case ITEM_COMMAND_INSPECT:
        case ITEM_COMMAND_GIVE:
        case ITEM_COMMAND_DROP:
          AudioCommand_PlayFar(SE_MENU_CONFIRM);
          break;
        case ITEM_COMMAND_REMOVE:
          AudioCommand_PlayFar(SE_117); // Remove
          break;
        default:
          AudioCommand_PlayFar(SE_MENU_CONFIRM);
          break;
        }
        menu->listMode = commandIndex;
        break;
      }
    }
    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      commandIndex = -1;
      menu->listMode = commandIndex;
      break;
    }

    if ((gKeysRepeat & KEY_UP) != 0) {
      row -= 1;
      redraw = 1;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
    } else if ((gKeysRepeat & KEY_DOWN) != 0) {
      row += 1;
      redraw = 1;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
    } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
      column += 1;
      redraw = 1;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
    } else if ((gKeysRepeat & KEY_LEFT) != 0) {
      column -= 1;
      redraw = 1;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
    }
  }

  menu->equipPreview = 0;
  EquipmentMenu_StartCompatibilityIndicators();
  return commandIndex;
}

/// Fills commandStates (indexed by ItemMenuCommand) with
/// ITEM_COMMAND_STATE_AVAILABLE or _DISABLED for the selected item: Use for
/// items Item_ClassifyUseMode accepts and are not broken, and for field items,
/// Equip for equipment the owner can wear and is not wearing, Remove for worn
/// equipment that is not cursed, Give with someone to give to, Drop unless the
/// item is indispensable, and neither for cursed equipment being worn. Details
/// is always available. Each later test overrides the earlier ones.
void ItemMenu_BuildCommands(s8 *commandStates) {
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;

  menu = gMenuWork;
  item = Item_GetDirectFar(ITEM_ID_MASK & menu->selectedItems[0]);

  if (item->type == 0) { // usable items
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;
    commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_DISABLED;
  } else {
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;
    commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_AVAILABLE;
  }

  if (Item_ClassifyUseMode(menu->paneOwner[0], menu->selectedItems[0]) != -1)
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;
  else
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;

  if (menu->selectedItems[0] & INVENTORY_BROKEN)
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;

  if (Item_CanOwnerEquip(menu->paneOwner[0],
                         menu->selectedItems[0] & ITEM_ID_MASK) == 0) {
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

  if (BattleFx_HasTriggerFar(menu->selectedItems[0] & ITEM_ID_MASK) != 0)
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;

  if (menu->partyCount <= 1)
    commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_DISABLED;

  if (item->flags & ITEM_INDISPENSABLE)
    commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_DISABLED;
}

#define TEXT_COLOR_DISABLED 14 ///< the grey of a command that refuses A

/// Writes the six command names into window, the disabled ones in grey.
void ItemMenu_DrawCommands(const s8 *commandStates, struct UiWindow *window) {
  s32 disabled;
  s32 useState;
  u32 firstCommandMessage;

  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  useState = commandStates[ITEM_COMMAND_USE];
  disabled = ITEM_COMMAND_STATE_DISABLED;
  if (useState == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);

  firstCommandMessage = MsgItemCommandUse;
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage, window, 0, 24); // "Use"
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_EQUIP] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage + 1, window,
                                  ITEM_COMMAND_TEXT_X,
                                  24); // "Equip"
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_GIVE] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage + 2, window, 0,
                                  32); // "Give"
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_DROP] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage + 3, window, 80,
                                  32); // "Drop"
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_INSPECT] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage + 4, window, 80,
                                  24); // "Details"
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_REMOVE] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  // "Remove"
  UiText_DrawCharacterAtOffsetFar(firstCommandMessage + 5, window,
                                  ITEM_COMMAND_TEXT_X, 32);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
}

/// Says how ownerId can use inventoryEntry from the item menu:
/// 0  the item works on the field (BattleFx_HasTriggerFar), or its action is
///    usable neither in the field nor in battle, and is used once the menu
///    closes;
/// 1  usable in the field: choose a party member to use it on;
/// 2  usable in the field on the whole party at once (range 0xff);
/// -1 it cannot be used here: it has no action, it is equipment the owner
///    cannot wear or of use type 3, or its action is usable only in battle.
/// Usable items always qualify; equipment only when the owner can wear it.
s32 Item_ClassifyUseMode(s32 ownerId, s32 inventoryEntry) {
  s32 itemId = inventoryEntry;
  struct ItemDefinition *itemDefinition;
  s32 result = -1;

  itemId &= ITEM_ID_MASK;
  itemDefinition = Item_GetDirectFar(itemId);

  if (BattleFx_HasTriggerFar(itemId) != 0) {
    return 0;
  }

  {
    struct BattleAct *action =
        BattleActGet(itemDefinition->actionId & OWNER_ACTION_ID_MASK);

    if (itemDefinition->actionId != 0) {
      if (itemDefinition->type != 0) { // equipment
        if (itemDefinition->useType != 3) {
          if (Item_CanOwnerEquip(ownerId, itemId) != 0) {
            result = 1;
          }
        }
      } else {
        result = 1;
      }

      if (result == 1) {
        s32 mask = ACTION_USABLE_IN_BATTLE;
        u8 flags = action->flags;

        if ((flags & ACTION_USABLE_IN_FIELD) != 0) {
          u8 range = action->range;
          result = (range == 0xff) ? 2 : 1;
        } else {
          result = (flags & mask) ? -1 : 0;
        }
      }
    }
  }

  return result;
}

/// After the selected item is used: an item of use type 2 (ITEM_USE_MAY_BREAK)
/// breaks one time in eight, with the break sound and "It broke!" in the info
/// window. Called only after a successful use on a chosen member.
void ItemMenu_TryBreak(void) {
  struct InventoryMenuState *menu;

  menu = gMenuWork;
  // One time in eight: 0x2000 of Random16's 0x10000 values.
  if ((Item_GetDirectFar(ITEM_ID_MASK & menu->selectedItems[0])->useType ==
       2) &&
      (Random16() < 0x2000U)) {
    Inventory_BreakFar(menu->paneOwner[0], menu->selectedSlots[0]);
    AudioCommand_PlayFar(SE_ITEM_BREAK);
    InventoryMenu_ShowModalMessage(MsgItemBroke, 0, -1);
#if !TBS_PATCH_1
    RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
  }
}

/// Opens the details window (top left, 13 by 10 tiles) and describes the item
/// in pane itemIndex there. Returns 1. The window is kept in the work's
/// equipWindow, which ItemMenu_Close closes. Nothing calls it: the Details
/// command uses Menu_SelectQuantity.
s32 ItemMenu_OpenDetail(s32 itemIndex) {
  struct InventoryMenuState *menu = gMenuWork;

  UiWindow_UpdateOrCreate(&menu->equipWindow, 0, 0, 13, 10, 2);
  Palette_CopyObjectBankToBackground14();

  if (menu->selectedItems[itemIndex] != 0)
    ItemMenu_DrawItemDetails(menu->equipWindow, menu->selectedItems[itemIndex]);

  return 1;
}

/// The item menu's Details screen: describes the item entry value in a large
/// window (the whole top half after patch 1) and waits for A or B. Up and Down
/// move an unused counter from 0 to 4. Returns the counter after A or Start,
/// or -1 after B; ITEM_STEP_INSPECT ignores it. Borrows background palette 14
/// for the element icons and makes the highlight again before returning.
s32 Menu_SelectQuantity(s32 value) {
#if TBS_PATCH_1
  s32 changed = 1;
  struct InventoryMenuState *menu = gMenuWork;
  struct RenderOutput *itemIcon = menu->selectedItemIcon;
#else
  struct InventoryMenuState *menu = gMenuWork;
  s32 changed = 1;
#endif
  struct UiWindow *window;
  s32 counter = 0;

#if TBS_PATCH_1
  itemIcon->active = ICON_HIDDEN;
#endif
  window = UiWindow_CreateFar(0, 0, ITEM_QUANTITY_WINDOW_WIDTH, 10, 2);
  // Hide the party figures (without their placement callback they are not
  // drawn) and the status panel's portrait (the work's cursor), which the
  // window covers, meanwhile.
  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);

  {
    struct RenderOutput *cursor = menu->cursor;
    cursor->active = ICON_HIDDEN;
  }
  Palette_CopyObjectBankToBackground14();
  WaitFrames(1);

  for (;;) {
    if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0)
      break;

    if (changed != 0) {
      changed = 0;
      counter = (counter + 5) % 5;
      ItemMenu_DrawItemDetails(window, value);
    }

    if (gKeysPressed & KEY_A)
      break;
    if (gKeysPressed & KEY_B) {
      counter = -1;
      break;
    }

    if (gKeysRepeat & KEY_UP) {
      counter -= 1;
      changed = 1;
    }
    if (gKeysRepeat & KEY_DOWN) {
      counter += 1;
      changed = 1;
    }
    WaitFrames(1);
  }

  // Close the window and bring the menu back.
  RenderOutput_RedrawSavedRectFar(window);
  WaitFrames(1);
  UiWork_FinalizeFar((void *)window, 1);
  RenderOutput_RedrawSavedRectFar(menu->mainWindow);
  Palette_LightenBankHighlight(14);
  // SCHEDULER_KEY_MAIN_LOOP, as PsynergyMenu_InitializeEntryObjects
  // schedules it.
  {
    s32 priority = 0xc80;

    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, priority);
  }

  // The Japanese window, 13 tiles wide, leaves the command grid at tile 13
  // up, so flags 1 tells ItemMenu_ChooseCommand not to draw it again; the
  // international one, 30 wide, covers it, and the grid's frame is redrawn.
  {
    struct RenderOutput *cursor = menu->cursor;
#if TBS_PATCH_1
    cursor->active = ICON_SHOWN;
#else
    u8 one = 1;

    cursor->active = one;
    menu->flags = one;
#endif
  }
#if LANGUAGE != LANGUAGE_JAPANESE
  UiWindow_DrawFrameFar(13, 0, 17, 10);
#endif

  return counter;
}

/// Writes the description of the inventoryEntry into window, one line a row:
/// for equipment its Attack and Defense bonuses, then each of its effects
/// (flat bonuses, rates as multipliers, elemental Power and Resist with the
/// element's icon, and the effects that are only text), whether it is cursed or
/// bestows Psynergy, whether it is used up or may break, and for a stack how
/// many there are. An item with nothing to say is rare, important, or
/// undescribed.
void ItemMenu_DrawItemDetails(struct UiWindow *window, s32 inventoryEntry) {
  struct ItemDefinition *def;
  s32 i;
  s32 hasEffects;
  s32 amount;
  s32 bestowsPsynergy;
  s8 row;

  row = 0;
  bestowsPsynergy = 0;
  def = Item_GetDirectFar(inventoryEntry & ITEM_ID_MASK);
  // Equipment: a heading, then its Attack and Defense bonuses.
  if (def->type != 0) {
    hasEffects = 0;
    if (def->primaryBonus == 0 && def->secondaryBonus == 0) {
      for (i = 0; i < 4; i++) {
        if (def->effects[i].kind != 0 || def->useType == 3) {
          hasEffects = 1;
          break;
        }
      }
    } else {
      hasEffects = 1;
    }
    if (hasEffects == 1) {
      UiText_DrawCharacterAtOffsetFar(MsgEquipEffectHeading, window, 16, 0);
      row = 1;
    }
    if (def->primaryBonus != 0) {
      UiText_DrawCharacterAtOffsetFar(MsgStatLabel, window, 0, row * 8);
      amount = def->primaryBonus;
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      row++;
    }
    if (def->secondaryBonus != 0) {
      UiText_DrawCharacterAtOffsetFar(MsgDefenseLabel, window, 0, row * 8);
      amount = def->secondaryBonus;
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      row++;
    }
  }
  // Each of its effects.
  for (i = 0; i < 4; i++) {
    if (def->effects[i].kind == 0)
      continue;
    amount = def->effects[i].amount;
    switch (def->effects[i].kind) {
    case ITEM_EFFECT_NONE:
      break;
    case ITEM_EFFECT_ADD_HP:
    case ITEM_EFFECT_ADD_HP_REGEN:
    case ITEM_EFFECT_ADD_PP:
    case ITEM_EFFECT_ADD_PP_REGEN:
    case ITEM_EFFECT_ADD_AGILITY:
    case ITEM_EFFECT_ADD_LUCK:
    case 26: // Turns
      UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + MsgEquipEffectName,
                                      window, 0, row * 8);
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      break;
    // Power (15 to 18) and Resist (19 to 22) of the four elements, after
    // the element's icon.
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
      UiWindow_SetTilemapEntryFar(
          window, (u8)((def->effects[i].kind - 15) % 4) + 1, 0, row, 2);
      UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + MsgEquipEffectName,
                                      window, 8, row * 8);
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      break;
    // Rates, in tenths: shown as a multiplier from 0.0 to 1.9.
    case ITEM_EFFECT_SCALE_HP:
    case ITEM_EFFECT_SCALE_HP_REGEN:
    case ITEM_EFFECT_SCALE_PP:
    case ITEM_EFFECT_SCALE_PP_REGEN:
    case ITEM_EFFECT_SCALE_ATTACK:
    case ITEM_EFFECT_SCALE_DEFENSE:
    case ITEM_EFFECT_SCALE_AGILITY:
    case ITEM_EFFECT_SCALE_LUCK:
      UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + MsgEquipEffectName,
                                      window, 0, row * 8);
#if LANGUAGE != LANGUAGE_JAPANESE
      UiText_DrawStringInWindowFar(gMenu_TimesString, window, 64, row * 8);
      if (amount > 9) {
        UiText_DrawNumberInWindowFar(1, 1, window, 72, row * 8);
        UiText_DrawStringInWindowFar(gMenu_PointString, window, 80, row * 8);
        UiText_DrawNumberInWindowFar(amount - 10, 1, window, 88, row * 8);
      } else {
        UiText_DrawNumberInWindowFar(0, 1, window, 72, row * 8);
        UiText_DrawStringInWindowFar(gMenu_PointString, window, 80, row * 8);
        UiText_DrawNumberInWindowFar(amount, 1, window, 88, row * 8);
      }
#else
      if (amount > 9) {
        UiText_DrawNumberInWindowFar(1, 1, window, 48, row * 8);
        UiText_DrawStringInWindowFar(gMenu_PointString, window, 56, row * 8);
        UiText_DrawNumberInWindowFar(amount - 10, 1, window, 64, row * 8);
      } else {
        UiText_DrawNumberInWindowFar(0, 1, window, 48, row * 8);
        UiText_DrawStringInWindowFar(gMenu_PointString, window, 56, row * 8);
        UiText_DrawNumberInWindowFar(amount, 1, window, 64, row * 8);
      }
      UiText_DrawCharacterAtOffsetFar(MsgEffectRateSuffix, window, 72, row * 8);
#endif
      break;
    case 23: // "Critical Hits increase"
    case 25: // "The curse can be removed"
    case 27: // "Boosts monster encounters"
      UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + MsgEquipEffectName,
                                      window, 0, row * 8);
      break;
    }
    row++;
  }
  // Curses, bestowed Psynergy and how the item is used up.
  if (def->flags & ITEM_CURSED) {
    UiText_DrawCharacterAtOffsetFar(MsgItemCursed, window, 0, row * 8);
    row++;
  }
  if (def->useType == 3) { // bestows Psynergy
    UiText_DrawCharacterAtOffsetFar(MsgBestowsPsynergy, window, 0, row * 8);
    bestowsPsynergy = 1;
    row++;
  }
  // "Number of Uses" and what using it costs, except for items that cost
  // nothing and Hermes' Water (ITEM_USE_EMPTIES); a Psynergy item has no
  // heading and no line.
  if (def->useType != 4 && def->useType != 0) {
    if (!bestowsPsynergy) {
      UiText_DrawCharacterAtOffsetFar(MsgUsesHeading, window, 16, row * 8);
      row++;
    }
    switch (def->useType) {
    case 0:
      break;
    case 1: // used up
      UiText_DrawCharacterAtOffsetFar(MsgSingleUse, window, 0, row * 8);
      row++;
      break;
    case 2: { // may break
      s32 m;

      // Two lines each: "It's broken, so" / "it can't be used", or "It might
      // break" / "if used in battle".
      if (inventoryEntry & INVENTORY_BROKEN) {
        m = MsgBrokenNotice;
        UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
        m++;
        UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
      } else {
        m = MsgMightBreak;
        UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
        m++;
        UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
      }
      break;
    }
    }
  }
  // How many there are in a stack.
  if (def->flags & ITEM_STACKABLE) {
    if (row != 0)
      row++;
    i = MsgNumberHeading;
    UiText_DrawCharacterAtOffsetFar(i, window, 16, row * 8);
    row++;
    amount =
        (inventoryEntry & INVENTORY_QUANTITY_MASK) / INVENTORY_QUANTITY_STEP;
    // The quantity field holds the count less one. The international
    // editions pass it as the argument of "I have <number>."; Japanese draws
    // the number before its label.
#if LANGUAGE != LANGUAGE_JAPANESE
    UiWork_PushValueSlotFar(amount + 1, 5);
    UiText_DrawCharacterAtOffsetFar(i + 1, window, 0, row * 8);
#else
    UiText_DrawNumberInWindowFar(amount + 1, 2, window, 0, row * 8);
    UiText_DrawCharacterAtOffsetFar(i + 1, window, 16, row * 8);
#endif
    row++;
  }
  // Nothing written yet: rare, important, or nothing known.
  hasEffects = 0;
  if (row == 0) {
    if (def->flags & 4) { // rare
      UiText_DrawCharacterAtOffsetFar(MsgRareItem, window, 0, 0);
      hasEffects = 1;
    }
    if (!hasEffects) {
      if (def->flags & ITEM_INDISPENSABLE) {
        UiText_DrawCharacterAtOffsetFar(MsgImportantItem, window, 0, 0);
        hasEffects = 1;
      }
      if (!hasEffects)
        UiText_DrawCharacterAtOffsetFar(MsgDetailsUnknown, window, 0, 0);
    }
  }
}

/// Writes a signed stat change at (x, y) of window: the number, with a plus or
/// minus sign just before its first digit.
void ItemMenu_DrawStat(s32 statChange, s32 unused, struct UiWindow *window,
                       s32 x, s32 y) {
  s32 digits;
  s32 magnitude;

  UiText_DrawNumberInWindowFar(statChange, 3, window, x, y);
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
    UiText_DrawStringInWindowFar(&gMenu_PlusSignString, window,
                                 x - digits * 8 + 16, y);
  } else {
    UiText_DrawStringInWindowFar(&gMenu_MinusSignString, window,
                                 x - digits * 8 + 16, y);
  }
}

/// Moves the item window to row 5 of the right-hand side.
void ItemMenu_SetItemWindowRow5(void) {
  UiWindow_SetBounds(
      (struct UiWindow *)((struct InventoryMenuState *)gMenuWork)->itemWindow,
      13, 5, 17, 10);
}

/// Moves the item window to row 3 of the right-hand side.
void ItemMenu_SetItemWindowRow3(void) {
  UiWindow_SetBounds(
      (struct UiWindow *)((struct InventoryMenuState *)gMenuWork)->itemWindow,
      13, 3, 17, 10);
}

/// Makes the message window, along the top right, 5 rows tall.
void ItemMenu_SetMessageWindowHeight5(void) {
  UiWindow_SetBounds((struct UiWindow *)((struct InventoryMenuState *)gMenuWork)
                         ->messageWindow,
                     13, 0, 17, 5);
}

void ItemMenu_SetMessageWindowHeight6(void) {
  UiWindow_SetBounds((struct UiWindow *)((struct InventoryMenuState *)gMenuWork)
                         ->messageWindow,
                     13, 0, 17, 6);
}

void ItemMenu_SetMessageWindowHeight7(void) {
  UiWindow_SetBounds((struct UiWindow *)((struct InventoryMenuState *)gMenuWork)
                         ->messageWindow,
                     13, 0, 17, 7);
}

void ItemMenu_SetMessageWindowHeight3(void) {
  UiWindow_SetBounds((struct UiWindow *)((struct InventoryMenuState *)gMenuWork)
                         ->messageWindow,
                     13, 0, 17, 3);
}

/// Asks how many of the selected item stack to give or drop: Left and Right
/// choose a number from 1 to quantityRange (shown as baseQuantity + 1 up to
/// baseQuantity + quantityRange on a dial of digit sprites), with the owner's
/// remaining count and, when giving (dropMode 0), the receiver's new count
/// beside their names. Returns the number less one, or -1 when the player backs
/// out.
s32 ItemMenu_SelectGiveQuantity(s32 baseQuantity, s32 quantityRange,
                                s32 dropMode) {
  s32 quantity = baseQuantity;
  s32 block;
  s32 changed;
  s32 receiverCount;
  s32 ownerCount;
  struct InventoryMenuState *menu = gMenuWork;
  // 1 KB of heap slot 14 to draw the dial's digits in, released before
  // returning.
  void *tiles = Runtime_AllocateBlock(14, 0x400);
  struct UiWindow *window;
  struct RenderOutput *rightDial;

  changed = 1;
  receiverCount = 0;
  window = menu->messageWindow;
  ItemMenu_SetMessageWindowHeight7();
  RenderOutput_RedrawSavedRectFar(window);
  // How many each member holds now.
  if (dropMode == 0)
    receiverCount = InventoryMenu_GetItemQuantity(
        menu->paneOwner[1], menu->selectedItems[0] & ITEM_ID_MASK);
  ownerCount = InventoryMenu_GetItemQuantity(
      menu->paneOwner[0], menu->selectedItems[0] & ITEM_ID_MASK);
  // Two dial sprites, left and right of the number.
  block = Resource_FindFreeEntry();
  if (block != 96) { // 96 (VRAM_CACHE_ENTRY_COUNT): no free VRAM block
    VramCache_Load(block, 256, 0);
    RenderOutput_CreateFar(block, 0x40004000, window, 48, 32);
    rightDial = RenderOutput_CreateFar(block, 0x40004000, window, 80, 32);
    rightDial->oam.index += 4;
    UiMenu_SlideCursor(ITEM_GIVE_CURSOR_X, 40);
    while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
      if (changed) {
        changed = 0;
        quantity = (quantityRange + quantity) % quantityRange;
        RenderOutput_RedrawSavedRectFar(window);
        UiText_DrawCharacterAtOffsetFar(MsgGiveHowMany, window, ITEM_GIVE_ASK_X,
                                        ITEM_GIVE_ASK_Y);
        Dma_Set(gMenu_QuantityTiles, tiles,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x40, REG_DMA3);
        Shop_FillSelectorFar(30, 14, tiles);
        Shop_FillSelectorFar(quantityRange + baseQuantity, 0, tiles);
        Shop_FillSelectorFar(baseQuantity + quantity + 1, 10, tiles);
        Shop_FillSelectorFar(baseQuantity, 2, tiles);
        VramCache_Load(block, 256, tiles);
        UiText_DrawNumberInWindowFar(quantity + 1, 2, window, 32, 32);
        UiText_DrawCharacterAtOffsetFar(
            (menu->selectedItems[0] & ITEM_ID_MASK) + MsgItemName, window, 16,
            8);
        UiText_DrawNumberInWindowFar(ownerCount - quantity - 1, 2, window, 16,
                                     24);
#if LANGUAGE != LANGUAGE_JAPANESE
        if (dropMode == 0)
          UiText_DrawNumberInWindowFar(receiverCount + quantity + 1, 2, window,
                                       80, 24);
#else
        UiText_DrawCharacterAtOffsetFar(MsgItemCounter, window, 32, 24);
        if (dropMode == 0) {
          UiText_DrawNumberInWindowFar(receiverCount + quantity + 1, 2, window,
                                       80, 24);
          UiText_DrawCharacterAtOffsetFar(MsgItemCounter, window, 96, 24);
        }
#endif
        UiText_DrawStringAtOffsetFar(UnitGetFar(menu->paneOwner[0])->name,
                                     window, 16, 16);
        if (dropMode == 0)
          UiText_DrawStringAtOffsetFar(UnitGetFar(menu->paneOwner[1])->name,
                                       window, 80, 16);
      }
      // A confirms, B backs out, Left and Right change the number.
      if (gKeysPressed & KEY_A) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        break;
      }
      if (gKeysPressed & KEY_B) {
        quantity = -1;
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        break;
      }
      UiMenu_PositionCursor(ITEM_GIVE_CURSOR_X, 40);
      if (gKeysRepeat & KEY_LEFT) {
        quantity--;
        changed = 1;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      }
      if (gKeysRepeat & KEY_RIGHT) {
        quantity++;
        changed = 1;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      }
      WaitFrames(1);
    }
  }
  RenderOutput_RedrawSavedRectFar(window);
  RenderOutput_ClearListFar((void *)window);
  Runtime_ReleaseHeapBlock(14);
  menu->selectedItemIcon->active = ICON_HIDDEN;
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
    quantity = -1;
  return quantity;
}

/// Shows the selected item at the top of the message window: its icon, its
/// owner's name and its name.
void ItemMenu_DrawItemHead(void) {
  struct InventoryMenuState *menu = gMenuWork;

  Resource_LoadByModeIntoSlotFar(2, menu->selectedItems[0],
                                 (u8)menu->selectedItemIcon->index, 0);
  menu->selectedItemIcon->active = ICON_SHOWN;
  menu->selectedItemIcon->x = 112;
  menu->selectedItemIcon->y = 8;
  UiIcon_PrepareObject(menu->selectedItemIcon);
  UiText_DrawStringAtOffsetFar(UnitGetFar(menu->paneOwner[0])->name,
                               menu->messageWindow, 16, 0);
  UiText_DrawCharacterAtOffsetFar((menu->selectedItems[0] & ITEM_ID_MASK) +
                                      MsgItemName,
                                  menu->messageWindow, 16, 8);
}

/// Asks whether to drop item (an inventory entry): its name, the question, and
/// Yes or No, No first. Returns 0 for Yes and 1 for No, B or Start.
s32 ItemMenu_ConfirmDrop(s32 item) {
  struct UiWindow *window;
  s32 itemId;
  s32 question;
  s32 answer;
  s32 rows = 2;
  s32 selection = 1;
  s32 changed = 1;

  window = UiWindow_CreateFar(13, 3, 17, 10, 2);
  itemId = item & ITEM_ID_MASK;
  Item_GetDirectFar(itemId);
  UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window, 24, 0);
  question = MsgConfirmDrop;
  UiText_DrawCharacterAtOffsetFar(question, window, ITEM_DROP_QUESTION_X, 16);
  question++;
  UiText_DrawCharacterAtOffsetFar(question, window, ITEM_DROP_QUESTION_X, 24);
  answer = MsgYes;
  UiText_DrawCharacterAtOffsetFar(answer, window, 24, 40);
  answer++;
  UiText_DrawCharacterAtOffsetFar(answer, window, 24, 56);
  UiMenu_SlideCursor(104, 86);
  for (;;) {
    if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
      break;
    if (changed) {
      changed = 0;
      selection = (selection + rows) % rows;
    }
    if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      break;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      selection = 1;
      break;
    }
    UiMenu_PositionCursor(104, (selection << 4) + 70);
    if (gKeysRepeat & KEY_UP) {
      selection--;
      changed = 1;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      selection++;
      changed = 1;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
    }
    WaitFrames(1);
  }
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    selection = 1;
  }
  UiWork_FinalizeFar((void *)window, 1);
  return selection;
}

/// After giving or trading the selected item to the member in pane 1, offers to
/// equip it there, showing the stats it would give. Returns 0 when it is
/// equipped; 1 when the player says No, backs out, or the member cannot equip
/// it, and the member is left as before.
s32 ItemMenu_ConfirmEquip(s32 unused) {
  s32 rows = 2;
  s32 selection = 0;
  s32 changed = 1;
  struct InventoryMenuState *menu = gMenuWork;
  struct BattleUnit *unit = UnitGetFar(menu->paneOwner[1]);
  struct BattleUnit *saved;
  struct UiWindow *window;
  s32 slot;
  s32 owner;

  slot = menu->selectedSlots[1];
  owner = menu->paneOwner[1];
  ItemMenu_DrawEquipPreview(owner, slot, 0, owner);
  saved = MemAlloc(BATTLE_UNIT_SIZE);
  Iwram_CopyWords(saved, unit, BATTLE_UNIT_SIZE);
  window = menu->messageWindow;
  // Equip it now; a result of -1 or -2 means it cannot be equipped.
  if ((u32)(Inventory_EquipFar(menu->paneOwner[1], menu->selectedSlots[1]) +
            2) <= 1) {
    selection = 1;
  } else {
#if LANGUAGE != LANGUAGE_JAPANESE
    UiText_DrawCharacterAtOffsetFar(MsgYes, window, 24, 24);
    UiText_DrawCharacterAtOffsetFar(MsgYes + 1, window, 72, 24);
    UiWindow_ClearInteriorTilesFar((void *)window, 16, 16, 96, 24);
    UiText_DrawCharacterAtOffsetFar(MsgEquipThisItem, window, 0, 16);
    UiMenu_SlideCursor(110, 32);
#else
    UiText_DrawCharacterAtOffsetFar(MsgYes, window, 96, 0);
    UiText_DrawCharacterAtOffsetFar(MsgYes + 1, window, 96, 16);
    UiWindow_ClearInteriorTilesFar((void *)window, 16, 16, 96, 24);
    UiText_DrawCharacterAtOffsetFar(MsgEquipThisItem, window, 8, 16);
    UiMenu_SlideCursor(184, 5);
#endif
    for (;;) {
      if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
        break;
      if (changed) {
        changed = 0;
        selection = (selection + rows) % rows;
      }
      if (gKeysPressed & KEY_A) {
        AudioCommand_PlayFar(SE_175); // the Equip sound
        break;
      }
      if (gKeysPressed & KEY_B) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        selection = 1;
        break;
      }
#if LANGUAGE != LANGUAGE_JAPANESE
      UiMenu_PositionCursor(selection * 48 + 110, 32);
      if (gKeysRepeat & KEY_LEFT) {
#else
      UiMenu_PositionCursor(184, (selection << 4) + 5);
      if (gKeysRepeat & KEY_UP) {
#endif
        selection--;
        changed = 1;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      }
#if LANGUAGE != LANGUAGE_JAPANESE
      if (gKeysRepeat & KEY_RIGHT) {
#else
      if (gKeysRepeat & KEY_DOWN) {
#endif
        selection++;
        changed = 1;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      }
      WaitFrames(1);
    }
  }
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
    selection = 1;
  // No: put the member back as they were.
  if (selection == 1)
    Iwram_CopyWords(unit, saved, BATTLE_UNIT_SIZE);
  MemFree(saved);
  UnitCalcFar(menu->paneOwner[1]);
  Unit_RefreshPsynergyFar(menu->paneOwner[1]);
  return selection;
}

/// Loads the stat arrows (up and down) into two free VRAM blocks, kept in the
/// menu work's resource slots.
void Resource_LoadPairedBlocks(void) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 block = Resource_FindFreeEntry();

  menu->resourceSlots[0] = block;
  VramCache_Load(block, 128, gMenu_ArrowTiles);
  block = Resource_FindFreeEntry();
  menu->resourceSlots[1] = block;
  VramCache_Load(block, 128, gMenu_SmallDownArrowTiles);
}

/// Fills result with the item list of the member in pane index: their unit,
/// how many items they hold, and the page and row of the item chosen last for
/// them (kept within the list), five items to a page. Returns 1.
s32 ItemMenu_PageResult(struct MenuResult *result, s32 index) {
  s32 unit;
  struct InventoryMenuState *menu = gMenuWork;
  s32 itemCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selected;

  itemCount = ItemMenu_CountItems(menu->paneOwner[index]);
  unit = (s32)UnitGetFar(menu->paneOwner[index]);
  selected = menu->selectedIndexByOwner[menu->paneOwner[index]];
  if ((selected + 1) > itemCount) {
    selected = itemCount - 1;
  }
  page = selected / LIST_PAGE_SIZE;
  row = selected % LIST_PAGE_SIZE;
  pageCount = itemCount / LIST_PAGE_SIZE;
  if (itemCount % LIST_PAGE_SIZE != 0) {
    pageCount++;
  }
  result->ownerState = unit;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = itemCount;
  result->selectedIndex = selected;
  return 1;
}

/// Updates the item list for the row under the cursor: puts the selected item's
/// plain name in the info window and highlights its row of the five. window and
/// ownerWork are not used.
s32 ItemMenu_DrawItemDetailPage(struct UiWindow *window, void *ownerWork,
                                struct MenuResult *pageState) {
  struct InventoryMenuState *menu;
  s32 page;
  s32 index;
  s32 row;

  page = pageState->page;
  menu = gMenuWork;
  index = page * LIST_PAGE_SIZE;
  index += pageState->row;
  pageState->selectedIndex = index;

  MENU_RESET_INFO_WINDOW(menu->infoWindow);
  WaitFrames(1);

  index = pageState->selectedIndex;
  if (menu->items[index] != 0) {
    s32 name = (menu->items[index] & ITEM_ID_MASK) + MsgItemPlainName;
    MENU_DRAW_INFO_TEXT(name, menu->infoWindow, 0, 0);
  }

  row = 0;
  do {
    if (row == pageState->row) {
      Render_SetTilemapFlagRect((void *)menu->itemWindow, 1, row * 2 + 1, 14, 1,
                                14); // highlighted
    } else {
      Render_SetTilemapFlagRect((void *)menu->itemWindow, 1, row * 2 + 1, 14, 1,
                                15); // normal
    }
    row++;
  } while (row < LIST_PAGE_SIZE);

  WaitFrames(1);
  return 1;
}

/// Draws one page of the item list into window: up to five item names with
/// their icons, and the page numbers when there is more than one page.
s32 ItemMenu_DrawNamePage(struct UiWindow *window, s32 unused,
                          const struct MenuResult *pageState) {
  struct InventoryMenuState *menu = gMenuWork;
  u32 page;
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  const u16 *entry;

  (void)unused;

  RenderOutput_RedrawSavedRectFar(window);
  UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

  page = pageState->page;
  firstEntry = page * LIST_PAGE_SIZE;
  visibleCount = (u8)(pageState->entryCount - firstEntry);
  if (visibleCount > LIST_PAGE_SIZE) {
    visibleCount = LIST_PAGE_SIZE;
  }

  Menu_SetPageIcons(LIST_PAGE_SIZE, firstEntry, window, ITEM_LIST_PAGE_ICONS_X,
                    34);
  Menu_DrawPageIndicator(window, pageState->entryCount, LIST_PAGE_SIZE,
                         pageState->page, 15);

  row = 0;
  if (visibleCount > row) {
    entry = &menu->items[firstEntry];
    do {
      UiText_DrawCharacterAtOffsetFar((entry[0] & ITEM_ID_MASK) + MsgItemName,
                                      menu->itemWindow, ITEM_LIST_NAME_X,
                                      row * 16 + 8);
      row++;
      entry++;
    } while (visibleCount > row);
  }

  return 1;
}

/// Nothing calls it.
UNUSED s32 InventoryMenu_ItemNamePageReturnTrue(void) { return 1; }

/// Nothing calls it.
UNUSED void InventoryMenu_ItemNamePageNoOp(void) {}

/// The item list of the member in paneIndex (0: the owner, 1: the receiver of a
/// trade), five items a page. The cursor's item is named in the info window and
/// previewed in the status panel, its icon animates, and the party members who
/// can equip it move. Up and Down move the cursor, Left and Right turn pages,
/// and in paneIndex 0 L and R switch to the previous or next member who has
/// items. Returns the chosen inventory entry, or -1 when the player backs out.
s32 ItemMenu_RunList(s32 paneIndex) {
  struct InventoryMenuState *menu;
  struct UiWindow *window;
  s32 changed;
  s32 moved;
  s32 member;
  u8 i;
  s32 result;
  s32 redraw;
  s32 done;
  struct BattleUnit *owner;
  s32 previous;
  struct RenderOutput *icon;
  s32 work[5];
  struct MenuResult state;

  menu = gMenuWork;
  result = 0;
  previous = 0;
  menu->equipPreview = result;
  UiWindow_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, 2);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    owner = UnitGetFar(menu->paneOwner[paneIndex]);
    menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
    ItemMenu_DrawIcons(menu->items, 0);
    menu->selectedItemIcon->active = ICON_HIDDEN;
    ItemMenu_PageResult(&state, paneIndex);
    UiMenu_PositionCursor(98, state.selectedIndex * 16 + 36);
    changed = 1;
    redraw = 1;

    // One member's list, until an item is chosen or the member changes.
    while (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
      UiMenu_PositionCursor(98, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (owner->inventory[previous] != 0) {
          icon = menu->entryIcons[previous];
          UiIcon_PrepareObject(icon);
        }
        if (redraw != 0) {
          WaitFrames(1);
          ItemMenu_DrawNamePage(window, 0, &state);
          if (paneIndex == 0) {
            UiText_DrawCharacterAtOffsetFar(MsgChangeCharacterHelp, window,
                                            HELP_TEXT_X, 88);
          }
          redraw = 0;
        }
        ItemMenu_DrawItemDetailPage(window, work, &state);
        menu->selectedItems[paneIndex] = menu->items[state.selectedIndex];
        ItemMenu_DrawEquipPreview(menu->paneOwner[paneIndex],
                                  state.selectedIndex, 0,
                                  menu->paneOwner[paneIndex]);
        if (menu->items[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          // Animate the selected icon: 9 is UI_SPRITE_SWELL, pulsing between
          // 1 and 1.5 times its size, from its first step, in depth slot 250.
          // UiIcon_PrepareObject stills it again when the cursor leaves.
          icon->active = 9;
          icon->animStep = 0;
          icon->sentinel = 250;
        }
        for (i = 0; i < menu->partyCount; i++) {
          AnimationObjects_SelectAnimationFar(menu->ownerObjects[i], 1);
        }
      }

      // Every 32 frames, the members who can equip the item move.
      if ((gFrameCount & 31) == 0) {
        for (i = 0; i < menu->partyCount; i++) {
          if (Item_CanOwnerEquip(menu->ownerIds[i],
                                 ITEM_ID_MASK &
                                     menu->items[state.selectedIndex]) != 0) {
            AnimationObjects_SelectAnimationFar(menu->ownerObjects[i], 3);
          }
        }
      }

      WaitFrames(1);
      previous = state.selectedIndex;
      moved = Menu_HandlePageInput(0, state.entryCount, LIST_PAGE_SIZE,
                                   &state.row, &state.page);
      if (moved == 1) {
        redraw = 1;
        changed = 1;
      }
      if (moved == 0) {
        changed = 1;
      }
      if (moved == -1) {
        changed = 0;
      }

      if ((gKeysPressed & KEY_A) != 0 &&
          menu->items[state.selectedIndex] != 0) {
        AudioCommand_PlayFar(SE_173); // the item choice sound
        result = menu->items[state.selectedIndex];
        done = 1;
        break;
      }
      if ((gKeysPressed & KEY_B) != 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }
      // L and R: the previous or next member who has items (pane 0 only).
      if ((gKeysRepeat & KEY_R) != 0 || (gKeysRepeat & KEY_L) != 0) {
        if (paneIndex == 1) {
          AudioCommand_PlayFar(SE_MENU_ERROR);
          WaitFrames(1);
        } else {
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          menu->selectedIndexByOwner[menu->paneOwner[paneIndex]] =
              state.selectedIndex;
          member = menu->paneIndex[paneIndex];
          do {
            if ((gKeysRepeat & KEY_R) != 0) {
              member++;
            } else {
              member--;
            }
            member = (member + menu->partyCount) % menu->partyCount;
            menu->selectedOwner = menu->ownerIds[member];
            menu->paneOwner[paneIndex] = menu->ownerIds[member];
            menu->paneIndex[paneIndex] = member;
            menu->itemCount = ItemMenu_CollectItems(
                UnitGetFar(menu->paneOwner[paneIndex]), menu->items, 0);
          } while (menu->itemCount == 0);
          for (i = 0; i <= 3; i++) {
            menu->ownerY[i] = 30;
          }
          menu->ownerY[member] = 26;
          break;
        }
      }
    }
  }

  UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
  UiIcon_PrepareObject(menu->gridCursor);
  menu->selectedSlots[paneIndex] = state.selectedIndex;
  menu->selectedIndexByOwner[menu->paneOwner[paneIndex]] = state.selectedIndex;
  menu->selectedItems[paneIndex] = result;
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0) {
    result = -1;
  }
  WaitFrames(1);
  return result;
}

#undef ITEM_MODAL_SHORT_X
#undef ITEM_MODAL_REMOVED_X
#undef ITEM_CANNOT_TRADE_X
#undef ITEM_TRADED_X
#undef ITEM_TRADE_PROMPT_X
#undef ITEM_COMMAND_TEXT_X
#undef ITEM_QUANTITY_WINDOW_WIDTH
#undef ITEM_GIVE_CURSOR_X
#undef ITEM_GIVE_ASK_X
#undef ITEM_GIVE_ASK_Y
#undef ITEM_DROP_QUESTION_X
#undef ITEM_LIST_PAGE_ICONS_X
#undef ITEM_LIST_NAME_X
#undef ITEM_STACK_MAX
#undef ITEM_IWRAM_COPY_WORDS
#undef ITEM_COMMAND_STATE_DISABLED
#undef ITEM_COMMAND_STATE_AVAILABLE
#undef TEXT_COLOR_DISABLED

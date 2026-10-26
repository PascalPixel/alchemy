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

/// The field menu's Item screen (FieldMenu_Run, FIELD_MENU_ITEM): choose
/// a party member, one of their items and a command. Items used here take
/// effect at once; an item to be used on the field is written as the field's
/// item request (FieldWork.item and itemArgument), run when the field menu has
/// closed. Owns the menu work (HEAP_SLOT_MENU) while it runs and frees it
/// before returning. Returns ItemMenu_RunCommands' result: 1 when an item is to
/// be used on the field, -1 when the player backed out or pressed Start (the
/// field menu then opens again).
s32 ItemMenu_Run(void) {
#if TBS_PATCH_1
  void *backup;
#endif
  struct ItemMenuWork *screen;
  s32 item;
  s32 target;
  s32 member;
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
  backup = AllocEwram(size);
#endif
  screen = AllocHeapSlot(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  cache = &((union HeapState *)gHeapSlots)->slots[HEAP_SLOT_OBJECT_SYSTEM];
  ((struct ObjectSystemWork *)cache[0])->suspended = 1;
  Window_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  Window_InitializeWork(0);
  screen->partyCount = Party_ListMembersFar(screen->memberIds);
  ItemMenu_Initialize(0, 3, 0, 7);
  Resource_LoadPairedBlocks();
  Palette_LightenBankHighlight(HIGHLIGHT_PALETTE);
  Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);
  screen->questionWindow = Window_CreateFar(13, 0, 17, 3, WINDOW_FLAG_INSTANT);
  // XXX (no evidence): why the Japanese edition leaves the field's tasks
  // running behind this screen, as the first editions do behind the Psynergy
  // screen; the patches only add the calls.
#if TBS_PATCH_1
  // Save the window tiles the menu draws over, and fill them with colour 3
  // (0x33333333: eight 4-bit pixels of colour 3 a word).
  Menu_SuspendFieldTasks();
  Iwram_CopyWords(backup, BG_CHAR_BLOCK(1), size);
  Iwram_FillWords(BG_CHAR_BLOCK(1), size, 0x33333333);
  Text_UseExtraTilesFar(1);
#endif

  // Let the player choose.
  Menu_CancelSoundReset();
  result = ItemMenu_RunCommands(&member, &target, &item);
  Menu_EnsureCancelSound();
  if (result == 1) {
    struct FieldWork *selection =
        cache[HEAP_SLOT_FIELD - HEAP_SLOT_OBJECT_SYSTEM];

    selection->item = (member << 10) | (item & INVENTORY_ITEM_ID_MASK);
    selection->itemArgument = screen->selectedSlots[0];
  }

  // Close the menu and give the screen back to the field.
  WindowSprite_ClearListFar(screen->statusWindow);
  process = &cache[HEAP_SLOT_WINDOW - HEAP_SLOT_OBJECT_SYSTEM];
  ((struct WindowWork *)*process)->holdTilemapUpload = 1;
  ItemMenu_Close();
  Window_DrawFrameFar(0, 0, 30, 20);
  IconMenu_ReleaseArrows();
  FreeHeapSlot(HEAP_SLOT_MENU);
  ((struct ObjectSystemWork *)cache[0])->suspended = 0;
#if TBS_PATCH_1
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords(BG_CHAR_BLOCK(1), backup, size);
  ((struct WindowWork *)*process)->holdTilemapUpload = 0;
  Free(backup);
  WaitFrames(1);
  Menu_ResumeFieldTasks();
#endif
  WaitFrames(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  ((struct WindowWork *)*process)->holdTilemapUpload = 0;
  FieldScript_ClearInvalidPackedValuesFar();
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

/// Runs the item menu: choose a party member, one of their items and a command
/// (Use, Equip, Give, Drop, Remove or Inspect), then carry it out, until the
/// player backs out or presses Start. Returns 1 when an item is to be used
/// outside the menu, with its member, target and item id in the out parameters;
/// -1 when the player backed out or pressed Start; 0 otherwise.
s32 ItemMenu_RunCommands(s32 *memberOut, s32 *targetOut, s32 *itemOut) {
  s32 done;
#if !TBS_PATCH_1
  s32 member;
  s32 chosen;
#endif
  s32 selection;
  s32 menuResult;
  struct Unit *sourceUnit;
  struct Unit *targetUnit;
  struct ItemMenuWork *menu;
  struct ItemDefinition *item;
  struct Unit *sourceCopy;
  struct Unit *targetCopy;
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
  member = 0;
  chosen = 0;
#endif
  selection = 0;
  menuResult = 0;
  state = ITEM_STEP_MEMBER;
  menu = gMenuWork;

  while (done == 0 && GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    switch (state) {
    // Choose whose items to look at.
    case ITEM_STEP_MEMBER:
      menu->selectedSlots[0] = 0;
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      menu->selectedItemIcon->motion = ICON_HIDDEN;
      ItemMenu_DrawMessage(0, MsgWhoseItem);
#if TBS_PATCH_1
      WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
      Text_DrawWorkValueWithLabel(menu->descriptionWindow);
      command = ItemMenu_SelectMember(0);
      if (command == -1) {
        selection = command = 0;
        menuResult = -1;
        done = 1;
      }
#else
      Text_DrawWorkValueWithLabel(menu->descriptionWindow);
      member = ItemMenu_SelectMember(0);
      if (member == -1) {
        chosen = selection = member = 0;
        menuResult = -1;
        done = 1;
      }
#endif
      WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
      ItemMenu_HideAllIcons();
      state = ITEM_STEP_ITEM;
      break;

    // Choose one of their items.
    case ITEM_STEP_ITEM:
      if (ItemMenu_CountItems(menu->sideMember[0]) == 0) {
        state = ITEM_STEP_MEMBER;
        break;
      }
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      menu->selectedItemIcon->motion = ICON_HIDDEN;
      menu->sideIcons[0]->motion = ICON_SHOWN;
      ItemMenu_DrawMessage(0, MsgWhichItem);
      selection = ItemMenu_RunList(0);
      state = ITEM_STEP_MEMBER;
      if (selection == -1) {
        break;
      }
      menu->listMode = 0xff;
      state = ITEM_STEP_COMMAND;
      break;

    // Choose what to do with it. An item that starts an effect on the field
    // (Scene_HasPsynergyTriggerFar) closes the menu and is returned to the
    // caller; other usable items are used here.
    case ITEM_STEP_COMMAND:
      command = ItemMenu_ChooseCommand();
      if (command == -1) {
        state = ITEM_STEP_ITEM;
        menu->completionFlag = 1;
      }
      if (command == ITEM_COMMAND_USE) {
        if (Scene_HasPsynergyTriggerFar(menu->selectedItems[0] &
                                        INVENTORY_ITEM_ID_MASK) != 0) {
          done = 1;
          // The international editions report target 0 (command is
          // ITEM_COMMAND_USE here) and the bare item id; the Japanese ones the
          // target chosen last and the list's whole inventory entry.
          // ItemMenu_Run reads only the member and the item id.
#if TBS_PATCH_1
          *memberOut = menu->sideMember[0];
          *targetOut = command;
          *itemOut = menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK;
#else
          *memberOut = member;
          *targetOut = chosen;
          *itemOut = selection;
#endif
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
          WindowSprite_ClearListFar(menu->descriptionWindow);
          ItemMenu_ShowMessage(menu->messageOffset + MsgItemUseResult, 0, -1);
#if !TBS_PATCH_1
          WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
#endif
          menu->sideIcons[0]->motion = ICON_HIDDEN;
          menu->itemCount = ItemMenu_CollectItems(
              Unit_GetFar(menu->sideMember[0]), menu->items, 0);
          ItemMenu_DrawIcons(menu->items, 0);
          state = ITEM_STEP_MEMBER;
        }
        if (result == ITEM_USE_MODE_UNUSABLE ||
            result == ITEM_USE_MODE_FROM_FIELD) {
          done = 1;
          *memberOut = menu->sideMember[0];
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
      ItemMenu_SetMessageWindowHeight5();
      ItemMenu_SetItemWindowRow5();
      WindowSprite_RedrawSavedRectFar(menu->questionWindow);
      ItemMenu_DrawItemHead();
      Text_DrawMessageAsTilesFar(MsgUseOnWhom, menu->questionWindow, 16, 16);
#if TBS_PATCH_1
      if (ItemMenu_SelectTarget(ITEM_TARGET_USE) != -1) {
#else
      chosen = ItemMenu_SelectTarget(ITEM_TARGET_USE);
      if (chosen != -1) {
#endif
        command = 0;
        if (ItemMenu_IsSpecial(menu->selectedItems[0] &
                               INVENTORY_ITEM_ID_MASK) != 0) {
          command = STATUS_PANEL_STATS;
        }
        result = ItemMenu_Use();
        StatusPanel_Draw(menu->statusWindow, menu->sideMember[1], 0, command);
        if (result != -1) {
          WindowSprite_ClearListFar(menu->descriptionWindow);
          ItemMenu_ShowMessage(menu->messageOffset + MsgItemUseResult, 0, -1);
#if !TBS_PATCH_1
          WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
#endif
          menu->sideIcons[0]->motion = ICON_HIDDEN;
          ItemMenu_TryBreak();
          state = ITEM_STEP_ITEM;
        }
        menu->itemCount = ItemMenu_CollectItems(
            Unit_GetFar(menu->sideMember[0]), menu->items, 0);
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
      WindowSprite_RedrawSavedRectFar(menu->questionWindow);
      ItemMenu_DrawItemHead();
      Text_DrawMessageAsTilesFar(MsgGiveToWhom, menu->questionWindow, 16, 16);
#if TBS_PATCH_1
      giveTarget = ItemMenu_SelectTarget(ITEM_TARGET_GIVE);
      state = ITEM_STEP_GIVE;
      if (giveTarget == -1) {
#else
      chosen = ItemMenu_SelectTarget(ITEM_TARGET_GIVE);
      state = ITEM_STEP_GIVE;
      if (chosen == -1) {
#endif
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
      Icon_PrepareObject(menu->selectedItemIcon);
      Window_ClearInteriorTilesFar((void *)menu->listWindow, 0, 72, 120, 96);
      WindowSprite_RedrawSavedRectFar(menu->questionWindow);
      // Yes: take result + 1 of the stack out, one at a time. A dropped rare
      // item goes back on the shops' counter (Item_AdjustCounterFar), so a
      // shop may sell it again.
      if (ItemMenu_ConfirmDrop(selection) == 0) {
#if TBS_PATCH_1
        command = menu->sideMember[0];
        Unit_GetFar(command);
        for (removedCount = 0; removedCount < result + 1; removedCount++) {
          Inventory_RemoveFar(command, menu->selectedSlots[0]);
          Item_AdjustCounterFar(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK,
                                1);
        }
        Unit_CalculateFar(command);
#else
        member = menu->sideMember[0];
        Unit_GetFar(member);
        for (removedCount = 0; removedCount < result + 1; removedCount++) {
          Inventory_RemoveFar(member, menu->selectedSlots[0]);
          Item_AdjustCounterFar(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK,
                                1);
        }
        Unit_CalculateFar(member);
#endif
        ItemMenu_SetItemWindowRow3();
        ItemMenu_RefreshMember(menu->sideMember[0], 0);
        menu->selectedItemIcon->motion = ICON_HIDDEN;
        menu->sideIcons[0]->motion = ICON_HIDDEN;
        WaitFrames(1);
        WindowSprite_ClearListFar(menu->descriptionWindow);
        ItemMenu_ShowMessage(MsgDroppedIt, ITEM_MODAL_SHORT_X, 13);
        menu->completionFlag = 1;
        state = ITEM_STEP_ITEM;
      } else {
        state = ITEM_STEP_COMMAND;
      }
      Unit_CalculateFar(menu->sideMember[0]);
      menu->selectedItemIcon->motion = ICON_HIDDEN;
      FieldScript_ClearInvalidPackedValuesFar();
      break;

    // Give the item, or some of a stack, to the chosen member. A full
    // inventory offers a trade instead; the new holder may equip it.
    case ITEM_STEP_GIVE:
      transferFailed = 0;
      item = Item_GetDirectFar(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
      if ((item->flags & ITEM_STACKABLE) != 0) {
        // A receiver with a full stack of it takes none; one with no free
        // slot and none of it must trade.
        heldCount = ItemMenu_GetItemQuantity(menu->sideMember[1],
                                             menu->selectedItems[0] &
                                                 INVENTORY_ITEM_ID_MASK);
        if (heldCount == ITEM_STACK_MAX) {
          transferFailed = 1;
        }
        if (ItemMenu_CountItems(menu->sideMember[1]) == INVENTORY_SLOTS &&
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
      Unit_CalculateFar(menu->sideMember[0]);
      Unit_CalculateFar(menu->sideMember[1]);
      Unit_RefreshPsynergyFar(menu->sideMember[0]);
      Unit_RefreshPsynergyFar(menu->sideMember[1]);
      result = 1;
      if (transferFailed == 0) {
        menu->sideMember[0] = menu->sideMember[1];
        menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
#if LANGUAGE != LANGUAGE_JAPANESE
        ItemMenu_SetMessageWindowHeight6();
#endif
        WindowSprite_RedrawSavedRectFar(menu->questionWindow);
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
      }
      if (GameFlag_GetFar(FLAG_MENU_START_PRESSED) != 0) {
        break;
      }
      ItemMenu_RefreshMember(menu->sideMember[1], 1);
      menu->sideIcons[0]->motion = ICON_HIDDEN;
      WaitFrames(1);
      // Say how it went: the receiver's stack was full, the item was given,
      // or it was given and equipped (ItemMenu_ConfirmEquip returned 0).
      if (transferFailed == 1) {
        WindowSprite_ClearListFar(menu->descriptionWindow);
        ItemMenu_ShowMessage(MsgCannotHoldMore, 15, 14);
      } else {
        WindowSprite_ClearListFar(menu->descriptionWindow);
        if (result == 1) {
          ItemMenu_ShowMessage(MsgGiven, 15, 14);
        } else {
          ItemMenu_DrawEquipPreview(menu->sideMember[1], menu->selectedSlots[1],
                                    0, menu->sideMember[1]);
          ItemMenu_ShowMessage(MsgEquippedIt, 15, 14);
          item = Item_GetDirectFar(menu->selectedItems[0]);
          if ((item->flags & ITEM_CURSED) != 0) {
            Sound_PlaySongFar(SE_CURSE);
            WindowSprite_ClearListFar(menu->descriptionWindow);
            ItemMenu_ShowMessage(MsgEquippedIt + 7, // "You were cursed!"
                                 ITEM_MODAL_SHORT_X, 14);
          }
        }
      }
      FieldScript_ClearInvalidPackedValuesFar();
      state = ITEM_STEP_MEMBER;
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
      sourceUnit = Unit_GetFar(menu->sideMember[0]);
      targetUnit = Unit_GetFar(menu->sideMember[1]);
      sourceCopy = Alloc(BATTLE_UNIT_SIZE);
      targetCopy = Alloc(BATTLE_UNIT_SIZE);
      Item_CopyWords(ITEM_IWRAM_COPY_WORDS, sourceCopy, sourceUnit,
                     BATTLE_UNIT_SIZE);
      Item_CopyWords(ITEM_IWRAM_COPY_WORDS, targetCopy, targetUnit,
                     BATTLE_UNIT_SIZE);
      // Take both whole stacks out, one item at a time (at most a stack of
      // 30): Inventory_RemoveFar returns INVENTORY_REMOVE_LAST for the last one
      // and INVENTORY_REMOVE_EMPTY_SLOT for an empty slot. Each count ends one
      // short and is made up after its loop.
      sourceQuantity = 0;
      while (sourceQuantity <= 29) {
        result =
            Inventory_RemoveFar(menu->sideMember[0], menu->selectedSlots[0]);
        if (result == INVENTORY_REMOVE_LAST) {
          break;
        }
        if (result == INVENTORY_REMOVE_EMPTY_SLOT) {
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
        targetQuantity++;
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
      WaitFrames(1);
      if (transferFailed == 1) {
        Item_CopyWords(ITEM_IWRAM_COPY_WORDS, sourceUnit, sourceCopy,
                       BATTLE_UNIT_SIZE);
        Item_CopyWords(ITEM_IWRAM_COPY_WORDS, targetUnit, targetCopy,
                       BATTLE_UNIT_SIZE);
        WindowSprite_ClearListFar(menu->descriptionWindow);
        ItemMenu_ShowMessage(MsgCannotTrade, ITEM_CANNOT_TRADE_X, 14);
#if TBS_PATCH_LEVEL == 3
        WindowSprite_RedrawSavedRectFar(menu->statusWindow);
#endif
      } else {
        Unit_CalculateFar(menu->sideMember[0]);
        Unit_CalculateFar(menu->sideMember[1]);
        Unit_RefreshPsynergyFar(menu->sideMember[0]);
        Unit_RefreshPsynergyFar(menu->sideMember[1]);
        ItemMenu_SetMessageWindowHeight5();
#if LANGUAGE != LANGUAGE_JAPANESE
        ItemMenu_SetMessageWindowHeight6();
        ItemMenu_HidePageIcons();
#endif
        WindowSprite_RedrawSavedRectFar(menu->questionWindow);
        menu->sideMember[0] = menu->sideMember[1];
        menu->selectedItems[0] &= INVENTORY_ITEM_ID_MASK;
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
        if (GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
          WindowSprite_ClearListFar(menu->descriptionWindow);
          ItemMenu_SetItemWindowRow5();
          ItemMenu_RefreshMember(menu->sideMember[1], 1);
          if (result == 0) {
            ItemMenu_DrawEquipPreview(menu->sideMember[1],
                                      menu->selectedSlots[1], 0,
                                      menu->sideMember[1]);
            ItemMenu_ShowMessage(MsgEquippedIt, 15, 14);
            item = Item_GetDirectFar(menu->selectedItems[0]);
            if ((item->flags & ITEM_CURSED) != 0) {
              Sound_PlaySongFar(SE_CURSE);
              WindowSprite_ClearListFar(menu->descriptionWindow);
              ItemMenu_ShowMessage(MsgEquippedIt + 7, // "You were cursed!"
                                   ITEM_MODAL_SHORT_X, 14);
            }
          } else {
            ItemMenu_ShowMessage(MsgTraded, ITEM_TRADED_X, 14);
          }
        }
      }
      Free(targetCopy);
      Free(sourceCopy);
      FieldScript_ClearInvalidPackedValuesFar();
      state = ITEM_STEP_MEMBER;
      break;

    // Equip it. Inventory_EquipFar returns -1 when the member cannot equip it
    // and -2 when the item it would replace is cursed on.
    case ITEM_STEP_EQUIP:
      result = Inventory_EquipFar(menu->sideMember[0], menu->selectedSlots[0]);
      if (result == -1) {
        state = ITEM_STEP_ITEM;
        break;
      }
      if (result == -2) {
        WindowSprite_ClearListFar(menu->descriptionWindow);
        ItemMenu_ShowMessage(MsgCannotRemoveItem, 0, -1);
#if !TBS_PATCH_1
        WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
#endif
        state = ITEM_STEP_ITEM;
        break;
      }
      Unit_CalculateFar(menu->sideMember[0]);
      Unit_RefreshPsynergyFar(menu->sideMember[0]);
      menu->sideIcons[0]->motion = ICON_HIDDEN;
      menu->itemCount = ItemMenu_CollectItems(Unit_GetFar(menu->sideMember[0]),
                                              menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      WaitFrames(1);
      ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0], 0,
                                menu->sideMember[0]);
      WindowSprite_ClearListFar(menu->descriptionWindow);
      ItemMenu_ShowMessage(MsgEquippedIt, 15, 8);
      item = Item_GetDirectFar(menu->selectedItems[0]);
      if ((item->flags & ITEM_CURSED) != 0) {
        Sound_PlaySongFar(SE_CURSE);
        WindowSprite_ClearListFar(menu->descriptionWindow);
        ItemMenu_ShowMessage(MsgEquippedIt + 7, // "You were cursed!"
                             ITEM_MODAL_SHORT_X, 8);
      }
      state = ITEM_STEP_ITEM;
      break;

    // Unequip it.
    case ITEM_STEP_REMOVE:
      Unit_GetFar(menu->sideMember[0])->inventory[menu->selectedSlots[0]] &=
          ~INVENTORY_EQUIPPED;
      Unit_CalculateFar(menu->sideMember[0]);
      Unit_RefreshPsynergyFar(menu->sideMember[0]);
      menu->sideIcons[0]->motion = ICON_HIDDEN;
      menu->itemCount = ItemMenu_CollectItems(Unit_GetFar(menu->sideMember[0]),
                                              menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      WaitFrames(1);
      // Show the stats as they are without it.
      menu->equipPreview = 1;
      ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0], 0,
                                menu->sideMember[0]);
      menu->equipPreview = 0;
      WindowSprite_ClearListFar(menu->descriptionWindow);
      ItemMenu_ShowMessage(MsgRemoved, ITEM_MODAL_REMOVED_X, 8);
      FieldScript_ClearInvalidPackedValuesFar();
      state = ITEM_STEP_ITEM;
      break;

    // Show its description.
    case ITEM_STEP_INSPECT:
      menu->sideIcons[0]->motion = ICON_HIDDEN;
      ItemMenu_ShowDetails(menu->selectedItems[0]);
      WindowSprite_RedrawSavedRectFar(menu->statusWindow);
      ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0], 0,
                                menu->sideMember[0]);
      menu->sideIcons[0]->motion = ICON_SHOWN;
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

  if (GameFlag_GetFar(FLAG_MENU_START_PRESSED) != 0) {
    menuResult = -1;
  }
  return menuResult;
}

/// Uses the selected item from the member in side 0 on the one in side 1.
/// On failure plays the error sound, shows why in the info window and returns
/// -1; otherwise plays the item's animation, recalculates both members and
/// returns 1.
s32 ItemMenu_Use(void) {
  struct ItemMenuWork *menu;
  s32 result;

  menu = gMenuWork;
  result = Item_Use(menu->selectedSlots[0], menu->sideMember[0],
                    menu->sideMember[1]);

  if (result == -1) {
    Sound_PlaySongFar(SE_MENU_ERROR);
    WindowSprite_ClearListFar(menu->descriptionWindow);
    ItemMenu_ShowMessage(menu->messageOffset + MsgItemUseResult, result,
                         result);
#if !TBS_PATCH_1
    WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
#endif
    menu->completionFlag = 1;
    return result;
  }

  Item_PlayUseAnimation(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
  Unit_CalculateFar(menu->sideMember[0]);
  Unit_CalculateFar(menu->sideMember[1]);
  return 1;
}

/// Sets up the item menu's first screen: the cursor window with the party's
/// figures, the info window along the bottom, and an 8 by 2 cursor grid. The
/// Psynergy and Djinn screens open with it too. The parameters are not used.
void ItemMenu_Initialize(s32 x, s32 y, s32 mode, s32 columns) {
  struct ItemMenuWork *menu = gMenuWork;
  s32 memberIndex;

  PsynergyMenu_InitializeEntryObjects(MenuCursor_Create(menu), 2, 2, 8, 0);
  // The four figures stand at screen y 30; the screens raise the member
  // chosen to 26.
  for (memberIndex = 3; memberIndex >= 0; memberIndex--)
    menu->memberY[memberIndex] = 30;

  {
    u8 zero = 0;
    s32 style;
    menu->buttonHelpWindow = NULL;
    menu->statusWindow = NULL;
    style = 2;
    menu->descriptionWindow = Window_CreateFar(0, 17, 30, 3, style);
    menu->itemWindow = NULL;
    menu->selectedColumn = zero;
    menu->selectedRow = zero;
    menu->columnCount = 8;
    menu->rowCount = style;
  }
}

/// Creates the 32 item icon sprites in window, off screen until placed: slots 0
/// to 7 at x 248, the rest at x 256, all at y 168, each showing item graphic
/// slot of the same number. The item, Psynergy and summary screens call it for
/// the window that holds their icons; the sprites are that window's render
/// outputs and go back to the pool when it closes.
void Menu_SpawnIconEntries(struct ItemMenuWork *menu, struct Window *window) {
  struct WindowSprite **firstIcons;
  s32 firstSlot;
  s32 firstY;
  struct WindowSprite **secondIcons;
  s32 secondSlot;
  s32 secondY;
  struct WindowSprite **remainingIcons;
  s32 remainingSlot;
  s32 remainingY;

  firstSlot = 0;
  firstY = 0xA8;
  firstIcons = &menu->entryIcons[0];
  do {
    *firstIcons++ = WindowSprite_CreateFromResourceFar(
        ICON_KIND_ITEM, firstSlot, window, 0xF8, firstY);
    firstSlot++;
  } while (firstSlot <= 7);

  secondSlot = 8;
  secondY = 0xA8;
  secondIcons = &menu->entryIcons[8];
  do {
    *secondIcons++ = WindowSprite_CreateFromResourceFar(
        ICON_KIND_ITEM, secondSlot, window, 0x100, secondY);
    secondSlot++;
  } while (secondSlot <= 15);

  remainingSlot = 16;
  remainingY = 0xA8;
  remainingIcons = &menu->entryIcons[16];
  do {
    *remainingIcons++ = WindowSprite_CreateFromResourceFar(
        ICON_KIND_ITEM, remainingSlot, window, 0x100, remainingY);
    remainingSlot++;
  } while (remainingSlot <= 31);
}

/// Hides all 32 item icons.
void ItemMenu_HideAllIcons(void) {
  s32 hiddenState = ICON_HIDDEN;
  struct WindowSprite **icons = ((struct ItemMenuWork *)gMenuWork)->entryIcons;
  s32 slot;

  for (slot = 31; slot >= 0; slot--) {
    struct WindowSprite *icon = *icons++;
    if (icon != 0) {
      icon->motion = hiddenState;
    }
  }
}

#if LANGUAGE != LANGUAGE_JAPANESE
/// Hides the item icons in the first column of the five-wide item grid.
void ItemMenu_HidePageIcons(void) {
  struct ItemMenuWork *menu = gMenuWork;
  s32 slot = 0;
  s32 hiddenState = ICON_HIDDEN;
  struct WindowSprite **iconSlot = menu->entryIcons;

  do {
    struct WindowSprite *icon = *iconSlot++;

    if (icon != 0 && slot % MENU_ITEM_GRID_COLUMNS == 0) {
      icon->motion = hiddenState;
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
  struct ItemMenuWork *menu;
  struct WindowSprite *cursor;

  menu = gMenuWork;
  Menu_ReleaseEntryObjects();
  ItemMenu_HideAllIcons();
  WaitFrames(1);
  cursor = menu->cursor;
  cursor->motion = ICON_HIDDEN;
  Window_CloseIfOpen(&menu->mainWindow, 1);
  Window_CloseIfOpen(&menu->itemWindow, 1);
  Window_CloseIfOpen(&menu->questionWindow, 1);
  Window_CloseIfOpen(&menu->statusWindow, 1);
  Window_CloseIfOpen(&menu->buttonHelpWindow, 1);
  Window_CloseIfOpen(&menu->descriptionWindow, 1);
  Window_CloseIfOpen(&menu->equipWindow, 1);
  Window_CloseIfOpen(&menu->listWindow, 1);
  Window_CloseIfOpen(&menu->unopenedWindow, 1);
  Window_CloseIfOpen(&menu->modalWindow, 1);
  Window_CloseIfOpen(&menu->secondUnopenedWindow, 1);
}

/// Lets the player choose a party member in side (0 or 1), starting from the
/// member chosen last. Returns the member's id, or -1 when the player backs
/// out.
s32 ItemMenu_SelectMember(s32 side) {
  struct ItemMenuWork *menu = gMenuWork;
  s32 result = 0;
  s32 memberIndex;
  struct Unit *unit;

  memberIndex = menu->sideIndex[side];
  WindowSprite_ClearListFar(menu->descriptionWindow);
  menu->sideCount[side] = menu->partyCount;
  if (memberIndex == -1) {
    menu->sideIndex[side] = result;
    memberIndex = 0;
  } else {
    MenuCursor_Slide(memberIndex * 24 - 10, 16);
  }
  unit = Unit_GetFar(menu->memberIds[memberIndex]);
  menu->itemCount = ItemMenu_CollectItems(unit, menu->items, 0);
  result = ItemMenu_RunMemberSelection(menu->memberIds, menu->items);
  Icon_PrepareObject(menu->sideIcons[side]);
  WaitFrames(1);
  return result;
}

/// The party member row of the item menu. Left and Right move between members,
/// showing each one's items and status; holding R shows their equipment
/// instead; L + A sorts the member's items, a new sort order each press; A
/// chooses a member who has items and B backs out. Returns the chosen member's
/// id, or -1.
s32 ItemMenu_RunMemberSelection(u16 *memberIds, u16 *items) {
  struct ItemMenuWork *menu;
  s32 selection;
  s32 memberCount;
  s32 result;
  struct Unit *unit;
  u8 sortMode;
  s32 showEquipment;
  s32 redraw;
  struct Window *window;
  s32 memberIndex;

  menu = gMenuWork;
  selection = menu->sideIndex[0];
  memberCount = menu->sideCount[0];
  redraw = 1;
  result = 0;
  sortMode = 0;
  showEquipment = 0;
  unit = Unit_GetFar(memberIds[selection]);
  if (Window_UpdateOrCreate(&menu->itemWindow, 13, 3, 17, 10, 2))
    Menu_SpawnIconEntries(menu, menu->itemWindow);
  if (Window_UpdateOrCreate(&menu->buttonHelpWindow, 13, 13, 17, 4, 2)) {
    menu->selectedItemIcon = WindowSprite_CreateFromResourceFar(
        ICON_KIND_ITEM, 0, menu->buttonHelpWindow, 0, result);
    menu->selectedItemIcon->motion = ICON_HIDDEN;
  }
  // The help text: L + A sorts, R shows the equipment.
#if LANGUAGE != LANGUAGE_JAPANESE
  Text_DrawMessageAsTilesFar(MsgArrangeItemsHelp, menu->buttonHelpWindow, 0, 0);
  Text_DrawMessageAsTilesFar(MsgArrangeItemsHelp + 1, menu->buttonHelpWindow, 0,
                             8);
#else
  Text_DrawStringInWindowFar(gItemMenu_ArrangeKeysString,
                             menu->buttonHelpWindow, 0, 0);
  Text_DrawMessageAsTilesFar(MsgArrangeItemsHelp, menu->buttonHelpWindow, 32,
                             0);
  Text_DrawStringInWindowFar(gItemMenu_EquipmentKeyString,
                             menu->buttonHelpWindow, 16, 8);
  Text_DrawMessageAsTilesFar(MsgArrangeItemsHelp + 1, menu->buttonHelpWindow,
                             32, 8);
#endif
  menu->sideIcons[0]->motion = redraw; // ICON_SHOWN
  while (!GameFlag_GetFar(FLAG_MENU_START_PRESSED)) {
    selection = (selection + memberCount) % memberCount;
    MenuCursor_Position(selection * 24 - 10, 16);
    if (redraw) {
      sortMode = 0;
      redraw = 0;
      window = menu->statusWindow;
      unit = Unit_GetFar(memberIds[selection]);
      if (showEquipment) {
        menu->itemCount = ItemMenu_CollectItems(
            Unit_GetFar(memberIds[selection]), menu->items, 0);
        ItemMenu_OpenCategory(memberIds[selection]);
        StatusPanel_Draw(window, memberIds[selection], 0, STATUS_PANEL_STATS);
      } else {
        ItemMenu_RefreshMember(memberIds[selection], 0);
        StatusPanel_Draw(window, memberIds[selection], 0,
                         STATUS_PANEL_EXPERIENCE);
      }
      // Raise the chosen member's figure.
      for (memberIndex = 3; memberIndex >= 0; memberIndex--)
        menu->memberY[memberIndex] = 30;
      menu->memberY[selection] = 26;
    }
    WaitFrames(1);
    // L + A sorts, cycling through the four sort orders.
    if (gKeysPressed & KEY_A) {
      if (gKeysHeld & KEY_L) {
        sortMode = (sortMode + LIST_SORT_COUNT) % LIST_SORT_COUNT;
        ItemMenu_Sort(unit->inventory, sortMode);
        sortMode++;
        ItemMenu_RefreshMember(memberIds[selection], 0);
        Sound_PlaySongFar(SE_MENU_CONFIRM);
      } else if (ItemMenu_CountItems(memberIds[selection])) {
        Sound_PlaySongFar(SE_MENU_CONFIRM);
        result = memberIds[selection];
        break;
      } else {
        Sound_PlaySongFar(SE_MENU_ERROR);
      }
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
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
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selection--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      selection++;
      redraw = 1;
    }
  }
  menu->sideIndex[0] = selection;
  menu->selectedMember = memberIds[selection];
  menu->sideMember[0] = memberIds[selection];
  return result;
}

/// Shows the equipment view of member memberId's items in the item window.
void ItemMenu_OpenCategory(s32 memberId) {
  struct ItemMenuWork *menu;

  menu = gMenuWork;
  ItemMenu_PositionCategory();
  WindowSprite_RedrawSavedRectFar(menu->itemWindow);
  ItemMenu_DrawCategory(menu->itemWindow, memberId, 0);
}

/// Chooses the party member to use or give the selected item (enum
/// ItemTargetMode) to. Using shows each member's status as the cursor passes;
/// giving shows each member's items, how many of this item they already hold,
/// or that they would have to trade, and the equipment preview. The party
/// members who can equip the item move while the cursor is up. Returns the
/// chosen member's id, or -1 when the player backs out; giving cannot choose
/// the member who holds the item.
s32 ItemMenu_SelectTarget(s32 mode) {

  struct ItemMenuWork *menu;
  struct Window *window;
  s32 memberCount;
  s32 memberIndex;
  s32 redraw;
  u8 result;
  s32 descriptionShown;
  s32 heldCount;
  struct WindowSprite *targetMarker;

  menu = gMenuWork;
  window = menu->itemWindow;
  memberIndex = menu->sideIndex[1];
  memberCount = menu->partyCount;
  redraw = 1;
  result = 0;
  descriptionShown = 0;
  Window_SetBounds(window, 13, 5, 17, 12);
  WindowSprite_RedrawSavedRectFar((void *)menu->itemWindow);
  Unit_GetFar(menu->memberIds[menu->sideIndex[0]]);
  Task_Add(EquipmentMenu_UpdateCompatibilityIndicators,
           TASK_PRIORITY_MAIN_LOOP);
  while (!GameFlag_GetFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      memberIndex = (memberIndex + memberCount) % memberCount;
      window = menu->itemWindow;
      Unit_GetFar(menu->memberIds[memberIndex]);
      // The marker over the chosen member, three tiles a member. Nothing in
      // TBS stores sideIcons[1], so it is still NULL from
      // Window_InitializeWork: these writes go to the BIOS area at address
      // 0, which ignores them, and no marker is drawn.
      targetMarker = menu->sideIcons[1];
      targetMarker->oam.x = targetMarker->x =
          ((menu->mainWindow->x + memberIndex * 3) << 3) - 2;
      // Giving: the member's items and how many of this one they hold.
      if (mode == ITEM_TARGET_GIVE) {
        ItemMenu_RefreshMember(menu->memberIds[memberIndex], 1);
        Window_DrawDividerLineFar(window, 0, 9, 16, 9);
        Window_ClearInteriorTilesFar((void *)window, 0, 72, 120, 80);
        if (memberIndex != menu->sideIndex[0]) {
          heldCount = ItemMenu_GetItemQuantity(menu->memberIds[memberIndex],
                                               menu->selectedItems[0] &
                                                   INVENTORY_ITEM_ID_MASK);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
          if (ItemMenu_CountItems(menu->memberIds[memberIndex]) ==
                  INVENTORY_SLOTS &&
              heldCount == 0)
            Text_DrawMessageAsTilesFar(MsgTradeForWhat, window, 0, 72);
          else if (heldCount != 0) {
            Text_DrawNumberInWindowFar(heldCount, 2, window, 8, 72);
            Text_DrawMessageAsTilesFar(MsgInStock, window, STOCK_LABEL_X, 72);
          } else {
            Text_DrawMessageAsTilesFar(MsgNoneInStock, window, 16, 72);
          }
#else
          if (heldCount != 0) {
            Text_DrawNumberInWindowFar(heldCount, 2, window, 8, 72);
            Text_DrawMessageAsTilesFar(MsgInStock, window, STOCK_LABEL_X, 72);
          } else {
            Text_DrawMessageAsTilesFar(MsgNoneInStock, window, 16, 72);
          }
          if (ItemMenu_CountItems(menu->memberIds[memberIndex]) ==
                  INVENTORY_SLOTS &&
              heldCount == 0)
            Text_DrawMessageAsTilesFar(MsgTradeForWhat, window,
                                       ITEM_TRADE_PROMPT_X, 72);
#endif
        }
        ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0],
                                  0, menu->memberIds[memberIndex]);
      }
      // Using: the member's status, and the item's name once.
      if (mode == ITEM_TARGET_USE) {
        if (ItemMenu_IsSpecial(menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK))
          StatusPanel_Draw(menu->statusWindow, menu->memberIds[memberIndex],
                           menu->selectedSlots[0], STATUS_PANEL_STATS);
        else
          StatusPanel_Draw(menu->statusWindow, menu->memberIds[memberIndex],
                           menu->selectedSlots[0], STATUS_PANEL_EXPERIENCE);
        if (!GameFlag_GetFar(FLAG_MENU_INFO_KEPT) && !descriptionShown) {
          MENU_RESET_DESCRIPTION_WINDOW((void *)menu->descriptionWindow);
          MENU_DRAW_DESCRIPTION(
              (menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
                  MsgItemPlainName,
              menu->descriptionWindow, 0, 0);
          descriptionShown = 1;
        } else {
          GameFlag_ClearFar(FLAG_MENU_INFO_KEPT);
        }
      }
    }
    MenuCursor_Position(memberIndex * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      if (mode == ITEM_TARGET_GIVE && memberIndex == menu->sideIndex[0]) {
        Sound_PlaySongFar(SE_MENU_ERROR);
        continue;
      }
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      result = menu->memberIds[memberIndex];
      break;
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      // The u8 255 is returned through (s8) as -1.
      result = 255;
      break;
    }
    if (gKeysRepeat & KEY_LEFT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      memberIndex--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
      memberIndex++;
    }
  }
  // The marker is NULL here too (see above).
  targetMarker = menu->sideIcons[1];
  menu->sideIndex[1] = memberIndex;
  Icon_PrepareObject(targetMarker);
  targetMarker->motion = ICON_HIDDEN;
  EquipmentMenu_StopCompatibilityIndicators();
  WaitFrames(1);
  menu->sideIndex[1] = memberIndex;
  menu->selectedMember = menu->memberIds[memberIndex];
  menu->sideMember[1] = menu->memberIds[memberIndex];
  return (s8)result;
}

/// Task while ItemMenu_SelectTarget chooses a member to use or
/// give the item to, and while the command grid's cursor is on Give: every 32
/// frames, sets each party figure's animation by whether that member can
/// equip the item (3) or not (1). EquipmentMenu_StopCompatibilityIndicators
/// removes it.
void EquipmentMenu_UpdateCompatibilityIndicators(void) {
  struct ItemMenuWork *menu;
  s8 memberIndex;

  menu = gMenuWork;
  if ((gFrameCount & 31) == 0 && menu->partyCount != 0) {
    memberIndex = 0;
    do {
      if (Item_CanUnitEquip(menu->memberIds[memberIndex],
                            menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) !=
          0) {
        struct Sprite *partyFigure = menu->memberObjects[memberIndex];
        Sprite_StartAnimFar(partyFigure, 3);
      } else {
        struct Sprite *partyFigure = menu->memberObjects[memberIndex];
        Sprite_StartAnimFar(partyFigure, 1);
      }
      memberIndex++;
    } while (memberIndex < menu->partyCount);
  }
}

/// Ends EquipmentMenu_UpdateCompatibilityIndicators: puts every party figure
/// back to animation 1 and removes the task. Removing a task that is not
/// running is harmless, so the command grid calls it at each cursor move.
void EquipmentMenu_StopCompatibilityIndicators(void) {
  struct ItemMenuWork *menu;
  s8 memberIndex;

  menu = gMenuWork;
  if (menu->partyCount != 0) {
    memberIndex = 0;
    do {
      struct Sprite *partyFigure = menu->memberObjects[memberIndex];
      Sprite_StartAnimFar(partyFigure, 1);
      memberIndex++;
    } while (memberIndex < menu->partyCount);
  }
  Task_Remove(&EquipmentMenu_UpdateCompatibilityIndicators);
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
  struct ItemMenuWork *menu;

  menu = gMenuWork;
  WindowSprite_RedrawSavedRectFar(menu->questionWindow);
  Text_DrawMessageAsTilesFar(message, menu->questionWindow, 0, 0);
}

/// Hides the icon of each of the 32 list entries whose item is 0. Each of the
/// 32 icon slots must hold a sprite (Menu_SpawnIconEntries).
void Menu_HideEmptyEntryIcons(const u16 *items) {
  struct ItemMenuWork *menu = gMenuWork;
  s32 slot;

  for (slot = 0; slot < 32; slot++) {
    if (items[slot] == 0) {
      Icon_PrepareObject(menu->entryIcons[slot]);
      menu->entryIcons[slot]->motion = ICON_HIDDEN;
    }
  }
}

/// Returns how many of member memberId's 15 inventory slots hold an item.
s32 ItemMenu_CountItems(s32 memberId) {
  s32 maskedItemId;
  s32 remainingSlots;
  s32 count;
  u16 *inventory;

  count = 0;
  inventory = Unit_GetFar(memberId)->inventory;
  remainingSlots = INVENTORY_SLOTS - 1;
  do {
    maskedItemId = INVENTORY_ITEM_ID_MASK & *inventory;
    inventory += 1;
    if (maskedItemId != 0) {
      count += 1;
    }
    remainingSlots -= 1;
  } while (remainingSlots >= 0);
  return count;
}

/// Returns how many of itemId the unit memberId holds: the quantity of
/// its first stack, or 0. The Spanish and Italian editions add up every stack
/// of it.
s32 ItemMenu_GetItemQuantity(s32 memberId, s32 itemId) {
  struct Unit *unit;
  s32 slotIndex;
  s32 quantity;

  quantity = 0;
  unit = Unit_GetFar(memberId);
  slotIndex = 0;
  do {
    if (unit->inventory[slotIndex] != 0 &&
        (unit->inventory[slotIndex] & INVENTORY_ITEM_ID_MASK) == itemId) {
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
s32 ItemMenu_CollectItems(struct Unit *unit, u16 *items, s32 mode) {
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
/// 0 draws the list's icons (ICON_KIND_ITEM); any other iconStyle the
/// equipment view's (ICON_KIND_EQUIPMENT_ITEM).
void ItemMenu_DrawIcons(u16 *inventoryEntries, s32 iconStyle) {
  s32 remaining;
  u16 *entries;
  struct WindowSprite **icons;
  s32 itemId;

  icons = ((struct ItemMenuWork *)gMenuWork)->entryIcons;
  entries = inventoryEntries;
  remaining = INVENTORY_SLOTS - 1;
  do {
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
    remaining--;
  } while (remaining >= 0);
  Menu_HideEmptyEntryIcons(inventoryEntries);
}

/// Shows member memberId's items in the item window, laid out for layout mode
/// (see ItemMenu_RefreshEntry), or "no items" when there are none.
void ItemMenu_RefreshMember(s32 memberId, s32 mode) {
  struct ItemMenuWork *menu;
  struct Unit *member;
  u16 *items;

  menu = gMenuWork;
  member = Unit_GetFar(memberId);
  items = menu->items;
  menu->itemCount = ItemMenu_CollectItems(member, items, 0);
  WindowSprite_RedrawSavedRectFar(menu->itemWindow);
  ItemMenu_RefreshEntry(mode);
  ItemMenu_DrawIcons(items, 0);
  if (ItemMenu_CountItems(memberId) == 0)
    Text_DrawMessageAsTilesFar(MsgItemMenuEmpty, menu->itemWindow, 8, 24);
}

UNUSED void ItemMenu_NoOp(void) {}

/// Shows in the status panel what the item in member's inventory slot would
/// mean to member target: for a usable item, target's status; for equipment,
/// target's stats with it equipped; for a Psynergy item, the Psynergy it
/// teaches. When target is someone else, the item is put into target's own
/// inventory for the preview, making room first if it is full, and target is
/// put back afterwards from a heap copy that is freed before returning. mode 1
/// redraws only the lower part of the panel.
void ItemMenu_DrawEquipPreview(s32 member, s32 slot, s32 mode, s32 target) {
  /* FAKEMATCH(temporary): the saved unit reuses member (and buffer for
   * Psynergy items), keeping member in r8 and panelStyle in r10; as a separate
   * void *saved, 22 of 172 instructions differ in tbs-en. */
  struct ItemMenuWork *menu = gMenuWork;
  s32 panelStyle = 0;
  struct Unit *unit = Unit_GetFar(member);
  s32 inventoryEntry = unit->inventory[slot];
  s32 previewSlot;

  if (mode == 1)
    panelStyle = STATUS_PANEL_LOWER_ONLY;
  switch (Item_GetDirectFar(inventoryEntry & INVENTORY_ITEM_ID_MASK)->type) {
  case ITEM_TYPE_NONE: // usable items
    StatusPanel_Draw(menu->statusWindow, target, slot, panelStyle);
    break;
  case ITEM_TYPE_WEAPON:
  case ITEM_TYPE_ARMOR:
  case ITEM_TYPE_SHIELD:
  case ITEM_TYPE_HELM:
  case ITEM_TYPE_BOOTS:
  case ITEM_TYPE_UNUSED:
  case ITEM_TYPE_RING:
  case ITEM_TYPE_SHIRT:
    if (member == target) {
      panelStyle |= STATUS_PANEL_EQUIP;
      StatusPanel_Draw(menu->statusWindow, target, slot, panelStyle);
    } else {
      s32 size;

      unit = Unit_GetFar(target);
      size = BATTLE_UNIT_SIZE;
      member = (s32)Alloc(size);
      Iwram_CopyWords((void *)member, unit, size);
      // With room made, add the item unequipped and preview it in its new
      // slot; otherwise the panel shows target's status without a preview.
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        inventoryEntry &= ~INVENTORY_EQUIPPED;
        previewSlot = Inventory_AddItemFar(target, inventoryEntry);
        if (previewSlot != -1) {
          panelStyle |= STATUS_PANEL_EQUIP;
          StatusPanel_Draw(menu->statusWindow, target, previewSlot, panelStyle);
        } else {
          StatusPanel_Draw(menu->statusWindow, target, slot, panelStyle);
        }
      } else {
        StatusPanel_Draw(menu->statusWindow, target, slot, panelStyle);
      }
      Iwram_CopyWords(unit, (void *)member, size);
      Free((void *)member);
    }
    break;
  case ITEM_TYPE_PSYNERGY:
    if (target == member) {
      panelStyle |= STATUS_PANEL_LEARN;
      StatusPanel_Draw(menu->statusWindow, target, slot, panelStyle);
    } else {
      s32 size;
      void *buffer;

      unit = Unit_GetFar(target);
      size = BATTLE_UNIT_SIZE;
      member = (s32)Alloc(size);
      buffer = (void *)member;
      Iwram_CopyWords(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        previewSlot = Inventory_AddItemFar(target, inventoryEntry);
        if (previewSlot != -1) {
          panelStyle |= STATUS_PANEL_LEARN;
          StatusPanel_Draw(menu->statusWindow, target, previewSlot, panelStyle);
        } else {
          StatusPanel_Draw(menu->statusWindow, target, slot, panelStyle);
        }
      } else {
        StatusPanel_Draw(menu->statusWindow, target, slot, panelStyle);
      }
      Iwram_CopyWords(unit, (void *)member, size);
      buffer = (void *)member;
      Free(buffer);
    }
    break;
  }
}

/// Makes room in memberId's inventory: if no slot is free, removes the whole
/// stack in the first slot that is not equipped. Returns 1 when a slot is free
/// (or was freed), 0 otherwise.
s32 Inventory_RemoveFirstUnflagged(s32 memberId) {
  struct Unit *unit = Unit_GetFar(memberId);
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
/// cursor: the member's status on Use, the stats with the item equipped on
/// Equip and Details, and without it on the bottom row, where Give also shows
/// who can equip it. The cursor starts on the command chosen last, or on the
/// first available one. Returns the command chosen, or -1 when the player backs
/// out.
s32 ItemMenu_ChooseCommand(void) {
  struct ItemMenuWork *menu;
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
    struct Window *messageWindow;

    ItemMenu_HideAllIcons();
    WindowSprite_RedrawSavedRectFar((void *)menu->listWindow);
    messageWindow = menu->questionWindow;
    ItemMenu_SetMessageWindowHeight7();
    WindowSprite_RedrawSavedRectFar((void *)messageWindow);
    Window_DrawDividerLineFar(messageWindow, 0, 3, 16, 3);
    ItemMenu_DrawItemHead();
    ItemMenu_DrawCommands(commandStates, messageWindow);
    MENU_RESET_DESCRIPTION_WINDOW((void *)menu->descriptionWindow);
    MENU_DRAW_DESCRIPTION((menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
                              MsgItemPlainName,
                          menu->descriptionWindow, 0, 0);
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
  MenuCursor_Slide(cursorX, cursorY);
#else
  MenuCursor_Slide(column * 40 + 86, row * 8 + 30);
#endif

  for (;;) {
    if (GameFlag_GetFar(FLAG_MENU_START_PRESSED) != 0)
      break;
    if (redraw != 0) {
      redraw = 0;
      column = (column + 3) % 3;
      row = (row + 2) % 2;
      commandIndex = row * 3 + column;
      EquipmentMenu_StopCompatibilityIndicators();
      // Preview the item in the status panel for the command under the cursor:
      // the bottom row (Give, Remove, Drop) without it, Equip and Details with
      // it, Use the member's status.
      if (commandIndex > 2) {
        menu->equipPreview = 1;
        ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0],
                                  0, menu->sideMember[0]);
        if (commandIndex == ITEM_COMMAND_GIVE) {
          Task_Add(&EquipmentMenu_UpdateCompatibilityIndicators,
                   TASK_PRIORITY_MAIN_LOOP);
        }
      } else if (commandIndex != 0) {
        menu->equipPreview = 0;
        ItemMenu_DrawEquipPreview(menu->sideMember[0], menu->selectedSlots[0],
                                  0, menu->sideMember[0]);
      } else {
        StatusPanel_Draw(menu->statusWindow, menu->sideMember[0], 0,
                         STATUS_PANEL_EXPERIENCE);
      }
    }
#if LANGUAGE != LANGUAGE_JAPANESE
    cursorX = ItemMenu_CommandCursorX(column, row);
    cursorY = ItemMenu_CommandCursorY(column, row);
    MenuCursor_Position(cursorX, cursorY);
#else
    MenuCursor_Position(column * 40 + 86, row * 8 + 30);
#endif
    WaitFrames(1);

    if ((gKeysPressed & KEY_A) != 0) {
      if (commandStates[commandIndex] == ITEM_COMMAND_STATE_DISABLED) {
        Sound_PlaySongFar(SE_MENU_ERROR);
      } else {
        switch (commandIndex) {
        case ITEM_COMMAND_USE:
          Sound_PlaySongFar(SE_NOTE_MID); // Use
          break;
        case ITEM_COMMAND_EQUIP:
          Sound_PlaySongFar(SE_NOTE_HIGH); // Equip
          break;
        case ITEM_COMMAND_INSPECT:
        case ITEM_COMMAND_GIVE:
        case ITEM_COMMAND_DROP:
          Sound_PlaySongFar(SE_MENU_CONFIRM);
          break;
        case ITEM_COMMAND_REMOVE:
          Sound_PlaySongFar(SE_ITEM_REMOVE); // Remove
          break;
        default:
          Sound_PlaySongFar(SE_MENU_CONFIRM);
          break;
        }
        menu->listMode = commandIndex;
        break;
      }
    }
    if ((gKeysPressed & KEY_B) != 0) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      commandIndex = -1;
      menu->listMode = commandIndex;
      break;
    }

    if ((gKeysRepeat & KEY_UP) != 0) {
      row -= 1;
      redraw = 1;
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
    } else if ((gKeysRepeat & KEY_DOWN) != 0) {
      row += 1;
      redraw = 1;
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
    } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
      column += 1;
      redraw = 1;
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
    } else if ((gKeysRepeat & KEY_LEFT) != 0) {
      column -= 1;
      redraw = 1;
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
    }
  }

  menu->equipPreview = 0;
  EquipmentMenu_StopCompatibilityIndicators();
  return commandIndex;
}

/// Fills commandStates (indexed by ItemMenuCommand) with
/// ITEM_COMMAND_STATE_AVAILABLE or _DISABLED for the selected item: Use for
/// items Item_ClassifyUseMode accepts and are not broken, and for field items,
/// Equip for equipment the member can wear and is not wearing, Remove for worn
/// equipment that is not cursed, Give with someone to give to, Drop unless the
/// item is indispensable, and neither for cursed equipment being worn. Details
/// is always available. Each later test overrides the earlier ones.
void ItemMenu_BuildCommands(s8 *commandStates) {
  struct ItemMenuWork *menu;
  struct ItemDefinition *item;

  menu = gMenuWork;
  item = Item_GetDirectFar(INVENTORY_ITEM_ID_MASK & menu->selectedItems[0]);

  if (item->type == 0) { // usable items
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;
    commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_DISABLED;
  } else {
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_DISABLED;
    commandStates[ITEM_COMMAND_EQUIP] = ITEM_COMMAND_STATE_AVAILABLE;
  }

  if (Item_ClassifyUseMode(menu->sideMember[0], menu->selectedItems[0]) !=
      ITEM_USE_MODE_UNUSABLE)
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

  if (Scene_HasPsynergyTriggerFar(menu->selectedItems[0] &
                                  INVENTORY_ITEM_ID_MASK) != 0)
    commandStates[ITEM_COMMAND_USE] = ITEM_COMMAND_STATE_AVAILABLE;

  if (menu->partyCount <= 1)
    commandStates[ITEM_COMMAND_GIVE] = ITEM_COMMAND_STATE_DISABLED;

  if (item->flags & ITEM_INDISPENSABLE)
    commandStates[ITEM_COMMAND_DROP] = ITEM_COMMAND_STATE_DISABLED;
}

#define TEXT_COLOR_DISABLED                                                    \
  TEXT_COLOR_GREY ///< the grey of a command that refuses A

/// Writes the six command names into window, the disabled ones in grey.
void ItemMenu_DrawCommands(const s8 *commandStates, struct Window *window) {
  s32 disabled;
  s32 useState;
  u32 firstCommandMessage;

  Text_SetColorFar(TEXT_COLOR_NORMAL);
  useState = commandStates[ITEM_COMMAND_USE];
  disabled = ITEM_COMMAND_STATE_DISABLED;
  if (useState == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);

  firstCommandMessage = MsgItemCommandUse;
  Text_DrawMessageAsTilesFar(firstCommandMessage, window, 0, 24); // "Use"
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_EQUIP] == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  Text_DrawMessageAsTilesFar(firstCommandMessage + 1, window,
                             ITEM_COMMAND_TEXT_X,
                             24); // "Equip"
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_GIVE] == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  Text_DrawMessageAsTilesFar(firstCommandMessage + 2, window, 0,
                             32); // "Give"
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_DROP] == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  Text_DrawMessageAsTilesFar(firstCommandMessage + 3, window, 80,
                             32); // "Drop"
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_INSPECT] == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  Text_DrawMessageAsTilesFar(firstCommandMessage + 4, window, 80,
                             24); // "Details"
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_REMOVE] == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  // "Remove"
  Text_DrawMessageAsTilesFar(firstCommandMessage + 5, window,
                             ITEM_COMMAND_TEXT_X, 32);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
}

/// Says how memberId can use inventoryEntry from the item menu (enum
/// ItemUseMode). Usable items always qualify; equipment only when the member
/// can wear it.
s32 Item_ClassifyUseMode(s32 memberId, s32 inventoryEntry) {
  s32 itemId = inventoryEntry;
  struct ItemDefinition *item;
  s32 result = ITEM_USE_MODE_UNUSABLE;

  itemId &= INVENTORY_ITEM_ID_MASK;
  item = Item_GetDirectFar(itemId);

  if (Scene_HasPsynergyTriggerFar(itemId) != 0) {
    return ITEM_USE_MODE_FROM_FIELD;
  }

  {
    struct AbilityDefinition *action =
        Ability_Get(item->abilityId & PSYNERGY_ABILITY_MASK);

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

/// After the selected item is used: an item that may break (ITEM_USE_MAY_BREAK)
/// breaks one time in eight, with the break sound and "It broke!" in the info
/// window. Called only after a successful use on a chosen member.
void ItemMenu_TryBreak(void) {
  struct ItemMenuWork *menu;

  menu = gMenuWork;
  // One time in eight: 0x2000 of Random16's 0x10000 values.
  if ((Item_GetDirectFar(INVENTORY_ITEM_ID_MASK & menu->selectedItems[0])
           ->useType == ITEM_USE_MAY_BREAK) &&
      (Random16() < 0x2000U)) {
    Inventory_BreakFar(menu->sideMember[0], menu->selectedSlots[0]);
    Sound_PlaySongFar(SE_ITEM_BREAK);
    ItemMenu_ShowMessage(MsgItemBroke, 0, -1);
#if !TBS_PATCH_1
    WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
#endif
  }
}

/// Opens the details window (top left, 13 by 10 tiles) and describes the item
/// in side itemIndex there. Returns 1. The window is kept in the work's
/// equipWindow, which ItemMenu_Close closes. Nothing calls it: the Details
/// command uses ItemMenu_ShowDetails.
s32 ItemMenu_OpenDetail(s32 itemIndex) {
  struct ItemMenuWork *menu = gMenuWork;

  Window_UpdateOrCreate(&menu->equipWindow, 0, 0, 13, 10, 2);
  Palette_LoadIconsIntoHighlight();

  if (menu->selectedItems[itemIndex] != 0)
    ItemMenu_DrawItemDetails(menu->equipWindow, menu->selectedItems[itemIndex]);

  return 1;
}

/// The item menu's Details screen: describes the item entry value in a large
/// window (the whole top half after patch 1) and waits for A or B. Up and Down
/// move an unused counter from 0 to 4. Returns the counter after A or Start,
/// or -1 after B; ITEM_STEP_INSPECT ignores it. Borrows HIGHLIGHT_PALETTE
/// for the element icons and makes the highlight again before returning.
s32 ItemMenu_ShowDetails(s32 value) {
#if TBS_PATCH_1
  s32 changed = 1;
  struct ItemMenuWork *menu = gMenuWork;
  struct WindowSprite *itemIcon = menu->selectedItemIcon;
#else
  struct ItemMenuWork *menu = gMenuWork;
  s32 changed = 1;
#endif
  struct Window *window;
  s32 counter = 0;

#if TBS_PATCH_1
  itemIcon->motion = ICON_HIDDEN;
#endif
  window = Window_CreateFar(0, 0, ITEM_QUANTITY_WINDOW_WIDTH, 10,
                            WINDOW_FLAG_INSTANT);
  // Hide the party figures (without their placement callback they are not
  // drawn) and the status panel's portrait (the work's cursor), which the
  // window covers, meanwhile.
  Task_Remove(Menu_UpdateEntryObjectTransforms);

  {
    struct WindowSprite *cursor = menu->cursor;
    cursor->motion = ICON_HIDDEN;
  }
  Palette_LoadIconsIntoHighlight();
  WaitFrames(1);

  for (;;) {
    if (GameFlag_GetFar(FLAG_MENU_START_PRESSED) != 0)
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
  WindowSprite_RedrawSavedRectFar(window);
  WaitFrames(1);
  Window_CloseFar((void *)window, 1);
  WindowSprite_RedrawSavedRectFar(menu->mainWindow);
  Palette_LightenBankHighlight(HIGHLIGHT_PALETTE);
  // As PsynergyMenu_InitializeEntryObjects schedules it.
  {
    s32 priority = TASK_PRIORITY_MAIN_LOOP;

    Task_Add(Menu_UpdateEntryObjectTransforms, priority);
  }

  // The Japanese window, 13 tiles wide, leaves the command grid at tile 13
  // up, so flags 1 tells ItemMenu_ChooseCommand not to draw it again; the
  // international one, 30 wide, covers it, and the grid's frame is redrawn.
  {
    struct WindowSprite *cursor = menu->cursor;
#if TBS_PATCH_1
    cursor->motion = ICON_SHOWN;
#else
    u8 one = 1;

    cursor->motion = one;
    menu->flags = one;
#endif
  }
#if LANGUAGE != LANGUAGE_JAPANESE
  Window_DrawFrameFar(13, 0, 17, 10);
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
void ItemMenu_DrawItemDetails(struct Window *window, s32 inventoryEntry) {
  struct ItemDefinition *def;
  s32 i;
  s32 hasEffects;
  s32 amount;
  s32 bestowsPsynergy;
  s8 row;

  row = 0;
  bestowsPsynergy = 0;
  def = Item_GetDirectFar(inventoryEntry & INVENTORY_ITEM_ID_MASK);
  // Equipment: a heading, then its Attack and Defense bonuses.
  if (def->type != ITEM_TYPE_NONE) {
    hasEffects = 0;
    if (def->primaryBonus == 0 && def->secondaryBonus == 0) {
      for (i = 0; i < 4; i++) {
        if (def->effects[i].kind != 0 || def->useType == ITEM_USE_TEACHES) {
          hasEffects = 1;
          break;
        }
      }
    } else {
      hasEffects = 1;
    }
    if (hasEffects == 1) {
      Text_DrawMessageAsTilesFar(MsgEquipEffectHeading, window, 16, 0);
      row = 1;
    }
    if (def->primaryBonus != 0) {
      Text_DrawMessageAsTilesFar(MsgStatLabel, window, 0, row * 8);
      amount = def->primaryBonus;
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      row++;
    }
    if (def->secondaryBonus != 0) {
      Text_DrawMessageAsTilesFar(MsgDefenseLabel, window, 0, row * 8);
      amount = def->secondaryBonus;
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      row++;
    }
  }
  // Each of its effects.
  for (i = 0; i < 4; i++) {
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
    case EQUIP_EFFECT_ADD_TURNS:
      Text_DrawMessageAsTilesFar(def->effects[i].kind + MsgEquipEffectName,
                                 window, 0, row * 8);
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      break;
    // Power and Resist in each of the four elements, after the element's
    // icon.
    case EQUIP_EFFECT_ADD_VENUS_POWER ... EQUIP_EFFECT_ADD_JUPITER_RESIST:
      Window_SetTilemapEntryFar(
          window,
          (u8)((def->effects[i].kind - EQUIP_EFFECT_ADD_VENUS_POWER) %
               ELEMENT_COUNT) +
              1,
          0, row, 2);
      Text_DrawMessageAsTilesFar(def->effects[i].kind + MsgEquipEffectName,
                                 window, 8, row * 8);
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      break;
    // Rates, in tenths: shown as a multiplier from 0.0 to 1.9.
    case EQUIP_EFFECT_SCALE_HP:
    case EQUIP_EFFECT_SCALE_HP_REGEN:
    case EQUIP_EFFECT_SCALE_PP:
    case EQUIP_EFFECT_SCALE_PP_REGEN:
    case EQUIP_EFFECT_SCALE_ATTACK:
    case EQUIP_EFFECT_SCALE_DEFENSE:
    case EQUIP_EFFECT_SCALE_AGILITY:
    case EQUIP_EFFECT_SCALE_LUCK:
      Text_DrawMessageAsTilesFar(def->effects[i].kind + MsgEquipEffectName,
                                 window, 0, row * 8);
#if LANGUAGE != LANGUAGE_JAPANESE
      Text_DrawStringInWindowFar(gMenu_TimesString, window, 64, row * 8);
      if (amount > 9) {
        Text_DrawNumberInWindowFar(1, 1, window, 72, row * 8);
        Text_DrawStringInWindowFar(gMenu_PointString, window, 80, row * 8);
        Text_DrawNumberInWindowFar(amount - 10, 1, window, 88, row * 8);
      } else {
        Text_DrawNumberInWindowFar(0, 1, window, 72, row * 8);
        Text_DrawStringInWindowFar(gMenu_PointString, window, 80, row * 8);
        Text_DrawNumberInWindowFar(amount, 1, window, 88, row * 8);
      }
#else
      if (amount > 9) {
        Text_DrawNumberInWindowFar(1, 1, window, 48, row * 8);
        Text_DrawStringInWindowFar(gMenu_PointString, window, 56, row * 8);
        Text_DrawNumberInWindowFar(amount - 10, 1, window, 64, row * 8);
      } else {
        Text_DrawNumberInWindowFar(0, 1, window, 48, row * 8);
        Text_DrawStringInWindowFar(gMenu_PointString, window, 56, row * 8);
        Text_DrawNumberInWindowFar(amount, 1, window, 64, row * 8);
      }
      Text_DrawMessageAsTilesFar(MsgEffectRateSuffix, window, 72, row * 8);
#endif
      break;
    case EQUIP_EFFECT_ADD_UNLEASH_CHANCE: // "Critical Hits increase"
    case EQUIP_EFFECT_CURSE_REMOVABLE:
    case EQUIP_EFFECT_LURE: // "Boosts monster encounters"
      Text_DrawMessageAsTilesFar(def->effects[i].kind + MsgEquipEffectName,
                                 window, 0, row * 8);
      break;
    }
    row++;
  }
  // Curses, bestowed Psynergy and how the item is used up.
  if (def->flags & ITEM_CURSED) {
    Text_DrawMessageAsTilesFar(MsgItemCursed, window, 0, row * 8);
    row++;
  }
  if (def->useType == ITEM_USE_TEACHES) {
    Text_DrawMessageAsTilesFar(MsgBestowsPsynergy, window, 0, row * 8);
    bestowsPsynergy = 1;
    row++;
  }
  // "Number of Uses" and what using it costs, except for items that cost
  // nothing and Hermes' Water (ITEM_USE_EMPTIES); a Psynergy item has no
  // heading and no line.
  if (def->useType != ITEM_USE_EMPTIES && def->useType != 0) {
    if (!bestowsPsynergy) {
      Text_DrawMessageAsTilesFar(MsgUsesHeading, window, 16, row * 8);
      row++;
    }
    switch (def->useType) {
    case 0:
      break;
    case ITEM_USE_CONSUMED:
      Text_DrawMessageAsTilesFar(MsgSingleUse, window, 0, row * 8);
      row++;
      break;
    case ITEM_USE_MAY_BREAK: {
      s32 m;

      // Two lines each: "It's broken, so" / "it can't be used", or "It might
      // break" / "if used in battle".
      if (inventoryEntry & INVENTORY_BROKEN) {
        m = MsgBrokenNotice;
        Text_DrawMessageAsTilesFar(m, window, 0, row * 8);
        row++;
        m++;
        Text_DrawMessageAsTilesFar(m, window, 0, row * 8);
        row++;
      } else {
        m = MsgMightBreak;
        Text_DrawMessageAsTilesFar(m, window, 0, row * 8);
        row++;
        m++;
        Text_DrawMessageAsTilesFar(m, window, 0, row * 8);
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
    Text_DrawMessageAsTilesFar(i, window, 16, row * 8);
    row++;
    amount =
        (inventoryEntry & INVENTORY_QUANTITY_MASK) / INVENTORY_QUANTITY_STEP;
    // The quantity field holds the count less one. The international
    // editions pass it as the argument of "I have <number>."; Japanese draws
    // the number before its label.
#if LANGUAGE != LANGUAGE_JAPANESE
    MessageArgument_PushFar(amount + 1, MESSAGE_ARGUMENT_NUMBER);
    Text_DrawMessageAsTilesFar(i + 1, window, 0, row * 8);
#else
    Text_DrawNumberInWindowFar(amount + 1, 2, window, 0, row * 8);
    Text_DrawMessageAsTilesFar(i + 1, window, 16, row * 8);
#endif
    row++;
  }
  // Nothing written yet: rare, important, or nothing known.
  hasEffects = 0;
  if (row == 0) {
    if (def->flags & ITEM_RARE) {
      Text_DrawMessageAsTilesFar(MsgRareItem, window, 0, 0);
      hasEffects = 1;
    }
    if (!hasEffects) {
      if (def->flags & ITEM_INDISPENSABLE) {
        Text_DrawMessageAsTilesFar(MsgImportantItem, window, 0, 0);
        hasEffects = 1;
      }
      if (!hasEffects)
        Text_DrawMessageAsTilesFar(MsgDetailsUnknown, window, 0, 0);
    }
  }
}

/// Writes a signed stat change at (x, y) of window: the number, with a plus or
/// minus sign just before its first digit.
void ItemMenu_DrawStat(s32 statChange, s32 unused, struct Window *window, s32 x,
                       s32 y) {
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

/// Moves the item window to row 5 of the right-hand side.
void ItemMenu_SetItemWindowRow5(void) {
  Window_SetBounds(
      (struct Window *)((struct ItemMenuWork *)gMenuWork)->itemWindow, 13, 5,
      17, 10);
}

/// Moves the item window to row 3 of the right-hand side.
void ItemMenu_SetItemWindowRow3(void) {
  Window_SetBounds(
      (struct Window *)((struct ItemMenuWork *)gMenuWork)->itemWindow, 13, 3,
      17, 10);
}

/// Makes the message window, along the top right, 5 rows tall.
void ItemMenu_SetMessageWindowHeight5(void) {
  Window_SetBounds(
      (struct Window *)((struct ItemMenuWork *)gMenuWork)->questionWindow, 13,
      0, 17, 5);
}

void ItemMenu_SetMessageWindowHeight6(void) {
  Window_SetBounds(
      (struct Window *)((struct ItemMenuWork *)gMenuWork)->questionWindow, 13,
      0, 17, 6);
}

void ItemMenu_SetMessageWindowHeight7(void) {
  Window_SetBounds(
      (struct Window *)((struct ItemMenuWork *)gMenuWork)->questionWindow, 13,
      0, 17, 7);
}

void ItemMenu_SetMessageWindowHeight3(void) {
  Window_SetBounds(
      (struct Window *)((struct ItemMenuWork *)gMenuWork)->questionWindow, 13,
      0, 17, 3);
}

/// Asks how many of the selected item stack to give or drop: Left and Right
/// choose a number from 1 to quantityRange (shown as baseQuantity + 1 up to
/// baseQuantity + quantityRange on a dial of digit sprites), with the member's
/// remaining count and, when giving (dropMode 0), the receiver's new count
/// beside their names. Returns the number less one, or -1 when the player backs
/// out.
s32 ItemMenu_SelectGiveQuantity(s32 baseQuantity, s32 quantityRange,
                                s32 dropMode) {
  s32 quantity = baseQuantity;
  s32 block;
  s32 changed;
  s32 receiverCount;
  s32 memberCount;
  struct ItemMenuWork *menu = gMenuWork;
  // 1 KB of HEAP_SLOT_TILE_SCRATCH to draw the dial's digits in, released
  // before returning.
  void *tiles = AllocHeapSlotEwram(HEAP_SLOT_TILE_SCRATCH, 0x400);
  struct Window *window;
  struct WindowSprite *rightDial;

  changed = 1;
  receiverCount = 0;
  window = menu->questionWindow;
  ItemMenu_SetMessageWindowHeight7();
  WindowSprite_RedrawSavedRectFar(window);
  // How many each member holds now.
  if (dropMode == 0)
    receiverCount = ItemMenu_GetItemQuantity(
        menu->sideMember[1], menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
  memberCount = ItemMenu_GetItemQuantity(
      menu->sideMember[0], menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK);
  // Two dial sprites, left and right of the number.
  block = VramCache_FindFreeEntry();
  if (block != 96) { // 96 (VRAM_CACHE_ENTRY_COUNT): no free VRAM block
    VramCache_Load(block, 256, 0);
    WindowSprite_CreateFar(block, OUTPUT_SIZE_32X8, window, 48, 32);
    rightDial = WindowSprite_CreateFar(block, OUTPUT_SIZE_32X8, window, 80, 32);
    rightDial->oam.index += 4;
    MenuCursor_Slide(ITEM_GIVE_CURSOR_X, 40);
    while (!GameFlag_GetFar(FLAG_MENU_START_PRESSED)) {
      if (changed) {
        changed = 0;
        quantity = (quantityRange + quantity) % quantityRange;
        WindowSprite_RedrawSavedRectFar(window);
        Text_DrawMessageAsTilesFar(MsgGiveHowMany, window, ITEM_GIVE_ASK_X,
                                   ITEM_GIVE_ASK_Y);
        Dma_Set(gMenu_QuantityTiles, tiles,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x40, REG_DMA3);
        QuantityBar_FillFar(30, 14, tiles);
        QuantityBar_FillFar(quantityRange + baseQuantity, 0, tiles);
        QuantityBar_FillFar(baseQuantity + quantity + 1, 10, tiles);
        QuantityBar_FillFar(baseQuantity, 2, tiles);
        VramCache_Load(block, 256, tiles);
        Text_DrawNumberInWindowFar(quantity + 1, 2, window, 32, 32);
        Text_DrawMessageAsTilesFar(
            (menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) + MsgItemName,
            window, 16, 8);
        Text_DrawNumberInWindowFar(memberCount - quantity - 1, 2, window, 16,
                                   24);
#if LANGUAGE != LANGUAGE_JAPANESE
        if (dropMode == 0)
          Text_DrawNumberInWindowFar(receiverCount + quantity + 1, 2, window,
                                     80, 24);
#else
        Text_DrawMessageAsTilesFar(MsgItemCounter, window, 32, 24);
        if (dropMode == 0) {
          Text_DrawNumberInWindowFar(receiverCount + quantity + 1, 2, window,
                                     80, 24);
          Text_DrawMessageAsTilesFar(MsgItemCounter, window, 96, 24);
        }
#endif
        Text_DrawStringAtOffsetFar(Unit_GetFar(menu->sideMember[0])->name,
                                   window, 16, 16);
        if (dropMode == 0)
          Text_DrawStringAtOffsetFar(Unit_GetFar(menu->sideMember[1])->name,
                                     window, 80, 16);
      }
      // A confirms, B backs out, Left and Right change the number.
      if (gKeysPressed & KEY_A) {
        Sound_PlaySongFar(SE_MENU_CONFIRM);
        break;
      }
      if (gKeysPressed & KEY_B) {
        quantity = -1;
        Sound_PlaySongFar(SE_MENU_CANCEL);
        break;
      }
      MenuCursor_Position(ITEM_GIVE_CURSOR_X, 40);
      if (gKeysRepeat & KEY_LEFT) {
        quantity--;
        changed = 1;
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      }
      if (gKeysRepeat & KEY_RIGHT) {
        quantity++;
        changed = 1;
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      }
      WaitFrames(1);
    }
  }
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar((void *)window);
  FreeHeapSlot(HEAP_SLOT_TILE_SCRATCH);
  menu->selectedItemIcon->motion = ICON_HIDDEN;
  if (GameFlag_GetFar(FLAG_MENU_START_PRESSED))
    quantity = -1;
  return quantity;
}

/// Shows the selected item at the top of the message window: its icon, its
/// member's name and its name.
void ItemMenu_DrawItemHead(void) {
  struct ItemMenuWork *menu = gMenuWork;

  Resource_LoadByModeIntoSlotFar(ICON_KIND_ITEM, menu->selectedItems[0],
                                 (u8)menu->selectedItemIcon->index, 0);
  menu->selectedItemIcon->motion = ICON_SHOWN;
  menu->selectedItemIcon->x = 112;
  menu->selectedItemIcon->y = 8;
  Icon_PrepareObject(menu->selectedItemIcon);
  Text_DrawStringAtOffsetFar(Unit_GetFar(menu->sideMember[0])->name,
                             menu->questionWindow, 16, 0);
  Text_DrawMessageAsTilesFar((menu->selectedItems[0] & INVENTORY_ITEM_ID_MASK) +
                                 MsgItemName,
                             menu->questionWindow, 16, 8);
}

/// Asks whether to drop item (an inventory entry): its name, the question, and
/// Yes or No, No first. Returns 0 for Yes and 1 for No, B or Start.
s32 ItemMenu_ConfirmDrop(s32 item) {
  struct Window *window;
  s32 itemId;
  s32 question;
  s32 answer;
  s32 rows = 2;
  s32 selection = 1;
  s32 changed = 1;

  window = Window_CreateFar(13, 3, 17, 10, WINDOW_FLAG_INSTANT);
  itemId = item & INVENTORY_ITEM_ID_MASK;
  Item_GetDirectFar(itemId);
  Text_DrawMessageAsTilesFar(itemId + MsgItemName, window, 24, 0);
  question = MsgConfirmDrop;
  Text_DrawMessageAsTilesFar(question, window, ITEM_DROP_QUESTION_X, 16);
  question++;
  Text_DrawMessageAsTilesFar(question, window, ITEM_DROP_QUESTION_X, 24);
  answer = MsgYes;
  Text_DrawMessageAsTilesFar(answer, window, 24, 40);
  answer++;
  Text_DrawMessageAsTilesFar(answer, window, 24, 56);
  MenuCursor_Slide(104, 86);
  for (;;) {
    if (GameFlag_GetFar(FLAG_MENU_START_PRESSED))
      break;
    if (changed) {
      changed = 0;
      selection = (selection + rows) % rows;
    }
    if (gKeysPressed & KEY_A) {
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      break;
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      selection = 1;
      break;
    }
    MenuCursor_Position(104, (selection << 4) + 70);
    if (gKeysRepeat & KEY_UP) {
      selection--;
      changed = 1;
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      selection++;
      changed = 1;
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
    }
    WaitFrames(1);
  }
  if (GameFlag_GetFar(FLAG_MENU_START_PRESSED)) {
    selection = 1;
  }
  Window_CloseFar((void *)window, 1);
  return selection;
}

/// After giving or trading the selected item to the member in side 1, offers to
/// equip it there, showing the stats it would give. Returns 0 when it is
/// equipped; 1 when the player says No, backs out, or the member cannot equip
/// it, and the member is left as before.
s32 ItemMenu_ConfirmEquip(s32 unused) {
  s32 rows = 2;
  s32 selection = 0;
  s32 changed = 1;
  struct ItemMenuWork *menu = gMenuWork;
  struct Unit *unit = Unit_GetFar(menu->sideMember[1]);
  struct Unit *saved;
  struct Window *window;
  s32 slot;
  s32 member;

  slot = menu->selectedSlots[1];
  member = menu->sideMember[1];
  ItemMenu_DrawEquipPreview(member, slot, 0, member);
  saved = Alloc(BATTLE_UNIT_SIZE);
  Iwram_CopyWords(saved, unit, BATTLE_UNIT_SIZE);
  window = menu->questionWindow;
  // Equip it now; a result of -1 or -2 means it cannot be equipped.
  if ((u32)(Inventory_EquipFar(menu->sideMember[1], menu->selectedSlots[1]) +
            2) <= 1) {
    selection = 1;
  } else {
#if LANGUAGE != LANGUAGE_JAPANESE
    Text_DrawMessageAsTilesFar(MsgYes, window, 24, 24);
    Text_DrawMessageAsTilesFar(MsgYes + 1, window, 72, 24);
    Window_ClearInteriorTilesFar((void *)window, 16, 16, 96, 24);
    Text_DrawMessageAsTilesFar(MsgEquipThisItem, window, 0, 16);
    MenuCursor_Slide(110, 32);
#else
    Text_DrawMessageAsTilesFar(MsgYes, window, 96, 0);
    Text_DrawMessageAsTilesFar(MsgYes + 1, window, 96, 16);
    Window_ClearInteriorTilesFar((void *)window, 16, 16, 96, 24);
    Text_DrawMessageAsTilesFar(MsgEquipThisItem, window, 8, 16);
    MenuCursor_Slide(184, 5);
#endif
    for (;;) {
      if (GameFlag_GetFar(FLAG_MENU_START_PRESSED))
        break;
      if (changed) {
        changed = 0;
        selection = (selection + rows) % rows;
      }
      if (gKeysPressed & KEY_A) {
        Sound_PlaySongFar(SE_NOTE_HIGH); // the Equip sound
        break;
      }
      if (gKeysPressed & KEY_B) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        selection = 1;
        break;
      }
#if LANGUAGE != LANGUAGE_JAPANESE
      MenuCursor_Position(selection * 48 + 110, 32);
      if (gKeysRepeat & KEY_LEFT) {
#else
      MenuCursor_Position(184, (selection << 4) + 5);
      if (gKeysRepeat & KEY_UP) {
#endif
        selection--;
        changed = 1;
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      }
#if LANGUAGE != LANGUAGE_JAPANESE
      if (gKeysRepeat & KEY_RIGHT) {
#else
      if (gKeysRepeat & KEY_DOWN) {
#endif
        selection++;
        changed = 1;
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      }
      WaitFrames(1);
    }
  }
  if (GameFlag_GetFar(FLAG_MENU_START_PRESSED))
    selection = 1;
  // No: put the member back as they were.
  if (selection == 1)
    Iwram_CopyWords(unit, saved, BATTLE_UNIT_SIZE);
  Free(saved);
  Unit_CalculateFar(menu->sideMember[1]);
  Unit_RefreshPsynergyFar(menu->sideMember[1]);
  return selection;
}

/// Loads the stat arrows (up and down) into two free VRAM blocks, kept in the
/// menu work's resource slots.
void Resource_LoadPairedBlocks(void) {
  struct ItemMenuWork *menu = gMenuWork;
  s32 block = VramCache_FindFreeEntry();

  menu->resourceSlots[0] = block;
  VramCache_Load(block, 128, gMenu_ArrowTiles);
  block = VramCache_FindFreeEntry();
  menu->resourceSlots[1] = block;
  VramCache_Load(block, 128, gMenu_SmallDownArrowTiles);
}

/// Fills result with the item list of the member in side index: their unit,
/// how many items they hold, and the page and row of the item chosen last for
/// them (kept within the list), five items to a page. Returns 1.
s32 ItemMenu_BuildPagedList(struct PagedList *result, s32 index) {
  s32 unit;
  struct ItemMenuWork *menu = gMenuWork;
  s32 itemCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selected;

  itemCount = ItemMenu_CountItems(menu->sideMember[index]);
  unit = (s32)Unit_GetFar(menu->sideMember[index]);
  selected = menu->selectedIndexByMember[menu->sideMember[index]];
  if ((selected + 1) > itemCount) {
    selected = itemCount - 1;
  }
  page = selected / LIST_PAGE_SIZE;
  row = selected % LIST_PAGE_SIZE;
  pageCount = itemCount / LIST_PAGE_SIZE;
  if (itemCount % LIST_PAGE_SIZE != 0) {
    pageCount++;
  }
  result->memberState = unit;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = itemCount;
  result->selectedIndex = selected;
  return 1;
}

/// Updates the item list for the row under the cursor: puts the selected item's
/// plain name in the info window and highlights its row of the five. window and
/// memberWork are not used.
s32 ItemMenu_DrawItemDetailPage(struct Window *window, void *memberWork,
                                struct PagedList *pageState) {
  struct ItemMenuWork *menu;
  s32 page;
  s32 index;
  s32 row;

  page = pageState->page;
  menu = gMenuWork;
  index = page * LIST_PAGE_SIZE;
  index += pageState->row;
  pageState->selectedIndex = index;

  MENU_RESET_DESCRIPTION_WINDOW(menu->descriptionWindow);
  WaitFrames(1);

  index = pageState->selectedIndex;
  if (menu->items[index] != 0) {
    s32 name = (menu->items[index] & INVENTORY_ITEM_ID_MASK) + MsgItemPlainName;
    MENU_DRAW_DESCRIPTION(name, menu->descriptionWindow, 0, 0);
  }

  row = 0;
  do {
    if (row == pageState->row) {
      Render_SetTilemapFlagRect((void *)menu->itemWindow, 1, row * 2 + 1, 14, 1,
                                HIGHLIGHT_PALETTE);
    } else {
      Render_SetTilemapFlagRect((void *)menu->itemWindow, 1, row * 2 + 1, 14, 1,
                                WINDOW_PALETTE);
    }
    row++;
  } while (row < LIST_PAGE_SIZE);

  WaitFrames(1);
  return 1;
}

/// Draws one page of the item list into window: up to five item names with
/// their icons, and the page numbers when there is more than one page.
s32 ItemMenu_DrawNamePage(struct Window *window, s32 unused,
                          const struct PagedList *pageState) {
  struct ItemMenuWork *menu = gMenuWork;
  u32 page;
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  const u16 *entry;

  (void)unused;

  WindowSprite_RedrawSavedRectFar(window);
  Window_DrawDividerLineFar(window, 0, 11, 16, 11);

  page = pageState->page;
  firstEntry = page * LIST_PAGE_SIZE;
  visibleCount = (u8)(pageState->entryCount - firstEntry);
  if (visibleCount > LIST_PAGE_SIZE) {
    visibleCount = LIST_PAGE_SIZE;
  }

  Menu_SetPageIcons(LIST_PAGE_SIZE, firstEntry, window, ITEM_LIST_PAGE_ICONS_X,
                    34);
  PagedList_DrawPageNumbers(window, pageState->entryCount, LIST_PAGE_SIZE,
                            pageState->page, 15);

  row = 0;
  if (visibleCount > row) {
    entry = &menu->items[firstEntry];
    do {
      Text_DrawMessageAsTilesFar(
          (entry[0] & INVENTORY_ITEM_ID_MASK) + MsgItemName, menu->itemWindow,
          ITEM_LIST_NAME_X, row * 16 + 8);
      row++;
      entry++;
    } while (visibleCount > row);
  }

  return 1;
}

UNUSED s32 ItemMenu_ItemNamePageReturnTrue(void) { return 1; }

UNUSED void ItemMenu_ItemNamePageNoOp(void) {}

/// The item list of the member in sideIndex (0: the holder, 1: the receiver of
/// a trade), five items a page. The cursor's item is named in the info window
/// and previewed in the status panel, its icon animates, and the party members
/// who can equip it move. Up and Down move the cursor, Left and Right turn
/// pages, and in sideIndex 0 L and R switch to the previous or next member who
/// has items. Returns the chosen inventory entry, or -1 when the player backs
/// out.
s32 ItemMenu_RunList(s32 sideIndex) {
  struct ItemMenuWork *menu;
  struct Window *window;
  s32 changed;
  s32 moved;
  s32 member;
  u8 i;
  s32 result;
  s32 redraw;
  s32 done;
  struct Unit *memberUnit;
  s32 previous;
  struct WindowSprite *icon;
  s32 work[5];
  struct PagedList state;

  menu = gMenuWork;
  result = 0;
  previous = 0;
  menu->equipPreview = result;
  Window_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, 2);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    memberUnit = Unit_GetFar(menu->sideMember[sideIndex]);
    menu->itemCount = ItemMenu_CollectItems(memberUnit, menu->items, 0);
    ItemMenu_DrawIcons(menu->items, 0);
    menu->selectedItemIcon->motion = ICON_HIDDEN;
    ItemMenu_BuildPagedList(&state, sideIndex);
    MenuCursor_Position(98, state.selectedIndex * 16 + 36);
    changed = 1;
    redraw = 1;

    // One member's list, until an item is chosen or the member changes.
    while (GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
      MenuCursor_Position(98, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (memberUnit->inventory[previous] != 0) {
          icon = menu->entryIcons[previous];
          Icon_PrepareObject(icon);
        }
        if (redraw != 0) {
          WaitFrames(1);
          ItemMenu_DrawNamePage(window, 0, &state);
          if (sideIndex == 0) {
            Text_DrawMessageAsTilesFar(MsgChangeCharacterHelp, window,
                                       HELP_TEXT_X, 88);
          }
          redraw = 0;
        }
        ItemMenu_DrawItemDetailPage(window, work, &state);
        menu->selectedItems[sideIndex] = menu->items[state.selectedIndex];
        ItemMenu_DrawEquipPreview(menu->sideMember[sideIndex],
                                  state.selectedIndex, 0,
                                  menu->sideMember[sideIndex]);
        if (menu->items[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          // Animate the selected icon: 9 is WINDOW_SPRITE_MOTION_SWELL, pulsing
          // between 1 and 1.5 times its size, from its first step, in depth
          // slot 250. Icon_PrepareObject stills it again when the cursor
          // leaves.
          icon->motion = 9;
          icon->animationStep = 0;
          icon->sentinel = 250;
        }
        for (i = 0; i < menu->partyCount; i++) {
          Sprite_StartAnimFar(menu->memberObjects[i], 1);
        }
      }

      // Every 32 frames, the members who can equip the item move.
      if ((gFrameCount & 31) == 0) {
        for (i = 0; i < menu->partyCount; i++) {
          if (Item_CanUnitEquip(menu->memberIds[i],
                                INVENTORY_ITEM_ID_MASK &
                                    menu->items[state.selectedIndex]) != 0) {
            Sprite_StartAnimFar(menu->memberObjects[i], 3);
          }
        }
      }

      WaitFrames(1);
      previous = state.selectedIndex;
      moved = PagedList_HandleInput(0, state.entryCount, LIST_PAGE_SIZE,
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
        Sound_PlaySongFar(SE_NOTE_LOW); // the item choice sound
        result = menu->items[state.selectedIndex];
        done = 1;
        break;
      }
      if ((gKeysPressed & KEY_B) != 0) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }
      // L and R: the previous or next member who has items (side 0 only).
      if ((gKeysRepeat & KEY_R) != 0 || (gKeysRepeat & KEY_L) != 0) {
        if (sideIndex == 1) {
          Sound_PlaySongFar(SE_MENU_ERROR);
          WaitFrames(1);
        } else {
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          menu->selectedIndexByMember[menu->sideMember[sideIndex]] =
              state.selectedIndex;
          member = menu->sideIndex[sideIndex];
          do {
            if ((gKeysRepeat & KEY_R) != 0) {
              member++;
            } else {
              member--;
            }
            member = (member + menu->partyCount) % menu->partyCount;
            menu->selectedMember = menu->memberIds[member];
            menu->sideMember[sideIndex] = menu->memberIds[member];
            menu->sideIndex[sideIndex] = member;
            menu->itemCount = ItemMenu_CollectItems(
                Unit_GetFar(menu->sideMember[sideIndex]), menu->items, 0);
          } while (menu->itemCount == 0);
          for (i = 0; i <= 3; i++) {
            menu->memberY[i] = 30;
          }
          menu->memberY[member] = 26;
          break;
        }
      }
    }
  }

  Window_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
  Icon_PrepareObject(menu->gridCursor);
  menu->selectedSlots[sideIndex] = state.selectedIndex;
  menu->selectedIndexByMember[menu->sideMember[sideIndex]] =
      state.selectedIndex;
  menu->selectedItems[sideIndex] = result;
  if (GameFlag_GetFar(FLAG_MENU_START_PRESSED) != 0) {
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

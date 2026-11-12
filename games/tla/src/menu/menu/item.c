/// @file
/// The item menu, as far as this object holds it: the field menu's Item
/// screen, its command loop, Use, the first screen's setup and the item icons.
// Part of menu.c.

#include "../menu_internal.h"

/// @file
/// The item menu, as far as this object holds it: the field menu's Item
/// screen, its command loop, Use, the first screen's setup and the item icons.
// Part of menu.c.

// Where the edition's text goes. The international editions have longer
// words, and some languages move a message to fit its own.
//
// The x of the small message windows "You were cursed!" and "Removed".
#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_MODAL_SHORT_X 14
#define ITEM_MODAL_REMOVED_X 14
#else
#define ITEM_MODAL_SHORT_X 15
#define ITEM_MODAL_REMOVED_X 17
#endif

// "Can't trade it!" starts four tiles further left in French, over the status
// window, which is redrawn after it.
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

void Resource_LoadPairedBlocks(void);
void ItemMenu_Close(void);
s32 ItemMenu_RunCommands(s32 *memberOut, s32 *targetOut, s32 *itemOut);
void ItemMenu_Initialize(s32 x, s32 y, s32 mode, s32 columns);
void ItemMenu_HideAllIcons(void);

static __inline__ s32 CopyWords_(void *destination, const void *source,
                                 s32 size) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants; as direct calls, 92 of 162 instructions differ in tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

static __inline__ s32 FillWords_(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants; as a direct call, 47 of 162 instructions differ in tla-en. */
  return Iwram_FillWords(destination, size, value);
}

/// The field menu's Item screen: choose a party member, one of their items and
/// a command. Owns the menu work while it runs and frees it before returning.
/// Returns ItemMenu_RunCommands' result: 1 when an item is to be used on the
/// field, -1 when the player backed out or pressed Start.
// The twin of The Broken Seal's ItemMenu_Run.
s32 Func_080f94a4(void) {
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

  screen = HeapSlot_Alloc(220, 0xcec);
#if TLA_PATCH_1
  size = 0x2000;
  backup = Heap_AllocEwram(size);
#endif
  Ram_HeapSlots->objectSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeWork(0);
  screen->partyCount = Party_ListActiveOwnersFar(screen->memberIds);
  ItemMenu_Initialize(0, 3, 0, 7);
  Resource_LoadPairedBlocks();
  Palette_LightenHighlight(14);
  Link_DrawShiftedTilePairFar(0x06002500);
  screen->questionWindow =
      (struct RenderInput *)Window_CreateFar(13, 0, 17, 3, 2);
#if TLA_PATCH_1
  Menu_SuspendFieldTasks();
  CopyWords_(backup, (void *)0x06004000, size);
  FillWords_((void *)0x06004000, size, 0x33333333);
  Text_UseExtraTilesFar(1);
#endif

  Menu_CancelSoundReset();
  result = ItemMenu_RunCommands(&owner, &target, &item);
  Menu_EnsureCancelSound();
  if (result == 1) {
    struct AssetSelectionResult {
      s32 unk000[0x5a];
      u16 chosenItem;
      u8 unk16a[0x18];
      u16 chosenSlot;
    } *selection = Ram_HeapSlots->eventWork;

    selection->chosenItem = (owner << 10) | (item & 0x1ff);
    selection->chosenSlot = screen->selectedSlots[0];
  }

  WindowSprite_ClearListFar(screen->statusWindow);
  Ram_HeapSlots->windowTiles[6] = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  Menu_ResetTwoResourceEntries();
  HeapSlot_Release(220);
  Ram_HeapSlots->objectSystem->suspended = 0;
#if TLA_PATCH_1
  Func_08038290();
  Text_UseExtraTilesFar(0);
  CopyWords_((void *)0x06004000, backup, size);
  Ram_HeapSlots->windowTiles[6] = 0;
  Heap_Release(backup);
  wait(1);
  Menu_ResumeFieldTasks();
#endif
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  Ram_HeapSlots->windowTiles[6] = 0;
  Event_ClearInvalidPackedValuesFar();
  return result;
}

/// The resident IWRAM word copy, called through its address.
typedef s32 (*ItemWordCopyFn)(void *dst, const void *src, s32 size);

/// Iwram_CopyWords's routine reached from IwramIrqMain rather than at its
/// fixed address, as in The Broken Seal: the trade's snapshot copies load the
/// linked address after their arguments (at Iwram_CopyWords the constant is
/// loaded before them).
#define ITEM_IWRAM_COPY_WORDS                                                  \
  ((ItemWordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))

s32 Unit_RefreshPsynergyFar(s32 unit);
void Func_080fae8c(s32 member, s32 slot, s32 mode, s32 target);
#if TLA_PATCH_2
void Func_08104484(void);
#endif

/// Runs the item menu: choose a party member, one of their items and a command
/// (Use, Equip, Give, Drop, Remove or Details), then carry it out, until the
/// player backs out or presses Start. Returns 1 when an item is to be used
/// outside the menu, with its party member, target and item id in the out
/// parameters; -1 when the player backed out or pressed Start; 0 otherwise.
s32 ItemMenu_RunCommands(s32 *ownerOut, s32 *targetOut, s32 *itemOut) {
  s32 done;
  s32 selection;
  s32 ret;
  struct BattleUnit *source;
  struct BattleUnit *target;
  struct ItemMenuWork *menu;
  struct ItemDefinition *item;
  void *sourceCopy;
  void *targetCopy;
  u32 state;
  s32 command;
  s32 result;
  s32 qty;
  s32 amount;
  s32 aborted;
  s32 work;
  s32 n;
  u8 movedSource;
  u8 movedTarget;
  s8 stack;
  s32 size;

  done = 0;
  selection = 0;
  ret = 0;
  state = 0;
  menu = Ram_HeapSlots->menuRuntime;

  while (done == 0 && GameFlag_Test(0x150) == 0) {
    switch (state) {
    case 0:
      menu->selectedSlots[0] = 0;
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      menu->selectedItemIcon->active = 13;
      ItemMenu_DrawMessage(0, (s32)&MsgWhoseItem);
#if TLA_PATCH_1
      // Japanese redraws the information window after each modal message
      // instead.
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
#endif
      Text_DrawWorkValueWithLabel((s32)menu->infoWindow);
      command = ItemMenu_SelectMember(0);
      if (command == -1) {
        /* FAKEMATCH(dead): command cleared with selection, though neither is
         * read again once done is set; as selection = 0;, 104 of 1355
         * instructions differ in tla-en. */
        selection = command = 0;
        ret = -1;
        done = 1;
      }
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
      ItemMenu_HideAllIcons();
      state = 1;
      break;

    case 1:
      if (ItemMenu_CountItems(menu->sideMember[0]) == 0) {
        state = 0;
        break;
      }
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      menu->selectedItemIcon->active = 13;
      menu->sideIcons[0]->active = 1;
      ItemMenu_DrawMessage(0, (s32)&MsgWhichItem);
      selection = Func_080fc6bc(0);
      state = 0;
      if (selection == -1) {
        break;
      }
      menu->listMode = 0xff;
      state = 9;
      break;

    case 9:
      command = Func_080fb104();
      if (command == -1) {
        state = 1;
        menu->completionFlag = 1;
      }
      if (command == 0) {
        if (BattleEffect_HasTriggerFar(menu->selectedItems[0] & 0x1ff) != 0) {
          done = 1;
          *ownerOut = menu->sideMember[0];
          *targetOut = command;
          *itemOut = menu->selectedItems[0] & 0x1ff;
          ret = 1;
          break;
        }
        result =
            Item_ClassifyUseMode(menu->sideMember[0], menu->selectedItems[0]);
        if (result == 1) {
          state = 2;
        }
        if (result == 2) {
          ItemMenu_Use();
          WindowSprite_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(
              menu->messageOffset + (s32)MsgItemUseResult, 0, -1);
#if !TLA_PATCH_1
          WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
#endif
          menu->sideIcons[0]->active = 13;
          menu->itemCount = ItemMenu_CollectItems(Unit_Get(menu->sideMember[0]),
                                                  menu->items, 0);
          ItemMenu_DrawIcons(menu->items, 0);
          state = 0;
        }
        if (result == -1 || result == 0) {
          done = 1;
          *ownerOut = menu->sideMember[0];
          *targetOut = menu->sideMember[1];
          *itemOut = menu->selectedItems[0] & 0x1ff;
          ret = 1;
        }
      }
      if (command == 1) {
        state = 3;
      }
      if (command == 3) {
        state = 6;
      }
      if (command == 5) {
        state = 5;
      }
      if (command == 4) {
        state = 11;
      }
      if (command == 2) {
        state = 10;
      }
      break;

    case 2:
      ItemMenu_HideAllIcons();
      ItemMenu_SetMessageWindowHeight5();
      ItemMenu_SetItemWindowRow5();
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->questionWindow);
      Func_080fc12c();
      Text_DrawCharacterAtOffsetFar((s32)MsgUseOnWhom, menu->questionWindow, 16,
                                    16);
      if (Func_080fa870(0) != -1) {
        command = 0;
        if (ItemMenu_IsSpecial(menu->selectedItems[0] & 0x1ff) != 0) {
          command = 8;
        }
        result = ItemMenu_Use();
        Func_080f8170(menu->statusWindow, menu->sideMember[1], 0, command);
        if (result != -1) {
          WindowSprite_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(
              menu->messageOffset + (s32)MsgItemUseResult, 0, -1);
#if !TLA_PATCH_1
          WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
#endif
          menu->sideIcons[0]->active = 13;
          Func_080fb6d4();
          state = 1;
        }
        menu->itemCount = ItemMenu_CollectItems(Unit_Get(menu->sideMember[0]),
                                                menu->items, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        menu->completionFlag = 1;
      } else {
        state = 9;
      }
      break;

    case 6:
      ItemMenu_SetMessageWindowHeight5();
      ItemMenu_SetItemWindowRow5();
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->questionWindow);
      Func_080fc12c();
      Text_DrawCharacterAtOffsetFar((s32)MsgGiveToWhom, menu->questionWindow,
                                    16, 16);
      n = Func_080fa870(1);
      state = 4;
      if (n == -1) {
        Func_080fae8c(menu->sideMember[0], menu->selectedSlots[0], 0,
                      menu->sideMember[0]);
        state = 9;
      }
      break;

    case 5:
      ItemMenu_HideAllIcons();
      item = Item_Get(menu->selectedItems[0] & 0x1ff);
      result = 0;
      if ((item->flags & 16) != 0) {
        qty = (menu->selectedItems[0] >> 11) + 1;
        if (qty > 1) {
          Func_080fc12c();
          result = Func_080fbe6c(0, qty, 1);
        }
      }
      if (result == -1) {
        state = 9;
        break;
      }
      menu->sideMember[1] = 0;
      // The icon's slot is read unsigned, as the bytes show.
      Func_08038288(2, (menu->selectedItems[0] & 0x1ff) | (result << 11),
                    (u8)menu->selectedItemIcon->index, 0);
      menu->selectedItemIcon->active = 1;
      menu->selectedItemIcon->x = 120;
      menu->selectedItemIcon->y = 28;
      Icon_PrepareActor(menu->selectedItemIcon);
      Window_ClearInteriorTilesFar(menu->listWindow, 0, 72, 120, 96);
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->questionWindow);
      if (Func_080fc1ac(selection) == 0) {
        command = menu->sideMember[0];
        // DEAD: the result is not used; the source keeps the call.
        Unit_Get(command);
        for (work = 0; work < result + 1; work++) {
          Inventory_RemoveFar(command, menu->selectedSlots[0]);
          Item_AdjustCounterFar(menu->selectedItems[0] & 0x1ff, 1);
        }
        /* FAKEMATCH(signature): Unit_RefreshPsynergyFar declared returning s32,
         * though this game's routine returns nothing, here and below; as void,
         * 4 of 1355 instructions differ in tla-en. */
        Unit_RefreshPsynergyFar(command);
        Unit_CalculateFar(command);
        ItemMenu_SetItemWindowRow3();
        ItemMenu_RefreshOwner(menu->sideMember[0], 0);
        menu->selectedItemIcon->active = 13;
        menu->sideIcons[0]->active = 13;
        wait(1);
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)MsgDroppedIt, 15, 13);
        menu->completionFlag = 1;
        state = 1;
      } else {
        state = 9;
      }
      Unit_CalculateFar(menu->sideMember[0]);
      menu->selectedItemIcon->active = 13;
      Event_ClearInvalidPackedValuesFar();
      break;

    case 4:
      aborted = 0;
      item = Item_Get(menu->selectedItems[0] & 0x1ff);
      if ((item->flags & 16) != 0) {
        qty = InventoryMenu_GetItemQuantity(menu->sideMember[1],
                                            menu->selectedItems[0] & 0x1ff);
        if (qty == 30) {
          aborted = 1;
        }
        if (ItemMenu_CountItems(menu->sideMember[1]) == 15 && qty == 0) {
          state = 7;
          break;
        }
        stack = (menu->selectedItems[0] >> 11) + 1;
        if (aborted == 0) {
          // A once-only block: it places a label between the count and its
          // sign-extended use (70 of 1355 differ as a plain assignment).
          do {
            amount = stack;
          } while (0);
          if (qty + amount > 30) {
            amount = 30 - qty;
          }
          if (stack > 1) {
            result = Func_080fbe6c(0, amount, 0);
          } else {
            result = 0;
          }
          if (result == -1) {
            state = 6;
            break;
          }
          for (command = 0; command < result + 1; command++) {
            qty = Inventory_AddItemFar(menu->sideMember[1],
                                       menu->selectedItems[0] & 0x5ff);
            if (qty != -1) {
              Inventory_RemoveFar(menu->sideMember[0], menu->selectedSlots[0]);
              menu->selectedSlots[1] = qty;
            } else {
              aborted = 1;
            }
          }
        }
      } else {
        result = Inventory_AddItemFar(menu->sideMember[1],
                                      menu->selectedItems[0] & 0x5ff);
        if (result == -1) {
          state = 7;
          break;
        }
        menu->selectedSlots[1] = result;
        result =
            Inventory_RemoveFar(menu->sideMember[0], menu->selectedSlots[0]);
        if (result == -1) {
          aborted = 1;
        }
      }
      Unit_RefreshPsynergyFar(menu->sideMember[0]);
      Unit_RefreshPsynergyFar(menu->sideMember[1]);
      Unit_CalculateFar(menu->sideMember[0]);
      Unit_CalculateFar(menu->sideMember[1]);
      result = 1;
      if (aborted == 0) {
        menu->sideMember[0] = menu->sideMember[1];
        menu->selectedItems[0] &= 0x1ff;
#if TLA_PATCH_2
        ItemMenu_SetMessageWindowHeight6();
#endif
        WindowSprite_RedrawSavedRectFar((struct Window *)menu->questionWindow);
        Func_080fc12c();
        result = Func_080fc2e0(0);
      }
      if (GameFlag_Test(0x150) != 0) {
        break;
      }
      ItemMenu_RefreshOwner(menu->sideMember[1], 1);
      menu->sideIcons[0]->active = 13;
      wait(1);
      if (aborted == 1) {
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)MsgCannotHoldMore, 15, 14);
      } else {
        WindowSprite_ClearListFar(menu->infoWindow);
        if (result == 1) {
          InventoryMenu_ShowModalMessage((s32)MsgGiven, 15, 14);
        } else {
          Func_080fae8c(menu->sideMember[1], menu->selectedSlots[1], 0,
                        menu->sideMember[1]);
          InventoryMenu_ShowModalMessage((s32)MsgEquippedIt, 15, 14);
          item = Item_Get(menu->selectedItems[0]);
          if ((item->flags & 1) != 0) {
            Sound_PlayFar(0x67);
            WindowSprite_ClearListFar(menu->infoWindow);
            InventoryMenu_ShowModalMessage((s32)MsgEquippedIt + 7,
                                           ITEM_MODAL_SHORT_X, 14);
          }
        }
      }
      Event_ClearInvalidPackedValuesFar();
      state = 0;
      break;

    case 7:
      aborted = 0;
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      ItemMenu_DrawMessage(0, (s32)&MsgSwapForWhat);
      if (Func_080fc6bc(1) == -1) {
        state = 6;
        break;
      }
      source = Unit_Get(menu->sideMember[0]);
      target = Unit_Get(menu->sideMember[1]);
      size = BATTLE_UNIT_SIZE;
      sourceCopy = Heap_Alloc(size);
      targetCopy = Heap_Alloc(size);
      ITEM_IWRAM_COPY_WORDS(sourceCopy, source, size);
      ITEM_IWRAM_COPY_WORDS(targetCopy, target, size);
      movedSource = 0;
      while (movedSource <= 29) {
        result =
            Inventory_RemoveFar(menu->sideMember[0], menu->selectedSlots[0]);
        if (result == 2) {
          break;
        }
        if (result == -1) {
          aborted = 1;
          break;
        }
        movedSource++;
      }
      movedSource++;
      movedTarget = 0;
      while (movedTarget <= 29) {
        if ((menu->selectedItems[1] & 0x200) != 0) {
          item = Item_Get(menu->selectedItems[1] & 0x1ff);
          if ((item->flags & 2) != 0) {
            aborted = 1;
          }
        }
        result =
            Inventory_RemoveFar(menu->sideMember[1], menu->selectedSlots[1]);
        if (result == 2) {
          break;
        }
        if (result == -1) {
          aborted = 1;
          break;
        }
        movedTarget++;
      }
      movedTarget++;
      while (movedSource != 0) {
        result = Inventory_AddItemFar(menu->sideMember[1],
                                      menu->selectedItems[0] & 0x5ff);
        if (result == -1) {
          aborted = 1;
          break;
        }
        menu->selectedSlots[1] = result;
        movedSource--;
      }
      while (movedTarget != 0) {
        result = Inventory_AddItemFar(menu->sideMember[0],
                                      menu->selectedItems[1] & 0x5ff);
        if (result == -1) {
          aborted = 1;
          break;
        }
        menu->selectedSlots[0] = result;
        movedTarget--;
      }
      wait(1);
      if (aborted == 1) {
        Iwram_CopyWords(source, sourceCopy, size);
        Iwram_CopyWords(target, targetCopy, size);
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)MsgCannotTrade, ITEM_CANNOT_TRADE_X,
                                       14);
#if LANGUAGE == LANGUAGE_FRENCH
        WindowSprite_RedrawSavedRectFar((struct Window *)menu->statusWindow);
#endif
      } else {
        Unit_RefreshPsynergyFar(menu->sideMember[0]);
        Unit_RefreshPsynergyFar(menu->sideMember[1]);
        Unit_CalculateFar(menu->sideMember[0]);
        Unit_CalculateFar(menu->sideMember[1]);
        ItemMenu_SetMessageWindowHeight5();
#if TLA_PATCH_2
        ItemMenu_SetMessageWindowHeight6();
        Func_08104484();
#endif
        WindowSprite_RedrawSavedRectFar((struct Window *)menu->questionWindow);
        menu->sideMember[0] = menu->sideMember[1];
        menu->selectedItems[0] &= 0x1ff;
        Func_080fc12c();
        result = Func_080fc2e0(0);
        if (GameFlag_Test(0x150) == 0) {
          WindowSprite_ClearListFar(menu->infoWindow);
          ItemMenu_SetItemWindowRow5();
          ItemMenu_RefreshOwner(menu->sideMember[1], 1);
          if (result == 0) {
            Func_080fae8c(menu->sideMember[1], menu->selectedSlots[1], 0,
                          menu->sideMember[1]);
            InventoryMenu_ShowModalMessage((s32)MsgEquippedIt, 15, 14);
            item = Item_Get(menu->selectedItems[0]);
            if ((item->flags & 1) != 0) {
              Sound_PlayFar(0x67);
              WindowSprite_ClearListFar(menu->infoWindow);
              InventoryMenu_ShowModalMessage((s32)MsgEquippedIt + 7,
                                             ITEM_MODAL_SHORT_X, 14);
            }
          } else {
            InventoryMenu_ShowModalMessage((s32)MsgTraded, ITEM_TRADED_X, 14);
          }
        }
      }
      Heap_Release(targetCopy);
      Heap_Release(sourceCopy);
      Event_ClearInvalidPackedValuesFar();
      state = 0;
      break;

    case 3:
      result = Inventory_EquipFar(menu->sideMember[0], menu->selectedSlots[0]);
      if (result == -1) {
        state = 1;
        break;
      }
      if (result == -2) {
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)MsgCannotRemoveItem, 0, -1);
#if !TLA_PATCH_1
        WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
#endif
        state = 1;
        break;
      }
      Unit_RefreshPsynergyFar(menu->sideMember[0]);
      Unit_CalculateFar(menu->sideMember[0]);
      menu->sideIcons[0]->active = 13;
      menu->itemCount =
          ItemMenu_CollectItems(Unit_Get(menu->sideMember[0]), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      wait(1);
      Func_080fae8c(menu->sideMember[0], menu->selectedSlots[0], 0,
                    menu->sideMember[0]);
      WindowSprite_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage((s32)MsgEquippedIt, 15, 8);
      item = Item_Get(menu->selectedItems[0]);
      if ((item->flags & 1) != 0) {
        Sound_PlayFar(0x67);
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)MsgEquippedIt + 7,
                                       ITEM_MODAL_SHORT_X, 8);
      }
      state = 1;
      break;

    case 11:
      ((struct BattleUnit *)Unit_Get(menu->sideMember[0]))->inventory[menu->selectedSlots[0]] &=
          0xfdff;
      Unit_RefreshPsynergyFar(menu->sideMember[0]);
      Unit_CalculateFar(menu->sideMember[0]);
      menu->sideIcons[0]->active = 13;
      menu->itemCount =
          ItemMenu_CollectItems(Unit_Get(menu->sideMember[0]), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      wait(1);
      menu->equipPreview = 1;
      Func_080fae8c(menu->sideMember[0], menu->selectedSlots[0], 0,
                    menu->sideMember[0]);
      menu->equipPreview = 0;
      WindowSprite_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage((s32)MsgRemoved, ITEM_MODAL_REMOVED_X, 8);
      Event_ClearInvalidPackedValuesFar();
      state = 1;
      break;

    case 10:
      menu->sideIcons[0]->active = 13;
      Func_080fb780(menu->selectedItems[0]);
      WindowSprite_RedrawSavedRectFar((struct Window *)menu->statusWindow);
      Func_080fae8c(menu->sideMember[0], menu->selectedSlots[0], 0,
                    menu->sideMember[0]);
      menu->sideIcons[0]->active = 1;
      state = 9;
      break;

    case 12:
      Func_080fbe6c(0, 30, 0);
      state = 1;
      break;

    case 8:
      break;

    default:
      done = 1;
      break;
    }
  }

  if (GameFlag_Test(0x150) != 0) {
    ret = -1;
  }
  return ret;
}

s32 Func_08100738(s32 slot, s32 owner, s32 target);
s32 Func_08100d40(s32 item);

/// Uses the selected item from the party member in side 0 on the one in side 1.
/// On failure plays the error sound, shows why in the info window and returns
/// -1; otherwise plays the item's animation, recalculates both party members
/// and returns 1.
s32 ItemMenu_Use(void) {
  struct ItemMenuWork *menu;
  s32 result;
  u32 item;
  u32 mask;

  menu = Ram_HeapSlots->menuRuntime;
  result = Func_08100738(menu->selectedSlots[0], menu->sideMember[0],
                         menu->sideMember[1]);

  if (result == -1) {
    Sound_PlayFar(114);
    WindowSprite_ClearListFar(menu->infoWindow);
    InventoryMenu_ShowModalMessage(menu->messageOffset + (s32)MsgItemUseResult,
                                   result, result);
    // Until patch 1, which closes the message's window instead.
#if !TLA_PATCH_1
    WindowSprite_RedrawSavedRectFar((struct Window *)menu->infoWindow);
#endif
    menu->completionFlag = 1;
    return result;
  }

  item = menu->selectedItems[0];
  /* FAKEMATCH(dead): an empty once-only block between the item load and the
   * 0x1ff mask; as mask = item & 0x1ff, 2 of 67 instructions differ in tla-en.
   */
  do {
  } while (0);
  mask = 0x1ff;
  mask &= item;
  Func_08100d40(mask);
  Unit_CalculateFar(menu->sideMember[0]);
  Unit_CalculateFar(menu->sideMember[1]);
  return 1;
}

/// Sets up the item menu's first screen: the cursor window with the party's
/// figures (Func_080f88d0, Menu_PlaceOwnerEntryObjects), the info window along
/// the bottom and an 8 by 2 cursor grid. The parameters are not used.
void ItemMenu_Initialize(s32 x, s32 y, s32 mode, s32 columns) {
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuRuntime;
  s32 ownerIndex;

  Menu_PlaceOwnerEntryObjects(Func_080f88d0((struct OwnerMenuState *)menu), 2,
                              2);
  for (ownerIndex = 3; ownerIndex >= 0; ownerIndex--)
    menu->memberY[ownerIndex] = 26;

  {
    u8 zero = 0;
    s32 style;
    menu->buttonHelpWindow = NULL;
    menu->statusWindow = NULL;
    style = 2;
    menu->infoWindow =
        (struct RenderInput *)Window_CreateFar(0, 17, 30, 3, style);
    menu->itemWindow = NULL;
    menu->selectedColumn = zero;
    menu->selectedRow = zero;
    menu->columnCount = 8;
    menu->rowCount = style;
  }
}

/// Creates the 32 item icon sprites in window, off screen until placed: slots 0
/// to 7 at x 248, the rest at x 256, all at y 168, each showing item graphic
/// slot of the same number.
void Menu_SpawnIconEntries(struct ItemMenuWork *menu,
                           struct RenderInput *window) {
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
    *firstIcons++ =
        WindowSprite_CreateFromResourceFar(2, firstSlot, window, 0xF8, firstY);
    firstSlot++;
  } while (firstSlot <= 7);
  secondSlot = 8;
  secondY = 0xA8;
  secondIcons = &menu->entryIcons[8];
  do {
    *secondIcons++ = WindowSprite_CreateFromResourceFar(2, secondSlot, window,
                                                        0x100, secondY);
    secondSlot++;
  } while (secondSlot <= 15);
  remainingSlot = 16;
  remainingY = 0xA8;
  remainingIcons = &menu->entryIcons[16];
  do {
    *remainingIcons++ = WindowSprite_CreateFromResourceFar(
        2, remainingSlot, window, 0x100, remainingY);
    remainingSlot++;
  } while (remainingSlot <= 31);
}

/// Hides all 32 item icons.
void ItemMenu_HideAllIcons(void) {
  s32 hiddenState = 13;
  struct WindowSprite **icons =
      ((struct ItemMenuWork *)Ram_HeapSlots->menuRuntime)->entryIcons;
  s32 slot;

  for (slot = 31; slot >= 0; slot--) {
    struct WindowSprite *icon = *icons++;
    if (icon != 0) {
      icon->active = hiddenState;
    }
  }
}

#if TLA_PATCH_2
/// Hides the item icons in every fifth slot, 0 to 30, after a trade.
void Func_08104484(void) {
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuRuntime;
  s32 slot;

  for (slot = 0; slot <= 31; slot++) {
    struct WindowSprite *icon = menu->entryIcons[slot];
    if (icon != 0 && slot % 5 == 0)
      icon->active = 13;
  }
}
#endif

#undef ITEM_IWRAM_COPY_WORDS

void Menu_ReleaseEntryObjects(void);

void ItemMenu_Close(void) {
  u8 *menu;
  u8 *cursor;

  menu = Ram_HeapSlots->menuRuntime;
  Menu_ReleaseEntryObjects();
  ItemMenu_HideAllIcons();
  wait(1);
  cursor = *(u8 **)(menu + 0x170);
  cursor[5] = 13;
  Window_CloseIfOpen(menu + 0x10, 1);
  Window_CloseIfOpen(menu + 0x24, 1);
  Window_CloseIfOpen(menu + 0xf0, 1);
  Window_CloseIfOpen(menu + 0x28, 1);
  Window_CloseIfOpen(menu + 0x2c, 1);
  Window_CloseIfOpen(menu + 0x30, 1);
  Window_CloseIfOpen(menu + 0x34, 1);
  Window_CloseIfOpen(menu + 0x38, 1);
  Window_CloseIfOpen(menu + 0x3c, 1);
  Window_CloseIfOpen(menu + 0x40, 1);
  Window_CloseIfOpen(menu + 0x44, 1);
}

struct ItemMenuIcon {
  u8 unk000[5];
  u8 state;
  u8 unk006[2];
};

struct ItemMenuOwner {
  u8 unk000[0xd8];
  u16 inventory[15];
};

s32 ItemMenu_CollectItems(struct OwnerInventoryState *owner, u16 *items, s32 mode);
s32 ItemMenu_RunOwnerSelection(u16 *ownerIds, u16 *items);

s32 ItemMenu_SelectMember(s32 pane) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
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
    Func_080f8ab4(index * 24 - 10, 16);
  }
  // Read unsigned (ldrh): the party's ids, as this routine's bytes load them.
  owner = Unit_Get((u16)menu->memberIds[index]);
  menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
  result = ItemMenu_RunOwnerSelection(menu->memberIds, menu->items);
  Icon_PrepareActor(menu->sideIcons[pane]);
  wait(1);
  return result;
}

/// The item menu's help, followed by the equipment help. "L+A: Arrange
/// items"; the next message is "R: View equipment".
extern u8 MsgArrangeItemsHelp[];
#if LANGUAGE == LANGUAGE_JAPANESE
/// The key names Japanese draws before the help: "L+A:" and "R:".
extern const u8 gItemMenu_ArrangeKeysString[];
extern const u8 gItemMenu_EquipmentKeyString[];
#endif

void Func_080fa84c(s32 owner);

void ItemMenu_RefreshOwner(s32 owner, s32 mode);

s32 ItemMenu_CountItems(s32 owner);

/// The party member row of the item menu: moving shows each member's items and
/// status, holding R their equipment; L + A sorts the member's items, a new
/// order each press; A chooses a member who has items and B backs out. The
/// members are paged four at a time by Menu_MoveGridCursor, which moves the
/// selection and says what to redraw (2 for a new page). Returns the chosen
/// member's id, or -1.
// As The Broken Seal's ItemMenu_RunOwnerSelection.
s32 ItemMenu_RunOwnerSelection(u16 *ownerIds, u16 *items) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
  s32 selection = menu->sideIndex[0];
  /* FAKEMATCH(temporary): the first owner's byte offset, taken before the
   * count; as Unit_Get(ownerIds[selection]) below, 2 of 307 instructions
   * differ in tla-en. */
  s32 firstOffset = selection * 2;
  s32 count;
  s32 redraw;
  s32 result;
  u8 sortMode;
  s32 showEquipment;
  struct ItemMenuOwner *owner;
  void *window;

  count = menu->sideCount[0];
  redraw = 2;
  result = 0;
  sortMode = 0;
  showEquipment = 0;
  owner = Unit_Get(*(u16 *)(firstOffset + (s32)ownerIds));

  if (Window_UpdateOrCreate(&menu->itemWindow, 13, 3, 17, 10, redraw))
    Menu_SpawnIconEntries(menu, menu->itemWindow);
  if (Window_UpdateOrCreate(&menu->buttonHelpWindow, 13, 13, 17, 4, redraw)) {
    menu->selectedItemIcon =
        WindowSprite_CreateFromResourceFar(2, 0, menu->buttonHelpWindow, 0, result);
    menu->selectedItemIcon->active = 13;
  }
#if LANGUAGE == LANGUAGE_JAPANESE
  // Japanese draws the key names as their own strings, the help beside them.
  Text_DrawStringInWindowFar(gItemMenu_ArrangeKeysString, menu->buttonHelpWindow, 0,
                               0);
  Text_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp, menu->buttonHelpWindow,
                                  32, 0);
  Text_DrawStringInWindowFar(gItemMenu_EquipmentKeyString, menu->buttonHelpWindow,
                               16, 8);
  Text_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp + 1,
                                  menu->buttonHelpWindow, 32, 8);
#else
  Text_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp, menu->buttonHelpWindow, 0,
                                  0);
  Text_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp + 1,
                                  menu->buttonHelpWindow, 0, 8);
#endif
  menu->sideIcons[0]->active = 1;
  while (!GameFlag_Test(0x150)) {
    if (redraw) {
      sortMode = 0;
      window = menu->statusWindow;
      owner = Unit_Get(ownerIds[selection]);
      if (showEquipment) {
        menu->itemCount = ItemMenu_CollectItems(Unit_Get(ownerIds[selection]),
                                                menu->items, 0);
        Func_080fa84c(ownerIds[selection]);
        Func_080f8170(window, ownerIds[selection], 0, 8);
      } else {
        ItemMenu_RefreshOwner(ownerIds[selection], 0);
        Func_080f8170(window, ownerIds[selection], 0, 0);
      }
      if (redraw == 2) {
        Menu_CreateOwnerEntryObjects(selection / 4 * 4);
        wait(1);
      }
      Menu_HighlightPageEntry(menu->mainWindow, selection, count);
    }
    Func_080f8a44(selection % 4 * 24 - 10, 16);
    wait(1);
    if (gInput.pressed & 1) {
      if (gInput.held & 0x200) {
        sortMode = (sortMode + 4) % 4;
        InventoryMenu_SortByListOrder(owner->inventory, sortMode);
        sortMode++;
        ItemMenu_RefreshOwner(ownerIds[selection], 0);
        Sound_PlayFar(112);
      } else if (ItemMenu_CountItems(ownerIds[selection])) {
        Sound_PlayFar(112);
        result = ownerIds[selection];
        break;
      } else {
        Sound_PlayFar(114);
      }
    }
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if (gInput.pressed & 0x100) {
      showEquipment = 1;
      redraw = 1;
    } else if (!(gInput.held & 0x100) && showEquipment == 1) {
      showEquipment = 0;
      redraw = 1;
    } else {
      redraw = Menu_MoveGridCursor(&selection, count, 4);
    }
  }
  menu->sideIndex[0] = selection;
  menu->selectedMember = ownerIds[selection];
  menu->sideMember[0] = ownerIds[selection];
  menu->ownerIcons[0]->active = 13;
  menu->ownerIcons[1]->active = 13;
  return result;
}

void Func_080fa84c(s32 category) {
  u8 *menu = Ram_HeapSlots->menuRuntime;

  ItemMenu_PositionCategory();
  WindowSprite_RedrawSavedRectFar(*(void **)(menu + 36));
  ItemMenu_DrawCategory(*(void **)(menu + 36), category, 0);
}

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define TRADE_LABEL_X 16
#define IN_STOCK_LABEL_X 24
#define ITEM_INFO_DRAW Text_DrawMessageFar
#else
#define TRADE_LABEL_X 0
#define ITEM_INFO_DRAW Text_DrawCharacterAtOffsetFar
#endif

// The Japanese release clears the info window; from patch 1 it redraws the
// window's saved rectangle.
#if TLA_PATCH_1
#define ITEM_INFO_CLEAR WindowSprite_RedrawSavedRectFar
#else
#define ITEM_INFO_CLEAR WindowSprite_ClearListFar
#endif

#if (LANGUAGE == LANGUAGE_SPANISH)
#define IN_STOCK_LABEL_X 30
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define IN_STOCK_LABEL_X 29
#elif (LANGUAGE != LANGUAGE_JAPANESE)
#define IN_STOCK_LABEL_X 32
#endif

struct TargetMarker {
  u8 unk00[5];
  u8 state;
  u16 x;
  u8 unk08[0xc];
  u8 oamY;
  u8 oamAttr0;
  u16 oamX : 9;
  u16 oamAttr1 : 7;
};

struct TargetWindow {
  u8 unk00[0xc];
  u16 x;
};

struct TargetMenu {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  struct TargetWindow *pageWindow;
  u8 unk014[4];
  struct TargetMarker *marker;
  u8 unk01c[0];
  s8 paneIndex[2];
  s8 ownerCount;
  u8 unk01f[5];
  void *itemWindow;
  void *statusWindow;
  u8 unk02c[4];
  void *infoWindow;
  u8 unk034[0x134];
  u16 selectedSlots[2];
  u16 selectedItems[2];
  u8 unk170[8];
  struct TargetMarker *ownerIcons[2];
  u8 unk180[0x84];
  u16 ownerIds[8];
  u8 unk214;
  u8 partyCount;
  u8 paneOwner[2];
};

void Func_080fabe0(void);

s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item);

s32 ItemMenu_IsSpecial(s32 item);

void Func_080fac58(void);

s32 Func_080fa870(s32 mode) {
  struct TargetMenu *menu = Ram_HeapSlots->menuRuntime;
  s32 selection = menu->paneIndex[1];
  void *window = menu->itemWindow;
  s32 count = menu->partyCount;
  u8 result = 0;
  s32 descriptionShown = 0;
  s8 redraw;
  s32 heldCount;
  s32 label;
  s32 labelX;
  struct TargetMarker *marker;

  Window_SetBounds(window, 13, 5, 17, 12);
  WindowSprite_RedrawSavedRectFar(menu->itemWindow);
  Unit_Get(menu->ownerIds[menu->paneIndex[0]]);
  Task_Add(Func_080fabe0, 0x480);
  redraw = 2;
  while (!GameFlag_Test(0x150)) {
    if (redraw) {
      window = menu->itemWindow;
      Unit_Get(menu->ownerIds[selection]);
      marker = menu->marker;
      marker->oamX = marker->x =
          ((selection * 3 + menu->pageWindow->x) << 3) - 2;
      if (redraw == 2) {
        Menu_CreateOwnerEntryObjects(selection / 4 * 4);
        wait(1);
      }
      Menu_HighlightPageEntry(menu->pageWindow, selection, count);
      if (mode == 1) {
        ItemMenu_RefreshOwner(menu->ownerIds[selection], 1);
        Window_DrawDividerLineFar(window, 0, 9, 16, 9);
        Window_ClearInteriorTilesFar(window, 0, 72, 120, 80);
        if (selection != menu->paneIndex[0]) {
          heldCount = InventoryMenu_GetItemQuantity(
              menu->ownerIds[selection], menu->selectedItems[0] & 0x1ff);
          if (ItemMenu_CountItems(menu->ownerIds[selection]) == 15 &&
              heldCount == 0) {
            labelX = TRADE_LABEL_X;
            label = (s32)MsgTradeForWhat;
          } else if (heldCount != 0) {
            Text_DrawNumberInWindowFar(heldCount, 2, window, 8, 72);
            /* FAKEMATCH(dead): an empty once-only block between
             * `Text_DrawNumberInWindowFar(heldCount, 2, window, 8, 72);` and
             * `labelX = IN_STOCK_LABEL_X;`, which keeps their instructions
             * apart; as plain statements, 2 of 387 instructions differ in
             * tla-en. */
            do {
            } while (0);
            labelX = IN_STOCK_LABEL_X;
            label = (s32)MsgInStock;
          } else {
            labelX = 16;
            /* FAKEMATCH(dead): an empty once-only block between `labelX = 16;`
             * and `label = (s32)MsgNoneInStock;`, which keeps their
             * instructions apart; as plain statements, 2 of 387 instructions
             * differ in tla-en. */
            do {
            } while (0);
            label = (s32)MsgNoneInStock;
          }
          Text_DrawCharacterAtOffsetFar(label, window, labelX, 72);
        }
        Func_080fae8c(menu->paneOwner[0], menu->selectedSlots[0], 0,
                      menu->ownerIds[selection]);
      }
      if (mode == 0) {
        if (ItemMenu_IsSpecial(menu->selectedItems[0] & 0x1ff))
          Func_080f8170(menu->statusWindow, menu->ownerIds[selection],
                        menu->selectedSlots[0], 8);
        else
          Func_080f8170(menu->statusWindow, menu->ownerIds[selection],
                        menu->selectedSlots[0], 0);
        if (!GameFlag_Test(0x151) && !descriptionShown) {
          ITEM_INFO_CLEAR(menu->infoWindow);
          ITEM_INFO_DRAW((menu->selectedItems[0] & 0x1ff) +
                             (s32)MsgItemPlainName,
                         menu->infoWindow, 0, 0);
          descriptionShown = 1;
        } else {
          GameFlag_ClearBit(0x151);
        }
      }
      redraw = 0;
    }
    Func_080f8a44(selection % 4 * 24 - 10, 16);
    wait(1);
    if (gInput.pressed & 1) {
      if (mode == 1 && selection == menu->paneIndex[0]) {
        Sound_PlayFar(114);
        continue;
      }
      Sound_PlayFar(112);
      result = menu->ownerIds[selection];
      break;
    }
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      result = 255;
      break;
    }
    redraw = Menu_MoveGridCursor(&selection, count, 4);
  }
  marker = menu->marker;
  Icon_PrepareActor(marker);
  marker->state = 13;
  Func_080fac58();
  wait(1);
  menu->paneIndex[1] = selection;
  menu->selectedOwner = menu->ownerIds[selection];
  menu->paneOwner[1] = menu->ownerIds[selection];
  selection = menu->paneIndex[0];
  count = menu->ownerCount;
  Menu_CreateOwnerEntryObjects(selection / 4 * 4);
  Menu_HighlightPageEntry(menu->pageWindow, selection, count);
  menu->ownerIcons[0]->state = 13;
  menu->ownerIcons[1]->state = 13;
  return (s8)result;
}

#undef IN_STOCK_LABEL_X
#undef IN_STOCK_LABEL_X
#undef IN_STOCK_LABEL_X
#undef IN_STOCK_LABEL_X

struct ItemMenuIcons {
  u8 unk000[0xf8];
  void *ownerIcons[8];
  u8 unk118[0x40];
  s16 owners[10];
  u16 item;
  u8 unk16e[0xa7];
  u8 partyCount;
  u8 unk216[2];
};

void Func_080fabe0(void) {
  struct ItemMenuIcons *menu = Ram_HeapSlots->menuRuntime;
  s8 i;

  if ((gFrameCount & 31) == 0) {
    for (i = 0; i <= 3; i++) {
      s32 owner = menu->owners[i];

      if (owner != -1) {
        if (Item_CanOwnerEquip(owner, menu->item & 0x1ff))
          Animation_ApplyChildArgumentFar(menu->ownerIcons[i], 3);
        else
          Animation_ApplyChildArgumentFar(menu->ownerIcons[i], 1);
      }
    }
  }
}

void Func_080fac58(void) {
  struct ItemMenuIcons *menu = Ram_HeapSlots->menuRuntime;
  s8 i;

  for (i = 0; i < menu->partyCount; i++)
    Animation_ApplyChildArgumentFar(menu->ownerIcons[i], 1);
  Task_Remove(Func_080fabe0);
}

struct ItemListMenu {
  u8 unk000[0xf0];
  struct WindowSprite *messageWindow;
};

s32 ItemMenu_IsSpecial(s32 item_id) {
  s32 result;
  s32 first;

  if (item_id > 0xC4) {
    goto L0;
  }
  first = 0xC1;
  if (item_id < first) {
    goto L0;
  }
  result = 1;
  return result;
L0:
  result = 0;
  return result;
}

void ItemMenu_DrawMessage(s32 unused, s32 message) {
  struct ItemListMenu *menu;

  menu = Ram_HeapSlots->menuRuntime;
  WindowSprite_RedrawSavedRectFar(menu->messageWindow);
  Text_DrawCharacterAtOffsetFar(message, (s32)menu->messageWindow, 0, 0);
}

struct ItemEntryIcon {
  u8 unk00[5];
  u8 state;
  u8 unk06[8];
  u8 slot;
};

struct ItemEntryMenu {
  u8 unk000[0x24];
  s32 itemWindow;
  u8 unk028[0x24];
  struct ItemEntryIcon *entryIcons[32];
  u8 unk0cc[0xf8];
  u16 items[32];
  u8 unk204[0x10];
  u8 itemCount;
  u8 unk215[3];
};

void Menu_HideEmptyEntryIcons(const u16 *items) {
  struct ItemEntryMenu *menu = Ram_HeapSlots->menuRuntime;
  s32 slot;

  for (slot = 0; slot < 32; slot++) {
    if (items[slot] == 0) {
      Icon_PrepareActor(menu->entryIcons[slot]);
      menu->entryIcons[slot]->state = 13;
    }
  }
}

/// Counts the nonempty slots in the owner's inventory.
s32 ItemMenu_CountItems(s32 owner_id) {
  s32 count = 0;
  struct OwnerInventoryState *state = Unit_Get(owner_id);
  s32 i;

  for (i = 0; i < 15; i++) {
    s32 item_id = state->inventory[i] & 0x1ff;

    if (item_id != 0)
      count++;
  }
  return count;
}

s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item) {
  struct OwnerInventoryState *state;
  s32 i;
  s32 quantity;

  quantity = 0;
  state = Unit_Get(owner);
  i = 0;
  do {
    if (state->inventory[i] != 0 && (state->inventory[i] & 0x1ff) == item) {
      quantity += (state->inventory[i] & 0xf800) >> 11;
      quantity++;
    }
    i++;
  } while (i <= 14);
  return quantity;
}

s32 ItemMenu_CollectItems(struct OwnerInventoryState *owner, u16 *items, s32 mode) {
  s32 count;
  s32 i;

  for (i = 0; i < 32; i++) {
    items[i] = 0;
  }
  count = 0;
  for (i = 0; i < 15; i++) {
    items[i] = 0;
    if (owner->inventory[i] != 0) {
      items[count] = owner->inventory[i];
      count++;
    }
  }
  return count;
}

void ItemMenu_DrawIcons(u16 *items, s32 style) {
  s32 remaining;
  u16 *entries;
  struct ItemEntryIcon **icons;
  s32 itemId;

  icons = ((struct ItemEntryMenu *)Ram_HeapSlots->menuRuntime)->entryIcons;
  entries = items;
  remaining = 14;
  do {
    itemId = *entries++;
    if (itemId != 0) {
      if (style == 0) {
        Func_08038288(2, itemId, (*icons)->slot, 0);
      } else {
        Func_08038288(7, itemId, (*icons)->slot, 0);
      }
    }
    icons++;
    remaining--;
  } while (remaining >= 0);
  Menu_HideEmptyEntryIcons(items);
}

void ItemMenu_RefreshOwner(s32 ownerId, s32 mode) {
  struct ItemEntryMenu *menu;
  struct OwnerInventoryState *owner;
  u16 *items;

  menu = Ram_HeapSlots->menuRuntime;
  owner = Unit_Get(ownerId);
  items = menu->items;
  menu->itemCount = ItemMenu_CollectItems(owner, items, 0);
  WindowSprite_RedrawSavedRectFar(menu->itemWindow);
  ItemMenu_RefreshEntry(mode);
  ItemMenu_DrawIcons(items, 0);
  if (ItemMenu_CountItems(ownerId) == 0)
    Text_DrawCharacterAtOffsetFar((s32)MsgItemMenuEmpty, menu->itemWindow, 8,
                                    24);
}

struct PreviewMenu {
  u8 unk000[0x28];
  void *statusWindow;
};

struct PreviewUnit {
  u8 unk000[0xd8];
  u16 inventory[15];
};

struct PreviewItem {
  u8 unk00[2];
  u8 type;
};

void Func_080fae8c(s32 owner, s32 slot, s32 mode, s32 target) {
  struct PreviewMenu *menu = Ram_HeapSlots->menuRuntime;
  s32 style = 0;
  struct PreviewUnit *unit = Unit_Get(owner);
  s32 entry = unit->inventory[slot];
  s32 previewSlot;
  void *buffer;
  struct PreviewItem *item = Item_Get(entry & 0x1ff);

  if (mode == 1)
    style = 0x100;
  switch (item->type) {
  case 0:
  case 11:
    /* FAKEMATCH(dead): the test of menu, whose two branches make the same call,
     * kept; as the one call, 34 of 217 instructions differ in tla-en. */
    if (menu)
      Func_080f8170(menu->statusWindow, target, slot, style);
    else
      Func_080f8170(menu->statusWindow, target, slot, style);
    break;
  case 10:
    if (owner == target) {
      style |= 9;
      Func_080f8170(menu->statusWindow, target, slot, style);
    } else {
      s32 size;

      unit = Unit_Get(target);
      size = 332;
      buffer = Heap_Alloc(size);
      Iwram_CopyWords(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        entry &= ~0x200;
        previewSlot = Inventory_AddItemFar(target, entry);
        if (previewSlot != -1) {
          style |= 9;
          Func_080f8170(menu->statusWindow, target, previewSlot, style);
        } else {
          Func_080f8170(menu->statusWindow, target, slot, style);
        }
      } else {
        Func_080f8170(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWords(unit, buffer, size);
      Heap_Release(buffer);
    }
    break;
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 7:
  case 8:
  case 9:
    if (owner == target) {
      style |= 2;
      Func_080f8170(menu->statusWindow, target, slot, style);
    } else {
      s32 size;

      unit = Unit_Get(target);
      size = 332;
      buffer = Heap_Alloc(size);
      Iwram_CopyWords(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        entry &= ~0x200;
        previewSlot = Inventory_AddItemFar(target, entry);
        if (previewSlot != -1) {
          style |= 2;
          Func_080f8170(menu->statusWindow, target, previewSlot, style);
        } else {
          Func_080f8170(menu->statusWindow, target, slot, style);
        }
      } else {
        Func_080f8170(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWords(unit, buffer, size);
      Heap_Release(buffer);
    }
    break;
  case 6:
    if (target == owner) {
      style |= 4;
      Func_080f8170(menu->statusWindow, target, slot, style);
    } else {
      s32 size;

      unit = Unit_Get(target);
      size = 332;
      buffer = Heap_Alloc(size);
      Iwram_CopyWords(buffer, unit, size);
      previewSlot = Inventory_RemoveFirstUnflagged(target);
      if (previewSlot != 0) {
        previewSlot = Inventory_AddItemFar(target, entry);
        if (previewSlot != -1) {
          style |= 4;
          Func_080f8170(menu->statusWindow, target, previewSlot, style);
        } else {
          Func_080f8170(menu->statusWindow, target, slot, style);
        }
      } else {
        Func_080f8170(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWords(unit, buffer, size);
      Heap_Release(buffer);
    }
    break;
  }
}

s32 Inventory_RemoveFirstUnflagged(s32 owner) {
  struct OwnerInventoryState *state = Unit_Get(owner);
  s32 result = 0;
  s32 i;

  for (i = 0; i < INVENTORY_SLOTS; i++) {
    if (state->inventory[i] == 0) {
      result = 1;
      break;
    }
    if ((state->inventory[i] & INVENTORY_EQUIPPED) == 0) {
      s32 quantity = state->inventory[i] >> INVENTORY_QUANTITY_SHIFT;
      s32 count = quantity + 1;
      s32 cnt;

      if (quantity == 0)
        count = 1;
      for (cnt = 0; cnt < count; cnt++)
        result = Inventory_RemoveFar(owner, i);
      if (result != 2)
        return 0;
      result = 1;
      break;
    }
  }
  return result;
}

// Where the cursor points at a command. The German, French and Spanish
// releases ask two helpers of their own (named by their German addresses):
// the first reads x from a table that sets the middle column 9 pixels left of
// the even 40-pixel spacing the other releases compute, the second gives y.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) ||          \
    (LANGUAGE == LANGUAGE_SPANISH)
s32 Func_08106b80(s32 column, s32 row);
s32 Func_08106ba8(s32 column, s32 row);
#define COMMAND_CURSOR_X(column, row) Func_08106b80(column, row)
#define COMMAND_CURSOR_Y(column, row) Func_08106ba8(column, row)
#else
#define COMMAND_CURSOR_X(column, row) ((column) * 40 + 86)
#define COMMAND_CURSOR_Y(column, row) ((row) * 8 + 30)
#endif

struct CommandMenu {
  u8 unk000[0x28];
  void *statusWindow;
  u8 unk02c[0xc];
  void *listWindow;
  u8 unk03c[0xb4];
  void *messageWindow;
  u8 unk0f4[0x74];
  u16 selectedSlots[2];
  u8 unk16c[0xaa];
  u8 paneOwner[2];
  u8 unk218[4];
  u16 flags;
  u8 unk21e[0x42];
  u8 equipPreview;
  s8 listMode;
};

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

void Func_080fb410(s8 *commandStates);

void ItemMenu_SetMessageWindowHeight7(void);

void Func_080fc12c(void);
void ItemMenu_DrawCommands(const s8 *commandStates, s32 window);

s32 Func_080fb104(void) {
  struct CommandMenu *menu;
  s8 commandStorage[6];
  u16 *redrawFlag;
  s32 column;
  s32 lastCommand;
  s32 row;
  s32 commandIndex;
  s32 redraw;
  s8 *commandStates;

  menu = Ram_HeapSlots->menuRuntime;
  commandIndex = 0;
  column = 0;
  row = 0;
  redraw = 1;
  commandStates = commandStorage;
  Func_080fb410(commandStates);
  redrawFlag = &menu->flags;
  if (*redrawFlag != 1) {
    void *messageWindow;

    ItemMenu_HideAllIcons();
    WindowSprite_RedrawSavedRectFar(menu->listWindow);
    messageWindow = menu->messageWindow;
    ItemMenu_SetMessageWindowHeight7();
    WindowSprite_RedrawSavedRectFar(messageWindow);
    Window_DrawDividerLineFar(messageWindow, 0, 3, 16, 3);
    Func_080fc12c();
    ItemMenu_DrawCommands(commandStates, messageWindow);
  }
  *redrawFlag = 0;

  lastCommand = menu->listMode;
  if (lastCommand == -1) {
    if (commandStates[ITEM_COMMAND_INSPECT] == 1) {
      column = 2;
      row = 0;
    }
    if (commandStates[ITEM_COMMAND_GIVE] == 1) {
      column = 0;
      row = 1;
    }
    if (commandStates[ITEM_COMMAND_EQUIP] == 1) {
      column = 1;
      row = 0;
    }
    if (commandStates[ITEM_COMMAND_REMOVE] == 1) {
      column = 1;
      row = 1;
    }
    if (commandStates[ITEM_COMMAND_USE] == 1) {
      column = 0;
      row = 0;
    }
  } else {
    column = (s8)(lastCommand % 3);
    row = (s8)(lastCommand / 3);
    commandIndex = row * 3 + column;
  }
  Func_080f8ab4(COMMAND_CURSOR_X(column, row), COMMAND_CURSOR_Y(column, row));

  for (;;) {
    if (GameFlag_Test(0x150) != 0)
      break;
    if (redraw != 0) {
      redraw = 0;
      column = (column + 3) % 3;
      row = (row + 2) % 2;
      commandIndex = row * 3 + column;
      Func_080fac58();
      if (commandIndex > 2) {
        menu->equipPreview = 1;
        Func_080fae8c(menu->paneOwner[0], menu->selectedSlots[0], 0,
                      menu->paneOwner[0]);
        if (commandIndex == ITEM_COMMAND_GIVE)
          Task_Add(Func_080fabe0, 0x480);
      } else if (commandIndex != 0) {
        menu->equipPreview = 0;
        Func_080fae8c(menu->paneOwner[0], menu->selectedSlots[0], 0,
                      menu->paneOwner[0]);
      } else {
        Func_080f8170(menu->statusWindow, menu->paneOwner[0], 0, 0);
      }
    }
    Func_080f8a44(COMMAND_CURSOR_X(column, row), COMMAND_CURSOR_Y(column, row));
    wait(1);

    if ((gInput.pressed & 1) != 0) {
      if (commandStates[commandIndex] == -1) {
        Sound_PlayFar(114);
      } else {
        switch (commandIndex) {
        case ITEM_COMMAND_USE:
          Sound_PlayFar(174);
          break;
        case ITEM_COMMAND_EQUIP:
          Sound_PlayFar(175);
          break;
        case ITEM_COMMAND_INSPECT:
        case ITEM_COMMAND_GIVE:
        case ITEM_COMMAND_DROP:
          Sound_PlayFar(112);
          break;
        case ITEM_COMMAND_REMOVE:
          Sound_PlayFar(117);
          break;
        default:
          Sound_PlayFar(112);
          break;
        }
        menu->listMode = commandIndex;
        break;
      }
    }
    if ((gInput.pressed & 2) != 0) {
      Sound_PlayFar(113);
      commandIndex = -1;
      menu->listMode = commandIndex;
      break;
    }

    if ((gInput.repeat & 64) != 0) {
      row -= 1;
      redraw = 1;
      Sound_PlayFar(111);
    } else if ((gInput.repeat & 128) != 0) {
      row += 1;
      redraw = 1;
      Sound_PlayFar(111);
    } else if ((gInput.repeat & 16) != 0) {
      column += 1;
      redraw = 1;
      Sound_PlayFar(111);
    } else if ((gInput.repeat & 32) != 0) {
      column -= 1;
      redraw = 1;
      Sound_PlayFar(111);
    }
  }

  menu->equipPreview = 0;
  Func_080fac58();
  return commandIndex;
}

#undef COMMAND_CURSOR_Y
#undef COMMAND_CURSOR_Y

// The item menu work block, as this function reads it.

#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1

s32 Item_ClassifyUseMode(s32 owner, s32 item);
s32 BattleEffect_HasTriggerFar(s32 item);

void Func_080fb410(s8 *commandStates) {
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;

  menu = Ram_HeapSlots->menuRuntime;
  item = Item_Get(ITEM_ID_MASK & menu->selectedItems[0]);

  if (item->type == 0) {
    commandStates[0] = COMMAND_AVAILABLE;
    commandStates[1] = COMMAND_DISABLED;
  } else {
    commandStates[0] = COMMAND_DISABLED;
    commandStates[1] = COMMAND_AVAILABLE;
  }

  if (Item_ClassifyUseMode(menu->paneOwner[0], menu->selectedItems[0]) != -1)
    commandStates[0] = COMMAND_AVAILABLE;
  else
    commandStates[0] = COMMAND_DISABLED;

  if (menu->selectedItems[0] & INVENTORY_BROKEN)
    commandStates[0] = COMMAND_DISABLED;

  if (Item_CanOwnerEquip(menu->paneOwner[0], menu->selectedItems[0] & ITEM_ID_MASK) ==
      0) {
    commandStates[1] = COMMAND_DISABLED;
  }

  commandStates[3] = COMMAND_AVAILABLE;
  commandStates[5] = COMMAND_AVAILABLE;
  commandStates[2] = COMMAND_AVAILABLE;

  if (menu->selectedItems[0] & INVENTORY_EQUIPPED) {
    commandStates[4] = COMMAND_AVAILABLE;
    commandStates[1] = COMMAND_DISABLED;
  } else {
    commandStates[4] = COMMAND_DISABLED;
  }

  if (item->flags & ITEM_CANNOT_UNEQUIP) {
    commandStates[4] = COMMAND_DISABLED;
    /* FAKEMATCH(dead): the equipped test in a once-only block, which
     * keeps the store above from sinking below the selected item's address;
     * as the plain if, 2 of 157 instructions differ in tla-en. */
    do {
      if (menu->selectedItems[0] & INVENTORY_EQUIPPED) {
        commandStates[3] = COMMAND_DISABLED;
        commandStates[5] = COMMAND_DISABLED;
      }
    } while (0);
  }

  if (BattleEffect_HasTriggerFar(menu->selectedItems[0] & ITEM_ID_MASK) != 0)
    commandStates[0] = COMMAND_AVAILABLE;

  if (menu->partyCount <= 1)
    commandStates[3] = COMMAND_DISABLED;

  if (item->flags & ITEM_INDISPENSABLE)
    commandStates[5] = COMMAND_DISABLED;
}

#undef COMMAND_AVAILABLE

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

/// An item command's state when the selected item refuses it.
#define ITEM_COMMAND_STATE_DISABLED (-1)

#define TEXT_COLOR_NORMAL 15   ///< the window text's default colour
#define TEXT_COLOR_DISABLED 14 ///< the grey of a command that refuses A

// The x of the command grid's middle column: German, Spanish and French
// draw Equip and Remove 8 pixels further left.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH)
#define ITEM_COMMAND_TEXT_X 32
#else
#define ITEM_COMMAND_TEXT_X 40
#endif

/// Writes the six command names into window, the disabled ones in grey.
void ItemMenu_DrawCommands(const s8 *commandStates, s32 window) {
  s32 disabled;
  s32 useState;
  u32 firstCommandMessage;

  Text_SetColorFar(TEXT_COLOR_NORMAL);
  useState = commandStates[ITEM_COMMAND_USE];
  disabled = ITEM_COMMAND_STATE_DISABLED;
  if (useState == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);

  firstCommandMessage = (u32)MsgItemCommandUse;
  // "Use"
  Text_DrawCharacterAtOffsetFar(firstCommandMessage, window, 0, 24);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_EQUIP] == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  // "Equip"
  Text_DrawCharacterAtOffsetFar(firstCommandMessage + 1, window,
                                  ITEM_COMMAND_TEXT_X, 24);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_GIVE] == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  // "Give"
  Text_DrawCharacterAtOffsetFar(firstCommandMessage + 2, window, 0, 32);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_DROP] == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  // "Drop"
  Text_DrawCharacterAtOffsetFar(firstCommandMessage + 3, window, 80, 32);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_INSPECT] == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  // "Details"
  Text_DrawCharacterAtOffsetFar(firstCommandMessage + 4, window, 80, 24);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_REMOVE] == disabled)
    Text_SetColorFar(TEXT_COLOR_DISABLED);
  // "Remove"
  Text_DrawCharacterAtOffsetFar(firstCommandMessage + 5, window,
                                  ITEM_COMMAND_TEXT_X, 32);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
}

s32 Item_ClassifyUseMode(s32 owner, s32 itemId) {
  s32 masked = itemId;
  void *itemData;
  s32 result = -1;

  masked &= 0x1ff;
  itemData = Item_Get(masked);

  if (BattleEffect_HasTriggerFar(masked) != 0) {
    return 0;
  }

  {
    u32 use = FIELD_AT_OFFSET(itemData, u16, 40);
    void *abilityData;
    u32 id;

    id = 0x3fff;
    id &= use;
    abilityData = BattleAction_Get(id);

    if (FIELD_AT_OFFSET(itemData, u16, 40) != 0) {
      if (FIELD_AT_OFFSET(itemData, u8, 2) != 0) {
        if (FIELD_AT_OFFSET(itemData, u8, 12) != 3) {
          if (Item_CanOwnerEquip(owner, masked) != 0) {
            result = 1;
          }
        }
      } else {
        result = 1;
      }

      if (result == 1) {
        s32 mask = 0x80;
        u8 field1 = FIELD_AT_OFFSET(abilityData, u8, 1);

        if ((field1 & 0x40) != 0) {
          u8 field8 = FIELD_AT_OFFSET(abilityData, u8, 8);
          result = (field8 == 0xff) ? 2 : 1;
        } else {
          result = (field1 & mask) ? -1 : 0;
        }
      }
    }
  }

  return result;
}

void Inventory_BreakFar(s32 owner, s32 slot);

void Func_080fb6d4(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  u32 item = *(u16 *)(work + 0x16c);
  u32 mask;

  mask = 0x1ff;
  mask &= item;
  if (((u8 *)Item_Get(mask))[12] == 2 && Random16() < 0x2000) {
    Inventory_BreakFar(work[0x216], *(u16 *)(work + 0x168));
    Sound_PlayFar(138);
    InventoryMenu_ShowModalMessage((s32)MsgItBroke, 0, -1);
    WindowSprite_RedrawSavedRectFar(*(void **)(work + 0x30));
  }
}

#undef ITEM_COMMAND_TEXT_X
#undef ITEM_COMMAND_TEXT_X

struct InventoryMenuDetailsState {
  u8 unk000[0x34];
  s32 detailsWindow;
  u8 unk038[0x134];
  u16 itemIds[0x100];
};

void ItemMenu_DrawItemDetails(s32, s32);

s32 ItemMenu_OpenDetail(s32 itemIndex) {
  struct InventoryMenuDetailsState *menu = Ram_HeapSlots->menuRuntime;

  Window_UpdateOrCreate(&menu->detailsWindow, 0, 0, 13, 10, 2);
  Palette_CopyObjectBankToBackground14();

  if (menu->itemIds[itemIndex] != 0)
    ItemMenu_DrawItemDetails(menu->detailsWindow, menu->itemIds[itemIndex]);

  return 1;
}

// The Details window's width in tiles: Japanese opens it over the left part
// only, beside the item window, the other editions over the whole top half.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DETAILS_WINDOW_WIDTH 13
#else
#define DETAILS_WINDOW_WIDTH 30
#endif

struct ItemDetailsMenu {
  u8 unk000[0x10];
  s32 mainWindow;
  u8 unk014[0x15c];
  struct ItemMenuIcon *cursor;
  u8 unk174[0xa4];
  struct ItemMenuIcon *selectedItemIcon;
  u16 unk21c; ///< bit 1 read by Func_080fd968; Japanese's Details sets 1
};

void Window_CloseFar(s32, s32);

/// The item menu's Details screen: describes the item entry value in a window
/// over the top half (DETAILS_WINDOW_WIDTH) and waits for A or B. Up and Down
/// move an unused counter from 0 to 4. Returns the counter after A or Start, or
/// -1 after B. The twin of The Broken Seal's ItemMenu_ShowDetails.
s32 Func_080fb780(s32 value) {
  struct ItemDetailsMenu *menu = Ram_HeapSlots->menuRuntime;
  s32 changed = 1;
#if LANGUAGE != LANGUAGE_JAPANESE
  struct ItemMenuIcon *itemIcon = menu->selectedItemIcon;
#endif
  s32 window;
  s32 counter = 0;

#if LANGUAGE != LANGUAGE_JAPANESE
  itemIcon->state = 13;
#endif
  window = Window_CreateFar(0, 0, DETAILS_WINDOW_WIDTH, 10, 2);
  Task_Remove(Menu_UpdateEntryObjectTransforms);

  {
    struct ItemMenuIcon *cursor = menu->cursor;
    cursor->state = 13;
  }
  Palette_CopyObjectBankToBackground14();
  wait(1);

  for (;;) {
    if (GameFlag_Test(336) != 0)
      break;

    if (changed != 0) {
      changed = 0;
      counter = (counter + 5) % 5;
      ItemMenu_DrawItemDetails(window, value);
    }

    if (gInput.pressed & 1)
      break;
    if (gInput.pressed & 2) {
      counter = -1;
      break;
    }

    if (gInput.repeat & 0x40) {
      counter -= 1;
      changed = 1;
    }
    if (gInput.repeat & 0x80) {
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
  Palette_LightenHighlight(14);
  {
    s32 priority = 0x480;

    Task_Add(Menu_UpdateEntryObjectTransforms, priority);
  }

  {
    struct ItemMenuIcon *cursor = menu->cursor;
    cursor->state = 1;
  }
#if LANGUAGE == LANGUAGE_JAPANESE
  menu->unk21c = 1;
#else
  Window_DrawBorderFar(13, 0, 17, 10);
#endif

  return counter;
}

#undef DETAILS_WINDOW_WIDTH
#undef DETAILS_WINDOW_WIDTH

void Func_080fb8b8(s32 a, s32 b, s32 c);

void ItemMenu_DrawItemDetails(s32 a, s32 b) { Func_080fb8b8(a, b, -1); }

// The headings are indented in English and Japanese; German, Spanish and
// Italian start all three at the window's left edge, French the uses heading
// only.
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

// Japanese draws the values nearer the names, and a multiplier further left to
// leave room for its suffix.
#if LANGUAGE == LANGUAGE_JAPANESE
#define ITEM_DETAILS_VALUE_X 64
#define ITEM_DETAILS_MULTIPLIER_X 48
#else
#define ITEM_DETAILS_VALUE_X 72
#define ITEM_DETAILS_MULTIPLIER_X 72
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
/// "ばい", times, after a multiplier.
extern u8 MsgTimesSuffix[];
#endif
extern u8 gMenu_PointString[];

void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y);

/// Describes an item (an inventory entry) in window, one 8-pixel row a line:
/// its equip bonuses and effects, a curse, the Psynergy it bestows, its uses
/// and its count, or failing all of these whether it is rare or important.
/// count is the count to show, which only Japanese reads;
/// ItemMenu_DrawItemDetails passes -1, the entry's own. The twin of The Broken
/// Seal's ItemMenu_DrawItemDetails.
void Func_080fb8b8(s32 window, s32 item, s32 count) {
  struct ItemDefinition *def;
  s32 i;
  s32 found;
  s32 amount;
  s32 consumable;
  s8 row;

  row = 0;
  consumable = 0;
  def = Item_Get(item & 0x1ff);
  if (def->type != 0) {
    found = 0;
    if (def->primaryBonus == 0 && def->secondaryBonus == 0) {
      for (i = 0; i < 4; i++) {
        if (def->effects[i].kind != 0 || def->useType == 3) {
          found = 1;
          break;
        }
      }
    } else {
      found = 1;
    }
    if (found == 1) {
      Text_DrawCharacterAtOffsetFar((s32)MsgEquipEffectHeading, window,
                                      ITEM_DETAILS_HEADING_X, 0);
      row = 1;
    }
    if (def->primaryBonus != 0) {
      Text_DrawCharacterAtOffsetFar((s32)MsgStatLabel, window, 0, row * 8);
      amount = def->primaryBonus;
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      row++;
    }
    if (def->secondaryBonus != 0) {
      Text_DrawCharacterAtOffsetFar((s32)MsgDefenseLabel, window, 0, row * 8);
      amount = def->secondaryBonus;
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      row++;
    }
  }
  for (i = 0; i < 4; i++) {
    if (def->effects[i].kind == 0)
      continue;
    amount = def->effects[i].amount;
    switch (def->effects[i].kind) {
    case 0:
      break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 26:
      Text_DrawCharacterAtOffsetFar(
          def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      break;
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
      Window_SetTilemapEntryFar(
          window, (u8)((def->effects[i].kind - 15) % 4) + 1, 0, row, 2);
      Text_DrawCharacterAtOffsetFar(
          def->effects[i].kind + (s32)MsgEquipEffectName, window, 8, row * 8);
      ItemMenu_DrawStat(amount, 3, window, ITEM_DETAILS_VALUE_X, row * 8);
      break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
      Text_DrawCharacterAtOffsetFar(
          def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
      if (amount > 9) {
        Text_DrawNumberInWindowFar(1, 1, window, ITEM_DETAILS_MULTIPLIER_X,
                                     row * 8);
        Text_DrawStringInWindowFar(gMenu_PointString, window,
                                     ITEM_DETAILS_MULTIPLIER_X + 8, row * 8);
        Text_DrawNumberInWindowFar(amount - 10, 1, window,
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
      Text_DrawCharacterAtOffsetFar((s32)MsgTimesSuffix, window,
                                      ITEM_DETAILS_MULTIPLIER_X + 24, row * 8);
#endif
      break;
    case 23:
    case 25:
    case 27:
      Text_DrawCharacterAtOffsetFar(
          def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
#if LANGUAGE == LANGUAGE_GERMAN
      // German continues these three names on a second line.
      row++;
      if (def->effects[i].kind == 23)
        Text_DrawCharacterAtOffsetFar((s32)MsgEquipEffectName + 28, window, 0,
                                        row * 8);
      else if (def->effects[i].kind == 25)
        Text_DrawCharacterAtOffsetFar((s32)MsgEquipEffectName + 29, window, 0,
                                        row * 8);
      else if (def->effects[i].kind == 27)
        Text_DrawCharacterAtOffsetFar((s32)MsgEquipEffectName + 30, window, 0,
                                        row * 8);
#endif
      break;
    }
    row++;
  }
  if (def->flags & 1) {
    Text_DrawCharacterAtOffsetFar((s32)MsgItemCursed, window, 0, row * 8);
    row++;
  }
  if (def->useType == 3) {
    Text_DrawCharacterAtOffsetFar((s32)MsgBestowsPsynergy, window, 0,
                                    row * 8);
    consumable = 1;
    row++;
  }
  if (def->useType != 4 && def->useType != 0) {
    if (!consumable) {
      Text_DrawCharacterAtOffsetFar((s32)MsgUsesHeading, window,
                                      ITEM_DETAILS_USES_X, row * 8);
      row++;
    }
    switch (def->useType) {
    case 0:
      break;
    case 1:
      Text_DrawCharacterAtOffsetFar((s32)MsgSingleUse, window, 0, row * 8);
      row++;
      break;
    case 2: {
      s32 m;

      if (item & 0x400) {
        m = (s32)MsgBrokenNotice;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#if LANGUAGE == LANGUAGE_GERMAN
        // German ends the sentence on a third line, six messages on.
        Text_DrawCharacterAtOffsetFar(m + 1, window, 0, row * 8);
        row++;
        m += 6;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#else
        m++;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#endif
      } else {
        m = (s32)MsgMightBreak;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#if LANGUAGE == LANGUAGE_GERMAN
        Text_DrawCharacterAtOffsetFar(m + 1, window, 0, row * 8);
        row++;
        m += 7;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#else
        m++;
        Text_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
#endif
      }
      break;
    }
    }
  }
  if (def->flags & 16) {
    if (row != 0)
      row++;
    i = (s32)MsgNumberHeading;
    Text_DrawCharacterAtOffsetFar(i, window, ITEM_DETAILS_NUMBER_X, row * 8);
    row++;
#if LANGUAGE == LANGUAGE_JAPANESE
    // Japanese draws the count itself: the entry's own for -1, "none" for 0.
    if (count == -1) {
      amount = (item & 0xf800) / 2048;
      Text_DrawNumberInWindowFar(amount + 1, 2, window, 0, row * 8);
      Text_DrawCharacterAtOffsetFar(i + 1, window, 16, row * 8);
      row++;
    } else if (count == 0) {
      Text_DrawCharacterAtOffsetFar(i + 8, window, 0, row * 8);
      row++;
    } else {
      amount = count;
      Text_DrawNumberInWindowFar(amount, 3, window, 0, row * 8);
      Text_DrawCharacterAtOffsetFar(i + 1, window, 24, row * 8);
      row++;
    }
#else
    amount = (item & 0xf800) / 2048;
    MessageArgument_PushFar(amount + 1, 5);
    Text_DrawCharacterAtOffsetFar(i + 1, window, 0, row * 8);
    row++;
#endif
  }
  found = 0;
  if (row == 0) {
    if (def->flags & 4) {
      Text_DrawCharacterAtOffsetFar((s32)MsgRareItem, window, 0, 0);
      found = 1;
    }
    if (!found) {
      if (def->flags & 8) {
        Text_DrawCharacterAtOffsetFar((s32)MsgImportantItem, window, 0, 0);
        found = 1;
      }
      if (!found)
        Text_DrawCharacterAtOffsetFar((s32)MsgDetailsUnknown, window, 0, 0);
    }
  }
}

#undef ITEM_DETAILS_MULTIPLIER_X
#undef ITEM_DETAILS_MULTIPLIER_X

#define Text_DrawStringInWindowFar                                           \
  ((void (*)(u8 *, s32, s32, s32))Text_DrawStringInWindowFar)

void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y) {
  s32 digits;
  s32 magnitude;

  Text_DrawNumberInWindowFar(delta, 3, window, x, y);
  digits = 1;
  magnitude = delta;
  if (delta < 0) {
    magnitude = -delta;
  }
  if (magnitude > 9) {
    digits = 2;
  }
  magnitude = delta;
  if (delta < 0) {
    magnitude = -delta;
  }
  if (magnitude > 99) {
    digits = 3;
  }
  if (delta > 0) {
    Text_DrawStringInWindowFar(&gMenu_PlusSignString, window,
                                 x - digits * 8 + 16, y);
  } else {
    Text_DrawStringInWindowFar(&gMenu_MinusSignString, window,
                                 x - digits * 8 + 16, y);
  }
}

struct ItemMenuWindows {
  u8 unk00[0x24];
  struct RenderInput *itemWindow;
  u8 unk28[0xc8];
  struct RenderInput *messageWindow;
};

#define ITEM_MENU ((struct ItemMenuWindows *)Ram_HeapSlots->menuRuntime)

void ItemMenu_SetItemWindowRow5(void) {
  Window_SetBounds(ITEM_MENU->itemWindow, 13, 5, 17, 10);
}

void ItemMenu_SetItemWindowRow3(void) {
  Window_SetBounds(ITEM_MENU->itemWindow, 13, 3, 17, 10);
}

void ItemMenu_SetMessageWindowHeight5(void) {
  Window_SetBounds(ITEM_MENU->messageWindow, 13, 0, 17, 5);
}

void ItemMenu_SetMessageWindowHeight6(void) {
  Window_SetBounds(ITEM_MENU->messageWindow, 13, 0, 17, 6);
}

void ItemMenu_SetMessageWindowHeight7(void) {
  Window_SetBounds(ITEM_MENU->messageWindow, 13, 0, 17, 7);
}

void ItemMenu_SetMessageWindowHeight3(void) {
  Window_SetBounds(ITEM_MENU->messageWindow, 13, 0, 17, 3);
}

#undef Text_DrawStringInWindowFar
#undef ITEM_MENU

// The Japanese window asks いくつ? ("How many?") on the count's row instead of
// the top row, draws the counter こ after each count held, and keeps its cursor
// further left.
#if LANGUAGE == LANGUAGE_JAPANESE
// いくつ?, the message three before MsgShortcutHelp.
#define MSG_HOW_MANY ((s32)MsgShortcutHelp - 3)
#define HOW_MANY_X 0
#define HOW_MANY_Y 32
// こ, the counter after a count: the message before MsgItemCommandUse.
#define MSG_PIECE ((s32)MsgItemCommandUse - 1)
#define CURSOR_X 88
#else
#define MSG_HOW_MANY ((s32)MsgHowMany)
#define HOW_MANY_X 32
#define HOW_MANY_Y 0
#define CURSOR_X 128
#endif

struct QuantityIcon {
  u8 unk00[5];
  u8 state;
  u8 unk06[0x12];
  u16 tile : 10;
  u16 attr2High : 6;
};

struct QuantityMenu {
  u8 unk000[0xf0];
  void *messageWindow;
  u8 unk0f4[0x78];
  u16 selectedItems[2];
  u8 unk170[0xa6];
  u8 paneOwner[2];
  struct QuantityIcon *selectedItemIcon;
};

#if LANGUAGE == LANGUAGE_JAPANESE
extern u8 MsgItemCommandUse[];
#else
// "How many?"
extern u8 MsgHowMany[];
#endif
extern const u8 gMenu_QuantityTiles[];

void Shop_FillSelectorFar(s32 value, s32 column, void *tiles);

s32 Func_080fbe6c(s32 base, s32 range, s32 mode) {
  s32 value = base;
  struct QuantityMenu *menu = Ram_HeapSlots->menuRuntime;
  u8 *tiles = HeapSlot_AllocEwram(56, 0x400);
  s32 entry;
  s32 changed = 1;
  s32 targetCount = 0;
  void *window = menu->messageWindow;
  s32 ownerCount;
  struct QuantityIcon *arrow;

  ItemMenu_SetMessageWindowHeight7();
  WindowSprite_RedrawSavedRectFar(window);
  if (mode == 0)
    targetCount = InventoryMenu_GetItemQuantity(menu->paneOwner[1],
                                                menu->selectedItems[0] & 0x1ff);
  ownerCount = InventoryMenu_GetItemQuantity(menu->paneOwner[0],
                                             menu->selectedItems[0] & 0x1ff);
  entry = Resource_FindFreeEntry();
  if (entry != 96) {
    VramCache_Load(entry, 0x100, 0);
    WindowSprite_CreateFar(entry, 0x40004000, window, 48, 32);
    arrow = WindowSprite_CreateFar(entry, 0x40004000, window, 80, 32);
    arrow->tile = arrow->tile + 4;
    Func_080f8ab4(CURSOR_X, 40);
    while (!GameFlag_Test(0x150)) {
      if (changed) {
        changed = 0;
        value = (value + range) % range;
        WindowSprite_RedrawSavedRectFar(window);
        Text_DrawCharacterAtOffsetFar(MSG_HOW_MANY, window, HOW_MANY_X,
                                      HOW_MANY_Y);
        Dma_Set(gMenu_QuantityTiles, tiles, (DMA_ENABLE | DMA_32BIT) << 16 | 64,
                REG_DMA3);
        Shop_FillSelectorFar(30, 14, tiles);
        Shop_FillSelectorFar(base + range, 0, tiles);
        Shop_FillSelectorFar(base + value + 1, 10, tiles);
        Shop_FillSelectorFar(base, 2, tiles);
        VramCache_Load(entry, 0x100, tiles);
        Text_DrawNumberInWindowFar(value + 1, 2, window, 32, 32);
        Text_DrawCharacterAtOffsetFar(
            (menu->selectedItems[0] & 0x1ff) + (s32)MsgItemName, window, 16, 8);
        Text_DrawNumberInWindowFar(ownerCount - value - 1, 2, window, 16, 24);
#if LANGUAGE == LANGUAGE_JAPANESE
        Text_DrawCharacterAtOffsetFar(MSG_PIECE, window, 32, 24);
#endif
        if (mode == 0) {
          Text_DrawNumberInWindowFar(targetCount + value + 1, 2, window, 80,
                                     24);
#if LANGUAGE == LANGUAGE_JAPANESE
          Text_DrawCharacterAtOffsetFar(MSG_PIECE, window, 96, 24);
#endif
        }
        Text_DrawStringAtOffsetFar(Unit_Get(menu->paneOwner[0]), window, 16,
                                   16);
        if (mode == 0)
          Text_DrawStringAtOffsetFar(Unit_Get(menu->paneOwner[1]), window, 80,
                                     16);
      }
      if (gInput.pressed & 1) {
        Sound_PlayFar(112);
        break;
      }
      if (gInput.pressed & 2) {
        value = -1;
        Sound_PlayFar(113);
        break;
      }
      Func_080f8a44(CURSOR_X, 40);
      if (gInput.repeat & 32) {
        value--;
        changed = 1;
        Sound_PlayFar(111);
      }
      if (gInput.repeat & 16) {
        value++;
        changed = 1;
        Sound_PlayFar(111);
      }
      wait(1);
    }
  }
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(window);
  HeapSlot_Release(56);
  menu->selectedItemIcon->state = 13;
  if (GameFlag_Test(0x150))
    value = -1;
  return value;
}

#undef HOW_MANY_Y
#undef CURSOR_X
#undef HOW_MANY_Y
#undef CURSOR_X

// Draws the selected item's icon, owner and item name in the heading.
void Func_080fc12c(void) {
  struct InventoryMenuState *menu = Ram_HeapSlots->menuRuntime;

  Func_08038288(2, menu->selectedItems[0], (u8)menu->selectedItemIcon->index,
                0);
  menu->selectedItemIcon->active = 1;
  menu->selectedItemIcon->x = 112;
  menu->selectedItemIcon->y = 8;
  Icon_PrepareActor(menu->selectedItemIcon);
  Text_DrawStringAtOffsetFar(Unit_Get(menu->paneOwner[0]), menu->messageWindow,
                               16, 0);
  Text_DrawCharacterAtOffsetFar((menu->selectedItems[0] & 0x1ff) +
                                      (s32)MsgItemName,
                                  menu->messageWindow, 16, 8);
}

// The French question starts at the window's left edge.
#if LANGUAGE == LANGUAGE_FRENCH
#define ITEM_DROP_QUESTION_X 0
#else
#define ITEM_DROP_QUESTION_X 8
#endif

/// Asks whether to drop item (an inventory entry): its name, the question, and
/// Yes or No, No first. Returns 0 for Yes and 1 for No, B or Start. The twin of
/// The Broken Seal's ItemMenu_ConfirmDrop.
s32 Func_080fc1ac(s32 item) {
  s32 window;
  s32 itemId;
  s32 question;
  s32 answer;
  s32 rows = 2;
  s32 selection = 1;
  s32 changed = 1;

  window = Window_CreateFar(13, 3, 17, 10, 2);
  itemId = item & 0x1ff;
  Item_Get(itemId);
  Text_DrawCharacterAtOffsetFar(itemId + (s32)MsgItemName, window, 24, 0);
  question = (s32)MsgConfirmDrop;
  Text_DrawCharacterAtOffsetFar(question, window, ITEM_DROP_QUESTION_X, 16);
  question++;
  Text_DrawCharacterAtOffsetFar(question, window, ITEM_DROP_QUESTION_X, 24);
  answer = (s32)MsgYes;
  Text_DrawCharacterAtOffsetFar(answer, window, 24, 40);
  answer++;
  Text_DrawCharacterAtOffsetFar(answer, window, 24, 56);
  Func_080f8ab4(104, 86);
  for (;;) {
    if (GameFlag_Test(336))
      break;
    if (changed) {
      changed = 0;
      selection = (selection + rows) % rows;
    }
    if (gInput.pressed & 1) {
      Sound_PlayFar(112);
      break;
    }
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      selection = 1;
      break;
    }
    Func_080f8a44(104, (selection << 4) + 70);
    if (gInput.repeat & 0x40) {
      selection--;
      changed = 1;
      Sound_PlayFar(111);
    }
    if (gInput.repeat & 0x80) {
      selection++;
      changed = 1;
      Sound_PlayFar(111);
    }
    wait(1);
  }
  if (GameFlag_Test(336)) {
    selection = 1;
  }
  Window_CloseFar(window, 1);
  return selection;
}

#undef ITEM_DROP_QUESTION_X
#undef ITEM_DROP_QUESTION_X

// The question and its answers, as each translation lays them out. English and
// Japanese stack Yes over No to the right of the question, and Up and Down move
// the cursor; German, Spanish, French and Italian set them side by side below
// it, and Left and Right move the cursor. Japanese indents the question.
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

// The question is named in the English catalog alone; in every edition it is
// the second message after MsgConfirmDrop.
#if LANGUAGE == LANGUAGE_ENGLISH
extern u8 MsgEquipThisItem[]; // the ☀️ twin's name
#define EQUIP_QUESTION ((s32)MsgEquipThisItem)
#else
extern u8 MsgConfirmDrop[];
#define EQUIP_QUESTION ((s32)MsgConfirmDrop + 2)
#endif

extern u8 MsgYes[]; // the ☀️ twin's name

s32 Func_080fc2e0(s32 unused) {
  s32 rows = 2;
  s32 selection = 0;
  s32 changed = 1;
  struct InventoryMenuState *menu =
      (struct InventoryMenuState *)Ram_HeapSlots->menuRuntime;
  void *unit = Unit_Get(menu->paneOwner[1]);
  void *saved;
  s32 window;
  s32 slot;
  s32 member;
  s32 size;

  slot = menu->selectedSlots[1];
  member = menu->paneOwner[1];
  Func_080fae8c(member, slot, 0, member);
  /* FAKEMATCH(temporary): the unit size held in size for the allocation and
   * both copies; as 332 at each, 6 of 179 instructions differ in tla-en. */
  size = 332;
  saved = Heap_Alloc(size);
  Iwram_CopyWords(saved, unit, size);
  window = menu->messageWindow;
  if ((u32)(Inventory_EquipFar(menu->paneOwner[1], menu->selectedSlots[1]) +
            2) <= 1) {
    selection = 1;
  } else {
    Text_DrawCharacterAtOffsetFar((s32)MsgYes, window, EQUIP_YES_X,
                                  EQUIP_YES_Y);
    Text_DrawCharacterAtOffsetFar((s32)MsgYes + 1, window, EQUIP_NO_X,
                                  EQUIP_NO_Y);
    Window_ClearInteriorTilesFar(window, 16, 16, 96, 24);
    Text_DrawCharacterAtOffsetFar(EQUIP_QUESTION, window, EQUIP_QUESTION_X, 16);
    Func_080f8ab4(EQUIP_CURSOR_X(0), EQUIP_CURSOR_Y(0));
    for (;;) {
      if (GameFlag_Test(336))
        break;
      if (changed) {
        changed = 0;
        selection = (selection + rows) % rows;
      }
      if (gInput.pressed & KEY_A) {
        Sound_PlayFar(175);
        break;
      }
      if (gInput.pressed & KEY_B) {
        Sound_PlayFar(113);
        selection = 1;
        break;
      }
      Func_080f8a44(EQUIP_CURSOR_X(selection), EQUIP_CURSOR_Y(selection));
      if (gInput.repeat & EQUIP_PREVIOUS_KEY) {
        selection--;
        changed = 1;
        Sound_PlayFar(111);
      }
      if (gInput.repeat & EQUIP_NEXT_KEY) {
        selection++;
        changed = 1;
        Sound_PlayFar(111);
      }
      wait(1);
    }
  }
  if (GameFlag_Test(336))
    selection = 1;
  if (selection == 1)
    Iwram_CopyWords(unit, saved, size);
  Heap_Release(saved);
  Unit_RefreshPsynergyFar(menu->paneOwner[1]);
  Unit_CalculateFar(menu->paneOwner[1]);
  return selection;
}

#undef EQUIP_QUESTION
#undef EQUIP_QUESTION

/// Item menu workspace (known fields only).
extern char ResourceId_MenuArrowTiles;
extern char ResourceId_MenuSmallDownArrowTiles;

/// Loads the two page arrows into free tile slots.
void Resource_LoadPairedBlocks(void) {
  struct InventoryMenuState *state = Ram_HeapSlots->menuRuntime;
  s32 value = Resource_FindFreeEntry();

  state->resourceSlots[0] = value;
  VramBlock_LoadResourceFar(value, 128, (s32)&ResourceId_MenuArrowTiles);
  value = Resource_FindFreeEntry();
  state->resourceSlots[1] = value;
  VramBlock_LoadResourceFar(value, 128,
                            (s32)&ResourceId_MenuSmallDownArrowTiles);
}

/// Splits the selected slot owner's items into pages of five rows.
s32 ItemMenu_PageResult(struct MenuResult *result, s32 index) {
  s32 encoded;
  struct InventoryMenuState *menu = Ram_HeapSlots->menuRuntime;
  s32 limit;
  s32 remainder;
  s32 quotient;
  s32 groups;
  s32 value;

  limit = ItemMenu_CountItems(menu->paneOwner[index]);
  encoded = Unit_Get(menu->paneOwner[index]);
  value = menu->selectedIndexByOwner[menu->paneOwner[index]];
  if ((value + 1) > limit) {
    value = limit - 1;
  }
  quotient = value / 5;
  remainder = value % 5;
  groups = limit / 5;
  if (limit % 5 != 0) {
    groups++;
  }
  result->ownerState = encoded;
  result->page = quotient;
  result->pageCount = groups;
  result->row = remainder;
  result->entryCount = limit;
  result->selectedIndex = value;
  return 1;
}

/// Draws the selected item-list row's name and highlights only that row; the
/// twin of The Broken Seal's ItemMenu_DrawItemDetailPage.
s32 Func_080fc558(s32 arg0, void *arg1, struct MenuResult *state) {
  struct InventoryMenuState *menu;
  s32 page;
  s32 combined;
  s32 row;
  s32 window;

  page = state->page;
  menu = Ram_HeapSlots->menuRuntime;
  window = menu->infoWindow;
  combined = page * 5;
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
    s32 masked = (menu->items[combined] & 0x1ff) + (s32)MsgItemPlainName;

    // Japanese draws the name as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
    Text_DrawMessageFar(masked, menu->infoWindow, 0, 0);
#else
    Text_DrawCharacterAtOffsetFar(masked, menu->infoWindow, 0, 0);
#endif
  }

  row = 0;
  do {
    if (row == state->row) {
      Render_SetTilemapFlagRect(menu->itemWindow, 1, row * 2 + 1, 14, 1, 14);
    } else {
      Render_SetTilemapFlagRect(menu->itemWindow, 1, row * 2 + 1, 14, 1, 15);
    }
    row++;
  } while (row <= 4);

  wait(1);
  return 1;
}

// The Japanese page draws its page icons and names further right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_X 120
#define ENTRY_X 32
#else
#define PAGE_X 116
#define ENTRY_X 24
#endif

/// Draws the names on one five-row page of the item list: The Broken Seal's
/// ItemMenu_DrawNamePage over this game's menu offsets.
s32 Func_080fc608(s32 window, s32 unused, const struct MenuResult *state) {
  struct InventoryMenuState *menu = Ram_HeapSlots->menuRuntime;
  u32 page;
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  const u16 *itemId;

  WindowSprite_RedrawSavedRectFar(window);
  Window_DrawDividerLineFar(window, 0, 11, 16, 11);

  page = state->page;
  firstEntry = page * 5;
  visibleCount = (u8)(state->entryCount - firstEntry);
  if (visibleCount > 5) {
    visibleCount = 5;
  }

  Menu_SetPageIcons(5, firstEntry, window, PAGE_X, 34);
  Menu_DrawPageIndicator(window, state->entryCount, 5, state->page, 15);

  row = 0;
  if (visibleCount > row) {
    itemId = &menu->items[firstEntry];
    do {
      Text_DrawCharacterAtOffsetFar((itemId[0] & 0x1ff) + (s32)MsgItemName,
                                      menu->itemWindow, ENTRY_X, row * 16 + 8);
      row++;
      itemId++;
    } while (visibleCount > row);
  }

  return 1;
}

#undef PAGE_X
#undef ENTRY_X
#undef PAGE_X
#undef ENTRY_X

// The Japanese list puts its help line at another column.
#if LANGUAGE == LANGUAGE_JAPANESE
#define CHANGE_HELP_X 16
#else
#define CHANGE_HELP_X 0
#endif

struct ListIcon {
  u8 unk00[5];
  u8 state;
  u8 unk06[6];
  u16 animStep;
  u8 unk0e;
  u8 sentinel;
};

struct ListMenu {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  void *pageWindow;
  u8 unk014[8];
  s8 paneIndex[2];
  u8 unk01e[0x1a];
  void *listWindow;
  u8 unk03c[0xc];
  struct ListIcon *gridCursor;
  struct ListIcon *entryIcons[32];
  u8 unk0cc[0x2c];
  void *ownerObjects[8];
  u8 unk118[0x40];
  s16 partySlots[4];
  u8 unk160[8];
  u16 selectedSlots[2];
  u16 selectedItems[2];
  u8 unk170[8];
  struct ListIcon *ownerIcons[2];
  u8 unk180[0x44];
  u16 items[32];
  u16 ownerIds[8];
  u8 itemCount;
  u8 partyCount;
  u8 paneOwner[2];
  struct ListIcon *selectedItemIcon;
  u8 unk21c[0x44];
  u8 equipPreview;
  u8 unk261[3];
  u8 selectedIndexByOwner[8];
};

struct ListOwner {
  u8 unk000[0xd8];
  u16 inventory[15];
};

// "L/R: Change character", under the list in pane 0.
extern u8 MsgChangeCharacterHelp[];

s32 Func_080fc6bc(s32 pane) {
  struct ListMenu *menu;
  void *window;
  s32 changed;
  s32 moved;
  s32 member;
  s32 count;
  u8 i;
  s32 result;
  s32 redraw;
  s32 done;
  struct ListOwner *owner;
  s32 previous;
  struct ListIcon *icon;
  s32 work[5];
  struct MenuResult state;

  menu = Ram_HeapSlots->menuRuntime;
  result = 0;
  previous = 0;
  menu->equipPreview = result;
  Window_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, 2);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && GameFlag_Test(0x150) == 0) {
    owner = Unit_Get(menu->paneOwner[pane]);
    menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
    ItemMenu_DrawIcons(menu->items, 0);
    menu->selectedItemIcon->state = 13;
    ItemMenu_PageResult(&state, pane);
    Func_080f8a44(98, state.selectedIndex * 16 + 36);
    changed = 1;
    redraw = 1;

    while (GameFlag_Test(0x150) == 0) {
      Func_080f8a44(98, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (owner->inventory[previous] != 0) {
          icon = menu->entryIcons[previous];
          Icon_PrepareActor(icon);
        }
        if (redraw != 0) {
          wait(1);
          Func_080fc608(window, 0, &state);
          if (pane == 0)
            Text_DrawCharacterAtOffsetFar((s32)MsgChangeCharacterHelp, window,
                                          CHANGE_HELP_X, 88);
          redraw = 0;
        }
        Func_080fc558(window, work, &state);
        menu->selectedItems[pane] = menu->items[state.selectedIndex];
        Func_080fae8c(menu->paneOwner[pane], state.selectedIndex, 0,
                      menu->paneOwner[pane]);
        if (menu->items[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          icon->state = 9;
          icon->animStep = 0;
          icon->sentinel = 250;
        }
        for (i = 0; i < menu->partyCount; i++)
          Animation_ApplyChildArgumentFar(menu->ownerObjects[i], 1);
      }

      if ((gFrameCount & 31) == 0) {
        for (i = 0; i <= 3; i++) {
          s32 slot = menu->partySlots[i];
          if (slot != -1 &&
              Item_CanOwnerEquip(slot,
                                 0x1ff & menu->items[state.selectedIndex]) != 0)
            Animation_ApplyChildArgumentFar(menu->ownerObjects[i], 3);
        }
      }

      wait(1);
      previous = state.selectedIndex;
      moved = PagedList_HandleInput(0, state.entryCount, 5, &state.row,
                                    &state.page);
      if (moved == 1) {
        redraw = 1;
        changed = 1;
      }
      if (moved == 0)
        changed = 1;
      if (moved == -1)
        changed = 0;

      if ((gInput.pressed & 1) != 0 && menu->items[state.selectedIndex] != 0) {
        Sound_PlayFar(173);
        result = menu->items[state.selectedIndex];
        done = 1;
        break;
      }
      if ((gInput.pressed & 2) != 0) {
        Sound_PlayFar(113);
        result = -1;
        done = 1;
        break;
      }
      if ((gInput.repeat & 0x100) != 0 || (gInput.repeat & 0x200) != 0) {
        if (pane == 1) {
          Sound_PlayFar(114);
          wait(1);
        } else {
          Sound_PlayFar(111);
          menu->selectedIndexByOwner[menu->paneOwner[pane]] =
              state.selectedIndex;
          member = menu->paneIndex[pane];
          count = menu->partyCount;
          do {
            if ((gInput.repeat & 0x100) != 0)
              member++;
            else
              member--;
            member = (member + count) % count;
            menu->selectedOwner = menu->ownerIds[member];
            menu->paneOwner[pane] = menu->ownerIds[member];
            menu->paneIndex[pane] = member;
            menu->itemCount = ItemMenu_CollectItems(
                Unit_Get(menu->paneOwner[pane]), menu->items, 0);
          } while (menu->itemCount == 0);
          Menu_CreateOwnerEntryObjects(member / 4 * 4);
          Menu_HighlightPageEntry(menu->pageWindow, member, count);
          menu->ownerIcons[0]->state = 13;
          menu->ownerIcons[1]->state = 13;
          wait(1);
          break;
        }
      }
    }
  }

  Window_ClearInteriorTilesFar(window, 0, 88, 120, 96);
  Icon_PrepareActor(menu->gridCursor);
  menu->selectedSlots[pane] = state.selectedIndex;
  menu->selectedIndexByOwner[menu->paneOwner[pane]] = state.selectedIndex;
  menu->selectedItems[pane] = result;
  if (GameFlag_Test(0x150) != 0)
    result = -1;
  wait(1);
  return result;
}

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) ||          \
    (LANGUAGE == LANGUAGE_SPANISH)
extern s32 gItemMenu_CommandColumnXTable[6];

/// The cursor's x at the item command in column and row, from the table; 0
/// outside three columns and three rows.
s32 Func_08106b80(s32 column, s32 row) {
  if (column > 2 || row > 2 || column < 0 || row < 0)
    return 0;
  return gItemMenu_CommandColumnXTable[row * 3 + column];
}

/// The cursor's y at the item command in row: 30 in the first row, 38 below.
s32 Func_08106ba8(s32 column, s32 row) {
  s32 y = 30;

  if (row != 0)
    y = 38;
  return y;
}
#endif

#undef CHANGE_HELP_X
#undef CHANGE_HELP_X

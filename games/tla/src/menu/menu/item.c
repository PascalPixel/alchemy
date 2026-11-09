/// @file
/// The item menu, as far as this object holds it: the field menu's Item
/// screen, its command loop, Use, the first screen's setup and the item icons.
// Part of menu.c.

#include "../menu_internal.h"
#include "irq.h"

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

void Window_DrawBorderFar(s32 x, s32 y, s32 width, s32 height);
void Window_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void Resource_LoadPairedBlocks(void);
void Palette_LightenHighlight(s32 palette);
void Text_UseExtraTilesFar(s32 flag);
void ItemMenu_Close(void);
void Menu_ResetTwoResourceEntries(void);
void Func_08038290(void);
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
      Unit_Get(menu->sideMember[0])->inventory[menu->selectedSlots[0]] &=
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
void Func_08100d40(s32 item);

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
  /* FAKEMATCH(barrier): an empty asm between the item load and the 0x1ff mask;
   * as mask = item & 0x1ff, 2 of 67 instructions differ in tla-en. */
  asm("");
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

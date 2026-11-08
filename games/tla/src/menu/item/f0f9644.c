#include "heap_release.h"
#include "types.h"
#include "iwram_call.h"
#include "item.h"
#include "owner_state.h"
#include "ram_buffer.h"
#include "irq.h"
#include "flag_call.h"

struct ItemMenuIcon {
  struct ItemMenuIcon *next;
  s8 kind;
  s8 state;
  u16 x;
  u16 y;
  u8 unk0a[4];
  u8 renderTarget;
};

struct ItemCommandWork {
  u8 unk000[0x14];
  struct ItemMenuIcon *listIcon;
  u8 unk018[0x10];
  s32 statusWindow;
  u8 unk02c[4];
  s32 infoWindow;
  u8 unk034[4];
  s32 previewWindow;
  u8 unk03c[0xb4];
  s32 messageWindow;
  u8 unk0f4[0x74];
  u16 selectedSlot;
  u16 targetSlot;
  u16 selectedItem;
  u16 targetItem;
  u8 unk170[0x54];
  u16 items[32];
  u8 unk204[0x10];
  u8 itemCount;
  u8 partyCount;
  u8 itemOwner;
  u8 targetOwner;
  struct ItemMenuIcon *selectedItemIcon;
  u8 unk21c[2];
  s16 completionFlag;
  u8 unk220[0x3e];
  s16 messageOffset;
  u8 equipPreview;
  u8 listMode;
};

/// The resident IWRAM word copy, called through its address.
typedef s32 (*ItemWordCopyFn)(void *dst, const void *src, s32 size);

/// Iwram_CopyWords's routine reached from IwramIrqMain rather than at its
/// fixed address, as in The Broken Seal: the trade's snapshot copies load the
/// linked address after their arguments (at Iwram_CopyWords the constant is
/// loaded before them).
#define ITEM_IWRAM_COPY_WORDS                                                  \
  ((ItemWordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))

// The item menu's messages, under The Broken Seal's names for the same text
// (message number in tla-en first).
extern const u8 MsgWhoseItem[];        // 04103: "Whose item?"
extern const u8 MsgWhichItem[];        // 04104: "Which item?"
extern const u8 MsgUseOnWhom[];        // 04106: "Use it on whom?"
extern const u8 MsgGiveToWhom[];       // 04107: "Give it to whom?"
extern const u8 MsgSwapForWhat[];      // 04108: "Swap it for what?"
extern const u8 MsgEquippedIt[];       // 04269: "Equipped it."
extern const u8 MsgDroppedIt[];        // 04270: "Dropped it."
extern const u8 MsgGiven[];            // 04272: "Given"
extern const u8 MsgRemoved[];          // 04273: "Removed"
extern const u8 MsgTraded[];           // 04274: "Traded"
extern const u8 MsgCannotRemoveItem[]; // 04275: "Can't remove it!"
extern const u8 MsgCannotTrade[];      // 04277: "Can't trade it!"
extern const u8 MsgCannotHoldMore[];   // 04278: "Can't hold more!"
extern const u8 MsgItemUseResult[]; // 04384: "You recovered all HP!", the first
                                    // item-use result

s32 Unit_RefreshPsynergyFar(s32 unit);

void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Func_080fae8c(s32 member, s32 slot, s32 mode, s32 target);

s32 Func_080f9644(s32 *ownerOut, s32 *targetOut, s32 *itemOut) {
  s32 done;
  s32 selection;
  s32 ret;
  struct OwnerInventoryState *source;
  struct OwnerInventoryState *target;
  struct ItemCommandWork *menu;
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
      menu->selectedSlot = 0;
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      menu->selectedItemIcon->state = 13;
      ItemMenu_DrawMessage(0, (s32)&MsgWhoseItem);
      WindowSprite_RedrawSavedRectFar(menu->infoWindow);
      Text_DrawWorkValueWithLabel(menu->infoWindow);
      command = ItemMenu_SelectMember(0);
      if (command == -1) {
        /* FAKEMATCH(dead): command cleared with selection, though neither is
         * read again once done is set; as selection = 0;, 104 of 1355
         * instructions differ in tla-en. */
        selection = command = 0;
        ret = -1;
        done = 1;
      }
      WindowSprite_RedrawSavedRectFar(menu->infoWindow);
      ItemMenu_HideAllIcons();
      state = 1;
      break;

    case 1:
      if (ItemMenu_CountItems(menu->itemOwner) == 0) {
        state = 0;
        break;
      }
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      menu->selectedItemIcon->state = 13;
      menu->listIcon->state = 1;
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
        if (BattleEffect_HasTriggerFar(menu->selectedItem & 0x1ff) != 0) {
          done = 1;
          *ownerOut = menu->itemOwner;
          *targetOut = command;
          *itemOut = menu->selectedItem & 0x1ff;
          ret = 1;
          break;
        }
        result = Item_ClassifyUseMode(menu->itemOwner, menu->selectedItem);
        if (result == 1) {
          state = 2;
        }
        if (result == 2) {
          ItemMenu_Use();
          WindowSprite_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(
              menu->messageOffset + (s32)MsgItemUseResult, 0, -1);
          menu->listIcon->state = 13;
          menu->itemCount =
              ItemMenu_CollectItems(Unit_Get(menu->itemOwner), menu->items, 0);
          ItemMenu_DrawIcons(menu->items, 0);
          state = 0;
        }
        if (result == -1 || result == 0) {
          done = 1;
          *ownerOut = menu->itemOwner;
          *targetOut = menu->targetOwner;
          *itemOut = menu->selectedItem & 0x1ff;
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
      WindowSprite_RedrawSavedRectFar(menu->messageWindow);
      Func_080fc12c();
      Text_DrawCharacterAtOffsetFar((s32)MsgUseOnWhom, menu->messageWindow, 16,
                                    16);
      if (Func_080fa870(0) != -1) {
        command = 0;
        if (ItemMenu_IsSpecial(menu->selectedItem & 0x1ff) != 0) {
          command = 8;
        }
        result = ItemMenu_Use();
        Func_080f8170(menu->statusWindow, menu->targetOwner, 0, command);
        if (result != -1) {
          WindowSprite_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(
              menu->messageOffset + (s32)MsgItemUseResult, 0, -1);
          menu->listIcon->state = 13;
          Func_080fb6d4();
          state = 1;
        }
        menu->itemCount =
            ItemMenu_CollectItems(Unit_Get(menu->itemOwner), menu->items, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        menu->completionFlag = 1;
      } else {
        state = 9;
      }
      break;

    case 6:
      ItemMenu_SetMessageWindowHeight5();
      ItemMenu_SetItemWindowRow5();
      WindowSprite_RedrawSavedRectFar(menu->messageWindow);
      Func_080fc12c();
      Text_DrawCharacterAtOffsetFar((s32)MsgGiveToWhom, menu->messageWindow, 16,
                                    16);
      n = Func_080fa870(1);
      state = 4;
      if (n == -1) {
        Func_080fae8c(menu->itemOwner, menu->selectedSlot, 0, menu->itemOwner);
        state = 9;
      }
      break;

    case 5:
      ItemMenu_HideAllIcons();
      item = Item_Get(menu->selectedItem & 0x1ff);
      result = 0;
      if ((item->flags & 16) != 0) {
        qty = (menu->selectedItem >> 11) + 1;
        if (qty > 1) {
          Func_080fc12c();
          result = Func_080fbe6c(0, qty, 1);
        }
      }
      if (result == -1) {
        state = 9;
        break;
      }
      menu->targetOwner = 0;
      Func_08038288(2, (menu->selectedItem & 0x1ff) | (result << 11),
                    menu->selectedItemIcon->renderTarget, 0);
      menu->selectedItemIcon->state = 1;
      menu->selectedItemIcon->x = 120;
      menu->selectedItemIcon->y = 28;
      Icon_PrepareActor(menu->selectedItemIcon);
      Window_ClearInteriorTilesFar(menu->previewWindow, 0, 72, 120, 96);
      WindowSprite_RedrawSavedRectFar(menu->messageWindow);
      if (Func_080fc1ac(selection) == 0) {
        command = menu->itemOwner;
        // DEAD: the result is not used; the source keeps the call.
        Unit_Get(command);
        for (work = 0; work < result + 1; work++) {
          Inventory_RemoveFar(command, menu->selectedSlot);
          Item_AdjustCounterFar(menu->selectedItem & 0x1ff, 1);
        }
        /* FAKEMATCH(signature): Unit_RefreshPsynergyFar declared returning s32,
         * though this game's routine returns nothing, here and below; as void,
         * 4 of 1355 instructions differ in tla-en. */
        Unit_RefreshPsynergyFar(command);
        Unit_CalculateFar(command);
        ItemMenu_SetItemWindowRow3();
        ItemMenu_RefreshOwner(menu->itemOwner, 0);
        menu->selectedItemIcon->state = 13;
        menu->listIcon->state = 13;
        wait(1);
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)MsgDroppedIt, 15, 13);
        menu->completionFlag = 1;
        state = 1;
      } else {
        state = 9;
      }
      Unit_CalculateFar(menu->itemOwner);
      menu->selectedItemIcon->state = 13;
      Event_ClearInvalidPackedValuesFar();
      break;

    case 4:
      aborted = 0;
      item = Item_Get(menu->selectedItem & 0x1ff);
      if ((item->flags & 16) != 0) {
        qty = InventoryMenu_GetItemQuantity(menu->targetOwner,
                                            menu->selectedItem & 0x1ff);
        if (qty == 30) {
          aborted = 1;
        }
        if (ItemMenu_CountItems(menu->targetOwner) == 15 && qty == 0) {
          state = 7;
          break;
        }
        stack = (menu->selectedItem >> 11) + 1;
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
            qty = Inventory_AddItemFar(menu->targetOwner,
                                       menu->selectedItem & 0x5ff);
            if (qty != -1) {
              Inventory_RemoveFar(menu->itemOwner, menu->selectedSlot);
              menu->targetSlot = qty;
            } else {
              aborted = 1;
            }
          }
        }
      } else {
        result =
            Inventory_AddItemFar(menu->targetOwner, menu->selectedItem & 0x5ff);
        if (result == -1) {
          state = 7;
          break;
        }
        menu->targetSlot = result;
        result = Inventory_RemoveFar(menu->itemOwner, menu->selectedSlot);
        if (result == -1) {
          aborted = 1;
        }
      }
      Unit_RefreshPsynergyFar(menu->itemOwner);
      Unit_RefreshPsynergyFar(menu->targetOwner);
      Unit_CalculateFar(menu->itemOwner);
      Unit_CalculateFar(menu->targetOwner);
      result = 1;
      if (aborted == 0) {
        menu->itemOwner = menu->targetOwner;
        menu->selectedItem &= 0x1ff;
        WindowSprite_RedrawSavedRectFar(menu->messageWindow);
        Func_080fc12c();
        result = Func_080fc2e0(0);
      }
      if (GameFlag_Test(0x150) != 0) {
        break;
      }
      ItemMenu_RefreshOwner(menu->targetOwner, 1);
      menu->listIcon->state = 13;
      wait(1);
      if (aborted == 1) {
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)MsgCannotHoldMore, 15, 14);
      } else {
        WindowSprite_ClearListFar(menu->infoWindow);
        if (result == 1) {
          InventoryMenu_ShowModalMessage((s32)MsgGiven, 15, 14);
        } else {
          Func_080fae8c(menu->targetOwner, menu->targetSlot, 0,
                        menu->targetOwner);
          InventoryMenu_ShowModalMessage((s32)MsgEquippedIt, 15, 14);
          item = Item_Get(menu->selectedItem);
          if ((item->flags & 1) != 0) {
            Sound_PlayFar(0x67);
            WindowSprite_ClearListFar(menu->infoWindow);
            InventoryMenu_ShowModalMessage((s32)MsgEquippedIt + 7, 14, 14);
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
      source = Unit_Get(menu->itemOwner);
      target = Unit_Get(menu->targetOwner);
      size = BATTLE_UNIT_SIZE;
      sourceCopy = Heap_Alloc(size);
      targetCopy = Heap_Alloc(size);
      ITEM_IWRAM_COPY_WORDS(sourceCopy, source, size);
      ITEM_IWRAM_COPY_WORDS(targetCopy, target, size);
      movedSource = 0;
      while (movedSource <= 29) {
        result = Inventory_RemoveFar(menu->itemOwner, menu->selectedSlot);
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
        if ((menu->targetItem & 0x200) != 0) {
          item = Item_Get(menu->targetItem & 0x1ff);
          if ((item->flags & 2) != 0) {
            aborted = 1;
          }
        }
        result = Inventory_RemoveFar(menu->targetOwner, menu->targetSlot);
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
        result =
            Inventory_AddItemFar(menu->targetOwner, menu->selectedItem & 0x5ff);
        if (result == -1) {
          aborted = 1;
          break;
        }
        menu->targetSlot = result;
        movedSource--;
      }
      while (movedTarget != 0) {
        result =
            Inventory_AddItemFar(menu->itemOwner, menu->targetItem & 0x5ff);
        if (result == -1) {
          aborted = 1;
          break;
        }
        menu->selectedSlot = result;
        movedTarget--;
      }
      wait(1);
      if (aborted == 1) {
        Iwram_CopyWords(source, sourceCopy, size);
        Iwram_CopyWords(target, targetCopy, size);
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)MsgCannotTrade, 15, 14);
      } else {
        Unit_RefreshPsynergyFar(menu->itemOwner);
        Unit_RefreshPsynergyFar(menu->targetOwner);
        Unit_CalculateFar(menu->itemOwner);
        Unit_CalculateFar(menu->targetOwner);
        ItemMenu_SetMessageWindowHeight5();
        WindowSprite_RedrawSavedRectFar(menu->messageWindow);
        menu->itemOwner = menu->targetOwner;
        menu->selectedItem &= 0x1ff;
        Func_080fc12c();
        result = Func_080fc2e0(0);
        if (GameFlag_Test(0x150) == 0) {
          WindowSprite_ClearListFar(menu->infoWindow);
          ItemMenu_SetItemWindowRow5();
          ItemMenu_RefreshOwner(menu->targetOwner, 1);
          if (result == 0) {
            Func_080fae8c(menu->targetOwner, menu->targetSlot, 0,
                          menu->targetOwner);
            InventoryMenu_ShowModalMessage((s32)MsgEquippedIt, 15, 14);
            item = Item_Get(menu->selectedItem);
            if ((item->flags & 1) != 0) {
              Sound_PlayFar(0x67);
              WindowSprite_ClearListFar(menu->infoWindow);
              InventoryMenu_ShowModalMessage((s32)MsgEquippedIt + 7, 14, 14);
            }
          } else {
            InventoryMenu_ShowModalMessage((s32)MsgTraded, 15, 14);
          }
        }
      }
      Heap_Release(targetCopy);
      Heap_Release(sourceCopy);
      Event_ClearInvalidPackedValuesFar();
      state = 0;
      break;

    case 3:
      result = Inventory_EquipFar(menu->itemOwner, menu->selectedSlot);
      if (result == -1) {
        state = 1;
        break;
      }
      if (result == -2) {
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)MsgCannotRemoveItem, 0, -1);
        state = 1;
        break;
      }
      Unit_RefreshPsynergyFar(menu->itemOwner);
      Unit_CalculateFar(menu->itemOwner);
      menu->listIcon->state = 13;
      menu->itemCount =
          ItemMenu_CollectItems(Unit_Get(menu->itemOwner), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      wait(1);
      Func_080fae8c(menu->itemOwner, menu->selectedSlot, 0, menu->itemOwner);
      WindowSprite_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage((s32)MsgEquippedIt, 15, 8);
      item = Item_Get(menu->selectedItem);
      if ((item->flags & 1) != 0) {
        Sound_PlayFar(0x67);
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)MsgEquippedIt + 7, 14, 8);
      }
      state = 1;
      break;

    case 11:
      ((struct OwnerInventoryState *)Unit_Get(menu->itemOwner))
          ->inventory[menu->selectedSlot] &= 0xfdff;
      Unit_RefreshPsynergyFar(menu->itemOwner);
      Unit_CalculateFar(menu->itemOwner);
      menu->listIcon->state = 13;
      menu->itemCount =
          ItemMenu_CollectItems(Unit_Get(menu->itemOwner), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      wait(1);
      menu->equipPreview = 1;
      Func_080fae8c(menu->itemOwner, menu->selectedSlot, 0, menu->itemOwner);
      menu->equipPreview = 0;
      WindowSprite_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage((s32)MsgRemoved, 14, 8);
      Event_ClearInvalidPackedValuesFar();
      state = 1;
      break;

    case 10:
      menu->listIcon->state = 13;
      Func_080fb780(menu->selectedItem);
      WindowSprite_RedrawSavedRectFar(menu->statusWindow);
      Func_080fae8c(menu->itemOwner, menu->selectedSlot, 0, menu->itemOwner);
      menu->listIcon->state = 1;
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

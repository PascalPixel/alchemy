// Near miss, differ 52 of 1355 (2026-10-26): the item menu's command loop (use,
// give, drop, equip, swap, trade, remove), the twin of The Broken Seal's
// ItemMenu_RunCommands (games/tbs/src/menu/menu/item.c), with The Lost Age's
// work layout (Ram_HeapSlots->menuRuntime), its calls and its own steps. Left:
// the trade's restoring copies rebuild their 0x14c size for each call (the
// native keeps IwramCopyWords in r5, not the size), and instruction order in
// the argument setups (the message number after the window height). Its
// messages are numbered here (Msg04103 ...), their names in The Broken Seal
// beside them; they need names in the catalogs before it links.
#include "mem.h"
#include "types.h"
#include "iwram_call.h"
#include "item.h"
#include "owner_state.h"
#include "ram_buffer.h"

struct ItemMenuIcon {
  u8 unk00[5];
  u8 state;
  u16 x;
  u16 y;
  u8 unk0a[4];
  u8 renderTarget;
};

struct ItemCommandWork {
  u8 unk000[0x14];
  union {
    struct ItemMenuIcon *icon;
    u32 word;
  } list;
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
  struct ItemMenuIcon *volatile selectedItemIcon;
  u8 unk21c[2];
  s16 completionFlag;
  u8 unk220[0x3e];
  s16 messageOffset;
  u8 equipPreview;
  u8 listMode;
};

// The item menu's messages, by number (their ☀️ names in the comments).
extern u8 Msg04103[]; // "Whose item?" (MsgWhoseItem)
extern u8 Msg04104[]; // "Which item?" (MsgWhichItem)
extern u8 Msg04106[]; // "Use it on whom?" (MsgUseOnWhom)
extern u8 Msg04107[]; // "Give it to whom?" (MsgGiveToWhom)
extern u8 Msg04108[]; // "Swap it for what?" (MsgSwapForWhat)
extern u8 Msg04269[]; // "Equipped it." (MsgEquippedIt)
extern u8 Msg04270[]; // "Dropped it." (MsgDroppedIt)
extern u8 Msg04272[]; // "Given" (MsgGiven)
extern u8 Msg04273[]; // "Removed" (MsgRemoved)
extern u8 Msg04274[]; // "Traded" (MsgTraded)
extern u8 Msg04275[]; // "Can't remove it!" (MsgCannotRemoveItem)
extern u8 Msg04277[]; // "Can't trade it!" (MsgCannotTrade)
extern u8 Msg04278[]; // "Can't hold more!" (MsgCannotHoldMore)
extern u8 Msg04384[]; // the first item-use result (MsgItemUseResult)

struct OwnerInventoryState *Unit_GetFar(s32 unit);
void Unit_RefreshPsynergyFar(s32 unit);

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

#define IWRAM_COPY_WORDS ((WordCopyFn)Iwram_CopyWords)

s32 ItemMenu_RunCommands(s32 *ownerOut, s32 *targetOut, s32 *itemOut) {
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

  done = 0;
  selection = 0;
  ret = 0;
  state = 0;
  menu = Ram_HeapSlots->menuRuntime;

  while (done == 0 && GameFlag_TestFar(0x150) == 0) {
    switch (state) {
    case 0:
      menu->selectedSlot = 0;
      ItemMenu_SetMessageWindowHeight3();
      ItemMenu_SetItemWindowRow3();
      menu->selectedItemIcon->state = 13;
      ItemMenu_DrawMessage(0, (s32)&Msg04103);
      WindowSprite_RedrawSavedRectFar(menu->infoWindow);
      Text_DrawWorkValueWithLabel(menu->infoWindow);
      command = ItemMenu_SelectMember(0);
      if (command == -1) {
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
      menu->list.icon->state = 1;
      ItemMenu_DrawMessage(0, (s32)&Msg04104);
      selection = ItemMenu_RunList(0);
      state = 0;
      if (selection == -1) {
        break;
      }
      menu->listMode = 0xff;
      state = 9;
      break;

    case 9:
      command = Func_080a414c();
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
              menu->messageOffset + (s32)&Msg04384, 0, -1);
          menu->list.icon->state = 13;
          menu->itemCount =
              ItemMenu_CollectItems(Unit_GetFar(menu->itemOwner), menu->items, 0);
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
      ItemMenu_DrawItemHead();
      Text_DrawCharacterAtOffsetFar((s32)&Msg04106, menu->messageWindow,
                                      16, 16);
      if (ItemMenu_SelectTarget(0) != -1) {
        command = 0;
        if (ItemMenu_IsSpecial(menu->selectedItem & 0x1ff) != 0) {
          command = 8;
        }
        result = ItemMenu_Use();
        Menu_DrawOwnerStatusPanel(menu->statusWindow, menu->targetOwner, 0,
                                  command);
        if (result != -1) {
          WindowSprite_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(
              menu->messageOffset + (s32)&Msg04384, 0, -1);
          menu->list.icon->state = 13;
          ItemMenu_TryBreak();
          state = 1;
        }
        menu->itemCount =
            ItemMenu_CollectItems(Unit_GetFar(menu->itemOwner), menu->items, 0);
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
      ItemMenu_DrawItemHead();
      Text_DrawCharacterAtOffsetFar((s32)&Msg04107, menu->messageWindow,
                                      16, 16);
      n = ItemMenu_SelectTarget(1);
      state = 4;
      if (n == -1) {
        ItemMenu_DrawEquipPreview(menu->itemOwner, menu->selectedSlot, 0,
                                  menu->itemOwner);
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
          ItemMenu_DrawItemHead();
          result = Func_080fbe6c(0, qty, 1);
        }
      }
      if (result == -1) {
        state = 9;
        break;
      }
      menu->targetOwner = 0;
      Resource_LoadByModeIntoSlotFar(
          2, (menu->selectedItem & 0x1ff) | (result << 11),
          menu->selectedItemIcon->renderTarget, 0);
      menu->selectedItemIcon->state = 1;
      menu->selectedItemIcon->x = 120;
      menu->selectedItemIcon->y = 28;
      Icon_PrepareActor(menu->selectedItemIcon);
      Window_ClearInteriorTilesFar(menu->previewWindow, 0, 72, 120, 96);
      WindowSprite_RedrawSavedRectFar(menu->messageWindow);
      if (ItemMenu_ConfirmDrop(selection) == 0) {
        command = menu->itemOwner;
        Unit_GetFar(command);
        for (work = 0; work < result + 1; work++) {
          Inventory_RemoveFar(command, menu->selectedSlot);
          Func_08077240(menu->selectedItem & 0x1ff, 1);
        }
        Unit_RefreshPsynergyFar(command);
        Unit_CalculateFar(command);
        ItemMenu_SetItemWindowRow3();
        ItemMenu_RefreshOwner(menu->itemOwner, 0);
        menu->selectedItemIcon->state = 13;
        menu->list.icon->state = 13;
        WaitFrames(1);
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)&Msg04270, 15, 13);
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
          /* FAKEMATCH: the loop only places a label between the
             count and its sign-extended use. */
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
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
      }
      if (GameFlag_TestFar(0x150) != 0) {
        break;
      }
      ItemMenu_RefreshOwner(menu->targetOwner, 1);
      menu->list.icon->state = 13;
      WaitFrames(1);
      if (aborted == 1) {
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)&Msg04278, 15, 14);
      } else {
        WindowSprite_ClearListFar(menu->infoWindow);
        if (result == 1) {
          InventoryMenu_ShowModalMessage((s32)&Msg04272, 15, 14);
        } else {
          ItemMenu_DrawEquipPreview(menu->targetOwner, menu->targetSlot, 0,
                                    menu->targetOwner);
          InventoryMenu_ShowModalMessage((s32)&Msg04269, 15, 14);
          item = Item_Get(menu->selectedItem);
          if ((item->flags & 1) != 0) {
            Sound_PlayFar(0x67);
            WindowSprite_ClearListFar(menu->infoWindow);
            InventoryMenu_ShowModalMessage((s32)&Msg04269 + 7, 14, 14);
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
      ItemMenu_DrawMessage(0, (s32)&Msg04108);
      if (ItemMenu_RunList(1) == -1) {
        state = 6;
        break;
      }
      source = Unit_GetFar(menu->itemOwner);
      target = Unit_GetFar(menu->targetOwner);
      sourceCopy = Heap_Alloc(0x14c);
      targetCopy = Heap_Alloc(0x14c);
      Iwram_CopyWords(sourceCopy, source, 0x14c);
      Iwram_CopyWords(targetCopy, target, 0x14c);
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
      WaitFrames(1);
      if (aborted == 1) {
        Iwram_CopyWords(source, sourceCopy, 0x14c);
        Iwram_CopyWords(target, targetCopy, 0x14c);
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)&Msg04277, 15, 14);
      } else {
        Unit_RefreshPsynergyFar(menu->itemOwner);
        Unit_RefreshPsynergyFar(menu->targetOwner);
        Unit_CalculateFar(menu->itemOwner);
        Unit_CalculateFar(menu->targetOwner);
        ItemMenu_SetMessageWindowHeight5();
        WindowSprite_RedrawSavedRectFar(menu->messageWindow);
        menu->itemOwner = menu->targetOwner;
        menu->selectedItem &= 0x1ff;
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
        if (GameFlag_TestFar(0x150) == 0) {
          WindowSprite_ClearListFar(menu->infoWindow);
          ItemMenu_SetItemWindowRow5();
          ItemMenu_RefreshOwner(menu->targetOwner, 1);
          if (result == 0) {
            ItemMenu_DrawEquipPreview(menu->targetOwner, menu->targetSlot, 0,
                                      menu->targetOwner);
            InventoryMenu_ShowModalMessage((s32)&Msg04269, 15, 14);
            item = Item_Get(menu->selectedItem);
            if ((item->flags & 1) != 0) {
              Sound_PlayFar(0x67);
              WindowSprite_ClearListFar(menu->infoWindow);
              InventoryMenu_ShowModalMessage((s32)&Msg04269 + 7, 14, 14);
            }
          } else {
            InventoryMenu_ShowModalMessage((s32)&Msg04274, 15, 14);
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
        InventoryMenu_ShowModalMessage((s32)&Msg04275, 0, -1);
        state = 1;
        break;
      }
      Unit_CalculateFar(menu->itemOwner);
      Func_080772c0(menu->itemOwner);
      menu->list.icon->state = 13;
      menu->itemCount =
          ItemMenu_CollectItems(Unit_GetFar(menu->itemOwner), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      WaitFrames(1);
      ItemMenu_DrawEquipPreview(menu->itemOwner, menu->selectedSlot, 0,
                                menu->itemOwner);
      WindowSprite_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage((s32)&Msg04269, 15, 8);
      item = Item_Get(menu->selectedItem);
      if ((item->flags & 1) != 0) {
        Sound_PlayFar(0x67);
        WindowSprite_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)&Msg04269 + 7, 14, 8);
      }
      state = 1;
      break;

    case 11:
      Unit_GetFar(menu->itemOwner)->inventory[menu->selectedSlot] &= 0xfdff;
      Unit_CalculateFar(menu->itemOwner);
      Func_080772c0(menu->itemOwner);
      menu->list.icon->state = 13;
      menu->itemCount =
          ItemMenu_CollectItems(Unit_GetFar(menu->itemOwner), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      WaitFrames(1);
      menu->equipPreview = 1;
      ItemMenu_DrawEquipPreview(menu->itemOwner, menu->selectedSlot, 0,
                                menu->itemOwner);
      menu->equipPreview = 0;
      WindowSprite_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage((s32)&Msg04273, 14, 8);
      Event_ClearInvalidPackedValuesFar();
      state = 1;
      break;

    case 10:
      menu->list.icon->state = 13;
      Menu_SelectQuantity(menu->selectedItem);
      WindowSprite_RedrawSavedRectFar(menu->statusWindow);
      ItemMenu_DrawEquipPreview(menu->itemOwner, menu->selectedSlot, 0,
                                menu->itemOwner);
      menu->list.icon->state = 1;
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

  if (GameFlag_TestFar(0x150) != 0) {
    ret = -1;
  }
  return ret;
}

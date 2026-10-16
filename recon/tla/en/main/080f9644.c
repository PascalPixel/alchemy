#include "mem.h"
#include "types.h"
#include "iwram_call.h"
#include "item.h"
#include "owner_state.h"
extern u8 gData_03001f2c[];

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
  u8 unk018[0x0c];
  s32 statusWindow;
  u8 unk028[4];
  s32 infoWindow;
  u8 unk030[4];
  s32 previewWindow;
  u8 unk038[0xd4];
  s32 messageWindow;
  u8 unk110[0x64];
  u16 selectedSlot;
  u16 targetSlot;
  u16 selectedItem;
  u16 targetItem;
  u8 unk17c[0x4c];
  u16 items[32];
  u8 unk208[0x10];
  u8 itemCount;
  u8 partyCount;
  u8 itemOwner;
  u8 targetOwner;
  struct ItemMenuIcon *volatile selectedItemIcon;
  u8 unk220[2];
  s16 completionFlag;
  u8 unk224[0x36];
  s16 messageOffset;
  u8 equipPreview;
  u8 listMode;
};

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

#define IWRAM_COPY_WORDS ((WordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))

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
  menu = *(struct ItemCommandWork **)((u32)&gData_03001f2c);

  while (done == 0 && GameFlag_TestFar(0x150) == 0) {
    switch (state) {
    case 0:
      menu->selectedSlot = 0;
      ItemMenu_SetMsgWin3();
      ItemMenu_SetItemWin3();
      menu->selectedItemIcon->state = 13;
      ItemMenu_DrawMsg(0, (s32)&MsgWhoseItem);
      RenderOutput_RedrawSavedRectFar(menu->infoWindow);
      UiText_DrawWorkValueWithLabel(menu->infoWindow);
      command = ItemMenu_PrepOwner(0);
      if (command == -1) {
        selection = command = 0;
        ret = -1;
        done = 1;
      }
      RenderOutput_RedrawSavedRectFar(menu->infoWindow);
      ItemMenu_HideAllIcons();
      state = 1;
      break;

    case 1:
      if (ItemMenu_Count(menu->itemOwner) == 0) {
        state = 0;
        break;
      }
      ItemMenu_SetMsgWin3();
      ItemMenu_SetItemWin3();
      menu->selectedItemIcon->state = 13;
      menu->list.icon->state = 1;
      ItemMenu_DrawMsg(0, (s32)&MsgWhichItem);
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
        if (BattleFx_HasTriggerFar(menu->selectedItem & 0x1ff) != 0) {
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
          RenderOutput_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(
              menu->messageOffset + (s32)&MsgItemUseResult, 0, -1);
          menu->list.icon->state = 13;
          menu->itemCount =
              ItemMenu_Collect(UnitGetFar(menu->itemOwner), menu->items, 0);
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
      ItemMenu_SetMsgWin5();
      ItemMenu_SetItemWin5();
      RenderOutput_RedrawSavedRectFar(menu->messageWindow);
      ItemMenu_DrawItemHead();
      UiText_DrawCharacterAtOffsetFar((s32)&MsgUseOnWhom, menu->messageWindow,
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
          RenderOutput_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(
              menu->messageOffset + (s32)&MsgItemUseResult, 0, -1);
          menu->list.icon->state = 13;
          ItemMenu_TryBreak();
          state = 1;
        }
        menu->itemCount =
            ItemMenu_Collect(UnitGetFar(menu->itemOwner), menu->items, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        menu->completionFlag = 1;
      } else {
        state = 9;
      }
      break;

    case 6:
      ItemMenu_SetMsgWin5();
      ItemMenu_SetItemWin5();
      RenderOutput_RedrawSavedRectFar(menu->messageWindow);
      ItemMenu_DrawItemHead();
      UiText_DrawCharacterAtOffsetFar((s32)&MsgGiveToWhom, menu->messageWindow,
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
          result = Func_080a4f08(0, qty, 1);
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
      UiIcon_PrepareObject(menu->selectedItemIcon);
      UiWindow_ClearInteriorTilesFar(menu->previewWindow, 0, 72, 120, 96);
      RenderOutput_RedrawSavedRectFar(menu->messageWindow);
      if (ItemMenu_ConfirmDrop(selection) == 0) {
        command = menu->itemOwner;
        UnitGetFar(command);
        for (work = 0; work < result + 1; work++) {
          Inventory_RemoveFar(command, menu->selectedSlot);
          Func_08077240(menu->selectedItem & 0x1ff, 1);
        }
        UnitCalcFar(command);
        ItemMenu_SetItemWin3();
        ItemMenu_RefreshOwner(menu->itemOwner, 0);
        menu->selectedItemIcon->state = 13;
        menu->list.icon->state = 13;
        WaitFrames(1);
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)&MsgDroppedIt, 14, 13);
        menu->completionFlag = 1;
        state = 1;
      } else {
        state = 9;
      }
      UnitCalcFar(menu->itemOwner);
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
        if (ItemMenu_Count(menu->targetOwner) == 15 && qty == 0) {
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
            result = Func_080a4f08(0, amount, 0);
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
      UnitCalcFar(menu->itemOwner);
      UnitCalcFar(menu->targetOwner);
      Func_080772c0(menu->itemOwner);
      Func_080772c0(menu->targetOwner);
      result = 1;
      if (aborted == 0) {
        menu->itemOwner = menu->targetOwner;
        menu->selectedItem &= 0x1ff;
        ItemMenu_SetMsgWin6();
        RenderOutput_RedrawSavedRectFar(menu->messageWindow);
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
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)&MsgCannotHoldMore, 15, 14);
      } else {
        RenderOutput_ClearListFar(menu->infoWindow);
        if (result == 1) {
          InventoryMenu_ShowModalMessage((s32)&MsgGiven, 15, 14);
        } else {
          ItemMenu_DrawEquipPreview(menu->targetOwner, menu->targetSlot, 0,
                                    menu->targetOwner);
          InventoryMenu_ShowModalMessage((s32)&MsgEquippedIt, 15, 14);
          item = Item_Get(menu->selectedItem);
          if ((item->flags & 1) != 0) {
            Audio_PlayCue(0x67);
            RenderOutput_ClearListFar(menu->infoWindow);
            InventoryMenu_ShowModalMessage((s32)&MsgEquippedIt + 7, 14, 14);
          }
        }
      }
      Event_ClearInvalidPackedValuesFar();
      state = 0;
      break;

    case 7:
      aborted = 0;
      ItemMenu_SetMsgWin3();
      ItemMenu_SetItemWin3();
      ItemMenu_DrawMsg(0, (s32)&MsgSwapForWhat);
      selection = ItemMenu_RunList(1);
      if (selection == -1) {
        state = 6;
        break;
      }
      source = UnitGetFar(menu->itemOwner);
      target = UnitGetFar(menu->targetOwner);
      sourceCopy = MemAlloc(0x14c);
      targetCopy = MemAlloc(0x14c);
      CopyWords(IWRAM_COPY_WORDS, sourceCopy, source, 0x14c);
      CopyWords(IWRAM_COPY_WORDS, targetCopy, target, 0x14c);
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
        CopyWords(IWRAM_COPY_WORDS, source, sourceCopy, 0x14c);
        CopyWords(IWRAM_COPY_WORDS, target, targetCopy, 0x14c);
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)&MsgCannotTrade, 15, 14);
      } else {
        UnitCalcFar(menu->itemOwner);
        UnitCalcFar(menu->targetOwner);
        Func_080772c0(menu->itemOwner);
        Func_080772c0(menu->targetOwner);
        ItemMenu_SetMsgWin5();
        ItemMenu_SetMsgWin6();
        ItemMenu_HidePageIcons();
        RenderOutput_RedrawSavedRectFar(menu->messageWindow);
        menu->itemOwner = menu->targetOwner;
        menu->selectedItem &= 0x1ff;
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
        if (GameFlag_TestFar(0x150) == 0) {
          RenderOutput_ClearListFar(menu->infoWindow);
          ItemMenu_SetItemWin5();
          ItemMenu_RefreshOwner(menu->targetOwner, 1);
          if (result == 0) {
            ItemMenu_DrawEquipPreview(menu->targetOwner, menu->targetSlot, 0,
                                      menu->targetOwner);
            InventoryMenu_ShowModalMessage((s32)&MsgEquippedIt, 15, 14);
            item = Item_Get(menu->selectedItem);
            if ((item->flags & 1) != 0) {
              Audio_PlayCue(0x67);
              RenderOutput_ClearListFar(menu->infoWindow);
              InventoryMenu_ShowModalMessage((s32)&MsgEquippedIt + 7, 14, 14);
            }
          } else {
            InventoryMenu_ShowModalMessage((s32)&MsgTraded, 15, 14);
          }
        }
      }
      MemFree(targetCopy);
      MemFree(sourceCopy);
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
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)&MsgCannotRemoveItem, 0, -1);
        state = 1;
        break;
      }
      UnitCalcFar(menu->itemOwner);
      Func_080772c0(menu->itemOwner);
      menu->list.icon->state = 13;
      menu->itemCount =
          ItemMenu_Collect(UnitGetFar(menu->itemOwner), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      WaitFrames(1);
      ItemMenu_DrawEquipPreview(menu->itemOwner, menu->selectedSlot, 0,
                                menu->itemOwner);
      RenderOutput_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage((s32)&MsgEquippedIt, 15, 8);
      item = Item_Get(menu->selectedItem);
      if ((item->flags & 1) != 0) {
        Audio_PlayCue(0x67);
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage((s32)&MsgEquippedIt + 7, 14, 8);
      }
      state = 1;
      break;

    case 11:
      UnitGetFar(menu->itemOwner)->inventory[menu->selectedSlot] &= 0xfdff;
      UnitCalcFar(menu->itemOwner);
      Func_080772c0(menu->itemOwner);
      menu->list.icon->state = 13;
      menu->itemCount =
          ItemMenu_Collect(UnitGetFar(menu->itemOwner), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      WaitFrames(1);
      menu->equipPreview = 1;
      ItemMenu_DrawEquipPreview(menu->itemOwner, menu->selectedSlot, 0,
                                menu->itemOwner);
      menu->equipPreview = 0;
      RenderOutput_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage((s32)&MsgRemoved, 14, 8);
      Event_ClearInvalidPackedValuesFar();
      state = 1;
      break;

    case 10:
      menu->list.icon->state = 13;
      Menu_SelectQuantity(menu->selectedItem);
      RenderOutput_RedrawSavedRectFar(menu->statusWindow);
      ItemMenu_DrawEquipPreview(menu->itemOwner, menu->selectedSlot, 0,
                                menu->itemOwner);
      menu->list.icon->state = 1;
      state = 9;
      break;

    case 12:
      result = Func_080a4f08(0, 30, 0);
      if (result != -1) {
        qty = result;
      }
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

// First draft of Func_080fa2d0: uses the selected item from the owner in pane
// 0 on the member in pane 1, the twin of ☀️'s ItemMenu_Use
// (games/tbs/src/menu/menu.c) in its TBS_PATCH_1 form. On failure plays sound
// 114, shows why in the info window (InventoryMenu_ShowModalMessage) and returns -1; otherwise
// plays the item's animation (Func_08100d40), recalculates both members and
// returns 1. Near miss, differ 2 of 67, instruction order only: the native
// loads selectedItems[0] before it starts the 0x1ff mask (ldrh r3; movs r0,
// #128); here the mask's first move comes first. The message base 0x1120 is
// pooled, so a linker symbol.
#include "types.h"
#include "ram_buffer.h"

struct InventoryMenuState {
  u8 unk000[0x30];
  void *infoWindow;
  u8 unk034[0x134];
  u16 selectedSlots[2];
  u16 selectedItems[2];
  u8 unk170[0xa6];
  u8 paneOwner[2];
  u8 unk218[6];
  u16 completionFlag;
  u8 unk220[0x3e];
  s16 messageOffset;
};

extern u8 MsgItemUseResult[]; // the ☀️ twin's name

s32 Func_08100738(s32 slot, s32 owner, s32 target);
void Func_08100d40(s32 item);
void Audio_PlayCue(s32 cue);
void RenderOutput_ClearListFar(void *window);
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y);
void UnitCalcFar(s32 unit);

s32 Func_080fa2d0(void) {
  struct InventoryMenuState *menu;
  s32 result;

  menu = (struct InventoryMenuState *)Ram_HeapSlots->menuRuntime;
  result =
      Func_08100738(menu->selectedSlots[0], menu->paneOwner[0],
                    menu->paneOwner[1]);

  if (result == -1) {
    Audio_PlayCue(114);
    RenderOutput_ClearListFar(menu->infoWindow);
    InventoryMenu_ShowModalMessage(menu->messageOffset + (s32)MsgItemUseResult, result, result);
    menu->completionFlag = 1;
    return result;
  }

  Func_08100d40(menu->selectedItems[0] & 0x1ff);
  UnitCalcFar(menu->paneOwner[0]);
  UnitCalcFar(menu->paneOwner[1]);
  return 1;
}

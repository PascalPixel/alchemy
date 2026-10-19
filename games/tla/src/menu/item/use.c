#include "types.h"
#include "edition.h"
#include "ram_buffer.h"

struct ItemMenuUseWork {
  u8 unk000[0x30];
  void *infoWindow;
  u8 unk034[0x134];
  u16 selectedSlots[2];
  u16 selectedItems[2];
  u8 unk170[0xa6];
  u8 paneOwner[2];
  u8 unk218[6];
  s16 completionFlag;
  u8 unk220[0x3e];
  s16 messageOffset;
};

/// The first of the item use results. "You recovered all HP!"
extern const u8 MsgItemUseResult[];

s32 Func_08100738(s32 slot, s32 owner, s32 target);
void Func_08100d40(s32 item);
void Audio_PlayCue(s32 cue);
void RenderOutput_ClearListFar(void *window);
void RenderOutput_RedrawSavedRectFar(void *window);
void InventoryMenu_ShowModalMessage(const u8 *message, s32 a, s32 b);
void UnitCalcFar(s32 unit);

/// Uses the selected item from the owner in pane 0 on the member in pane 1, as
/// The Broken Seal's ItemMenu_Use. On failure plays the error sound, shows why
/// in the info window and returns -1; otherwise plays the item's animation,
/// recalculates both members and returns 1.
s32 ItemMenu_Use(void) {
  struct ItemMenuUseWork *menu;
  s32 result;
  u32 item;
  u32 mask;

  menu = Ram_HeapSlots->menuRuntime;
  result = Func_08100738(menu->selectedSlots[0], menu->paneOwner[0],
                         menu->paneOwner[1]);

  if (result == -1) {
    Audio_PlayCue(114);
    RenderOutput_ClearListFar(menu->infoWindow);
    InventoryMenu_ShowModalMessage(menu->messageOffset + MsgItemUseResult, result, result);
#if LANGUAGE == LANGUAGE_JAPANESE
    RenderOutput_RedrawSavedRectFar(menu->infoWindow);
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
  UnitCalcFar(menu->paneOwner[0]);
  UnitCalcFar(menu->paneOwner[1]);
  return 1;
}

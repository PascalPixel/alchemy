// First draft of Func_080fa870: the twin of ☀️'s ItemMenu_SelectTarget
// (games/tbs/src/menu/menu/item.c), choosing the party member to use (mode 0)
// or give (mode 1) the selected item to. This game pages the members four at a
// time through the cursor helper Func_08104c00, which also replaces the Left
// and Right tests; the item's owner is restored to the page on the way out.
//
// Near miss, differ 12 of 387, instruction order only: the native issues sub
// sp right after the menu load and the 0x215 offset after the selection load
// (here the offset's movs comes first); it adds the page x to selection * 3
// after the marker load, passes 2 before the window to the number draw, and
// sets the x before the message in the third label case. The window loaded
// before the count, the label and x chosen in each case before one draw, the
// marker's x as u16 and selection * 3 written first took differ 127 to 12.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"

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

extern u8 MsgInStock[];
extern u8 MsgTradeForWhat[];
extern u8 MsgNoneInStock[];
extern u8 MsgItemPlainName[];

void Func_080fabe0(void);
void UiWindow_SetBounds(void *window, s32 x, s32 y, s32 width, s32 height);
void RenderOutput_RedrawSavedRectFar(void *window);
void *UnitGet(s32 owner);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_08104ef8(s32 first);
void WaitFrames(s32 frames);
void Func_08104d5c(void *window, s32 selection, s32 count);
void ItemMenu_RefreshOwner(s32 owner, s32 mode);
void UiWindow_DrawDividerLineFar(void *window, s32 x0, s32 y0, s32 x1, s32 y1);
void UiWindow_ClearInteriorTilesFar(void *window, s32 x, s32 y, s32 width,
                                    s32 height);
s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item);
s32 ItemMenu_Count(s32 owner);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, void *window, s32 x,
                                  s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void Func_080fae8c(s32 owner, s32 slot, s32 mode, s32 target);
s32 ItemMenu_IsSpecial(s32 item);
void Func_080f8170(void *window, s32 owner, s32 slot, s32 style);
void Func_080f8a44(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
u8 Func_08104c00(s32 *selection, s32 count, s32 columns);
void UiIcon_PrepareObject(struct TargetMarker *icon);
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

  UiWindow_SetBounds(window, 13, 5, 17, 12);
  RenderOutput_RedrawSavedRectFar(menu->itemWindow);
  UnitGet(menu->ownerIds[menu->paneIndex[0]]);
  Scheduler_AddOrUpdateCallback(Func_080fabe0, 0x480);
  redraw = 2;
  while (!GameFlag_Test(0x150)) {
    if (redraw) {
      window = menu->itemWindow;
      UnitGet(menu->ownerIds[selection]);
      marker = menu->marker;
      marker->oamX = marker->x =
          ((selection * 3 + menu->pageWindow->x) << 3) - 2;
      if (redraw == 2) {
        Func_08104ef8(selection / 4 * 4);
        WaitFrames(1);
      }
      Func_08104d5c(menu->pageWindow, selection, count);
      if (mode == 1) {
        ItemMenu_RefreshOwner(menu->ownerIds[selection], 1);
        UiWindow_DrawDividerLineFar(window, 0, 9, 16, 9);
        UiWindow_ClearInteriorTilesFar(window, 0, 72, 120, 80);
        if (selection != menu->paneIndex[0]) {
          heldCount = InventoryMenu_GetItemQuantity(
              menu->ownerIds[selection], menu->selectedItems[0] & 0x1ff);
          if (ItemMenu_Count(menu->ownerIds[selection]) == 15 &&
              heldCount == 0) {
            labelX = 0;
            label = (s32)MsgTradeForWhat;
          } else if (heldCount != 0) {
            UiText_DrawNumberInWindowFar(heldCount, 2, window, 8, 72);
            labelX = 32;
            label = (s32)MsgInStock;
          } else {
            labelX = 16;
            label = (s32)MsgNoneInStock;
          }
          UiText_DrawCharacterAtOffsetFar(label, window, labelX, 72);
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
          RenderOutput_RedrawSavedRectFar(menu->infoWindow);
          UiText_DrawCharacterAtOffsetFar((menu->selectedItems[0] & 0x1ff) +
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
    WaitFrames(1);
    if (gInput.pressed & 1) {
      if (mode == 1 && selection == menu->paneIndex[0]) {
        Audio_PlayCue(114);
        continue;
      }
      Audio_PlayCue(112);
      result = menu->ownerIds[selection];
      break;
    }
    if (gInput.pressed & 2) {
      Audio_PlayCue(113);
      result = 255;
      break;
    }
    redraw = Func_08104c00(&selection, count, 4);
  }
  marker = menu->marker;
  UiIcon_PrepareObject(marker);
  marker->state = 13;
  Func_080fac58();
  WaitFrames(1);
  menu->paneIndex[1] = selection;
  menu->selectedOwner = menu->ownerIds[selection];
  menu->paneOwner[1] = menu->ownerIds[selection];
  selection = menu->paneIndex[0];
  count = menu->ownerCount;
  Func_08104ef8(selection / 4 * 4);
  Func_08104d5c(menu->pageWindow, selection, count);
  menu->ownerIcons[0]->state = 13;
  menu->ownerIcons[1]->state = 13;
  return (s8)result;
}

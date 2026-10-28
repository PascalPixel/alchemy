// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 16 of 338 in tla-en. First difference: this issues `mov r8, r1` earlier (row
// 10) than the native does (row 12).
// First draft of Func_080fb104: the twin of ☀️'s ItemMenu_ChooseCmd
// (games/tbs/src/menu/menu/item.c) in its first-release form (the cursor at
// column * 40 + 86, row * 8 + 30), choosing a command in the item command grid:
// Use, Equip and Details on the top row, Give, Remove and Drop below. This
// game draws no item name in the info window here. Returns the command chosen,
// or -1 when the player backs out.
//
// Near miss, differ 22 of 338, instruction order only: the native issues a
// load or move ahead of the first half of a constant six times (the 0x261 last
// command offset after the flag pointer reload, the 0x216 owner offset after
// the 0x260 store, and so on), as in Func_080fc6bc.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"

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

enum {
  ITEM_COMMAND_USE,
  ITEM_COMMAND_EQUIP,
  ITEM_COMMAND_INSPECT,
  ITEM_COMMAND_GIVE,
  ITEM_COMMAND_REMOVE,
  ITEM_COMMAND_DROP
};

void Func_080fb410(s8 *commandStates);
void ItemMenu_HideAllIcons(void);
void WindowSprite_RedrawSavedRectFar(void *window);
void ItemMenu_SetMessageWindowHeight7(void);
void Window_DrawDividerLineFar(void *window, s32 x0, s32 y0, s32 x1, s32 y1);
void Func_080fc12c(void);
void ItemMenu_DrawCommands(const s8 *commandStates, void *window);
void Func_080f8ab4(s32 x, s32 y);
void Func_080f8a44(s32 x, s32 y);
s32 GameFlag_Test(s32 flag);
void Func_080fac58(void);
void Func_080fae8c(s32 owner, s32 slot, s32 mode, s32 target);
void Func_080fabe0(void);
void Task_Add(void (*callback)(void), s32 priority);
void Func_080f8170(void *window, s32 owner, s32 slot, s32 style);
void WaitFrames(s32 frames);
void Sound_PlayFar(s32 cue);

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

  commandIndex = 0;
  column = 0;
  row = 0;
  menu = Ram_HeapSlots->menuRuntime;
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
  Func_080f8ab4(column * 40 + 86, row * 8 + 30);

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
    Func_080f8a44(column * 40 + 86, row * 8 + 30);
    WaitFrames(1);

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

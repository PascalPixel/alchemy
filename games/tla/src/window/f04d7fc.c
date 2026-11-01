#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "resource_menu.h"
#include "ram_buffer.h"
#include "task.h"

s32 Menu_CenterResourceEntries(s32, s32, s32);
void Menu_DrawModeLabel(void);
void Menu_DrawModeIndicator(void);
extern char MsgTransferMethod[];
extern char MsgPasswordEntry[];
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
void Text_DrawMessage(s32 message, void *window, s32 x, s32 y);
void WindowSprite_PrepareForRedraw(void *window);

s32 Func_0804d7fc(s32 mode) {
  struct ResourceMenuWork *work;
  struct Window *window;
  s32 msg;
  struct Window **upper;
  s32 result;

  mode &= 1;
  AffineEffect_InitializeWork();
  work = Ram_HeapSlots->menuSelectWork;
  if (mode == 0) {
    Menu_AppendResourceEntry(44);
    Menu_AppendResourceEntry(45);
  } else {
    Menu_AppendResourceEntry(46);
    Menu_AppendResourceEntry(47);
    Menu_AppendResourceEntry(48);
  }
  Menu_CenterResourceEntries(17, 7, 0);
  if (mode != 0) {
    Task_Add(Menu_DrawModeLabel, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = Window_Create(8, 0, 14, 4, 2);
    msg = (s32)MsgPasswordEntry;
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage(msg, window, 0, 4);
    window = Window_Create(4, 4, 22, 12, 2);
    work->lowerWindow = window;
    Text_DrawMessage(msg + 1, window, 0, 4);
    msg += 2;
    Text_DrawMessage(msg, work->lowerWindow, 0, 16);
  } else {
    Task_Add(Menu_DrawModeIndicator, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = Window_Create(6, 0, 18, 4, 2);
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage((s32)MsgTransferMethod, window, 12, 4);
    work->lowerWindow = Window_Create(5, 5, 21, 7, 2);
  }
  result = Menu_RunResourceSelectionLoop(0);
  if (mode != 0)
    Task_Remove(Menu_DrawModeLabel);
  else
    Task_Remove(Menu_DrawModeIndicator);
  WindowSprite_PrepareForRedraw(*upper);
  WindowSprite_PrepareForRedraw(work->lowerWindow);
  Window_Close(*upper, 2);
  Window_Close(work->lowerWindow, 2);
  wait(1);
  Menu_EndResourceSelection();
  return result;
}

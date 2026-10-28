// Under -mthumb-slow-loads (2026-10-26): differ 2 of 147 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Near miss, differ 4 of 147 (two reordered pairs), 2026-10-18: The
// Broken Seal's Menu_SelectResourceLayout with the mode reduced to its low
// bit, callbacks at 0x476 and the English edition's windows. Messages 4472
// ("Transfer Method") and 4473 ("Password Entry") are pool symbols the
// catalogs must name before adoption (MsgTransferMethodX and
// MsgPasswordEntryX here). Remaining, in the password case: the native
// forms the upper window's address (adds r6, r7) before it copies the new
// window to r1, and moves the message to r0 before it stores the window;
// this copies first and stores first. One-pass blocks after the message's
// load (differ 4) or after the store (the store stays first) do not help.
// Messages assigned after the window store or tied to an
// empty asm measure the same.

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
extern char MsgTransferMethodX[];
extern char MsgPasswordEntryX[];

s32 Func_0804d7fc(s32 mode) {
  struct ResourceMenuWork *work;
  struct UiWindow *window;
  s32 msg;
  struct UiWindow **upper;
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
    window = UiWindow_Create(8, 0, 14, 4, 2);
    msg = (s32)MsgPasswordEntryX;
    upper = &work->upperWindow;
    *upper = window;
    UiText_DrawResource(msg, (s32)window, 0, 4);
    window = UiWindow_Create(4, 4, 22, 12, 2);
    work->lowerWindow = window;
    UiText_DrawResource(msg + 1, (s32)window, 0, 4);
    msg += 2;
    UiText_DrawResource(msg, (s32)work->lowerWindow, 0, 16);
  } else {
    Task_Add(Menu_DrawModeIndicator, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = UiWindow_Create(6, 0, 18, 4, 2);
    upper = &work->upperWindow;
    *upper = window;
    /* FAKEMATCH: the one-pass block issues the message's pool load ahead
       of the two coordinates, as the native does (differ 6 without). */
    do {
    } while (0);
    UiText_DrawResource((s32)MsgTransferMethodX, (s32)window, 12, 4);
    work->lowerWindow = UiWindow_Create(5, 5, 21, 7, 2);
  }
  result = Menu_RunResourceSelectionLoop(0);
  if (mode != 0)
    Task_Remove(Menu_DrawModeLabel);
  else
    Task_Remove(Menu_DrawModeIndicator);
  RenderOutput_PrepareForRedraw(*upper);
  RenderOutput_PrepareForRedraw(work->lowerWindow);
  UiWork_Finalize(*upper, 2);
  UiWork_Finalize(work->lowerWindow, 2);
  WaitFrames(1);
  Menu_EndResourceSelection();
  return result;
}

// Near miss, differ 2 of 147 (tla-en, 2026-10-30): one reordered pair in the
// password case, instruction order only. The native moves the message to r0
// (adds r0, r5) before it stores the upper window; here the store comes
// first. In the sched2 dump the two tie (priority 70, three dependents
// each), so the store goes first by insn order; the message's move is the
// call's argument load, emitted at the call after every statement before
// it, so no statement order puts it ahead. Tried 2026-10-30, none closer:
// the window stored inside the call's argument, the call reading *upper
// (2), the store through upper set before Window_Create (49, 51); for the
// transfer case's pool load, without the one-pass block, a message
// temporary (51 shared, 4 block-local), *upper as the argument, and
// &MsgTransferMethodX[0] (4 each). Five minutes of permute (29,000
// candidates) found nothing closer. No empty asm before any one statement
// lowers it (2026-10-26).
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
    msg = (s32)MsgPasswordEntryX;
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage(msg, (s32)window, 0, 4);
    window = Window_Create(4, 4, 22, 12, 2);
    work->lowerWindow = window;
    Text_DrawMessage(msg + 1, (s32)window, 0, 4);
    msg += 2;
    Text_DrawMessage(msg, (s32)work->lowerWindow, 0, 16);
  } else {
    Task_Add(Menu_DrawModeIndicator, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = Window_Create(6, 0, 18, 4, 2);
    upper = &work->upperWindow;
    *upper = window;
    /* FAKEMATCH(one-pass): an empty one-pass block after the window's store
     * issues the message's pool load ahead of the two coordinates; as the
     * call alone, 4 of 147 instructions differ in tla-en. */
    do {
    } while (0);
    Text_DrawMessage((s32)MsgTransferMethodX, (s32)window, 12, 4);
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

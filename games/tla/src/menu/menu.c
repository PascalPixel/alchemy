//--------------------------------------------------------------------------
// confirm_selection.c
// Menu: select command icons, such as Yes and No.
//--------------------------------------------------------------------------

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
#include "text/msg_ids.h"

s32 Menu_CenterResourceEntries(s32, s32, s32);

// Icons are indices into ResourceId_CommandIcons.
// Show the selected command's name in a window.
//--------------------------------------------------------------------------
// Select centered icons 0x11-0x14 on row 17.
// selection: initial selection; return the selected position.
//--------------------------------------------------------------------------
s32 Menu_SelectEntry11To14(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x11);
  Menu_AppendResourceEntry(0x12);
  Menu_AppendResourceEntry(0x13);
  Menu_AppendResourceEntry(0x14);
#if defined(TBS_EDITION_DE) || defined(TLA_EDITION_DE)
  Menu_CenterResourceEntries(17, 8, 0); // Name window width.
#elif EDITION_INTERNATIONAL
  Menu_CenterResourceEntries(17, 7, 0);
#else
  Menu_CenterResourceEntries(17, 6, 0);
#endif
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

//--------------------------------------------------------------------------
// Select centered icons 0x19-0x1c on row 17.
//--------------------------------------------------------------------------
s32 Menu_SelectEntry19To1c(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x19);
  Menu_AppendResourceEntry(0x1A);
  Menu_AppendResourceEntry(0x1B);
  Menu_AppendResourceEntry(0x1C);
#if EDITION_INTERNATIONAL
  Menu_CenterResourceEntries(17, 10, 0);
#else
  Menu_CenterResourceEntries(17, 8, 0);
#endif
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

//--------------------------------------------------------------------------
// Select Yes/No (icons 5 and 6). Cancel returns 1 (No).
// low: row 0 if zero, otherwise row 17.
// message: first name message, or 0 for command names.
// w: name window width, or 3 if zero.
// selection: initial selection.
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelection(s32 low, s32 message, s32 w, s32 selection) {
  s32 row;

  row = 0;

  AffineEffect_InitializeWork();

  if (w == 0) {
    w = 3;
  }
  if (low != 0) {
    row = 17;
  }

  Menu_AppendResourceEntry(5);
  Menu_AppendResourceEntry(6);
  Menu_CenterResourceEntries(row, w, message);

  selection = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  if (selection == -1) {
    selection = 1;
  }
  return selection;
}

//--------------------------------------------------------------------------
// Select Yes/No at (x, y). Cancel returns 1 (No).
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 selection) {
  s32 message = (s32)MsgCommandYes;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(5);
  Menu_AppendResourceEntry(6);
  Menu_LayoutResourceEntries(x, y, 3, message);
  selection = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  if (selection == -1) {
    selection = 1;
  }
  return selection;
}

//--------------------------------------------------------------------------

// select_entry_20_to_21.c

// Menu: select between command icons 0x20 and 0x21.

//--------------------------------------------------------------------------

// Name window width for each edition.
#if !EDITION_INTERNATIONAL
#define PAIR_WIDTH 7
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) ||                    \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

//--------------------------------------------------------------------------
// Select centered icons 0x20 and 0x21 on row 17.
// selection: initial selection; return the selected position.
//--------------------------------------------------------------------------
s32 Menu_SelectEntry20To21(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x20);
  Menu_AppendResourceEntry(0x21);
  Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

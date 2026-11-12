#include "types.h"
#include "heap_release.h"
#include "dma.h"
#include "task.h"
#include "io_reg.h"
#include "resource.h"
#include "ram_buffer.h"
#include "system.h"
#include "scene.h"
#include "uiflow.h"

#define AFFINE_EFFECT_SLOT 232
void Func_0804cda8(void);

void *AffineEffect_InitializeWork(void) {
  void *work = HeapSlot_AllocEwram(AFFINE_EFFECT_SLOT, 152);

  Dma_Fill32(0, work, 152);
  Task_Add((TaskFunc)Func_0804cda8, TASK_PASS_VBLANK - 10);
  return work;
}

void Menu_EndResourceSelection(void) {
  struct Window *child;
  s32 i;
  u16 *entry;
  void *work;

  work = Ram_HeapSlots->menuSelectWork;
  Task_Remove(Func_0804cda8);
  child = FIELD_AT_OFFSET(work, struct Window **, 0x78);
  if (child != 0) {
    Window_Close(child, 2);
  }
  i = 0;
  while (i < (s32)FIELD_AT_OFFSET(work, s16 *, 0x8E)) {
    entry = (u16 *)((u8 *)work + 0x12) + i * 10;
    Resource_ResetEntry(*entry);
    i += 1;
  }
  HeapSlot_Release(232);
  wait(1U);
}

#include "resource_menu.h"
#include "input.h"
#include "text/msg_ids.h"
#include "text_render.h"

void Sound_PlayFar(s32 cue);
void WindowSprite_PrepareForRedraw(struct Window *);

s32 Menu_RunResourceSelectionLoop(s32 initial) {
  struct ResourceMenuWork *work = Ram_HeapSlots->menuSelectWork;
  s32 resource;

  work->selection = initial;
redraw:
  WindowSprite_PrepareForRedraw(work->window);
  if (work->resourceBase != 0)
    resource = work->resourceBase + work->selection;
  else
    resource = work->resourceIds[work->selection] + (s32)MsgCommandName;
  Text_DrawCharacterAtOffset(resource, work->window, 0, 0);
  for (;;) {
    wait(1);
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(112);
      return work->selection;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(113);
      return -1;
    }
    if (gInput.pressed & KEY_START) {
      Sound_PlayFar(113);
      return -1;
    }
    if ((gInput.repeat & KEY_LEFT) || (gInput.repeat & KEY_UP)) {
      Sound_PlayFar(111);
      work->selection--;
      if (work->selection < 0)
        work->selection = work->count - 1;
    } else if ((gInput.repeat & KEY_RIGHT) || (gInput.repeat & KEY_DOWN)) {
      Sound_PlayFar(111);
      work->selection++;
      if (work->selection >= work->count)
        work->selection = 0;
    } else
      continue;
    goto redraw;
  }
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"

/// The menu sounds.
// Named as The Broken Seal's sounds.h names them (SE_MENU_*).
#define SOUND_MENU_CURSOR_MOVE 111
#define SOUND_MENU_CONFIRM 112

struct MenuSelectionState {
  u8 padding000[0x78];
  void *work;
  u8 padding07c[8];
  u8 resourceIds[8];
  s16 selection;
  s16 itemCount;
  s16 field090;
  s16 resourceBase;
};

extern u8 gMenu_SelectionStepDelays[];

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs) {
  if (diff >= 0)
    return diff;
  return rhs - lhs;
}

/// Steps the selection from start to goal, redrawing the label each step and
/// slowing as it nears the goal, then confirms.
// ☀️'s function of the same name in window/battle_command.c.
s32 Menu_SelectResource(s32 start, s32 goal) {
  s16 resourceBase;
  s16 cur;
  s32 diff;
  s32 resourceId;
  s32 dist;
  s32 pos;
  s32 step;
  s32 delay;
  const u8 *table;
  struct MenuSelectionState *state;

  state = Ram_HeapSlots->menuSelectWork;
  step = 1;
  delay = 12;
  state->selection = (s16)start;
  if (goal < start)
    step = -1;
  pos = start;

  for (;;) {
    WindowSprite_PrepareForRedraw(state->work);
    resourceBase = state->resourceBase;
    if (resourceBase != 0) {
      resourceId = resourceBase + state->selection;
    } else {
      resourceId = state->resourceIds[state->selection] + (s32)MsgCommandName;
    }
    Text_DrawCharacterAtOffset(resourceId, state->work, 0, 0);

    cur = state->selection;
    table = gMenu_SelectionStepDelays;
    diff = cur - goal;
    dist = AbsoluteDifference(diff, cur, goal);
    wait(table[dist] + delay);

    if (pos == goal)
      break;

    state->selection = (s16)((u16)state->selection + step);
    Sound_PlayFar(SOUND_MENU_CURSOR_MOVE);
    delay = 0;
    pos += step;
  }

  wait(48);
  Sound_PlayFar(SOUND_MENU_CONFIRM);
  return goal;
}

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style);

void Menu_LoadResourceSlot(s32 slot, s32 index) {
  s32 size = 1024;
  void *buffer = (void *)Heap_Alloc(size);
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  Resource_DecodeByteLzInRam((void *)((u32)base + base[index]), buffer);
  VramCache_Load(slot, size, buffer);
  Heap_Release(buffer);
}

void Menu_AppendResourceEntry(s32 no) {
  struct ResourceMenuWork *work;
  struct ResourceMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 flags;

  work = Ram_HeapSlots->menuSelectWork;
  index = work->count;
  if (index <= 5) {
    work->count++;
    entry = &work->entries[index];
    slot = Resource_FindFreeEntry();
    Menu_LoadResourceSlot(slot, no);
    entry->x = index * 24 + 32;
    flags = 136;
    entry->y = flags;
    entry->slot = slot;
    work->resourceIds[index] = no;
  }
}

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *menu = Ram_HeapSlots->menuSelectWork;
  s32 x;
  s32 i;
  s32 count;

  width += 2;
  menu->width = width;
  menu->resourceBase = resourceBase;
  menu->row = row;
  count = menu->count;
  x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
  for (i = 0; i < menu->count; i++) {
    struct ResourceMenuEntry *entry = &menu->entries[i];

    entry->x = x * 8;
    entry->y = row * 8;
    x += 3;
  }
  menu->window = Window_Create(x, row, menu->width, 3, 2);
}

#include "tla_edition.h"

#include "edition.h"

/// Lays the entries out in a row three cells apart from x, on row y, and
/// opens the menu's window after them.
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *work;
  s32 i;
  struct ResourceMenuEntry *entry;
  s16 *widthp;

  work = Ram_HeapSlots->menuSelectWork;
  widthp = &work->width;
  *widthp = width + 2;
  work->resourceBase = resourceBase;
  work->row = y;
  for (i = 0; i < work->count; i++) {
    entry = &work->entries[i];
    entry->y = y << 3;
    entry->x = x << 3;
    x += 3;
  }
  work->window = Window_Create(x, y, *widthp, 3, 2);
}

static __inline__ s32 TableGet(s8 *table, s32 index) { return table[index]; }

s32 Menu_SelectTopEntry(s32 selection) {
  s32 group = 0;
  s32 offset;
  s32 triple;
  s32 ret;
  s8 *table;

  if (Party_SumDjinnCountsFar(-1) == 0) {
    group = 1;
  }

  triple = group * 3;
  table = gMenu_TopEntryPositionByCommand;
  offset = triple << 1;
  selection = TableGet(table, selection + offset) - 1;
  if (selection < 0) {
    selection = 0;
  }

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  if (group == 0) {
    Menu_AppendResourceEntry(15);
  }
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();

  if (ret >= 0) {
    s8 *commands = gMenu_TopEntryCommandByPosition;
    s32 index;

    index = ret + offset + 1;
    ret = commands[index];
  }

  return ret;
}

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1) {
  PartyStatus_OpenAndWait();
  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  Menu_AppendResourceEntry(0xF);
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  // Only Japanese centres the entries before selecting.
#if LANGUAGE == LANGUAGE_JAPANESE
  Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
#endif
  arg1 = Menu_SelectResource(arg0, arg1 - 1);
  Menu_EndResourceSelection();
  Window_CloseAndRelease();
  return arg1;
}

extern s16 gData_020036d8;
extern s16 gData_02005350;
extern s8 gMenu_SaveSlotActionByPosition[];

s32 Func_080432a4(void);

/// Offers the actions the files allow (plus two optional entries
/// behind EWRAM flags) and maps the chosen position to an action. The second
/// optional entry is 31, and with the first option off, action 4 becomes 5.
// As ☀️'s Menu_SelectSaveSlotAction. A module of its own: in the Japanese
// edition it follows Menu_AnimateSelectionToEntry, which is still scaffolded
// there.
s32 Menu_SelectSaveSlotAction(void) {
  s32 type;
  s32 ret;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  type = Func_080432a4();
  if (type < 0)
    return -1;
  if (type == 0)
    return 0;
  if (type == 3)
    group = 1;
  else if (type == 103)
    group = 2;
  else if (type > 100)
    group = 3;
  else
    initial = 1;
  AffineEffect_InitializeWork();
  if (group == 0 || group == 3)
    Menu_AppendResourceEntry(21);
  if (group <= 1)
    Menu_AppendResourceEntry(22);
  if (group == 0 || group == 3)
    Menu_AppendResourceEntry(23);
  Menu_AppendResourceEntry(24);
  if (gData_020036d8 != 0)
    Menu_AppendResourceEntry(29);
  if (gData_02005350 != 0)
    Menu_AppendResourceEntry(31);
  Menu_CenterResourceEntries(17, SAVE_SLOT_MENU_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(initial);
  Menu_EndResourceSelection();
  if (ret >= 0) {
    u32 twice = group * 2;

    ret = gMenu_SaveSlotActionByPosition[ret + (twice + group) * 2];
  }
  if (gData_020036d8 == 0 && ret == 4)
    ret = 5;
  return ret;
}

// The label area is narrower in Japanese and wider in French.
#if LANGUAGE == LANGUAGE_JAPANESE
#define LABEL_RIGHT 80
#elif LANGUAGE == LANGUAGE_FRENCH
#define LABEL_RIGHT 160
#else
#define LABEL_RIGHT 128
#endif

struct MenuModeLabelState {
  u8 unk00[0x7c];
  void *window;
  u8 unk80[0x0c];
  s16 mode;
  u8 unk8e[8];
  s16 previousMode;
};

void Window_ClearInteriorTiles(void *window, s32 x, s32 y, s32 right,
                               s32 bottom);

/// When the selected mode changes, clears the lower window's label area and
/// writes the mode's lines of text. The area starts at x 8 and ends at
/// LABEL_RIGHT; a mode past 2 draws nothing.
// ☀️'s function of the same name.
void Menu_DrawModeLabel(void) {
  struct MenuModeLabelState *state = Ram_HeapSlots->menuSelectWork;

  if (state->previousMode != state->mode) {
    state->previousMode = state->mode;
    Window_ClearInteriorTiles(state->window, 8, 40, LABEL_RIGHT, 80);

    switch (state->mode) {
    case 0: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawCharacterAtOffset(text, state->window, 8, 40);
      Text_DrawCharacterAtOffset(text + 1, state->window, 8, 48);
      Text_DrawCharacterAtOffset(text + 2, state->window, 8, 56);
      Text_DrawCharacterAtOffset(text + 3, state->window, 8, 64);
      text += 4;
      Text_DrawCharacterAtOffset(text, state->window, 8, 72);
      break;
    }
    case 1: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawCharacterAtOffset(text, state->window, 8, 40);
      Text_DrawCharacterAtOffset(text + 1, state->window, 8, 48);
      text += 2;
      Text_DrawCharacterAtOffset(text, state->window, 8, 56);
      break;
    }
    case 2: {
      s32 text = (s32)MsgPasswordLevel;

      Text_DrawCharacterAtOffset(text++, state->window, 8, 40);
      Text_DrawCharacterAtOffset(text, state->window, 8, 48);
      break;
    }
    }
  }
}

void Text_DrawMessage(s32 message, void *window, s32 x, s32 y);

// The Spanish descriptions are indented.
#if LANGUAGE == LANGUAGE_SPANISH
#define HELP_X 6
#else
#define HELP_X 0
#endif

/// Redraws the selected method's description in the lower window. Spanish,
/// French and Italian describe two methods, the password and everything else
/// as the cable; French clears the window with WindowSprite_ClearList and
/// redraws only when the description changes.
void Menu_DrawModeIndicator(void) {
  struct ResourceMenuWork *state = Ram_HeapSlots->menuSelectWork;
  s16 *shown = &state->previousMode;
  s16 *current = &state->selection;

#if LANGUAGE == LANGUAGE_FRENCH
  if (*shown != *current) {
    if (*current == 0) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, 8, 4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow, 0,
                       16);
    } else if (*shown == 0) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage((s32)MsgCableTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWindow, 0,
                       16);
      Text_DrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWindow, 0,
                       28);
    }
    // Through the pointers, the selection would be loaded once at the top.
    state->previousMode = state->selection;
  }
#else
  if (*shown != *current) {
    *shown = *current;
    WindowSprite_PrepareForRedraw(state->lowerWindow);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
    if (*current == 0) {
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, HELP_X,
                       4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow,
                       HELP_X, 16);
    } else {
      Text_DrawMessage((s32)MsgCableTransferHelp, state->lowerWindow, HELP_X,
                       4);
      Text_DrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWindow,
                       HELP_X, 16);
      Text_DrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWindow,
                       HELP_X, 28);
    }
#else
    switch (*current) {
    case 0:
      Text_DrawMessage((s32)MsgPasswordTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWindow, 0,
                       16);
      break;
    case 1:
      Text_DrawMessage((s32)MsgCableTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWindow, 0,
                       16);
      Text_DrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWindow, 0,
                       28);
      break;
    default:
      Text_DrawMessage((s32)MsgClearDataTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage((s32)MsgClearDataTransferHelp + 1, state->lowerWindow, 0,
                       16);
      break;
    }
#endif
  }
#endif
}

#include "minigame.h"

// The entries' width and the windows' places and widths, in tiles, and where
// the lines of text start, in pixels, are fitted to each language's text.
#if LANGUAGE == LANGUAGE_JAPANESE
#define ENTRY_WIDTH 5
#define PASSWORD_UPPER_X 9
#define PASSWORD_UPPER_WIDTH 13
#define PASSWORD_LOWER_X 5
#define PASSWORD_LOWER_WIDTH 21
#define TRANSFER_UPPER_X 9
#define TRANSFER_UPPER_WIDTH 13
#define TRANSFER_TITLE_X 0
#define TRANSFER_LOWER_X 5
#define TRANSFER_LOWER_WIDTH 21
#elif LANGUAGE == LANGUAGE_GERMAN
#define ENTRY_WIDTH 7
#define PASSWORD_UPPER_X 7
#define PASSWORD_UPPER_WIDTH 16
#define PASSWORD_LOWER_X 4
#define PASSWORD_LOWER_WIDTH 22
#define TRANSFER_UPPER_X 6
#define TRANSFER_UPPER_WIDTH 18
#define TRANSFER_TITLE_X 6
#define TRANSFER_LOWER_X 2
#define TRANSFER_LOWER_WIDTH 26
#elif LANGUAGE == LANGUAGE_SPANISH
#define ENTRY_WIDTH 9
#define PASSWORD_UPPER_X 5
#define PASSWORD_UPPER_WIDTH 21
#define PASSWORD_LOWER_X 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X 4
#define TRANSFER_UPPER_WIDTH 22
#define TRANSFER_TITLE_X 2
#define TRANSFER_LOWER_X 1
#define TRANSFER_LOWER_WIDTH 28
#elif LANGUAGE == LANGUAGE_FRENCH
#define ENTRY_WIDTH 9
#define PASSWORD_UPPER_X 5
#define PASSWORD_UPPER_WIDTH 20
#define PASSWORD_LOWER_X 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X 5
#define TRANSFER_UPPER_WIDTH 20
#define TRANSFER_TITLE_X 2
#define TRANSFER_LOWER_X 1
#define TRANSFER_LOWER_WIDTH 28
#elif LANGUAGE == LANGUAGE_ITALIAN
#define ENTRY_WIDTH 7
#define PASSWORD_UPPER_X 7
#define PASSWORD_UPPER_WIDTH 17
#define PASSWORD_LOWER_X 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X 5
#define TRANSFER_UPPER_WIDTH 20
#define TRANSFER_TITLE_X 2
#define TRANSFER_LOWER_X 1
#define TRANSFER_LOWER_WIDTH 28
#else
#define ENTRY_WIDTH 7
#define PASSWORD_UPPER_X 8
#define PASSWORD_UPPER_WIDTH 14
#define PASSWORD_LOWER_X 4
#define PASSWORD_LOWER_WIDTH 22
#define TRANSFER_UPPER_X 6
#define TRANSFER_UPPER_WIDTH 18
#define TRANSFER_TITLE_X 12
#define TRANSFER_LOWER_X 5
#define TRANSFER_LOWER_WIDTH 21
#endif

// Spanish, French and Italian indent the password's lines and set them
// closer together.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define PASSWORD_TEXT_X 8
#define PASSWORD_TEXT_Y 0
#define PASSWORD_LINE_HEIGHT 11
#else
#define PASSWORD_TEXT_X 0
#define PASSWORD_TEXT_Y 4
#define PASSWORD_LINE_HEIGHT 12
#endif

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
  Menu_CenterResourceEntries(17, ENTRY_WIDTH, 0);
  if (mode != 0) {
    Task_Add(Menu_DrawModeLabel, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = Window_Create(PASSWORD_UPPER_X, 0, PASSWORD_UPPER_WIDTH, 4, 2);
    msg = (s32)MsgPasswordEntry;
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage(msg, window, 0, 4);
    window = Window_Create(PASSWORD_LOWER_X, 4, PASSWORD_LOWER_WIDTH, 12, 2);
    work->lowerWindow = window;
    Text_DrawMessage(msg + 1, window, PASSWORD_TEXT_X, PASSWORD_TEXT_Y);
#if LANGUAGE == LANGUAGE_FRENCH
    // French writes a third line: "Niveaux" (MsgPasswordLevel).
    Text_DrawMessage(msg + 2, work->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT);
    msg += 3;
    Text_DrawMessage(msg, work->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT * 2);
#else
    msg += 2;
    Text_DrawMessage(msg, work->lowerWindow, PASSWORD_TEXT_X,
                     PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT);
#endif
  } else {
    Task_Add(Menu_DrawModeIndicator, TASK_PASS_VBLANK - 10);
    work->previousMode = 0xffff;
    window = Window_Create(TRANSFER_UPPER_X, 0, TRANSFER_UPPER_WIDTH, 4, 2);
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage((s32)MsgTransferMethod, window, TRANSFER_TITLE_X, 4);
    work->lowerWindow =
        Window_Create(TRANSFER_LOWER_X, 5, TRANSFER_LOWER_WIDTH, 7, 2);
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

// Menu: select command icons, such as Yes and No.

/// Selects a shop command from icons 0x11-0x14, centred on row 17, showing
/// the selected command's name in a window. Icons are indices into
/// ResourceId_CommandIcons.
/// selection: initial selection. Returns the selected position.
s32 Menu_SelectShopCommand(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x11);
  Menu_AppendResourceEntry(0x12);
  Menu_AppendResourceEntry(0x13);
  Menu_AppendResourceEntry(0x14);
#if (LANGUAGE == LANGUAGE_GERMAN)
  Menu_CenterResourceEntries(17, 8, 0); // Name window width.
#elif (LANGUAGE != LANGUAGE_JAPANESE)
  Menu_CenterResourceEntries(17, 7, 0);
#else
  Menu_CenterResourceEntries(17, 6, 0);
#endif
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

/// Selects a sanctum service from icons 0x19-0x1c, centred on row 17.
s32 Menu_SelectSanctumService(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x19);
  Menu_AppendResourceEntry(0x1A);
  Menu_AppendResourceEntry(0x1B);
  Menu_AppendResourceEntry(0x1C);
#if (LANGUAGE != LANGUAGE_JAPANESE)
  Menu_CenterResourceEntries(17, 10, 0);
#else
  Menu_CenterResourceEntries(17, 8, 0);
#endif
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

/// Selects Yes or No (icons 5 and 6). Cancel returns 1 (No).
/// low: row 0 if zero, otherwise row 17.
/// message: first name message, or 0 for command names.
/// w: name window width, or 3 if zero.
/// selection: initial selection.
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

/// Selects Yes or No at (x, y). Cancel returns 1 (No).
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
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PAIR_WIDTH 7
#elif (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

/// Selects coins or medals from icons 0x20 and 0x21, centred on row 17.
/// selection: initial selection. Returns the selected position.
s32 Menu_SelectCoinsOrMedals(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x20);
  Menu_AppendResourceEntry(0x21);
  Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

void Menu_RunSelection(void);

void Func_0804dabc(void) { Task_Add(Menu_RunSelection, TASK_PASS_VBLANK); }

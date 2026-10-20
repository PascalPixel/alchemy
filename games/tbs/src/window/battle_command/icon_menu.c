/// @file
/// The command icon menus of the field, title and transfer screens.
// Part of battle_command.c.

#include "../battle_command_internal.h"

#if LANGUAGE == LANGUAGE_GERMAN
#define TRANSFER_MODE_DRAW_HELP UiText_DrawCharacterAtOffset
#define TRANSFER_PASSWORD_HELP_X 32
#define TRANSFER_PASSWORD_HELP_FIRST_Y 8
#define TRANSFER_PASSWORD_HELP_LAST_Y 24
#define TRANSFER_CABLE_HELP_FIRST_Y 0
#define TRANSFER_CABLE_HELP_LAST_Y 32
#else
#define TRANSFER_MODE_DRAW_HELP UiText_DrawResource
#define TRANSFER_PASSWORD_HELP_X PASSWORD_HELP_X
#define TRANSFER_PASSWORD_HELP_FIRST_Y 4
#define TRANSFER_PASSWORD_HELP_LAST_Y 16
#define TRANSFER_CABLE_HELP_FIRST_Y 4
#define TRANSFER_CABLE_HELP_LAST_Y 28
#endif

/// One line's horizontal window edges: window 0's and window 1's, each
/// (left << 8 | right).
struct WindowLine {
  u16 left; ///< window 0's edges, for WIN0H
  u16 span; ///< window 1's edges, for WIN1H
};

/// A page of the display transition's window edges: WININ and WINOUT, then
/// each of the 160 lines' edges.
struct WindowBuffer {
  u8 unk000[4]; ///< WININ and WINOUT, not touched here
  struct WindowLine lines[160];
};

/// Draws the resource-menu icons, pulsing the selected icon with one affine
/// matrix, and updates the horizontal window spans to reveal the active icon
/// row. A scheduler callback of the command icon menus
/// (AffineEffect_InitializeWork). Each icon is a 32x32, 256-colour sprite;
/// the selected one is double-size affine, pulsing between 1.19 and 1.31
/// times, on layer 246, the others on 245, and one at x 0 is not drawn.
/// While a window transition runs (gDisplayTransition), it widens window 1
/// over the icon row so the icons show through: from the first icon on the
/// row's 24 lines, keeping each line's right edge, and around the selected
/// icon on the lines it overhangs (below a row at the top, above one at the
/// bottom, whose lines are 136 to 159).
void AffineEffect_UpdateFrame(void) {
  // UB: native pointers traverse scalar link/span fields into adjacent fields
  // and records. Use their real owners under UBFIX. Plain OAM field accesses
  // and indexed scanlines each shrink the EN body from 0x348 to 0x344 bytes.
  struct ResourceMenuWork *menu;
  struct ResourceMenuEntry *entry;
  struct AffineTransform transform;
  s32 pulseScale;
  s32 affineIndex;
  s32 packedSpan;

  s32 spriteY;
  u32 lastLine;
  u32 index;
#ifndef UBFIX
  u16 *spanCursor;
#endif
  struct DisplayTransitionState *transition;

  menu = gMenuSelectWork;
  entry = menu->entries;
  pulseScale = gUi_BounceScales[(gFrameCount * 2) & 31];
  pulseScale = (pulseScale - 256) / 4 + 304;
  transform.scaleX = pulseScale;
  transform.scaleY = pulseScale;
  transform.angle = 0;
  affineIndex = AffineMatrix_BuildForEffect(&transform);
  // Pulse only the selection; the other icons keep their normal OAM shape.
  for (index = 0; index < menu->count; index++, entry++) {
#ifndef UBFIX
    u32 *nativeAttributes;
#endif
    s32 drawLayer;
    s32 x;

    x = entry->x;
    if (x == 0)
      continue;
    if (index == menu->selection) {
      s32 left = x + pulseScale * 7 / 512 - 20;

      // On the top row the double-size box starts above the screen: its y
      // wraps within OAM's 8 bits.
      if (entry->y != 0)
        spriteY = entry->y + pulseScale * 3 / 256 - 20;
      else
        spriteY = (pulseScale * 15 / 256 - 30) & 255;
#ifdef UBFIX
      entry->link = 0;
      entry->attr01 = (affineIndex << 25) | spriteY | (left << 16) | 0x80002300;
      entry->attr2 = gVramBlockCache[entry->slot].offset >> 5;
#else
      nativeAttributes = &entry->link;
      *nativeAttributes++ = 0;
      *nativeAttributes++ =
          (affineIndex << 25) | spriteY | (left << 16) | 0x80002300;
      *nativeAttributes = gVramBlockCache[entry->slot].offset >> 5;
#endif
      drawLayer = 246;
    } else {
      u32 vramOffset;
      u32 packedAttributes;

#ifndef UBFIX
      nativeAttributes = &entry->link;
      // Read through the attribute pointer, y keeps the pointer copy before
      // its load.
      spriteY = ((s16 *)nativeAttributes)[7];
      *nativeAttributes++ = 0;
#else
      spriteY = entry->y;
      entry->link = 0;
#endif
      packedAttributes = spriteY | (x << 16) | 0x80002000;
      vramOffset = gVramBlockCache[entry->slot].offset;
#ifdef UBFIX
      entry->attr01 = packedAttributes;
      entry->attr2 = vramOffset >> 5;
#else
      *nativeAttributes++ = packedAttributes;
      *nativeAttributes = vramOffset >> 5;
#endif
      drawLayer = 245;
    }
    Runtime_PushSlotEntry(entry, drawLayer);
  }
  // The top and bottom rows reveal opposite sides of the selected icon.
  if (menu->row == 0) {
    struct WindowBuffer *page;

    s32 halfWidth;

    transition = (struct DisplayTransitionState *)gDisplayTransition;
    if (transition == 0 || menu->count == 0)
      return;
    page = (struct WindowBuffer *)transition->pages[transition->page];
    packedSpan = menu->entries[menu->selection].x;
    halfWidth = (pulseScale * 12 - 2817) / 256;
    packedSpan =
        ((packedSpan - halfWidth) << 8) + (packedSpan + halfWidth) + 23;
    lastLine = menu->entries[menu->selection].y + 24;
    lastLine += (pulseScale * 32 - 7937) / 512 + 1;
#ifndef UBFIX
    spanCursor = &page->lines[24].span;
#endif
    index = 24;
    if (index < lastLine) {
      do {
#ifdef UBFIX
        page->lines[index].span = (page->lines[index].span & 0xff) | packedSpan;
#else
        *spanCursor = (*spanCursor & 0xff) | packedSpan;
#endif
        index++;
#ifndef UBFIX
        spanCursor += 2;
#endif
      } while (index < lastLine);
    }
    packedSpan = menu->entries[0].x;
    if (menu->selection == 0)
      packedSpan -= (pulseScale * 12 - 2817) / 256;
    packedSpan <<= 8;
#ifndef UBFIX
    spanCursor = &page->lines[0].span;
#endif
    for (index = 0; index <= 23; index++) {
#ifdef UBFIX
      page->lines[index].span = (page->lines[index].span & 0xff) | packedSpan;
#else
      *spanCursor = (*spanCursor & 0xff) | packedSpan;
#endif
#ifndef UBFIX
      spanCursor += 2;
#endif
    }
  } else {
    struct WindowBuffer *page;
    s32 firstLine;

    s32 halfWidth;

    transition = (struct DisplayTransitionState *)gDisplayTransition;
    if (transition == 0 || menu->count == 0)
      return;
    page = (struct WindowBuffer *)transition->pages[transition->page];
    packedSpan = menu->entries[menu->selection].x;
    halfWidth = (pulseScale * 12 - 2817) / 256;
    packedSpan =
        ((packedSpan - halfWidth) << 8) + (packedSpan + halfWidth) + 23;
    firstLine = menu->entries[menu->selection].y;
    firstLine -= (pulseScale * 32 - 7937) / 512 + 1;
#ifndef UBFIX
    spanCursor = &page->lines[firstLine].span;
#endif
    index = firstLine;
    if (index <= 135) {
      do {
#ifdef UBFIX
        page->lines[index].span = (page->lines[index].span & 0xff) | packedSpan;
#else
        *spanCursor = (*spanCursor & 0xff) | packedSpan;
#endif
        index++;
#ifndef UBFIX
        spanCursor += 2;
#endif
      } while (index <= 135);
    }
    packedSpan = menu->entries[0].x;
    if (menu->selection == 0)
      packedSpan -= (pulseScale * 12 - 2817) / 256;
    packedSpan <<= 8;
#ifndef UBFIX
    spanCursor = &page->lines[136].span;
#endif
    for (index = 136; index <= 159; index++) {
#ifdef UBFIX
      page->lines[index].span = (page->lines[index].span & 0xff) | packedSpan;
#else
      *spanCursor = (*spanCursor & 0xff) | packedSpan;
#endif
#ifndef UBFIX
      spanCursor += 2;
#endif
    }
  }
}

/// Allocates and clears the resource-menu state, then schedules its icon pulse.
/// The work (struct ResourceMenuWork, 152 bytes) lives in heap slot 58 until
/// Menu_EndResourceSelection frees it; it is returned, and also reached as
/// gMenuSelectWork. Every command icon menu starts here.
void *AffineEffect_InitializeWork(void) {
  // The matching allocation size is the complete six-entry work block.
  void *work;
  work = Runtime_AllocateBlock(58, 152);
  Dma_Fill32(0, work, 152);
  Scheduler_AddOrUpdateCallback(AffineEffect_UpdateFrame, 0xc76);
  return work;
}

/// Stops the icon pulse, closes the label window and releases each loaded icon
/// before freeing the resource-menu state (heap slot 58, 0x3a), then waits a
/// frame.
void Menu_EndResourceSelection(void) {
  struct UiWindow *labelWindow;
  s32 i;
  struct ResourceMenuWork *work;

  work = gMenuSelectWork;
  Scheduler_RemoveCallback(AffineEffect_UpdateFrame);
  labelWindow = work->window;
  if (labelWindow != 0) {
    UiWork_Finalize(labelWindow, 2);
  }
  for (i = 0; i < work->count; i++) {
    Resource_ResetEntry(work->entries[i].slot);
  }
  Runtime_ReleaseHeapBlock(0x3A);
  WaitFrames(1U);
}

/// Selects an icon position with wrapped directional input. A returns the
/// position; B or Start returns -1. Redraws the label only after a move. The
/// label is message resourceBase + position when the layout set a
/// resourceBase, else the icon's command name (MsgCommandName + its icon).
s32 Menu_RunResourceSelectionLoop(s32 initial) {
  struct ResourceMenuWork *work = gMenuSelectWork;
  s32 resource;

  work->selection = initial;
redraw:
  RenderOutput_PrepareForRedraw(work->window);
  if (work->resourceBase != 0)
    resource = work->resourceBase + work->selection;
  else
    resource = work->resourceIds[work->selection] + MsgCommandName;
  UiText_DrawCharacterAtOffset(resource, work->window, 0, 0);
  for (;;) {
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      return work->selection;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      return -1;
    }
    if (gKeysPressed & KEY_START) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      return -1;
    }
    if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP)) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      work->selection--;
      if (work->selection < 0)
        work->selection = work->count - 1;
    } else if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN)) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      work->selection++;
      if (work->selection >= work->count)
        work->selection = 0;
    } else
      continue;
    goto redraw;
  }
}

/// Returns the distance between lhs and rhs, reusing their supplied difference.
static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs) {
  if (diff >= 0)
    return diff;
  return rhs - lhs;
}

/// Animates a cursor from start to goal with table-driven delays. Draws each
/// intervening label, pauses at the destination, and returns its position.
/// It moves one icon at a time with the cursor sound, waiting
/// gMenu_SelectionStepDelays[icons still to go] frames at each (12 more at
/// the first), then 48 frames on the goal before the confirm sound: a menu
/// choice made for the player to watch (Menu_AnimateSelectionToEntry).
s32 Menu_SelectResource(s32 start, s32 goal) {
  s16 resourceBase;
  s16 selection;
  s32 diff;
  s32 resourceId;
  s32 distance;
  s32 position;
  s32 step;
  s32 delay;
  const u8 *stepDelays;
  struct ResourceMenuWork *state;

  state = gMenuSelectWork;
  step = 1;
  delay = 12;
  state->selection = start;
  if (goal < start)
    step = -1;
  position = start;

  for (;;) {
    RenderOutput_PrepareForRedraw(state->window);
    resourceBase = state->resourceBase;
    if (resourceBase != 0) {
      resourceId = resourceBase + state->selection;
    } else {
      resourceId = state->resourceIds[state->selection] + MsgCommandName;
    }
    UiText_DrawCharacterAtOffset(resourceId, state->window, 0, 0);

    selection = state->selection;
    stepDelays = gMenu_SelectionStepDelays;
    diff = selection - goal;
    distance = AbsoluteDifference(diff, selection, goal);
    WaitFrames(stepDelays[distance] + delay);

    if (position == goal)
      break;

    state->selection += step;
    AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
    delay = 0;
    position += step;
  }

  WaitFrames(48);
  AudioCommand_PlayFar(SE_MENU_CONFIRM);
  return goal;
}

/// Decodes one command-icon table entry into the requested VRAM cache slot,
/// through a 1 KB block it allocates and frees.
void Menu_LoadResourceSlot(s32 slot, s32 index) {
  s32 size = 1024;
  void *buffer = MemAlloc(size);
  u16 *offsetTable = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  Resource_DecodeByteLz((void *)((u32)offsetTable + offsetTable[index]),
                        buffer);
  VramCache_Load(slot, size, buffer);
  MemFree(buffer);
}

/// Appends a command icon if fewer than six are present. Its initial position
/// is on the bottom icon row; a layout routine places the final row. The
/// icon's number (ResourceId_CommandIcons) is also its command name's
/// (MsgCommandName + icon). Its VRAM slot is taken unchecked.
void Menu_AppendResourceEntry(s32 icon) {
  struct ResourceMenuWork *work;
  struct ResourceMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 y;

  work = gMenuSelectWork;
  index = work->count;
  if (index <= 5) {
    work->count++;
    entry = &work->entries[index];
    slot = Resource_FindFreeEntry();
    Menu_LoadResourceSlot(slot, icon);
    entry->x = index * 24 + 32;
    y = 136;
    entry->y = y;
    entry->slot = slot;
    work->resourceIds[index] = icon;
  }
}

/// Centers the icons and their label window together on the given tile row.
/// A nonzero resourceBase supplies labels; otherwise command-name IDs do.
/// The icons stand three tiles apart and the label window, `width` + 2 tiles
/// wide and 3 tall, follows them; the centring counts two thirds of the
/// window's width.
void Menu_CenterResourceEntries(s32 row, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *menu = gMenuSelectWork;
  s32 x;
  s32 i;
  s32 count;

  menu->width = width + 2;
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
  menu->window = UiWindow_Create(x, row, menu->width, 3, 2);
}

/// Places icons at three-tile intervals from (x, y), then creates their label
/// window immediately to the right. Coordinates and label width are in tiles.
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *work;
  s32 i;
  struct ResourceMenuEntry *entry;

  work = gMenuSelectWork;
  work->width = width + 2;
  work->resourceBase = resourceBase;
  work->row = y;
  for (i = 0; i < work->count; i++) {
    entry = &work->entries[i];
    entry->y = y << 3;
    entry->x = x << 3;
    x += 3;
  }
  work->window = UiWindow_Create(x, y, work->width, 3, 2);
}

/// Reads the signed command-position mapping used by the top resource menu.
static __inline__ s32 ResourceMenu_ReadCommandPosition(s8 *table, s32 index) {
  return table[index];
}

/// Builds the field command menu, omitting Djinn when none are owned.
/// Translates between command IDs and visible icon positions using the two
/// lookup rows. The icons are Psynergy, Djinn, Item and Status on row 17;
/// `selection` is the command to start on and the result the command
/// chosen: 1 Psynergy, 2 Djinn, 3 Item, 4 Status, or -1 when cancelled. The
/// field's menu runs it (window/window/selection.c).
s32 Menu_SelectTopEntry(s32 selection) {
  s32 group = 0;
  s32 offset;
  s32 groupTriple;
  s32 result;
  s8 *commandPositions;

  if (Party_SumDjinnCountsFar(-1) == 0) {
    group = 1;
  }

  groupTriple = group * 3;
  commandPositions = gMenu_TopEntryPositionByCommand;
  offset = groupTriple << 1;
  selection =
      ResourceMenu_ReadCommandPosition(commandPositions, selection + offset) -
      1;
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
  result = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();

  if (result >= 0) {
    result = gMenu_TopEntryCommandByPosition[result + offset + 1];
  }

  return result;
}

#if TBS_PATCH_1
#define ANIMATE_ENTRIES_ROW 7
#else
#define ANIMATE_ENTRIES_ROW 5
#endif

/// Opens the field command menu and animates from start to the command's icon.
/// Returns the final icon position after closing the menu. The Djinn icon is
/// always shown, so `command` (1 to 4, as Menu_SelectTopEntry gives it) is
/// position command - 1. The world map's scenes show it picking Djinn
/// (scene/world_map.c).
s32 Menu_AnimateSelectionToEntry(s32 start, s32 command) {
  UiWindow_OpenMode1AndWaitFrame();
  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  Menu_AppendResourceEntry(0xF);
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  Menu_CenterResourceEntries(0x11, ANIMATE_ENTRIES_ROW, 0);
  command = Menu_SelectResource(start, command - 1);
  Menu_EndResourceSelection();
  UiWork_CloseAndRelease();
  return command;
}

/// Builds the title menu from the save summaries and optional send flags.
/// Translates the chosen visible position back to its save-action code:
/// 0 New Game, 1 Continue, 2 Copy, 3 Erase, 4 Battle, 5 Send, or -1 when
/// cancelled or the backup memory cannot be read. With no files it returns 0
/// at once. The icons, by SaveState_ScanRecordFlags' count: New Game,
/// Continue, Copy and Erase for one or two files (starting on Continue);
/// no New Game or Copy with three; with every file cleared no Continue, and
/// only Erase with three such files. Battle and Send follow when enabled.
/// XXX: with Send shown but not Battle, Send's place maps to 4 (Battle),
/// because gMenu_SaveSlotActionByPosition lists Battle before Send in every
/// row; whether a file can ever be cleared without any of flags 48-127 set
/// is not established. The file menu runs it (menu/clear.c).
s32 Menu_SelectSaveSlotAction(void) {
  s32 fileState;
  s32 result;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  fileState = SaveState_ScanRecordFlags();
  if (fileState < 0) {
    return -1;
  }
  if (fileState == 0) {
    return 0;
  }
  if (fileState == 3) {
    group = 1;
  } else if (fileState == 0x67) {
    group = 2;
  } else if (fileState > 0x64) {
    group = 3;
  } else {
    initial = 1;
  }
  AffineEffect_InitializeWork();
  if ((group == 0) || (group == 3)) {
    Menu_AppendResourceEntry(0x15);
  }
  if (group <= 1U) {
    Menu_AppendResourceEntry(0x16);
  }
  if ((group == 0) || (group == 3)) {
    Menu_AppendResourceEntry(0x17);
  }
  Menu_AppendResourceEntry(0x18);
  if ((*(s16 *)gTitleExtraOptionEnabled) != 0) {
    Menu_AppendResourceEntry(0x1D);
  }
  if (gTitleSendOptionEnabled != 0) {
    Menu_AppendResourceEntry(0x1E);
  }
  Menu_CenterResourceEntries(0x11, TYPE_MENU_WIDTH, 0);
  result = Menu_RunResourceSelectionLoop(initial);
  Menu_EndResourceSelection();
  if (result >= 0) {
    result = gMenu_SaveSlotActionByPosition[result + (group * 6)];
  }
  return result;
}

#if TBS_PATCH_1
#define MODE_LABEL_X 18
#define MODE_LABEL_RIGHT 144
#else
#define MODE_LABEL_X 8
#define MODE_LABEL_RIGHT 80
#endif

/// Redraws the password-level explanation when the selected level changes.
/// A scheduler callback of the password level menu: what each level's
/// password carries, from MsgPasswordLevel on: Gold (0) "Level", "Djinn",
/// "Character stats", "Items" and "Coins", Silver (1) the first three,
/// Bronze (2) the first two.
void Menu_DrawModeLabel(void) {
  struct ResourceMenuWork *state = gMenuSelectWork;

  if (state->previousMode != state->selection) {
    state->previousMode = state->selection;
    UiWindow_ClearInteriorTiles(state->lowerWindow, 8, 40, MODE_LABEL_RIGHT,
                                80);

    switch (state->selection) {
    case 0: {
      s32 text = MsgPasswordLevel;

      UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 40);
      UiText_DrawCharacterAtOffset(text + 1, state->lowerWindow, MODE_LABEL_X,
                                   48);
      UiText_DrawCharacterAtOffset(text + 2, state->lowerWindow, MODE_LABEL_X,
                                   56);
      UiText_DrawCharacterAtOffset(text + 3, state->lowerWindow, MODE_LABEL_X,
                                   64);
      text += 4;
      UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 72);
      break;
    }
    case 1: {
      s32 text = MsgPasswordLevel;

      UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 40);
      UiText_DrawCharacterAtOffset(text + 1, state->lowerWindow, MODE_LABEL_X,
                                   48);
      text += 2;
      UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 56);
      break;
    }
    case 2:
    default: {
      s32 text = MsgPasswordLevel;

      UiText_DrawCharacterAtOffset(text++, state->lowerWindow, MODE_LABEL_X,
                                   40);
      UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 48);
      break;
    }
    }
  }
}

#if TBS_PATCH_1
#define PASSWORD_HELP_X 16
#else
#define PASSWORD_HELP_X 0
#endif

#if (LANGUAGE == LANGUAGE_FRENCH)

/// Redraws password/cable transfer help when the chosen method changes.
/// The French layout keeps its distinct line splitting and redraw path. A
/// scheduler callback of the transfer method menu: Password (0) or Cable.
void Menu_DrawModeIndicator(void) {
  struct ResourceMenuWork *state = gMenuSelectWork;

  if (state->previousMode != state->selection) {
    if (state->selection == 0) {
      RenderOutput_ClearList(state->lowerWindow);
      UiText_DrawResource(MsgPasswordTransferHelp, state->lowerWindow, 16, 4);
      UiText_DrawResource(MsgPasswordTransferHelp + 1, state->lowerWindow, 16,
                          16);
    } else if (state->previousMode == 0) {
      RenderOutput_ClearList(state->lowerWindow);
      UiText_DrawResource(MsgCableTransferHelp, state->lowerWindow, 0, 4);
      UiText_DrawResource(MsgCableTransferHelp + 1, state->lowerWindow, 0, 16);
      UiText_DrawResource(MsgCableTransferHelp + 2, state->lowerWindow, 0, 28);
    }
    state->previousMode = state->selection;
  }
}
#else
/// Redraws password/cable transfer help when the chosen method changes. A
/// scheduler callback of the transfer method menu: Password (0) or Cable.
void Menu_DrawModeIndicator(void) {
  struct ResourceMenuWork *state = gMenuSelectWork;
  s16 *shown = &state->previousMode;
  s16 *current = &state->selection;

  if (*shown != *current) {
    *shown = *current;
    RenderOutput_PrepareForRedraw(state->lowerWindow);
    if (*current == 0) {
      TRANSFER_MODE_DRAW_HELP(MsgPasswordTransferHelp, state->lowerWindow,
                              TRANSFER_PASSWORD_HELP_X,
                              TRANSFER_PASSWORD_HELP_FIRST_Y);
      TRANSFER_MODE_DRAW_HELP(MsgPasswordTransferHelp + 1, state->lowerWindow,
                              TRANSFER_PASSWORD_HELP_X,
                              TRANSFER_PASSWORD_HELP_LAST_Y);
    } else {
      TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp, state->lowerWindow, 0,
                              TRANSFER_CABLE_HELP_FIRST_Y);
      TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp + 1, state->lowerWindow, 0,
                              16);
      TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp + 2, state->lowerWindow, 0,
                              TRANSFER_CABLE_HELP_LAST_Y);
    }
  }
}
#endif

/// Selects either the transfer method (mode 0) or password level. Schedules the
/// appropriate help redraw while the icon menu is active, then closes both
/// help windows and the icon menu. Mode 0 offers Password and Cable (icons
/// 44 and 45), mode 1 Gold, Silver and Bronze (46 to 48). Returns the place
/// chosen, or -1 when cancelled. The Send screens run it (menu/clear.c).
s32 Menu_SelectResourceLayout(s32 mode) {
  struct ResourceMenuWork *work;
  struct UiWindow *window;
  s32 message;
  struct UiWindow **upper;
  s32 result;

  AffineEffect_InitializeWork();
  work = gMenuSelectWork;
  if (mode == 0) {
    Menu_AppendResourceEntry(44);
    Menu_AppendResourceEntry(45);
  } else {
    Menu_AppendResourceEntry(46);
    Menu_AppendResourceEntry(47);
    Menu_AppendResourceEntry(48);
#if (LANGUAGE == LANGUAGE_SPANISH)
    Menu_CenterResourceEntries(17, 5, 0);
#endif
  }
#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
  Menu_CenterResourceEntries(17, 7, 0);
#elif (LANGUAGE == LANGUAGE_SPANISH)
#elif (LANGUAGE == LANGUAGE_FRENCH)
  Menu_CenterResourceEntries(17, 9, 0);
#else
  Menu_CenterResourceEntries(17, 5, 0);
#endif
  if (mode != 0) {
    Scheduler_AddOrUpdateCallback(Menu_DrawModeLabel, 0xc76);
    work->previousMode = 0xffff;
    window = UiWindow_Create(RESOURCE_MODE0_WINDOW_X, 0,
                             RESOURCE_MODE0_WINDOW_WIDTH, 4, 2);
    message = MsgPasswordSelection;
    upper = &work->upperWindow;
    *upper = window;
    UiText_DrawResource(message, window, 0, 4);
#if TBS_PATCH_1
    window = UiWindow_Create(RESOURCE_MODE1_WINDOW_X, 4,
                             RESOURCE_MODE1_WINDOW_WIDTH, 12, 2);
    work->lowerWindow = window;
    UiText_DrawResource(message + 1, window, 8, 0);
    UiText_DrawResource(message + 2, work->lowerWindow, 8, 11);
    message += 3;
    UiText_DrawResource(message, work->lowerWindow, 8, 22);
#else
    window = UiWindow_Create(5, 4, 21, 12, 2);
    work->lowerWindow = window;
    UiText_DrawResource(message + 1, window, 0, 4);
    message += 2;
    UiText_DrawResource(message, work->lowerWindow, 0, 16);
#endif
  } else {
    Scheduler_AddOrUpdateCallback(Menu_DrawModeIndicator, 0xc76);
    work->previousMode = 0xffff;
    window = UiWindow_Create(TRANSFER_METHOD_WINDOW_X, 0,
                             TRANSFER_METHOD_WINDOW_WIDTH, 4, 2);
    upper = &work->upperWindow;
    *upper = window;
    UiText_DrawResource(MsgTransferMethod, window, TRANSFER_METHOD_LABEL_X, 4);
    work->lowerWindow = UiWindow_Create(TRANSFER_HELP_WINDOW_X, 5,
                                        TRANSFER_HELP_WINDOW_WIDTH, 7, 2);
  }
  result = Menu_RunResourceSelectionLoop(0);
  if (mode != 0)
    Scheduler_RemoveCallback(Menu_DrawModeLabel);
  else
    Scheduler_RemoveCallback(Menu_DrawModeIndicator);
  RenderOutput_PrepareForRedraw(*upper);
  RenderOutput_PrepareForRedraw(work->lowerWindow);
  UiWork_Finalize(*upper, 2);
  UiWork_Finalize(work->lowerWindow, 2);
  WaitFrames(1);
  Menu_EndResourceSelection();
  return result;
}

// Command icons arranged in a row to choose from, such as Yes and No.
// Icon numbers refer to ResourceId_CommandIcons.
// Each shows the name of the icon under the cursor in a window.

/// Selects a shop command, icons 0x11-0x14 (Buy, Sell, Artifacts, Repair),
/// centered on row 17, starting on position `selection`. Returns the
/// position chosen, or -1 when cancelled.
s32 Menu_SelectShopCommand(s32 selection) {
  s32 result;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x11);
  Menu_AppendResourceEntry(0x12);
  Menu_AppendResourceEntry(0x13);
  Menu_AppendResourceEntry(0x14);
  Menu_CenterResourceEntries(17, ENTRY11_COMMAND_LABEL_WIDTH, 0);
  result = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return result;
}

/// Selects a Sanctum service, icons 0x19-0x1c (Revive, Cure Poison, Repel
/// Evil, Remove Curse), centered on row 17, starting on position
/// `selection`. Returns the position chosen, or -1 when cancelled.
s32 Menu_SelectSanctumService(s32 selection) {
  s32 result;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x19);
  Menu_AppendResourceEntry(0x1A);
  Menu_AppendResourceEntry(0x1B);
  Menu_AppendResourceEntry(0x1C);
  Menu_CenterResourceEntries(17, ENTRY19_COMMAND_LABEL_WIDTH, 0);
  result = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return result;
}

/// Selects Yes/No (icons 5/6), returning 0 for Yes and 1 for No; cancelling
/// returns 1 (No). The menu sits on row 17 when `bottom` is nonzero, else on
/// row 0. `message` is the first of the two names to show, or 0 for the
/// command names; `labelWidth` is the name window's width inside its frame,
/// or 0 for 3. The cursor starts on position `selection`.
s32 Menu_RunConfirmSelection(s32 bottom, s32 message, s32 labelWidth,
                             s32 selection) {
  s32 row;

  row = 0;

  AffineEffect_InitializeWork();

  if (labelWidth == 0) {
    labelWidth = 3;
  }
  if (bottom != 0) {
    row = 17;
  }

  Menu_AppendResourceEntry(5);
  Menu_AppendResourceEntry(6);
  Menu_CenterResourceEntries(row, labelWidth, message);

  selection = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  if (selection == -1) {
    selection = 1;
  }
  return selection;
}

/// Selects Yes/No with the icons from tile (x, y) and the name window after
/// them, returning 0 for Yes and 1 for No; cancelling returns 1 (No).
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 selection) {
  s32 message = MsgCommandYes;

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

// Name window width for each edition
#if !TBS_PATCH_1
#define PAIR_WIDTH 7
#elif (LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

/// Selects icons 0x20 and 0x21 (Coins, Lucky Medals) centered on row 17,
/// starting on position `selection`. Returns the position chosen, or -1 when
/// cancelled. Tolbi's spring asks it (scene/tolbi_spring.c).
s32 Menu_SelectCoinsOrMedals(s32 selection) {
  s32 result;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x20);
  Menu_AppendResourceEntry(0x21);
  Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
  result = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return result;
}

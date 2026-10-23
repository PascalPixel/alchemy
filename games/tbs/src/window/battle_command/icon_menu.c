/// @file
/// The command icon menus of the field, title and transfer screens.
// Part of battle_command.c.

#include "../battle_command_internal.h"

#if LANGUAGE == LANGUAGE_GERMAN
#define TRANSFER_MODE_DRAW_HELP Text_DrawMessageAsTiles
#define TRANSFER_PASSWORD_HELP_X 32
#define TRANSFER_PASSWORD_HELP_FIRST_Y 8
#define TRANSFER_PASSWORD_HELP_LAST_Y 24
#define TRANSFER_CABLE_HELP_FIRST_Y 0
#define TRANSFER_CABLE_HELP_LAST_Y 32
#else
#define TRANSFER_MODE_DRAW_HELP Text_DrawMessage
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

/// A page of the screen transition's window edges: WININ and WINOUT, then
/// each of the 160 lines' edges.
struct WindowBuffer {
  u8 winInOut[4]; ///< WININ and WINOUT, not touched here
  struct WindowLine lines[160];
};

/// Draws the resource-menu icons, pulsing the selected icon with one affine
/// matrix, and updates the horizontal window spans to reveal the active icon
/// row. A task of the command icon menus
/// (IconMenu_Open). Each icon is a 32x32, 256-colour sprite;
/// the selected one is double-size affine, pulsing between 1.19 and 1.31
/// times, on layer 246, the others on 245, and one at x 0 is not drawn.
/// While a window transition runs (gScreenTransitionWork), it widens window 1
/// over the icon row so the icons show through: from the first icon on the
/// row's 24 lines, keeping each line's right edge, and around the selected
/// icon on the lines it overhangs (below a row at the top, above one at the
/// bottom, whose lines are 136 to 159).
void IconMenu_DrawIcons(void) {
  // UB: native pointers traverse scalar link/span fields into adjacent fields
  // and records. Use their real owners under UBFIX. Plain OAM field accesses
  // and indexed scanlines each shrink the EN body from 0x348 to 0x344 bytes.
  struct IconMenuWork *work;
  struct IconMenuEntry *entry;
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
  struct ScreenTransitionState *state;

  work = gIconMenuWork;
  entry = work->entries;
  pulseScale = gWindowSprite_BounceScales[(gFrameCount * 2) & 31];
  pulseScale = (pulseScale - 256) / 4 + 304;
  transform.scaleX = pulseScale;
  transform.scaleY = pulseScale;
  transform.angle = 0;
  affineIndex = OamMatrix_AllocFromTransform(&transform);
  // Pulse only the selection; the other icons keep their normal OAM shape.
  for (index = 0; index < work->count; index++, entry++) {
#ifndef UBFIX
    u32 *nativeAttributes;
#endif
    s32 drawLayer;
    s32 x;

    x = entry->x;
    if (x == 0)
      continue;
    if (index == work->selection) {
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
      entry->attr2 = gVramCacheEntries[entry->slot].offset >> 5;
#else
      nativeAttributes = &entry->link;
      *nativeAttributes++ = 0;
      *nativeAttributes++ =
          (affineIndex << 25) | spriteY | (left << 16) | 0x80002300;
      *nativeAttributes = gVramCacheEntries[entry->slot].offset >> 5;
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
      vramOffset = gVramCacheEntries[entry->slot].offset;
#ifdef UBFIX
      entry->attr01 = packedAttributes;
      entry->attr2 = vramOffset >> 5;
#else
      *nativeAttributes++ = packedAttributes;
      *nativeAttributes = vramOffset >> 5;
#endif
      drawLayer = 245;
    }
    Oam_AddSpriteAtDepth(entry, drawLayer);
  }
  // The top and bottom rows reveal opposite sides of the selected icon.
  if (work->row == 0) {
    struct WindowBuffer *page;

    s32 halfWidth;

    state = (struct ScreenTransitionState *)gScreenTransitionWork;
    if (state == 0 || work->count == 0)
      return;
    page = (struct WindowBuffer *)state->pages[state->page];
    packedSpan = work->entries[work->selection].x;
    halfWidth = (pulseScale * 12 - 2817) / 256;
    packedSpan =
        ((packedSpan - halfWidth) << 8) + (packedSpan + halfWidth) + 23;
    lastLine = work->entries[work->selection].y + 24;
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
    packedSpan = work->entries[0].x;
    if (work->selection == 0)
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

    state = (struct ScreenTransitionState *)gScreenTransitionWork;
    if (state == 0 || work->count == 0)
      return;
    page = (struct WindowBuffer *)state->pages[state->page];
    packedSpan = work->entries[work->selection].x;
    halfWidth = (pulseScale * 12 - 2817) / 256;
    packedSpan =
        ((packedSpan - halfWidth) << 8) + (packedSpan + halfWidth) + 23;
    firstLine = work->entries[work->selection].y;
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
    packedSpan = work->entries[0].x;
    if (work->selection == 0)
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
/// The work (struct IconMenuWork, 152 bytes) lives in HEAP_SLOT_ICON_MENU
/// until IconMenu_Close frees it; it is returned, and also reached
/// as gIconMenuWork. Every command icon menu starts here.
void *IconMenu_Open(void) {
  // The matching allocation size is the complete six-entry work block.
  void *work;
  work = AllocHeapSlotEwram(HEAP_SLOT_ICON_MENU, 152);
  Dma_Fill32(0, work, 152);
  Task_Add(IconMenu_DrawIcons, 0xc76);
  return work;
}

/// Stops the icon pulse, closes the label window and releases each loaded icon
/// before freeing the resource-menu state (HEAP_SLOT_ICON_MENU), then waits a
/// frame.
void IconMenu_Close(void) {
  struct Window *labelWindow;
  s32 i;
  struct IconMenuWork *work;

  work = gIconMenuWork;
  Task_Remove(IconMenu_DrawIcons);
  labelWindow = work->window;
  if (labelWindow != 0) {
    Window_Close(labelWindow, 2);
  }
  for (i = 0; i < work->count; i++) {
    VramCache_FreeEntry(work->entries[i].slot);
  }
  FreeHeapSlot(HEAP_SLOT_ICON_MENU);
  WaitFrames(1U);
}

/// Selects an icon position with wrapped directional input. A returns the
/// position; B or Start returns -1. Redraws the label only after a move. The
/// label is message resourceBase + position when the layout set a
/// resourceBase, else the icon's command name (MsgCommandName + its icon).
s32 IconMenu_Run(s32 initial) {
  struct IconMenuWork *work = gIconMenuWork;
  s32 resource;

  work->selection = initial;
  // The redraw is a loop written with goto (MATCHING.md): as a for (;;) round
  // the key loop, left with break after a move, the frame and registers change
  // and only 52 of 134 instructions still line up in tbs-en.
redraw:
  WindowSprite_PrepareForRedraw(work->window);
  if (work->labelBase != 0)
    resource = work->labelBase + work->selection;
  else
    resource = work->commandIcons[work->selection] + MsgCommandName;
  Text_DrawMessageAsTiles(resource, work->window, 0, 0);
  for (;;) {
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      return work->selection;
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      return -1;
    }
    if (gKeysPressed & KEY_START) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      return -1;
    }
    if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP)) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      work->selection--;
      if (work->selection < 0)
        work->selection = work->count - 1;
    } else if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN)) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
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
s32 IconMenu_AnimateTo(s32 start, s32 goal) {
  s16 resourceBase;
  s16 selection;
  s32 diff;
  s32 resourceId;
  s32 distance;
  s32 position;
  s32 step;
  s32 delay;
  const u8 *stepDelays;
  struct IconMenuWork *state;

  state = gIconMenuWork;
  step = 1;
  delay = 12;
  state->selection = start;
  if (goal < start)
    step = -1;
  position = start;

  for (;;) {
    WindowSprite_PrepareForRedraw(state->window);
    resourceBase = state->labelBase;
    if (resourceBase != 0) {
      resourceId = resourceBase + state->selection;
    } else {
      resourceId = state->commandIcons[state->selection] + MsgCommandName;
    }
    Text_DrawMessageAsTiles(resourceId, state->window, 0, 0);

    selection = state->selection;
    stepDelays = gMenu_SelectionStepDelays;
    diff = selection - goal;
    distance = AbsoluteDifference(diff, selection, goal);
    WaitFrames(stepDelays[distance] + delay);

    if (position == goal)
      break;

    state->selection += step;
    Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
    delay = 0;
    position += step;
  }

  WaitFrames(48);
  Sound_PlaySongFar(SE_MENU_CONFIRM);
  return goal;
}

/// Decodes one command-icon table entry into the requested VRAM cache slot,
/// through a 1 KB block it allocates and frees.
void CommandIcon_LoadIntoSlot(s32 slot, s32 index) {
  s32 size = 1024;
  void *buffer = Alloc(size);
  u16 *offsetTable = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  Decompress_ByteLz((void *)((u32)offsetTable + offsetTable[index]), buffer);
  VramCache_Load(slot, size, buffer);
  Free(buffer);
}

/// Appends a command icon if fewer than six are present. Its initial position
/// is on the bottom icon row; a layout routine places the final row. The
/// icon's number (ResourceId_CommandIcons) is also its command name's
/// (MsgCommandName + icon). Its VRAM slot is taken unchecked.
void IconMenu_AddIcon(s32 icon) {
  struct IconMenuWork *work;
  struct IconMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 y;

  work = gIconMenuWork;
  index = work->count;
  if (index <= 5) {
    work->count++;
    entry = &work->entries[index];
    slot = VramCache_FindFreeEntry();
    CommandIcon_LoadIntoSlot(slot, icon);
    entry->x = index * 24 + 32;
    y = 136;
    entry->y = y;
    entry->slot = slot;
    work->commandIcons[index] = icon;
  }
}

/// Centers the icons and their label window together on the given tile row.
/// A nonzero resourceBase supplies labels; otherwise command-name IDs do.
/// The icons stand three tiles apart and the label window, `width` + 2 tiles
/// wide and 3 tall, follows them; the centring counts two thirds of the
/// window's width.
void IconMenu_Center(s32 row, s32 width, s32 resourceBase) {
  struct IconMenuWork *work = gIconMenuWork;
  s32 x;
  s32 i;
  s32 count;

  work->width = width + 2;
  work->labelBase = resourceBase;
  work->row = row;
  count = work->count;
  x = 15 - (count * 3 + work->width * 2 / 3) / 2;
  for (i = 0; i < work->count; i++) {
    struct IconMenuEntry *entry = &work->entries[i];

    entry->x = x * 8;
    entry->y = row * 8;
    x += 3;
  }
  work->window = Window_Create(x, row, work->width, 3, 2);
}

/// Places icons at three-tile intervals from (x, y), then creates their label
/// window immediately to the right. Coordinates and label width are in tiles.
void IconMenu_LayoutIcons(s32 x, s32 y, s32 width, s32 resourceBase) {
  struct IconMenuWork *work;
  s32 i;
  struct IconMenuEntry *entry;

  work = gIconMenuWork;
  work->width = width + 2;
  work->labelBase = resourceBase;
  work->row = y;
  for (i = 0; i < work->count; i++) {
    entry = &work->entries[i];
    entry->y = y << 3;
    entry->x = x << 3;
    x += 3;
  }
  work->window = Window_Create(x, y, work->width, 3, 2);
}

/// Reads the signed command-position mapping used by the icon menu.
static __inline__ s32 IconMenu_ReadCommandPosition(s8 *table, s32 index) {
  return table[index];
}

/// Builds the field command menu, omitting Djinn when none are owned.
/// Translates between command IDs and visible icon positions using the two
/// lookup rows. The icons are Psynergy, Djinn, Item and Status on row 17;
/// `selection` is the command to start on and the result the command
/// chosen (enum FieldMenuCommand), or -1 when cancelled. The
/// field's menu runs it (window/window/selection.c).
s32 FieldMenu_SelectCommand(s32 selection) {
  s32 group = 0;
  s32 offset;
  s32 groupTriple;
  s32 result;
  s8 *commandPositions;

  if (Party_SumDjinnCountsFar(-1) == 0) {
    group = 1;
  }

  groupTriple = group * 3;
  commandPositions = gFieldMenu_PositionByCommand;
  offset = groupTriple << 1;
  selection =
      IconMenu_ReadCommandPosition(commandPositions, selection + offset) - 1;
  if (selection < 0) {
    selection = 0;
  }

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_PSYNERGY);
  if (group == 0) {
    IconMenu_AddIcon(COMMAND_ICON_DJINN);
  }
  IconMenu_AddIcon(COMMAND_ICON_ITEM);
  IconMenu_AddIcon(COMMAND_ICON_STATUS);
  IconMenu_Center(17, SELECT_MENU_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();

  if (result >= 0) {
    result = gFieldMenu_CommandByPosition[result + offset + 1];
  }

  return result;
}

#if LANGUAGE != LANGUAGE_JAPANESE
#define ANIMATE_ENTRIES_ROW 7
#else
#define ANIMATE_ENTRIES_ROW 5
#endif

/// Opens the field command menu and animates from start to the command's icon.
/// Returns the final icon position after closing the menu. The Djinn icon is
/// always shown, so `command` (enum FieldMenuCommand, as
/// FieldMenu_SelectCommand gives it) is position command - 1. The world map's
/// cutscenes show it picking Djinn (map/world_map.c).
s32 Menu_AnimateSelectionToEntry(s32 start, s32 command) {
  PartyStatus_OpenAndWait();
  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_PSYNERGY);
  IconMenu_AddIcon(COMMAND_ICON_DJINN);
  IconMenu_AddIcon(COMMAND_ICON_ITEM);
  IconMenu_AddIcon(COMMAND_ICON_STATUS);
  IconMenu_Center(0x11, ANIMATE_ENTRIES_ROW, 0);
  command = IconMenu_AnimateTo(start, command - 1);
  IconMenu_Close();
  UiWork_CloseAndRelease();
  return command;
}

/// Builds the title menu from the save summaries and optional send flags.
/// Translates the chosen visible position back to its action (enum
/// FileAction), or -1 when cancelled or the backup memory cannot be read.
/// With no files it returns 0 at once. The icons, by Save_ScanRecordFlags'
/// count: New Game, Continue, Copy and Erase for one or two files (starting on
/// Continue); no New Game or Copy with three; with every file cleared no
/// Continue, and only Erase with three such files. Battle and Send follow when
/// enabled.
/// XXX: with Send shown but not Battle, Send's place maps to
/// SAVE_ACTION_BATTLE,
/// because gFileMenu_ActionByPosition lists Battle before Send in every
/// row; whether a file can ever be cleared without any of flags 48-127 set
/// is not established. The file menu runs it (menu/clear.c).
s32 FileMenu_SelectAction(void) {
  s32 fileState;
  s32 result;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  fileState = Save_ScanRecordFlags();
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
  IconMenu_Open();
  if ((group == 0) || (group == 3)) {
    IconMenu_AddIcon(COMMAND_ICON_NEW_GAME);
  }
  if (group <= 1U) {
    IconMenu_AddIcon(COMMAND_ICON_CONTINUE);
  }
  if ((group == 0) || (group == 3)) {
    IconMenu_AddIcon(COMMAND_ICON_COPY);
  }
  IconMenu_AddIcon(COMMAND_ICON_ERASE);
  if ((*(s16 *)gTitleExtraOptionEnabled) != 0) {
    IconMenu_AddIcon(COMMAND_ICON_BATTLE);
  }
  if (gTitleSendOptionEnabled != 0) {
    IconMenu_AddIcon(COMMAND_ICON_SEND);
  }
  IconMenu_Center(0x11, TYPE_MENU_WIDTH, 0);
  result = IconMenu_Run(initial);
  IconMenu_Close();
  if (result >= 0) {
    result = gFileMenu_ActionByPosition[result + (group * 6)];
  }
  return result;
}

#if LANGUAGE != LANGUAGE_JAPANESE
#define MODE_LABEL_X 18
#define MODE_LABEL_RIGHT 144
#else
#define MODE_LABEL_X 8
#define MODE_LABEL_RIGHT 80
#endif

/// Redraws the password-level explanation when the selected level changes.
/// A task of the password level menu: what each level's
/// password carries, from MsgPasswordLevel on: Gold (0) "Level", "Djinn",
/// "Character stats", "Items" and "Coins", Silver (1) the first three,
/// Bronze (2) the first two.
void Menu_DrawModeLabel(void) {
  struct IconMenuWork *state = gIconMenuWork;

  if (state->previousMode != state->selection) {
    state->previousMode = state->selection;
    Window_ClearInteriorTiles(state->lowerWindow, 8, 40, MODE_LABEL_RIGHT, 80);

    switch (state->selection) {
    case PASSWORD_LEVEL_GOLD: {
      s32 text = MsgPasswordLevel;

      Text_DrawMessageAsTiles(text, state->lowerWindow, MODE_LABEL_X, 40);
      Text_DrawMessageAsTiles(text + 1, state->lowerWindow, MODE_LABEL_X, 48);
      Text_DrawMessageAsTiles(text + 2, state->lowerWindow, MODE_LABEL_X, 56);
      Text_DrawMessageAsTiles(text + 3, state->lowerWindow, MODE_LABEL_X, 64);
      text += 4;
      Text_DrawMessageAsTiles(text, state->lowerWindow, MODE_LABEL_X, 72);
      break;
    }
    case PASSWORD_LEVEL_SILVER: {
      s32 text = MsgPasswordLevel;

      Text_DrawMessageAsTiles(text, state->lowerWindow, MODE_LABEL_X, 40);
      Text_DrawMessageAsTiles(text + 1, state->lowerWindow, MODE_LABEL_X, 48);
      text += 2;
      Text_DrawMessageAsTiles(text, state->lowerWindow, MODE_LABEL_X, 56);
      break;
    }
    case PASSWORD_LEVEL_BRONZE:
    default: {
      s32 text = MsgPasswordLevel;

      Text_DrawMessageAsTiles(text++, state->lowerWindow, MODE_LABEL_X, 40);
      Text_DrawMessageAsTiles(text, state->lowerWindow, MODE_LABEL_X, 48);
      break;
    }
    }
  }
}

#if LANGUAGE != LANGUAGE_JAPANESE
#define PASSWORD_HELP_X 16
#else
#define PASSWORD_HELP_X 0
#endif

// French alone redraws the transfer help its own way: it releases the help's
// window sprites instead of preparing them for a redraw, and draws Cable's
// help only when coming from Password. Spanish and Italian, which take the
// French updates (TBS_PATCH_3), keep the others' form, so the test is on
// the language.
// XXX: why French needs its own redraw is unknown; a French recording of the
// menu switching methods would show what it changes.
#if (LANGUAGE == LANGUAGE_FRENCH)

/// Redraws password/cable transfer help when the chosen method changes. A
/// task of the transfer method menu: Password or Cable.
void Menu_DrawModeIndicator(void) {
  struct IconMenuWork *state = gIconMenuWork;

  if (state->previousMode != state->selection) {
    if (state->selection == TRANSFER_METHOD_PASSWORD) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage(MsgPasswordTransferHelp, state->lowerWindow, 16, 4);
      Text_DrawMessage(MsgPasswordTransferHelp + 1, state->lowerWindow, 16, 16);
    } else if (state->previousMode == TRANSFER_METHOD_PASSWORD) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage(MsgCableTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage(MsgCableTransferHelp + 1, state->lowerWindow, 0, 16);
      Text_DrawMessage(MsgCableTransferHelp + 2, state->lowerWindow, 0, 28);
    }
    state->previousMode = state->selection;
  }
}
#else
/// Redraws password/cable transfer help when the chosen method changes. A
/// task of the transfer method menu: Password or Cable.
void Menu_DrawModeIndicator(void) {
  struct IconMenuWork *state = gIconMenuWork;
  s16 *shown = &state->previousMode;
  s16 *current = &state->selection;

  if (*shown != *current) {
    *shown = *current;
    WindowSprite_PrepareForRedraw(state->lowerWindow);
    if (*current == TRANSFER_METHOD_PASSWORD) {
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

/// Selects either the transfer method or the password level (enum
/// ResourceLayoutMode). Schedules the
/// appropriate help redraw while the icon menu is active, then closes both
/// help windows and the icon menu. Returns the place
/// chosen, or -1 when cancelled. The Send screens run it (menu/clear.c).
s32 IconMenu_ChooseSendOption(s32 mode) {
  struct IconMenuWork *work;
  struct Window *window;
  s32 message;
  struct Window **upper;
  s32 result;

  IconMenu_Open();
  work = gIconMenuWork;
  if (mode == RESOURCE_LAYOUT_TRANSFER_METHOD) {
    IconMenu_AddIcon(COMMAND_ICON_PASSWORD);
    IconMenu_AddIcon(COMMAND_ICON_CABLE);
  } else {
    IconMenu_AddIcon(COMMAND_ICON_GOLD);
    IconMenu_AddIcon(COMMAND_ICON_SILVER);
    IconMenu_AddIcon(COMMAND_ICON_BRONZE);
#if (LANGUAGE == LANGUAGE_SPANISH)
    IconMenu_Center(17, 5, 0);
#endif
  }
#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
  IconMenu_Center(17, 7, 0);
#elif (LANGUAGE == LANGUAGE_SPANISH)
#elif (LANGUAGE == LANGUAGE_FRENCH)
  IconMenu_Center(17, 9, 0);
#else
  IconMenu_Center(17, 5, 0);
#endif
  if (mode != RESOURCE_LAYOUT_TRANSFER_METHOD) {
    Task_Add(Menu_DrawModeLabel, 0xc76);
    work->previousMode = 0xffff;
    window = Window_Create(RESOURCE_MODE0_WINDOW_X, 0,
                           RESOURCE_MODE0_WINDOW_WIDTH, 4, 2);
    message = MsgPasswordSelection;
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage(message, window, 0, 4);
#if LANGUAGE != LANGUAGE_JAPANESE
    window = Window_Create(RESOURCE_MODE1_WINDOW_X, 4,
                           RESOURCE_MODE1_WINDOW_WIDTH, 12, 2);
    work->lowerWindow = window;
    Text_DrawMessage(message + 1, window, 8, 0);
    Text_DrawMessage(message + 2, work->lowerWindow, 8, 11);
    message += 3;
    Text_DrawMessage(message, work->lowerWindow, 8, 22);
#else
    window = Window_Create(5, 4, 21, 12, 2);
    work->lowerWindow = window;
    Text_DrawMessage(message + 1, window, 0, 4);
    message += 2;
    Text_DrawMessage(message, work->lowerWindow, 0, 16);
#endif
  } else {
    Task_Add(Menu_DrawModeIndicator, 0xc76);
    work->previousMode = 0xffff;
    window = Window_Create(TRANSFER_METHOD_WINDOW_X, 0,
                           TRANSFER_METHOD_WINDOW_WIDTH, 4, 2);
    upper = &work->upperWindow;
    *upper = window;
    Text_DrawMessage(MsgTransferMethod, window, TRANSFER_METHOD_LABEL_X, 4);
    work->lowerWindow = Window_Create(TRANSFER_HELP_WINDOW_X, 5,
                                      TRANSFER_HELP_WINDOW_WIDTH, 7, 2);
  }
  result = IconMenu_Run(0);
  if (mode != RESOURCE_LAYOUT_TRANSFER_METHOD)
    Task_Remove(Menu_DrawModeLabel);
  else
    Task_Remove(Menu_DrawModeIndicator);
  WindowSprite_PrepareForRedraw(*upper);
  WindowSprite_PrepareForRedraw(work->lowerWindow);
  Window_Close(*upper, 2);
  Window_Close(work->lowerWindow, 2);
  WaitFrames(1);
  IconMenu_Close();
  return result;
}

// Command icons arranged in a row to choose from, such as Yes and No.
// Icon numbers refer to ResourceId_CommandIcons.
// Each shows the name of the icon under the cursor in a window.

/// Selects a shop command (Buy, Sell, Artifacts, Repair),
/// centered on row 17, starting on position `selection`. Returns the
/// position chosen, or -1 when cancelled.
s32 Menu_SelectShopCommand(s32 selection) {
  s32 result;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_BUY);
  IconMenu_AddIcon(COMMAND_ICON_SELL);
  IconMenu_AddIcon(COMMAND_ICON_ARTIFACTS);
  IconMenu_AddIcon(COMMAND_ICON_REPAIR);
  IconMenu_Center(17, ENTRY11_COMMAND_LABEL_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();
  return result;
}

/// Selects a Sanctum service (Revive, Cure Poison, Repel Evil, Remove Curse),
/// centered on row 17, starting on position `selection`. Returns the position
/// chosen, or -1 when cancelled.
s32 Menu_SelectSanctumService(s32 selection) {
  s32 result;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_REVIVE);
  IconMenu_AddIcon(COMMAND_ICON_CURE_POISON);
  IconMenu_AddIcon(COMMAND_ICON_REPEL_EVIL);
  IconMenu_AddIcon(COMMAND_ICON_REMOVE_CURSE);
  IconMenu_Center(17, ENTRY19_COMMAND_LABEL_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();
  return result;
}

/// Selects Yes or No, returning YES or NO; cancelling
/// returns NO. The menu sits on row 17 when `bottom` is nonzero, else
/// on row 0. `message` is the first of the two names to show, or 0 for the
/// command names; `labelWidth` is the name window's width inside its frame,
/// or 0 for 3. The cursor starts on position `selection`.
s32 YesNoMenu_Run(s32 bottom, s32 message, s32 labelWidth, s32 selection) {
  s32 row;

  row = 0;

  IconMenu_Open();

  if (labelWidth == 0) {
    labelWidth = 3;
  }
  if (bottom != 0) {
    row = 17;
  }

  IconMenu_AddIcon(COMMAND_ICON_YES);
  IconMenu_AddIcon(COMMAND_ICON_NO);
  IconMenu_Center(row, labelWidth, message);

  selection = IconMenu_Run(selection);
  IconMenu_Close();
  if (selection == -1) {
    selection = NO;
  }
  return selection;
}

/// Selects Yes or No with the icons from tile (x, y) and the name window after
/// them, returning YES or NO; cancelling returns NO.
s32 YesNoMenu_RunAt(s32 x, s32 y, s32 selection) {
  s32 message = MsgCommandYes;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_YES);
  IconMenu_AddIcon(COMMAND_ICON_NO);
  IconMenu_LayoutIcons(x, y, 3, message);
  selection = IconMenu_Run(selection);
  IconMenu_Close();
  if (selection == -1) {
    selection = NO;
  }
  return selection;
}

// Name window width for each edition
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAIR_WIDTH 7
#elif (LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

/// Selects Coins or Lucky Medals, centered on row 17,
/// starting on position `selection`. Returns the position chosen, or -1 when
/// cancelled. Tolbi's spring asks it (map/tolbi_spring.c).
s32 Menu_SelectCoinsOrMedals(s32 selection) {
  s32 result;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_COINS);
  IconMenu_AddIcon(COMMAND_ICON_LUCKY_MEDALS);
  IconMenu_Center(17, PAIR_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();
  return result;
}

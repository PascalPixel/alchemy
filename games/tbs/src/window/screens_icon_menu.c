/// @file
/// The command icon menus of the field, title and transfer screens.
// Part of screens.c.

#include "screens_internal.h"

/// How the transfer method's help is drawn ("Use a password to..." and "Use a
/// Game Link cable to connect..."), and where its lines go, in pixels.
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
/// The y of the cable help's second line, in pixels, in every edition.
#define TRANSFER_CABLE_HELP_SECOND_Y 16

/// The icon menus' bottom row, in tiles, where most of them stand; a Yes or No
/// may stand at the top, row 0, instead.
#define ICON_MENU_BOTTOM_ROW 17
/// The lines an icon row covers: its label window's three tiles.
#define ICON_ROW_LINES 24
/// The most icons a menu shows, and so the row length of
/// gFileList_ActionByPosition, which has an action for each place.
#define ICON_MENU_MAX_ICONS ((s32)ARRAY_COUNT(gIconMenuState->entries))

/// How far the selected icon overhangs its place while it pulses at the 8.8
/// `scale`: on each side, half its 24-pixel width grown by the scale, rounded
/// up; and past its row, half its 32-pixel height grown by the scale, rounded,
/// and a line more.
#define PULSE_OVERHANG_X(scale) ((((scale) - Q_8_8(1)) * 12 + 255) / 256)
#define PULSE_OVERHANG_LINES(scale)                                            \
  ((((scale) - Q_8_8(1)) * 32 + 255) / 512 + 1)

/// Draws the icon menu's icons, a task of the icon menus (IconMenu_Open). Each
/// icon is a 32x32, 256-colour sprite, and one at x 0 is not drawn; the
/// selected one pulses, double-size, in front of the others. While a screen
/// transition runs, it widens window 1 over the icon row so the icons show
/// through.
void IconMenu_DrawIcons(void) {
  struct IconMenuState *iconMenuState;
  struct IconMenuEntry *entry;
  struct AffineTransform transform;
  s32 pulseScale;
  s32 affineIndex;
  s32 span;
  s32 spriteY;
  u32 lastLine;
  u32 index;
  // UB: edge steps from one line's window 1 edges to the next line's, past
  // the member it points into.
  u16 *edge;
  struct ScreenTransitionState *state;

  iconMenuState = gIconMenuState;
  entry = iconMenuState->entries;
  pulseScale = gWindowSprite_BounceScales[(gFrameCount * 2) & 31];
  pulseScale = (pulseScale - Q_8_8(1)) / 4 + Q_8_8(1.1875);
  transform.scaleX = pulseScale;
  transform.scaleY = pulseScale;
  transform.angle = 0;
  affineIndex = OamMatrix_AllocFromTransform(&transform);
  // Pulse only the selection; the other icons keep their normal OAM shape.
  for (index = 0; index < iconMenuState->count; index++, entry++) {
    s32 drawLayer;
    u32 *word;
    s32 x;

    x = entry->x;
    if (x == 0)
      continue;
    if (index == iconMenuState->selection) {
      s32 left = x + pulseScale * 7 / 512 - 20;

      // The icon grows to 32 * scale / Q_8_8(1) pixels in the middle of its
      // 64-pixel double-size box: on the top row its top edge stays at the
      // screen's top, the box starting above it with its y wrapped within
      // OAM's 8 bits; elsewhere it grows mostly upward from its row.
      if (entry->y != 0)
        spriteY = entry->y + pulseScale * 3 / 256 - 20;
      else
        spriteY = (pulseScale * 15 / 256 - 30) & 255;
      word = entry->sprite;
      *word++ = 0;
      *word++ = OBJ_MATRIX(affineIndex) | spriteY | OBJ_X(left) |
                OBJ_SIZE_32X32 | OBJ_256_COLORS | OBJ_DOUBLE_SIZE | OBJ_AFFINE;
      *word = gVramCacheEntries[entry->slot].offset >> 5;
      drawLayer = OAM_DEPTH_WINDOW + 6;
    } else {
      u32 attributes;
      u32 tileOffset;

      word = entry->sprite;
      spriteY = *(s16 *)&entry->y;
      *word++ = 0;
      attributes = spriteY | OBJ_X(x) | OBJ_SIZE_32X32 | OBJ_256_COLORS;
      tileOffset = gVramCacheEntries[entry->slot].offset;
      *word++ = attributes;
      *word = tileOffset >> 5;
      drawLayer = OAM_DEPTH_WINDOW + 5;
    }
    Oam_AddSpriteAtDepth(entry, drawLayer);
  }
  // Window 1 widens around the selected icon on the lines it overhangs, below
  // it on the top row and above it on the bottom one, then covers the row's
  // ICON_ROW_LINES lines from the first icon, keeping each line's right edge.
  if (iconMenuState->row == 0) {
    struct WindowBuffer *page;
    s32 halfWidth;

    state = gScreenTransitionState;
    if (state == NULL || iconMenuState->count == 0)
      return;
    page = (struct WindowBuffer *)state->pages[state->page];
    span = iconMenuState->entries[iconMenuState->selection].x;
    halfWidth = PULSE_OVERHANG_X(pulseScale);
    span = ((span - halfWidth) << 8) + (span + halfWidth) + 23;
    lastLine =
        iconMenuState->entries[iconMenuState->selection].y + ICON_ROW_LINES;
    lastLine += PULSE_OVERHANG_LINES(pulseScale);
    edge = &page->lines[ICON_ROW_LINES].window1;
    for (index = ICON_ROW_LINES; index < lastLine; index++, edge += 2)
      *edge = (*edge & 0xff) | span;
    span = iconMenuState->entries[0].x;
    if (iconMenuState->selection == 0)
      span -= PULSE_OVERHANG_X(pulseScale);
    span <<= 8;
    edge = &page->lines[0].window1;
    for (index = 0; index < ICON_ROW_LINES; index++, edge += 2)
      *edge = (*edge & 0xff) | span;
  } else {
    struct WindowBuffer *page;
    s32 firstLine;
    s32 halfWidth;

    state = gScreenTransitionState;
    if (state == NULL || iconMenuState->count == 0)
      return;
    page = (struct WindowBuffer *)state->pages[state->page];
    span = iconMenuState->entries[iconMenuState->selection].x;
    halfWidth = PULSE_OVERHANG_X(pulseScale);
    span = ((span - halfWidth) << 8) + (span + halfWidth) + 23;
    firstLine = iconMenuState->entries[iconMenuState->selection].y;
    firstLine -= PULSE_OVERHANG_LINES(pulseScale);
    edge = &page->lines[firstLine].window1;
    for (index = firstLine; index < ICON_MENU_BOTTOM_ROW * 8;
         index++, edge += 2)
      *edge = (*edge & 0xff) | span;
    span = iconMenuState->entries[0].x;
    if (iconMenuState->selection == 0)
      span -= PULSE_OVERHANG_X(pulseScale);
    span <<= 8;
    edge = &page->lines[ICON_MENU_BOTTOM_ROW * 8].window1;
    for (index = ICON_MENU_BOTTOM_ROW * 8; index < DISPLAY_HEIGHT;
         index++, edge += 2)
      *edge = (*edge & 0xff) | span;
  }
}

/// Allocates and clears the icon menu's state, which lives in
/// HEAP_SLOT_ICON_MENU until IconMenu_Close frees it, and starts the task that
/// draws its icons. Every icon menu starts here.
void *IconMenu_Open(void) {
  void *state;
  state =
      HeapSlot_AllocEwram(HEAP_SLOT_ICON_MENU, sizeof(struct IconMenuState));
  Dma_Fill32(0, state, sizeof(struct IconMenuState));
  Task_Add(IconMenu_DrawIcons, TASK_PASS_FRAME - 10);
  return state;
}

/// Stops drawing the icons, closes the label window and releases each loaded
/// icon before freeing the icon menu's state (HEAP_SLOT_ICON_MENU), then waits
/// a frame.
void IconMenu_Close(void) {
  struct Window *labelWindow;
  s32 i;
  struct IconMenuState *state;

  state = gIconMenuState;
  Task_Remove(IconMenu_DrawIcons);
  labelWindow = state->window;
  if (labelWindow != 0) {
    Window_Close(labelWindow, 2);
  }
  for (i = 0; i < state->count; i++) {
    VramCache_FreeEntry(state->entries[i].slot);
  }
  HeapSlot_Release(HEAP_SLOT_ICON_MENU);
  wait(1U);
}

/// Selects an icon position with wrapped directional input. A returns the
/// position; B or Start returns -1. Redraws the label only after a move: the
/// message labelBase + position when the layout set a labelBase, else the
/// icon's command name.
s32 IconMenu_Run(s32 initial) {
  struct IconMenuState *state = gIconMenuState;
  s32 resource;

  state->selection = initial;
redraw:
  WindowSprite_PrepareForRedraw(state->window);
  if (state->labelBase != 0)
    resource = state->labelBase + state->selection;
  else
    resource = state->commandIcons[state->selection] + MsgCommandName;
  Text_DrawMessageAsTiles(resource, state->window, 0, 0);
  for (;;) {
    wait(1);
    if (gKeysPressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      return state->selection;
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      return -1;
    }
    if (gKeysPressed & KEY_START) {
      Sound_PlayFar(SE_MENU_CANCEL);
      return -1;
    }
    if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP)) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      state->selection--;
      if (state->selection < 0)
        state->selection = state->count - 1;
    } else if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN)) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      state->selection++;
      if (state->selection >= state->count)
        state->selection = 0;
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

/// Moves the selection from start to goal one icon at a time with the cursor
/// sound, redrawing the label each step and waiting
/// gMenu_SelectionStepDelays[icons still to go] frames at each (12 more at the
/// first), then 48 frames on the goal before the confirm sound: a menu choice
/// made for the player to watch (Menu_AnimateSelectionToEntry). Returns goal.
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
  struct IconMenuState *state;

  state = gIconMenuState;
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
    wait(stepDelays[distance] + delay);

    if (position == goal)
      break;

    state->selection += step;
    Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    delay = 0;
    position += step;
  }

  wait(48);
  Sound_PlayFar(SE_MENU_CONFIRM);
  return goal;
}

/// Decodes command icon `index` into the VRAM cache entry `slot`, through a
/// block it allocates and frees.
void CommandIcon_LoadIntoSlot(s32 slot, s32 index) {
  s32 size = COMMAND_ICON_BYTES;
  void *buffer = Heap_Alloc(size);
  u16 *offsetTable = Resource_GetTableEntry(RESOURCE_COMMAND_ICONS);

  Decompress_ByteLz((void *)((u32)offsetTable + offsetTable[index]), buffer);
  VramCache_Load(slot, size, buffer);
  Heap_Release(buffer);
}

/// Appends a command icon if fewer than ICON_MENU_MAX_ICONS are present. It
/// starts on the bottom row; IconMenu_Center or IconMenu_LayoutIcons places the
/// row. Its VRAM cache entry is taken unchecked.
void IconMenu_AddIcon(s32 icon) {
  struct IconMenuState *state;
  struct IconMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 y = ICON_MENU_BOTTOM_ROW * 8;

  state = gIconMenuState;
  index = state->count;
  if (index < ICON_MENU_MAX_ICONS) {
    state->count++;
    entry = &state->entries[index];
    slot = VramCache_FindFreeEntry();
    CommandIcon_LoadIntoSlot(slot, icon);
    entry->x = index * 24 + 32;
    entry->y = y;
    entry->slot = slot;
    state->commandIcons[index] = icon;
  }
}

/// Centres the icons and their label window together on tile row `row`. A
/// nonzero resourceBase is the first label message; otherwise each icon is
/// labelled by its command name. The icons stand three tiles apart and the
/// label window, `width` + 2 tiles wide and 3 tall, follows them; the centring
/// counts two thirds of the window's width.
void IconMenu_Center(s32 row, s32 width, s32 resourceBase) {
  struct IconMenuState *state = gIconMenuState;
  s32 x;
  s32 i;
  s32 count;

  state->width = width + 2;
  state->labelBase = resourceBase;
  state->row = row;
  count = state->count;
  x = DISPLAY_TILE_WIDTH / 2 - (count * 3 + state->width * 2 / 3) / 2;
  for (i = 0; i < state->count; i++) {
    struct IconMenuEntry *entry = &state->entries[i];

    entry->x = x * 8;
    entry->y = row * 8;
    x += 3;
  }
  state->window = Window_Create(x, row, state->width, 3, WINDOW_FLAG_INSTANT);
}

/// Places the icons three tiles apart from tile (x, y), then opens their label
/// window, `width` + 2 tiles wide, just after them.
void IconMenu_LayoutIcons(s32 x, s32 y, s32 width, s32 resourceBase) {
  struct IconMenuState *state;
  s32 i;
  struct IconMenuEntry *entry;

  state = gIconMenuState;
  state->width = width + 2;
  state->labelBase = resourceBase;
  state->row = y;
  for (i = 0; i < state->count; i++) {
    entry = &state->entries[i];
    entry->y = y << 3;
    entry->x = x << 3;
    x += 3;
  }
  state->window = Window_Create(x, y, state->width, 3, WINDOW_FLAG_INSTANT);
}

/// Reads the signed command-position mapping used by the icon menu.
static __inline__ s32 IconMenu_ReadCommandPosition(s8 *table, s32 index) {
  return table[index];
}

/// The field menu's row of icons: Psynergy, Djinn, Item and Status, without
/// Djinn when the party has none. `selection` is the command to start on and
/// the result the command chosen (enum FieldMenuCommand), or -1 when
/// cancelled; two tables turn commands into icon positions and back.
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
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, FIELD_MENU_ICON_ROW_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();

  if (result >= 0) {
    result = gFieldMenu_CommandByPosition[result + offset + 1];
  }

  return result;
}

/// The width of the field command menu's label window, in tiles.
#if LANGUAGE != LANGUAGE_JAPANESE
#define FIELD_COMMAND_LABEL_WIDTH 7
#else
#define FIELD_COMMAND_LABEL_WIDTH 5
#endif

/// Opens the field command menu and animates from start to the command's icon.
/// Returns the final icon position after closing the menu.
s32 Menu_AnimateSelectionToEntry(s32 start, s32 command) {
  PartyStatus_OpenAndWait();
  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_PSYNERGY);
  IconMenu_AddIcon(COMMAND_ICON_DJINN);
  IconMenu_AddIcon(COMMAND_ICON_ITEM);
  IconMenu_AddIcon(COMMAND_ICON_STATUS);
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, FIELD_COMMAND_LABEL_WIDTH, 0);
  command = IconMenu_AnimateTo(start, command - 1);
  IconMenu_Close();
  PartyStatus_CloseWithMenu();
  return command;
}

/// The File Screen's rows of icons, by the files saved, each a row of
/// gFileList_ActionByPosition.
enum FileActionRow {
  FILE_ROW_SOME = 0, ///< one or two files: New Game, Continue, Copy, Erase
  FILE_ROW_FULL = 1, ///< three files: Continue, Erase
  FILE_ROW_FULL_CLEARED = 2, ///< three files, all cleared: Erase
  FILE_ROW_CLEARED = 3 ///< one or two files, all cleared: New Game, Copy, Erase
};

/// The File Screen's row of icons, from the save summaries: New Game,
/// Continue, Copy and Erase for one or two files (starting on Continue); no New
/// Game or Copy with three; with every file cleared no Continue, and only Erase
/// with three such files; then Battle and Send when they are enabled. Returns
/// the action chosen (enum FileAction) by its position. With no files it
/// returns 0 at once, and -1 when cancelled or the backup memory cannot be
/// read.
s32 FileList_SelectAction(void) {
  s32 fileState;
  s32 result;
  s32 initial;
  u32 group;

  group = FILE_ROW_SOME;
  initial = 0;
  fileState = Save_ScanFileSummaries();
  if (fileState < 0) {
    return -1;
  }
  if (fileState == 0) {
    return 0;
  }
  if (fileState == SAVE_FILE_COUNT) {
    group = FILE_ROW_FULL;
  } else if (fileState == SAVE_SCAN_ALL_CLEARED + SAVE_FILE_COUNT) {
    group = FILE_ROW_FULL_CLEARED;
  } else if (fileState > SAVE_SCAN_ALL_CLEARED) {
    group = FILE_ROW_CLEARED;
  } else {
    initial = 1;
  }
  IconMenu_Open();
  if ((group == FILE_ROW_SOME) || (group == FILE_ROW_CLEARED)) {
    IconMenu_AddIcon(COMMAND_ICON_NEW_GAME);
  }
  if (group <= (u32)FILE_ROW_FULL) {
    IconMenu_AddIcon(COMMAND_ICON_CONTINUE);
  }
  if ((group == FILE_ROW_SOME) || (group == FILE_ROW_CLEARED)) {
    IconMenu_AddIcon(COMMAND_ICON_COPY);
  }
  IconMenu_AddIcon(COMMAND_ICON_ERASE);
  if (gTitleBattleOptionEnabled != 0) {
    IconMenu_AddIcon(COMMAND_ICON_BATTLE);
  }
  if (gTitleSendOptionEnabled != 0) {
    IconMenu_AddIcon(COMMAND_ICON_SEND);
  }
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, FILE_SCREEN_ICON_ROW_WIDTH, 0);
  result = IconMenu_Run(initial);
  IconMenu_Close();
  // With Send shown but not Battle, Send's place maps to SAVE_ACTION_BATTLE:
  // gFileList_ActionByPosition lists Battle before Send in every row.
  if (result >= 0) {
    result = gFileList_ActionByPosition[result + (group * ICON_MENU_MAX_ICONS)];
  }
  return result;
}

/// The x of the list of what a password level carries, and the right edge of
/// the area cleared for it, in pixels.
#if LANGUAGE != LANGUAGE_JAPANESE
#define MODE_LABEL_X 18
#define MODE_LABEL_RIGHT 144
#else
#define MODE_LABEL_X 8
#define MODE_LABEL_RIGHT 80
#endif

/// A task of the password level menu: when the selection moves, lists what
/// that level's password carries, from MsgPasswordLevel on: Gold "Level",
/// "Djinn", "Character stats", "Items" and "Coins", Silver the first three,
/// Bronze the first two.
void PasswordLevel_DrawHelp(void) {
  struct IconMenuState *state = gIconMenuState;

  if (state->drawnSelection != state->selection) {
    state->drawnSelection = state->selection;
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

/// The x of the password transfer's help lines, in pixels.
#if LANGUAGE != LANGUAGE_JAPANESE
#define PASSWORD_HELP_X 16
#else
#define PASSWORD_HELP_X 0
#endif

// French alone redraws the transfer help its own way: it releases the help's
// window sprites instead of preparing them for a redraw, and draws Cable's help
// only when coming from Password.
#if LANGUAGE == LANGUAGE_FRENCH

/// A task of the transfer method menu: when the selection moves between
/// Password and Cable, explains the one chosen.
void TransferMethod_DrawHelp(void) {
  struct IconMenuState *state = gIconMenuState;

  if (state->drawnSelection != state->selection) {
    if (state->selection == TRANSFER_METHOD_PASSWORD) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage(MsgPasswordTransferHelp, state->lowerWindow, 16, 4);
      Text_DrawMessage(MsgPasswordTransferHelp + 1, state->lowerWindow, 16, 16);
    } else if (state->drawnSelection == TRANSFER_METHOD_PASSWORD) {
      WindowSprite_ClearList(state->lowerWindow);
      Text_DrawMessage(MsgCableTransferHelp, state->lowerWindow, 0, 4);
      Text_DrawMessage(MsgCableTransferHelp + 1, state->lowerWindow, 0, 16);
      Text_DrawMessage(MsgCableTransferHelp + 2, state->lowerWindow, 0, 28);
    }
    state->drawnSelection = state->selection;
  }
}
#else
/// A task of the transfer method menu: when the selection moves between
/// Password and Cable, explains the one chosen.
void TransferMethod_DrawHelp(void) {
  struct IconMenuState *state = gIconMenuState;
  s16 *shown = &state->drawnSelection;

  if (*shown != state->selection) {
    *shown = state->selection;
    WindowSprite_PrepareForRedraw(state->lowerWindow);
    if (state->selection == TRANSFER_METHOD_PASSWORD) {
      // "Use a password to" "pass along your saved quest."
      TRANSFER_MODE_DRAW_HELP(MsgPasswordTransferHelp, state->lowerWindow,
                              TRANSFER_PASSWORD_HELP_X,
                              TRANSFER_PASSWORD_HELP_FIRST_Y);
      TRANSFER_MODE_DRAW_HELP(MsgPasswordTransferHelp + 1, state->lowerWindow,
                              TRANSFER_PASSWORD_HELP_X,
                              TRANSFER_PASSWORD_HELP_LAST_Y);
    } else {
      // "Use a Game Link cable to connect" "two Game Boy Advance systems."
      // "Pass along your saved quest."
      TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp, state->lowerWindow, 0,
                              TRANSFER_CABLE_HELP_FIRST_Y);
      TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp + 1, state->lowerWindow, 0,
                              TRANSFER_CABLE_HELP_SECOND_Y);
      TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp + 2, state->lowerWindow, 0,
                              TRANSFER_CABLE_HELP_LAST_Y);
    }
  }
}
#endif

/// Selects either the transfer method or the password level (enum
/// ResourceLayoutMode).
s32 IconMenu_ChooseSendOption(s32 mode) {
  struct IconMenuState *state;
  struct Window *window;
  s32 message;
  struct Window **upper;
  s32 result;

  IconMenu_Open();
  state = gIconMenuState;
  if (mode == SEND_OPTION_TRANSFER_METHOD) {
    IconMenu_AddIcon(COMMAND_ICON_PASSWORD);
    IconMenu_AddIcon(COMMAND_ICON_CABLE);
  } else {
    IconMenu_AddIcon(COMMAND_ICON_GOLD);
    IconMenu_AddIcon(COMMAND_ICON_SILVER);
    IconMenu_AddIcon(COMMAND_ICON_BRONZE);
#if LANGUAGE == LANGUAGE_SPANISH
    IconMenu_Center(ICON_MENU_BOTTOM_ROW, 5, 0);
#endif
  }
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_GERMAN ||             \
    LANGUAGE == LANGUAGE_ITALIAN
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, 7, 0);
#elif LANGUAGE == LANGUAGE_SPANISH
#elif LANGUAGE == LANGUAGE_FRENCH
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, 9, 0);
#else
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, 5, 0);
#endif
  if (mode != SEND_OPTION_TRANSFER_METHOD) {
    Task_Add(PasswordLevel_DrawHelp, TASK_PASS_FRAME - 10);
    state->drawnSelection = -1;
    window = Window_Create(PASSWORD_LEVEL_WINDOW_X, 0,
                           PASSWORD_LEVEL_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
    message = MsgPasswordSelection;
    upper = &state->upperWindow;
    *upper = window;
    // "Password Selection", then "The data stored in your" "password depends
    // on which" "level you select:"
    Text_DrawMessage(message, window, 0, 4);
#if LANGUAGE != LANGUAGE_JAPANESE
    window = Window_Create(PASSWORD_HELP_WINDOW_X, 4,
                           PASSWORD_HELP_WINDOW_WIDTH, 12, WINDOW_FLAG_INSTANT);
    state->lowerWindow = window;
    Text_DrawMessage(message + 1, window, 8, 0);
    Text_DrawMessage(message + 2, state->lowerWindow, 8, 11);
    message += 3;
    Text_DrawMessage(message, state->lowerWindow, 8, 22);
#else
    window = Window_Create(5, 4, 21, 12, WINDOW_FLAG_INSTANT);
    state->lowerWindow = window;
    Text_DrawMessage(message + 1, window, 0, 4);
    message += 2;
    Text_DrawMessage(message, state->lowerWindow, 0, 16);
#endif
  } else {
    Task_Add(TransferMethod_DrawHelp, TASK_PASS_FRAME - 10);
    state->drawnSelection = -1;
    window =
        Window_Create(TRANSFER_METHOD_WINDOW_X, 0, TRANSFER_METHOD_WINDOW_WIDTH,
                      4, WINDOW_FLAG_INSTANT);
    upper = &state->upperWindow;
    *upper = window;
    // "Transfer Method"
    Text_DrawMessage(MsgTransferMethod, window, TRANSFER_METHOD_LABEL_X, 4);
    state->lowerWindow =
        Window_Create(TRANSFER_HELP_WINDOW_X, 5, TRANSFER_HELP_WINDOW_WIDTH, 7,
                      WINDOW_FLAG_INSTANT);
  }
  result = IconMenu_Run(0);
  if (mode != SEND_OPTION_TRANSFER_METHOD)
    Task_Remove(PasswordLevel_DrawHelp);
  else
    Task_Remove(TransferMethod_DrawHelp);
  WindowSprite_PrepareForRedraw(*upper);
  WindowSprite_PrepareForRedraw(state->lowerWindow);
  Window_Close(*upper, 2);
  Window_Close(state->lowerWindow, 2);
  wait(1);
  IconMenu_Close();
  return result;
}

// Command icons arranged in a row to choose from, such as Yes and No, each a
// picture of RESOURCE_COMMAND_ICONS (enum CommandIcon).

/// Selects a shop command (Buy, Sell, Artifacts, Repair), centred on the bottom
/// row, starting on position `selection`.
s32 Menu_SelectShopCommand(s32 selection) {
  s32 result;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_BUY);
  IconMenu_AddIcon(COMMAND_ICON_SELL);
  IconMenu_AddIcon(COMMAND_ICON_ARTIFACTS);
  IconMenu_AddIcon(COMMAND_ICON_REPAIR);
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, SHOP_COMMAND_LABEL_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();
  return result;
}

/// Selects a Sanctum service (Revive, Cure Poison, Repel Evil, Remove Curse),
/// centred on the bottom row, starting on position `selection`.
s32 Menu_SelectSanctumService(s32 selection) {
  s32 result;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_REVIVE);
  IconMenu_AddIcon(COMMAND_ICON_CURE_POISON);
  IconMenu_AddIcon(COMMAND_ICON_REPEL_EVIL);
  IconMenu_AddIcon(COMMAND_ICON_REMOVE_CURSE);
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, SANCTUM_SERVICE_LABEL_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();
  return result;
}

/// Selects Yes or No, returning YES or NO; cancelling returns NO.
s32 YesNoMenu_Run(s32 bottom, s32 message, s32 labelWidth, s32 selection) {
  s32 row;

  row = 0;

  IconMenu_Open();

  if (labelWidth == 0) {
    labelWidth = 3;
  }
  if (bottom != 0) {
    row = ICON_MENU_BOTTOM_ROW;
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

/// The width of the label window under Coins and Lucky Medals, fitted to each
/// language's text.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAIR_WIDTH 7
#elif LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

/// Selects Coins or Lucky Medals, centred on the bottom row, starting on
/// position `selection`. Returns the position chosen, or -1 when cancelled.
s32 Menu_SelectCoinsOrMedals(s32 selection) {
  s32 result;

  IconMenu_Open();
  IconMenu_AddIcon(COMMAND_ICON_COINS);
  IconMenu_AddIcon(COMMAND_ICON_LUCKY_MEDALS);
  IconMenu_Center(ICON_MENU_BOTTOM_ROW, PAIR_WIDTH, 0);
  result = IconMenu_Run(selection);
  IconMenu_Close();
  return result;
}

#undef TRANSFER_CABLE_HELP_SECOND_Y
#undef ICON_MENU_BOTTOM_ROW
#undef ICON_ROW_LINES
#undef ICON_MENU_MAX_ICONS
#undef PULSE_OVERHANG_X
#undef PULSE_OVERHANG_LINES

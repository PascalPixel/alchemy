/// @file
/// The name entry screen.
// Part of screens.c.

#include "../screens_internal.h"

#if (LANGUAGE != LANGUAGE_JAPANESE)
/// The international name entry screen has no voicing marks: where the Japanese
/// edition checks a mark against the letter before it, this edition keeps a
/// function that accepts every character. Nothing calls it.
UNUSED s32 NameEntry_AcceptsMark(s32 mark, s32 letter) { return 1; }
#else

// The kana voicing marks of the Japanese name entry screen, which join the
// letter before them.
#define NAME_VOICED_MARK 0xde     ///< dakuten
#define NAME_SEMIVOICED_MARK 0xdf ///< handakuten

/// Returns whether `mark` may follow `letter` in a Japanese name: any
/// character but a voicing mark may, and a voicing mark only after a letter
/// in its table of ranges (pairs of first and last letter, ended by 0).
s32 NameEntry_AcceptsMark(s32 mark, s32 letter) {
  const u8 *range;

  if (mark < NAME_VOICED_MARK || mark > NAME_SEMIVOICED_MARK)
    return 1;
  if (mark == NAME_VOICED_MARK)
    range = gNameEntry_VoicedMarkRanges;
  else
    range = gNameEntry_SemivoicedMarkRanges;
  for (; *range != 0; range += 2) {
    if (letter >= range[0] && letter <= range[1])
      return 1;
  }
  return 0;
}

/// Returns how many letters the Japanese name `name` has, not counting the
/// voicing marks.
s32 NameEntry_CountLetters(const u8 *name) {
  s32 count;

  count = 0;
  for (; *name != 0; name++) {
    // Neither NAME_VOICED_MARK nor NAME_SEMIVOICED_MARK.
    if ((u8)(*name - NAME_VOICED_MARK) > 1)
      count++;
  }
  return count;
}
#endif

/// Decompresses the type-01 tilemap `source` and copies it over `window`'s
/// rectangle, both into the BG screen block at 0x06002000 and into the UI
/// tilemap that mirrors it.
void Window_CopyTilemapRegion(const struct Window *window, const void *source) {
  struct WindowWork *work = WINDOW_WORK;
  s16 *mirror = (s16 *)work->tilemap;
  s16 *buffer = AllocEwram(0x300);
  s16 *input = buffer;
  u32 cell;
  s16 *vram;
  s32 row;

  Decompress_TaggedLz(source, buffer);
  cell = window->y * 32 + window->x;
  vram = (s16 *)WINDOW_TILEMAP_VRAM + cell;
  mirror += cell;

  for (row = 0; row < window->height; row++) {
    s32 column;

    for (column = 0; column < window->width; column++) {
      s16 value = *input++;

      *vram++ = value;
      *mirror++ = value;
    }
    vram += 32 - window->width;
    mirror += 32 - window->width;
  }
  Free(buffer);
}

/// Like Window_SetTextTileBitRect, but ORs all of `palette` into the
/// palette number of each entry (bits 12-15) after clearing only its lowest
/// bit. On entries of WINDOW_PALETTE or HIGHLIGHT_PALETTE that sets them to
/// `palette`, either 14: the name entry screen's key and the Colosso choice
/// under the cursor flash this way.
void Window_SetTextTileRect(const struct Window *window, s32 x, s32 y,
                            s32 width, s32 height, u32 palette) {
  u8 *tilemap = gWindowWork[0];

  x += window->x + 1;
  y += window->y + 1;
  palette <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    y <<= 6;
    x = y + (x << 1);
    do {
      u16 *cell = (u16 *)(x + (u32)tilemap);
      s32 remaining = width;
      while (remaining != 0) {
        u32 entry = *cell;
        entry &= ~0x1000;
        entry |= palette;
        remaining--;
        *cell = entry;
        cell++;
      }
      height--;
      x += 64;
    } while (height != 0);
    ((struct WindowWork *)tilemap)->dirty = 1;
  }
}

/// Loads the pointing hand cursor's tiles into VRAM slot `slot`.
void NameEntry_LoadPointingHandTiles(s32 slot) {
  VramCache_Load(slot, 0x80, gPointingHandTiles);
}

/// Loads the 16x16 sprite gNameEntry_CaretTiles into VRAM slot `slot`.
void NameEntry_LoadCaretTiles(s32 slot) {
  VramCache_Load(slot, 0x80, gNameEntry_CaretTiles);
}

// The name entry screen: a keyboard window of letter keys, with a column of
// command keys on its right (Delete and End, and in Japanese the keyboard
// switch), the name above it and the character's portrait.
#define NAME_COMMAND_COLUMN 18 ///< the keyboard column of the command keys
#define NAME_MAX_LETTERS 5
#define NAME_MAX_BYTES 14 ///< stored name bytes, excluding its terminator

#if LANGUAGE != LANGUAGE_JAPANESE
/// Puts the string `str` into the UI's text entries and returns its width in
/// pixels. It overwrites the start of the decoded text ring
/// (WindowWork.textBuffer), so any message decoded there is lost; the name
/// entry measures the name with it to place its caret.
s32 Text_SetString(const u8 *str) {
  struct WindowWork *work;
  u16 *dst;
  s32 count;
  u32 width;
  u32 height;

  work = WINDOW_WORK;
  count = 0;
  if (*str != 0) {
    dst = work->textBuffer;
    do {
      *dst = *str;
      str++;
      dst++;
      count++;
    } while (*str != 0);
  }
  work->textBuffer[count] = 0;
  Text_MeasureDecoded(0, &width, &height, 0);
  return width;
}

/// The text control code that sets the colour of the text after it, from the
/// byte that follows ({text_color:N} in the message catalogs).
#define TEXT_COLOR_COMMAND 8

/// Draws the name being entered, `name`, at the top left of `window`,
/// followed in TEXT_COLOR_RED by an underscore for each of the
/// NAME_MAX_LETTERS places still free: the loop fills the text to 7 bytes,
/// the name and the two-byte colour code before the underscores.
void Text_DrawPaddedLabel(struct Window *window, u8 *name) {
  u8 text[20];
  s32 length = 0;

  if (*name != 0) {
    do {
      text[length] = *name;
      name++;
      length++;
    } while (*name != 0);
  }

  text[length++] = TEXT_COLOR_COMMAND;
  text[length++] = 2;

  while (length <= 6) {
    text[length++] = '_';
  }

  text[length++] = TEXT_COLOR_COMMAND;
  text[length++] = 15;
  text[length] = 0;
  Text_DrawString(text, window, 0, -2);
}

// The width of the window showing the name, in tiles.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define NAME_LABEL_WIDTH 9
#else
#define NAME_LABEL_WIDTH 8
#endif

// Whether the edition draws the Delete and End key labels as text over the
// keyboard, and how wide its End key is, in tiles.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH)
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#if (LANGUAGE == LANGUAGE_GERMAN)
#define NAME_END_KEY_WIDTH 4
#else
#define NAME_END_KEY_WIDTH 3
#endif

/// The name entry screen for party member `member`: the player types a name
/// of up to NAME_MAX_LETTERS letters on the keyboard and ends with End (or
/// Start, which jumps there). A puts the key under the cursor into the name,
/// B or Delete removes the last letter, and B on an empty name cancels. Ending
/// with an empty name keeps the old one. Returns 0, or -1 when cancelled.
/// The new name is written over the member's unit name only on End. The
/// title's new game (menu/file_screen.c) and the debug menu reach it.
///
/// The keyboard: letter keys in columns 0 to 16, in groups of five with gaps
/// at columns 5, 11 and 17, rows 0 to 5; column 18 holds Delete (row 4) and
/// End (row 5).
s32 NameEntry_EditMemberName(s32 member) {
  struct Window *nameWindow;
  s32 result = 0;
  s32 originalLength = 0;
  s32 letterCount = 0;
  u8 text[NAME_MAX_BYTES + 2];
  u8 *editedName = text + 1;
  u8 *originalName = Unit_GetFar(member)->name;
  struct WindowWork *work = WINDOW_WORK;
  s32 textDirty = 1;
  s32 cursorDirty = 1;
  struct Window *keyboardWindow;
  union MenuCursorRef keyCursor;
  union MenuCursorRef nameCaret;
  s32 keyColumn, keyRow, vramSlot;
  struct WindowSprite *sprite;
  u8 *end;
  u16 *palette;

  // The keyboard, the name and the portrait.
  Window_LoadGraphics();
  keyboardWindow =
      (struct Window *)Window_Create(3, 6, 24, 9, WINDOW_FLAG_INSTANT);
  nameWindow = (struct Window *)Window_Create(8, 3, NAME_LABEL_WIDTH, 3,
                                              WINDOW_FLAG_INSTANT);
  Portrait_OpenWindow(Portrait_GetForSpeaker(member), 0, 3, 1);
  Window_CopyTilemapRegion(keyboardWindow, gNameEntry_KeyboardTilemap);
#if NAME_KEY_LABELS
  Text_DrawStringInWindow(gNameEntry_DeleteKeyLabel, keyboardWindow, 144, 32);
  Text_DrawStringInWindow(gNameEntry_EndKeyLabel, keyboardWindow, 144, 40);
#endif
  Window_DrawDividerLine(keyboardWindow, 18, 0, 18, 7);
  work->dirty = textDirty;
  // A zero byte before editedName, as in the Japanese entry, where it is the
  // letter before the first; nothing here reads it.
  text[0] = result;
  {
    s32 i;

    for (i = 0; i < NAME_MAX_BYTES; i++) {
      u32 c = originalName[i];

      editedName[i] = c;
      if (c != 0) {
        originalLength++;
        letterCount++;
      }
    }
  }
  editedName[NAME_MAX_BYTES] = 0;
  Text_DrawPaddedLabel(nameWindow, originalName);
  vramSlot = VramCache_FindFreeEntry();
  // The cursor starts on End. 95 is the last of the VRAM_CACHE_ENTRY_COUNT
  // slots; without a free one a sprite is not made, though the loop below
  // still uses it.
  keyColumn = NAME_COMMAND_COLUMN;
  keyRow = 5;
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gPointingHandTiles);
    sprite =
        WindowSprite_Create(vramSlot, OUTPUT_SIZE_16X16, keyboardWindow, 0, 0);
    keyCursor.sprite = sprite;
    MenuCursor_SetPositionImmediateFar(&keyCursor.cursor,
                                       keyboardWindow->x * 8 + 140,
                                       keyboardWindow->y * 8 + 52);
  }
  vramSlot = VramCache_FindFreeEntry();
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gNameEntry_CaretTiles);
    sprite =
        WindowSprite_Create(vramSlot, OUTPUT_SIZE_16X16, keyboardWindow, 0, 0);
    nameCaret.sprite = sprite;
    sprite->sentinel = 255;
    sprite->oam.priority = 0;
    // The caret stands after the name, at its width in pixels.
    MenuCursor_SetPositionImmediateFar(&nameCaret.cursor,
                                       Text_SetString(editedName) + 70, 22);
  }
  // The key under the cursor flashes between WINDOW_PALETTE and
  // HIGHLIGHT_PALETTE, a copy of it with WINDOW_BACKGROUND_COLOR grey.
  palette = (u16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
  Dma_Set((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette, DMA_COPY32(8),
          REG_DMA3);
  palette[4] = 0x6318;
  end = (u8 *)(originalLength + (s32)editedName);
  while (1) {
    s32 width = 1;

    // Delete and End are wider than a letter key.
    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 4)
        width = 3;
      if (keyRow == 5)
        width = NAME_END_KEY_WIDTH;
    }
    // Flash the key under the cursor, then move the cursor and the caret.
    Window_SetTextTileRect(keyboardWindow, keyColumn, keyRow, width, 1,
                           HIGHLIGHT_PALETTE);
    WaitFrames(1);
    Window_SetTextTileRect(keyboardWindow, keyColumn, keyRow, width, 1,
                           WINDOW_PALETTE);
    if (cursorDirty) {
      cursorDirty = 0;
      MenuCursor_SetFar(&keyCursor.cursor,
                        (keyboardWindow->x + keyColumn) * 8 - 7,
                        (keyboardWindow->y + keyRow) * 8 + 15, 3);
    }
    if (textDirty) {
      textDirty = 0;
      MenuCursor_SetFar(&nameCaret.cursor, Text_SetString(editedName) + 70, 22,
                        3);
    }
    MenuCursor_AdvanceFar(&keyCursor.cursor);
    MenuCursor_MoveTowardTargetFar(&nameCaret.cursor);
    {
      u32 frame = (gVBlankCount >> 1) & 7;

      sprite = nameCaret.sprite;
      sprite->oam.x = sprite->x + gNameEntry_CaretWave[frame];
      sprite->oam.y = sprite->y + gNameEntry_CaretWave[(frame + 5) & 7];
    }
    // The D-pad moves over the keys, skipping the gaps between groups and
    // wrapping around; Start jumps to End.
    if (gKeysRepeat & KEY_UP) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyRow--;
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == -1)
          keyRow = 5;
      } else {
        // In the command column up and down swap Delete and End.
        keyRow = (keyRow != 3);
        keyRow = 5 - keyRow;
      }
    }
    if (gKeysRepeat & KEY_DOWN) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyRow++;
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == 6)
          keyRow = 0;
      } else {
        keyRow = (keyRow != 6);
        keyRow += 4;
      }
    }
    if (gKeysRepeat & KEY_LEFT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn--;
      // Left of column 0 wraps to the command column on the rows it has keys,
      // else to the last letter column.
      if (keyColumn == -1) {
        keyColumn = NAME_COMMAND_COLUMN;
        if (keyRow < 4 || keyRow > 5)
          keyColumn = 16;
      } else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17) {
        keyColumn--;
      }
    }
    if (gKeysRepeat & KEY_RIGHT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn++;
      if (keyColumn == 19)
        keyColumn = 0;
      else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17)
        keyColumn++;
      if (keyColumn == NAME_COMMAND_COLUMN && (keyRow < 4 || keyRow > 5))
        keyColumn = 0;
    }
    if (gKeysPressed & KEY_START) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn = NAME_COMMAND_COLUMN;
      keyRow = 5;
    }
    if (gKeysRepeat & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
    remove_character:
      if (letterCount != 0) {
        letterCount--;
        *--end = 0;
        WindowSprite_PrepareForRedraw(nameWindow);
        Text_DrawPaddedLabel(nameWindow, editedName);
        textDirty = 1;
        continue;
      }
      result = -1;
      break;
    }
    // A: a command key, or a letter.
    if (!(gKeysRepeat & KEY_A))
      continue;
    Sound_PlaySongFar(SE_MENU_CONFIRM);
    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 5) {
        // End: an empty name shows the old one again for 10 frames, the
        // caret hidden (WINDOW_SPRITE_MOTION_HIDDEN), and keeps it; otherwise
        // the 14 name bytes and their terminator are copied over the unit's
        // name.
        if (letterCount == 0) {
          nameCaret.sprite->motion = 13;
          WindowSprite_PrepareForRedraw(nameWindow);
          Text_DrawPaddedLabel(nameWindow, originalName);
          WaitFrames(10);
        } else {
          u8 *dst = originalName;
          u8 *src = editedName;
          s32 i = 0;
          u8 *dstSave = dst;
          u8 *srcSave = src;

          /* FAKEMATCH(dead): no-op saves and restores keep destination,
           * source and index in r2, r1 and r0; without them, 9 of 506
           * instructions differ in tbs-en. */
          if (originalName)
            src = NULL;
          if (originalName)
            src = srcSave;
          if (originalName)
            dst = NULL;
          if (originalName)
            dst = dstSave;
          while (i < 15) {
            *dst++ = *src++;
            i++;
          }
        }
        break;
      }
      if (keyRow == 4)
        goto remove_character;
      continue;
    }
    {
      u32 tx = keyboardWindow->x + keyColumn + 1;
      u32 cell;
      u32 letter;

      cell = ((keyboardWindow->y + keyRow + 1) << 5) + tx;
      // Key glyphs use the low byte of the mirrored tilemap entries.
      letter = ((u8 *)&work->tilemap)[cell * 2];
      if (letterCount == NAME_MAX_LETTERS)
        continue;
      *end++ = letter;
      *end = 0;
      letterCount++;
      // A full name sends the cursor to End.
      if (letterCount == NAME_MAX_LETTERS) {
        keyColumn = NAME_COMMAND_COLUMN;
        keyRow = 5;
        cursorDirty = 1;
      }
      WindowSprite_PrepareForRedraw(nameWindow);
      Text_DrawPaddedLabel(nameWindow, editedName);
      textDirty = 1;
    }
  }
  Window_Close(keyboardWindow, 2);
  Window_Close(nameWindow, 2);
  Portrait_CloseWindow(member);
  WaitFrames(1);
  return result;
}
#else

/// The name entry screen for party member `member`: the player types a name
/// of up to NAME_MAX_LETTERS letters on the keyboard and ends with End (or
/// Start, which jumps there). A puts the key under the cursor into the name,
/// B or Delete removes the last letter, and B on an empty name cancels. Ending
/// with an empty name keeps the old one. Returns 0, or -1 when cancelled.
/// A voicing mark joins the letter before it and does not count as a letter.
///
/// The keyboard: letter keys in columns 0 to 16, in groups of five with gaps
/// at columns 5, 11 and 17, on rows 0 to 4 and 6 to 10; column 18 holds the
/// keyboard switch (row 0, or Select), Delete (row 8) and End (row 10).
s32 NameEntry_EditMemberName(s32 member) {
  struct Window *nameWindow;
  s32 result = 0;
  s32 keyboard = 0;
  s32 bytes = 0;
  s32 letterCount = 0;
  u8 text[NAME_MAX_BYTES + 2];
  u8 *editedName = text + 1;
  u8 *originalName = Unit_GetFar(member)->name;
  struct WindowWork *work = WINDOW_WORK;
  s32 textDirty = 1;
  s32 cursorDirty = 1;
  struct Window *keyboardWindow;
  union MenuCursorRef keyCursor;
  union MenuCursorRef nameCaret;
  s32 keyColumn, keyRow, vramSlot, x;
  struct WindowSprite *sprite;
  u32 letter;
  u16 *palette;

  // The keyboard, the name and the portrait.
  Window_LoadGraphics();
  keyboardWindow =
      (struct Window *)Window_Create(3, 6, 24, 13, WINDOW_FLAG_INSTANT);
  nameWindow = (struct Window *)Window_Create(8, 3, 8, 3, WINDOW_FLAG_INSTANT);
  Portrait_OpenWindow(Portrait_GetForSpeaker(member), 0, 3, 1);
  Window_CopyTilemapRegion(keyboardWindow, gNameEntry_KeyboardTilemap);
  Window_DrawDividerLine(keyboardWindow, 18, 0, 18, 12);
  work->dirty = textDirty;
  // A zero byte before editedName: the letter before the first, which
  // NameEntry_AcceptsMark reads when a voicing mark is entered first.
  text[0] = result;
  {
    s32 i;

    for (i = 0; i < NAME_MAX_BYTES; i++) {
      letter = originalName[i];

      editedName[i] = letter;
      if (letter != 0) {
        bytes++;
        letterCount++;
        if ((u8)(letter - NAME_VOICED_MARK) <= 1)
          letterCount--;
      }
    }
  }
  editedName[NAME_MAX_BYTES] = 0;
  Text_DrawString(originalName, nameWindow, 0, 0);
  keyColumn = NAME_COMMAND_COLUMN;
  vramSlot = VramCache_FindFreeEntry();
  keyRow = 10;
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gPointingHandTiles);
    sprite =
        WindowSprite_Create(vramSlot, OUTPUT_SIZE_16X16, keyboardWindow, 0, 0);
    keyCursor.sprite = sprite;
    MenuCursor_SetPositionImmediateFar(&keyCursor.cursor,
                                       keyboardWindow->x * 8 + 140,
                                       keyboardWindow->y * 8 + 92);
  }
  vramSlot = VramCache_FindFreeEntry();
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gNameEntry_CaretTiles);
    sprite =
        WindowSprite_Create(vramSlot, OUTPUT_SIZE_16X16, keyboardWindow, 0, 0);
    nameCaret.sprite = sprite;
    sprite->sentinel = 255;
    sprite->oam.priority = 0;
    // The caret stands after the name: its letters are 10 pixels apart.
    x = 72 + NameEntry_CountLetters(editedName) * 10;
    MenuCursor_SetPositionImmediateFar(&nameCaret.cursor, x, 22);
  }
  // The key under the cursor flashes between WINDOW_PALETTE and
  // HIGHLIGHT_PALETTE, a copy of it with WINDOW_BACKGROUND_COLOR grey.
  palette = (u16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
  Dma_Set((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette, DMA_COPY32(8),
          REG_DMA3);
  palette[4] = 0x6318;
  while (1) {
    s32 width = 1;

    // The command keys are wider than a letter key.
    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 0)
        width = 4;
      if (keyRow == 8)
        width = 3;
      if (keyRow == 10)
        width = 3;
    }
    // Flash the key under the cursor, then move the cursor and the caret.
    Window_SetTextTileRect(keyboardWindow, keyColumn, keyRow, width, 1,
                           HIGHLIGHT_PALETTE);
    WaitFrames(1);
    Window_SetTextTileRect(keyboardWindow, keyColumn, keyRow, width, 1,
                           WINDOW_PALETTE);
    if (cursorDirty) {
      cursorDirty = 0;
      MenuCursor_SetFar(&keyCursor.cursor,
                        (keyboardWindow->x + keyColumn) * 8 - 7,
                        (keyboardWindow->y + keyRow) * 8 + 15, 3);
    }
    if (textDirty) {
      textDirty = 0;
      x = 72 + NameEntry_CountLetters(editedName) * 10;
      MenuCursor_SetFar(&nameCaret.cursor, x, 22, 3);
    }
    MenuCursor_AdvanceFar(&keyCursor.cursor);
    MenuCursor_MoveTowardTargetFar(&nameCaret.cursor);
    {
      u32 frame = (gVBlankCount >> 1) & 7;

      sprite = nameCaret.sprite;
      sprite->oam.x = sprite->x + gNameEntry_CaretWave[frame];
      sprite->oam.y = sprite->y + gNameEntry_CaretWave[(frame + 5) & 7];
    }
    // The D-pad moves over the keys, skipping the gaps between groups and
    // wrapping around; Start jumps to End.
    if (gKeysRepeat & KEY_UP) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyRow--;
      // Letters skip the gap row 5; the command column cycles the switch,
      // Delete and End.
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == -1)
          keyRow = 10;
        else if (keyRow == 5)
          keyRow = 4;
      } else {
        if (keyRow == -1) {
          keyRow = 10;
        } else {
          keyRow = (keyRow != 7);
          keyRow *= 8;
        }
      }
    }
    if (gKeysRepeat & KEY_DOWN) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyRow++;
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == 11)
          keyRow = 0;
        if (keyRow == 5)
          keyRow = 6;
      } else {
        if (keyRow == 1)
          keyRow = 8;
        else if (keyRow == 9)
          keyRow = 10;
        else
          keyRow = 0;
      }
    }
    if (gKeysRepeat & KEY_LEFT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn--;
      if (keyColumn == -1) {
        if (keyRow == 0 || keyRow == 8 || keyRow == 10)
          keyColumn = NAME_COMMAND_COLUMN;
        else
          keyColumn = 16;
      } else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17) {
        keyColumn--;
      }
    }
    if (gKeysRepeat & KEY_RIGHT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn++;
      if (keyColumn == 19)
        keyColumn = 0;
      else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17)
        keyColumn++;
      if (keyColumn == NAME_COMMAND_COLUMN) {
        if (keyRow == 0 || keyRow == 8 || keyRow == 10)
          keyColumn = NAME_COMMAND_COLUMN;
        else
          keyColumn = 0;
      }
    }
    if (gKeysPressed & KEY_START) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn = NAME_COMMAND_COLUMN;
      keyRow = 10;
    }
    if (gKeysPressed & KEY_SELECT) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
    change_keyboard:
      keyboard ^= 1;
      if (keyboard)
        Window_CopyTilemapRegion(keyboardWindow,
                                 gNameEntry_AlternateKeyboardTilemap);
      else
        Window_CopyTilemapRegion(keyboardWindow, gNameEntry_KeyboardTilemap);
      Window_DrawDividerLine(keyboardWindow, 18, 0, 18, 12);
      work->dirty = 1;
      continue;
    }
    if (gKeysRepeat & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
    remove_character:
      // A letter with a voicing mark goes with its mark.
      if (letterCount != 0) {
        letterCount--;
        bytes--;
        letter = editedName[bytes];
        editedName[bytes] = 0;
        if ((u8)(letter - NAME_VOICED_MARK) <= 1) {
          bytes--;
          editedName[bytes] = 0;
        }
      } else {
        result = -1;
        break;
      }
      WindowSprite_PrepareForRedraw(nameWindow);
      Text_DrawString(editedName, nameWindow, 0, 0);
      textDirty = 1;
      continue;
    }
    // A: a command key, or a letter.
    if (!(gKeysRepeat & KEY_A))
      continue;
    Sound_PlaySongFar(SE_MENU_CONFIRM);
    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 10) {
        if (letterCount == 0) {
          nameCaret.sprite->motion = 13;
          WindowSprite_PrepareForRedraw(nameWindow);
          Text_DrawString(originalName, nameWindow, 0, 0);
          WaitFrames(10);
        } else {
          u8 *dst;
          u8 *src;
          s32 i;
          u8 *dstSave;
          u8 *srcSave;

          dst = originalName;
          src = editedName;
          i = 0;
          /* FAKEMATCH(dead): no-op saves and restores keep destination,
           * source and index in r1, r2 and r0; without them, 11 of 663
           * instructions differ in tbs-ja. */
          dstSave = dst;
          srcSave = src;
          if (keyboardWindow->height)
            dst = NULL;
          if (keyboardWindow->height)
            dst = dstSave;
          if (keyboardWindow->height)
            src = NULL;
          if (keyboardWindow->height)
            src = srcSave;
          while (i < 15) {
            *dst++ = *src++;
            i++;
          }
        }
        break;
      }
      if (keyRow == 8)
        goto remove_character;
      if (keyRow == 0)
        goto change_keyboard;
      continue;
    }
    {
      u32 tx = keyboardWindow->x + keyColumn + 1;
      u32 cell;
      u8 *dst;

      cell = ((keyboardWindow->y + keyRow + 1) << 5) + tx;
      // Key glyphs use the low byte of the mirrored tilemap entries.
      letter = ((u8 *)&work->tilemap)[cell * 2];
      // The keyboard's dakuten key.
      if (letter == 0xfe)
        letter = NAME_VOICED_MARK;
      // The keyboard's handakuten key.
      if (letter == 0xff)
        letter = NAME_SEMIVOICED_MARK;
      // A full name still takes a voicing mark on its last letter.
      if (letterCount == NAME_MAX_LETTERS &&
          (u8)(letter - NAME_VOICED_MARK) > 1)
        continue;
      dst = editedName + bytes;
      if (!NameEntry_AcceptsMark(letter, editedName[bytes - 1])) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        continue;
      }
      *dst = letter;
      bytes++;
      editedName[bytes] = 0;
      letterCount++;
      if ((u8)(letter - NAME_VOICED_MARK) <= 1)
        letterCount--;
      if (letterCount == NAME_MAX_LETTERS) {
        keyColumn = NAME_COMMAND_COLUMN;
        keyRow = 10;
        cursorDirty = 1;
      }
      WindowSprite_PrepareForRedraw(nameWindow);
      Text_DrawString(editedName, nameWindow, 0, 0);
      textDirty = 1;
    }
  }
  Window_Close(keyboardWindow, 2);
  Window_Close(nameWindow, 2);
  Portrait_CloseWindow(member);
  WaitFrames(1);
  return result;
}
#endif

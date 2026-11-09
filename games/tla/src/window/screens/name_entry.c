/// @file
/// The name entry screen.
// Part of screens.c.

#include "../screens_internal.h"

// The Japanese name entry screen enters kana, whose voicing marks (dakuten
// and handakuten) are characters of their own that join the kana before them.
// The other editions have no marks: in the first function's place they keep
// one that accepts every letter, and they drop the second.
#if LANGUAGE == LANGUAGE_JAPANESE
extern const u8 gNameEntry_VoicedMarkRanges[];
extern const u8 gNameEntry_SemivoicedMarkRanges[];

/// Returns whether `letter` may follow `previous` in a name: any character but
/// a voicing mark may, and a voicing mark only after a kana in its table of
/// ranges (pairs of first and last kana, ended by 0).
s32 Func_08044488(s32 letter, s32 previous) {
  const u8 *range;

  if (letter < TEXT_MARK_VOICED || letter > TEXT_MARK_SEMIVOICED)
    return 1;
  if (letter == TEXT_MARK_VOICED)
    range = gNameEntry_VoicedMarkRanges;
  else
    range = gNameEntry_SemivoicedMarkRanges;
  for (; *range != 0; range += 2) {
    if (previous >= range[0] && previous <= range[1])
      return 1;
  }
  return 0;
}

/// Returns how many letters `name` has, not counting its voicing marks.
s32 Func_080444c4(const u8 *name) {
  s32 count;

  count = 0;
  for (; *name != 0; name++) {
    if ((u8)(*name - TEXT_MARK_VOICED) >
        TEXT_MARK_SEMIVOICED - TEXT_MARK_VOICED)
      count++;
  }
  return count;
}
#else
/// Unused: the Japanese edition's mark test (Func_08044488 there), which
/// accepts every letter where no letter takes a mark.
UNUSED s32 Window_ReturnOne(void) { return 1; }
#endif

void Func_08044348(struct Window *window, const void *resource) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *buffer = Heap_AllocEwram(0x300);
  s32 offset;
  u16 *map;
  u16 *vram;
  u16 *source;
  s32 row;
  s32 col;

  source = buffer;
  Resource_DecodeType01(resource, buffer);
  offset = (((s16)window->y << 5) + (s16)window->x) * 2;
  map = (u16 *)((u8 *)work->tilemap + offset);
  vram = (u16 *)(offset + 0x06002000);
  for (row = 0; row < window->height; row++) {
    for (col = 0; col < window->width;) {
      s16 tile = *source;

      col++;
      *vram = tile;
      *map = tile;
      source++;
      vram++;
      map++;
    }
    vram += 32 - window->width;
    map += 32 - window->width;
  }
  Heap_Release(buffer);
}

s32 Text_MeasureEntry(s32 start, u32 *width, u32 *height, s32 mode);

void Window_SetTilemapFlagRect(const struct Window *object, s32 x, s32 y,
                                 s32 width, s32 height, u32 field) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  x += (s16)object->x + 1;
  y += (s16)object->y + 1;
  field <<= 12;
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
    do {
      u16 *pixel = &work->tilemap[y][x];
      s32 remaining = width;
      while (remaining != 0) {
        u32 value = *pixel;
        value &= 0xFFFFEFFF;
        value |= field;
        remaining--;
        *pixel = value;
        pixel++;
      }
      height--;
      y++;
    } while (height != 0);
    work->dirty = 1;
  }
}

s32 Resource_LoadFixedBlockBIntoFreeSlot(void) {
  s32 slot;

  slot = Resource_FindFreeEntry();
  VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiIconTiles);
  return slot;
}

void Resource_CopyFixedBlockB(s32 slot) {
  VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiIconTiles);
}

void Resource_CopyFixedBlockA(s32 slot) {
  VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiFixedBlockATiles);
}

#if (LANGUAGE != LANGUAGE_JAPANESE)
s32 Text_SetString(const u8 *str) {
  struct WindowWork *work;
  u16 *dst;
  s32 count;
  u32 width;
  u32 height;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  count = 0;
  if (*str != 0) {
    dst = work->entries;
    do {
      *dst = *str;
      str++;
      dst++;
      count++;
    } while (*str != 0);
  }
  work->entries[count] = 0;
  Text_MeasureEntry(0, &width, &height, 0);
  return width;
}

void Text_DrawPaddedLabel(s32 output, u8 *input) {
  u8 text[20];
  s32 length = 0;

  if (*input != 0) {
    do {
      text[length] = *input;
      input++;
      length++;
    } while (*input != 0);
  }

  text[length++] = 8;
  text[length++] = 2;

  while (length <= 6) {
    text[length++] = 95;
  }

  text[length++] = 8;
  text[length++] = 15;
  text[length] = 0;
  Text_DrawString(text, output, 0, -2);
}
#endif

#define NAME_MAX_BYTES 14
#define NAME_MAX_LETTERS 5
#define NAME_COMMAND_COLUMN 18

// The Japanese keyboard is a kana grid of eleven rows with a blank row in its
// middle, and its command column has a third key, at the top, that turns to
// the other kana page.
#if LANGUAGE == LANGUAGE_JAPANESE
#define NAME_KEYBOARD_HEIGHT 13
#define NAME_DIVIDER_BOTTOM 12
#define NAME_KEY_CURSOR_Y 92
#define NAME_REMOVE_ROW 8
#define NAME_FINISH_ROW 10
#else
#define NAME_KEYBOARD_HEIGHT 9
#define NAME_DIVIDER_BOTTOM 7
#define NAME_KEY_CURSOR_Y 52
#define NAME_REMOVE_ROW 4
#define NAME_FINISH_ROW 5
#endif
#define NAME_PAGE_ROW 0
#define NAME_BLANK_ROW 5
#define NAME_LAST_ROW NAME_FINISH_ROW

// The width of the window showing the name, in tiles.
#if LANGUAGE == LANGUAGE_SPANISH
#define NAME_LABEL_WIDTH 9
#else
#define NAME_LABEL_WIDTH 8
#endif

// Whether the edition draws the remove and finish keys' labels as text over
// the keyboard, and how wide its finish key is, in tiles.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
#define NAME_FINISH_WIDTH 4
#else
#define NAME_FINISH_WIDTH 3
#endif

// A dakuten or handakuten, which joins the kana before it and is no letter of
// its own.
#define IS_KANA_MARK(c)                                                        \
  ((u8)((c) - TEXT_MARK_VOICED) <= TEXT_MARK_SEMIVOICED - TEXT_MARK_VOICED)

struct NameCursorMotion {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

union NameCursor {
  struct WindowSprite *sprite;
  struct NameCursorMotion cursor;
};

extern u32 gVBlankCount;
extern char ResourceId_UiIconTiles, ResourceId_UiFixedBlockATiles;
extern const s8 gNameEntry_CaretWave[];
extern const u8 gNameEntry_KeyboardTilemap[];
#if LANGUAGE == LANGUAGE_JAPANESE
extern const u8 gNameEntry_AlternateKeyboardTilemap[];

void Text_DrawString(u8 *text, s32 window, s32 x, s32 y);
// Japanese only, at 0x08044488 there.
// Japanese only, at 0x080444c4 there: how many letters `name` has, its marks
// not counted.
#endif
#if NAME_KEY_LABELS
extern u8 gNameEntry_DeleteKeyLabel[];
extern u8 gNameEntry_EndKeyLabel[];

#endif

void Window_LoadGraphics(void);
void Func_0803f9c0(s32 glow, s32 sub);
void Func_0803d3c0(s32 member, s32 x, s32 y, s32 side);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadResourceFar(s32 slot, s32 size, s32 resource);
void Func_08108048(struct NameCursorMotion *cursor, s32 x, s32 y);
void Func_08108040(struct NameCursorMotion *cursor, s32 x, s32 y, s32 speed);
void Func_08108030(struct NameCursorMotion *cursor);
void Func_08108038(struct NameCursorMotion *cursor);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);
void Func_0803d450(s32 member);

// The Japanese form enters kana: a dakuten or handakuten joins the kana before
// it (Func_08044488 refuses a mark that cannot), takes a byte but is no
// letter, and goes with it when that kana is removed; Select or the top
// command key turns to the other kana page. It also returns without waiting
// for every button to be released, which the later editions added.
#if LANGUAGE == LANGUAGE_JAPANESE
s32 Func_08044558(s32 member) {
  struct Window *nameWindow;
  s32 result = 0;
  s32 page = 0;
  s32 byteCount = 0;
  s32 letterCount = 0;
  u8 text[NAME_MAX_BYTES + 2];
  u8 *editedName = text + 1;
  u8 *originalName = Unit_Get(member);
  struct WindowWork *render = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 textDirty = 1;
  s32 cursorDirty = 1;
  struct Window *keyboardWindow;
  union NameCursor keyCursor;
  union NameCursor nameCaret;
  s32 keyColumn, keyRow, vramSlot, x;
  struct WindowSprite *output;
  struct NameCursorMotion *keyMotion;
  u32 letter;
  u8 *end;
  u16 *palette;

  Window_LoadGraphics();
  Func_0803f9c0(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  keyboardWindow = Window_Create(3, 6, 24, NAME_KEYBOARD_HEIGHT, 2);
  nameWindow = Window_Create(8, 3, 8, 3, 2);
  Func_0803d3c0(member, 0, 3, 1);
  Func_08044348(keyboardWindow, gNameEntry_KeyboardTilemap);
  Window_DrawDividerLine(keyboardWindow, 18, 0, 18, NAME_DIVIDER_BOTTOM);
  render->dirty = textDirty;
  text[0] = result;
  {
    s32 i;

    for (i = 0; i < NAME_MAX_BYTES; i++) {
      letter = originalName[i];

      editedName[i] = letter;
      if (letter != 0) {
        byteCount++;
        letterCount++;
        if (IS_KANA_MARK(letter))
          letterCount--;
      }
    }
  }
  editedName[NAME_MAX_BYTES] = 0;
  Text_DrawString(originalName, (s32)nameWindow, 0, 0);
  keyColumn = NAME_COMMAND_COLUMN;
  vramSlot = Resource_FindFreeEntry();
  keyRow = NAME_FINISH_ROW;
  if (vramSlot <= 95) {
    VramBlock_LoadResourceFar(vramSlot, 128, (s32)&ResourceId_UiIconTiles);
    output = WindowSprite_Create(vramSlot, 0x40000000,
                                 (struct RenderInput *)keyboardWindow, 0, 0);
    keyMotion = &keyCursor.cursor;
    keyCursor.sprite = output;
    Func_08108048(keyMotion, (s16)keyboardWindow->x * 8 + 140,
                  (s16)keyboardWindow->y * 8 + NAME_KEY_CURSOR_Y);
  } else {
    keyMotion = &keyCursor.cursor;
  }
  vramSlot = Resource_FindFreeEntry();
  if (vramSlot <= 95) {
    VramBlock_LoadResourceFar(vramSlot, 128,
                              (s32)&ResourceId_UiFixedBlockATiles);
    output = WindowSprite_Create(vramSlot, 0x40000000,
                                 (struct RenderInput *)keyboardWindow, 0, 0);
    nameCaret.sprite = output;
    output->sentinel = 255;
    output->sprite.oam.priority = 0;
    x = 72 + Func_080444c4(editedName) * 10;
    Func_08108048(&nameCaret.cursor, x, 22);
  }
  palette = (u16 *)0x050001c0;
  Dma_Set((void *)0x050001e0, palette, 0x84000008, REG_DMA3);
  palette[4] = 0x6318;
  end = (u8 *)(byteCount + (s32)editedName) - 1;
  while (1) {
    s32 width = 1;

    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == NAME_PAGE_ROW)
        width = 4;
      if (keyRow == NAME_REMOVE_ROW)
        width = 3;
      if (keyRow == NAME_FINISH_ROW)
        width = 3;
    }
    Window_SetTilemapFlagRect(keyboardWindow, keyColumn, keyRow, width, 1, 14);
    wait(1);
    Window_SetTilemapFlagRect(keyboardWindow, keyColumn, keyRow, width, 1, 15);
    if (cursorDirty) {
      cursorDirty = 0;
      Func_08108040(keyMotion, ((s16)keyboardWindow->x + keyColumn) * 8 - 7,
                    ((s16)keyboardWindow->y + keyRow) * 8 + 15, 3);
    }
    if (textDirty) {
      textDirty = 0;
      x = 72 + Func_080444c4(editedName) * 10;
      Func_08108040(&nameCaret.cursor, x, 22, 3);
    }
    Func_08108030(keyMotion);
    Func_08108038(&nameCaret.cursor);
    {
      u32 frame = (gVBlankCount >> 1) & 7;

      output = nameCaret.sprite;
      output->sprite.oam.x = output->x + gNameEntry_CaretWave[frame];
      output->sprite.oam.y = output->y + gNameEntry_CaretWave[(frame + 5) & 7];
    }
    if (gInput.repeat & 0x40) {
      Sound_PlayFar(111);
      cursorDirty = 1;
      keyRow--;
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == -1)
          keyRow = NAME_LAST_ROW;
        else if (keyRow == NAME_BLANK_ROW)
          keyRow = NAME_BLANK_ROW - 1;
      } else if (keyRow == -1) {
        keyRow = NAME_FINISH_ROW;
      } else {
        keyRow = (keyRow != NAME_REMOVE_ROW - 1);
        keyRow *= NAME_REMOVE_ROW;
      }
    }
    if (gInput.repeat & 0x80) {
      Sound_PlayFar(111);
      cursorDirty = 1;
      keyRow++;
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == NAME_LAST_ROW + 1)
          keyRow = 0;
        if (keyRow == NAME_BLANK_ROW)
          keyRow = NAME_BLANK_ROW + 1;
      } else if (keyRow == NAME_PAGE_ROW + 1) {
        keyRow = NAME_REMOVE_ROW;
      } else if (keyRow == NAME_REMOVE_ROW + 1) {
        keyRow = NAME_FINISH_ROW;
      } else {
        keyRow = NAME_PAGE_ROW;
      }
    }
    if (gInput.repeat & 0x20) {
      Sound_PlayFar(111);
      cursorDirty = 1;
      keyColumn--;
      if (keyColumn == -1) {
        if (keyRow == NAME_PAGE_ROW || keyRow == NAME_REMOVE_ROW ||
            keyRow == NAME_FINISH_ROW)
          keyColumn = NAME_COMMAND_COLUMN;
        else
          keyColumn = 16;
      } else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17) {
        keyColumn--;
      }
    }
    if (gInput.repeat & 0x10) {
      Sound_PlayFar(111);
      cursorDirty = 1;
      keyColumn++;
      if (keyColumn == 19)
        keyColumn = 0;
      else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17)
        keyColumn++;
      if (keyColumn == NAME_COMMAND_COLUMN) {
        if (keyRow == NAME_PAGE_ROW || keyRow == NAME_REMOVE_ROW ||
            keyRow == NAME_FINISH_ROW)
          keyColumn = NAME_COMMAND_COLUMN;
        else
          keyColumn = 0;
      }
    }
    if (gInput.pressed & 8) {
      Sound_PlayFar(111);
      cursorDirty = 1;
      keyColumn = NAME_COMMAND_COLUMN;
      keyRow = NAME_FINISH_ROW;
    }
    if (gInput.pressed & 4) {
      Sound_PlayFar(111);
    turn_page:
      page ^= 1;
      if (page)
        Func_08044348(keyboardWindow, gNameEntry_AlternateKeyboardTilemap);
      else
        Func_08044348(keyboardWindow, gNameEntry_KeyboardTilemap);
      Window_DrawDividerLine(keyboardWindow, 18, 0, 18, NAME_DIVIDER_BOTTOM);
      render->dirty = 1;
      continue;
    }
    if (gInput.repeat & 2) {
      Sound_PlayFar(113);
    remove_character:
      if (letterCount != 0) {
        letterCount--;
        end--;
        byteCount--;
        letter = end[1];
        end[1] = 0;
        if (IS_KANA_MARK(letter)) {
          end--;
          byteCount--;
          end[1] = 0;
        }
      } else {
        result = -1;
        break;
      }
      WindowSprite_PrepareForRedraw(nameWindow);
      Text_DrawString(editedName, (s32)nameWindow, 0, 0);
      textDirty = 1;
      continue;
    }
    if (!(gInput.repeat & 1))
      continue;
    Sound_PlayFar(112);
    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == NAME_FINISH_ROW) {
        if (letterCount == 0) {
          nameCaret.sprite->active = 13;
          WindowSprite_PrepareForRedraw(nameWindow);
          Text_DrawString(originalName, (s32)nameWindow, 0, 0);
          wait(10);
        } else {
          u8 *dst = originalName;
          u8 *src = editedName;
          s32 i;

          for (i = 0; i < 15; i++)
            *dst++ = *src++;
        }
        break;
      }
      if (keyRow == NAME_REMOVE_ROW)
        goto remove_character;
      if (keyRow == NAME_PAGE_ROW)
        goto turn_page;
      continue;
    }
    {
      u32 tx = (s16)keyboardWindow->x + keyColumn + 1;
      u8 *slot;

      // `tx + tx`: as `tx << 1`, the shift is scheduled before the row's
      // address.
      letter = ((u8 *)render->tilemap +
                (((s16)keyboardWindow->y + keyRow + 1) << 6))[tx + tx];
      // The keyboard keeps the marks as 0xFE and 0xFF.
      if (letter == 0xFE)
        letter = TEXT_MARK_VOICED;
      if (letter == 0xFF)
        letter = TEXT_MARK_SEMIVOICED;
      if (letterCount == NAME_MAX_LETTERS && !IS_KANA_MARK(letter))
        continue;
      slot = editedName + byteCount;
      if (!Func_08044488(letter, *end)) {
        Sound_PlayFar(113);
        continue;
      }
      *slot = letter;
      end++;
      byteCount++;
      end[1] = 0;
      letterCount++;
      if (IS_KANA_MARK(letter))
        letterCount--;
      if (letterCount == NAME_MAX_LETTERS) {
        keyColumn = NAME_COMMAND_COLUMN;
        keyRow = NAME_FINISH_ROW;
        cursorDirty = 1;
      }
      WindowSprite_PrepareForRedraw(nameWindow);
      Text_DrawString(editedName, (s32)nameWindow, 0, 0);
      textDirty = 1;
    }
  }
  Window_Close(keyboardWindow, 2);
  Window_Close(nameWindow, 2);
  Func_0803d450(member);
  wait(1);
  return result;
}
#else
s32 Func_08044558(s32 member) {
  struct Window *nameWindow;
  s32 result = 0;
  s32 originalLength = 0;
  s32 letterCount = 0;
  u8 text[NAME_MAX_BYTES + 2];
  u8 *editedName = text + 1;
  u8 *originalName = Unit_Get(member);
  struct WindowWork *render = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 textDirty = 1;
  s32 cursorDirty = 1;
  struct Window *keyboardWindow;
  union NameCursor keyCursor;
  union NameCursor nameCaret;
  s32 keyColumn, keyRow, vramSlot;
  struct WindowSprite *output;
  struct NameCursorMotion *keyMotion;
  u8 *end;
  u16 *palette;

  Window_LoadGraphics();
  Func_0803f9c0(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  keyboardWindow = Window_Create(3, 6, 24, 9, 2);
  nameWindow = Window_Create(8, 3, NAME_LABEL_WIDTH, 3, 2);
  Func_0803d3c0(member, 0, 3, 1);
  Func_08044348(keyboardWindow, gNameEntry_KeyboardTilemap);
#if NAME_KEY_LABELS
  Text_DrawStringInWindow(gNameEntry_DeleteKeyLabel, keyboardWindow, 144, 32);
  Text_DrawStringInWindow(gNameEntry_EndKeyLabel, keyboardWindow, 144, 40);
#endif
  Window_DrawDividerLine(keyboardWindow, 18, 0, 18, 7);
  render->dirty = textDirty;
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
  Text_DrawPaddedLabel((s32)nameWindow, originalName);
  keyRow = 5;
  vramSlot = Resource_FindFreeEntry();
  keyColumn = NAME_COMMAND_COLUMN;
  if (vramSlot <= 95) {
    VramBlock_LoadResourceFar(vramSlot, 128, (s32)&ResourceId_UiIconTiles);
    output = WindowSprite_Create(vramSlot, 0x40000000,
                                 (struct RenderInput *)keyboardWindow, 0, 0);
    keyMotion = &keyCursor.cursor;
    keyCursor.sprite = output;
    Func_08108048(keyMotion, (s16)keyboardWindow->x * 8 + 140,
                  (s16)keyboardWindow->y * 8 + 52);
  } else {
    keyMotion = &keyCursor.cursor;
  }
  vramSlot = Resource_FindFreeEntry();
  if (vramSlot <= 95) {
    VramBlock_LoadResourceFar(vramSlot, 128,
                              (s32)&ResourceId_UiFixedBlockATiles);
    output = WindowSprite_Create(vramSlot, 0x40000000,
                                 (struct RenderInput *)keyboardWindow, 0, 0);
    nameCaret.sprite = output;
    output->sentinel = 255;
    output->sprite.oam.priority = 0;
    Func_08108048(&nameCaret.cursor, Text_SetString(editedName) + 70, 22);
  }
  palette = (u16 *)0x050001c0;
  Dma_Set((void *)0x050001e0, palette, 0x84000008, REG_DMA3);
  palette[4] = 0x6318;
  end = (u8 *)(originalLength + (s32)editedName);
  while (1) {
    s32 width = 1;

    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 4)
        width = 3;
      if (keyRow == 5)
        width = NAME_FINISH_WIDTH;
    }
    Window_SetTilemapFlagRect(keyboardWindow, keyColumn, keyRow, width, 1, 14);
    wait(1);
    Window_SetTilemapFlagRect(keyboardWindow, keyColumn, keyRow, width, 1, 15);
    if (cursorDirty) {
      cursorDirty = 0;
      Func_08108040(keyMotion, ((s16)keyboardWindow->x + keyColumn) * 8 - 7,
                    ((s16)keyboardWindow->y + keyRow) * 8 + 15, 3);
    }
    if (textDirty) {
      textDirty = 0;
      Func_08108040(&nameCaret.cursor, Text_SetString(editedName) + 70, 22, 3);
    }
    Func_08108030(keyMotion);
    Func_08108038(&nameCaret.cursor);
    {
      u32 frame = (gVBlankCount >> 1) & 7;

      output = nameCaret.sprite;
      output->sprite.oam.x = output->x + gNameEntry_CaretWave[frame];
      output->sprite.oam.y = output->y + gNameEntry_CaretWave[(frame + 5) & 7];
    }
    if (gInput.repeat & 0x40) {
      Sound_PlayFar(111);
      cursorDirty = 1;
      keyRow--;
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == -1)
          keyRow = 5;
      } else {
        keyRow = (keyRow != 3);
        keyRow = 5 - keyRow;
      }
    }
    if (gInput.repeat & 0x80) {
      Sound_PlayFar(111);
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
    if (gInput.repeat & 0x20) {
      Sound_PlayFar(111);
      cursorDirty = 1;
      keyColumn--;
      if (keyColumn == -1) {
        if (keyRow >= 4 && keyRow <= 5)
          keyColumn = NAME_COMMAND_COLUMN;
        else
          keyColumn = 16;
      } else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17) {
        keyColumn--;
      }
    }
    if (gInput.repeat & 0x10) {
      Sound_PlayFar(111);
      cursorDirty = 1;
      keyColumn++;
      if (keyColumn == 19)
        keyColumn = 0;
      else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17)
        keyColumn++;
      if (keyColumn == NAME_COMMAND_COLUMN && (keyRow < 4 || keyRow > 5))
        keyColumn = 0;
    }
    if (gInput.pressed & 8) {
      Sound_PlayFar(111);
      cursorDirty = 1;
      keyColumn = NAME_COMMAND_COLUMN;
      keyRow = 5;
    }
    if (gInput.repeat & 2) {
      Sound_PlayFar(113);
    remove_character:
      if (letterCount != 0) {
        letterCount--;
        *--end = 0;
        WindowSprite_PrepareForRedraw(nameWindow);
        Text_DrawPaddedLabel((s32)nameWindow, editedName);
        textDirty = 1;
        continue;
      }
      result = -1;
      break;
    }
    if (!(gInput.repeat & 1))
      continue;
    Sound_PlayFar(112);
    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 5) {
        if (letterCount == 0) {
          nameCaret.sprite->active = 13;
          WindowSprite_PrepareForRedraw(nameWindow);
          Text_DrawPaddedLabel((s32)nameWindow, originalName);
          wait(10);
        } else {
          u8 *dst = originalName;
          u8 *src = editedName;
          s32 i;

          for (i = 0; i < 15; i++)
            *dst++ = *src++;
        }
        break;
      }
      if (keyRow == 4)
        goto remove_character;
      continue;
    }
    {
      u32 tx = (s16)keyboardWindow->x + keyColumn + 1;
      u32 letter = ((u8 *)render->tilemap +
                    (((s16)keyboardWindow->y + keyRow + 1) << 6))[tx << 1];

      if (letterCount == NAME_MAX_LETTERS)
        continue;
      *end++ = letter;
      *end = 0;
      letterCount++;
      if (letterCount == NAME_MAX_LETTERS) {
        keyColumn = NAME_COMMAND_COLUMN;
        keyRow = 5;
        cursorDirty = 1;
      }
      WindowSprite_PrepareForRedraw(nameWindow);
      Text_DrawPaddedLabel((s32)nameWindow, editedName);
      textDirty = 1;
    }
  }
  Window_Close(keyboardWindow, 2);
  Window_Close(nameWindow, 2);
  Func_0803d450(member);
  wait(1);
  while (gInput.held != 0)
    wait(1);
  return result;
}
#endif

#undef NAME_MAX_BYTES
#undef NAME_MAX_LETTERS
#undef NAME_COMMAND_COLUMN
#undef NAME_KEYBOARD_HEIGHT
#undef NAME_DIVIDER_BOTTOM
#undef NAME_KEY_CURSOR_Y
#undef NAME_REMOVE_ROW
#undef NAME_FINISH_ROW
#undef NAME_PAGE_ROW
#undef NAME_BLANK_ROW
#undef NAME_LAST_ROW
#undef NAME_LABEL_WIDTH
#undef NAME_KEY_LABELS
#undef NAME_FINISH_WIDTH
#undef IS_KANA_MARK

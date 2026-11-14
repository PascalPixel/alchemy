/// @file
/// The debug menus' keys, the flag browser, and hexadecimal numbers written as
/// text.
// Part of window.c.

#include "../window_internal.h"
#include "flag_call.h"

// The debug selection row's keys (The Broken Seal's
// DebugSceneSelect_HandleRowInput): A returns -1 and B -2; Up or Down switches
// the field, Left and Right step the value (0 to 324) or the second number
// (0 to 99) by one, R and L by ten, and the row is redrawn after a step.

/// Edits the debug scene or entrance, returning -1 to exit and -2 to
/// cancel. Shoulder buttons step by ten; it keeps the original 99-step
/// entrance wrap. Otherwise it returns the scene after this frame's keys. Up
/// and Down switch the field (`selectedField`: 0 scene, 1 entrance), Left
/// and Right step by one, wrapping scenes 0 to 200 and entrances 0 to 99.
/// R and L step a scene by ten, setting the entrance to 0, and past
/// either end go to the other (195 + 10 gives 0); they step an entrance
/// by ten, wrapping by 99 (95 + 10 gives 6).
s32 DebugSceneSelect_HandleRowInput(s32 arg, s16 value, s16 *sub, s16 *mode) {
  if (gInput.repeat & KEY_A)
    return -1;
  if (gInput.repeat & KEY_B)
    return -2;
  if (gInput.repeat & KEY_DOWN) {
    *mode ^= 1;
  } else if (gInput.repeat & KEY_UP) {
    *mode ^= 1;
  } else if (gInput.repeat & KEY_RIGHT) {
    if (*mode == 0) {
      value++;
    } else {
      (*sub)++;
      if (*sub > 99)
        *sub = 0;
    }
    if (value > 324)
      value = 0;
    DebugSceneSelect_DrawRow(arg, value, sub);
  } else if (gInput.repeat & KEY_LEFT) {
    if (*mode == 0) {
      value--;
    } else {
      (*sub)--;
      if (*sub < 0)
        *sub = 99;
    }
    if (value < 0)
      value = 324;
    DebugSceneSelect_DrawRow(arg, value, sub);
  } else if (gInput.repeat & KEY_R) {
    if (*mode == 0) {
      *sub = 0;
      value += 10;
    } else {
      *sub += 10;
      if (*sub > 99)
        *sub -= 99;
    }
    if (value > 324)
      value = 0;
    DebugSceneSelect_DrawRow(arg, value, sub);
  } else if (gInput.repeat & KEY_L) {
    if (*mode == 0) {
      *sub = 0;
      value -= 10;
    } else {
      *sub -= 10;
      if (*sub < 0)
        *sub += 99;
    }
    if (value < 0)
      value = 324;
    DebugSceneSelect_DrawRow(arg, value, sub);
  }
  return value;
}

// Menu: flag list (move the cursor and press A to toggle).

/// Shows 16x16 flags per page and takes input until closed, starting on page
/// 8. Returns 0.
s32 DebugFlagEditor_Run(void) {
  u8 obj[12]; // Cursor object.
  s32 cur[2]; // Cursor column and row.
  s32 res;
  s32 page;
  s32 win;
  s32 ret;

  page = 8;
  win = 0;
  cur[0] = win;
  cur[1] = win;
  win = Window_Create(1, 0, 28, 20, 2);
  DebugFlagEditor_DrawTable(win, page);
  MenuArrow_Create(obj, &res);
  for (;;) {
    wait(1);
    ret = DebugFlagEditor_HandleInput(win, &page, cur);
    if (ret == -1)
      break; // Close.
    if (ret == 1)
      DebugFlagEditor_DrawTable(win, page); // A flag or the page changed.
    MenuArrow_SetPosition(obj, cur[0] * 8 + 58, cur[1] * 8 + 20);
  }
  MenuArrow_Release(res);
  Window_Close(win, 2);
  return 0;
}

/// Writes the lowest count digits of value (up to five), most significant
/// first, to out as uppercase hexadecimal, without a terminating zero.
void String_FormatHexDigits(u32 value, u32 count, u8 *out) {
  u8 buf[8];
  u32 i;

  if (count > 5)
    count = 5;

  for (i = 0; i != count; i++) {
    u32 d = value & 15;
    if (d <= 9)
      buf[i] = d + '0';
    else
      buf[i] = d + 'A' - 10;
    value >>= 4;
  }

  for (i = count - 1; (s32)i >= 0; i--)
    *out++ = buf[i];
}

/* VIEW(handle_selection_row_input): routine WindowSprite_PrepareForRedraw as
 * void WindowSprite_PrepareForRedraw(void); resource_work has void
 * WindowSprite_PrepareForRedraw(struct Window *); not tried:
 * WindowSprite_PrepareForRedraw's declaration holds a FAKEMATCH tag. */
extern void WindowSprite_PrepareForRedraw__handle_selection_row_input(
    void) __asm__("WindowSprite_PrepareForRedraw");

/// Draws one page of 256 flags as sixteen rows of hexadecimal labels and bits.
/// Each row: its first flag's number in three hex digits, then its 16 flags
/// as 0 or 1, lowest first, under a heading of hex digits.
void DebugFlagEditor_DrawTable(s32 window, s32 startFlag) {
  s32 row;
  s32 y;
  s32 flag;
  u8 label[5];
  char bits[17];

  WindowSprite_PrepareForRedraw__handle_selection_row_input();
  Text_DrawStringInWindow((s32)gMenu_HexDigitsString, window, 48, 0);

  flag = startFlag << 8;
  for (row = 0; row != 16; row++) {
    s32 i;

    y = row * 8 + 16;

    for (i = 0; i != 5; i++) {
      label[i] = 0;
    }
    String_FormatHexDigits(flag, 3, label);
    Text_DrawStringInWindow((s32)label, window, 0, y);
    Text_DrawStringInWindow((s32)gMenu_ColonString, window, 32, y);

    for (i = 0; i < 16; i++) {
      s32 val = GameFlag_Get(flag);
      bits[i] = (val != 0) + 48;
      flag++;
    }
    bits[i] = 0;
    Text_DrawStringInWindow((s32)bits, window, 48, y);
  }
}

/// Moves within the flag grid, changes pages, or toggles the selected flag.
/// Returns 1 when flags/page need redrawing, -1 to close, and 0 otherwise.
/// A toggles flag page * 256 + row * 16 + column; B or Select closes. L and
/// R turn a page, wrapping 0 to 15; with Start held they jump ten, to the
/// other end when that passes it (5 - 10 gives 15).
s32 DebugFlagEditor_HandleInput(s32 window, s32 *page, s32 *cursor) {
  s32 *row = &cursor[1];

  if (gInput.repeat & KEY_A) {
    s32 flag = ((*page << 4) + *row << 4) + cursor[0];

    if (GameFlag_Get(flag))
      GameFlag_Clear(flag);
    else
      GameFlag_Set(flag);
    return 1;
  }
  if ((gInput.pressed & KEY_B) || (gInput.repeat & KEY_SELECT))
    return -1;
  if (gInput.repeat & KEY_UP) {
    if (--*row < 0)
      *row = 15;
  } else if (gInput.repeat & KEY_DOWN) {
    if (++*row > 15)
      *row = 0;
  } else if (gInput.repeat & KEY_LEFT) {
    if (--cursor[0] < 0)
      cursor[0] = 15;
  } else if (gInput.repeat & KEY_RIGHT) {
    if (++cursor[0] > 15)
      cursor[0] = 0;
  } else if ((gInput.repeat & KEY_L) && (gInput.repeat & KEY_START)) {
    *page -= 10;
    if (*page < 0)
      *page = 15;
    return 1;
  } else if ((gInput.repeat & KEY_R) && (gInput.repeat & KEY_START)) {
    *page += 10;
    if (*page > 15)
      *page = 0;
    return 1;
  } else if (gInput.repeat & KEY_L) {
    if (--*page < 0)
      *page = 15;
    return 1;
  } else if (gInput.repeat & KEY_R) {
    if (++*page > 15)
      *page = 0;
    return 1;
  }
  return 0;
}

void Menu_ReservedNoOp(void) {}

void Djinn_SetFoundFlagAndAdd(s32 arg0, s32 arg1, s32 arg2) {
  GameFlag_Set((arg1 * 0x14) + arg2 + 0x30);
  Djinn_GiveFar(arg0, arg1, arg2);
}

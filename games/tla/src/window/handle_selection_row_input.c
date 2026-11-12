// The debug selection row's keys (The Broken Seal's
// DebugSceneSelect_HandleRowInput): A returns -1 and B -2; Up or Down switches
// the field, Left and Right step the value (0 to 324) or the second number
// (0 to 99) by one, R and L by ten, and the row is redrawn after a step.
#include "types.h"
#include "io_reg.h"
#include "input.h"

void Menu_DrawSelectionRow(s32 arg, s32 value, s16 *sub);

s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode) {
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
    Menu_DrawSelectionRow(arg, value, sub);
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
    Menu_DrawSelectionRow(arg, value, sub);
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
    Menu_DrawSelectionRow(arg, value, sub);
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
    Menu_DrawSelectionRow(arg, value, sub);
  }
  return value;
}

// Menu: flag list (move the cursor and press A to toggle).

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
#include "window.h"
#include "uiflow.h"

/// Shows 16x16 flags per page and takes input until closed, starting on page
/// 8. Returns 0.
s32 Menu_RunSelectionWithCursorObject(void) {
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
  Menu_DrawFlagBitTable(win, page);
  UiTextResource_Initialize(obj, &res);
  for (;;) {
    wait(1);
    ret = Menu_HandleFlagGridInput(win, &page, cur);
    if (ret == -1)
      break; // Close.
    if (ret == 1)
      Menu_DrawFlagBitTable(win, page); // A flag or the page changed.
    UiTextResource_SetPosition(obj, cur[0] * 8 + 58, cur[1] * 8 + 20);
  }
  UiTextResource_Release(res);
  Window_Close(win, 2);
  return 0;
}

/// Writes the lowest count digits of value (up to five), most significant
/// first, to out as uppercase hexadecimal, without a terminating zero.
void Text_FormatHex(u32 value, u32 count, u8 *out) {
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

extern const u8 gMenu_HexDigitsString[];

extern const u8 gMenu_ColonString[];

void WindowSprite_PrepareForRedraw(void);
void Text_DrawStringInWindow(s32 text, s32 window, s32 x, s32 y);
s32 GameFlag_Test(s32 flag);

void Menu_DrawFlagBitTable(s32 window, s32 startFlag) {
  s32 row;
  s32 y;
  s32 flag;
  u8 label[5];
  char bits[17];

  WindowSprite_PrepareForRedraw();
  Text_DrawStringInWindow((s32)gMenu_HexDigitsString, window, 48, 0);

  flag = startFlag << 8;
  for (row = 0; row != 16; row++) {
    s32 i;

    y = row * 8 + 16;

    for (i = 0; i != 5; i++) {
      label[i] = 0;
    }
    Text_FormatHex(flag, 3, label);
    Text_DrawStringInWindow((s32)label, window, 0, y);
    Text_DrawStringInWindow((s32)gMenu_ColonString, window, 32, y);

    for (i = 0; i < 16; i++) {
      s32 val = GameFlag_Test(flag);
      bits[i] = (val != 0) + 48;
      flag++;
    }
    bits[i] = 0;
    Text_DrawStringInWindow((s32)bits, window, 48, y);
  }
}

s32 GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

s32 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor) {
  s32 *row = &cursor[1];

  if (gInput.repeat & 1) {
    s32 flag = ((*page << 4) + *row << 4) + cursor[0];

    if (GameFlag_Test(flag))
      GameFlag_ClearBit(flag);
    else
      GameFlag_SetBit(flag);
    return 1;
  }
  if ((gInput.pressed & 2) || (gInput.repeat & 4))
    return -1;
  if (gInput.repeat & 0x40) {
    if (--*row < 0)
      *row = 15;
  } else if (gInput.repeat & 0x80) {
    if (++*row > 15)
      *row = 0;
  } else if (gInput.repeat & 0x20) {
    if (--cursor[0] < 0)
      cursor[0] = 15;
  } else if (gInput.repeat & 0x10) {
    if (++cursor[0] > 15)
      cursor[0] = 0;
  } else if ((gInput.repeat & 0x200) && (gInput.repeat & 8)) {
    *page -= 10;
    if (*page < 0)
      *page = 15;
    return 1;
  } else if ((gInput.repeat & 0x100) && (gInput.repeat & 8)) {
    *page += 10;
    if (*page > 15)
      *page = 0;
    return 1;
  } else if (gInput.repeat & 0x200) {
    if (--*page < 0)
      *page = 15;
    return 1;
  } else if (gInput.repeat & 0x100) {
    if (++*page > 15)
      *page = 0;
    return 1;
  }
  return 0;
}

void Menu_ReservedNoOp(void) {}

void Djinn_SetFoundFlagAndAdd(s32 arg0, s32 arg1, s32 arg2) {
  GameFlag_SetBit((arg1 * 0x14) + arg2 + 0x30);
  Djinn_AddToOwnerFar(arg0, arg1, arg2);
}

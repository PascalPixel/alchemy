// Draft, differ 422 of 1077, skeleton gap 50 (tla-en, 2026-11-01; ja and fr
// differ in layout): the password entry screen: a 17 by 4 character grid, the
// entered password in pages of 50 and its check before the clear data is
// loaded. Settled: WindowSprite_Create takes five arguments and
// OUTPUT_SIZE_16X16, the two cursors are 16-byte ShopCursor locals, gInput is
// the tree's InputState, the discarded cursor % 10 is a call, the code read
// from the grid is s32, the key test is (keys << 16) & 0x10000, the first
// column step shares column + 1, the page position is -pages + 18 in the native
// but -pages then += 18 measures better here, and the page, cell and wait loops
// are goto loops (their constants stay inside).
// Closed this round, and worth keeping: the static inlines give every cursor
// use its add rN, sp at the call, since an inlined parameter is substituted as
// (plus sfp N) and PRE sees no pseudo set; the sprite local puts the address
// after the create call. The stack layout is now the native's (frame 124,
// column spilled). shared is one s32 across the native's r8 (x, the mask 7, the
// result box, the cell pointer) and action across sl (with size and count); as
// separate variables each is caller-saved or takes a callee register, and
// column stays in r11. value is the native's second r6 (message, frame, size
// less 2): in i itself i falls one reference short of 128 and loses r6 to the
// clearing pointer. held is a set-once pointer used only by the four direction
// tests, which reload rematerializes; a function-wide one rematerializes the
// keys too. entry holds the main work base because a set-once local there is
// folded back to pool loads. The nested length test gives the second ldr of
// mode.
// Left, in this order:
// 1. window takes fp here and r9 there, page and the i * 4 giv the reverse, and
// action takes r9 for sl. global.c allocates page (10 refs / 75, 0.40) and the
// giv (9 / 74, 0.36) before window (43 / 724, 0.30), so page takes r10 and the
// giv r9. The native needs window first, and no plain form found lowers both.
// By hand count, not measured: without loop weights all three orders come out
// right (page 6, giv 5, and x and the mask stop being caller-save profitable
// with no sharing at all), which points at unweighted refs, as optimize_size
// gives. That is a compiler question (K3), not tried here; shared, action and
// value may be artefacts of it.
// 2. The first clearing loop enters at its bottom test only so PRE leaves
// work + 2 inside the loop; this adds a b.n the native lacks. A top-entered
// goto loop hoists it (435), a for measured 551, and four address spellings
// 480. Something else must stop that hoist.
// 3. Reload's rotation leaves the native at the Iwram_ClearWords in the cell
// setup loop: the store takes r1 for work here and r3 there, so r3 is overwritten
// by 99 and work is reloaded in the native. Most of the rest is that rotation.
#include "types.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "dma.h"
#include "input.h"
#include "uiwindow.h"
#include "oam.h"
#include "fixed_math.h"

/// A selection cursor, as the shops keep theirs (shop/cursor_set.c).
struct ShopCursor {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

struct PasswordWork {
  void *glyphs[50];
  void *cells[50];
  u8 shown[50];
  u8 unk1c2[26];
};

extern u32 gVBlankCount;
extern s8 gData_02002438[];
extern u8 gData_02002440[];
extern u8 gData_02002454[];
extern u8 gData_02002468[];
extern u8 gData_0200247c[];
extern u8 gData_0200265e[];
extern u8 Data_0200274a[];
extern u8 Data_0200288a[];
extern u8 gGameFlagBytes[];
extern u8 Data_02012f94[];
extern char MsgFieldSpace[];
extern char MsgFieldPasswordAccepted[];
extern char MsgFieldInvalidPassword[];

#define MSGNO(name) ((s32)(name))

void *Heap_AllocEwram(s32 size);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 mode);
void Window_DrawDividerLine(struct Window *window, s32 x, s32 y, s32 length,
                            s32 mode);
void Window_SetTilemapEntry(struct Window *window, s32 tile, s32 x, s32 y,
                            s32 mode);
void Text_DrawStringInWindow(const u8 *text, struct Window *window, s32 x,
                             s32 y);
void Text_DrawCharacterAtOffset(s32 message, struct Window *window, s32 x,
                                s32 y);
void Window_Close(struct Window *window, s32 release);
void WindowSprite_RedrawSavedRect(struct Window *window);
s32 Resource_FindFreeEntry(void);
void Party_RemoveActiveOwner(s32 id);
void Party_AddActiveOwner(s32 id);
void wait(s32 frames);
void Func_02000a70(s32 cell, u16 *text);
void Func_02000db4(struct Window *window, s32 x, s32 y, s32 width, s32 height,
                   s32 palette);
u16 Func_0200217c(u32 length, u8 *p);
void Func_020018e0(s32 mode, u8 *data);
s32 Func_02002068(u8 *src, s32 length, u8 *dst);
void Func_02002208(void);
s32 Func_020022b0(s32 mode);
void Func_020022f8(s32 mode);
void Func_02002300(void);
void Func_02002308(void);
void Func_02002310(s32 hue, s32 brightness);
void Func_02002320(void *vram);
void Func_02002328(s32 glyph, void *cell);
void *Func_02002330(struct Window *window, void *glyph, s32 x, s32 y);
void Func_02002350(void);
void Func_020023e8(void);
void Func_020023f0(struct ShopCursor *cursor);
void Func_020023f8(struct ShopCursor *cursor, s32 x, s32 y, s32 kind);
void Func_02002400(struct ShopCursor *cursor, s32 x, s32 y);
void Func_02002408(s32 cue);

/// Gives a cursor its sprite.
static inline void Cursor_Attach(struct ShopCursor *cursor,
                                 struct WindowSprite *sprite) {
  cursor->anchor = sprite;
}

/// Sets a cursor where it starts.
static inline void Cursor_Place(struct ShopCursor *cursor, s32 x, s32 y) {
  Func_02002400(cursor, x, y);
}

/// Moves a cursor and shows it.
static inline void Cursor_Show(struct ShopCursor *cursor, s32 x, s32 y) {
  Func_020023f8(cursor, x, y, 1);
  Func_020023f0(cursor);
}

/// Draws the password screen's title and key help into the main window.
#define DRAW_TITLE(window)                                                     \
  {                                                                            \
    Text_DrawStringInWindow(gData_02002440, window, 0, 0);                     \
    Text_DrawStringInWindow(gData_02002454, window, 0, 8);                     \
    Text_DrawStringInWindow(gData_02002468, window, 0, 16);                    \
    Text_DrawStringInWindow(gData_0200247c, window, 0, 24);                    \
    value = MSGNO(MsgFieldSpace);                                              \
    Window_DrawDividerLine(window, 20, 0, 20, 5);                              \
    Text_DrawCharacterAtOffset(value++, window, 160, 0);                       \
    Text_DrawCharacterAtOffset(value++, window, 160, 8);                       \
    Text_DrawCharacterAtOffset(value, window, 160, 24);                        \
  }

s32 Func_02000e44(void) {
  struct ShopCursor cursorStore;
  struct ShopCursor gridStore;
  u16 glyph[12];
  s32 mode;
  s32 cursor;
  struct Window *grid;
  s32 column;
  s32 row;
  s32 length;
  s32 pages;
  struct PasswordWork *work;
  s32 drawColumn;
  s32 drawRow;
  s32 drawMode;
  s32 cursorX;
  s32 cursorY;
  s32 state;
  s32 advance;
  struct Window *window;
  s32 shared;
  s32 action;
  s32 page;
  s32 i;
  s32 value;
  s32 entry;
  struct InputState *held;
  struct WindowSprite *sprite;
  u16 *palette;

  Func_02002208();
  Func_02002310(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  palette = (u16 *)0x050001c0;
  Dma_Set((const void *)0x050001e0, palette, (DMA_ENABLE | DMA_32BIT) << 16 | 8,
          (volatile u32 *)0x040000d4);
  palette[4] = 0x6318;
  mode = Func_020022b0(1);
  if (mode == -1)
    return -1;
  Func_02002408(71);
  cursor = 0;
  column = 0;
  row = 0;
  work = Heap_AllocEwram(476);
  advance = 0;
  Func_02002320((void *)0x06006000);
  if (mode == 0) {
    length = 260;
  } else {
    length = 61;
    if (mode != 1)
      length = 16;
  }
  pages = length / 50 + 1;
  window = Window_Create(1, 13, 29, 6, 2);
  DRAW_TITLE(window)
  grid = Window_Create(5, 1, 20, 12, 2);
  i = 0;
  goto clear_test;
clear_unknown:
  work->unk1c2[i] = 0;
  i++;
clear_test:
  if (i != 26)
    goto clear_unknown;
  {
    u8 *p = Data_0200274a;

    i = 0;
  clear_entered:
    *p = 99;
    i++;
    p++;
    if (i != 260)
      goto clear_entered;
  }
  for (i = 0; i != 50; i++) {
    s32 col = i % 10;
    s32 y;

    shared = col * 12 + 8;
    y = i / 10 * 16 + 2;
    if (col > 4)
      shared = col * 12 + 18;
    work->cells[i] = Func_02002330(grid, work->glyphs[i], shared, y);
    work->shown[i] = 99;
    Iwram_ClearWords(work->cells[i], 128);
  }
  entry = Resource_FindFreeEntry();
  if (entry <= 95) {
    Func_02002300();
    sprite = WindowSprite_Create(entry, OUTPUT_SIZE_16X16,
                                 (struct RenderInput *)window, 0, 0);
    Cursor_Attach(&cursorStore, sprite);
    Cursor_Place(&cursorStore, (s16)window->x * 8 - 2,
                  (s16)window->y * 8 + 12);
  }
  entry = Resource_FindFreeEntry();
  if (entry <= 95) {
    Func_02002308();
    sprite = WindowSprite_Create(entry, OUTPUT_SIZE_16X16,
                                 (struct RenderInput *)window, 0, 0);
    Cursor_Attach(&gridStore, sprite);
    sprite->sentinel = 255;
    sprite->oam.priority = 0;
    Cursor_Place(&gridStore, (s16)grid->x * 8 - 4, (s16)grid->y * 8 + 12);
  }
  held = &gInput;
  drawMode = 1;
  drawColumn = 0;
  drawRow = 0;
  cursorX = (s16)window->x * 8 - 8;
  cursorY = (s16)window->y * 8 + 11;
  state = 0;
  for (;;) {
    action = -1;
    if (state == 2)
      state = 1;
    Func_02000db4(window, drawColumn, drawRow, drawMode, 1, 15);
    if (state == 0) {
      if (held->repeat & 16) {
        Func_02002408(111);
        {
          s32 next = column + 1;

          if (row == 2)
            column = next % 16;
          else
            column = next % 17;
        }
      }
      if (held->repeat & 32) {
        Func_02002408(111);
        if (row == 2)
          column = (column + 15) % 16;
        else
          column = (column + 16) % 17;
      }
      if (held->repeat & 128) {
        Func_02002408(111);
        row = (row + 1) % 4;
        if (column == 16 && row == 2)
          row = 3;
      }
      if (held->repeat & 64) {
        Func_02002408(111);
        row = (row + 3) % 4;
        if (column == 16 && row == 2)
          row = 1;
      }
    }
    drawMode = 6;
    if (column != 16)
      drawMode = 1;
    drawColumn = column;
    drawRow = row;
    if (column > 4)
      drawColumn++;
    if (column > 9)
      drawColumn++;
    if (column > 14)
      drawColumn++;
    if (column > 15)
      drawColumn++;
    Func_02000db4(window, drawColumn, row, drawMode, 1, 14);
    {
      s32 x = ((s16)window->x + column) * 8 - 8;
      s32 y = ((s16)window->y + row) * 8 + 11;

      if (column > 4)
        x = ((s16)window->x + column) * 8;
      if (column > 9)
        x += 8;
      if (column > 14)
        x += 8;
      if (column > 15)
        x += 8;
      cursorX += (x - cursorX) / 2;
      cursorY += (y - cursorY) / 2;
    }
    Cursor_Show(&cursorStore, cursorX, cursorY);
    {
      s32 cell = cursor % 50;
      s32 y = (cell / 10 * 2 + (s16)grid->y) * 8 - 2;
      s32 col;
      s32 x;

      shared = 7;
      value = (gVBlankCount >> 1) & shared;
      col = cell % 10;
      x = col * 12 + (s16)grid->x * 8 + 16;

      if (col > 4)
        x += 10;
      x += gData_02002438[value];
      y += gData_02002438[(value + 5) & shared];
      Cursor_Show(&gridStore, x, y);
    }
    if (advance > 0 && ++advance == 8) {
      advance = 0;
      cursor++;
    }
    if (advance == 0) {
      s16 keys;
      struct InputState *input = &gInput;

      if (column == 16 && row == 0)
        keys = input->repeat;
      else if (column == 16 && row == 1)
        keys = input->repeat;
      else
        keys = input->pressed;
      if ((keys << 16) & 0x10000 && state == 0) {
        action = 0;
        if (column == 16) {
          if (row == 0)
            action = 1;
          else if (row == 1)
            action = 2;
          else if (row == 3)
            action = 3;
        } else if (cursor < length) {
          s32 code = gData_0200265e[column + row * 16];

          if (code <= 63) {
            Func_02002408(112);
            Data_0200274a[cursor] = code;
            // The native calls the remainder and reads nothing back.
            __modsi3(cursor, 10);
            if (cursor < length - 1) {
              if (cursor % 50 == 49)
                advance = 1;
              else
                cursor++;
            } else {
              state = 2;
            }
          }
        }
      }
      if (action != 0 || state == 1) {
        if (gInput.pressed & 8 || action == 3 || state == 1) {
          u16 check;
          s16 crc;

          action = Func_02002068(Data_0200274a, length, Data_0200288a);
          value = action - 2;
          check = Func_0200217c(value, Data_0200288a);

          state = 0;
          shared = (s32)Window_Create(5, 12, 20, 3, 2);
          crc = check;
          if (Data_0200288a[value] == (u16)crc >> 8 &&
              Data_0200288a[action - 1] == (u16)crc % 256) {
            Text_DrawCharacterAtOffset(MSGNO(MsgFieldPasswordAccepted), (struct Window *)shared,
                                       0, 0);
            Func_02002408(112);
          wait_accepted:
            wait(1);
            if (!(gInput.pressed & 1))
              goto wait_accepted;
            Func_02002408(112);
            Func_02002350();
            entry = (s32)&gMainRoutineWork;
            ((struct MainRoutineWork *)entry)->currentOwner = 0;
            Party_RemoveActiveOwner(4);
            Party_RemoveActiveOwner(5);
            Party_RemoveActiveOwner(6);
            Party_RemoveActiveOwner(7);
            Party_AddActiveOwner(0);
            Party_AddActiveOwner(1);
            Party_AddActiveOwner(2);
            Party_AddActiveOwner(3);
            Func_020018e0(mode, Data_0200288a);
            Window_Close((struct Window *)shared, 2);
            Window_Close(grid, 2);
            Window_Close(window, 2);
            Func_02002310(((struct MainRoutineWork *)entry)->paletteGlow[0],
                          ((struct MainRoutineWork *)entry)->paletteGlow[1]);
            Func_02002408(74);
            wait(6);
            Func_020022f8(0);
            Func_020023e8();
            Iwram_CopyWords(Data_02012f94 + 0xac, gGameFlagBytes, 0xf40);
            return 0;
          }
          Func_02002408(113);
          Text_DrawCharacterAtOffset(MSGNO(MsgFieldInvalidPassword), (struct Window *)shared, 0,
                                     0);
        wait_rejected:
          wait(1);
          if (!(gInput.pressed & 1) && !(gInput.pressed & 2))
            goto wait_rejected;
          Window_Close((struct Window *)shared, 2);
          WindowSprite_RedrawSavedRect(window);
          DRAW_TITLE(window)
        } else if (gInput.repeat & 512 || action == 2) {
          if (cursor > 0) {
            Func_02002408(111);
            cursor--;
          }
        } else if (gInput.repeat & 256 || action == 1) {
          if (cursor < length - 1) {
            Func_02002408(111);
            cursor++;
          }
        } else if (gInput.pressed & 2) {
          if (cursor > 0) {
            Func_02002408(111);
            cursor--;
          } else {
            Func_02002408(113);
            Window_Close(grid, 2);
            Window_Close(window, 2);
            Func_02002408(74);
            return -1;
          }
        }
      }
    }
    page = cursor / 50;
    WindowSprite_RedrawSavedRect(grid);
    {
      s32 y;

      for (i = 0, y = 2; i != 4; i++, y += 2)
        Window_DrawDividerLine(grid, 0, y, 18, y);
    }
    {
      s32 x;

      i = 0;
      if (pages != 0) {
        x = -pages;
        x += 18;
      draw_page:
        {
          s32 tile = i + 0xf301;

          if (i == page)
            tile = i + 0xf30b;
          Window_SetTilemapEntry(grid, tile, x, 10, 0);
        }
        i++;
        x++;
        if (i != pages)
          goto draw_page;
      }
    }
    action = length;
    if (length > 49) {
      if (pages - 1 == page)
        action = length % 50;
      else
        action = 50;
    }
    i = 0;
    if (action != 0) {
      u8 *shown;

      shared = (s32)work->cells;
      shown = work->shown;

    draw_cell:
      {
        s32 code = Data_0200274a[page * 50 + i];

        if (code != *shown) {
          *shown = code;
          if (code == 99) {
            Iwram_ClearWords(*(void **)shared, 128);
          } else {
            Func_02000a70(Data_0200274a[page * 50 + i], glyph);
            Func_02002328(glyph[0], *(void **)shared);
          }
        }
      }
      i++;
      shared += 4;
      shown++;
      if (i != action)
        goto draw_cell;
    }
    if (i != 50) {
      void **cell = &work->cells[i];
      u8 *shown = &work->shown[i];

    clear_cell:
      if (*shown != 99) {
        *shown = 99;
        Iwram_ClearWords(*cell, 128);
      }
      i++;
      cell++;
      shown++;
      if (i != 50)
        goto clear_cell;
    }
    wait(1);
  }
}

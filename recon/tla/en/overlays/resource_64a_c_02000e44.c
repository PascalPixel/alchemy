// Draft, differ 961 of 1077, skeleton gap 193 (tla-en, 2026-11-01; ja and fr
// differ in layout): the password entry screen: a 17 by 4 character grid, the
// entered password in pages of 50 and its check before the clear data is
// loaded. Settled: WindowSprite_Create takes five arguments and
// OUTPUT_SIZE_16X16, the two cursors are 16-byte ShopCursor locals, gInput is
// the tree's InputState, the discarded cursor % 10 is a call, the code read
// from the grid is s32, the key test is (keys << 16) & 0x10000, the first
// column step shares column + 1, the page position is -pages + 18, and the
// clearing, page, cell and both wait loops are goto loops (their constants stay
// inside). Left is nearly all allocation, in this order:
// 1. column takes r11 here; the native spills it, so some 45 lines read mov
// for ldr or str. There the cell loop fills r5 to r11, with x in r8; here x
// crosses one call with 6 refs, so global.c gives it r2 with caller saves
// (4 * calls < refs) and r11 stays free. Native x may not have been caller-save
// profitable; fewer refs or a second crossed call would settle it. A goto cell
// loop with shown and offset variables measured 217, a separate near pseudo
// 221, and a goto main loop 210.
// 2. page takes r10 here and fp there, after window in r9: native page is
// lower priority, which floor_log2 gives under 8 refs; here it has 10.
// 3. The native recomputes add rN, sp, #imm at every use of the two cursors,
// and stores through r0, which is what reload does for a pseudo with REG_EQUIV.
// Here the address is one pseudo spilled to a slot (frame 136, not 124): no
// REG_EQUAL reaches it, since CSE notes only a second sighting in a block and
// copy propagation folds a copied pointer. &local at each use measured 200, and
// PRE then joins the first test's address to the main loop's.
// 4. gInput's address is loaded once before the key selection in the native;
// a pointer local measured 197 and a ternary 199. The final palette glow reads
// through a pool constant because the base pseudo is not allocated.
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

/// Draws the password screen's title and key help into the main window.
#define DRAW_TITLE(window)                                                     \
  {                                                                            \
    s32 message;                                                               \
                                                                               \
    Text_DrawStringInWindow(gData_02002440, window, 0, 0);                     \
    Text_DrawStringInWindow(gData_02002454, window, 0, 8);                     \
    Text_DrawStringInWindow(gData_02002468, window, 0, 16);                    \
    Text_DrawStringInWindow(gData_0200247c, window, 0, 24);                    \
    message = MSGNO(MsgFieldSpace);                                            \
    Window_DrawDividerLine(window, 20, 0, 20, 5);                              \
    Text_DrawCharacterAtOffset(message++, window, 160, 0);                     \
    Text_DrawCharacterAtOffset(message++, window, 160, 8);                     \
    Text_DrawCharacterAtOffset(message, window, 160, 24);                      \
  }

s32 Func_02000e44(void) {
  struct ShopCursor cursorStore;
  struct ShopCursor gridStore;
  u16 glyph[12];
  struct ShopCursor *cursorSprite;
  struct ShopCursor *gridSprite;
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
  struct Window *box;
  s32 action;
  s32 page;
  s32 count;
  s32 i;
  s32 entry;
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
  length = 260;
  if (mode != 0) {
    length = 61;
    if (mode != 1)
      length = 16;
  }
  pages = length / 50 + 1;
  window = Window_Create(1, 13, 29, 6, 2);
  DRAW_TITLE(window)
  grid = Window_Create(5, 1, 20, 12, 2);
  i = 0;
clear_unknown:
  work->unk1c2[i] = 0;
  i++;
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
    s32 x = col * 12 + 8;
    s32 y = i / 10 * 16 + 2;

    if (col > 4)
      x = col * 12 + 18;
    work->cells[i] = Func_02002330(grid, work->glyphs[i], x, y);
    work->shown[i] = 99;
    Iwram_ClearWords(work->cells[i], 128);
  }
  cursorSprite = &cursorStore;
  gridSprite = &gridStore;
  entry = Resource_FindFreeEntry();
  if (entry <= 95) {
    Func_02002300();
    cursorSprite->anchor = WindowSprite_Create(
        entry, OUTPUT_SIZE_16X16, (struct RenderInput *)window, 0, 0);
    Func_02002400(cursorSprite, (s16)window->x * 8 - 2,
                  (s16)window->y * 8 + 12);
  }
  entry = Resource_FindFreeEntry();
  if (entry <= 95) {
    Func_02002308();
    gridSprite->anchor = WindowSprite_Create(
        entry, OUTPUT_SIZE_16X16, (struct RenderInput *)window, 0, 0);
    gridSprite->anchor->sentinel = 255;
    gridSprite->anchor->oam.priority = 0;
    Func_02002400(gridSprite, (s16)grid->x * 8 - 4, (s16)grid->y * 8 + 12);
  }
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
      if (gInput.repeat & 16) {
        Func_02002408(111);
        {
          s32 next = column + 1;

          if (row == 2)
            column = next % 16;
          else
            column = next % 17;
        }
      }
      if (gInput.repeat & 32) {
        Func_02002408(111);
        if (row == 2)
          column = (column + 15) % 16;
        else
          column = (column + 16) % 17;
      }
      if (gInput.repeat & 128) {
        Func_02002408(111);
        row = (row + 1) % 4;
        if (column == 16 && row == 2)
          row = 3;
      }
      if (gInput.repeat & 64) {
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
    Func_020023f8(cursorSprite, cursorX, cursorY, 1);
    Func_020023f0(cursorSprite);
    {
      s32 cell = cursor % 50;
      s32 y = (cell / 10 * 2 + (s16)grid->y) * 8 - 2;
      s32 frame = (gVBlankCount >> 1) & 7;
      s32 col = cell % 10;
      s32 x = col * 12 + (s16)grid->x * 8 + 16;

      if (col > 4)
        x += 10;
      Func_020023f8(gridSprite, x + gData_02002438[frame],
                    y + gData_02002438[(frame + 5) & 7], 1);
      Func_020023f0(gridSprite);
    }
    if (advance > 0 && ++advance == 8) {
      advance = 0;
      cursor++;
    }
    if (advance == 0) {
      s16 keys;

      if (column == 16 && row == 0)
        keys = gInput.repeat;
      else if (column == 16 && row == 1)
        keys = gInput.repeat;
      else
        keys = gInput.pressed;
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
          s32 size = Func_02002068(Data_0200274a, length, Data_0200288a);
          u16 check = Func_0200217c(size - 2, Data_0200288a);
          s16 crc;

          state = 0;
          box = Window_Create(5, 12, 20, 3, 2);
          crc = check;
          if (Data_0200288a[size - 2] == (u16)crc >> 8 &&
              Data_0200288a[size - 1] == (u16)crc % 256) {
            Text_DrawCharacterAtOffset(MSGNO(MsgFieldPasswordAccepted), box,
                                       0, 0);
            Func_02002408(112);
          wait_accepted:
            wait(1);
            if (!(gInput.pressed & 1))
              goto wait_accepted;
            Func_02002408(112);
            Func_02002350();
            gMainRoutineWork.currentOwner = 0;
            Party_RemoveActiveOwner(4);
            Party_RemoveActiveOwner(5);
            Party_RemoveActiveOwner(6);
            Party_RemoveActiveOwner(7);
            Party_AddActiveOwner(0);
            Party_AddActiveOwner(1);
            Party_AddActiveOwner(2);
            Party_AddActiveOwner(3);
            Func_020018e0(mode, Data_0200288a);
            Window_Close(box, 2);
            Window_Close(grid, 2);
            Window_Close(window, 2);
            Func_02002310(gMainRoutineWork.paletteGlow[0],
                          gMainRoutineWork.paletteGlow[1]);
            Func_02002408(74);
            wait(6);
            Func_020022f8(0);
            Func_020023e8();
            Iwram_CopyWords(Data_02012f94 + 0xac, gGameFlagBytes, 0xf40);
            return 0;
          }
          Func_02002408(113);
          Text_DrawCharacterAtOffset(MSGNO(MsgFieldInvalidPassword), box, 0,
                                     0);
        wait_rejected:
          wait(1);
          if (!(gInput.pressed & 1) && !(gInput.pressed & 2))
            goto wait_rejected;
          Window_Close(box, 2);
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
    count = length;
    if (length > 49) {
      if (pages - 1 == page)
        count = length % 50;
      else
        count = 50;
    }
    i = 0;
    if (count != 0) {
      void **cell = work->cells;
      u8 *shown = work->shown;

    draw_cell:
      {
        s32 code = Data_0200274a[page * 50 + i];

        if (code != *shown) {
          *shown = code;
          if (code == 99) {
            Iwram_ClearWords(*cell, 128);
          } else {
            Func_02000a70(Data_0200274a[page * 50 + i], glyph);
            Func_02002328(glyph[0], *cell);
          }
        }
      }
      i++;
      cell++;
      shown++;
      if (i != count)
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

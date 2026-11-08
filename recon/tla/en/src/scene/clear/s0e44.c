#include "edition.h"

// The Japanese and French forms differ and are still on their listing.
#if (LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_FRENCH)

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
  s32 action;
  s32 page;
  s32 i;
  s32 message;
  s32 frame;
  s32 size2;
  s32 entry;
  struct WindowSprite *sprite;
  u16 *palette;
  s32 height;

  Func_02002208();
  Func_02002310(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  palette = (u16 *)0x050001c0;
  Dma_Set((const void *)0x050001e0, palette, (DMA_ENABLE | DMA_32BIT) << 16 | 8,
          (volatile u32 *)SPLIT_CONSTANT(0x4000000, 212));
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
  for (i = 0; i != 26; i++)
    work->unk1c2[i] = 0;
  for (i = 0; i != 260; i++)
    Data_0200274a[i] = 99;
  for (i = 0; i != 50; i++) {
    s32 col = i % 10;
    s32 y;
    s32 x;

    x = col * 12 + 8;
    y = i / 10 * 16 + 2;
    if (col > 4)
      x = col * 12 + 18;
    work->cells[i] = Func_02002330(grid, work->glyphs[i], x, y);
    work->shown[i] = 99;
    Iwram_ClearWords(work->cells[i], 128);
  }
  entry = Resource_FindFreeEntry();
  if (entry <= 95) {
    Func_02002300();
    sprite = WindowSprite_Create(entry, OUTPUT_SIZE_16X16,
                                 (struct RenderInput *)window, 0, 0);
    Cursor_Attach(&cursorStore, sprite);
    Cursor_Place(&cursorStore, (s16)window->x * 8 - 2, (s16)window->y * 8 + 12);
  }
  entry = Resource_FindFreeEntry();
  if (entry <= 95) {
    Func_02002308();
    sprite = WindowSprite_Create(entry, OUTPUT_SIZE_16X16,
                                 (struct RenderInput *)window, 0, 0);
    Cursor_Attach(&gridStore, sprite);
    sprite->sentinel = 255;
    sprite->sprite.oam.priority = 0;
    Cursor_Place(&gridStore, (s16)grid->x * 8 - 4, (s16)grid->y * 8 + 12);
  }
  height = 1;
  drawColumn = 0;
  drawRow = 0;
  drawMode = 1;
  cursorX = (s16)window->x * 8 - 8;
  cursorY = (s16)window->y * 8 + 11;
  state = 0;
  for (;;) {
    action = -1;
    if (state == 2)
      state = 1;
    Func_02000db4(window, drawColumn, drawRow, drawMode, height, 15);
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
        if (column != 16)
          goto not_side;
        if (row == 2)
          row = 1;
      }
    }
    drawMode = 6;
    if (column != 16) {
    not_side:
      drawMode = 1;
    }
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
    Func_02000db4(window, drawColumn, row, drawMode, height, 14);
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

      frame = (gVBlankCount >> 1) & 7;
      col = cell % 10;
      x = col * 12 + (s16)grid->x * 8 + 16;

      if (col > 4)
        x += 10;
      x += gData_02002438[frame];
      y += gData_02002438[(frame + 5) & 7];
      Cursor_Show(&gridStore, x, y);
    }
    if (advance > 0 && ++advance == 8) {
      advance = 0;
      cursor++;
    }
    if (advance == 0) {
      s32 mask;
      u16 raw;
      /* FAKEMATCH(pin): the keys in r2, as the native keeps them; as a plain
       * local, 4 of 1077 instructions differ in tla-en. */
      register s32 keys asm("r2");
      struct InputState *input = &gInput;

      if (column == 16 && row == 0)
        raw = input->repeat;
      else if (column == 16 && row == 1)
        raw = input->repeat;
      else
        raw = input->pressed;
      keys = (s16)raw;
      keys <<= 16;
      mask = 0x10000;
      mask &= keys;
      if (mask && state == 0) {
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
          struct Window *box;
          struct MainRoutineWork *party;
          s16 crc;

          action = Func_02002068(Data_0200274a, length, Data_0200288a);
          size2 = action - 2;
          check = Func_0200217c(size2, Data_0200288a);

          state = 0;
          box = Window_Create(5, 12, 20, 3, 2);
          crc = check;
          if (Data_0200288a[size2] == (u16)crc >> 8 &&
              Data_0200288a[action - 1] == (u16)crc % 256) {
            Text_DrawCharacterAtOffset(MSGNO(MsgFieldPasswordAccepted), box, 0,
                                       0);
            Func_02002408(112);
          wait_accepted:
            wait(1);
            if (!(gInput.pressed & 1))
              goto wait_accepted;
            Func_02002408(112);
            Func_02002350();
            party = &gMainRoutineWork;
            party->currentOwner = 0;
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
            Func_02002310(party->paletteGlow[0], party->paletteGlow[1]);
            Func_02002408(74);
            wait(6);
            Func_020022f8(0);
            Func_020023e8();
            Iwram_CopyWords(Data_02012f94 + 0xac, gGameFlagBytes, 0xf40);
            return 0;
          }
          Func_02002408(113);
          Text_DrawCharacterAtOffset(MSGNO(MsgFieldInvalidPassword), box, 0, 0);
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
      for (i = 0; i != pages; i++) {
        s32 tile = i + 0xf301;

        if (i == page)
          tile = i + 0xf30b;
        Window_SetTilemapEntry(grid, tile, i - pages + 18, 10, 0);
      }
    }
    action = length;
    /* FAKEMATCH(barrier): the count is not known to be the length, whose
     * remainder the native reloads from the stack; as no asm, 4 of 1077
     * instructions differ in tla-en. */
    asm volatile("" : "+r"(action));
    if (length > 49) {
      if (pages - 1 == page) {
        /* FAKEMATCH(barrier): r2 no longer holds the length; as no asm, 2 of
         * 1077 instructions differ in tla-en. */
        asm volatile("" : : : "r2");
        action = length % 50;
      } else
        action = 50;
    }
    for (i = 0; i != action; i++) {
      u8 *entered = Data_0200274a;
      s32 index = i + page * 50;
      s32 raw = entered[index];
      u8 code = raw;

      if (code != work->shown[i]) {
        work->shown[i] = raw;
        if (code == 99) {
          Iwram_ClearWords(work->cells[i], 128);
        } else {
          Func_02000a70(entered[index], glyph);
          Func_02002328(glyph[0], work->cells[i]);
        }
      }
    }
    for (; i != 50; i++) {
      if (work->shown[i] != 99) {
        work->shown[i] = 99;
        Iwram_ClearWords(work->cells[i], 128);
      }
    }
    wait(1);
  }
}

#endif

/// @file
/// Scene code of overlay 0x64a, in ROM order, from s0e44's offset up to the next object the overlay links.

#include "edition.h"
#include "types.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "dma.h"
#include "input.h"
#include "uiwindow.h"
#include "oam.h"
#include "fixed_math.h"
#include "event_inline.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "battle/battle_unit.h"
#include "owner_state.h"
#include "version.h"
#include "battle_unit.h"

/// A selection cursor, as the shops keep theirs (struct ShopCursor).
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
extern u8 Data_0200284e[];
extern u8 gGameFlagBytes[];
extern u8 Data_02012f94[];
extern char MsgFieldSpace[];
extern char MsgFieldPasswordAccepted[];
extern char MsgFieldInvalidPassword[];
extern char MsgFieldPasswordKana[];

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
void Func_02002320(void *vram);
void Func_02002328(s32 glyph, void *cell);
void *Func_02002330(struct Window *window, void *glyph, s32 x, s32 y);
void Func_02002350(void);
void Func_020023e8(void);
void Func_020023f0(struct ShopCursor *cursor);
void Func_020023f8(struct ShopCursor *cursor, s32 x, s32 y, s32 kind);
void Func_02002400(struct ShopCursor *cursor, s32 x, s32 y);

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

#if (LANGUAGE == LANGUAGE_JAPANESE)
// The Japanese grid has 20 columns of kana and its side column is 20.
#define SIDE_COLUMN 20
#define SIDE_HIGHLIGHT_WIDTH 3
#define CHECK_WINDOW_X 8
#define CHECK_WINDOW_WIDTH 14
// Japanese decodes into the bytes after its 260-byte entered password.
#define DECODED_PASSWORD Data_0200284e
#define CURSOR_ON_SIDE_COLUMN(column) ((column) == SIDE_COLUMN)
#else
#define SIDE_COLUMN 16
#define SIDE_HIGHLIGHT_WIDTH 6
#define CHECK_WINDOW_X 5
#define CHECK_WINDOW_WIDTH 20
#define DECODED_PASSWORD Data_0200288a
#define CURSOR_ON_SIDE_COLUMN(column) ((column) > SIDE_COLUMN - 1)
#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// Draws the kana rows and the key help into the main window.
#define DRAW_TITLE(window)                                                     \
  {                                                                            \
    message = MSGNO(MsgFieldPasswordKana);                                     \
    Text_DrawCharacterAtOffset(message++, window, 0, 0);                       \
    Text_DrawCharacterAtOffset(message++, window, 0, 8);                       \
    Text_DrawCharacterAtOffset(message++, window, 0, 16);                      \
    Text_DrawCharacterAtOffset(message++, window, 0, 24);                      \
    Text_DrawCharacterAtOffset(message++, window, 48, 0);                      \
    Text_DrawCharacterAtOffset(message++, window, 48, 8);                      \
    Text_DrawCharacterAtOffset(message++, window, 48, 16);                     \
    Text_DrawCharacterAtOffset(message++, window, 48, 24);                     \
    Text_DrawCharacterAtOffset(message++, window, 96, 0);                      \
    Text_DrawCharacterAtOffset(message++, window, 96, 8);                      \
    Text_DrawCharacterAtOffset(message++, window, 96, 16);                     \
    Text_DrawCharacterAtOffset(message++, window, 96, 24);                     \
    Text_DrawCharacterAtOffset(message++, window, 144, 0);                     \
    Text_DrawCharacterAtOffset(message++, window, 144, 8);                     \
    Window_DrawDividerLine(window, 24, 0, 24, 5);                              \
    Text_DrawCharacterAtOffset(message++, window, 192, 0);                     \
    Text_DrawCharacterAtOffset(message++, window, 192, 8);                     \
    Text_DrawCharacterAtOffset(message, window, 192, 24);                      \
  }
#else
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
#endif

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
  Func_0803f9c0(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  palette = (u16 *)0x050001c0;
  Dma_Set((const void *)0x050001e0, palette, (DMA_ENABLE | DMA_32BIT) << 16 | 8,
          (volatile u32 *)0x040000d4);
  palette[4] = 0x6318;
  mode = Func_020022b0(1);
  if (mode == -1)
    return -1;
  Sound_Play(71);
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
        Sound_Play(111);
        {
          s32 next = column + 1;

          if (row == 2)
            column = next % SIDE_COLUMN;
          else
            column = next % (SIDE_COLUMN + 1);
        }
      }
      if (gInput.repeat & 32) {
        Sound_Play(111);
        if (row == 2)
          column = (column + SIDE_COLUMN - 1) % SIDE_COLUMN;
        else
          column = (column + SIDE_COLUMN) % (SIDE_COLUMN + 1);
      }
      if (gInput.repeat & 128) {
        Sound_Play(111);
        row = (row + 1) % 4;
        if (column == SIDE_COLUMN && row == 2)
          row = 3;
      }
      if (gInput.repeat & 64) {
        Sound_Play(111);
        row = (row + 3) % 4;
        if (column != SIDE_COLUMN)
          goto not_side;
        if (row == 2)
          row = 1;
      }
    }
    drawMode = SIDE_HIGHLIGHT_WIDTH;
    if (column != SIDE_COLUMN) {
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
    if (column > SIDE_COLUMN - 1)
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
      if (CURSOR_ON_SIDE_COLUMN(column))
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
      s32 keys;
      struct InputState *input = &gInput;

      if (column == SIDE_COLUMN && row == 0)
        raw = input->repeat;
      else if (column == SIDE_COLUMN && row == 1)
        raw = input->repeat;
      else
        raw = input->pressed;
      keys = (s16)raw;
      keys <<= 16;
      mask = 0x10000;
      mask &= keys;
      if (mask && state == 0) {
        action = 0;
        if (column == SIDE_COLUMN) {
          if (row == 0)
            action = 1;
          else if (row == 1)
            action = 2;
          else if (row == 3)
            action = 3;
        } else if (cursor < length) {
          s32 code = gData_0200265e[column + row * SIDE_COLUMN];

          if (code <= 63) {
            Sound_Play(112);
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

          action = Func_02002068(Data_0200274a, length, DECODED_PASSWORD);
          size2 = action - 2;
          check = Func_0200217c(size2, DECODED_PASSWORD);

          state = 0;
          box = Window_Create(CHECK_WINDOW_X, 12, CHECK_WINDOW_WIDTH, 3, 2);
          crc = check;
          if (DECODED_PASSWORD[size2] == (u16)crc >> 8 &&
              DECODED_PASSWORD[action - 1] == (u16)crc % 256) {
            Text_DrawCharacterAtOffset(MSGNO(MsgFieldPasswordAccepted), box, 0,
                                       0);
            Sound_Play(112);
          wait_accepted:
            System_Wait(1);
            if (!(gInput.pressed & 1))
              goto wait_accepted;
            Sound_Play(112);
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
            Func_020018e0(mode, DECODED_PASSWORD);
            Window_Close(box, 2);
            Window_Close(grid, 2);
            Window_Close(window, 2);
            Func_0803f9c0(party->paletteGlow[0], party->paletteGlow[1]);
            Sound_Play(74);
            System_Wait(6);
            Func_020022f8(0);
            Func_020023e8();
            Iwram_CopyWords(Data_02012f94 + 0xac, gGameFlagBytes, 0xf40);
            return 0;
          }
          Sound_Play(113);
          Text_DrawCharacterAtOffset(MSGNO(MsgFieldInvalidPassword), box, 0, 0);
        wait_rejected:
          System_Wait(1);
          if (!(gInput.pressed & 1) && !(gInput.pressed & 2))
            goto wait_rejected;
          Window_Close(box, 2);
          WindowSprite_RedrawSavedRect(window);
          DRAW_TITLE(window)
        } else if (gInput.repeat & 512 || action == 2) {
          if (cursor > 0) {
            Sound_Play(111);
            cursor--;
          }
        } else if (gInput.repeat & 256 || action == 1) {
          if (cursor < length - 1) {
            Sound_Play(111);
            cursor++;
          }
        } else if (gInput.pressed & 2) {
          if (cursor > 0) {
            Sound_Play(111);
            cursor--;
          } else {
            Sound_Play(113);
            Window_Close(grid, 2);
            Window_Close(window, 2);
            Sound_Play(74);
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
     * remainder the native reloads from the stack; as no asm, 2 of 1077
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
    System_Wait(1);
  }
}

struct Panel_02001804 {
  u8 unk00[8];
  u16 stride;
  u8 unk0a[2];
  s16 x;
  s16 y;
};

void Func_020021f8(s32 source, void *work);

/// Unpacks source into a scratch block, then writes a 16 by 8 block of tiles
/// at the panel's cell into the BG map at 0x06002000 and into the window
/// tilemap's shadow: palette 15, the tile of each cell the row times the
/// panel's stride plus the column. The loops are the listing's: no loop pass
/// touches them, so the tile is rebuilt and the mask reloaded on every pass.
void Func_02001804(struct Panel_02001804 *panel, s32 sourceArg) {
  /* FAKEMATCH(pin): the shadow pointer in r5; as a plain local, 7 of 53
   * instructions differ in tla-en. */
  register u16 *shadow asm("r5") = (u16 *)Ram_HeapSlots->windowTiles;
  void *work = Heap_AllocEwram(0x300);
  u16 *tiles;
  s32 offset;
  s32 row;
  s32 column;

  Func_020021f8(sourceArg, work);
  offset = panel->y * 32 + panel->x;

  offset *= 2;
  tiles = (u16 *)(offset + 0x06002000);
  shadow = (u16 *)((u8 *)shadow + offset + 8);
  row = 0;
next_row:
  column = 0;
next_column:
  // One cell, the same tile in the map and in the shadow.
  {
    s16 tile = (panel->stride * row + column) | 0xf000;

    *tiles++ = tile;
    *shadow++ = tile;
  }
  column++;
  if (column <= 15)
    goto next_column;
  tiles += 16;
  shadow += 16;
  row++;
  if (row <= 7)
    goto next_row;
  Heap_Release(work);
}

void Func_0200187c(struct Panel_02001804 *panel) {
  /* FAKEMATCH(pin): the shadow pointer in r5; as a plain local, 23 of 43
   * instructions differ in tla-en. */
  register u16 *shadow asm("r5") = (u16 *)Ram_HeapSlots->windowTiles;
  void *work = Heap_AllocEwram(0x300);
  u16 *tiles;
  s32 offset;
  s32 row;
  s32 column;

  offset = (panel->y * 32 + panel->x) * 2;
  tiles = (u16 *)(offset + 0x06002000);
  shadow = (u16 *)((u8 *)shadow + offset + 8);
  row = 0;
next_row:
  column = 0;
next_column:
  // One cell, the same tile in the map and in the shadow.
  {
    s16 tile = (row * 16 + column + 32) | 0xf000;

    *tiles++ = tile;
    *shadow++ = tile;
  }
  column++;
  if (column <= 15)
    goto next_column;
  tiles += 16;
  shadow += 16;
  row++;
  if (row <= 7)
    goto next_row;
  Heap_Release(work);
}

extern s32 gData_020026a0[];
extern u16 gData_020026b0[];
extern u16 gData_020026c0[];
extern u16 gData_02002490[];

s32 Unit_GetExperienceForLevel(s32 id, s32 level);
void PartyInventory_Add(s32 item);
void Unit_Calculate(s32 id);

void Func_020018e0(s32 mode, u8 *in) {
  u32 levelBits;
  u32 djinnBits;
  u8 flagBits;
  u8 keyItemBits;
  u32 packedStats[8];
  s32 i;
  s32 j;
  s32 n;
  s32 m;
  s32 position;
  s32 bit;

  levelBits = in[0] | in[1] << 8 | in[2] << 16 | (in[3] & 0xf0) << 20;
  djinnBits = (in[3] & 0x0f) | in[4] << 4 | in[5] << 12 | in[6] << 20;
  flagBits = in[7];
  keyItemBits = in[8];

  if (mode != 2) {
    s32 statsOffset = 8 + (mode != 0);

    for (i = 0; i != 2; i++) {
      packedStats[i * 4] = in[statsOffset] << 24 | in[statsOffset + 1] << 16 |
                           in[statsOffset + 2] << 8 | in[statsOffset + 3];
      packedStats[i * 4 + 1] = in[statsOffset + 4] << 24 |
                               in[statsOffset + 5] << 16 |
                               in[statsOffset + 6] << 8 | in[statsOffset + 7];
      packedStats[i * 4 + 2] =
          in[statsOffset + 7] << 28 | in[statsOffset + 8] << 20 |
          in[statsOffset + 9] << 12 | in[statsOffset + 10] << 4 |
          in[statsOffset + 11] >> 4;
      packedStats[i * 4 + 3] = in[statsOffset + 11] << 28 |
                               in[statsOffset + 12] << 20 |
                               in[statsOffset + 13] << 12 |
                               in[statsOffset + 14] << 4;
      statsOffset += 15;
    }
    for (i = 0; i != 4; i++) {
      s16 *stats = &((struct BattleUnit *)Unit_Get(gData_020026a0[i]))->baseHp;
      s32 at = i * 8;

#define WORD (*(u32 *)((u8 *)packedStats + at))
      stats[0] = WORD >> 21;
      stats[1] = WORD << 11 >> 21;
      stats[4] = WORD & 0x1ff;
      at += 4;
      stats[5] = WORD >> 22 & 0x1ff;
      stats[6] = WORD >> 12 & 0x1ff;
      ((u8 *)stats)[14] = WORD >> 4 & 0x7f;
#undef WORD
    }
  }

  for (i = 0; i != 6; i++) {
    if ((flagBits >> i) & 1)
      GameFlag_SetBit(gData_02002490[i]);
    else
      GameFlag_ClearBit(gData_02002490[i]);
  }

  for (i = 0; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

    *(u8 *)&unit->level = levelBits >> (i * 7) & 0x7f;
#if TLA_PATCH_1
    unit->experience = Unit_GetExperienceForLevel(i, unit->level);
#endif
  }

  Djinn_GetRecoveryTable(0)->count = 0;
  for (i = 0; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

    // The native clears the same 40 bytes, djinnAvailable through
    // djinnActiveCounts, four times.
    for (n = 0; n != 4; n++)
      Iwram_ClearWordsEntry(unit->djinnAvailable, 40);
  }

  for (i = 0; i != 28; i++) {
    s32 element = i / 7;
    s32 number = i % 7;

    GameFlag_ClearBit(element * 20 + number + 48);
  }

  for (i = 0; i != 28; i++) {
    if ((djinnBits >> i) & 1)
      Djinn_AddToLeastLoadedOwner(i / 7, i % 7);
  }

  if (mode != 0) {
    Unit_Get(gData_020026a0[0]);
    for (i = 0; i != 8; i++) {
      if ((keyItemBits >> i) & 1)
        PartyInventory_Add(gData_020026b0[i]);
    }
  }

  if (mode == 0) {
    position = 39;
    bit = 0;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

      for (n = 0; n != 15; n++) {
        unit->inventory[n] =
            (in[position] << (bit + 1) | in[position + 1] >> (7 - bit)) & 0x1ff;
        bit++;
        position++;
        if (bit == 7) {
          bit = 0;
          position++;
        }
      }
    }

    position = 107;
    bit = -1;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

      for (m = 0; m != 23; m++) {
        j = 0;
        if (bit < 0) {
          j = in[position] << -bit & 0x1f;
          position++;
          bit += 8;
        }
        j += in[position] >> bit & 0x1f;
        bit -= 5;
        if (bit == -5) {
          position++;
          bit = 3;
        }
        j &= 0x1f;
        for (n = 0; n != 15; n++) {
          if ((unit->inventory[n] & 0x1ff) == gData_020026c0[m])
            unit->inventory[n] |= j << 11;
        }
      }
    }

    gMainRoutineWork.coins = in[165] << 16 | in[166] << 8 | in[167];
  }

  for (i = 0; i != 4; i++) {
    Unit_UpdateClass(gData_020026a0[i]);
    Unit_Calculate(gData_020026a0[i]);
  }
}

struct UnitBaseStats {
  s16 hp;
  s16 pp;
  s16 hpGauge;
  s16 ppGauge;
  u16 attack;
  u16 defense;
  u16 agility;
  u8 luck;
};

extern u16 gData_02002700[];
extern s32 gData_020026f0[];
extern u16 gData_0200270c[];
extern u16 gData_0200271c[];

void *Unit_Get(s32 owner);

s32 Func_02001c7c(s32 unused, s32 mode, u8 *out) {
  s32 size;
  u32 levels;
  u32 djinn;
  u8 found;
  u8 flags;
  u32 stats[8];
  s32 i;
  s32 pos;
  s32 bit;
  s32 j;
  s32 k;
  s32 n;
  s32 m;

  size = 11;
  switch (mode) {
  case 0:
    size = 173;
    break;
  case 1:
    size = 39;
    break;
  case 2:
    size = 9;
    break;
  }
  for (i = 0; i != size; i++)
    out[i] = 0;
  levels = 0;
  djinn = 0;
  found = 0;
  flags = 0;
  for (i = 0; i != 8; i++)
    stats[i] = 0;
  for (i = 0; i != 6; i++) {
    if (GameFlag_Test(gData_02002700[i]))
      flags |= 1 << i;
  }
  i = 0;
  pos = 4;
  for (; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);
    struct UnitBaseStats *base = (struct UnitBaseStats *)&unit->baseHp;
    s32 value;

    if (base->hp > 1999)
      base->hp = 1999;
    value = (u16)base->hp;
    if ((s16)value < 0)
      base->hp = 0;
    if (base->pp > 1999)
      base->pp = 1999;
    value = (u16)base->pp;
    if ((s16)value < 0)
      base->pp = 0;
    if (base->attack > 999)
      base->attack = 999;
    value = base->attack;
    if (value < 0)
      value = 0;
    base->attack = value;
    if (base->defense > 999)
      base->defense = 999;
    value = base->defense;
    if (value < 0)
      value = 0;
    base->defense = value;
    if (base->agility > 999)
      base->agility = 999;
    value = base->agility;
    if (value < 0)
      value = 0;
    base->agility = value;
    if (base->luck > 99)
      base->luck = 99;
    value = base->luck;
    if (value < 0)
      value = 0;
    base->luck = value;
    stats[i * 2] = (u32)base->hp << 21 | base->pp << 10 | base->attack;
    *(u32 *)((u8 *)stats + pos) =
        (u32)base->defense << 22 | base->agility << 12 | base->luck << 4;
    if (unit->level > 99)
      unit->level = 99;
    value = unit->level;
    if (value == 0)
      unit->level = 1;
    levels |= unit->level << (i * 7);
    for (j = 0; j != 4; j++)
      djinn += unit->djinnAvailable[j] << (j * 7);
    for (j = 0; j != 15; j++) {
      s32 item = unit->inventory[j] & 0x1ff;

      for (k = 0; k != 8; k++) {
        if (item == gData_0200270c[k])
          found |= 1 << k;
      }
    }
    pos += 8;
  }
  if (mode == 0) {
    pos = 39;
    bit = 0;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);

      for (n = 0; n != 15; n++) {
        u16 item = unit->inventory[n] & 0x1ff;

        out[pos] += item >> (bit + 1);
        out[pos + 1] += item << (7 - bit);
        pos++;
        bit++;
        if (bit == 7) {
          bit = 0;
          pos++;
        }
      }
    }
    pos = 107;
    bit = -1;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);

      for (m = 0; m != 23; m++) {
        s32 count = 0;
        u16 quantity;
        /* FAKEMATCH(dead): a halfword copy of the count nothing reads, which
         * keeps the zero-extension hoisted above the sign test; without it, 139
         * of 479 instructions differ in tla-en. */

        for (n = 0; n != 15; n++) {
          if ((unit->inventory[n] & 0x1ff) == gData_0200271c[m])
            count = unit->inventory[n] >> 11;
        }
        quantity = count;
        if (bit < 0) {
          out[pos] += (u16)count >> -bit;
          bit += 8;
          pos++;
        }
        out[pos] += (u16)count << bit;
        bit -= 5;
        if (bit == -5) {
          pos++;
          bit = 3;
        }
      }
    }
    out[165] = (u32)gMainRoutineWork.coins >> 16;
    out[166] = (u32)gMainRoutineWork.coins >> 8;
    out[167] = gMainRoutineWork.coins;
  }
  if (mode != 2) {
    s32 statsOffset = 8 + (mode != 0);

    for (i = 0; i != 2; i++) {
      out[statsOffset] = stats[i * 4] >> 24;
      out[statsOffset + 1] = stats[i * 4] >> 16;
      out[statsOffset + 2] = stats[i * 4] >> 8;
      out[statsOffset + 3] = stats[i * 4];
      out[statsOffset + 4] = stats[i * 4 + 1] >> 24;
      out[statsOffset + 5] = stats[i * 4 + 1] >> 16;
      out[statsOffset + 6] = stats[i * 4 + 1] >> 8;
      out[statsOffset + 7] = stats[i * 4 + 1];
      out[statsOffset + 7] |= stats[i * 4 + 2] >> 28;
      out[statsOffset + 8] = stats[i * 4 + 2] >> 20;
      out[statsOffset + 9] = stats[i * 4 + 2] >> 12;
      out[statsOffset + 10] = stats[i * 4 + 2] >> 4;
      out[statsOffset + 11] = stats[i * 4 + 2] << 4;
      out[statsOffset + 11] |= stats[i * 4 + 3] >> 28;
      out[statsOffset + 12] = stats[i * 4 + 3] >> 20;
      out[statsOffset + 13] = stats[i * 4 + 3] >> 12;
      out[statsOffset + 14] = stats[i * 4 + 3] >> 4;
      statsOffset += 15;
    }
  }
  out[0] = levels;
  out[1] = levels >> 8;
  out[2] = levels >> 16;
  out[3] = ((levels >> 20) & 0xf0) | (djinn & 15);
  out[4] = djinn >> 4;
  out[5] = djinn >> 12;
  out[6] = djinn >> 20;
  out[7] = flags;
  if (mode != 0)
    out[8] = found;
  return size;
}

s32 Func_02002068(u8 *src, s32 length, u8 *dst) {
  u8 saved[260];
  s32 i;
  s32 shift = 2;
  s32 k = 0;
  s32 n = 0;
  s32 size = 11;
  u8 last;
  u8 c;

  if (length != 16) {
    size = 41;
    if (length != 61)
      size = 175;
  }
  for (i = 0; i != size; i++)
    dst[i] = 0;
  for (i = 0; i != length; i++) {
    saved[i] = src[i];
    src[i] = (saved[i] - i) & 63;
  }

  for (i = 0; i != length; i++, k++) {
    if (i % 10 > 8)
      continue;
    c = src[k];
    if (shift < 0) {
      dst[n] += c >> -shift;
      n++;
      if (n == size)
        break;
      shift += 8;
    }
    dst[n] += c << shift;
    shift -= 6;
    if (shift == -6) {
      shift = 2;
      n++;
      if (n == size)
        break;
    }
  }
  last = dst[n - 1];
  for (i = 0; i != n - 1; i++)
    dst[i] ^= last;
  for (i = 0; i != length; i++)
    src[i] = saved[i];
  return n;
}

/// The CRC-16 of length bytes at p (polynomial 0x1021, seed 0xffff, the
/// result inverted): the clear screen checks each decoded block with it,
/// over all but the block's last two bytes. The loops are the listing's,
/// counted up and tested at the bottom.
u16 Func_0200217c(u32 length, u8 *p) {
  u32 crc;
  u32 i;
  u32 j;

  crc = 0xffff;
  i = 0;
  if (length != 0) {
  next_byte:
    crc ^= *p << 8;
    j = 0;
  next_bit:
    // One bit: shift, adding the polynomial when the top bit was set.
    {
      u32 high = crc & 0x8000;
      u32 next = crc << 1;

      j++;
      crc = next + 0xffffefdf;
      if (high == 0)
        crc = next;
    }
    if (j != 8)
      goto next_bit;
    i++;
    p++;
    if (i != length)
      goto next_byte;
  }
  return ~crc;
}

// Draft, differ 1199 of 1077 (tla-en; ja and fr differ in layout): the
// password entry screen: a 17 by 4 character grid, the entered password in
// pages of 50 and its check before the clear data is loaded. Left: register
// choice and the frame throughout.
#include "types.h"
#include "party_state.h"
#include "iwram_call.h"
#include "dma.h"

struct PasswordWork {
  void *glyphs[50];
  void *cells[50];
  u8 shown[50];
  u8 unk1c2[26];
};

struct UiWindow {
  u8 unk00[12];
  s16 x;
  s16 y;
};

extern u32 gInput[];
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

void *Runtime_BumpAllocateAlternatePool(s32 size);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 mode);
void UiWindow_DrawDividerLine(struct UiWindow *window, s32 x, s32 y, s32 length,
                              s32 mode);
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 tile, s32 x, s32 y,
                              s32 mode);
void UiText_DrawStringInWindow(const u8 *text, struct UiWindow *window, s32 x,
                               s32 y);
void UiText_DrawCharacterAtOffset(s32 message, struct UiWindow *window, s32 x,
                                  s32 y);
void UiWork_Finalize(struct UiWindow *window, s32 release);
void RenderOutput_RedrawSavedRect(struct UiWindow *window);
u8 *RenderOutput_Create(s32 entry, s32 address, struct UiWindow *window,
                        s32 mode);
s32 Resource_FindFreeEntry(void);
void Party_RemoveActiveOwner(s32 id);
void Party_AddActiveOwner(s32 id);
void WaitFrames(s32 frames);
void Func_02000a70(s32 code, u16 *glyph);
void Func_02000db4(struct UiWindow *window, s32 column, s32 row, s32 mode,
                   s32 a, s32 b);
s32 Func_0200217c(s32 length, u8 *data);
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
void *Func_02002330(struct UiWindow *window, void *glyph, s32 x, s32 y);
void Func_02002350(void);
void Func_020023e8(void);
void Func_020023f0(u8 **sprite);
void Func_020023f8(u8 **sprite, s32 x, s32 y, s32 visible);
void Func_02002400(u8 *sprite, s32 x, s32 y);
void Func_02002408(s32 cue);

/// Draws the password screen's title and key help into the main window.
#define DRAW_TITLE(window)                                                     \
  {                                                                            \
    s32 message = MSGNO(MsgFieldSpace);                                        \
                                                                               \
    UiText_DrawStringInWindow(gData_02002440, window, 0, 0);                   \
    UiText_DrawStringInWindow(gData_02002454, window, 0, 8);                   \
    UiText_DrawStringInWindow(gData_02002468, window, 0, 16);                  \
    UiText_DrawStringInWindow(gData_0200247c, window, 0, 24);                  \
    UiWindow_DrawDividerLine(window, 20, 0, 20, 5);                            \
    UiText_DrawCharacterAtOffset(message++, window, 160, 0);                   \
    UiText_DrawCharacterAtOffset(message++, window, 160, 8);                   \
    UiText_DrawCharacterAtOffset(message, window, 160, 24);                    \
  }

s32 Func_02000e44(void) {
  u8 *sprite[2][4];
  struct PasswordWork *work;
  struct UiWindow *window, *grid, *box;
  s32 mode, length, pages, page, cursor = 0, column = 0, row = 0;
  s32 advance = 0, state, action, drawColumn, drawRow, drawMode;
  s32 cursorX, cursorY, i, entry, x, y, count;
  u16 *palette = (u16 *)0x050001c0;

  Func_02002208();
  Func_02002310(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  Dma_Set((const void *)0x050001e0, palette, (DMA_ENABLE | DMA_32BIT) << 16 | 8,
          (volatile u32 *)0x040000d4);
  palette[4] = 0x6318;
  mode = Func_020022b0(1);
  if (mode == -1)
    return -1;
  Func_02002408(71);
  work = Runtime_BumpAllocateAlternatePool(476);
  Func_02002320((void *)0x06006000);
  length = 260;
  if (mode != 0) {
    length = 61;
    if (mode != 1)
      length = 16;
  }
  pages = length / 50 + 1;
  window = UiWindow_Create(1, 13, 29, 6, 2);
  DRAW_TITLE(window)
  grid = UiWindow_Create(5, 1, 20, 12, 2);
  for (i = 0; i != 26; i++)
    work->unk1c2[i] = 0;
  for (i = 0; i != 260; i++)
    Data_0200274a[i] = 99;
  for (i = 0; i != 50; i++) {
    s32 col = i % 10;

    x = col * 12 + 8;
    y = i / 10 * 16 + 2;
    if (col > 4)
      x = col * 12 + 18;
    work->cells[i] = Func_02002330(grid, work->glyphs[i], x, y);
    work->shown[i] = 99;
    Iwram_ClearWordsEntry(work->cells[i], 128);
  }
  entry = Resource_FindFreeEntry();
  if (entry <= 95) {
    Func_02002300();
    sprite[1][0] = RenderOutput_Create(entry, 0x20000000, window, 0);
    Func_02002400(sprite[1][0], window->x * 8 - 2, window->y * 8 + 12);
  }
  entry = Resource_FindFreeEntry();
  if (entry <= 95) {
    Func_02002308();
    sprite[0][0] = RenderOutput_Create(entry, 0x20000000, window, 0);
    sprite[0][0][15] = 255;
    sprite[0][0][25] &= ~12;
    Func_02002400(sprite[0][0], grid->x * 8 - 4, grid->y * 8 + 12);
  }
  drawMode = 1;
  drawColumn = 0;
  drawRow = 0;
  cursorX = window->x * 8 - 8;
  cursorY = window->y * 8 + 11;
  state = 0;
  for (;;) {
    action = -1;
    if (state == 2)
      state = 1;
    Func_02000db4(window, drawColumn, drawRow, drawMode, 1, 15);
    if (state == 0) {
      if (gInput[3] & 16) {
        Func_02002408(111);
        if (row == 2)
          column = (column + 1) % 16;
        else
          column = (column + 1) % 17;
      }
      if (gInput[3] & 32) {
        Func_02002408(111);
        if (row == 2)
          column = (column + 15) % 16;
        else
          column = (column + 16) % 17;
      }
      if (gInput[3] & 128) {
        Func_02002408(111);
        row = (row + 1) % 4;
        if (column == 16 && row == 2)
          row = 3;
      }
      if (gInput[3] & 64) {
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
    x = (window->x + column) * 8 - 8;
    y = (window->y + row) * 8 + 11;
    if (column > 4)
      x = (window->x + column) * 8;
    if (column > 9)
      x += 8;
    if (column > 14)
      x += 8;
    if (column > 15)
      x += 8;
    cursorX += (x - cursorX) / 2;
    cursorY += (y - cursorY) / 2;
    Func_020023f8(sprite[1], cursorX, cursorY, 1);
    Func_020023f0(sprite[1]);
    {
      s32 cell = cursor % 50;
      s32 frame = (gVBlankCount >> 1) & 7;
      s32 col = cell % 10;

      y = (cell / 10 * 2 + grid->y) * 8 - 2;
      x = col * 12 + grid->x * 8 + 16;
      if (col > 4)
        x += 10;
      Func_020023f8(sprite[0], x + gData_02002438[frame],
                    y + gData_02002438[(frame + 5) & 7], 1);
      Func_020023f0(sprite[0]);
    }
    if (advance > 0 && ++advance == 8) {
      advance = 0;
      cursor++;
    }
    if (advance == 0) {
      u32 keys;

      if (column == 16 && row == 0)
        keys = gInput[3];
      else if (column == 16 && row == 1)
        keys = gInput[3];
      else
        keys = gInput[1];
      if ((s16)keys & 1 && state == 0) {
        action = 0;
        if (column == 16) {
          if (row == 0)
            action = 1;
          else if (row == 1)
            action = 2;
          else if (row == 3)
            action = 3;
        } else if (cursor < length) {
          u8 code = gData_0200265e[column + row * 16];

          if (code <= 63) {
            Func_02002408(112);
            Data_0200274a[cursor] = code;
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
        if (gInput[1] & 8 || action == 3 || state == 1) {
          s32 size = Func_02002068(Data_0200274a, length, Data_0200288a);
          s32 check = Func_0200217c(size - 2, Data_0200288a);

          state = 0;
          box = UiWindow_Create(5, 12, 20, 3, 2);
          if (Data_0200288a[size - 2] == (u8)((u16)check >> 8) &&
              Data_0200288a[size - 1] == (u8)check) {
            UiText_DrawCharacterAtOffset(MSGNO(MsgFieldPasswordAccepted), box,
                                         0, 0);
            Func_02002408(112);
            do
              WaitFrames(1);
            while (!(gInput[1] & 1));
            Func_02002408(112);
            Func_02002350();
            gPartyState.currentOwner = 0;
            Party_RemoveActiveOwner(4);
            Party_RemoveActiveOwner(5);
            Party_RemoveActiveOwner(6);
            Party_RemoveActiveOwner(7);
            Party_AddActiveOwner(0);
            Party_AddActiveOwner(1);
            Party_AddActiveOwner(2);
            Party_AddActiveOwner(3);
            Func_020018e0(mode, Data_0200288a);
            UiWork_Finalize(box, 2);
            UiWork_Finalize(grid, 2);
            UiWork_Finalize(window, 2);
            Func_02002310(gPartyState.paletteGlow[0],
                          gPartyState.paletteGlow[1]);
            Func_02002408(74);
            WaitFrames(6);
            Func_020022f8(0);
            Func_020023e8();
            Iwram_CopyWords(Data_02012f94 + 0xac, gGameFlagBytes, 0xf40);
            return 0;
          }
          Func_02002408(113);
          UiText_DrawCharacterAtOffset(MSGNO(MsgFieldInvalidPassword), box, 0,
                                       0);
          do
            WaitFrames(1);
          while (!(gInput[1] & 1) && !(gInput[1] & 2));
          UiWork_Finalize(box, 2);
          RenderOutput_RedrawSavedRect(window);
          DRAW_TITLE(window)
        } else if (gInput[3] & 512 || action == 2) {
          if (cursor > 0) {
            Func_02002408(111);
            cursor--;
          }
        } else if (gInput[3] & 256 || action == 1) {
          if (cursor < length - 1) {
            Func_02002408(111);
            cursor++;
          }
        } else if (gInput[1] & 2) {
          if (cursor > 0) {
            Func_02002408(111);
            cursor--;
          } else {
            Func_02002408(113);
            UiWork_Finalize(grid, 2);
            UiWork_Finalize(window, 2);
            Func_02002408(74);
            return -1;
          }
        }
      }
    }
    page = cursor / 50;
    RenderOutput_RedrawSavedRect(grid);
    for (i = 0, y = 2; i != 4; i++, y += 2)
      UiWindow_DrawDividerLine(grid, 0, y, 18, y);
    for (i = 0, x = 18 - pages; i != pages; i++, x++) {
      s32 tile = i + 0xf301;

      if (i == page)
        tile = i + 0xf30b;
      UiWindow_SetTilemapEntry(grid, tile, x, 10, 0);
    }
    count = length;
    if (length > 49) {
      if (pages - 1 == page)
        count = length % 50;
      else
        count = 50;
    }
    for (i = 0; i != count; i++) {
      u8 code = Data_0200274a[page * 50 + i];

      if (code != work->shown[i]) {
        work->shown[i] = code;
        if (code == 99) {
          Iwram_ClearWordsEntry(work->cells[i], 128);
        } else {
          u16 glyph;

          Func_02000a70(Data_0200274a[page * 50 + i], &glyph);
          Func_02002328(glyph, work->cells[i]);
        }
      }
    }
    for (; i != 50; i++) {
      if (work->shown[i] != 99) {
        work->shown[i] = 99;
        Iwram_ClearWordsEntry(work->cells[i], 128);
      }
    }
    WaitFrames(1);
  }
}

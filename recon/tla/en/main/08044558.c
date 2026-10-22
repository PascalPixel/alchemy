// Draft, differ 436 of 564: the name entry screen for party member member,
// The Broken Seal's NameEntry_EditOwnerName on this game's keyboard: names
// of up to five letters in a fourteen-byte buffer, the current palette glow
// restored first, and the screen left only once every key is released. The
// code follows the listing block for block; the difference is register
// choice. The native reloads gInput's address into r5 at the end of each
// key test's body, so the B and A tests read it from r5 and nothing holds it
// across the calls; here one pseudo holds it and is spilled to [sp, #8]
// around each call, which pushes the end pointer to [sp, #12], frees the
// key cursor's address from its stack slot into r11 and moves the column,
// row, keyboard and redraw flag down one register each (the native keeps
// them in r7, r8, r10 and r11). The resource ids 0x1fa and 0x202 are
// placeholders for their table names.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "render_input.h"
#include "party_state.h"
#include "input.h"
#include "dma.h"
#include "io_reg.h"

#define NAME_MAX_BYTES 14
#define NAME_MAX_LETTERS 5
#define NAME_COMMAND_COLUMN 18

struct NameCursor {
  struct RenderOutput *output;
  u8 rest[12];
};

extern u32 gFrameTick;
extern char ResourceId_01fa, ResourceId_0202;
extern const s8 gNameEntry_CaretWave[];
extern const u8 gNameEntry_KeyboardTilemap[];

u8 *Unit_Get(s32 owner);
void Ui_LoadWindowGraphics(void);
void Func_0803f9c0(s32 glow, s32 sub);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);
void Func_0803d3c0(s32 member, s32 x, s32 y, s32 side);
void Func_08044348(struct UiWindow *window, const void *resource);
void UiWindow_DrawDividerLine(struct UiWindow *window, s32 x0, s32 y0, s32 x1,
                              s32 y1);
void UiText_DrawPaddedLabel(struct UiWindow *window, u8 *name);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadResourceFar(s32 slot, s32 size, s32 resource);
void Func_08108048(struct NameCursor *cursor, s32 x, s32 y);
void Func_08108040(struct NameCursor *cursor, s32 x, s32 y, s32 speed);
void Func_08108030(struct NameCursor *cursor);
void Func_08108038(struct NameCursor *cursor);
s32 UiText_SetRenderString(const u8 *str);
void RenderOutput_PrepareForRedraw(struct UiWindow *window);
void WaitFrames(s32 frames);
void UiWindow_SetTilemapFlagRect(struct UiWindow *window, s32 x, s32 y,
                                 s32 width, s32 height, s32 palette);
void Audio_PlayCue(s32 cue);
void Func_0803d450(s32 member);

s32 Func_08044558(s32 member) {
  struct UiWindow *nameWindow;
  s32 result = 0;
  s32 originalLength = 0;
  s32 letterCount = 0;
  u8 text[NAME_MAX_BYTES + 2];
  u8 *editedName = text + 1;
  u8 *originalName = Unit_Get(member);
  struct UiRenderWork *render =
      (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  s32 textDirty = 1;
  s32 cursorDirty = 1;
  struct UiWindow *keyboardWindow;
  struct NameCursor keyCursor;
  struct NameCursor nameCaret;
  s32 keyColumn, keyRow, vramSlot;
  struct RenderOutput *output;
  u8 *end;
  u16 *palette;

  Ui_LoadWindowGraphics();
  Func_0803f9c0(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  keyboardWindow = UiWindow_Create(3, 6, 24, 9, 2);
  nameWindow = UiWindow_Create(8, 3, 8, 3, 2);
  Func_0803d3c0(member, 0, 3, 1);
  Func_08044348(keyboardWindow, gNameEntry_KeyboardTilemap);
  UiWindow_DrawDividerLine(keyboardWindow, 18, 0, 18, 7);
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
  UiText_DrawPaddedLabel(nameWindow, originalName);
  keyRow = 5;
  vramSlot = Resource_FindFreeEntry();
  keyColumn = NAME_COMMAND_COLUMN;
  if (vramSlot <= 95) {
    VramBlock_LoadResourceFar(vramSlot, 128, (s32)&ResourceId_01fa);
    output = RenderOutput_Create(vramSlot, 0x40000000,
                                 (struct RenderInput *)keyboardWindow, 0, 0);
    keyCursor.output = output;
    Func_08108048(&keyCursor, (s16)keyboardWindow->x * 8 + 140,
                  (s16)keyboardWindow->y * 8 + 52);
  }
  vramSlot = Resource_FindFreeEntry();
  if (vramSlot <= 95) {
    VramBlock_LoadResourceFar(vramSlot, 128, (s32)&ResourceId_0202);
    output = RenderOutput_Create(vramSlot, 0x40000000,
                                 (struct RenderInput *)keyboardWindow, 0, 0);
    nameCaret.output = output;
    output->sentinel = 255;
    output->oam.priority = 0;
    Func_08108048(&nameCaret, UiText_SetRenderString(editedName) + 70, 22);
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
        width = 3;
    }
    UiWindow_SetTilemapFlagRect(keyboardWindow, keyColumn, keyRow, width, 1,
                                14);
    WaitFrames(1);
    UiWindow_SetTilemapFlagRect(keyboardWindow, keyColumn, keyRow, width, 1,
                                15);
    if (cursorDirty) {
      cursorDirty = 0;
      Func_08108040(&keyCursor, ((s16)keyboardWindow->x + keyColumn) * 8 - 7,
                    ((s16)keyboardWindow->y + keyRow) * 8 + 15, 3);
    }
    if (textDirty) {
      textDirty = 0;
      Func_08108040(&nameCaret, UiText_SetRenderString(editedName) + 70, 22, 3);
    }
    Func_08108030(&keyCursor);
    Func_08108038(&nameCaret);
    {
      u32 frame = (gFrameTick >> 1) & 7;

      output = nameCaret.output;
      output->oam.x = output->x + gNameEntry_CaretWave[frame];
      output->oam.y = output->y + gNameEntry_CaretWave[(frame + 5) & 7];
    }
    if (gInput.repeat & 0x40) {
      Audio_PlayCue(111);
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
      Audio_PlayCue(111);
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
      Audio_PlayCue(111);
      cursorDirty = 1;
      keyColumn--;
      if (keyColumn == -1) {
        keyColumn = NAME_COMMAND_COLUMN;
        if (keyRow < 4 || keyRow > 5)
          keyColumn = 16;
      } else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17) {
        keyColumn--;
      }
    }
    if (gInput.repeat & 0x10) {
      Audio_PlayCue(111);
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
      Audio_PlayCue(111);
      cursorDirty = 1;
      keyColumn = NAME_COMMAND_COLUMN;
      keyRow = 5;
    }
    if (gInput.repeat & 2) {
      Audio_PlayCue(113);
    remove_character:
      if (letterCount != 0) {
        letterCount--;
        *--end = 0;
        RenderOutput_PrepareForRedraw(nameWindow);
        UiText_DrawPaddedLabel(nameWindow, editedName);
        textDirty = 1;
        continue;
      }
      result = -1;
      break;
    }
    if (!(gInput.repeat & 1))
      continue;
    Audio_PlayCue(112);
    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 5) {
        if (letterCount == 0) {
          nameCaret.output->active = 13;
          RenderOutput_PrepareForRedraw(nameWindow);
          UiText_DrawPaddedLabel(nameWindow, originalName);
          WaitFrames(10);
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
      u32 letter = render->tilemap[(s16)keyboardWindow->y + keyRow + 1]
                                  [(s16)keyboardWindow->x + keyColumn + 1];

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
      RenderOutput_PrepareForRedraw(nameWindow);
      UiText_DrawPaddedLabel(nameWindow, editedName);
      textDirty = 1;
    }
  }
  UiWork_Finalize(keyboardWindow, 2);
  UiWork_Finalize(nameWindow, 2);
  Func_0803d450(member);
  WaitFrames(1);
  while (gInput.held != 0)
    WaitFrames(1);
  return result;
}

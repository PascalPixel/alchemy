// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 4 of 306 in tla-en. First difference: the native issues `mov r9, r3` earlier
// (row 17) than this does (row 20). No single empty asm or one-pass block
// reaches 0.
// First draft of Func_080fe894: the member chooser of the Djinn menu, the
// twin of The Broken Seal's CharacterMenu_SelectOwner
// (games/tbs/src/menu/menu/status.c). A chooses (1), B backs out (-1); R
// and L move the member along the party order (Func_080fecfc), Select shows
// the list of Djinn found (Func_080fe638) once the first is; the cursor moves
// through Menu_MoveGridCursor. MsgRearrangeHelp and MsgDjinnListHelp are messages
// 4156 and 4165 in English; they need their names in the six catalogs before
// this links. Differ 16 of 306 (2026-10-19): the literal pool after the first
// DMA comes one instruction later, which shifts later pool offsets, and the
// redraw 2 and the selection offset are one slot off.
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "input.h"
#include "ram_buffer.h"

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((volatile struct InputState *)&gInput)->pressed)

struct MenuIcon {
  u8 unk00[5];
  u8 active;
};

struct CharacterMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  s32 rowWindow;
  u8 unk014[8];
  s8 ownerIndex;
  u8 unk01d;
  s8 ownerCount;
  u8 unk01f[0xd1];
  s32 selectorWindow;
  u8 unk0f4[0x84];
  struct MenuIcon *arrows[2];
  u8 unk180[0x84];
  u16 ownerIds[8];
  u8 unk214[2];
  u8 selectedId;
  u8 unk217[5];
  u16 page;
  u8 unk21e[0x12];
  u16 rowX[4];
  u16 rowY[4];
};

extern u8 MsgRearrangeHelp[], MsgDjinnListHelp[];

void *Unit_Get(s32 owner);
void Palette_LightenBankHighlight(s32 bank);
s32 GameFlag_Test(s32 flag);
void RenderOutput_RedrawSavedRectFar(s32 window);
void UiText_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void Menu_CreateWindowAndEntryObjects(s32 resource, s32 frame);
void PsynergyMenu_CallIconRoutineWithValue(struct CharacterMenuState *menu,
                                           s32 ownerId);
void Func_08104ef8(s32 first);
void WaitTicks(s32 ticks);
void Menu_HighlightPageEntry(s32 window, s32 selection, s32 count);
void Func_080f8a44(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
s32 Func_080fecfc(s32 selection, s32 direction);
void Func_080fe638(void);
s32 Menu_MoveGridCursor(s32 *selection, s32 count, s32 columns);

s32 Func_080fe894(void) {
  struct CharacterMenuState *menu;
  s32 ownerIndex;
  s32 ownerCount;
  s32 page;
  s32 redraw;
  s32 result;
  s32 rowIndex;

  menu = Ram_HeapSlots->menuRuntime;
  ownerIndex = menu->ownerIndex;
  ownerCount = menu->ownerCount;
  redraw = 2;
  result = 0;
  page = menu->page;
  Unit_Get(menu->ownerIds[ownerIndex]);
  for (rowIndex = 0; rowIndex < 4; rowIndex++) {
    menu->rowX[rowIndex] = 130 + rowIndex * 32;
    menu->rowY[rowIndex] = 128;
  }
  Palette_LightenBankHighlight(14);
  Dma_Set((void *)OBJ_PLTT, (void *)BG_PLTT, DMA_ENABLE << 16 | 16, REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(14, 4), (void *)&BG_PLTT_COLOR(0, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0), DMA_ENABLE << 16 | 16,
          REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(1, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  while (!GameFlag_Test(0x150)) {
    if (redraw) {
      RenderOutput_RedrawSavedRectFar(menu->selectorWindow);
      UiText_DrawCharacterAtOffsetFar(MsgRearrangeHelp, menu->selectorWindow, 0,
                                      0);
      if (GameFlag_Test(55))
        UiText_DrawCharacterAtOffsetFar(MsgDjinnListHelp, menu->selectorWindow,
                                        0, 16);
      UiText_DrawCharacterAtOffsetFar(MsgRearrangeHelp - 3,
                                      menu->selectorWindow, 0, 8);
      ownerIndex = (ownerIndex + ownerCount) % ownerCount;
      Unit_Get(menu->ownerIds[ownerIndex]);
      page = (page + 3) % 3;
      Menu_CreateWindowAndEntryObjects(menu->ownerIds[ownerIndex], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[ownerIndex]);
      if (redraw == 2) {
        Func_08104ef8(ownerIndex / 4 * 4);
        WaitTicks(1);
      }
      Menu_HighlightPageEntry(menu->rowWindow, ownerIndex, ownerCount);
      redraw = 0;
    }
    Func_080f8a44((ownerIndex % 4) * 24 - 10, 16);
    WaitTicks(1);
    if (KEYS_PRESSED & 1) {
      Audio_PlayCue(112);
      result = 1;
      break;
    }
    if (KEYS_PRESSED & 2) {
      Audio_PlayCue(113);
      result = -1;
      break;
    }
    if (gInput.repeat & 0x100) {
      if (Func_080fecfc(ownerIndex, 1)) {
        Audio_PlayCue(112);
        ownerIndex++;
        Func_08104ef8(ownerIndex / 4 * 4);
        Menu_HighlightPageEntry(menu->rowWindow, ownerIndex, ownerCount);
      } else {
        Audio_PlayCue(114);
      }
      WaitTicks(1);
    } else if (gInput.repeat & 0x200) {
      if (Func_080fecfc(ownerIndex, 0)) {
        Audio_PlayCue(112);
        ownerIndex--;
        Func_08104ef8(ownerIndex / 4 * 4);
        Menu_HighlightPageEntry(menu->rowWindow, ownerIndex, ownerCount);
      } else {
        Audio_PlayCue(114);
      }
      WaitTicks(1);
    } else if ((KEYS_PRESSED & 4) && GameFlag_Test(55)) {
      Func_080fe638();
      redraw = 1;
    } else {
      redraw = Menu_MoveGridCursor(&ownerIndex, ownerCount, 4);
    }
  }
  menu->ownerIndex = ownerIndex;
  menu->selectedOwner = menu->ownerIds[ownerIndex];
  menu->selectedId = menu->ownerIds[ownerIndex];
  menu->arrows[0]->active = 13;
  menu->arrows[1]->active = 13;
  return result;
}

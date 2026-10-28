// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 166 of 1832 in tla-en. The native spills its arguments to the stack before it
// builds the heap slots' address; this builds the address first (row 8).
// First draft of Func_08102008, the Djinn list's chooser, the twin of The
// Broken Seal's DjinnMenu_SelectDjinn (games/tbs/src/menu/menu/djinn.c).
// Mode 0 chooses a Djinni in any member's column, mode 1 what to do with it.
// The Lost Age shows four members' columns a page at a time (redraw bit 2
// redraws the page and its headers), places the cursor while it redraws,
// and keeps each Djinni's owner, element, number and state
// (Func_08101a04: 0 on standby, 1 set, 2 recovering) from its packed entry.
// Reached from Func_0810106c. The message names are The Broken Seal's; this
// game's text has no symbols for them yet.
// Differ 212 of 1832 (2026-10-25): the structure is the listing's; left are
// instruction order (the native splits constants around other rows and
// issues stack reloads earlier) and register choice in a few blocks (the
// tutorial's Recovery tail, which the native cross-jumps with step 24, and
// the stores of the chosen entry at the end). Menu_GetModuloOfSum is
// declared const: the member loop's bound is read once although the loop
// calls it (with a plain prototype and the bound in a local, 248 differ).
// Tried: an empty asm keeping the entry live at the end, as The Broken Seal
// does (236).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "adventure.h"
#include "battle_unit.h"

struct DjinnMenuBackdrop {
  u16 ids[8][10];
  s8 counts[8];
  u8 unk0a8[0x2128 - 0xa8];
  s32 tutorialTimer;
  s32 tutorialStep;
};

struct DjinnMenuIcon {
  u8 unk00[5];
  u8 active;
};

struct DjinnMenuWork {
  u8 unk000[0x10];
  s32 statusWindow;
  struct DjinnMenuIcon *cursor;
  u8 unk018[4];
  s8 selectedColumn[2];
  u8 unk01e[0x16];
  s32 listWindow;
  u8 unk038[0xb8];
  s32 helpWindow;
  u8 unk0f4[4];
  s32 headerWindows[4];
  u8 unk108[0x60];
  u16 selection[2];
  u16 pairEntries[2];
  u8 unk170[0x10];
  struct DjinnMenuBackdrop *backdrop;
  u8 unk184[0x80];
  u16 owners[8];
  u8 unk214;
  u8 memberCount;
  u8 shownMember[2];
  u8 unk218[0x20];
  u16 figureY;
  u8 unk23a[0x1e];
  u8 pairIndex[2];
  u8 pairElement[2];
  u8 pairMember[2];
};

struct DjinnWindowWork {
  u8 unk0[3];
  u8 dirty;
};

extern u8 MsgClassName[];
extern u8 MsgDjinnEffect[];
extern u8 MsgDjinnSetAll[];
extern u8 MsgDjinnAllStandby[];
extern u8 MsgDjinnChoose[];
extern u8 MsgDjinnRecovering[];
extern u8 MsgDjinnCharacterStatus[];
extern u8 MsgDjinnGiveAction[];
extern u8 MsgDjinnSetAction[];
extern u8 MsgDjinnStandbyAction[];
extern u8 MsgDjinnTradeAction[];
extern u8 MsgDjinnOwnerLabel[];
extern u8 MsgDjinnNowRecovering[];
extern u8 gDjinnMenu_TextLevel[];

void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
s32 Func_08104928(s8 *table, s32 self);
void Func_080f8a44(s32 x, s32 y);
void Func_080f8ab4(s32 x, s32 y);
void Func_08104ef8(s32 first);
void DjinnMenu_DrawPage(struct DjinnMenuBackdrop *backdrop, s32 first);
void DjinnMenu_DrawFirstPage(struct DjinnMenuBackdrop *backdrop);
void UiPanel_HighlightGridCell(s32 panel, s32 index, s32 row, s32 selected);
void UiPanel_SetRectPalette(s32 panel, s32 x, s32 y, s32 width, s32 height,
                            s32 palette);
void Menu_SetPageArrowsB(s32 index, s32 count);
s32 Func_08101a04(u32 entry);
void MenuIcon_SetPosition(s32 index, s32 x, s32 y);
void MenuIcon_SetPicture(s32 index, s32 frame);
void Func_08105350(s32 index, s32 scale);
void MenuIcon_ReplaceObject(s32 index, s32 kind, s32 animation);
void *Unit_Get(s32 owner);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void UiText_DrawStringAtOffsetFar(const u8 *text, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 width, s32 window, s32 x,
                                  s32 y);
void UiText_DrawQuantity(s32 value, s32 kind);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 mode);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width,
                                    s32 height);
s32 Menu_GetDjinnGiveMessage(void);
s32 UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, s32 style);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizeFar(s32 window, s32 style);
void DjinnRecovery_CountDownFar(void);
void Djinn_ActivateFar(s32 owner, s32 element, s32 index);
void Djinn_DeactivateFar(s32 owner, s32 element, s32 index);
s32 DjinnRecovery_AddStandbyFar(s32 owner, s32 element, s32 index);
s32 DjinnRecovery_RemoveFar(s32 owner, s32 element, s32 index);
void Unit_CalculateFar(s32 owner);
s32 Menu_GetModuloOfSum(s32 value, s32 count) __attribute__((const));

#define DJINN_OWNER(entry) (((entry) & 0xf00) >> 8)
#define DJINN_ELEMENT(entry) (((entry) & 0xe0) >> 5)
#define DJINN_NUMBER(entry) ((entry) & 31)

// Shows one tutorial message and waits for its text to finish.
#define DJINN_TUTORIAL_MESSAGE(window, offset)                                 \
  do {                                                                         \
    (window) = UiText_OpenMessageWindowFar(                                    \
        Menu_GetDjinnGiveMessage() + (offset), 9, 9, 1);                       \
    gAdventure.eventMode = 1;                                                 \
    while (UiWork_IsCompleteFar() == 0)                                        \
      WaitFrames(1);                                                           \
  } while (0)

s32 Func_08102008(s32 mode) {
  s8 columnFlags[8];
  u8 requiresTrade[16];
  struct DjinnMenuWork *menu = Ram_HeapSlots->menuRuntime;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;
  s32 helpWindow = menu->helpWindow;
  s32 listWindow = menu->listWindow;
  struct BattleUnit *unit;
  s32 window;
  u32 redraw = 4;
  s32 i;
  s32 j;
  s32 count;
  s32 selectedRow;
  s32 memberColumn = (u16)(menu->selection[mode] % 10);
  s32 djinnRow = (u16)(menu->selection[mode] / 10);
  s32 savedRow = 0;
  u32 djinn = 0;
  s32 groupMode = 0;
  s32 setAll = 0;
  s32 owner = 0;
  s32 element = 0;
  s32 number = 0;
  s32 state = 0;
  s32 result;
  s32 message;
  u32 pressed;
  u32 repeat;
  s32 messageWindow;
  s8 *status;

  selectedRow = -1;
  status = columnFlags;
  for (i = 7; i >= 0; i--)
    status[i] = 0;
  if (mode == 0) {
    savedRow = djinnRow;
    for (i = 0; i < menu->memberCount; i++) {
      if (backdrop->counts[i] == 0)
        status[i] = 4;
    }
    for (i = 0; i < menu->memberCount; i++) {
      if (status[memberColumn] == 4) {
        memberColumn++;
        memberColumn = Menu_GetModuloOfSum(memberColumn, menu->memberCount);
      }
    }
  } else {
    Func_08104928((s8 *)requiresTrade, menu->selectedColumn[0]);
    for (i = 0; i < menu->memberCount; i++) {
      if (i == menu->selectedColumn[0]) {
        columnFlags[i] = 7;
      } else if (requiresTrade[i] != 0) {
        columnFlags[i] = 0;
      } else {
        columnFlags[i] = 3;
        if (backdrop->counts[i] == 0)
          status[i] = columnFlags[i] | 4;
      }
    }
  }
  menu->cursor->active = 1;
  for (;;) {
    if (redraw != 0) {
      selectedRow = -1;
      if (!(columnFlags[memberColumn] & 1))
        selectedRow = djinnRow;
      if (columnFlags[memberColumn] & 1)
        Func_080f8a44(memberColumn % 4 * 56 - 8, 52);
      else
        Func_080f8a44(memberColumn % 4 * 56 - 8, djinnRow * 8 + 60);
      if (redraw >> 2) {
        s32 page = memberColumn / 4 * 4;

        Func_08104ef8(page);
        DjinnMenu_DrawPage(backdrop, page);
        if (mode == 1) {
          s32 fromX;
          s32 fromY;
          u16 sourceCursor;
          s32 x;
          s32 height;

          sourceCursor = menu->selection[0];
          fromX = (u16)((u32)sourceCursor % (u32)10);
          fromY = (u16)((u32)sourceCursor / (u32)10);
          if (fromX / 4 * 4 == page) {
            height = backdrop->counts[fromX];
            UiPanel_HighlightGridCell(listWindow, fromX, fromY, 1);
            UiPanel_SetRectPalette(listWindow, (fromX - page) * 7 + 1, 2, 6,
                                   height, 6);
            menu->figureY = 54;
          } else {
            menu->figureY = 200;
          }
          for (i = 0, x = 8; i <= 3; i++, x += 56) {
            if (menu->headerWindows[i] != 0) {
              if (page + i == menu->selectedColumn[0]) {
                if (menu->pairEntries[0] & 0x8000)
                  message = (s32)MsgDjinnStandbyAction;
                else
                  message = (s32)MsgDjinnSetAction;
              } else if (columnFlags[page + i] & 2) {
                message = (s32)MsgDjinnGiveAction;
              } else {
                message = (s32)MsgDjinnTradeAction;
              }
              UiText_DrawCharacterAtOffsetFar(message, listWindow, x, 8);
            }
          }
        }
        Menu_SetPageArrowsB(memberColumn, menu->memberCount);
      }
      if (!(columnFlags[memberColumn] & 1))
        UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 1);
      if (selectedRow != -1)
        djinn = backdrop->ids[memberColumn][selectedRow];
      owner = DJINN_OWNER(djinn);
      element = DJINN_ELEMENT(djinn);
      number = DJINN_NUMBER(djinn);
      state = Func_08101a04(djinn);
      if (selectedRow == -1) {
        MenuIcon_SetPosition(mode, 0, 200);
      } else {
        s32 frame;
        s32 animation;

        switch (state) {
        case 0:
          frame = 0x4000;
          animation = 1;
          break;
        case 1:
          frame = 0x4000;
          animation = 2;
          break;
        default:
          frame = 0x8000;
          animation = 1;
          break;
        }
        MenuIcon_SetPosition(mode, memberColumn % 4 * 56 + 48,
                             mode == 0 ? 62 : 54);
        MenuIcon_SetPicture(mode, frame);
        if (redraw & 2)
          Func_08105350(mode, 0);
        MenuIcon_ReplaceObject(mode, element, animation);
      }
      window = menu->statusWindow;
      unit = Unit_Get(menu->owners[memberColumn]);
      RenderOutput_RedrawSavedRectFar(window);
      UiText_DrawStringAtOffsetFar(unit->name, window, 0, 0);
      UiText_DrawCharacterAtOffsetFar(unit->classIndex + (s32)MsgClassName,
                                      window, 0, 8);
      UiText_DrawStringAtOffsetFar(gDjinnMenu_TextLevel, window, 48, 0);
      UiText_DrawNumberInWindowFar(unit->level, 2, window, 72, 0);
      if (mode == 0)
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnCharacterStatus, window, 0,
                                        16);
      RenderOutput_RedrawSavedRectFar(helpWindow);
      if (mode == 0 && selectedRow != -1) {
        if (groupMode) {
          if (setAll == 0)
            UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnSetAll, helpWindow, 0,
                                            0);
          else
            UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnAllStandby, helpWindow,
                                            0, 0);
          if (state == 2)
            UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnRecovering, helpWindow,
                                            0, 16);
        } else {
          UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnChoose, helpWindow, 0,
                                          0);
          switch (state) {
          case 0:
            message = (s32)MsgDjinnChoose + 3;
            break;
          case 1:
            message = (s32)MsgDjinnChoose + 2;
            break;
          default:
            message = (s32)MsgDjinnRecovering;
            break;
          }
          UiText_DrawCharacterAtOffsetFar(message, helpWindow, 0, 16);
        }
      }
      WaitFrames(1);
      if (mode == 1) {
        UiText_DrawQuantity(menu->shownMember[0], 1);
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnOwnerLabel, helpWindow, 0,
                                        0);
        UiText_DrawQuantity(DJINN_ELEMENT(menu->pairEntries[0]) * 20 +
                                DJINN_NUMBER(menu->pairEntries[0]) + 300,
                            4);
        UiWindow_SetTilemapEntryFar(
            helpWindow, DJINN_ELEMENT(menu->pairEntries[0]) + 0x5001, 6, 0, 0);
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnOwnerLabel + 1, helpWindow,
                                        56, 0);
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnOwnerLabel + 2, helpWindow,
                                        0, 8);
      }
      WaitFrames(1);
      RenderOutput_ClearListFar(listWindow);
      if (selectedRow != -1) {
        UiWindow_ClearInteriorTilesFar(listWindow, 0, 96, 224, 104);
        UiText_DrawCharacterAtOffsetFar(
            element * 20 + number + (s32)MsgDjinnEffect, listWindow, 0, 96);
      }
      ((struct DjinnWindowWork *)Ram_HeapSlots->windowTiles)->dirty = 1;
      redraw = 0;
    }
    WaitFrames(1);
    if (!(gInput.held & 0x100) || (gInput.pendingPressed & 0x100)) {
      if (groupMode)
        redraw = 1;
      groupMode = 0;
      setAll = 0;
    }
    repeat = gInput.repeat;
    pressed = gInput.pressed;
    if (backdrop->tutorialStep != 0) {
      pressed = 0;
      repeat = 0;
      backdrop->tutorialTimer++;
      switch (backdrop->tutorialStep) {
      case 28:
        while (!(gInput.pressed & 1)) {
          Func_080f8a44(150, 26);
          WaitFrames(1);
        }
        pressed = 2;
        repeat = 2;
        break;
      case 27:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow, 12);
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          WaitFrames(1);
          DJINN_TUTORIAL_MESSAGE(messageWindow, 13);
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          backdrop->tutorialTimer = 0;
          DjinnRecovery_CountDownFar();
          DjinnRecovery_CountDownFar();
          DjinnRecovery_CountDownFar();
          Djinn_DeactivateFar(4, 0, 7);
          DjinnRecovery_AddStandbyFar(4, 0, 7);
          Unit_CalculateFar(4);
          repeat = 2;
          pressed = 2;
        }
        break;
      case 1:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow, 0);
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 2;
        }
        break;
      case 2:
      case 4:
      case 8:
      case 9:
      case 22:
        if (backdrop->tutorialTimer == 90) {
          repeat = 1;
          pressed = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case 3:
        if (backdrop->tutorialTimer == 90) {
          repeat = 16;
          pressed = 16;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 4;
        }
        break;
      case 6:
      case 7:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow, 1);
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 8;
        }
        break;
      case 13:
        if (backdrop->tutorialTimer == 40) {
          backdrop->tutorialTimer = 0;
          repeat = 2;
          pressed = 2;
          backdrop->tutorialStep++;
        }
        break;
      case 14:
      case 16:
      case 17:
        if (backdrop->tutorialTimer == 40) {
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case 15:
        if (backdrop->tutorialTimer == 60) {
          gAdventure.eventMode = 1;
          messageWindow = UiText_OpenMessageWindowFar(
              Menu_GetDjinnGiveMessage() + 4, 9, 9, 1);
          Func_080f8ab4(2, 146);
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          while (!(gInput.pressed & 1)) {
            Func_080f8a44(2, 146);
            WaitFrames(1);
          }
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          WaitFrames(1);
          messageWindow = UiText_OpenMessageWindowFar(
              Menu_GetDjinnGiveMessage() + 5, 9, 9, 1);
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          while (!(gInput.pressed & 1)) {
            Func_080f8a44(2, 146);
            WaitFrames(1);
          }
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          WaitFrames(1);
          messageWindow = UiText_OpenMessageWindowFar(
              Menu_GetDjinnGiveMessage() + 6, 9, 9, 1);
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          while (!(gInput.pressed & 1)) {
            Func_080f8a44(2, 146);
            WaitFrames(1);
          }
          UiWork_FinalizeFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          WaitFrames(1);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 16;
        }
        break;
      case 18:
      case 20:
        if (backdrop->tutorialTimer == 90) {
          repeat = 1;
          pressed = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 21;
        }
        break;
      case 21:
        if (backdrop->tutorialTimer == 90) {
          backdrop->tutorialTimer = 0;
          repeat = 32;
          pressed = 32;
          backdrop->tutorialStep = 22;
        }
        break;
      case 24:
        if (backdrop->tutorialTimer == 60) {
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 25;
          repeat = 2;
          pressed = 2;
        }
        break;
      }
    }
    // R on a Djinni: put it on standby or set it at once.
    if (mode == 0 && (pressed & 0x100)) {
      if (selectedRow == -1) {
        Audio_PlayCue(114);
        continue;
      }
      groupMode = 1;
      gInput.pendingPressed = 0;
      switch (state) {
      case 0:
        Audio_PlayCue(139);
        Djinn_ActivateFar(owner, element, number);
        DjinnRecovery_RemoveFar(owner, element, number);
        break;
      case 1:
        Audio_PlayCue(175);
        Djinn_DeactivateFar(owner, element, number);
        DjinnRecovery_AddStandbyFar(owner, element, number);
        break;
      default:
        Audio_PlayCue(114);
        redraw = 1;
        continue;
      }
      Unit_CalculateFar(owner);
      DjinnMenu_DrawPage(backdrop, memberColumn / 4 * 4);
      redraw = 1;
    }
    if (mode == 0 && (pressed & 0x200)) {
      Audio_PlayCue(112);
      result = 7;
      break;
    }
    if ((pressed & 1) || (mode == 1 && (pressed & 0x100))) {
      if (selectedRow != -1 && state == 2) {
        Audio_PlayCue(114);
        RenderOutput_ClearListFar(listWindow);
        UiWindow_ClearInteriorTilesFar(listWindow, 0, 96, 224, 104);
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnNowRecovering, listWindow,
                                        0, 96);
        continue;
      }
      if (mode == 1) {
        if (memberColumn == menu->selectedColumn[0]) {
          if (menu->pairEntries[0] & 0x8000)
            result = 2;
          else
            result = 1;
        } else if (columnFlags[memberColumn] & 1) {
          result = 3;
        } else {
          result = 4;
        }
      } else {
        result = 1;
      }
      Audio_PlayCue(112);
      break;
    }
    if (pressed & 8) {
      Audio_PlayCue(113);
      result = -2;
      break;
    }
    if (pressed & 2) {
      Audio_PlayCue(113);
      result = -1;
      break;
    }
    // Select: with R held, set every Djinni not recovering or, the next
    // time, put every one on standby; otherwise open the help.
    if (mode == 0 && (pressed & 4)) {
      if (groupMode) {
        setAll ^= 1;
        if (setAll)
          Audio_PlayCue(139);
        else
          Audio_PlayCue(175);
        for (i = 0; i < menu->memberCount; i++) {
          for (j = 0; j < backdrop->counts[i]; j++) {
            u32 entry = backdrop->ids[i][j];
            s32 entryOwner = DJINN_OWNER(entry);
            s32 entryElement = DJINN_ELEMENT(entry);
            s32 entryNumber = DJINN_NUMBER(entry);

            switch (Func_08101a04(entry)) {
            case 0:
              if (setAll) {
                Djinn_ActivateFar(entryOwner, entryElement, entryNumber);
                DjinnRecovery_RemoveFar(entryOwner, entryElement, entryNumber);
                Unit_CalculateFar(entryOwner);
              }
              break;
            case 1:
              if (!setAll) {
                Djinn_DeactivateFar(entryOwner, entryElement, entryNumber);
                DjinnRecovery_AddStandbyFar(entryOwner, entryElement,
                                            entryNumber);
                Unit_CalculateFar(entryOwner);
              }
              break;
            }
          }
        }
        DjinnMenu_DrawPage(backdrop, memberColumn / 4 * 4);
        redraw = 1;
      } else {
        UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
        Audio_PlayCue(112);
        result = 10;
        break;
      }
    }
    if (repeat & 0x40) {
      if (columnFlags[memberColumn] & 4)
        continue;
      if (backdrop->counts[memberColumn] == 0)
        continue;
      UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
      Audio_PlayCue(111);
      if (columnFlags[memberColumn] & 1) {
        columnFlags[memberColumn] &= ~1;
        djinnRow = 0;
      } else if (djinnRow == 0 && (columnFlags[memberColumn] & 2)) {
        columnFlags[memberColumn] |= 1;
        redraw = 2;
        continue;
      }
      djinnRow--;
      djinnRow = Menu_GetModuloOfSum(djinnRow, backdrop->counts[memberColumn]);
      savedRow = djinnRow;
      redraw = 2;
      continue;
    }
    if (repeat & 0x80) {
      if (columnFlags[memberColumn] & 4)
        continue;
      if (backdrop->counts[memberColumn] == 0)
        continue;
      UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
      Audio_PlayCue(111);
      djinnRow++;
      djinnRow = Menu_GetModuloOfSum(djinnRow, backdrop->counts[memberColumn]);
      if (columnFlags[memberColumn] & 1) {
        columnFlags[memberColumn] &= ~1;
        djinnRow = 0;
      } else if (djinnRow == 0 && (columnFlags[memberColumn] & 2)) {
        columnFlags[memberColumn] |= 1;
      }
      savedRow = djinnRow;
      redraw = 2;
      continue;
    }
    if (repeat & 0x30) {
      s32 oldColumn;

      Audio_PlayCue(111);
      oldColumn = memberColumn;
      if (!(columnFlags[memberColumn] & 4))
        UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
      if (repeat & 0x20)
        memberColumn--;
      else
        memberColumn++;
      if (memberColumn < 0)
        memberColumn = 0;
      if (memberColumn > menu->memberCount - 1)
        memberColumn = menu->memberCount - 1;
      if (mode == 0) {
        for (i = 0; i < menu->memberCount; i++) {
          if (columnFlags[memberColumn] != 4)
            break;
          if (repeat & 0x20)
            memberColumn--;
          else
            memberColumn++;
          if (memberColumn < 0 || memberColumn >= menu->memberCount) {
            memberColumn = oldColumn;
            break;
          }
        }
      }
      djinnRow = savedRow;
      count = backdrop->counts[memberColumn];
      if (count == 0)
        count = 1;
      djinnRow = Menu_GetModuloOfSum(djinnRow, count);
      redraw = 2;
      if (oldColumn / 4 * 4 != memberColumn / 4 * 4)
        redraw = 6;
    }
  }
  menu->selectedColumn[mode] = memberColumn;
  if (selectedRow != -1) {
    u32 entry;

    menu->pairEntries[mode] = entry = backdrop->ids[memberColumn][selectedRow];
    menu->pairIndex[mode] = DJINN_NUMBER(entry);
    menu->pairElement[mode] = DJINN_ELEMENT(entry);
    menu->pairMember[mode] = DJINN_OWNER(entry);
  }
  menu->selection[mode] = memberColumn + djinnRow * 10;
  MenuIcon_SetPicture(mode, 0x4000);
  return result;
}

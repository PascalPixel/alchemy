// Draft, differ 1070 of 1246: the battle Djinn menu, The Broken Seal's
// BattleDjinnMenu_Run (games/tbs/src/window/screens/djinn_menu.c) on this
// game's work, messages and change preview (Func_080464dc). The list window
// is 10 tiles wide from column 20, a recovering Djinni's turns take two
// digits at x 48, and letting go of R redraws the list as well as the stats
// page. Structure follows the listing; left: the frame (220 bytes here, the
// native 224 with the text and sprite addresses spilled) and the stack slots,
// so allocation differs from the first spill on.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "iwram_call.h"

struct DjinnMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct DjinnMenuWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
};

struct DjinnMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
  u8 unke0[4];
  u32 chosenDjinn[8];
};

struct DjinnMenuUnit {
  u8 unk000[0xf8];
  u32 djinnHeld[4];
  u32 djinnSet[4];
};

struct DjinnRecoveryEntry {
  u8 element;
  u8 index;
  u8 unitId;
  s8 turns;
};

struct DjinnRecoveryList {
  struct DjinnRecoveryEntry entries[72];
  s32 count;
};

struct DjinnRecoveryTable {
  u8 unk000[8];
  struct DjinnRecoveryList list;
};

struct DjinnMenuPartyStatus {
  struct DjinnMenuWindow *window;
  u8 unk04[8];
  u16 flags;
};

struct DjinnMenuSprite {
  u8 unk00[4];
  u32 attr01;
  u32 attr23;
};

struct DjinnMenuOam {
  u8 unk00[4];
  u8 y;
  u8 attr0high;
  u16 x : 9;
  u16 attr1high : 7;
  u16 tile : 10;
  u16 attr2high : 6;
};

#define NAVIGATION ((struct DjinnMenuNavigation *)Ram_HeapSlots->unke4[0])

#define DJINN_ENTRY_STANDBY 0x10000
#define DJINN_ENTRY_RECOVERY_MASK 0x3e0000
#define DJINN_ENTRY_RECOVERY_SHIFT 17
#define DJINN_ENTRY_END 0x80000000

extern const u8 gDjinnMenu_BlinkTile[32];
extern const u8 gDjinnMenu_ElementXOffsets[4];
extern const u8 gDjinnMenu_ElementYOffsets[4];
extern const u8 gResource_FixedBlockBTiles[];

s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct DjinnMenuWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                        s32 flags);
void UiWork_SetAltFlagAndClearTable(s32 on);
struct DjinnMenuUnit *Unit_Get(s32 member);
struct DjinnRecoveryTable *Djinn_GetRecoveryTableFar(s32 side);
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 palette);
void UiWork_Finalize(void *window, s32 mode);
void Ui_FillVramBlockPattern(void);
void UiWork_ClearValueNameTables(void);
void UiWork_PushValueSlot(s32 value, s32 kind);
void UiText_CopyMessageString(s32 message, u16 *text, s32 capacity);
struct DjinnMenuWindow *Func_080464dc(struct DjinnMenuWindow *window,
                                      s32 member, s32 encodedDjinn, s32 page,
                                      s32 *pageCount);
void UiText_RenderWideStringAtOffset(u16 *text, struct DjinnMenuWindow *window,
                                     s32 x, s32 y);
void RenderOutput_RedrawSavedRect(struct DjinnMenuWindow *window);
void UiWindow_SetTilemapEntry(struct DjinnMenuWindow *window, s32 entry, s32 x,
                              s32 y, s32 flags);
void Func_08041f70(s32 color);
void UiText_DrawCharacterAtOffset(s32 message, struct DjinnMenuWindow *window,
                                  s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 digits,
                               struct DjinnMenuWindow *window, s32 x, s32 y);
void Audio_PlayCue(s32 cue);
void RenderOutput_ClearList(struct DjinnMenuWindow *window);
void Input_ResetKeyRepeat(void);
void Runtime_PushSlotEntry(void *sprite, s32 depth);
void Resource_ResetEntry(s32 slot);
void Func_08041b68(void);
void MemFree(void *block);
void WaitFrames(s32 frames);

s32 Func_0804868c(s32 member) {
  struct DjinnMenuOam cursorSprite;
  u16 messageText[64];
  struct DjinnMenuWork *work;
  struct DjinnMenuWindow *previewWindow;
  s32 drawnRow;
  s32 cursorSlot;
  u32 *djinnEntries;
  s32 djinnCount;
  s32 drawnFirstEntry;
  struct DjinnMenuWindow *messageWindow;
  s32 standbyElements;
  s32 preferredRow;
  s32 previewPage;
  s32 redraw;
  s32 alreadySelected;
  s32 redrawList;
  s32 previewPages;
  struct DjinnMenuWindow *listWindow;
  s32 firstEntry;
  s32 selectedRow;
  struct DjinnMenuUnit *unit;
  u32 selectedDjinn;
  u32 encodedDjinn;
  s32 pressed;
  s32 repeated;
  s32 held;
  s32 element;
  s32 djinnIndex;
  s32 i;
  s32 j;
  s32 cursorX;
  s32 cursorY;
  struct DjinnMenuNavigation *navigation;
  struct DjinnMenuPartyStatus *partyStatus;
  u32 blinkPhase;

  work = (struct DjinnMenuWork *)Ram_HeapSlots->windowTiles;
  previewWindow = NULL;
  drawnRow = -1;
  cursorSlot = VramCache_AllocEntry(0x80);
  djinnEntries = Runtime_BumpAllocateAlternatePool(84 * sizeof(*djinnEntries));
  djinnCount = 0;
  drawnFirstEntry = drawnRow;
  standbyElements = 0;
  previewPage = 0;
  previewPages = 0;
  redraw = 0;
  alreadySelected = 0;
  redrawList = 0;
  messageWindow = UiWindow_Create(0, 4, 30, 4, 42);
  UiWork_SetAltFlagAndClearTable(1);
  listWindow = UiWindow_Create(20, 9, 10, 11, 6);
  navigation = NAVIGATION;
  firstEntry = navigation->page;
  selectedRow = navigation->row;
  preferredRow = navigation->preferredRow;
  unit = Unit_Get(member);

  for (element = 0; element <= 3; element++) {
    u32 *entryCursor = &djinnEntries[djinnCount];

    for (djinnIndex = 0; djinnIndex <= 19; djinnIndex++) {
      if (unit->djinnSet[element] & (1 << djinnIndex)) {
        *entryCursor++ = (element << 8) | djinnIndex;
        djinnCount++;
      } else if (unit->djinnHeld[element] & (1 << djinnIndex)) {
        struct DjinnRecoveryList *list =
            &Djinn_GetRecoveryTableFar((u32)member > 7)->list;
        u32 v;
        s32 k;

        for (k = 0; k < list->count; k++) {
          if (list->entries[k].unitId == member &&
              list->entries[k].element == element &&
              list->entries[k].index == djinnIndex)
            break;
        }
        v = (element << 8) | djinnIndex | DJINN_ENTRY_STANDBY;
        *entryCursor = v;
        if (list->entries[k].turns > 0) {
          v |= list->entries[k].turns << DJINN_ENTRY_RECOVERY_SHIFT;
          *entryCursor = v;
        }
        entryCursor++;
        djinnCount++;
      }
    }
  }
  djinnEntries[djinnCount] = DJINN_ENTRY_END;
  work->dirty = 1;

  for (;;) {
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow ||
        redraw != 0) {
      selectedDjinn = djinnEntries[firstEntry + selectedRow];
      standbyElements = 0;
      work->holdTilemapUpload = 1;
      Ui_SetRectHighlight(listWindow->x + 1, listWindow->y + drawnRow * 2 + 1,
                          listWindow->width - 2, 1, 15);
      if (previewPage != 0) {
        UiWork_Finalize(messageWindow, 1);
        messageWindow = UiWindow_Create(0, 4, 30, 4, 42);
        Ui_FillVramBlockPattern();
      }
      redraw = 0;
      if (djinnCount != 0) {
        s32 k;

        UiWork_ClearValueNameTables();
        alreadySelected = 0;
        k = redraw;
        if (NAVIGATION->chosenDjinn[0] == selectedDjinn) {
          alreadySelected = 1;
        } else {
          for (;;) {
            k++;
            if (k > 7)
              break;
            if (NAVIGATION->chosenDjinn[k] == selectedDjinn) {
              alreadySelected = 1;
              break;
            }
          }
        }
        if (alreadySelected != 0) {
          UiText_CopyMessageString(0xd50, messageText, 52);
          if (previewWindow != NULL) {
            UiWork_Finalize(previewWindow, 1);
            previewWindow = NULL;
            previewPage = 0;
          }
        } else if (selectedDjinn & DJINN_ENTRY_STANDBY) {
          if (selectedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            UiWork_PushValueSlot((selectedDjinn & DJINN_ENTRY_RECOVERY_MASK) >>
                                     DJINN_ENTRY_RECOVERY_SHIFT,
                                 5);
            UiText_CopyMessageString(((selectedDjinn & 0xf00) >> 8) * 20 +
                                         (selectedDjinn & 0xff) + 0x9b1,
                                     messageText, 52);
            if (previewWindow != NULL) {
              UiWork_Finalize(previewWindow, 1);
              previewWindow = NULL;
              previewPage = 0;
            }
          } else {
            previewWindow = Func_080464dc(previewWindow, member, selectedDjinn,
                                          previewPage, &previewPages);
            UiText_CopyMessageString(0xcf9, messageText, 52);
            standbyElements = 1 << ((selectedDjinn & 0xf00) >> 8);
          }
        } else {
          previewWindow = Func_080464dc(previewWindow, member, selectedDjinn,
                                        previewPage, &previewPages);
          UiText_CopyMessageString(((selectedDjinn & 0xf00) >> 8) * 20 +
                                       (selectedDjinn & 0xff) + 0x9b1,
                                   messageText, 52);
        }
      } else {
        UiText_CopyMessageString(0xd4e, messageText, 52);
      }
      work->holdTilemapUpload = 0;
      if (previewPage == 0) {
        work->holdTilemapUpload = 1;
        UiWork_Finalize(messageWindow, 1);
        messageWindow = UiWindow_Create(0, 4, 30, 4, 42);
        work->holdTilemapUpload = previewPage;
      }
      UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      if (firstEntry != drawnFirstEntry || redrawList == 1) {
        RenderOutput_RedrawSavedRect(listWindow);
        j = 0;
        while ((encodedDjinn = djinnEntries[firstEntry + j]) !=
               DJINN_ENTRY_END) {
          UiWindow_SetTilemapEntry(
              listWindow, ((encodedDjinn & 0xf00) >> 8) + 0x5001, 0, j * 2, 0);
          if (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK)
            Func_08041f70(4);
          else if (encodedDjinn & DJINN_ENTRY_STANDBY)
            Func_08041f70(2);
          UiText_DrawCharacterAtOffset(((encodedDjinn & 0xf00) >> 8) * 20 +
                                           (encodedDjinn & 0xff) + 0x6d3,
                                       listWindow, 8, j * 16);
          if (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK)
            UiText_DrawNumberAtOffset(
                (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK) >>
                    DJINN_ENTRY_RECOVERY_SHIFT,
                2, listWindow, 48, j * 16);
          Func_08041f70(15);
          j++;
          if (j > 4)
            break;
        }
        drawnFirstEntry = firstEntry;
        redrawList = 0;
      }
      if (djinnCount > 5) {
        s32 listPages;

        for (j = 0; j < (listPages = (djinnCount + 4) / 5); j++) {
          s32 tile = j + 0xf301;

          if (j == firstEntry / 5)
            tile = j + 0xf30b;
          UiWindow_SetTilemapEntry(
              listWindow, tile, listWindow->width - listPages + j - 2, -1, 0);
        }
      }
      Ui_SetRectHighlight(listWindow->x + 1,
                          listWindow->y + selectedRow * 2 + 1,
                          listWindow->width - 2, 1, 14);
      work->dirty = 1;
      work->holdTilemapUpload = 0;
    }

    if (djinnCount > 5) {
      for (j = 0; j < (djinnCount + 4) / 5; j++) {
        s32 tile = j + 0xf301;

        if ((gInput.held & 0x100) || (gFrameCount & 15) <= 11) {
          if (j == firstEntry / 5)
            tile = j + 0xf30b;
        }
        UiWindow_SetTilemapEntry(
            listWindow, tile, listWindow->width - (djinnCount + 4) / 5 + j - 2,
            -1, 0);
      }
      if ((gInput.held & 0x100) == 0) {
        UiWindow_SetTilemapEntry(listWindow, 0xf334,
                                 listWindow->width - (djinnCount + 4) / 5 - 3,
                                 -1, 0);
        UiWindow_SetTilemapEntry(listWindow, 0xf335, listWindow->width - 2, -1,
                                 0);
      } else {
        UiWindow_SetTilemapEntry(listWindow, 0xf011,
                                 listWindow->width - (djinnCount + 4) / 5 - 3,
                                 -1, 0);
        UiWindow_SetTilemapEntry(listWindow, 0xf012, listWindow->width - 2, -1,
                                 0);
      }
      work->dirty |= 2 << ((u32)(listWindow->y - 1) >> 2);
    }

    pressed = gInput.pressed;
    repeated = gInput.repeat;
    held = gInput.held;
    navigation = NAVIGATION;
    if (navigation->tutorialStep != 0) {
      repeated = 0;
      pressed = 0;
      held = 0;
      if (navigation->tutorialDelay == 0) {
        navigation->tutorialDelay = 60;
        repeated = 1;
        pressed = 1;
      } else {
        navigation->tutorialDelay--;
      }
    }
    navigation = NAVIGATION;
    if (navigation->timer == 0 || (pressed & 2)) {
      Audio_PlayCue(113);
      encodedDjinn = -1;
      break;
    }
    if (pressed & 1) {
      if (djinnCount != 0) {
        u32 selection = djinnEntries[firstEntry + selectedRow];
        u32 turns = selection & DJINN_ENTRY_RECOVERY_MASK;

        if (turns == 0) {
          if (alreadySelected == 0) {
            navigation->page = firstEntry;
            navigation->row = selectedRow;
            navigation->preferredRow = preferredRow;
            encodedDjinn = selection;
            break;
          }
        } else if (alreadySelected == 0) {
          Ui_FillVramBlockPattern();
          UiWork_ClearValueNameTables();
          UiWork_PushValueSlot((turns & DJINN_ENTRY_RECOVERY_MASK) >>
                                   DJINN_ENTRY_RECOVERY_SHIFT,
                               5);
          UiText_CopyMessageString(0xcf8, messageText, 52);
          UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
        }
        Audio_PlayCue(114);
      } else {
        encodedDjinn = -1;
        break;
      }
    }

    if (djinnCount != 0) {
      if (repeated & 0x80) {
        Audio_PlayCue(111);
        selectedRow++;
        if (selectedRow == 5 || firstEntry + selectedRow == djinnCount)
          selectedRow = 0;
        preferredRow = selectedRow;
      } else if (repeated & 0x40) {
        Audio_PlayCue(111);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry == (djinnCount - 1) / 5 * 5)
            selectedRow = djinnCount - firstEntry - 1;
          else
            selectedRow = 4;
        }
        preferredRow = selectedRow;
      } else if (held & 0x100) {
        if (previewWindow != NULL) {
          for (i = 0; i < previewPages; i++) {
            s32 tile = i + 0xf301;

            if ((gFrameCount & 15) <= 11 && i == previewPage - 1)
              tile = previewPage + 0xf30a;
            UiWindow_SetTilemapEntry(
                previewWindow, tile,
                previewWindow->width - previewPages + i - 2, -1, 0);
          }
          UiWindow_SetTilemapEntry(previewWindow, 0xf334,
                                   previewWindow->width - previewPages - 3, -1,
                                   0);
          UiWindow_SetTilemapEntry(previewWindow, 0xf335,
                                   previewWindow->width - 2, -1, 0);
          work->dirty |= 2 << (previewWindow->y >> 2);
        }
        if (previewPage == 0 && previewPages != 0) {
          if (previewWindow != NULL)
            RenderOutput_ClearList(previewWindow);
          previewPage = 1;
          redraw = 1;
        } else {
          if (previewPage > previewPages)
            previewPage = previewPages;
          if (previewPage != 0) {
            if (repeated & 0x10) {
              Audio_PlayCue(111);
              previewPage++;
              if (previewPage > previewPages)
                previewPage = 1;
              redraw = 1;
            } else if (repeated & 0x20) {
              Audio_PlayCue(111);
              previewPage--;
              if (previewPage <= 0)
                previewPage = previewPages;
              redraw = 1;
            }
          }
        }
      } else if (previewPage != 0) {
        if (previewWindow != NULL)
          RenderOutput_ClearList(previewWindow);
        previewPage = 0;
        redraw = 1;
        redrawList = 1;
      } else if (repeated & 0x10) {
        Audio_PlayCue(111);
        Input_ResetKeyRepeat();
        if (firstEntry + 5 >= djinnCount) {
          if (firstEntry != 0) {
            firstEntry = 0;
            selectedRow = preferredRow;
          }
        } else {
          firstEntry += 5;
          selectedRow = preferredRow;
          if (firstEntry == (djinnCount - 1) / 5 * 5) {
            selectedRow = djinnCount - firstEntry - 1;
            if (selectedRow > preferredRow)
              selectedRow = preferredRow;
          }
        }
      } else if (repeated & 0x20) {
        Audio_PlayCue(111);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          firstEntry -= 5;
          selectedRow = preferredRow;
        } else {
          firstEntry = (djinnCount - 1) / 5 * 5;
          selectedRow = preferredRow;
          if (firstEntry != 0) {
            selectedRow = djinnCount - firstEntry - 1;
            if (selectedRow > preferredRow)
              selectedRow = preferredRow;
          }
        }
      }
    }

    cursorX = listWindow->x * 8 - 2;
    cursorY = (selectedRow * 2 + listWindow->y) * 8 + 20;
    ((struct DjinnMenuSprite *)&cursorSprite)->attr01 = 0x40000000;
    ((struct DjinnMenuSprite *)&cursorSprite)->attr23 = 0;
    cursorSprite.tile =
        VramCache_Reload(cursorSlot, gResource_FixedBlockBTiles);
    cursorSprite.x = cursorX + ((gFrameCount & 4) >> 1) - 6;
    cursorSprite.y = cursorY - ((gFrameCount & 4) >> 2) - 8;
    if (djinnCount != 0)
      Runtime_PushSlotEntry(&cursorSprite, 242);

    partyStatus = Ram_HeapSlots->windowLayout;
    blinkPhase = gFrameCount & 4;
    if (partyStatus->flags & 2) {
      struct DjinnMenuWindow *iconWindow = partyStatus->window;

      for (i = 0; i <= 3; i++) {
        s32 palette = 15 - (blinkPhase != 0);

        if (((1 << i) & standbyElements) == 0)
          palette = 15;
        Ui_SetRectHighlight(iconWindow->x + gDjinnMenu_ElementXOffsets[i] + 1,
                            iconWindow->y + gDjinnMenu_ElementYOffsets[i] + 1,
                            2, 2, palette);
      }
    }
    if (gFrameCount & 4) {
      Iwram_CopyWords((void *)0x06006500, gDjinnMenu_BlinkTile, 32);
      Iwram_CopyWords((void *)0x06006520, gDjinnMenu_BlinkTile, 32);
    } else {
      Iwram_FillWords((void *)0x06006500, 32, 0x44444444);
      Iwram_CopyWords((void *)0x06006520, gDjinnMenu_BlinkTile, 32);
    }
    WaitFrames(1);
  }

  partyStatus = Ram_HeapSlots->windowLayout;
  if (partyStatus->flags & 2)
    Ui_SetRectHighlight(partyStatus->window->x + 1, partyStatus->window->y + 1,
                        4, 4, 15);
  Resource_ResetEntry(cursorSlot);
  UiWork_Finalize(messageWindow, 1);
  UiWork_Finalize(previewWindow, 1);
  UiWork_Finalize(listWindow, 1);
  Func_08041b68();
  UiWork_SetAltFlagAndClearTable(0);
  MemFree(djinnEntries);
  WaitFrames(1);
  return encodedDjinn;
}

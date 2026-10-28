// Draft, differ 678 of 1246 (1009 before, 2026-10-26): the battle Djinn menu,
// The Broken Seal's BattleDjinnMenu_Run
// (games/tbs/src/window/screens/djinn_menu.c) on this game's work, messages and
// change preview (Func_080464dc). The list window is 10 tiles wide from column
// 20, a recovering Djinni's turns take two digits at x 48, and letting go of R
// redraws the list as well as the stats page. Rewritten toward the twin: the
// cursor through a sprite pointer set before the loop and again in it, the
// cursor position as a two-word struct (spilled whole, as the native's sp 12
// and 16), the twin's chosen-Djinn search, x block, barrier and entry packing
// (1009 to 678; the twin's devices are untagged here until it matches). Left:
// the stack frame is 228 bytes, one stack word more than the native's, and held,
// repeated and pressed take other registers (the native: r8, r7 and r1), so
// allocation differs from the input reads on.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "iwram_call.h"

extern u8 MsgDjinnName[];
extern u8 Msg_09b1[];
extern u8 Msg_0cf8[];
extern u8 Msg_0cf9[];
extern u8 Msg_0d4e[];
extern u8 Msg_0d50[];

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
struct DjinnMenuWindow *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                        s32 flags);
void Text_UseExtraTiles(s32 on);
struct DjinnMenuUnit *Unit_Get(s32 member);
struct DjinnRecoveryTable *Djinn_GetRecoveryTableFar(s32 side);
void Window_HighlightRect(s32 x, s32 y, s32 width, s32 height, s32 palette);
void Window_Close(void *window, s32 mode);
void Vram_FillWindowBlockPattern(void);
void MessageArgument_ClearAll(void);
void MessageArgument_Push(s32 value, s32 kind);
void Message_CopyExpanded(s32 message, u16 *text, s32 capacity);
struct DjinnMenuWindow *Func_080464dc(struct DjinnMenuWindow *window,
                                      s32 member, s32 encodedDjinn, s32 page,
                                      s32 *pageCount);
void Text_DrawWideStringAtOffset(u16 *text, struct DjinnMenuWindow *window,
                                     s32 x, s32 y);
void WindowSprite_RedrawSavedRect(struct DjinnMenuWindow *window);
void Window_SetTilemapEntry(struct DjinnMenuWindow *window, s32 entry, s32 x,
                              s32 y, s32 flags);
void Func_08041f70(s32 color);
void Text_DrawCharacterAtOffset(s32 message, struct DjinnMenuWindow *window,
                                  s32 x, s32 y);
void Text_DrawNumberAtOffset(s32 value, s32 digits,
                               struct DjinnMenuWindow *window, s32 x, s32 y);
void Sound_PlayFar(s32 cue);
void WindowSprite_ClearList(struct DjinnMenuWindow *window);
void Input_ResetKeyRepeat(void);
void Oam_AddSpriteAtDepth(void *sprite, s32 depth);
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
  s32 previewPages;
  s32 redrawList;
  struct DjinnMenuNavigation *navigation;
  struct DjinnMenuUnit *unit;
  struct DjinnRecoveryList *list;
  struct DjinnMenuWindow *listWindow;
  struct DjinnMenuPartyStatus *partyStatus;
  u32 *entryCursor;
  u32 selectedDjinn;
  u32 blinkPhase;
  s32 firstEntry;
  s32 selectedRow;
  u32 encodedDjinn;
  struct {
    s32 x, y;
  } cursorPos;
  s32 pressed;
  s32 repeated;
  s32 held;
  s32 element;
  s32 djinnIndex;
  s32 i;
  s32 j;
  struct DjinnMenuOam *sprite;

  work = (struct DjinnMenuWork *)Ram_HeapSlots->windowTiles;
  previewWindow = NULL;
  drawnRow = -1;
  cursorSlot = VramCache_AllocEntry(0x80);
  djinnEntries = Runtime_BumpAllocateAlternatePool(84 * sizeof(*djinnEntries));
  djinnCount = 0;
  drawnFirstEntry = drawnRow;
  encodedDjinn = 0;
  standbyElements = 0;
  previewPage = 0;
  previewPages = 0;
  redraw = 0;
  alreadySelected = 0;
  redrawList = 0;
  messageWindow = Window_Create(0, 4, 30, 4, 42);
  Text_UseExtraTiles(1);
  listWindow = Window_Create(20, 9, 10, 11, 6);
  navigation = NAVIGATION;
  firstEntry = navigation->page;
  selectedRow = navigation->row;
  preferredRow = navigation->preferredRow;
  unit = Unit_Get(member);

  for (element = 0; element <= 3; element++) {
    for (djinnIndex = 0,
        entryCursor = (u32 *)((djinnCount << 2) + (u32)djinnEntries);
         djinnIndex <= 19; djinnIndex++) {
      if (unit->djinnSet[element] & (u32)(1 << djinnIndex)) {
        *entryCursor = (u32)((element << 8) | djinnIndex);
        entryCursor++;
        djinnCount++;
      } else if (unit->djinnHeld[element] & (u32)(1 << djinnIndex)) {
        u32 v;
        s32 k;

        list = &Djinn_GetRecoveryTableFar((u32)member > 7 ? 1 : 0)->list;
        for (k = 0; k < list->count; k++) {
          if (list->entries[k].unitId == member &&
              list->entries[k].element == element &&
              list->entries[k].index == djinnIndex) {
            break;
          }
        }
        v = (u32)((element << 8) | djinnIndex | DJINN_ENTRY_STANDBY);
        *entryCursor = v;
        if (list->entries[k].turns > 0) {
          v |= (u32)(list->entries[k].turns << DJINN_ENTRY_RECOVERY_SHIFT);
          *entryCursor = v;
        }
        djinnCount++;
        entryCursor++;
      }
    }
  }
  djinnEntries[djinnCount] = DJINN_ENTRY_END;
  work->dirty = 1;

  sprite = &cursorSprite;
  for (;;) {
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow ||
        redraw != 0) {
      selectedDjinn = djinnEntries[firstEntry + selectedRow];
      standbyElements = 0;
      work->holdTilemapUpload = 1;
      Window_HighlightRect(listWindow->x + 1, listWindow->y + drawnRow * 2 + 1,
                          listWindow->width - 2, 1, 15);
      if (previewPage != 0) {
        Window_Close(messageWindow, 1);
        messageWindow = Window_Create(0, 4, 30, 4, 42);
        Vram_FillWindowBlockPattern();
      }
      redraw = 0;
      if (djinnCount != 0) {
        s32 k;
        u32 selected;

        MessageArgument_ClearAll();
        alreadySelected = 0;
        selected = NAVIGATION->chosenDjinn[redraw];
        for (k = redraw; k == redraw;) {
          k = redraw;
          break;
        }
        if (selected == selectedDjinn) {
          alreadySelected = 1;
        } else {
          for (;;) {
            k += 1 - alreadySelected;
            if (k > 7)
              break;
            if (NAVIGATION->chosenDjinn[k] == selectedDjinn) {
              alreadySelected = 1;
              break;
            }
          }
        }
        if (alreadySelected != 0) {
          Message_CopyExpanded((s32)Msg_0d50, messageText, 52);
          if (previewWindow != NULL) {
            Window_Close(previewWindow, 1);
            previewWindow = NULL;
            previewPage = 0;
          }
        } else if (selectedDjinn & DJINN_ENTRY_STANDBY) {
          if (selectedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            MessageArgument_Push((selectedDjinn & DJINN_ENTRY_RECOVERY_MASK) >>
                                     DJINN_ENTRY_RECOVERY_SHIFT,
                                 5);
            Message_CopyExpanded(((selectedDjinn & 0xf00) >> 8) * 20 +
                                         (selectedDjinn & 0xff) + (s32)Msg_09b1,
                                     messageText, 52);
            if (previewWindow != NULL) {
              Window_Close(previewWindow, 1);
              previewWindow = NULL;
              previewPage = 0;
            }
          } else {
            previewWindow = Func_080464dc(previewWindow, member, selectedDjinn,
                                          previewPage, &previewPages);
            Message_CopyExpanded((s32)Msg_0cf9, messageText, 52);
            standbyElements = 1 << ((selectedDjinn & 0xf00) >> 8);
          }
        } else {
          previewWindow = Func_080464dc(previewWindow, member, selectedDjinn,
                                        previewPage, &previewPages);
          Message_CopyExpanded(((selectedDjinn & 0xf00) >> 8) * 20 +
                                       (selectedDjinn & 0xff) + (s32)Msg_09b1,
                                   messageText, 52);
        }
      } else {
        Message_CopyExpanded((s32)Msg_0d4e, messageText, 52);
      }
      work->holdTilemapUpload = 0;
      if (previewPage == 0) {
        work->holdTilemapUpload = 1;
        Window_Close(messageWindow, 1);
        messageWindow = Window_Create(0, 4, 30, 4, 42);
        work->holdTilemapUpload = previewPage;
      }
      Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      if (firstEntry != drawnFirstEntry || redrawList == 1) {
        WindowSprite_RedrawSavedRect(listWindow);
        j = 0;
        while ((encodedDjinn = djinnEntries[firstEntry + j]) !=
               DJINN_ENTRY_END) {
          Window_SetTilemapEntry(
              listWindow, ((encodedDjinn & 0xf00) >> 8) + 0x5001, 0, j * 2, 0);
          if (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK)
            Func_08041f70(4);
          else if (encodedDjinn & DJINN_ENTRY_STANDBY)
            Func_08041f70(2);
          Text_DrawCharacterAtOffset(((encodedDjinn & 0xf00) >> 8) * 20 +
                                           (encodedDjinn & 0xff) +
                                           (s32)MsgDjinnName,
                                       listWindow, 8, j * 16);
          if (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK)
            Text_DrawNumberAtOffset(
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
          Window_SetTilemapEntry(
              listWindow, tile, listWindow->width - listPages + j - 2, -1, 0);
        }
      }
      Window_HighlightRect(listWindow->x + 1,
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
        Window_SetTilemapEntry(
            listWindow, tile, listWindow->width - (djinnCount + 4) / 5 + j - 2,
            -1, 0);
      }
      if ((gInput.held & 0x100) == 0) {
        Window_SetTilemapEntry(listWindow, 0xf334,
                                 listWindow->width - (djinnCount + 4) / 5 - 3,
                                 -1, 0);
        Window_SetTilemapEntry(listWindow, 0xf335, listWindow->width - 2, -1,
                                 0);
      } else {
        Window_SetTilemapEntry(listWindow, 0xf011,
                                 listWindow->width - (djinnCount + 4) / 5 - 3,
                                 -1, 0);
        Window_SetTilemapEntry(listWindow, 0xf012, listWindow->width - 2, -1,
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
      Sound_PlayFar(113);
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
          Vram_FillWindowBlockPattern();
          MessageArgument_ClearAll();
          MessageArgument_Push((turns & DJINN_ENTRY_RECOVERY_MASK) >>
                                   DJINN_ENTRY_RECOVERY_SHIFT,
                               5);
          Message_CopyExpanded((s32)Msg_0cf8, messageText, 52);
          Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
        }
        Sound_PlayFar(114);
      } else {
        encodedDjinn = -1;
        break;
      }
    }

    if (djinnCount != 0) {
      if (repeated & 0x80) {
        Sound_PlayFar(111);
        selectedRow++;
        if (selectedRow == 5 || firstEntry + selectedRow == djinnCount)
          selectedRow = 0;
        preferredRow = selectedRow;
      } else if (repeated & 0x40) {
        Sound_PlayFar(111);
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
            Window_SetTilemapEntry(
                previewWindow, tile,
                previewWindow->width - previewPages + i - 2, -1, 0);
          }
          Window_SetTilemapEntry(previewWindow, 0xf334,
                                   previewWindow->width - previewPages - 3, -1,
                                   0);
          Window_SetTilemapEntry(previewWindow, 0xf335,
                                   previewWindow->width - 2, -1, 0);
          work->dirty |= 2 << (previewWindow->y >> 2);
        }
        if (previewPage == 0 && previewPages != 0) {
          if (previewWindow != NULL)
            WindowSprite_ClearList(previewWindow);
          previewPage = 1;
          redraw = 1;
        } else {
          if (previewPage > previewPages)
            previewPage = previewPages;
          if (previewPage != 0) {
            if (repeated & 0x10) {
              Sound_PlayFar(111);
              previewPage++;
              if (previewPage > previewPages)
                previewPage = 1;
              redraw = 1;
            } else if (repeated & 0x20) {
              Sound_PlayFar(111);
              previewPage--;
              if (previewPage <= 0)
                previewPage = previewPages;
              redraw = 1;
            }
          }
        }
      } else if (previewPage != 0) {
        if (previewWindow != NULL)
          WindowSprite_ClearList(previewWindow);
        previewPage = 0;
        redraw = 1;
        redrawList = 1;
      } else if (repeated & 0x10) {
        Sound_PlayFar(111);
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
        Sound_PlayFar(111);
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

    {
      s32 cx;

      do {
        cx = listWindow->x * 8;
      } while (0);
      asm("" : : "l"(selectedRow));
      cursorPos.x = cx - 2;
    }
    cursorPos.y = (selectedRow * 2 + listWindow->y) * 8 + 20;
    sprite = &cursorSprite;
    ((struct DjinnMenuSprite *)sprite)->attr01 = 0x40000000;
    ((struct DjinnMenuSprite *)sprite)->attr23 = 0;
    sprite->tile = VramCache_Reload(cursorSlot, gResource_FixedBlockBTiles);
    sprite->x = cursorPos.x + ((gFrameCount & 4) >> 1) - 6;
    sprite->y = cursorPos.y - ((gFrameCount & 4) >> 2) - 8;
    if (djinnCount != 0)
      Oam_AddSpriteAtDepth(sprite, 242);

    partyStatus = Ram_HeapSlots->windowLayout;
    blinkPhase = gFrameCount & 4;
    if (partyStatus->flags & 2) {
      struct DjinnMenuWindow *iconWindow = partyStatus->window;

      for (i = 0; i <= 3; i++) {
        s32 palette = 15 - (blinkPhase != 0);

        if (((1 << i) & standbyElements) == 0)
          palette = 15;
        Window_HighlightRect(iconWindow->x + gDjinnMenu_ElementXOffsets[i] + 1,
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
    Window_HighlightRect(partyStatus->window->x + 1, partyStatus->window->y + 1,
                        4, 4, 15);
  Resource_ResetEntry(cursorSlot);
  Window_Close(messageWindow, 1);
  Window_Close(previewWindow, 1);
  Window_Close(listWindow, 1);
  Func_08041b68();
  Text_UseExtraTiles(0);
  MemFree(djinnEntries);
  WaitFrames(1);
  return encodedDjinn;
}

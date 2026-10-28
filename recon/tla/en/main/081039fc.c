// Near miss, differ 38 of 1244 (2026-10-26; 148 before), instruction order
// only. What changed, each from the twin's shape: the page row marked through
// UiRender_MarkRow (136); Start's cue played before result = -2, so its tail
// joins the other cancel's (the listing's jump into it); row cleared before
// the pair entries are read and set to 1, scaled at the call, so jump.c keeps
// the branch rather than a store-flag form (90; the row = 8 device is
// gone); the message window declared in each tutorial case (60) and the saved
// tilemap's size in a variable (52), as the twin has them; the twin's tagged
// one-pass blocks around the cursor's stores (38). Left: about nine pairs of
// a load or constant one slot from the native (the prologue's mode spill, the
// cursor store at entry, the first pass's cleared element and index, the
// tutorial's timer offsets). A greedy search of empty asm barriers lowers it
// by 2 per barrier (four: 28), so none is kept.
// First draft of Func_081039fc, the Djinn change preview, the twin of The
// Broken Seal's PsynergyList_RunCompareLoop
// (games/tbs/src/menu/menu/djinn_preview.c). mode is 0 for a trade, 1 for a
// give, 2 for standby and 3 for set. The Lost Age places the two members'
// figures by slot rather than by column, draws each Djinni's name through
// Func_08101a54 and keeps the window work's tilemap 8 bytes in.
// Reached from Func_0810106c. The message names are The Broken Seal's; this
// game's text has no symbols for them yet.
// Differ 160 of 1244 (2026-10-25). Devices: the two wrappers around the frame
// tile's copy and fill, as menu/cancel.c has them (each orders the pool
// loads; 8 more differ without), and the tutorial's one-pass blocks.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "sequence_parent_work.h"
#include "iwram_call.h"
#include "uiwindow.h"

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

struct DjinnPreviewWindow {
  u8 unk00[8];
  u16 width;
  u8 unk0a[4];
  s16 y;
};

struct DjinnMenuWork {
  u8 unk000[0x10];
  s32 statusWindow;
  struct DjinnMenuIcon *cursor;
  u8 unk018[4];
  s8 selectedColumn[2];
  u8 unk01e[0xa];
  s32 window;
  s32 unopenedWindow;
  u8 unk030[4];
  s32 listWindow;
  struct DjinnPreviewWindow *secondWindow;
  u8 unk03c[0xb4];
  s32 helpWindow;
  u8 unk0f4[0x24];
  s16 memberX[8];
  s16 memberY[8];
  u8 unk138[0x30];
  u16 selection[2];
  u16 pairEntries[2];
  u8 unk170[0x10];
  struct DjinnMenuBackdrop *backdrop;
  u8 unk184[0x80];
  u16 owners[8];
  u8 unk214;
  u8 memberCount;
  u8 shownMember[2];
  u8 unk218[0x40];
  u8 pairIndex[2];
  u8 pairElement[2];
  u8 pairMember[2];
};

struct DjinnWindowWork {
  u8 unk0[3];
  u8 dirty;
  u8 unk4[2];
  u8 holdTilemapUpload;
  u8 unk7;
  u8 tilemap[0x200];
};

extern u8 MsgDjinnCompareSet[];
extern u8 MsgDjinnCompareStandby[];
extern u8 MsgDjinnCompareGive[];
extern u8 MsgDjinnCompareTrade[];
extern u8 MsgDjinnPsyHelp[];
extern u8 MsgDjinnStatusHelp[];
extern u8 MsgDjinnCompareConfirm[];
extern u8 gMenu_BackdropBorderTile[];

void wait(s32 frames);
void Sound_PlayFar(s32 cue);
s32 Party_ListActiveOwnersFar(u16 *owners);
void Func_08104e38(s32 first, s32 second);
void WindowSprite_ClearListFar(s32 window);
void WindowSprite_RedrawSavedRectFar(s32 window);
void WindowSprite_PrepareForRedrawFar(s32 window);
void Window_CloseIfOpen(void *window, s32 mode);
s32 Window_UpdateOrCreate(void *handle, s32 x, s32 y, s32 width, s32 height,
                            s32 style);
void *Heap_AllocEwram(s32 size);
void Heap_Release(void *buffer);
void *Unit_Get(s32 owner);
void Djinn_ActivateFar(s32 owner, s32 element, s32 index);
void Djinn_DeactivateFar(s32 owner, s32 element, s32 index);
s32 Djinn_AddToOwnerFar(s32 owner, s32 element, s32 index);
void Unit_CalculateFar(s32 owner);
s32 Func_08101860(u16 *before, u16 *after, u16 *out, s32 *gained, s32 *lost);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Func_08101a54(s32 window, s32 x, s32 y, s32 entry);
s32 Text_SetColorFar(s32 color);
void Window_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 mode);
void Window_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width,
                                    s32 height);
void MessageArgument_PushFar(s32 value, s32 kind);
s32 Menu_GetDjinnGiveMessage(void);
s32 Message_OpenWindowFar(s32 message, s32 x, s32 y, s32 style);
s32 Message_AllTypedFar(void);
void Window_CloseFar(s32 window, s32 style);
void Func_08038290(void);
void Func_080f8a44(s32 x, s32 y);
void Func_080f8ab4(s32 x, s32 y);
s32 Func_08103168(s32 mode, s32 param);
void Func_08103218(s32 window, s32 x, s32 y, s32 owner, s32 a, s32 b, s32 c,
                   s32 page, s32 d);
void Func_08105350(s32 index, s32 scale);
void MenuIcon_SetPosition(s32 index, s32 x, s32 y);
s32 Trig_Sin(s32 angle);
s32 Menu_GetModuloOfSum(s32 value, s32 count);
void Input_ResetKeyRepeat(void);
s32 Task_Add(void (*func)(void), s32 order);
void Menu_UpdateEntryObjectTransforms(void);

typedef s32 (*CopyFn)(void *, const void *, s32);
typedef s32 (*FillFn)(void *, s32, u32);

static __inline__ void CopyWords(const void *src, CopyFn copy, void *dst,
                                 s32 size) {
  /* FAKEMATCH(wrapper): the source loads before the routine and the
   * destination, as the native does. */
  copy(dst, src, size);
}

static __inline__ void FillWords(FillFn fill, void *dst, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): the routine loads before its arguments. */
  fill(dst, size, value);
}

s32 Func_081039fc(u32 mode) {
  struct DjinnMenuWork *menu;
  struct DjinnMenuBackdrop *backdrop;
  struct DjinnWindowWork *work;
  struct DjinnPreviewWindow *list;
  s32 pageCounts[2];
  s32 selectedPages[2];
  u16 owners[14];
  s32 confirmationChoice;
  u32 redraw;
  s32 frame;
  s32 page;
  s32 help;
  s32 previewMode;
  s32 addPairSlot;
  s32 step;
  s32 result;
  s32 index;
  s32 memberIndex;
  s32 first;
  s32 second;
  u32 entry0;
  u32 entry1;
  s32 row;

  menu = Ram_HeapSlots->menuRuntime;
  confirmationChoice = 0;
  result = 0;
  redraw = 1;
  frame = 0;
  page = 2;
  help = menu->helpWindow;
  menu->cursor->active = 13;
  selectedPages[0] = 0;
  selectedPages[1] = 0;
  Party_ListActiveOwnersFar(owners);
  first = owners[menu->selectedColumn[0]];
  second = owners[menu->selectedColumn[1]];
  Func_08104e38(first, second);
  for (memberIndex = 3; memberIndex >= 0; memberIndex--)
    menu->memberY[memberIndex] = 200;
  menu->memberX[0] = 16;
  menu->memberY[0] = 32;
  if (first != second) {
    menu->memberX[1] = 120;
    menu->memberY[1] = 32;
  }
  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  Window_CloseIfOpen(&menu->helpWindow, 1);
  Window_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);
  pageCounts[0] = 1;
  pageCounts[1] = 1;
  if (mode <= 1) {
    u16 *diffWork;
    u8 *original;
    u8 *unit;
    s32 memberId;
    s32 firstElement;
    s32 firstIndex;
    s32 secondElement;
    s32 secondIndex;
    s32 firstWasSet;
    s32 secondWasSet;
    s32 lostCount;
    s32 gainedCount;
    s32 size;

    diffWork = Heap_AllocEwram(96);
    size = 332;
    original = Heap_AllocEwram(size);
    secondElement = 0;
    secondIndex = 0;
    memberId = menu->shownMember[0];
    firstElement = menu->pairElement[0];
    firstIndex = menu->pairIndex[0] & 31;
    firstWasSet = (u16)(menu->pairEntries[0] & 0x8000);
    unit = Unit_Get(memberId);
    Iwram_CopyWords(original, unit, size);
    Djinn_DeactivateFar(memberId, firstElement, firstIndex);
    if (mode == 0) {
      secondElement = menu->pairElement[1];
      secondIndex = menu->pairIndex[1] & 31;
      secondWasSet = (u16)(menu->pairEntries[1] & 0x8000);
      Djinn_AddToOwnerFar(memberId, secondElement, secondIndex);
      if (secondWasSet != 0)
        Djinn_ActivateFar(memberId, secondElement, secondIndex);
    }
    Unit_CalculateFar(memberId);
    pageCounts[0] = Func_08101860((u16 *)(original + 88), (u16 *)(unit + 88),
                                  diffWork, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, original, size);

    memberId = menu->shownMember[1];
    unit = Unit_Get(memberId);
    Iwram_CopyWords(original, unit, size);
    if (mode == 0)
      Djinn_DeactivateFar(memberId, secondElement, secondIndex);
    Djinn_AddToOwnerFar(memberId, firstElement, firstIndex);
    if (firstWasSet != 0)
      Djinn_ActivateFar(memberId, firstElement, firstIndex);
    Unit_CalculateFar(memberId);
    pageCounts[1] = Func_08101860((u16 *)(original + 88), (u16 *)(unit + 88),
                                  diffWork, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, original, size);
    Heap_Release(original);
    Heap_Release(diffWork);
  } else if (mode == 2 || mode == 3) {
    u16 *diffWork;
    u8 *original;
    u8 *unit;
    s32 memberId;
    s32 firstElement;
    s32 firstIndex;
    s32 applySecond;
    s32 lostCount;
    s32 gainedCount;

    diffWork = Heap_AllocEwram(96);
    original = Heap_AllocEwram(332);
    memberId = menu->shownMember[0];
    firstElement = menu->pairElement[0];
    firstIndex = menu->pairIndex[0] & 31;
    applySecond = (mode == 3);
    unit = Unit_Get(memberId);
    Iwram_CopyWords(original, unit, 332);
    Djinn_DeactivateFar(memberId, firstElement, firstIndex);
    if (applySecond != 0)
      Djinn_ActivateFar(memberId, firstElement, firstIndex);
    Unit_CalculateFar(memberId);
    pageCounts[0] = Func_08101860((u16 *)(original + 88), (u16 *)(unit + 88),
                                  diffWork, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, original, 332);
    pageCounts[1] = pageCounts[0];
    page = 0;
    Heap_Release(original);
    Heap_Release(diffWork);
  }
  pageCounts[0] = (pageCounts[0] - 1) / 5 + 1;
  if (pageCounts[0] == 0)
    pageCounts[0] = 1;
  pageCounts[1] = (pageCounts[1] - 1) / 5 + 1;
  if (pageCounts[1] == 0)
    pageCounts[1] = 1;
  Window_UpdateOrCreate(&menu->window, 0, 5, 15, 15, 2);
  Window_UpdateOrCreate(&menu->secondWindow, 15, 5, 15, 15, 2);
  WindowSprite_RedrawSavedRectFar(help);
  if (mode == 2) {
    Text_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareStandby, help, 96, 0);
    Text_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareConfirm, help, 96, 16);
  } else if (mode == 3) {
    Text_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareSet, help, 96, 0);
    Text_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareConfirm, help, 96, 16);
  } else if (mode == 0) {
    Text_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareTrade, help, 128, 0);
    Text_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareConfirm, help, 128, 8);
  } else {
    Text_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareGive, help, 128, 0);
    Text_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareConfirm, help, 128, 8);
  }
  row = 0;
  entry0 = menu->pairEntries[0];
  entry1 = menu->pairEntries[1];
  if (mode == 1)
    row = 1;
  if ((entry0 & 0x8000) == 0)
    Text_SetColorFar(2);
  Func_08101a54(help, 40, row * 8, entry0);
  if (mode == 0) {
    if ((entry1 & 0x8000) == 0)
      Text_SetColorFar(2);
    Func_08101a54(help, 40, 16, entry1);
  } else if (mode - 2 <= 1) {
    if ((entry0 & 0x8000) != 0)
      Text_SetColorFar(2);
    Func_08101a54(help, 40, 16, entry0);
    Window_SetTilemapEntryFar(help, 0xf296, 7, 1, 0);
  }

  for (;;) {
    u32 pressed;
    u32 repeat;

    work = (struct DjinnWindowWork *)Ram_HeapSlots->windowTiles;
    backdrop = menu->backdrop;
    pressed = gInput.pressed;
    repeat = gInput.repeat;
    if (backdrop->tutorialStep != 0) {
      u8 *saved;
      s32 size;

      size = 0x200;
      saved = Heap_AllocEwram(size);
      pressed = 0;
      repeat = 0;
      backdrop->tutorialTimer++;
      switch (backdrop->tutorialStep) {
      case 5:
      case 12:
      case 23:
        if (backdrop->tutorialTimer == 100) {
          repeat = 1;
          pressed = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case 10:
        if (backdrop->tutorialTimer == 60) {
          s32 window;

          Iwram_CopyWords(saved, work->tilemap, size);
          window = Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + 3,
                                               8, 0, 1);
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, as The
           * Broken Seal's twin has it; as a plain statement (both cases), 46 of
           * 1244 instructions differ in tla-en rather than 38. */
          do {
            menu->cursor->active = 1;
          } while (0);
          Func_080f8ab4(2, 96);
          gSequenceParentWork.eventMode = 1;
          while (Message_AllTypedFar() == 0)
            wait(1);
          Window_CloseFar(window, 1);
          Iwram_CopyWords(work->tilemap, saved, size);
          Func_08038290();
          work->dirty = 1;
          redraw = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 11;
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, as The
           * Broken Seal's twin has it; as a plain statement (both cases), 44 of
           * 1244 instructions differ in tla-en rather than 38. */
          do {
            menu->cursor->active = 13;
          } while (0);
        }
        break;
      case 11:
        if (backdrop->tutorialTimer == 60) {
          s32 window;

          Iwram_CopyWords(saved, work->tilemap, size);
          window = Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + 2,
                                               8, 0, 1);
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, as The
           * Broken Seal's twin has it; as a plain statement (both cases), 46 of
           * 1244 instructions differ in tla-en rather than 38. */
          do {
            menu->cursor->active = 1;
          } while (0);
          Func_080f8ab4(106, 56);
          gSequenceParentWork.eventMode = 1;
          while (Message_AllTypedFar() == 0)
            wait(1);
          Window_CloseFar(window, 1);
          Iwram_CopyWords(work->tilemap, saved, size);
          Func_08038290();
          work->dirty = 1;
          wait(1);
          redraw = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 12;
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, as The
           * Broken Seal's twin has it; as a plain statement (both cases), 44 of
           * 1244 instructions differ in tla-en rather than 38. */
          do {
            menu->cursor->active = 13;
          } while (0);
        }
        break;
      }
      Heap_Release(saved);
    }

    if (redraw != 0) {
      addPairSlot = 1;
      if (mode <= 1) {
        Window_ClearInteriorTilesFar(help, 128, 16, 224, 24);
        if (page == 1) {
          Text_DrawCharacterAtOffsetFar((s32)MsgDjinnStatusHelp, help, 128,
                                          16);
        } else {
          MessageArgument_PushFar(menu->shownMember[page != 2], 1);
          Text_DrawCharacterAtOffsetFar((s32)MsgDjinnPsyHelp, help, 128, 16);
        }
      }
      work->holdTilemapUpload = 1;
      WindowSprite_PrepareForRedrawFar(menu->window);
      WindowSprite_PrepareForRedrawFar((s32)menu->secondWindow);
      if ((redraw >> 1) != 0)
        wait(1);
      if (mode == 3) {
        previewMode = 0;
        addPairSlot = 0;
      } else if (mode == 2) {
        previewMode = 1;
        addPairSlot = 0;
      } else {
        previewMode = 2;
      }
      switch (page) {
      default:
        Func_08103168(mode, 0);
        break;
      case 1:
        if (mode == 1)
          previewMode = 4;
        Func_08103218(menu->window, 0, 0, menu->shownMember[1], page, 0,
                      previewMode, 0, 0);
        Func_08103218((s32)menu->secondWindow, 0, 0, menu->shownMember[1], page,
                      0, previewMode, selectedPages[1] + 1, 0);
        break;
      case 0:
        if (mode == 1) {
          previewMode = 1;
          addPairSlot = 0;
        }
        Func_08103218(menu->window, 0, 0, menu->pairMember[0], page,
                      addPairSlot, previewMode, page, 1);
        Func_08103218((s32)menu->secondWindow, 0, 0, menu->pairMember[0], page,
                      addPairSlot, previewMode, selectedPages[0] + 1, 1);
        break;
      }
      work->holdTilemapUpload = 0;
    }

    if (page <= 1) {
      list = menu->secondWindow;
      if (pageCounts[page] > 1) {
        for (index = 0; index < pageCounts[page]; index++) {
          s32 tile;

          tile = 0xf031 + index;
          if (index > 8)
            tile = 0xf03f;
          if (index == selectedPages[page])
            tile -= 0x1000;
          Window_SetTilemapEntryFar(
              (s32)list, tile, list->width - pageCounts[page] + index - 2, -1,
              0);
        }
        Window_SetTilemapEntryFar((s32)list, 0xf128,
                                    list->width - pageCounts[page] - 3, -1, 0);
        Window_SetTilemapEntryFar((s32)list, 0xf129, list->width - 2, -1, 0);
        UiRender_MarkRow(work, list->y);
      }
    }

    frame++;
    step = frame % 60 - 5;
    if (step < 0)
      step = 0;
    if (step > 29)
      step = 29;
    Func_08105350(0, 0xffff4000);
    Func_08105350(1, 0xffff4000);
    if (mode <= 1) {
      s32 rise;
      s32 slide;

      rise = -Iwram_MulQ16(16, Trig_Sin(step % 30 * 0x444));
      if (rise < -6)
        rise = -6;
      if (rise > 12)
        rise = 12;
      slide = step % 35 * 2;
      MenuIcon_SetPosition(0, slide + 34, rise + 20);
      if (mode == 0)
        MenuIcon_SetPosition(1, 99 - slide, 36 - rise);
    } else {
      MenuIcon_SetPosition(0, 32, 30);
    }
    if (redraw != 0) {
      redraw = 0;
      confirmationChoice = Menu_GetModuloOfSum(confirmationChoice, 2);
    }
    Func_080f8a44(confirmationChoice * 56 + 80, 16);
    if ((frame & 3) == 0) {
      if ((frame & 4) != 0)
        CopyWords(gMenu_BackdropBorderTile, Iwram_CopyWords, (void *)0x060052c0,
                  32);
      else
        FillWords(Iwram_FillWords, (void *)0x060052c0, 32, 0x44444444);
    }

    if ((pressed & 1) != 0) {
      if (confirmationChoice == 0) {
        result = 1;
        break;
      }
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if ((pressed & 8) != 0) {
      Sound_PlayFar(113);
      result = -2;
      break;
    } else if ((pressed & 2) != 0) {
      Sound_PlayFar(113);
      result = -1;
      break;
    } else if ((pressed & 0x100) != 0) {
      if (mode <= 1) {
        page = (page + 1) % 3;
        redraw = 2;
        Sound_PlayFar(111);
      }
    } else if ((repeat & 0x20) != 0) {
      if (page <= 1) {
        selectedPages[page]--;
        selectedPages[page] =
            Menu_GetModuloOfSum(selectedPages[page], pageCounts[page]);
        Sound_PlayFar(111);
        Input_ResetKeyRepeat();
        redraw = 1;
      }
    } else if ((repeat & 0x10) != 0) {
      if (page <= 1) {
        Sound_PlayFar(111);
        Input_ResetKeyRepeat();
        selectedPages[page]++;
        redraw = 1;
        selectedPages[page] =
            Menu_GetModuloOfSum(selectedPages[page], pageCounts[page]);
      }
    }
    wait(1);
  }

  Func_08105350(0, 0);
  Func_08105350(1, 0);
  Task_Add(Menu_UpdateEntryObjectTransforms, 0x480);
  ((struct DjinnWindowWork *)Ram_HeapSlots->windowTiles)->holdTilemapUpload = 1;
  Window_CloseIfOpen(&menu->helpWindow, 1);
  wait(1);
  Window_UpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, 2);
  Window_CloseIfOpen(&menu->window, 1);
  Window_CloseIfOpen(&menu->secondWindow, 1);
  WindowSprite_RedrawSavedRectFar(menu->listWindow);
  WindowSprite_RedrawSavedRectFar(menu->unopenedWindow);
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  ((struct DjinnWindowWork *)Ram_HeapSlots->windowTiles)->holdTilemapUpload = 0;
  wait(1);
  return result;
}

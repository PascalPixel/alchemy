/// @file
/// The Djinn menu: its backdrop screen, the Djinn list and the screen's loop.
// Part of menu.c.

#include "../menu_internal.h"

s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1) { return (arg0 + arg1) % arg1; }

s32 Menu_SetFirstObjectRowCoordinates(s32 arg0) {
  u8 *current;
  s32 value;
  s32 count;
  current = (u8 *)Ram_HeapSlots->menuRuntime + 0x118;
  arg0 += 0x3D;
  value = 0x20;
  count = 3;
  do {
    count--;
    *(s16 *)current = value;
    *(s16 *)(current + 0x10) = arg0;
    value += 0x38;
    current += 2;
  } while (count >= 0);
  return arg0;
}

s32 Menu_GetDjinnGiveMessage(void) {
  if (GameFlag_Test(0x175))
    return (s32)MsgDjinnGiveTutorialB;
  return (s32)MsgDjinnGiveTutorialA;
}

// Func_08100e7c runs the Djinn menu, keeping the field's tiles and the party's
// event mode; the twin of The Broken Seal's DjinnMenu_Run
// (games/tbs/src/menu/menu/djinn.c), rewritten with this game's heap block
// (220, 0xcec), menu work and calls (Func_0810106c is the screen loop).

void Resource_LoadPairedBlocksIfAvailable(void);

s32 Func_0810106c(void);

s32 Func_08100e7c(void) {
  struct DjinnMenuWork *menu;
  u32 savedEventMode;
  s32 enabled;
  struct DjinnMenuBackdrop *backdrop;
  s32 zero;

  menu = HeapSlot_Alloc(220, 0xcec);
  savedEventMode = gMainRoutineWork.eventMode;
  gMainRoutineWork.eventMode = 2;
  enabled = Ram_HeapSlots->objectSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeWork(0);
  backdrop = Heap_AllocEwram(0x2130);
  menu->backdrop = backdrop;
  backdrop->tutorialTimer = 0;
  backdrop->tutorialStep = 0;
  if (GameFlag_Test(0x16e)) {
    if (!GameFlag_Test(0x16f)) {
      if (!GameFlag_Test(0x171))
        backdrop->tutorialStep = enabled;
      else
        backdrop->tutorialStep = 14;
    } else {
      if (!GameFlag_Test(0x171))
        backdrop->tutorialStep = 27;
      else
        backdrop->tutorialStep = 28;
    }
  }
  Menu_SuspendFieldTasks();
  Text_UseExtraTilesFar(1);
  Link_DrawShiftedTilePairFar((void *)0x06002500);
  menu->memberCount = Party_ListActiveOwnersFar(menu->owners);
  Resource_LoadPairedBlocksIfAvailable();
  ItemMenu_Initialize(0, 3, 0, 7);
  Menu_SetFirstObjectRowCoordinates(0);
  Palette_LightenHighlight(14);
  menu->helpWindow = Window_CreateFar(13, 0, 17, 5, 2);
  menu->pairEntries[0] = 255;
  zero = 0;
  menu->selectedColumn[0] = zero;
  menu->selectedColumn[1] = zero;
  menu->selection[0] = 0;
  menu->selection[1] = 0;
  Func_081051a8();
  Func_0810106c();
  MenuIcon_ReleaseObjects();
  Menu_ResetTwoResourceEntries();
  wait(1);
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  Ram_HeapSlots->objectSystem->suspended = 0;
  Func_08038290();
  Text_UseExtraTilesFar(0);
  /* FAKEMATCH(wrapper): forwarding through item.c's CopyWords_ inline keeps
   * each call's own constants; as plain calls, 4 of 201 instructions differ
   * in tla-en. */
  CopyWords_((void *)0x06004000, backdrop->tiles, 0x2000);
  CopyWords_((void *)&BG_PLTT_COLOR(4, 0), backdrop->palette, 128);
  wait(1);
  Menu_ResumeFieldTasks();
  Window_EraseBorderRectFar(0, 0, 30, 20);
  Heap_Release(menu->backdrop);
  HeapSlot_Release(220);
  Event_ClearInvalidPackedValuesFar();
  gMainRoutineWork.eventMode = savedEventMode;
  return 1;
}

struct DjinnElementList {
  u16 ids[8][10];
  s8 counts[8];
};

void Func_08101638(void);

s32 Func_08102008(s32 mode);
s32 Func_08104540(void);
s32 Func_08101d5c(void);
s32 Func_081039fc(u32 mode);
s32 Djinn_TransferFar(s32 owner, s32 element, s32 index, s32 recipient);

u32 Djinn_DeactivateFar(s32 owner, s32 element, s32 index);
s32 DjinnRecovery_AddStandbyFar(s32 owner, s32 element, s32 index);

void DjinnMenu_BuildLists(struct OwnerList *list);

// Find the moved Djinni in the list and select its new position.
#define DJINN_RESELECT_PAIR_ENTRY(menu, i)                                     \
  do {                                                                         \
    s32 column;                                                                \
    s32 found;                                                                 \
    u8 entry;                                                                  \
                                                                               \
    column = (u16)((u32)(menu)->selection[1] % 10);                            \
    found = 0;                                                                 \
    entry = (menu)->pairEntries[0];                                            \
    for ((i) = 0; (i) < (menu)->backdrop->counts[column]; (i)++) {             \
      if (entry == (u8)(menu)->backdrop->ids[column][(i)]) {                   \
        found = (i);                                                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    (menu)->selection[0] = column + found * 10;                                \
  } while (0)

/// The Djinn menu's screen loop, the twin of The Broken Seal's
/// DjinnMenu_RunScreen. The Lost Age drops the pair steps 4 and 8, hides the
/// page arrows before each confirmation and adds Func_08104540 (step 10) and
/// Func_08101d5c (step 15) as its status and help pages.
s32 Func_0810106c(void) {
  /* FAKEMATCH(temporary): one icon local holding the cursor and then each
     arrow; as a cursor local and plain arrow stores, 109 of 603 instructions
     differ in tla-en. */
  struct DjinnMenuWork *menu;
  struct MenuIcon *icon;
  s32 result;
  s32 step;
  s32 menuResult;
  s32 done;
  s32 listIndex;

  done = 0;
  result = 0;
  menuResult = 0;
  menu = Ram_HeapSlots->menuRuntime;
  icon = menu->cursor;
  icon->active = 13;
  icon->animStep = 0;
  Func_08101638();
  wait(1);
  step = 2;
  do {
    switch (step) {
    case 0:
      if (result < 0) {
        menuResult = -1;
        done = 1;
      }
      step = 2;
      break;
    case 2:
      Menu_SetFirstObjectRowCoordinates(0);
      MenuIcon_SetPosition(1, 0, 200);
      result = Func_08102008(0);
      step = 15;
      if (result == 10)
        break;
      step = 0;
      if (result < 0)
        break;
      menu->selection[1] = menu->selectedColumn[0];
      step = 10;
      if (result == 7)
        break;
      step = 3;
      break;
    case 10:
      menu->owner = menu->owners[menu->selectedColumn[0]];
      menu->shownMember[0] = menu->owners[menu->selectedColumn[0]];
      icon = menu->arrows[0];
      icon->active = 13;
      icon = menu->arrows[1];
      icon->active = 13;
      result = Func_08104540();
      if (result == -2)
        done = 1;
      step = 2;
      break;
    case 15:
      icon = menu->arrows[0];
      icon->active = 13;
      icon = menu->arrows[1];
      icon->active = 13;
      result = Func_08101d5c();
      if (result == -2)
        done = 1;
      step = 2;
      break;
    case 3:
      Menu_SetFirstObjectRowCoordinates(-8);
      menu->owner = menu->owners[menu->selectedColumn[0]];
      menu->shownMember[0] = menu->owners[menu->selectedColumn[0]];
      MenuIcon_SetPosition(0, menu->selectedColumn[0] * 56 + 48, 54);
      result = Func_08102008(1);
      for (listIndex = 0; listIndex < menu->memberCount; listIndex++)
        menu->memberY[listIndex] += 8;
      if (result == -2)
        done = 1;
      if (result < 0) {
        step = 2;
        break;
      }
      if (result == 3 || result == 4 || result == 8 || result == 9)
        menu->shownMember[1] = menu->owners[menu->selectedColumn[1]];
      if (result < 0)
        step = 2;
      else if (result == 1)
        step = 5;
      else if (result == 2)
        step = 6;
      else if (result == 3) {
        menu->pending = 2;
        step = 7;
      } else if (result == 4) {
        menu->pending = 2;
        step = 9;
      } else if (result == 5)
        step = 11;
      else if (result == 6)
        step = 12;
      else if (result == 8) {
        menu->pending = 2;
        step = 13;
      } else if (result == 9) {
        menu->pending = 2;
        step = 14;
      }
      break;
    case 7:
      icon = menu->arrows[0];
      icon->active = 13;
      icon = menu->arrows[1];
      icon->active = 13;
      result = Func_081039fc(1);
      if (result == -2)
        done = 1;
      step = 3;
      if (result < 0)
        break;
    case 13:
      Sound_PlayFar(126);
      result = Djinn_TransferFar(menu->shownMember[0], menu->pairElement[0],
                                 menu->pairIndex[0], menu->shownMember[1]);
      Unit_CalculateFar(menu->shownMember[0]);
      Unit_CalculateFar(menu->shownMember[1]);
      menu->cursor->active = 13;
      WindowSprite_ClearListFar(menu->listWindow);
      DjinnMenu_BuildLists(menu->backdrop);
      DJINN_RESELECT_PAIR_ENTRY(menu, listIndex);
      menu->cursor->active = 1;
      step = 0;
      break;
    case 6:
      icon = menu->arrows[0];
      icon->active = 13;
      icon = menu->arrows[1];
      icon->active = 13;
      result = Func_081039fc(2);
      if (result == -2)
        done = 1;
      step = 3;
      if (result < 0)
        break;
    case 12:
      Sound_PlayFar(175);
      Djinn_DeactivateFar(menu->shownMember[0], menu->pairElement[0],
                          menu->pairIndex[0]);
      result = DjinnRecovery_AddStandbyFar(
          menu->shownMember[0], menu->pairElement[0], menu->pairIndex[0]);
      Unit_CalculateFar(menu->shownMember[0]);
      menu->cursor->active = 13;
      WindowSprite_ClearListFar(menu->listWindow);
      menu->cursor->active = 1;
      step = 2;
      break;
    case 9:
      icon = menu->arrows[0];
      icon->active = 13;
      icon = menu->arrows[1];
      icon->active = 13;
      result = Func_081039fc(0);
      if (result == -2)
        done = 1;
      step = 3;
      if (result < 0)
        break;
    case 14:
      Sound_PlayFar(126);
      Djinn_TransferFar(menu->shownMember[0], menu->pairElement[0],
                        menu->pairIndex[0], menu->shownMember[1]);
      result = Djinn_TransferFar(menu->shownMember[1], menu->pairElement[1],
                                 menu->pairIndex[1], menu->shownMember[0]);
      Unit_CalculateFar(menu->shownMember[0]);
      Unit_CalculateFar(menu->shownMember[1]);
      DjinnMenu_BuildLists(menu->backdrop);
      DJINN_RESELECT_PAIR_ENTRY(menu, listIndex);
      menu->cursor->active = 1;
      step = 2;
      break;
    case 5:
      icon = menu->arrows[0];
      icon->active = 13;
      icon = menu->arrows[1];
      icon->active = 13;
      result = Func_081039fc(3);
      if (result == -2)
        done = 1;
      step = 3;
      if (result < 0)
        break;
    case 11:
      Sound_PlayFar(139);
      Djinn_ActivateFar(menu->shownMember[0], menu->pairElement[0],
                        menu->pairIndex[0]);
      result = DjinnRecovery_RemoveFar(
          menu->shownMember[0], menu->pairElement[0], menu->pairIndex[0]);
      Unit_CalculateFar(menu->shownMember[0]);
      menu->cursor->active = 13;
      WindowSprite_ClearListFar(menu->listWindow);
      menu->cursor->active = 1;
      step = 2;
      break;
    default:
      done = 1;
      break;
    }
  } while (done == 0);
  return menuResult;
}

#undef DJINN_RESELECT_PAIR_ENTRY

// Menu: make brightness-adjusted copies of BG palettes.

/// Adds d to the RGB components of the 16 colours of BG palettes 15, 5 and 7,
/// clamped to 0-31, and writes palettes 14, 4 and 6. Palette 7 uses -12
/// instead of d.
void Graphics_AdjustPalettes(s32 d) {
  s32 n;
  s32 paletteNumber;
  s32 col;
  s32 mask;
  s32 idx;

  paletteNumber = 15;
  n = 0;
  mask = 31;
  do {
    for (col = 0; col <= 15; col++) {
      u32 color;
      s32 b;
      s32 g;
      s32 r;
      s32 v;

      idx = paletteNumber * 16 + col;
      color = ((u16 *)0x05000000)[idx];
      b = (color >> 10) & mask;
      g = (color >> 5) & mask;
      r = color & mask;
      b += d;
      g += d;
      r += d;
      if (b > 31)
        b = 31;
      if (g > 31)
        g = 31;
      if (r > 31)
        r = 31;
      if (b < 0)
        b = 0;
      if (g < 0)
        g = 0;
      if (r < 0)
        r = 0;
      v = b << 10;
      v |= g << 5;
      v |= r;
      ((s16 *)0x04FFFFE0)[idx] =
          v; // 16 colors earlier (the preceding palette).
    }
    if (n == 0) {
      paletteNumber = 5;
    } else {
      paletteNumber = 7;
      d = -12;
    }
    n++;
  } while (n <= 2);
}

// Opens the Djinn menu's backdrop screen, the twin of The Broken Seal's
// Menu_OpenBackdropScreen: saves the tiles and palettes it covers, draws the
// backdrop, lists the owners (DjinnMenu_BuildLists), then makes the list's cursor and
// the two page arrows and places the arrows at the list window's corners.

extern char ResourceId_ShopGemTiles;

void Graphics_ExpandVramTilesByColorTableFar(void *tiles);
s32 Palette_GetBasicObjColors(void);

/// Opens the Djinn menu's backdrop screen, the twin of The Broken Seal's
/// Menu_OpenBackdropScreen: saves the tiles and palettes it covers, draws the
/// backdrop, lists the owners (DjinnMenu_BuildLists), then makes the list's cursor and
/// the two page arrows and places the arrows at the list window's corners.
void Func_08101638(void) {
  struct DjinnMenuWork *menu = Ram_HeapSlots->menuRuntime;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;
  struct MenuWindow *window;
  struct MenuIcon *icon;
  s32 entry;
  s32 x;
  s32 y;

  Window_UpdateOrCreate(&menu->listWindow, 0, 5, 30, 15, 2);
  wait(1);
  /* FAKEMATCH(wrapper): forwarding through item.c's CopyWords_ inline keeps
   * each call's own constants; as direct calls, 134 of 237 instructions
   * differ in tla-en. */
  CopyWords_(backdrop->tiles, BG_CHAR_BLOCK(1), 0x2000);
  CopyWords_(backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0), 128);
  /* FAKEMATCH(wrapper): forwarding through item.c's FillWords_ inline keeps
   * each call's own constants; as direct calls, 2 of 237 instructions differ
   * in tla-en. */
  FillWords_(BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
  FillWords_((void *)&BG_PLTT_COLOR(4, 0), 128, 0x55555555);
  Graphics_ExpandVramTilesByColorTableFar(BG_CHAR_BLOCK(1) + 0x1000);
  CopyWords_(BG_CHAR_BLOCK(1) + 150 * 32, gMenu_BackdropBorderTile, 32);
  Dma_Set(Palette_GetBasicObjColors(), (void *)&BG_PLTT_COLOR(5, 0),
          DMA_ENABLE << 16 | 16, REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  Dma_Set((void *)&BG_PLTT_COLOR(15, 0), (void *)&BG_PLTT_COLOR(7, 0),
          DMA_ENABLE << 16 | 16, REG_DMA3);
  Graphics_AdjustPalettes(8);
  BG_PLTT_COLOR(7, 4) = BG_PLTT_COLOR(15, 4);
  BG_PLTT_COLOR(6, 4) = BG_PLTT_COLOR(15, 4);
  DjinnMenu_BuildLists(backdrop);

  window = menu->statusWindow;
  WindowSprite_ClearListFar(window);
  icon = Icon_CreateWithResourceVariant(window, -8, 11);
  icon->active = 13;
  menu->cursor = icon;
  entry = Resource_FindFreeEntry();
  if (entry < 96) {
    VramBlock_LoadResourceFar(entry, 128, (s32)&ResourceId_ShopGemTiles);
    icon = WindowSprite_CreateFar(entry, 0x40000000, window, 0, 0);
    icon->active = 13;
    menu->arrows[0] = icon;
    icon->oam.affineIndex |= 8;
    icon = WindowSprite_CreateFar(entry, 0x40000000, window, 0, 0);
    icon->active = 13;
    menu->arrows[1] = icon;
  }

  window = menu->listWindow;
  icon = menu->arrows[0];
  x = window->x * 8;
  icon->oam.x = icon->x = x;
  y = window->y * 8 + 4;
  icon->oam.y = icon->y = y;
  icon = menu->arrows[1];
  x += 224;
  icon->oam.x = icon->x = x;
  icon->oam.y = icon->y = y;
}

#define ACTION_MASK 0x3fff
#define PREVIEW_GAINED 0x8000
#define PREVIEW_LOST 0x4000

/// Lists the actions after a preview, marking gains, then appends the losses.
/// The two output counts distinguish gained and lost actions.
///
/// Writes every action of after (up to 32), with PREVIEW_GAINED on those
/// before lacks, then each action of before that after lacks, with
/// PREVIEW_LOST. Returns the number written, up to 64.
s32 PsynergyList_Diff(struct OwnerActionSlot *before,
                      struct OwnerActionSlot *after, u16 *actionsOut,
                      s32 *gainedCountOut, s32 *lostCountOut) {
  s32 actionIndex;
  s32 otherIndex;
  s32 actionCount;
  s32 gainedCount;
  s32 lostCount;

  actionCount = 0;
  gainedCount = 0;
  lostCount = 0;
  for (actionIndex = 0;
       actionIndex < 32 && after[actionIndex].encodedAction != 0;
       actionIndex++) {
    actionsOut[actionCount] = after[actionIndex].encodedAction & ACTION_MASK;
    actionCount++;
    for (otherIndex = 0; otherIndex < 32; otherIndex++) {
      if (((after[actionIndex].encodedAction ^
            before[otherIndex].encodedAction) &
           ACTION_MASK) == 0)
        break;
    }
    if (otherIndex == 32) {
      gainedCount++;
      actionsOut[actionCount - 1] |= PREVIEW_GAINED;
    }
  }
  for (actionIndex = 0;
       actionIndex < 32 && before[actionIndex].encodedAction != 0;
       actionIndex++) {
    for (otherIndex = 0; otherIndex < 32; otherIndex++) {
      if (((before[actionIndex].encodedAction ^
            after[otherIndex].encodedAction) &
           ACTION_MASK) == 0)
        break;
    }
    if (otherIndex == 32) {
      lostCount++;
      actionsOut[actionCount] =
          (before[actionIndex].encodedAction & ACTION_MASK) | PREVIEW_LOST;
      actionCount++;
    }
  }
  *gainedCountOut = gainedCount;
  *lostCountOut = lostCount;
  return actionCount;
}

#undef PREVIEW_GAINED
#undef PREVIEW_LOST

struct OwnerEntry {
  u8 unk00[20];
};

struct OwnerList {
  struct OwnerEntry entries[8];
  u8 results[8];
};

s32 Djinn_ListOwnerEntries(u16 *entriesOut, s32 memberId, s32 element);

/// For each active party member of the menu work, fills a 20-byte entry of the
/// list with Djinn_ListOwnerEntries (owner, -1) and keeps its result byte at offset 160
/// of the list.
// With the owner read as the argument the two walking pointers swap r5 and
// r6 (differ 16).
void DjinnMenu_BuildLists(struct OwnerList *list) {
  struct InventoryMenuState *menu =
      (struct InventoryMenuState *)Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 0; i < menu->partyCount; i++) {
    // Read unsigned (ldrh): the party's ids, as this routine's bytes load them.
    s32 owner = (u16)menu->ownerIds[i];

    list->results[i] = Djinn_ListOwnerEntries(&list->entries[i], owner, -1);
  }
}

extern u8 MsgDjinnName[]; ///< the first djinni's name

s32 Djinn_IsReadyFar(s32 owner, s32 element, s32 index);
s32 Djinn_IsActiveFar(s32 owner, s32 element, s32 index);

/// The state of a packed djinni entry (owner in bits 8-11, element in 5-7,
/// index in 0-4, bit 15 set): 2 when the djinni is neither ready nor active,
/// otherwise whether bit 15 is set. DjinnMenu_DrawPage colours each name by it.
s32 Func_08101a04(u32 entry) {
  s32 owner = (entry & 0xf00) >> 8;
  s32 element = (entry & 0xe0) >> 5;
  s32 index = entry & 31;
  s32 state;

  if (Djinn_IsReadyFar(owner, element, index) ||
      Djinn_IsActiveFar(owner, element, index)) {
    u32 set = entry & 0x8000;

    state = ((0u - set) | set) >> 31;
  } else {
    state = 2;
  }
  return state;
}

/// Draws a packed djinni entry in a window: its name 8 pixels right of (x, y)
/// and its element's icon tile at the tile under (x, y), then resets the text
/// colour. The names run 20 to an element.
// As ☀️'s MsgDjinnName (games/tbs/src/menu/menu.c).
void Func_08101a54(s32 window, s32 x, s32 y, s32 entry) {
  s32 element = (entry & 0xe0) / 32;

  Text_DrawCharacterAtOffsetFar(
      (s32)MsgDjinnName + element * 20 + (entry & 31), window, x + 8, y);
  Window_SetTilemapEntryFar(window, element + 0x5001, x / 8, y / 8, 0);
  Text_SetColorFar(15);
}

struct DjinnMenuState {
  u8 unk000[0x34];
  s32 listWindow;
  u8 unk038[0xc0];
  void *owners[4];
};

void DjinnMenu_DrawPage(struct DjinnMenuBackdrop *backdrop, s32 page) {
  struct DjinnMenuState *menu =
      (struct DjinnMenuState *)Ram_HeapSlots->menuRuntime;
  u8 *tiles = Ram_HeapSlots->windowTiles;
  s32 window = menu->listWindow;
  s32 owners;
  s32 column;
  s32 element;
  s32 i;
  s32 x;
  s32 y;
  s32 djinn;

  tiles[6] = 1;
  WindowSprite_RedrawSavedRectFar(window);
  owners = Party_CountActiveOwnersFar();
  djinn = Party_SumDjinnCountsFar(-1);
  if (owners != 0 && (djinn - 1) / owners <= 6) {
    Text_DrawCharacterAtOffsetFar((s32)MsgDjinnUnleashEffectLabel, window, 0,
                                  80);
    Window_DrawDividerLineFar(window, 0, 10, 28, 10);
  }
  DjinnMenu_BuildLists(backdrop);
  for (column = 0, x = 8; column <= 3; column++, x += 56) {
    u16 *entries = backdrop->ids[page + column];

    y = 16;
    if (menu->owners[column] != NULL) {
      for (element = 0; element <= 3; element++) {
        for (i = 0; i < backdrop->counts[page + column]; i++) {
          if (element == (entries[i] & 0xe0) >> 5) {
            switch (Func_08101a04(entries[i])) {
            case 0:
              Text_SetColorFar(2);
              break;
            case 1:
              Text_SetColorFar(15);
              break;
            default:
              Text_SetColorFar(4);
              break;
            }
            Func_08101a54(window, x, y, entries[i]);
            y += 8;
          }
        }
      }
      // From patch 4 each column waits a frame after it is drawn.
#if TLA_PATCH_4
      wait(1);
#endif
    }
  }
  // The Japanese release reads the window tiles again for the first store
  // and does not wait; from patch 1 the page waits three frames.
#if TLA_PATCH_1
  tiles[3] = 1;
  tiles[6] = 0;
  wait(3);
#else
  Ram_HeapSlots->windowTiles[3] = 1;
  tiles[6] = 0;
#endif
}

// A grid cell's highlight is a tile narrower in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define GRID_CELL_WIDTH 5
#else
#define GRID_CELL_WIDTH 6
#endif

struct UiPanel {
  u8 unk00[0xc];
  s16 x;
  s16 y;
};

void DjinnMenu_DrawFirstPage(struct DjinnMenuBackdrop *backdrop) {
  DjinnMenu_DrawPage(backdrop, 0);
}

void UiPanel_SetRectHighlight(struct UiPanel *panel, s32 x, s32 y, s32 width,
                              s32 height, s32 style) {
  Window_HighlightRectFar(panel->x + x + 1, panel->y + y + 1, width, height,
                         style);
}

/// Highlights the cell of a four-column grid at index's column and row's line,
/// with style 14 when selected and 15 otherwise.
void UiPanel_HighlightGridCell(struct UiPanel *panel, s32 index, s32 row,
                               s32 selected) {
  s32 style = ((0u - selected) | selected) >> 31;

  style = 15 - style;
  UiPanel_SetRectHighlight(panel, index % 4 * 7 + 1, row + 2, GRID_CELL_WIDTH,
                           1, style);
}

#undef GRID_CELL_WIDTH
#undef GRID_CELL_WIDTH

struct WindowTiles {
  u8 unk00[3];
  u8 dirty; ///< bits: one per four tile rows, from bit 1
  u8 unk04[4];
  u16 cells[32][32];
};

/// Gives every cell of a tile rectangle that uses palette 15 the palette
/// given instead, clipped to the screen, and marks its rows for upload.
void Func_08101c7c(s32 x, s32 y, s32 width, s32 height, s32 palette) {
  struct WindowTiles *tiles = (struct WindowTiles *)Ram_HeapSlots->windowTiles;

  palette <<= 12;

  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }

  if (width > 0 && height > 0) {
    do {
      u16 *p = &tiles->cells[y][x];
      s32 col = width;

      if (col != 0) {
        do {
          u32 tile = *p;

          if (((tile >> 12) & 15) == 15) {
            tile &= ~0xf000;
            tile |= palette;
            *p = tile;
          }
          col--;
          p++;
        } while (col != 0);
      }
      /* FAKEMATCH(dead): the dirty bit's update in a once-only block;
       * as the plain statement, 4 of 90 instructions differ in tla-en. */
      do {
        tiles->dirty |= 2 << ((u32)y >> 2);
      } while (0);

      height--;
      y++;
    } while (height != 0);
  }
}

void UiPanel_SetRectPalette(struct UiPanel *panel, s32 x, s32 y, s32 width,
                            s32 height, s32 palette) {
  Func_08101c7c(panel->x + x + 1, panel->y + y + 1, width, height, palette);
}

struct UiChannelSlot *Func_080383d8(void *window, s32 message);

s32 Func_08101d5c(void) {
  struct UiPanel *helpWindow;
  struct UiPanel *statusWindow;
  struct DjinnMenuWork *menu;
  struct WindowWork *render;
  s32 result;
  s32 previous;
  s32 selection;
  struct UiPanel *list;
  struct UiChannelSlot *channel;
  s32 message;
  s32 helpIndex;
  s32 listMessage;

  menu = Ram_HeapSlots->menuRuntime;
  selection = 0;
  render = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  result = 0;
  previous = 0;
  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  WindowSprite_PrepareForRedrawFar(menu->helpWindow);
  message = (s32)MsgDjinnInfoPrompt;
  Text_DrawCharacterAtOffsetFar(message, menu->helpWindow, 0, 0);
  message++;
  Text_DrawCharacterAtOffsetFar(message, menu->helpWindow, 0, 16);
  Func_08101c7c(1, 1, 11, 3, 6);
  UiPanel_SetRectPalette(menu->listWindow, 0, 0, 28, 10, 6);
  list = Window_CreateFar(0, 9, 8, 10, 6);
  helpWindow = Window_CreateFar(8, 12, 22, 7, 2);
  statusWindow = Window_CreateFar(8, 9, 22, 3, 2);
  Func_08038290();
  helpIndex = 0;
  listMessage = (s32)MsgDjinnInfoTopic;
  do {
    /* FAKEMATCH(temporary): the topic's row held in y before the call; as
       the argument helpIndex * 8, 2 of 289 instructions differ in tla-en. */
    s32 y = helpIndex * 8;

    Text_DrawCharacterAtOffsetFar(helpIndex + listMessage, list, 0, y);
    helpIndex++;
  } while (helpIndex <= 6);
  do {
    WindowSprite_PrepareForRedrawFar(statusWindow);
    Text_DrawMessageFar(selection + (s32)MsgDjinnInfoTopic, statusWindow, 0, 0);
    channel = Func_080383d8(helpWindow, selection + (s32)MsgDjinnInfoText);
    UiPanel_SetRectHighlight(list, 0, previous, 6, 1, 15);
    UiPanel_SetRectHighlight(list, 0, selection, 6, 1, 14);
    previous = selection;
    for (;;) {
      Func_080f8a44(-12, (list->y + selection) * 8 + 8);
      wait(1);
      if (gInput.repeat & 0x90) {
        selection++;
        selection = Menu_GetModuloOfSum(selection, 7);
        Sound_PlayFar(111);
        break;
      }
      if (gInput.repeat & 0x60) {
        selection--;
        selection = Menu_GetModuloOfSum(selection, 7);
        Sound_PlayFar(111);
        break;
      } else if (gInput.pressed & 8) {
        Sound_PlayFar(113);
        result = -2;
        break;
      } else if (gInput.pressed & 6) {
        Sound_PlayFar(113);
        result = -1;
        break;
      } else if (gInput.pressed & 1) {
        if (Message_AllTypedFar()) {
          selection++;
          selection = Menu_GetModuloOfSum(selection, 7);
          Sound_PlayFar(112);
          break;
        }
        Sound_PlayFar(111);
      }
    }
    if (render->glyphResource != 99) {
      Resource_ResetEntry(render->glyphResource);
      render->glyphResource = 99;
    }
    ((struct WindowWork *)Ram_HeapSlots->windowTiles)->unk1388 = 0;
    WindowSprite_PrepareForRedrawFar(helpWindow);
    {
      struct Window *window = channel->work;

      window->frame = 0;
      window->duration = 0;
      window->state = 0;
    }
    channel->work = 0;
  } while (result == 0);
  ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 1;
  WindowSprite_ClearListFar(statusWindow);
  WindowSprite_ClearListFar(helpWindow);
  wait(1);
  Window_CloseFar(statusWindow, 1);
  Window_CloseFar(list, 1);
  Window_CloseFar(helpWindow, 1);
  Func_08038290();
  if (result == -2) {
    WindowSprite_PrepareForRedrawFar(menu->helpWindow);
    WindowSprite_PrepareForRedrawFar(menu->listWindow);
    WindowSprite_PrepareForRedrawFar(menu->statusWindow);
    ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 0;
  }
  Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_VBLANK);
  return result;
}

// A grid cell's highlight is a tile narrower in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define GRID_CELL_WIDTH 5
#else
#define GRID_CELL_WIDTH 6
#endif

// Spanish, French and Italian name the Djinni before its owner in one message
// on the trade's help line, so the Djinni's icon leads the line.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define TRADE_HELP_DJINNI_FIRST 1
#else
#define TRADE_HELP_DJINNI_FIRST 0
#endif

struct DjinnWindowWork {
  u8 unk0[3];
  u8 dirty;
};

extern u8 gDjinnMenu_TextLevel[];

s32 Djinn_MarkTradeOnlyRecipients(s8 *table, s32 self);

void Menu_SetPageArrowsB(s32 index, s32 count);
void MenuIcon_SetPicture(s32 index, s32 frame);
void MenuIcon_ReplaceObject(s32 index, s32 kind, s32 animation);

void DjinnRecovery_CountDownFar(void);
s32 Menu_GetModuloOfSum(s32 value, s32 count) __attribute__((const));

#define DJINN_OWNER(entry) (((entry) & 0xf00) >> 8)
#define DJINN_ELEMENT(entry) (((entry) & 0xe0) >> 5)
#define DJINN_NUMBER(entry) ((entry) & 31)

// Shows one tutorial message and waits for its text to finish.
#define DJINN_TUTORIAL_MESSAGE(window, offset)                                 \
  do {                                                                         \
    (window) =                                                                 \
        Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + (offset), 9, 9, 1); \
    gMainRoutineWork.eventMode = 1;                                            \
    while (Message_AllTypedFar() == 0)                                         \
      wait(1);                                                                 \
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
    Djinn_MarkTradeOnlyRecipients((s8 *)requiresTrade, menu->selectedColumn[0]);
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

        Menu_CreateOwnerEntryObjects(page);
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
            UiPanel_SetRectPalette(listWindow, (fromX - page) * 7 + 1, 2,
                                   GRID_CELL_WIDTH, height, 6);
            menu->rowY[0] = 54;
          } else {
            menu->rowY[0] = 200;
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
              Text_DrawCharacterAtOffsetFar(message, listWindow, x, 8);
            }
          }
        }
        Menu_SetPageArrowsB(memberColumn, menu->memberCount);
#if !TLA_PATCH_1
        wait(1);
#endif
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
          animation = 1;
          frame = 0x4000;
          break;
        case 1:
          animation = 2;
          frame = 0x4000;
          break;
        default:
          animation = 1;
          frame = 0x8000;
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
      WindowSprite_RedrawSavedRectFar(window);
      Text_DrawStringAtOffsetFar(unit->name, window, 0, 0);
      Text_DrawCharacterAtOffsetFar(unit->classIndex + (s32)MsgClassName,
                                    window, 0, 8);
      Text_DrawStringAtOffsetFar(gDjinnMenu_TextLevel, window, 48, 0);
      Text_DrawNumberInWindowFar(unit->level, 2, window, 72, 0);
      if (mode == 0)
        Text_DrawCharacterAtOffsetFar((s32)MsgDjinnCharacterStatus, window, 0,
                                      16);
      WindowSprite_RedrawSavedRectFar(helpWindow);
      if (mode == 0 && selectedRow != -1) {
        if (groupMode) {
          if (setAll == 0)
            Text_DrawCharacterAtOffsetFar((s32)MsgDjinnSetAll, helpWindow, 0,
                                          0);
          else
            Text_DrawCharacterAtOffsetFar((s32)MsgDjinnAllStandby, helpWindow,
                                          0, 0);
          if (state == 2)
            Text_DrawCharacterAtOffsetFar((s32)MsgDjinnRecovering, helpWindow,
                                          0, 16);
        } else {
          Text_DrawCharacterAtOffsetFar((s32)MsgDjinnChoose, helpWindow, 0, 0);
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
          Text_DrawCharacterAtOffsetFar(message, helpWindow, 0, 16);
        }
      }
#if TLA_PATCH_1
      wait(1);
#endif
      if (mode == 1) {
#if TRADE_HELP_DJINNI_FIRST
        Window_SetTilemapEntryFar(
            helpWindow, DJINN_ELEMENT(menu->pairEntries[0]) + 0x5001, 0, 0, 0);
        MessageArgument_PushFar(menu->shownMember[0], 1);
        MessageArgument_PushFar(DJINN_ELEMENT(menu->pairEntries[0]) * 20 +
                                    DJINN_NUMBER(menu->pairEntries[0]) + 300,
                                4);
        Text_DrawCharacterAtOffsetFar((s32)MsgDjinnOwnerLabel, helpWindow, 8,
                                      0);
#else
        MessageArgument_PushFar(menu->shownMember[0], 1);
        Text_DrawCharacterAtOffsetFar((s32)MsgDjinnOwnerLabel, helpWindow, 0,
                                      0);
        MessageArgument_PushFar(DJINN_ELEMENT(menu->pairEntries[0]) * 20 +
                                    DJINN_NUMBER(menu->pairEntries[0]) + 300,
                                4);
        Window_SetTilemapEntryFar(
            helpWindow, DJINN_ELEMENT(menu->pairEntries[0]) + 0x5001, 6, 0, 0);
        Text_DrawCharacterAtOffsetFar((s32)MsgDjinnOwnerLabel + 1, helpWindow,
                                      56, 0);
#endif
        Text_DrawCharacterAtOffsetFar((s32)MsgDjinnOwnerLabel + 2, helpWindow,
                                      0, 8);
      }
#if TLA_PATCH_1
      wait(1);
#endif
      WindowSprite_ClearListFar(listWindow);
      if (selectedRow != -1) {
#if TLA_PATCH_1
        Window_ClearInteriorTilesFar(listWindow, 0, 96, 224, 104);
#endif
        // Japanese draws the effect as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
        Text_DrawMessageFar(element * 20 + number + (s32)MsgDjinnEffect,
                            listWindow, 0, 96);
#else
        Text_DrawCharacterAtOffsetFar(
            element * 20 + number + (s32)MsgDjinnEffect, listWindow, 0, 96);
#endif
#if TLA_PATCH_4
      } else {
        Window_ClearInteriorTilesFar(listWindow, 0, 96, 224, 104);
#endif
      }
      ((struct DjinnWindowWork *)Ram_HeapSlots->windowTiles)->dirty = 1;
      redraw = 0;
    }
    wait(1);
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
          wait(1);
        }
        pressed = 2;
        repeat = 2;
        break;
      case 27:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow, 12);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          wait(1);
          DJINN_TUTORIAL_MESSAGE(messageWindow, 13);
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          backdrop->tutorialTimer = 0;
          DjinnRecovery_CountDownFar();
          DjinnRecovery_CountDownFar();
          DjinnRecovery_CountDownFar();
          Djinn_DeactivateFar(4, 0, 7);
          DjinnRecovery_AddStandbyFar(4, 0, 7);
          Unit_CalculateFar(4);
          repeat = 2;
          goto pressB;
        }
        break;
      case 1:
        if (backdrop->tutorialTimer == 60) {
          DJINN_TUTORIAL_MESSAGE(messageWindow, 0);
          Window_CloseFar(messageWindow, 1);
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
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 8;
        }
        break;
      case 13:
        if (backdrop->tutorialTimer == 40) {
          repeat = 2;
          pressed = 2;
          backdrop->tutorialTimer = 0;
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
          gMainRoutineWork.eventMode = 1;
          messageWindow =
              Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + 4, 9, 9, 1);
          Func_080f8ab4(2, 146);
          while (Message_AllTypedFar() == 0)
            wait(1);
          while (!(gInput.pressed & 1)) {
            Func_080f8a44(2, 146);
            wait(1);
          }
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          wait(1);
          messageWindow =
              Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + 5, 9, 9, 1);
          while (Message_AllTypedFar() == 0)
            wait(1);
          while (!(gInput.pressed & 1)) {
            Func_080f8a44(2, 146);
            wait(1);
          }
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          wait(1);
          messageWindow =
              Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + 6, 9, 9, 1);
          while (Message_AllTypedFar() == 0)
            wait(1);
          while (!(gInput.pressed & 1)) {
            Func_080f8a44(2, 146);
            wait(1);
          }
          Window_CloseFar(messageWindow, 1);
          DjinnMenu_DrawFirstPage(backdrop);
          wait(1);
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
          repeat = 32;
          pressed = 32;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 22;
        }
        break;
      case 24:
        if (backdrop->tutorialTimer == 60) {
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 25;
          repeat = 2;
        pressB:
          pressed = 2;
        }
        break;
      }
    }
    // R on a Djinni: put it on standby or set it at once.
    if (mode == 0 && (pressed & 0x100)) {
      if (selectedRow == -1) {
        Sound_PlayFar(114);
        continue;
      }
      groupMode = 1;
      gInput.pendingPressed = 0;
      switch (state) {
      case 0:
        Sound_PlayFar(139);
        Djinn_ActivateFar(owner, element, number);
        DjinnRecovery_RemoveFar(owner, element, number);
        break;
      case 1:
        Sound_PlayFar(175);
        Djinn_DeactivateFar(owner, element, number);
        DjinnRecovery_AddStandbyFar(owner, element, number);
        break;
      default:
        Sound_PlayFar(114);
        redraw = 1;
        continue;
      }
      Unit_CalculateFar(owner);
      DjinnMenu_DrawPage(backdrop, memberColumn / 4 * 4);
      redraw = 1;
    }
    if (mode == 0 && (pressed & 0x200)) {
      result = 7;
      Sound_PlayFar(112);
      break;
    }
    if ((pressed & 1) || (mode == 1 && (pressed & 0x100))) {
      if (selectedRow != -1 && state == 2) {
        Sound_PlayFar(114);
        WindowSprite_ClearListFar(listWindow);
#if TLA_PATCH_1
        Window_ClearInteriorTilesFar(listWindow, 0, 96, 224, 104);
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
        Text_DrawMessageFar((s32)MsgDjinnNowRecovering, listWindow, 0, 96);
#else
        Text_DrawCharacterAtOffsetFar((s32)MsgDjinnNowRecovering, listWindow, 0,
                                      96);
#endif
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
      Sound_PlayFar(112);
      break;
    }
    if (pressed & 8) {
      Sound_PlayFar(113);
      result = -2;
      break;
    }
    if (pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    // Select: with R held, set every Djinni not recovering or, the next
    // time, put every one on standby; otherwise open the help.
    if (mode == 0 && (pressed & 4)) {
      if (groupMode) {
        setAll ^= 1;
        if (setAll)
          Sound_PlayFar(139);
        else
          Sound_PlayFar(175);
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
        result = 10;
        Sound_PlayFar(112);
        break;
      }
    }
    if (repeat & 0x40) {
      if (columnFlags[memberColumn] & 4)
        continue;
      if (backdrop->counts[memberColumn] == 0)
        continue;
      UiPanel_HighlightGridCell(listWindow, memberColumn, djinnRow, 0);
      Sound_PlayFar(111);
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
      Sound_PlayFar(111);
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

      Sound_PlayFar(111);
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
      result = backdrop->counts[memberColumn];
      if (result == 0)
        result = 1;
      djinnRow = Menu_GetModuloOfSum(djinnRow, result);
      redraw = 2;
      if (oldColumn / 4 * 4 != memberColumn / 4 * 4)
        redraw = 6;
    }
  }
  menu->selectedColumn[mode] = memberColumn;
  if (selectedRow != -1) {
    u32 id;
    u32 entry;

    menu->pairEntries[mode] = id = backdrop->ids[memberColumn][selectedRow];
    entry = (u16)id;
    menu->pairIndex[mode] = entry % 32;
    menu->pairElement[mode] = DJINN_ELEMENT(entry);
    menu->pairMember[mode] = DJINN_OWNER(entry);
  }
  menu->selection[mode] = memberColumn + djinnRow * 10;
  MenuIcon_SetPicture(mode, 0x4000);
  return result;
}

#undef DJINN_TUTORIAL_MESSAGE

#define DJINN_ENTRY_SET (-0x8000)

s32 Djinn_ListOwnerEntries(u16 *entriesOut, s32 memberId, s32 element) {
  struct DjinnUnit *unit = Unit_Get(memberId);
  s32 count = 0;
  s32 elementIndex;
  s32 djinnIndex;
  s32 encodedDjinn;

  if (element == -1) {
    for (elementIndex = 0; elementIndex < 4; elementIndex++) {
      for (djinnIndex = 0; djinnIndex < 20; djinnIndex++) {
        if (unit->djinnSet[elementIndex] & (1 << djinnIndex)) {
          encodedDjinn = (elementIndex << 5) | djinnIndex | DJINN_ENTRY_SET;
          encodedDjinn |= memberId << 8;
          entriesOut[count++] = encodedDjinn;
        } else if (unit->djinnHeld[elementIndex] & (1 << djinnIndex)) {
          entriesOut[count++] =
              (elementIndex << 5) | djinnIndex | (memberId << 8);
        }
      }
    }
  } else {
    for (djinnIndex = 0; djinnIndex < 20; djinnIndex++) {
      if (unit->djinnSet[element] & (1 << djinnIndex))
        entriesOut[count++] = (element << 5) | djinnIndex | DJINN_ENTRY_SET;
      else if (unit->djinnHeld[element] & (1 << djinnIndex))
        entriesOut[count++] = (element << 5) | djinnIndex;
    }
  }
  return count;
}

#undef DJINN_ENTRY_SET

s32 Func_08103218(s32 window, s32 x, s32 y, s32 ownerId, s32 removePairSlot, s32 addPairSlot, s32 previewMode, s32 page, s32 side);

#define ALT_PARAM 4

/// Draws both sides of a Djinn preview for page pageIndex: the second member
/// in the second window and the first in the first. Mode 0 draws the pair
/// members, both with preview mode 2; any other mode the shown members, with
/// modes 4 and 1. Returns 1.
s32 DjinnMenu_DrawPairPreview(s32 mode, s32 pageIndex) {
  s32 sp14;
  u8 first;
  u8 owner;
  struct DjinnMenuWork *menu;

  menu = Ram_HeapSlots->menuRuntime;
  if (mode == 0) {
    sp14 = mode;
    first = menu->pairMember[1];
    Func_08103218((s32)menu->secondWindow, 0, 0, first, 1, mode, 2, pageIndex,
                  1);
    Func_08103218(menu->window, 0, 0, menu->pairMember[0], mode, 1, 2,
                  pageIndex, mode);
  } else {
    owner = menu->shownMember[1];
    Func_08103218((s32)menu->secondWindow, 0, 0, owner, 1, 0, ALT_PARAM,
                  pageIndex, 1);
    Func_08103218(menu->window, 0, 0, menu->shownMember[0], 0, 0, 1, pageIndex,
                  0);
  }
  return 1;
}

#undef ALT_PARAM

// Previews a djinni change on a member's status, the twin of The Broken
// Seal's DjinnMenu_DrawStatPreview: the unit is saved, the change applied and
// the unit recalculated; page 0 draws the stats with an arrow beside each one
// that changes, later pages the Psynergy gained and lost; then the saved unit
// is copied back.

// The stat preview's columns, in pixels from x * 8: the Japanese edition draws
// Attack, Defense and Agility with four digits from HP's column and Luck a tile
// further left, sets every arrow in one column, and puts the Psynergy page's PP
// heading further left.
//
// The Japanese text leaves the preview's messages unnamed, so each is written
// by its place after a named one: "HP", the first of the six stat labels, 13
// after "Luck rises"; "* Psynergy Gained" and "* Psynergy Lost", the two after
// the status help; "* No change", the one after "Trade".
#if LANGUAGE != LANGUAGE_JAPANESE
#define STAT_DIGITS 3
#define STAT_X 48
#define LUCK_X 56
#define POOL_ARROW_X 62
#define STAT_ARROW_X 70
#define PP_HEADING_X 88
#define MSG_STAT_LABELS ((s32)MsgKey0d0e)
#define MSG_PSYNERGY_GAINED ((s32)MsgKey10d3)
#define MSG_PSYNERGY_LOST ((s32)MsgKey10d4)
#define MSG_NO_CHANGE ((s32)MsgKey10d9)
#else
#define STAT_DIGITS 4
#define STAT_X 32
#define LUCK_X 48
#define POOL_ARROW_X 64
#define STAT_ARROW_X 64
#define PP_HEADING_X 56
#define MSG_STAT_LABELS ((s32)MsgLuckRises + 13)
#define MSG_PSYNERGY_GAINED ((s32)MsgDjinnStatusHelp + 1)
#define MSG_PSYNERGY_LOST ((s32)MsgDjinnStatusHelp + 2)
#define MSG_NO_CHANGE ((s32)MsgDjinnCompareTrade + 1)
#endif

extern const char gDjinnMenu_TextSlash[];

#if LANGUAGE != LANGUAGE_JAPANESE

#else

#endif

s32 Icon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resourceId);
s32 Icon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant);

/// Previews a djinni change on a member's status, the twin of The Broken Seal's
/// DjinnMenu_DrawStatPreview: the unit is saved, the change applied and the
/// unit recalculated; page 0 draws the stats with an arrow beside each one that
/// changes, later pages the Psynergy gained and lost; then the saved unit is
/// copied back.
s32 Func_08103218(s32 window, s32 x, s32 y, s32 ownerId, s32 removePairSlot,
                  s32 addPairSlot, s32 previewMode, s32 page, s32 side) {
  // One constant for its whole life: the djinn tile loop starts from it.
  s32 firstElement = 0;
  // One constant for its whole life: the closing copy's size.
  s32 size;
  struct BattleUnit *unit;
  struct BattleUnit *originalUnit;
  s32 giveElement;
  s32 giveIndex;
  s32 giveSet;
  s32 takeElement;
  s32 takeIndex;
  s32 takeSet;
  s32 lost;
  s32 gained;
  u16 actions[48];
  struct DjinnMenuWork *menu;

  unit = Unit_Get(ownerId);
  menu = Ram_HeapSlots->menuRuntime;
  giveElement = menu->pairElement[removePairSlot];
  giveIndex = menu->pairIndex[removePairSlot];
  giveSet = menu->pairEntries[removePairSlot] & 0x8000;
  takeElement = menu->pairElement[addPairSlot];
  takeIndex = menu->pairIndex[addPairSlot];
  takeSet = menu->pairEntries[addPairSlot] & 0x8000;
  originalUnit = Heap_Alloc(sizeof(struct BattleUnit));
  Iwram_CopyWords(originalUnit, unit, sizeof(struct BattleUnit));

  if (page == 0) {
    if (previewMode == 3) {
      Text_DrawNumberInWindowFar(unit->maxHp, 4, window, x * 8 + 72,
                                 y * 8 + 56);
      Text_DrawNumberInWindowFar(unit->maxPp, 4, window, x * 8 + 72,
                                 y * 8 + 64);
      Text_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 32, y * 8 + 56);
      Text_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 32, y * 8 + 64);
      Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextSlash, window, x * 8 + 64,
                                 y * 8 + 56);
      Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextSlash, window, x * 8 + 64,
                                 y * 8 + 64);
    } else {
      Text_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 32, y * 8 + 56);
      Text_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 32, y * 8 + 64);
    }
    Text_DrawNumberInWindowFar(unit->atk, STAT_DIGITS, window, x * 8 + STAT_X,
                               y * 8 + 72);
    Text_DrawNumberInWindowFar(unit->def, STAT_DIGITS, window, x * 8 + STAT_X,
                               y * 8 + 80);
    Text_DrawNumberInWindowFar(unit->agi, STAT_DIGITS, window, x * 8 + STAT_X,
                               y * 8 + 88);
    Text_DrawNumberInWindowFar(unit->luck, 2, window, x * 8 + LUCK_X,
                               y * 8 + 96);
  }

  switch (previewMode) {
  case 0:
    Djinn_AddToOwnerFar(ownerId, takeElement, takeIndex & 31);
    Djinn_ActivateFar(ownerId, takeElement, takeIndex & 31);
    break;
  case 1:
    giveIndex &= 31;
    Djinn_DeactivateFar(ownerId, giveElement, giveIndex);
    break;
  case 2:
    if (giveSet) {
      giveIndex &= 31;
      Djinn_DeactivateFar(ownerId, giveElement, giveIndex);
    }
    Djinn_AddToOwnerFar(ownerId, takeElement, takeIndex & 31);
    if (takeSet)
      Djinn_ActivateFar(ownerId, takeElement, takeIndex & 31);
    break;
  case 4:
    Djinn_AddToOwnerFar(ownerId, takeElement, takeIndex & 31);
    if (takeSet)
      Djinn_ActivateFar(ownerId, takeElement, takeIndex & 31);
    break;
  }

  Unit_CalculateFar(ownerId);
  unit = Unit_Get(ownerId);

  if (page == 0) {
    s32 label;

    Text_DrawStringAtOffsetFar((s32)unit->name, window, x * 8 + 40, y * 8);
    Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextLevel, window, x * 8 + 40,
                               y * 8 + 16);
    Text_DrawNumberInWindowFar(unit->level, 2, window, x * 8 + 88, y * 8 + 16);
    label = MSG_STAT_LABELS;
    Text_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
    Text_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
    Text_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
    Text_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
    Text_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
    Text_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
    Text_DrawCharacterAtOffsetFar((s32)MsgClassName + originalUnit->classIndex,
                                  window, x * 8, y * 8 + 32);
  }

  {
    s8 i = 0;

    if (page == 0) {
      s32 column;

      if (originalUnit->classIndex != unit->classIndex) {
        Text_DrawCharacterAtOffsetFar((s32)MsgClassName + unit->classIndex,
                                      window, x * 8, y * 8 + 48);
        Window_SetTilemapEntryFar(window, 0xf296, x + 2, 5, 0);
      }
      column = x;
      if (originalUnit->classIndex != unit->classIndex)
        column += 5;
      for (i = firstElement; i < 4; i++) {
        s32 tileColumn;
        s32 tile = 0x5001 + i;

        tileColumn = column + i * 2;
        Window_SetTilemapEntryFar(window, tile, tileColumn, y + 5, 0);
        tileColumn++;
        Window_SetTilemapEntryFar(window, unit->djinnActiveCounts[i] + 0xf030,
                                  tileColumn, y + 5, 0);
      }
      // An empty once-only block: it ends the tile loop's block before the
      // stats.
      do {
      } while (0);
      if (unit->hp != originalUnit->hp) {
        Text_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 72, y * 8 + 56);
        if (unit->hp > originalUnit->hp)
          Icon_DrawVariantWithTileOffset(window, x * 8 + POOL_ARROW_X,
                                         y * 8 + 56, 0);
        else
          Icon_DrawVariantWithTileOffset(window, x * 8 + POOL_ARROW_X,
                                         y * 8 + 56, 1);
      }
      if (unit->pp != originalUnit->pp) {
        Text_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 72, y * 8 + 64);
        if (unit->pp > originalUnit->pp)
          Icon_DrawVariantWithTileOffset(window, x * 8 + POOL_ARROW_X,
                                         y * 8 + 64, 0);
        else
          Icon_DrawVariantWithTileOffset(window, x * 8 + POOL_ARROW_X,
                                         y * 8 + 64, 1);
      }
      if (unit->atk != originalUnit->atk) {
        Text_DrawNumberInWindowFar(unit->atk, 4, window, x * 8 + 72,
                                   y * 8 + 72);
        if (unit->atk > originalUnit->atk)
          Icon_DrawVariantWithTileOffset(window, x * 8 + STAT_ARROW_X,
                                         y * 8 + 72, 0);
        else
          Icon_DrawVariantWithTileOffset(window, x * 8 + STAT_ARROW_X,
                                         y * 8 + 72, 1);
      }
      if (unit->def != originalUnit->def) {
        Text_DrawNumberInWindowFar(unit->def, 4, window, x * 8 + 72,
                                   y * 8 + 80);
        if (unit->def > originalUnit->def)
          Icon_DrawVariantWithTileOffset(window, x * 8 + STAT_ARROW_X,
                                         y * 8 + 80, 0);
        else
          Icon_DrawVariantWithTileOffset(window, x * 8 + STAT_ARROW_X,
                                         y * 8 + 80, 1);
      }
      if (unit->agi != originalUnit->agi) {
        Text_DrawNumberInWindowFar(unit->agi, 4, window, x * 8 + 72,
                                   y * 8 + 88);
        if (unit->agi > originalUnit->agi)
          Icon_DrawVariantWithTileOffset(window, x * 8 + STAT_ARROW_X,
                                         y * 8 + 88, 0);
        else
          Icon_DrawVariantWithTileOffset(window, x * 8 + STAT_ARROW_X,
                                         y * 8 + 88, 1);
      }
      if (unit->luck != originalUnit->luck) {
        Text_DrawNumberInWindowFar(unit->luck, 2, window, x * 8 + 88,
                                   y * 8 + 96);
        if (unit->luck > originalUnit->luck)
          Icon_DrawVariantWithTileOffset(window, x * 8 + STAT_ARROW_X,
                                         y * 8 + 96, 0);
        else
          Icon_DrawVariantWithTileOffset(window, x * 8 + STAT_ARROW_X,
                                         y * 8 + 96, 1);
      }
    }
  }

  if (page > 0) {
    s32 rows;
    s32 first;
    s8 count;
    s8 row;
    s8 line;

    rows = 6 - (previewMode != 3);
    first = rows * (page - 1);
    count = PsynergyList_Diff(originalUnit->actionSlots, unit->actionSlots,
                              actions, &gained, &lost);
    line = 0;
    for (row = 0; first < count && row < rows; row++, first++) {
      Icon_CreateWithLoadedResource(window, x * 8, (y + line * 2) * 8 + 4,
                                    actions[first] & 0x3fff);
      if (actions[first] & 0x8000)
        Text_SetColorFar(4);
      else if (actions[first] & 0x4000)
        Text_SetColorFar(2);
      else
        Text_SetColorFar(15);
      Text_DrawCharacterAtOffsetFar((s32)MsgAbilityNothing +
                                        (actions[first] & 0x3fff),
                                    window, x * 8 + 16, (y + line * 2) * 8 + 8);
      Text_DrawNumberAtOffsetFar(BattleAction_Get(actions[first])->ppCost, 2,
                                 window, x * 8 + 88, (y + line * 2) * 8 + 8);
      line++;
    }
    Text_SetColorFar(15);
    Text_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window,
                                  x * 8 + PP_HEADING_X, y * 8);
    if (previewMode != 3) {
      s32 lines;

      lines = 0;
      if (gained) {
        Text_SetColorFar(4);
        Text_DrawCharacterAtOffsetFar(MSG_PSYNERGY_GAINED, window, x * 8,
                                      y * 8 + 88);
        lines = 1;
      }
      if (lost) {
        Text_SetColorFar(2);
        Text_DrawCharacterAtOffsetFar(MSG_PSYNERGY_LOST, window, x * 8,
                                      (y + lines) * 8 + 88);
        lines++;
      }
      if (lines == 0)
        Text_DrawCharacterAtOffsetFar(MSG_NO_CHANGE, window, x * 8, y * 8 + 88);
      Text_SetColorFar(15);
      Window_DrawDividerLineFar(window, 0, 11, 13, 11);
    }
    ((struct WindowWork *)Ram_HeapSlots->windowTiles)->dirty = 1;
  }

  size = sizeof(struct BattleUnit);
  if (page == 0)
    SideObject_CreateFar(ownerId, 0, side, window, 0, 0);
  Iwram_CopyWords(unit, originalUnit, size);
  Heap_Release(originalUnit);
  return 1;
}

#undef MSG_NO_CHANGE
#undef MSG_NO_CHANGE

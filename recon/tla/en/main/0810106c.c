// Near miss, differ 120 (2026-10-26, statements reordered).
// First draft of Func_0810106c: the Djinn menu's screen loop, the twin of
// The Broken Seal's DjinnMenu_RunScreen (games/tbs/src/menu/menu/djinn.c).
// ⚓️ drops the pair steps 4 and 8 (they close the screen), hides the page
// arrows before each confirmation and adds Func_08104540 (step 10) and
// Func_08101d5c (step 15) as its status and help pages.
// Differ 137 of 603 (2026-10-19, three instructions short): the native
// builds the arrows' offsets afresh (movs 190; lsls) and loads each arrow into
// the offset's register, where agscc derives the second offset from a
// register it kept; the setup's constants are also in another order.
#include "types.h"
#include "ram_buffer.h"

struct MenuIcon {
  u8 unk00[5];
  u8 active;
  u8 unk06[6];
  u16 animStep;
};

struct DjinnElementList {
  u16 ids[8][10];
  s8 counts[8];
};

struct DjinnMenuWork {
  u8 unk000[8];
  s32 owner;
  u8 unk00c[8];
  struct MenuIcon *cursor;
  u8 unk018[4];
  s8 selectedColumn[2];
  u8 unk01e[0x16];
  s32 listWindow;
  u8 unk038[0xf0];
  s16 ownerY[8];
  u8 unk138[0x30];
  u16 selection[2];
  u8 pairEntries[2];
  u8 unk16e[0x0a];
  struct MenuIcon *arrows[2];
  struct DjinnElementList *backdrop;
  u8 unk184[0x80];
  u16 owners[8];
  u8 unk214;
  u8 ownerCount;
  u8 shownOwner[2];
  u8 pairIndex[2];
  u8 pairElement[2];
  u16 pending;
};

void Func_08101638(void);
void wait(s32 frames);
void Menu_SetFirstObjectRowCoordinates(s32 offset);
void MenuIcon_SetPosition(s32 slot, s32 x, s32 y);
s32 Func_08102008(s32 mode);
s32 Func_08104540(void);
s32 Func_08101d5c(void);
s32 Func_081039fc(s32 mode);
void Sound_PlayFar(s32 cue);
s32 Djinn_TransferFar(s32 owner, s32 element, s32 index, s32 recipient);
void Djinn_ActivateFar(s32 owner, s32 element, s32 index);
void Djinn_DeactivateFar(s32 owner, s32 element, s32 index);
s32 DjinnRecovery_AddStandbyFar(s32 owner, s32 element, s32 index);
s32 DjinnRecovery_RemoveFar(s32 owner, s32 element, s32 index);
void Unit_CalculateFar(s32 unitId);
void WindowSprite_ClearListFar(s32 window);
void Func_081019a4(struct DjinnElementList *list);

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

s32 Func_0810106c(void) {
  struct DjinnMenuWork *menu;
  struct MenuIcon *cursor;
  s32 result;
  s32 step;
  s32 menuResult;
  s32 done;
  s32 listIndex;

  result = 0;
  done = 0;
  menuResult = 0;
  menu = Ram_HeapSlots->menuRuntime;
  cursor = menu->cursor;
  cursor->active = 13;
  cursor->animStep = 0;
  Func_08101638();
  wait(1);
  step = 2;
  do {
    switch (step) {
    case 0:
      if (result < 0) {
        done = 1;
        menuResult = -1;
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
      menu->shownOwner[0] = menu->owners[menu->selectedColumn[0]];
      menu->arrows[0]->active = 13;
      menu->arrows[1]->active = 13;
      result = Func_08104540();
      if (result == -2)
        done = 1;
      step = 2;
      break;
    case 15:
      menu->arrows[0]->active = 13;
      menu->arrows[1]->active = 13;
      result = Func_08101d5c();
      if (result == -2)
        done = 1;
      step = 2;
      break;
    case 3:
      Menu_SetFirstObjectRowCoordinates(-8);
      menu->owner = menu->owners[menu->selectedColumn[0]];
      menu->shownOwner[0] = menu->owners[menu->selectedColumn[0]];
      MenuIcon_SetPosition(0, menu->selectedColumn[0] * 56 + 48, 54);
      result = Func_08102008(1);
      for (listIndex = 0; listIndex < menu->ownerCount; listIndex++)
        menu->ownerY[listIndex] += 8;
      if (result == -2)
        done = 1;
      if (result < 0) {
        step = 2;
        break;
      }
      if (result == 3 || result == 4 || result == 8 || result == 9)
        menu->shownOwner[1] = menu->owners[menu->selectedColumn[1]];
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
      menu->arrows[0]->active = 13;
      menu->arrows[1]->active = 13;
      result = Func_081039fc(1);
      if (result == -2)
        done = 1;
      step = 3;
      if (result < 0)
        break;
    case 13:
      Sound_PlayFar(126);
      result = Djinn_TransferFar(menu->shownOwner[0], menu->pairElement[0],
                                 menu->pairIndex[0], menu->shownOwner[1]);
      Unit_CalculateFar(menu->shownOwner[0]);
      Unit_CalculateFar(menu->shownOwner[1]);
      menu->cursor->active = 13;
      WindowSprite_ClearListFar(menu->listWindow);
      Func_081019a4(menu->backdrop);
      DJINN_RESELECT_PAIR_ENTRY(menu, listIndex);
      menu->cursor->active = 1;
      step = 0;
      break;
    case 6:
      menu->arrows[0]->active = 13;
      menu->arrows[1]->active = 13;
      result = Func_081039fc(2);
      if (result == -2)
        done = 1;
      step = 3;
      if (result < 0)
        break;
    case 12:
      Sound_PlayFar(175);
      Djinn_DeactivateFar(menu->shownOwner[0], menu->pairElement[0],
                          menu->pairIndex[0]);
      result = DjinnRecovery_AddStandbyFar(
          menu->shownOwner[0], menu->pairElement[0], menu->pairIndex[0]);
      Unit_CalculateFar(menu->shownOwner[0]);
      menu->cursor->active = 13;
      WindowSprite_ClearListFar(menu->listWindow);
      menu->cursor->active = 1;
      step = 2;
      break;
    case 9:
      menu->arrows[0]->active = 13;
      menu->arrows[1]->active = 13;
      result = Func_081039fc(0);
      if (result == -2)
        done = 1;
      step = 3;
      if (result < 0)
        break;
    case 14:
      Sound_PlayFar(126);
      Djinn_TransferFar(menu->shownOwner[0], menu->pairElement[0],
                        menu->pairIndex[0], menu->shownOwner[1]);
      result = Djinn_TransferFar(menu->shownOwner[1], menu->pairElement[1],
                                 menu->pairIndex[1], menu->shownOwner[0]);
      Unit_CalculateFar(menu->shownOwner[0]);
      Unit_CalculateFar(menu->shownOwner[1]);
      Func_081019a4(menu->backdrop);
      DJINN_RESELECT_PAIR_ENTRY(menu, listIndex);
      menu->cursor->active = 1;
      step = 2;
      break;
    case 5:
      menu->arrows[0]->active = 13;
      menu->arrows[1]->active = 13;
      result = Func_081039fc(3);
      if (result == -2)
        done = 1;
      step = 3;
      if (result < 0)
        break;
    case 11:
      Sound_PlayFar(139);
      Djinn_ActivateFar(menu->shownOwner[0], menu->pairElement[0],
                        menu->pairIndex[0]);
      result = DjinnRecovery_RemoveFar(
          menu->shownOwner[0], menu->pairElement[0], menu->pairIndex[0]);
      Unit_CalculateFar(menu->shownOwner[0]);
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

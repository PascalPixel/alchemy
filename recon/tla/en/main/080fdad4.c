// First draft of Func_080fdad4: the Psynergy list of the member in
// paneIndex, the twin of The Broken Seal's PsynergyMenu_RunList
// (games/tbs/src/menu/menu/psynergy.c). ⚓️ sets the field shortcuts as the
// party's packed events (Party_SetPackedEvent), moves the member row through
// Func_08104ef8 and Func_08104d5c, and tests Select as held. MsgShortcutHelp
// and MsgChangeCharacterHelp are messages 4112 and 4282 in English; they need
// their names in the six catalogs before this links. Differ 228 of 762
// (2026-10-19, four instructions long): the native shares one call of
// Func_080fd6f0 between the two list modes, where the tails here differ by
// the order of the state pointer's reload; the counters' registers and some
// offsets' moves differ after it. The locals are declared in the order that
// gives the native stack slots (otherwise differ 268).
#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "battle_unit.h"

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((volatile struct InputState *)&gInput)->pressed)

struct MenuResult {
  s32 ownerState;
  s32 unk04;
  s32 page;
  s32 pageCount;
  s32 row;
  s32 entryCount;
  s32 selectedIndex;
};

struct MenuIcon {
  u8 unk00[5];
  u8 active;
  u8 unk06[6];
  u16 animStep;
  u8 unk0e;
  u8 sentinel;
};

struct PsynergyMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  s32 rowWindow;
  struct MenuIcon *paneIcon[2];
  s8 tabIndex[2];
  u8 unk01e[0x0a];
  s32 statusWindow;
  u8 unk02c[0x0c];
  s32 listWindow;
  u8 unk03c[0x0c];
  struct MenuIcon *entryGridCursor;
  struct MenuIcon *entryIcons[32];
  u8 unk0cc[0x2c];
  void *tabObjects[8];
  u8 unk118[0x50];
  u16 paneRow[2];
  u16 paneAction[2];
  u8 unk170[8];
  struct MenuIcon *arrows[2];
  u8 unk180[0x44];
  u16 psynergies[32];
  u16 ownerTable[8];
  u8 psynergyCount;
  u8 ownerCount;
  u8 ownerIds[2];
  struct MenuIcon *cursorIcon;
  u16 flags;
  u8 unk21e[0x46];
  s8 selectedIndexByOwner[8];
  u8 mode;
};

extern u8 MsgShortcutHelp[], MsgChangeCharacterHelp[];

s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height,
                            s32 frame);
s32 GameFlag_Test(s32 flag);
struct BattleUnit *Unit_Get(s32 owner);
s32 Func_080fd6f0(struct BattleUnit *unit, u16 *actions, s32 mode);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
s32 Func_080fd80c(struct MenuResult *result, s32 index);
void Func_080f8a44(s32 x, s32 y);
void UiIcon_PrepareObject(struct MenuIcon *icon);
void WaitFrames(s32 frames);
s32 Func_080fd968(s32 window, s32 unused, struct MenuResult *state);
s32 Func_080fd8a0(s32 window, s32 *work, struct MenuResult *state);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);
s32 Func_080f8f9c(s32 mode, s32 count, s32 rows, s32 *row, s32 *page);
struct BattleAct *BattleActGet(s32 action);
void Audio_PlayCue(s32 cue);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width,
                                    s32 height);
void UiText_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
s32 PsynergyMenu_IsActionRestricted(s32 actionEntry);
void Func_08104ef8(s32 first);
void Func_08104d5c(s32 window, s32 selection, s32 count);
void Func_080f8170(s32 window, s32 ownerId, s32 x, s32 y);
void PsynergyMenu_CallIconRoutineWithValue(struct PsynergyMenuState *menu,
                                           s32 ownerId);
s32 Party_SetPackedEvent(s32 kind, s32 id, s32 second);

s32 Func_080fdad4(s32 paneIndex) {
  struct PsynergyMenuState *menu;
  struct BattleAct *ability;
  struct MenuIcon *icon;
  s32 window;
  s32 changed;
  s32 moved;
  s8 listMode;
  s32 member;
  s32 ownerCount;
  u8 i;
  struct BattleUnit *owner;
  s32 result;
  s32 redraw;
  s32 done;
  s32 previous;
  s32 shortcutHelp;
  s32 work[5];
  struct MenuResult state;

  menu = Ram_HeapSlots->menuRuntime;
  result = 0;
  previous = 0;
  shortcutHelp = 0;
  menu->paneIcon[paneIndex]->active = 13;
  UiWindow_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, 2);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && !GameFlag_Test(0x150)) {
    owner = Unit_Get(menu->ownerIds[paneIndex]);
    if (menu->mode != 0)
      menu->psynergyCount = Func_080fd6f0(owner, menu->psynergies, 1);
    else
      menu->psynergyCount = Func_080fd6f0(owner, menu->psynergies, 2);
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
    Func_080fd80c(&state, paneIndex);
    redraw = 1;
    changed = 1;
    menu->paneIcon[paneIndex]->active = 1;

    while (!GameFlag_Test(0x150)) {
      Func_080f8a44(88, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (menu->psynergies[previous] != 0)
          UiIcon_PrepareObject(menu->entryIcons[previous]);
        if (redraw != 0) {
          redraw = 0;
          WaitFrames(1);
          Func_080fd968(window, 0, &state);
        }
        Func_080fd8a0(window, work, &state);
        menu->paneAction[paneIndex] = menu->psynergies[state.selectedIndex];
        menu->cursorIcon->active = 13;
        if (menu->psynergies[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          icon->active = 9;
          icon->animStep = 0;
          icon->sentinel = 250;
        }
        for (i = 0; i < menu->ownerCount; i++)
          Animation_ApplyChildArgumentFar(menu->tabObjects[i], 1);
      }

      WaitFrames(1);
      previous = state.selectedIndex;

      if ((gInput.held & 4) == 0)
        moved = Func_080f8f9c(0, state.entryCount, 5, &state.row, &state.page);
      else
        moved = -1;
      if (moved == 1) {
        redraw = 1;
        changed = 1;
      }
      if (moved == 0)
        changed = 1;
      if (moved == -1)
        changed = 0;

      if (menu->mode == 0) {
        if ((KEYS_PRESSED & 4) && shortcutHelp == 0) {
          ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                                 OWNER_ACTION_ID_MASK);
          if (ability->unk04[2] == 0) {
            Audio_PlayCue(114);
          } else {
            Audio_PlayCue(174);
            shortcutHelp = 1;
            menu->flags |= 2;
            UiWindow_ClearInteriorTilesFar(window, 0, 88, 120, 96);
            UiText_DrawCharacterAtOffsetFar(MsgShortcutHelp, window, 0, 88);
          }
        }
        if ((gInput.held & 4) == 0 && shortcutHelp == 1) {
          shortcutHelp = 0;
          menu->flags &= ~2;
          UiWindow_ClearInteriorTilesFar(window, 0, 88, 120, 96);
          UiText_DrawCharacterAtOffsetFar(MsgChangeCharacterHelp, window, 0,
                                          88);
        }
      }

      if (KEYS_PRESSED & 1) {
        if (menu->mode != 0) {
          Audio_PlayCue(130);
          result = menu->psynergies[state.selectedIndex];
          done = 1;
          break;
        }
        if (menu->psynergies[state.selectedIndex] != 0) {
          if (PsynergyMenu_IsActionRestricted(
                  menu->psynergies[state.selectedIndex]) != 0) {
            Audio_PlayCue(114);
            continue;
          }
          ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                                 OWNER_ACTION_ID_MASK);
          if (ability->ppCost > owner->pp) {
            Audio_PlayCue(114);
          } else {
            Audio_PlayCue(173);
            result = menu->psynergies[state.selectedIndex];
            done = 1;
            break;
          }
        }
      }

      if (KEYS_PRESSED & 2) {
        Audio_PlayCue(113);
        result = -1;
        done = 1;
        break;
      }

      if (((gInput.repeat & 0x100) || (gInput.repeat & 0x200)) &&
          (gInput.held & 4) == 0) {
        member = menu->tabIndex[paneIndex];
        ownerCount = menu->ownerCount;
        listMode = menu->mode != 0 ? 1 : 2;
        Audio_PlayCue(111);
        menu->selectedIndexByOwner[menu->ownerIds[paneIndex]] =
            state.selectedIndex;
        do {
          if (gInput.repeat & 0x100)
            member++;
          else
            member--;
          member = (member + ownerCount) % ownerCount;
          menu->selectedOwner = menu->ownerTable[member];
          menu->ownerIds[0] = menu->ownerTable[member];
          menu->psynergyCount = Func_080fd6f0(Unit_Get(menu->ownerIds[0]),
                                              menu->psynergies, listMode);
        } while (menu->psynergyCount == 0);
        menu->tabIndex[paneIndex] = member;
        Func_08104ef8(member / 4 * 4);
        Func_08104d5c(menu->rowWindow, member, ownerCount);
        menu->arrows[0]->active = 13;
        menu->arrows[1]->active = 13;
        WaitFrames(1);
        Func_080f8170(menu->statusWindow, menu->ownerTable[member], 0, 0);
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerTable[member]);
        break;
      }

      if ((KEYS_PRESSED & 0x200) && (gInput.held & 4)) {
        ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                               OWNER_ACTION_ID_MASK);
        if (ability->unk04[2] == 0) {
          Audio_PlayCue(114);
        } else if (Party_SetPackedEvent(menu->ownerIds[paneIndex],
                                        menu->psynergies[state.selectedIndex],
                                        0) != 0) {
          Audio_PlayCue(130);
          result = menu->psynergies[state.selectedIndex];
          menu->mode = 1;
          done = 1;
          break;
        }
      }

      if ((KEYS_PRESSED & 0x100) && (gInput.held & 4)) {
        ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                               OWNER_ACTION_ID_MASK);
        if (ability->unk04[2] == 0) {
          Audio_PlayCue(114);
        } else if (Party_SetPackedEvent(menu->ownerIds[paneIndex],
                                        menu->psynergies[state.selectedIndex],
                                        1) != 0) {
          Audio_PlayCue(130);
          result = menu->psynergies[state.selectedIndex];
          menu->mode = 2;
          done = 1;
          break;
        }
      }
    }
  }

  menu->flags &= ~2;
  UiIcon_PrepareObject(menu->entryGridCursor);
  menu->paneRow[paneIndex] = state.selectedIndex;
  menu->selectedIndexByOwner[menu->ownerIds[paneIndex]] = state.selectedIndex;
  menu->paneAction[paneIndex] = result;
  if (GameFlag_Test(0x150) != 0)
    result = -1;
  WaitFrames(1);
  return result;
}

// Matches: differ 0 of 239 (tla-en, 2026-11-07).
// History: 14, 8, then 4 with a one-pass block; prototypes for the far calls
// reach 0; named messages, the shared MenuResult and named values keep 0.
// The psynergy menu's description page (the selected ability's description
// and where it can be used, then each row's element icon and highlight), ☀️'s
// PsynergyMenu_DrawRangePage in games/tbs/src/menu/menu/status.c, with ⚓️'s
// work-block offsets (info window 0x30, list 0x1c4) and its use test on the
// action's byte 6.
#include "types.h"
#include "battle_type.h"
#include "battle_system.h"
#include "menu_result.h"
#include "ram_buffer.h"

// The psynergy menu work block, as this function reads it.
struct PsynergyListWork {
  u8 unk000[0x30];
  void *infoWindow;
  u8 unk034[0x190];
  u16 psynergies[32];
};

#define ACTION_ID_MASK OWNER_ACTION_ID_MASK
#define PAGE_ROWS 5

extern u8 MsgAbilityDescription[];
extern u8 MsgUsableInBattle[];
extern u8 MsgUsableInField[];
// In the English release its text is MsgUsableInField's again.
extern u8 MsgUsableAnywhere[];

void WindowSprite_RedrawSavedRectFar(void *);
void Text_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
void Window_ClearInteriorTilesFar(s32, s32, s32, s32, s32);
void Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);
void Window_SetTilemapEntryFar(s32, s32, s32, s32, s32);

s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused,
                               struct MenuResult *state) {
  struct PsynergyListWork *menu;
  struct BattleAct *ability;
  s32 row;
  s32 base;

  menu = (struct PsynergyListWork *)Ram_HeapSlots->menuRuntime;
  state->selectedIndex = state->page * PAGE_ROWS + state->row;
  WindowSprite_RedrawSavedRectFar(menu->infoWindow);
  wait(1);
  if (menu->psynergies[state->selectedIndex] != 0) {
    Text_DrawCharacterAtOffsetFar(
        (menu->psynergies[state->selectedIndex] & ACTION_ID_MASK) +
            (s32)MsgAbilityDescription,
        (s32)menu->infoWindow, 0, 0);
    ability = BattleAction_Get(menu->psynergies[state->selectedIndex] &
                               ACTION_ID_MASK);
    Window_ClearInteriorTilesFar(window, 0, 96, 224, 104);
    row = 0;
    if (ability->unk04[2] != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      row = 2;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      row |= 1;
    }
    if (row == 3) {
      Text_DrawCharacterAtOffsetFar((s32)MsgUsableAnywhere, window, 0, 96);
    } else if (row == 2) {
      Text_DrawCharacterAtOffsetFar((s32)MsgUsableInField, window, 0, 96);
    } else if (row == 1) {
      Text_DrawCharacterAtOffsetFar((s32)MsgUsableInBattle, window, 0, 96);
    }
  }

  base = state->page * PAGE_ROWS;
  for (row = 0; row <= PAGE_ROWS - 1; row++) {
    if (row == state->row) {
      ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24, row * 2 + 2,
                                  0);
        Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 15, 1, 14);
        Render_SetTilemapFlagRect(window, 25, row * 2 + 2, 3, 1, 14);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 19, 1, 14);
      }
    } else {
      ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24, row * 2 + 2,
                                  4);
        Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 15, 1, 15);
        Render_SetTilemapFlagRect(window, 25, row * 2 + 2, 3, 1, 15);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 19, 1, 15);
      }
    }
  }
  wait(1);
  return 1;
}

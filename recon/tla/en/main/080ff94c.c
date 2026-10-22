// Near miss, differ 14 of 239: the psynergy menu's description page (the
// selected ability's description and where it can be used, then each row's
// element icon and highlight), ☀️'s PsynergyMenu_DrawRangePage in
// games/tbs/src/menu/menu.c, with ⚓️'s work-block offsets (info window 0x30,
// list 0x1c4) and its use test on the action's byte 6. Instruction order
// only: the selected index's store, the 0x3fff mask's first moves and the
// window argument's move each sit one or two slots from the listing's.
#include "types.h"
#include "battle_type.h"
#include "ram_buffer.h"

struct MenuResult {
  s32 ownerState;
  s32 unk04;
  s32 page;
  s32 pageCount;
  s32 row;
  s32 entryCount;
  s32 selectedIndex;
};

// The psynergy menu work block, as this function reads it.
struct PsynergyListWork {
  u8 unk000[0x30];
  s32 infoWindow;
  u8 unk034[0x190];
  u16 psynergies[32];
};

#define ACTION_ID_MASK OWNER_ACTION_ID_MASK

extern u8 Msg02181[]; // the first ability description (MsgAbilityDescription)
extern u8 Msg04162[]; // "Can be used in Battle" (MsgUsableInBattle)
extern u8 Msg04163[]; // "Can be used in Towns" (MsgUsableInField)
extern u8 Msg04164[]; // the same text again (MsgUsableAnywhere)

struct BattleAct *BattleAction_Get(s32 action);
void UiWindow_Commit(s32 window);

void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);

s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused,
                               struct MenuResult *state) {
  struct PsynergyListWork *menu;
  struct BattleAct *ability;
  s32 row;
  s32 base;

  menu = (struct PsynergyListWork *)Ram_HeapSlots->menuRuntime;
  state->selectedIndex = state->page * 5 + state->row;
  RenderOutput_RedrawSavedRectFar((s32)menu->infoWindow);
  WaitFrames(1);
  if (menu->psynergies[state->selectedIndex] != 0) {
    UiText_DrawCharacterAtOffsetFar(
        (menu->psynergies[state->selectedIndex] & ACTION_ID_MASK) +
            (s32)&Msg02181,
        (s32)menu->infoWindow, 0, 0);
    ability =
        BattleAction_Get(menu->psynergies[state->selectedIndex] & ACTION_ID_MASK);
    UiWindow_ClearInteriorTilesFar(window, 0, 96, 224, 104);
    row = 0;
    if (ability->unk04[2] != 0 || (ability->flags & 0x40) != 0) {
      row = 2;
    }
    if ((ability->flags & 0x80) != 0) {
      row |= 1;
    }
    if (row == 3) {
      UiText_DrawCharacterAtOffsetFar((s32)&Msg04164, window, 0, 96);
    } else if (row == 2) {
      UiText_DrawCharacterAtOffsetFar((s32)&Msg04163, window, 0, 96);
    } else if (row == 1) {
      UiText_DrawCharacterAtOffsetFar((s32)&Msg04162, window, 0, 96);
    }
  }

  base = state->page * 5;
  for (row = 0; row <= 4; row++) {
    if (row == state->row) {
      ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
      if (ability->element != 4) {
        UiWindow_SetTilemapEntryFar(window, ability->element + 1, 24,
                                    row * 2 + 2, 0);
        Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 15, 1, 14);
        Render_SetTilemapFlagRect(window, 25, row * 2 + 2, 3, 1, 14);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 19, 1, 14);
      }
    } else {
      ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
      if (ability->element != 4) {
        UiWindow_SetTilemapEntryFar(window, ability->element + 1, 24,
                                    row * 2 + 2, 4);
        Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 15, 1, 15);
        Render_SetTilemapFlagRect(window, 25, row * 2 + 2, 3, 1, 15);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 19, 1, 15);
      }
    }
  }
  WaitFrames(1);
  return 1;
}

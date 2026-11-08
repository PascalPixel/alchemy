#include "types.h"
#include "battle_type.h"
#include "battle_system.h"
#include "menu_result.h"
#include "ram_buffer.h"
#include "edition.h"
#include "version.h"

// The psynergy menu work block, as this function reads it.
struct PsynergyListWork {
  u8 unk000[0x30];
  void *infoWindow;
  u8 unk034[0x190];
  u16 psynergies[32];
};

#define ACTION_ID_MASK OWNER_ACTION_ID_MASK

// The Japanese page lists six abilities rather than five, a row higher, and
// says where the chosen ability can be used in short lines stacked at the
// window's left edge rather than in one sentence across it. FIRST_TILE_ROW is
// the tile row of the page's first ability; USABLE_TOP, USABLE_RIGHT and
// USABLE_BOTTOM bound that text in pixels inside the window.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#define FIRST_TILE_ROW 1
#define USABLE_TOP 72
#define USABLE_RIGHT 64
#define USABLE_BOTTOM 96
// まちやダンジョン ("towns and dungeons"), the message two after MsgExpLabel.
#define MSG_USABLE_IN_FIELD ((s32)MsgExpLabel + 2)
// せんとう ("battle"), the message after MsgExpLabel.
#define MSG_USABLE_IN_BATTLE ((s32)MsgExpLabel + 1)
// でつかえます ("can be used"), the message before MsgDjinnListHelp.
#define MSG_CAN_BE_USED ((s32)MsgDjinnListHelp - 1)
#else
#define PAGE_ROWS 5
#define FIRST_TILE_ROW 2
#define USABLE_TOP 96
#define USABLE_RIGHT 224
#define USABLE_BOTTOM 104
#endif

extern u8 MsgAbilityDescription[];
#if LANGUAGE == LANGUAGE_JAPANESE
extern u8 MsgExpLabel[], MsgDjinnListHelp[];
#else
extern u8 MsgUsableInBattle[];
extern u8 MsgUsableInField[];
// In the English release its text is MsgUsableInField's again.
extern u8 MsgUsableAnywhere[];
#endif

void WindowSprite_RedrawSavedRectFar(void *);
void WindowSprite_ClearListFar(void *);
void Text_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
void Text_DrawMessageFar(s32, s32, s32, s32);
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
  // The Japanese release clears the info window; from patch 1 it redraws the
  // window's saved rectangle.
#if TLA_PATCH_1
  WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#else
  WindowSprite_ClearListFar(menu->infoWindow);
#endif
  wait(1);
  if (menu->psynergies[state->selectedIndex] != 0) {
    // Japanese draws the description as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
    Text_DrawMessageFar(
        (menu->psynergies[state->selectedIndex] & ACTION_ID_MASK) +
            (s32)MsgAbilityDescription,
        (s32)menu->infoWindow, 0, 0);
#else
    Text_DrawCharacterAtOffsetFar(
        (menu->psynergies[state->selectedIndex] & ACTION_ID_MASK) +
            (s32)MsgAbilityDescription,
        (s32)menu->infoWindow, 0, 0);
#endif
    ability = BattleAction_Get(menu->psynergies[state->selectedIndex] &
                               ACTION_ID_MASK);
    Window_ClearInteriorTilesFar(window, 0, USABLE_TOP, USABLE_RIGHT,
                                 USABLE_BOTTOM);
    row = 0;
#if LANGUAGE == LANGUAGE_JAPANESE
    // One line for each place the ability works, then the end of the
    // sentence on the line below them.
    if (ability->unk04[2] != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      Text_DrawCharacterAtOffsetFar(MSG_USABLE_IN_FIELD, window, 0, USABLE_TOP);
      row = 1;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      Text_DrawCharacterAtOffsetFar(MSG_USABLE_IN_BATTLE, window, 0,
                                    row * 8 + USABLE_TOP);
      row++;
    }
    Text_DrawCharacterAtOffsetFar(MSG_CAN_BE_USED, window, 0,
                                  row * 8 + USABLE_TOP);
#else
    if (ability->unk04[2] != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      row = 2;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      row |= 1;
    }
    if (row == 3) {
      Text_DrawCharacterAtOffsetFar((s32)MsgUsableAnywhere, window, 0,
                                    USABLE_TOP);
    } else if (row == 2) {
      Text_DrawCharacterAtOffsetFar((s32)MsgUsableInField, window, 0,
                                    USABLE_TOP);
    } else if (row == 1) {
      Text_DrawCharacterAtOffsetFar((s32)MsgUsableInBattle, window, 0,
                                    USABLE_TOP);
    }
#endif
  }

  base = state->page * PAGE_ROWS;
  for (row = 0; row <= PAGE_ROWS - 1; row++) {
    if (row == state->row) {
      ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24,
                                  row * 2 + FIRST_TILE_ROW, 0);
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 15, 1,
                                  14);
        Render_SetTilemapFlagRect(window, 25, row * 2 + FIRST_TILE_ROW, 3, 1,
                                  14);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 19, 1,
                                  14);
      }
    } else {
      ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24,
                                  row * 2 + FIRST_TILE_ROW, 4);
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 15, 1,
                                  15);
        Render_SetTilemapFlagRect(window, 25, row * 2 + FIRST_TILE_ROW, 3, 1,
                                  15);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 19, 1,
                                  15);
      }
    }
  }
  wait(1);
  return 1;
}

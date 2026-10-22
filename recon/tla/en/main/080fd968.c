// Near miss, differ 10 of 158: draws one page of the psynergy list (name, PP
// cost, greyed when the PP is short or the action restricted), ☀️'s
// PsynergyMenu_DrawActionPage in games/tbs/src/menu/menu.c, with ⚓️'s
// work-block offsets (flags 0x21c, entries 0x1c4, owner byte 0x216). Instruction
// order only: the entry's halfword load and the 0x3fff mask's first move swap
// at each of three reads, and one move of the row moves past the mask.
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
struct PsynergyMenuState {
  u8 unk000[0x216];
  u8 ownerIds[2];
};

void RenderOutput_RedrawSavedRectFar(s32 window);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 width, s32 height,
                                 s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Menu_SetPageIcons(s32 page_size, s32 first_entry, s32 window, s32 x,
                       s32 y);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 page_size, s32 page,
                            s32 style);
s32 UiWork_SetParamNibbleFar(s32 color);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 layer, s32 x,
                                  s32 y);
struct BattleUnit *Unit_Get(s32 owner);
struct BattleAct *BattleAction_Get(s32 action);
extern u8 MsgAbilityName;
extern u8 MsgShortcutHelp[];
extern u8 MsgChangeCharacterHelp[];
extern u8 MsgPsynergyPp[];

s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused,
                                const struct MenuResult *state) {
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  s32 cursor;
  struct BattleUnit *owner;
  struct BattleAct *ability;
  struct PsynergyMenuState *menu = Ram_HeapSlots->menuRuntime;

  (void)unused;

  RenderOutput_RedrawSavedRectFar(window);
  UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

  if (2 & *(u16 *)((u8 *)menu + 0x21c)) {
    UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutHelp, window, 0, 88);
  } else {
    UiText_DrawCharacterAtOffsetFar((s32)MsgChangeCharacterHelp, window, 0, 88);
  }

  firstEntry = state->page * 5;
  visibleCount = (u8)(state->entryCount - firstEntry);
  if (visibleCount > 5) {
    visibleCount = 5;
  }

  Menu_SetPageIcons(5, firstEntry, window, 0x70, 0x22);
  Menu_DrawPageIndicator(window, state->entryCount, 5, state->page, 15);
  UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, 0x60, 0);

  row = 0;
  if (visibleCount > row) {
    cursor = firstEntry * 2 + 0x1c4;
    do {
      owner = Unit_Get(menu->ownerIds[0]);
      ability = BattleAction_Get(0x3fff & *(const u16 *)(cursor + (s32)menu));

      if (ability->ppCost > owner->pp) {
        UiWork_SetParamNibbleFar(2);
      } else if (PsynergyMenu_IsActionRestricted(
                     0x3fff & *(const u16 *)(cursor + (s32)menu)) != 0) {
        UiWork_SetParamNibbleFar(4);
      } else {
        UiWork_SetParamNibbleFar(15);
      }

      UiText_DrawCharacterAtOffsetFar(
          (0x3fff & *(const u16 *)(cursor + (s32)menu)) + (s32)&MsgAbilityName,
          window, 16, row * 16 + 8);
      UiText_DrawNumberAtOffsetFar(ability->ppCost, 2, window, 104,
                                   row * 16 + 8);
      UiWork_SetParamNibbleFar(15);

      row++;
      cursor += 2;
    } while (visibleCount > row);
  }

  return 1;
}

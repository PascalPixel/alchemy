#include "types.h"
#include "battle_type.h"
#include "ram_buffer.h"
#include "edition.h"

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
  u8 unk000[0x1c4];
  u16 psynergies[40];
  u8 psynergyCount;
  u8 unk215[1];
  u8 ownerIds[2];
};

#define PSY_LIST_OFFSET 0x1c4
#define ACT_ID_MASK OWNER_ACTION_ID_MASK
#define OWNER_CLASS_MSG_OFFSET 0x129
#define OWNER_LEVEL_OFFSET 15

// The Japanese page lists six abilities rather than five, a row higher, and
// lays its heading, names, costs, page icons and "no Psynergy" message out on
// its own columns.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#define PAGE_ICONS_X 86
#define PAGE_ICONS_Y 53
#define HEADING_X 144
#define FIRST_ROW_Y 8
#define NAME_X 96
#define COST_X 168
#define FIRST_RANGE_ROW 1
#define EMPTY_MESSAGE_Y 8
#else
#define PAGE_ROWS 5
#define PAGE_ICONS_X 80
#define PAGE_ICONS_Y 58
#define HEADING_X 176
#define FIRST_ROW_Y 16
#define NAME_X 88
#define COST_X 176
#define FIRST_RANGE_ROW 2
#define EMPTY_MESSAGE_Y 17
#endif

extern u8 MsgAbilityNothing[];
extern u8 MsgClassName[];
extern u8 MsgPsynergyChooseTarget[];
extern u8 MsgPsynergyChooseForR[];
extern const u8 gMenu_LvString[];

u8 *Unit_Get(s32 owner);
void RenderOutput_RedrawSavedRectFar(s32 window);
struct BattleAct *BattleAction_Get(s32 action);
void Menu_SetPageIcons(s32 pageSize, s32 first, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 pageSize, s32 page,
                            s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 window, s32 x,
                                  s32 y);
void PsynergyMenu_DrawRange(s32 window, s32 x, s32 y, s32 range, s32 style);
void UiText_DrawStringAtOffsetFar(const u8 *text, s32 window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(const u8 *text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x,
                                  s32 y);

/// The Psynergy menu's list page: each ability's name, PP cost and range, "no
/// Psynergy" when empty, then the owner's name, class and level.
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused,
                              const struct MenuResult *res) {
  struct PsynergyMenuState *menu = Ram_HeapSlots->menuRuntime;
  u8 *owner;
  u32 first;
  u32 rows;
  u8 row;
  s32 offset;

  (void)unused;

  owner = Unit_Get(menu->ownerIds[0]);

  RenderOutput_RedrawSavedRectFar(window);

  first = res->page * PAGE_ROWS;
  rows = (u8)(res->entryCount - first);
  if (rows > PAGE_ROWS) {
    rows = PAGE_ROWS;
  }

  Menu_SetPageIcons(PAGE_ROWS, first, window, PAGE_ICONS_X, PAGE_ICONS_Y);
  Menu_DrawPageIndicator(window, res->entryCount, PAGE_ROWS, res->page, 28);

  UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyChooseTarget + 2, window,
                                  HEADING_X, 0);

  row = 0;
  if (rows > row) {
    offset = (s32)(first * 2) + PSY_LIST_OFFSET;
    do {
      struct BattleAct *act;
      s32 msg;
      s32 y;
      s32 range;
      act = BattleAction_Get(ACT_ID_MASK & *(u16 *)(offset + (s32)menu));
      msg =
          (*(u16 *)(offset + (s32)menu) & ACT_ID_MASK) + (s32)MsgAbilityNothing;
      y = row * 16 + FIRST_ROW_Y;

      UiText_DrawCharacterAtOffsetFar(msg, window, NAME_X, y);
      UiText_DrawNumberAtOffsetFar(act->ppCost, 2, window, COST_X, y);

      range = act->range;
      if (range == 0xff) {
        range = 11;
      } else {
        range--;
      }
      PsynergyMenu_DrawRange(window, 25, row * 2 + FIRST_RANGE_ROW, range, 0);

      row++;
      offset += 2;
    } while (rows > row);
  }

  if (menu->psynergyCount == 0) {
    UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyChooseForR - 1, window, 96,
                                    EMPTY_MESSAGE_Y);
  }

  UiText_DrawStringAtOffsetFar(owner, window, 40, 0);
  UiText_DrawCharacterAtOffsetFar(
      owner[OWNER_CLASS_MSG_OFFSET] + (s32)MsgClassName, window, 0, 32);
  // Spanish and French draw the level label at an offset, as the owner's name.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
  UiText_DrawStringAtOffsetFar(gMenu_LvString, window, 0, 48);
#else
  UiText_DrawStringInWindowFar(gMenu_LvString, window, 0, 48);
#endif
  UiText_DrawNumberInWindowFar(owner[OWNER_LEVEL_OFFSET], 2, window, 24, 48);

  return 1;
}

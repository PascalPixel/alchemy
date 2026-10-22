// Near miss, differ 10 of 164: the psynergy menu's list page (each ability's
// name, PP cost and range, "no Psynergy" when empty, then the owner's name,
// class and level), ☀️'s PsynergyMenu_DrawListPage in
// games/tbs/src/menu/menu.c, rewritten with ⚓️'s calls (Unit_Get,
// RenderOutput_RedrawSavedRectFar, BattleAction_Get, the far text routines), its
// work-block offsets (list 0x1c4, count 0x214, owner 0x216) and its numbered
// messages. Instruction order only: the entry's halfword load against the
// 0x3fff mask's first move, the row's move, and the class offset's move.
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

extern u8 Msg01447[]; // the first ability name (MsgAbilityName)
extern u8 Msg02915[]; // the first class name (MsgClassName)
extern u8 Msg04124[]; // "PP" (MsgPsynergyPp)
extern u8 Msg04126[]; // "You have no Psynergy." (MsgNoPsynergy)
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
void UiText_DrawStringAtOffsetFar(u8 *text, s32 window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(const u8 *text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x,
                                  s32 y);

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

  first = res->page * 5;
  rows = (u8)(res->entryCount - first);
  if (rows > 5) {
    rows = 5;
  }

  Menu_SetPageIcons(5, first, window, 80, 58);
  Menu_DrawPageIndicator(window, res->entryCount, 5, res->page, 28);

  UiText_DrawCharacterAtOffsetFar((s32)&Msg04124, window, 176, 0);

  row = 0;
  if (rows > row) {
    offset = (s32)(first * 2) + PSY_LIST_OFFSET;
    do {
      struct BattleAct *act;
      s32 msg;
      s32 y;
      s32 range;
      act = BattleAction_Get(ACT_ID_MASK & *(u16 *)(offset + (s32)menu));
      msg = (*(u16 *)(offset + (s32)menu) & ACT_ID_MASK) + (s32)&Msg01447;
      y = row * 16 + 16;

      UiText_DrawCharacterAtOffsetFar(msg, window, 88, y);
      UiText_DrawNumberAtOffsetFar(act->ppCost, 2, window, 176, y);

      range = act->range;
      if (range == 0xff) {
        range = 11;
      } else {
        range--;
      }
      PsynergyMenu_DrawRange(window, 25, row * 2 + 2, range, 0);

      row++;
      offset += 2;
    } while (rows > row);
  }

  if (menu->psynergyCount == 0) {
    UiText_DrawCharacterAtOffsetFar((s32)&Msg04126, window, 96, 17);
  }

  UiText_DrawStringAtOffsetFar(owner, window, 40, 0);
  UiText_DrawCharacterAtOffsetFar(
      owner[OWNER_CLASS_MSG_OFFSET] + (s32)&Msg02915, window, 0, 32);
  UiText_DrawStringInWindowFar(gMenu_LvString, window, 0, 48);
  UiText_DrawNumberInWindowFar(owner[OWNER_LEVEL_OFFSET], 2, window, 24, 48);

  return 1;
}
